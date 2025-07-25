#include "pbrt_proto/shared/parser.h"

#include <cassert>
#include <functional>
#include <iostream>
#include <istream>
#include <memory>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "absl/container/inlined_vector.h"
#include "absl/status/status.h"
#include "absl/strings/numbers.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/shared/tokenizer.h"

namespace pbrt_proto {
namespace {

class ParameterStorage {
 public:
  absl::string_view Add(absl::string_view to_add);

  absl::InlinedVector<std::array<double, 3>, 16>& NextFloat3();
  absl::InlinedVector<std::array<double, 2>, 16>& NextFloat2();
  absl::InlinedVector<double, 16>& NextFloat();
  absl::InlinedVector<int, 16>& NextInt();
  absl::InlinedVector<bool, 16>& NextBool();
  absl::InlinedVector<absl::string_view, 16>& NextString();

  void Clear();

 private:
  std::vector<std::vector<char>> chars_;
  size_t chars_size_ = 0;

  std::vector<std::unique_ptr<absl::InlinedVector<std::array<double, 3>, 16>>>
      float3_;
  size_t float3_size_ = 0;

  std::vector<std::unique_ptr<absl::InlinedVector<std::array<double, 2>, 16>>>
      float2_;
  size_t float2_size_ = 0;

  std::vector<std::unique_ptr<absl::InlinedVector<double, 16>>> float_;
  size_t float_size_ = 0;

  std::vector<std::unique_ptr<absl::InlinedVector<int32_t, 16>>> int_;
  size_t int_size_ = 0;

  std::vector<std::unique_ptr<absl::InlinedVector<absl::string_view, 16>>>
      strings_;
  size_t string_size_ = 0;

  std::vector<std::unique_ptr<absl::InlinedVector<bool, 16>>> bool_;
  size_t bool_size_ = 0;
};

absl::string_view ParameterStorage::Add(absl::string_view to_add) {
  if (chars_size_ == chars_.size()) {
    chars_.emplace_back();
  }

  auto& result = chars_[chars_size_++];
  result.clear();

  for (char c : to_add) {
    result.emplace_back(c);
  }

  return absl::string_view(result.data(), result.size());
}

absl::InlinedVector<std::array<double, 3>, 16>& ParameterStorage::NextFloat3() {
  if (float3_size_ == float3_.size()) {
    float3_.emplace_back(
        std::make_unique<absl::InlinedVector<std::array<double, 3>, 16>>());
  }

  auto& result = float3_[float3_size_++];
  result->clear();

  return *result;
}

absl::InlinedVector<std::array<double, 2>, 16>& ParameterStorage::NextFloat2() {
  if (float2_size_ == float2_.size()) {
    float2_.emplace_back(
        std::make_unique<absl::InlinedVector<std::array<double, 2>, 16>>());
  }

  auto& result = float2_[float2_size_++];
  result->clear();

  return *result;
}

absl::InlinedVector<double, 16>& ParameterStorage::NextFloat() {
  if (float_size_ == float_.size()) {
    float_.emplace_back(std::make_unique<absl::InlinedVector<double, 16>>());
  }

  auto& result = float_[float_size_++];
  result->clear();

  return *result;
}

absl::InlinedVector<int, 16>& ParameterStorage::NextInt() {
  if (int_size_ == int_.size()) {
    int_.emplace_back(std::make_unique<absl::InlinedVector<int32_t, 16>>());
  }

  auto& result = int_[int_size_++];
  result->clear();

  return *result;
}

absl::InlinedVector<bool, 16>& ParameterStorage::NextBool() {
  if (bool_size_ == bool_.size()) {
    bool_.emplace_back(std::make_unique<absl::InlinedVector<bool, 16>>());
  }

  auto& result = bool_[bool_size_++];
  result->clear();

  return *result;
}

absl::InlinedVector<absl::string_view, 16>& ParameterStorage::NextString() {
  if (string_size_ == strings_.size()) {
    strings_.emplace_back(
        std::make_unique<absl::InlinedVector<absl::string_view, 16>>());
  }

  auto& result = strings_[string_size_++];
  result->clear();

  return *result;
}

void ParameterStorage::Clear() {
  chars_size_ = 0;
  float3_size_ = 0;
  float2_size_ = 0;
  float_size_ = 0;
  int_size_ = 0;
  bool_size_ = 0;
  string_size_ = 0;
}

absl::Status MissingValueError(absl::string_view directive,
                               absl::string_view type, absl::string_view name) {
  return absl::InvalidArgumentError(absl::StrCat(
      "Missing value for ", directive, " ", type, " parameter: '", name, "'"));
}

absl::Status InvalidValueCountError(absl::string_view directive,
                                    absl::string_view type,
                                    absl::string_view name) {
  return absl::InvalidArgumentError(
      absl::StrCat("Invalid number of values for ", directive, " ", type,
                   " parameter: '", name, "'"));
}

absl::Status UnterminatedArrayError(absl::string_view directive,
                                    absl::string_view type,
                                    absl::string_view name) {
  return absl::InvalidArgumentError(
      absl::StrCat("Unterminated array in directive ", directive, " for ", type,
                   " parameter: '", name, "'"));
}

absl::Status InvalidTokenError(absl::string_view directive,
                               absl::string_view type, absl::string_view name,
                               absl::string_view token) {
  absl::string_view wrapper = (token[0] == '"') ? "" : "'";
  return absl::InvalidArgumentError(
      absl::StrCat("Failed to parse ", type, " value for ", directive,
                   " parameter ", name, ": ", wrapper, token, wrapper));
}

absl::Status InvalidParameterCount(absl::string_view directive,
                                   size_t num_parameters, bool is_array) {
  absl::string_view array_modifier = (is_array) ? "an array of " : "";
  return absl::InvalidArgumentError(
      absl::StrCat("Directive ", directive, " requires ", array_modifier,
                   "exactly ", num_parameters, " parameters"));
}

absl::Status CheckForNextValue(Tokenizer& tokenizer,
                               absl::string_view directive,
                               absl::string_view type, absl::string_view name) {
  absl::StatusOr<const std::string*> next = tokenizer.Peek();
  if (!next.ok()) {
    return next.status();
  }

  if (!*next) {
    return MissingValueError(directive, type, name);
  }

  return absl::OkStatus();
}

absl::Status ParseValue(absl::string_view directive, absl::string_view type,
                        absl::string_view name, ParameterStorage& storage,
                        Tokenizer& tokenizer, double& out) {
  assert(CheckForNextValue(tokenizer, directive, type, name).ok());

  if (const std::string* next = *tokenizer.Next();
      !absl::SimpleAtod(*next, &out)) {
    return InvalidTokenError(directive, type, name, *next);
  }

  return absl::OkStatus();
}

absl::Status ParseValue(absl::string_view directive, absl::string_view type,
                        absl::string_view name, ParameterStorage& storage,
                        Tokenizer& tokenizer, std::array<double, 2>& out) {
  assert(CheckForNextValue(tokenizer, directive, type, name).ok());

  if (absl::Status status =
          ParseValue(directive, type, name, storage, tokenizer, out[0]);
      !status.ok()) {
    return status;
  }

  if (absl::Status status = CheckForNextValue(tokenizer, directive, type, name);
      !status.ok()) {
    return status;
  }

  if (**tokenizer.Peek() == "]") {
    // TODO: Make this compatible with all PBRT versions
    return MissingValueError(directive, type, name);
  }

  if (absl::Status status =
          ParseValue(directive, type, name, storage, tokenizer, out[1]);
      !status.ok()) {
    return status;
  }

  return absl::OkStatus();
}

absl::Status ParseValue(absl::string_view directive, absl::string_view type,
                        absl::string_view name, ParameterStorage& storage,
                        Tokenizer& tokenizer, std::array<double, 3>& out) {
  assert(CheckForNextValue(tokenizer, directive, type, name).ok());

  if (absl::Status status =
          ParseValue(directive, type, name, storage, tokenizer, out[0]);
      !status.ok()) {
    return status;
  }

  if (absl::Status status = CheckForNextValue(tokenizer, directive, type, name);
      !status.ok()) {
    return status;
  }

  if (**tokenizer.Peek() == "]") {
    // TODO: Make this compatible with all PBRT versions
    return MissingValueError(directive, type, name);
  }

  if (absl::Status status =
          ParseValue(directive, type, name, storage, tokenizer, out[1]);
      !status.ok()) {
    return status;
  }

  if (absl::Status status = CheckForNextValue(tokenizer, directive, type, name);
      !status.ok()) {
    return status;
  }

  if (**tokenizer.Peek() == "]") {
    // TODO: Make this compatible with all PBRT versions
    return MissingValueError(directive, type, name);
  }

  if (absl::Status status =
          ParseValue(directive, type, name, storage, tokenizer, out[2]);
      !status.ok()) {
    return status;
  }

  return absl::OkStatus();
}

absl::Status ParseValue(absl::string_view directive, absl::string_view type,
                        absl::string_view name, ParameterStorage& storage,
                        Tokenizer& tokenizer, int32_t& out) {
  assert(CheckForNextValue(tokenizer, directive, type, name).ok());

  if (const std::string* next = *tokenizer.Next();
      !absl::SimpleAtoi(*next, &out)) {
    return InvalidTokenError(directive, type, name, *next);
  }

  return absl::OkStatus();
}

absl::Status ParseValue(absl::string_view directive, absl::string_view type,
                        absl::string_view name, ParameterStorage& storage,
                        Tokenizer& tokenizer, bool& out) {
  assert(CheckForNextValue(tokenizer, directive, type, name).ok());

  const std::string* next = *tokenizer.Next();

  if (*next == "\"true\"") {
    out = true;
    return absl::OkStatus();
  }

  if (*next == "\"false\"") {
    out = false;
    return absl::OkStatus();
  }

  // TODO: Make this compatible with all PBRT versions
  return InvalidTokenError(directive, type, name, *next);
}

absl::Status ParseValue(absl::string_view directive, absl::string_view type,
                        absl::string_view name, ParameterStorage& storage,
                        Tokenizer& tokenizer, absl::string_view& out) {
  assert(CheckForNextValue(tokenizer, directive, type, name).ok());

  const std::string* next = *tokenizer.Next();

  if ((*next)[0] != '"') {
    return absl::InvalidArgumentError(
        absl::StrCat("Unquoted ", type, " value for ", directive, " parameter ",
                     name, ": '", *next, "'"));
  }

  out = storage.Add(*next);
  out.remove_prefix(1);
  out.remove_suffix(1);

  return absl::OkStatus();
}

template <typename T>
absl::Status ParseParameterListImpl(
    absl::string_view directive, absl::string_view type, absl::string_view name,
    ParameterStorage& storage, Tokenizer& tokenizer,
    absl::InlinedVector<T, 16>& output, bool loop) {
  for (;;) {
    absl::StatusOr<const std::string*> next = tokenizer.Peek();
    if (!next.ok()) {
      return next.status();
    }

    if (!*next) {
      if (!loop) {
        tokenizer.Next().IgnoreError();
        break;
      }

      return UnterminatedArrayError(directive, type, name);
    }

    if (loop && **next == "]") {
      tokenizer.Next().IgnoreError();
      break;
    }

    T value;
    if (absl::Status status =
            ParseValue(directive, type, name, storage, tokenizer, value);
        !status.ok()) {
      return status;
    }

    output.push_back(value);

    if (!loop) {
      break;
    }
  }

  return absl::OkStatus();
}

template <typename T>
absl::Status ParseParameterList(absl::string_view directive,
                                absl::string_view type, absl::string_view name,
                                ParameterStorage& storage, Tokenizer& tokenizer,
                                absl::InlinedVector<T, 16>& output,
                                bool must_loop) {
  absl::StatusOr<const std::string*> next = tokenizer.Peek();
  if (!next.ok()) {
    return next.status();
  }

  if (!*next) {
    return MissingValueError(directive, type, name);
  }

  bool loop;
  if ((**next == "[")) {
    tokenizer.Next().IgnoreError();
    loop = true;
  } else {
    if (must_loop) {
      // TODO: Make this compatible with all PBRT versions
      return MissingValueError(directive, type, name);
    }
    loop = false;
  }

  return ParseParameterListImpl(directive, type, name, storage, tokenizer,
                                output, loop);
}

absl::Status ParseSpectrumParameter(
    absl::string_view directive, absl::string_view type, absl::string_view name,
    ParameterStorage& storage, Tokenizer& tokenizer, ParameterValues& output) {
  absl::StatusOr<const std::string*> next = tokenizer.Peek();
  if (!next.ok()) {
    return next.status();
  }

  if (!*next) {
    return MissingValueError(directive, type, name);
  }

  bool loop;
  if ((**next == "[")) {
    tokenizer.Next().IgnoreError();

    next = tokenizer.Peek();
    if (!next.ok()) {
      return next.status();
    }

    if (!*next) {
      return UnterminatedArrayError(directive, type, name);
    }

    loop = true;
  } else {
    if ((**next)[0] != '"') {
      // TODO: Make this compatible with all PBRT versions
      return absl::InvalidArgumentError(absl::StrCat(
          "Non-array ", type, " value for ", directive, " parameter ", name,
          " was not a string: '", **next, "'"));
    }
    loop = false;
  }

  absl::Status status;
  if ((**next)[0] == '"') {
    auto& output_storage = storage.NextString();
    status = ParseParameterListImpl(directive, type, name, storage, tokenizer,
                                    output_storage, loop);
    output = absl::MakeSpan(output_storage);
  } else {
    auto& output_storage = storage.NextFloat2();
    status = ParseParameterListImpl(directive, type, name, storage, tokenizer,
                                    output_storage, loop);
    output = absl::MakeSpan(output_storage);
  }

  return status;
}

absl::StatusOr<absl::string_view> ReadQuotedString(absl::string_view directive,
                                                   Tokenizer& tokenizer) {
  absl::StatusOr<const std::string*> next = tokenizer.Next();
  if (!next.ok()) {
    return next.status();
  }

  if (!*next) {
    return absl::InvalidArgumentError(
        absl::StrCat("Missing parameter to directive ", directive));
  }

  if ((**next)[0] != '"') {
    return absl::InvalidArgumentError(absl::StrCat(
        "Invalid parameter to directive ", directive, ": '", **next, "'"));
  }

  absl::string_view view(**next);
  view.remove_prefix(1);
  view.remove_suffix(1);

  return view;
}

absl::StatusOr<absl::InlinedVector<double, 16>> ReadFloatParameters(
    absl::string_view directive, ParameterStorage& storage,
    Tokenizer& tokenizer, size_t num_to_read, bool is_array) {
  if (is_array) {
    absl::StatusOr<const std::string*> next = tokenizer.Next();
    if (!next.ok()) {
      return next.status();
    }

    if (!*next) {
      return InvalidParameterCount(directive, num_to_read, is_array);
    }

    if (**next != "[") {
      return InvalidParameterCount(directive, num_to_read, is_array);
    }
  }

  absl::InlinedVector<double, 16> results;
  for (size_t i = 0; i < num_to_read; i++) {
    absl::StatusOr<const std::string*> next = tokenizer.Next();
    if (!next.ok()) {
      return next.status();
    }

    if (!*next || (is_array && **next == "]")) {
      return InvalidParameterCount(directive, num_to_read, is_array);
    }

    double value;
    if (!absl::SimpleAtod(**next, &value)) {
      return absl::InvalidArgumentError(absl::StrCat(
          "Invalid parameter to directive ", directive, ": '", **next, "'"));
    }

    results.push_back(value);
  }

  if (is_array) {
    absl::StatusOr<const std::string*> next = tokenizer.Next();
    if (!next.ok()) {
      return next.status();
    }

    if (!*next) {
      return InvalidParameterCount(directive, num_to_read, is_array);
    }

    if (**next != "]") {
      return InvalidParameterCount(directive, num_to_read, is_array);
    }
  }

  return results;
}

absl::StatusOr<absl::string_view> ReadTypeName(
    absl::string_view directive, ParameterStorage& storage,
    Tokenizer& tokenizer, absl::string_view first_parameter_name) {
  absl::StatusOr<const std::string*> next = tokenizer.Next();
  if (!next.ok()) {
    return next.status();
  }

  if (!*next) {
    return absl::InvalidArgumentError(
        absl::StrCat("Missing ", first_parameter_name,
                     " parameter to directive ", directive));
  }

  if ((**next)[0] != '"') {
    return absl::InvalidArgumentError(absl::StrCat(
        "Unquoted ", first_parameter_name, " parameter to directive ",
        directive, ": '", **next, "'"));
  }

  absl::string_view type_name = **next;
  type_name.remove_prefix(1);
  type_name.remove_suffix(1);

  return storage.Add(type_name);
}

absl::StatusOr<std::optional<
    std::tuple<ParameterType, absl::string_view, absl::string_view>>>
ReadParameterTypeAndName(
    absl::string_view directive,
    const absl::flat_hash_map<absl::string_view, ParameterType>&
        parameter_type_names,
    ParameterStorage& storage, Tokenizer& tokenizer) {
  absl::StatusOr<const std::string*> next = tokenizer.Peek();
  if (!next.ok()) {
    return next.status();
  }

  if (!*next || (**next)[0] != '"') {
    return std::nullopt;
  }

  absl::string_view token = storage.Add(**tokenizer.Next());
  absl::string_view unquoted_token = token;
  unquoted_token.remove_prefix(1);
  unquoted_token.remove_suffix(1);

  std::array<absl::string_view, 3> tokens = absl::StrSplit(
      unquoted_token, absl::ByAsciiWhitespace(), absl::SkipEmpty());
  if (tokens[0].empty() || tokens[1].empty() || !tokens[2].empty()) {
    // TODO: Make this compatible with all PBRT versions
    return absl::InvalidArgumentError(
        absl::StrCat("Invalid parameter declaration for directive ", directive,
                     ": ", token, ""));
  }

  auto type_iter = parameter_type_names.find(tokens[0]);
  if (type_iter == parameter_type_names.end()) {
    return absl::InvalidArgumentError(
        absl::StrCat("Invalid parameter type for directive ", directive, ": '",
                     tokens[0], "'"));
  }

  return std::make_tuple(type_iter->second, tokens[0], tokens[1]);
}

absl::StatusOr<absl::string_view> ReadParameters(
    absl::string_view directive,
    const absl::flat_hash_map<absl::string_view, ParameterType>&
        parameter_type_names,
    ParameterStorage& storage, Tokenizer& tokenizer,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view first_parameter_name = "type") {
  absl::StatusOr<absl::string_view> type_name =
      ReadTypeName(directive, storage, tokenizer, first_parameter_name);
  if (!type_name.ok()) {
    return type_name.status();
  }

  parameters.clear();
  for (;;) {
    absl::StatusOr<std::optional<
        std::tuple<ParameterType, absl::string_view, absl::string_view>>>
        parameter_type_and_name = ReadParameterTypeAndName(
            directive, parameter_type_names, storage, tokenizer);
    if (!parameter_type_and_name.ok()) {
      return parameter_type_and_name.status();
    }

    if (!*parameter_type_and_name) {
      break;
    }

    absl::Status status;
    ParameterValues values;
    switch (std::get<0>(**parameter_type_and_name)) {
      case ParameterType::BLACKBODY_V1: {
        auto& output = storage.NextFloat2();
        status = ParseParameterList(
            directive, std::get<1>(**parameter_type_and_name),
            std::get<2>(**parameter_type_and_name), storage, tokenizer, output,
            /*must_loop=*/true);
        values = absl::MakeSpan(output);
        break;
      }
      case ParameterType::BLACKBODY_V2: {
        auto& output = storage.NextFloat();
        status = ParseParameterList(
            directive, std::get<1>(**parameter_type_and_name),
            std::get<2>(**parameter_type_and_name), storage, tokenizer, output,
            /*must_loop=*/false);
        values = absl::MakeSpan(output);
        break;
      }
      case ParameterType::BOOL: {
        auto& output = storage.NextBool();
        status = ParseParameterList(
            directive, std::get<1>(**parameter_type_and_name),
            std::get<2>(**parameter_type_and_name), storage, tokenizer, output,
            /*must_loop=*/false);
        values = absl::MakeSpan(output);
        break;
      }
      case ParameterType::FLOAT: {
        auto& output = storage.NextFloat();
        status = ParseParameterList(
            directive, std::get<1>(**parameter_type_and_name),
            std::get<2>(**parameter_type_and_name), storage, tokenizer, output,
            /*must_loop=*/false);
        values = absl::MakeSpan(output);
        break;
      }
      case ParameterType::INTEGER: {
        auto& output = storage.NextInt();
        status = ParseParameterList(
            directive, std::get<1>(**parameter_type_and_name),
            std::get<2>(**parameter_type_and_name), storage, tokenizer, output,
            /*must_loop=*/false);
        values = absl::MakeSpan(output);
        break;
      }
      case ParameterType::NORMAL3: {
        auto& output = storage.NextFloat3();
        status = ParseParameterList(
            directive, std::get<1>(**parameter_type_and_name),
            std::get<2>(**parameter_type_and_name), storage, tokenizer, output,
            /*must_loop=*/true);
        values = absl::MakeSpan(output);
        break;
      }
      case ParameterType::POINT2: {
        auto& output = storage.NextFloat2();
        status = ParseParameterList(
            directive, std::get<1>(**parameter_type_and_name),
            std::get<2>(**parameter_type_and_name), storage, tokenizer, output,
            /*must_loop=*/true);
        values = absl::MakeSpan(output);
        break;
      }
      case ParameterType::POINT3: {
        auto& output = storage.NextFloat3();
        status = ParseParameterList(
            directive, std::get<1>(**parameter_type_and_name),
            std::get<2>(**parameter_type_and_name), storage, tokenizer, output,
            /*must_loop=*/true);
        values = absl::MakeSpan(output);
        break;
      }
      case ParameterType::RGB: {
        auto& output = storage.NextFloat3();
        status = ParseParameterList(
            directive, std::get<1>(**parameter_type_and_name),
            std::get<2>(**parameter_type_and_name), storage, tokenizer, output,
            /*must_loop=*/true);
        values = absl::MakeSpan(output);
        break;
      }
      case ParameterType::SPECTRUM: {
        status = ParseSpectrumParameter(
            directive, std::get<1>(**parameter_type_and_name),
            std::get<2>(**parameter_type_and_name), storage, tokenizer, values);
        break;
      }
      case ParameterType::STRING: {
        auto& output = storage.NextString();
        status = ParseParameterList(
            directive, std::get<1>(**parameter_type_and_name),
            std::get<2>(**parameter_type_and_name), storage, tokenizer, output,
            /*must_loop=*/false);
        values = absl::MakeSpan(output);
        break;
      }
      case ParameterType::TEXTURE: {
        auto& output = storage.NextString();
        status = ParseParameterList(
            directive, std::get<1>(**parameter_type_and_name),
            std::get<2>(**parameter_type_and_name), storage, tokenizer, output,
            /*must_loop=*/false);
        values = absl::MakeSpan(output);
        break;
      }
      case ParameterType::VECTOR2: {
        auto& output = storage.NextFloat2();
        status = ParseParameterList(
            directive, std::get<1>(**parameter_type_and_name),
            std::get<2>(**parameter_type_and_name), storage, tokenizer, output,
            /*must_loop=*/true);
        values = absl::MakeSpan(output);
        break;
      }
      case ParameterType::VECTOR3: {
        auto& output = storage.NextFloat3();
        status = ParseParameterList(
            directive, std::get<1>(**parameter_type_and_name),
            std::get<2>(**parameter_type_and_name), storage, tokenizer, output,
            /*must_loop=*/true);
        values = absl::MakeSpan(output);
        break;
      }
      case ParameterType::XYZ: {
        auto& output = storage.NextFloat3();
        status = ParseParameterList(
            directive, std::get<1>(**parameter_type_and_name),
            std::get<2>(**parameter_type_and_name), storage, tokenizer, output,
            /*must_loop=*/true);
        values = absl::MakeSpan(output);
        break;
      }
    }

    if (!status.ok()) {
      return status;
    }

    Parameter& parameter = parameters[std::get<2>(**parameter_type_and_name)];
    parameter.directive = directive;
    parameter.type = std::get<0>(**parameter_type_and_name);
    parameter.type_name = std::get<1>(**parameter_type_and_name);
    parameter.values = std::move(values);
  }

  return *type_name;
}

template <ParameterType type, typename T>
absl::Status TryRemoveValues(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name, std::optional<size_t> required_size,
    std::optional<absl::Span<T>>& value) {
  auto iter = parameters.find(parameter_name);
  if (iter == parameters.end()) {
    return absl::OkStatus();
  }

  if (iter->second.type != type) {
    return absl::OkStatus();
  }

  const absl::Span<T>* values =
      std::get_if<absl::Span<T>>(&iter->second.values);
  if (!values) {
    return absl::OkStatus();
  }

  if (required_size.has_value() && values->size() != *required_size) {
    return InvalidValueCountError(iter->second.directive,
                                  iter->second.type_name, parameter_name);
  }

  value = *values;
  parameters.erase(iter);

  return absl::OkStatus();
}

template <ParameterType type, typename T>
absl::Status TryRemoveValue(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name, std::optional<T>& value) {
  auto iter = parameters.find(parameter_name);
  if (iter == parameters.end()) {
    return absl::OkStatus();
  }

  if (iter->second.type != type) {
    return absl::OkStatus();
  }

  const absl::Span<T>* values =
      std::get_if<absl::Span<T>>(&iter->second.values);
  if (!values) {
    return absl::OkStatus();
  }

  if (values->size() != 1) {
    // This error value is currently not exposed
    return absl::InvalidArgumentError("TODO_Q");
  }

  value = (*values)[0];
  parameters.erase(iter);

  return absl::OkStatus();
}

}  // namespace

absl::Status Parser::ReadFrom(std::istream& stream) {
  Tokenizer tokenizer(stream);

  ParameterStorage storage;
  absl::flat_hash_map<absl::string_view, Parameter> parameters;
  absl::flat_hash_map<absl::string_view, std::function<absl::Status()>>
      directives = {
          {"Accelerator",
           [&]() {
             absl::StatusOr<absl::string_view> type_name =
                 ReadParameters("Accelerator", parameter_type_names_, storage,
                                tokenizer, parameters);
             if (!type_name.ok()) {
               return type_name.status();
             }

             return Accelerator(*type_name, parameters);
           }},
          {"ActiveTransform",
           [&]() {
             absl::StatusOr<const std::string*> next = tokenizer.Next();
             if (!next.ok()) {
               return next.status();
             }

             if (!*next) {
               return absl::InvalidArgumentError(
                   "Missing parameter to directive ActiveTransform");
             }

             ActiveTransformation transformation;
             if (**next == "All") {
               transformation = ActiveTransformation::ALL;
             } else if (**next == "StartTime") {
               transformation = ActiveTransformation::START_TIME;
             } else if (**next == "EndTime") {
               transformation = ActiveTransformation::END_TIME;
             } else {
               return absl::InvalidArgumentError(
                   "Invalid parameter to directive ActiveTransforms: '" +
                   **next + "'");
             }

             return ActiveTransform(transformation);
           }},
          {"AreaLightSource",
           [&]() {
             absl::StatusOr<absl::string_view> type_name =
                 ReadParameters("AreaLightSource", parameter_type_names_,
                                storage, tokenizer, parameters);
             if (!type_name.ok()) {
               return type_name.status();
             }

             return AreaLightSource(*type_name, parameters);
           }},
          {"AttributeBegin", [&]() { return AttributeBegin(); }},
          {"AttributeEnd", [&]() { return AttributeEnd(); }},
          {"Camera",
           [&]() {
             absl::StatusOr<absl::string_view> type_name =
                 ReadParameters("Camera", parameter_type_names_, storage,
                                tokenizer, parameters);
             if (!type_name.ok()) {
               return type_name.status();
             }

             return Camera(*type_name, parameters);
           }},
          {"ConcatTransform",
           [&]() {
             auto values = ReadFloatParameters("ConcatTransform", storage,
                                               tokenizer, 16, true);
             if (!values.ok()) {
               return values.status();
             }

             return ConcatTransform(
                 (*values)[0], (*values)[1], (*values)[2], (*values)[3],
                 (*values)[4], (*values)[5], (*values)[6], (*values)[7],
                 (*values)[8], (*values)[9], (*values)[10], (*values)[11],
                 (*values)[12], (*values)[13], (*values)[14], (*values)[15]);
           }},
          {"CoordinateSystem",
           [&]() {
             auto name = ReadQuotedString("CoordinateSystem", tokenizer);
             if (!name.ok()) {
               return name.status();
             }

             return CoordinateSystem(*name);
           }},
          {"CoordSysTransform",
           [&]() {
             auto name = ReadQuotedString("CoordSysTransform", tokenizer);
             if (!name.ok()) {
               return name.status();
             }

             return CoordSysTransform(*name);
           }},
          {"Film",
           [&]() {
             absl::StatusOr<absl::string_view> type_name = ReadParameters(
                 "Film", parameter_type_names_, storage, tokenizer, parameters);
             if (!type_name.ok()) {
               return type_name.status();
             }

             return Film(*type_name, parameters);
           }},
          {"Identity", [&]() { return Identity(); }},
          {"Include",
           [&]() {
             auto name = ReadQuotedString("Include", tokenizer);
             if (!name.ok()) {
               return name.status();
             }

             return Include(*name);
           }},
          {"Integrator",
           [&]() {
             absl::StatusOr<absl::string_view> type_name =
                 ReadParameters("Integrator", parameter_type_names_, storage,
                                tokenizer, parameters);
             if (!type_name.ok()) {
               return type_name.status();
             }

             return Integrator(*type_name, parameters);
           }},
          {"Import",
           [&]() {
             auto name = ReadQuotedString("Import", tokenizer);
             if (!name.ok()) {
               return name.status();
             }

             return Import(*name);
           }},
          {"LightSource",
           [&]() {
             absl::StatusOr<absl::string_view> type_name =
                 ReadParameters("LightSource", parameter_type_names_, storage,
                                tokenizer, parameters);
             if (!type_name.ok()) {
               return type_name.status();
             }

             return LightSource(*type_name, parameters);
           }},
          {"LookAt",
           [&]() {
             auto values =
                 ReadFloatParameters("LookAt", storage, tokenizer, 9, false);
             if (!values.ok()) {
               return values.status();
             }

             return LookAt((*values)[0], (*values)[1], (*values)[2],
                           (*values)[3], (*values)[4], (*values)[5],
                           (*values)[6], (*values)[7], (*values)[8]);
           }},
          {"MakeNamedMaterial",
           [&]() {
             absl::StatusOr<absl::string_view> material_name =
                 ReadParameters("MakeNamedMaterial", parameter_type_names_,
                                storage, tokenizer, parameters, "name");
             if (!material_name.ok()) {
               return material_name.status();
             }

             return MakeNamedMaterial(*material_name, parameters);
           }},
          {"MakeNamedMedium",
           [&]() {
             absl::StatusOr<absl::string_view> medium_name =
                 ReadParameters("MakeNamedMedium", parameter_type_names_,
                                storage, tokenizer, parameters, "name");
             if (!medium_name.ok()) {
               return medium_name.status();
             }

             return MakeNamedMedium(*medium_name, parameters);
           }},
          {"Material",
           [&]() {
             absl::StatusOr<absl::string_view> type_name =
                 ReadParameters("Material", parameter_type_names_, storage,
                                tokenizer, parameters);
             if (!type_name.ok()) {
               return type_name.status();
             }

             return Material(*type_name, parameters);
           }},
          {"MediumInterface",
           [&]() {
             auto inside = ReadQuotedString("MediumInterface", tokenizer);
             if (!inside.ok()) {
               return inside.status();
             }

             absl::string_view persisted_inside = storage.Add(*inside);

             auto outside = ReadQuotedString("MediumInterface", tokenizer);
             if (!outside.ok()) {
               return outside.status();
             }

             return MediumInterface(persisted_inside, *outside);
           }},
          {"NamedMaterial",
           [&]() {
             auto name = ReadQuotedString("NamedMaterial", tokenizer);
             if (!name.ok()) {
               return name.status();
             }

             return NamedMaterial(*name);
           }},
          {"ObjectBegin",
           [&]() {
             auto name = ReadQuotedString("ObjectBegin", tokenizer);
             if (!name.ok()) {
               return name.status();
             }

             return ObjectBegin(*name);
           }},
          {"ObjectEnd", [&]() { return ObjectEnd(); }},
          {"ObjectInstance",
           [&]() {
             auto name = ReadQuotedString("ObjectInstance", tokenizer);
             if (!name.ok()) {
               return name.status();
             }

             return ObjectInstance(*name);
           }},
          {"PixelFilter",
           [&]() {
             absl::StatusOr<absl::string_view> type_name =
                 ReadParameters("PixelFilter", parameter_type_names_, storage,
                                tokenizer, parameters);
             if (!type_name.ok()) {
               return type_name.status();
             }

             return PixelFilter(*type_name, parameters);
           }},
          {"ReverseOrientation", [&]() { return ReverseOrientation(); }},
          {"Rotate",
           [&]() {
             auto values =
                 ReadFloatParameters("Rotate", storage, tokenizer, 4, false);
             if (!values.ok()) {
               return values.status();
             }

             return Rotate((*values)[0], (*values)[1], (*values)[2],
                           (*values)[3]);
           }},
          {"Sampler",
           [&]() {
             absl::StatusOr<absl::string_view> type_name =
                 ReadParameters("Sampler", parameter_type_names_, storage,
                                tokenizer, parameters);
             if (!type_name.ok()) {
               return type_name.status();
             }

             return Sampler(*type_name, parameters);
           }},
          {"Scale",
           [&]() {
             auto values =
                 ReadFloatParameters("Scale", storage, tokenizer, 3, false);
             if (!values.ok()) {
               return values.status();
             }

             return Scale((*values)[0], (*values)[1], (*values)[2]);
           }},
          {"Shape",
           [&]() {
             absl::StatusOr<absl::string_view> type_name =
                 ReadParameters("Shape", parameter_type_names_, storage,
                                tokenizer, parameters);
             if (!type_name.ok()) {
               return type_name.status();
             }

             return Shape(*type_name, parameters);
           }},
          {"Texture",
           [&]() {
             auto name = ReadQuotedString("Texture", tokenizer);
             if (!name.ok()) {
               return name.status();
             }

             absl::string_view texture_name = storage.Add(*name);

             auto type = ReadQuotedString("Texture", tokenizer);
             if (!type.ok()) {
               return type.status();
             }

             absl::Status (Parser::*impl)(
                 absl::string_view, absl::string_view,
                 absl::flat_hash_map<absl::string_view, Parameter>&);
             if (*type == "color" || *type == "spectrum") {
               impl = &Parser::SpectrumTexture;
             } else if (*type == "float") {
               impl = &Parser::FloatTexture;
             } else {
               return absl::InvalidArgumentError(
                   absl::StrCat("Unrecgonized Texture type: \"", *type, "\""));
             }

             absl::StatusOr<absl::string_view> type_name =
                 ReadParameters("Texture", parameter_type_names_, storage,
                                tokenizer, parameters);
             if (!type_name.ok()) {
               return type_name.status();
             }

             return (this->*impl)(texture_name, *type_name, parameters);
           }},
          {"Transform",
           [&]() {
             auto values =
                 ReadFloatParameters("Transform", storage, tokenizer, 16, true);
             if (!values.ok()) {
               return values.status();
             }

             return Transform(
                 (*values)[0], (*values)[1], (*values)[2], (*values)[3],
                 (*values)[4], (*values)[5], (*values)[6], (*values)[7],
                 (*values)[8], (*values)[9], (*values)[10], (*values)[11],
                 (*values)[12], (*values)[13], (*values)[14], (*values)[15]);
           }},
          {"TransformBegin", [&]() { return TransformBegin(); }},
          {"TransformEnd", [&]() { return TransformEnd(); }},
          {"TransformTimes",
           [&]() {
             auto values = ReadFloatParameters("TransformTimes", storage,
                                               tokenizer, 2, false);
             if (!values.ok()) {
               return values.status();
             }

             return TransformTimes((*values)[0], (*values)[1]);
           }},
          {"Translate",
           [&]() {
             auto values =
                 ReadFloatParameters("Translate", storage, tokenizer, 3, false);
             if (!values.ok()) {
               return values.status();
             }

             return Translate((*values)[0], (*values)[1], (*values)[2]);
           }},
          {"WorldBegin", [&]() { return WorldBegin(); }},
          {"WorldEnd", [&]() { return WorldEnd(); }}};

  for (;;) {
    absl::StatusOr<const std::string*> next = tokenizer.Next();
    if (!next.ok()) {
      return next.status();
    }

    if (!*next) {
      break;
    }

    auto iter = directives.find(**next);
    if (iter == directives.end()) {
      return absl::InvalidArgumentError(
          absl::StrCat("Unrecognized directive: '", **next, "'"));
    }

    if (absl::Status status = iter->second(); !status.ok()) {
      return status;
    }

    for (const auto& [name, parameter] : parameters) {
      std::cerr << "Unused " << parameter.directive << " "
                << parameter.type_name << " parameter: '" << name << "'"
                << std::endl;
    }

    parameters.clear();
    storage.Clear();
  }

  return absl::OkStatus();
}

absl::Status TryRemoveFloats(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name, size_t required_size,
    std::optional<absl::Span<double>>& result) {
  return TryRemoveValues<ParameterType::FLOAT>(parameters, parameter_name,
                                               required_size, result);
}

absl::Status TryRemoveIntegers(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name, size_t required_size,
    std::optional<absl::Span<int32_t>>& result) {
  return TryRemoveValues<ParameterType::INTEGER>(parameters, parameter_name,
                                                 required_size, result);
}

std::optional<absl::Span<double>> TryRemoveFloats(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name) {
  std::optional<absl::Span<double>> result;
  TryRemoveValues<ParameterType::FLOAT>(parameters, parameter_name,
                                        std::nullopt, result)
      .IgnoreError();
  return result;
}

std::optional<absl::Span<int32_t>> TryRemoveIntegers(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name) {
  std::optional<absl::Span<int32_t>> result;
  TryRemoveValues<ParameterType::INTEGER>(parameters, parameter_name,
                                          std::nullopt, result)
      .IgnoreError();
  return result;
}

std::optional<std::array<double, 2>> TryRemoveBlackbodyV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name) {
  std::optional<std::array<double, 2>> result;
  TryRemoveValue<ParameterType::BLACKBODY_V1>(parameters, parameter_name,
                                              result)
      .IgnoreError();
  return result;
}

std::optional<bool> TryRemoveBool(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name) {
  std::optional<bool> result;
  TryRemoveValue<ParameterType::BOOL>(parameters, parameter_name, result)
      .IgnoreError();
  return result;
}

std::optional<double> TryRemoveFloat(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name) {
  std::optional<double> result;
  TryRemoveValue<ParameterType::FLOAT>(parameters, parameter_name, result)
      .IgnoreError();
  return result;
}

std::optional<int32_t> TryRemoveInteger(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name) {
  std::optional<int32_t> result;
  TryRemoveValue<ParameterType::INTEGER>(parameters, parameter_name, result)
      .IgnoreError();
  return result;
}

std::optional<absl::Span<std::array<double, 3>>> TryRemoveNormals(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name) {
  std::optional<absl::Span<std::array<double, 3>>> result;
  TryRemoveValues<ParameterType::NORMAL3>(parameters, parameter_name,
                                          std::nullopt, result)
      .IgnoreError();
  return result;
}

std::optional<absl::Span<std::array<double, 2>>> TryRemovePoint2s(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name) {
  std::optional<absl::Span<std::array<double, 2>>> result;
  TryRemoveValues<ParameterType::POINT2>(parameters, parameter_name,
                                         std::nullopt, result)
      .IgnoreError();
  return result;
}

std::optional<std::array<double, 3>> TryRemovePoint3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name) {
  std::optional<std::array<double, 3>> result;
  TryRemoveValue<ParameterType::POINT3>(parameters, parameter_name, result)
      .IgnoreError();
  return result;
}

std::optional<absl::Span<std::array<double, 3>>> TryRemovePoint3s(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name) {
  std::optional<absl::Span<std::array<double, 3>>> result;
  TryRemoveValues<ParameterType::POINT3>(parameters, parameter_name,
                                         std::nullopt, result)
      .IgnoreError();
  return result;
}

std::optional<std::array<double, 3>> TryRemoveRgb(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name) {
  std::optional<std::array<double, 3>> result;
  TryRemoveValue<ParameterType::RGB>(parameters, parameter_name, result)
      .IgnoreError();
  return result;
}

std::optional<absl::string_view> TryRemoveSpectrumFilename(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name) {
  std::optional<absl::string_view> result;
  TryRemoveValue<ParameterType::SPECTRUM>(parameters, parameter_name, result)
      .IgnoreError();
  return result;
}

std::optional<absl::Span<std::array<double, 2>>> TryRemoveSpectralSamples(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name) {
  std::optional<absl::Span<std::array<double, 2>>> result;
  TryRemoveValues<ParameterType::SPECTRUM>(parameters, parameter_name,
                                           std::nullopt, result)
      .IgnoreError();
  return result;
}

std::optional<absl::string_view> TryRemoveString(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name) {
  std::optional<absl::string_view> result;
  TryRemoveValue<ParameterType::STRING>(parameters, parameter_name, result)
      .IgnoreError();
  return result;
}

std::optional<absl::string_view> TryRemoveTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name) {
  std::optional<absl::string_view> result;
  TryRemoveValue<ParameterType::TEXTURE>(parameters, parameter_name, result)
      .IgnoreError();
  return result;
}

std::optional<std::array<double, 3>> TryRemoveVector3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name) {
  std::optional<std::array<double, 3>> result;
  TryRemoveValue<ParameterType::VECTOR3>(parameters, parameter_name, result)
      .IgnoreError();
  return result;
}

std::optional<absl::Span<std::array<double, 3>>> TryRemoveVector3s(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name) {
  std::optional<absl::Span<std::array<double, 3>>> result;
  TryRemoveValues<ParameterType::VECTOR3>(parameters, parameter_name,
                                          std::nullopt, result)
      .IgnoreError();
  return result;
}

std::optional<std::array<double, 3>> TryRemoveXyz(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name) {
  std::optional<std::array<double, 3>> result;
  TryRemoveValue<ParameterType::XYZ>(parameters, parameter_name, result)
      .IgnoreError();
  return result;
}

}  // namespace pbrt_proto
