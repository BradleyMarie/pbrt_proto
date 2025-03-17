#include "pbrt_proto/shared/parser.h"

#include <cassert>
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
  chars_.back().clear();

  for (char c : to_add) {
    chars_.back().emplace_back(c);
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
    Tokenizer& tokenizer, size_t num_to_read) {
  absl::InlinedVector<double, 16> results;
  for (size_t i = 0; i < num_to_read; i++) {
    absl::StatusOr<const std::string*> next = tokenizer.Next();
    if (!next.ok()) {
      return next.status();
    }

    if (!*next) {
      return absl::InvalidArgumentError(
          absl::StrCat("Directive ", directive, " requires exactly ",
                       num_to_read, " parameters"));
    }

    double value;
    if (!absl::SimpleAtod(**next, &value)) {
      return absl::InvalidArgumentError(absl::StrCat(
          "Invalid parameter to directive ", directive, ": '", **next, "'"));
    }

    results.push_back(value);
  }

  return results;
}

absl::StatusOr<absl::string_view> ReadTypeName(absl::string_view directive,
                                               ParameterStorage& storage,
                                               Tokenizer& tokenizer) {
  absl::StatusOr<const std::string*> next = tokenizer.Next();
  if (!next.ok()) {
    return next.status();
  }

  if (!*next) {
    return absl::InvalidArgumentError(
        absl::StrCat("Missing type parameter to directive ", directive));
  }

  if ((**next)[0] != '"') {
    return absl::InvalidArgumentError(
        absl::StrCat("Unquoted type parameter to directive ", directive, ": '",
                     **next, "'"));
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
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  absl::StatusOr<absl::string_view> type_name =
      ReadTypeName(directive, storage, tokenizer);
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
    parameter.type = std::get<0>(**parameter_type_and_name);
    parameter.type_name = std::get<1>(**parameter_type_and_name);
    parameter.values = std::move(values);
  }

  return *type_name;
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

  const absl::Span<T>& values =
      *std::get_if<absl::Span<T>>(&iter->second.values);
  if (values.size() != 1) {
    return absl::InvalidArgumentError("TODO_Q");
  }

  value = values[0];
  parameters.erase(iter);

  return absl::OkStatus();
}

}  // namespace

absl::Status Parser::ReadFrom(std::istream& stream) {
  Tokenizer tokenizer(stream);

  ParameterStorage storage;
  absl::flat_hash_map<absl::string_view, Parameter> parameters;
  for (;;) {
    absl::StatusOr<const std::string*> next = tokenizer.Next();
    if (!next.ok()) {
      return next.status();
    }

    if (!*next) {
      break;
    }

    absl::Status status;
    if (**next == "Accelerator") {
      absl::StatusOr<absl::string_view> type_name = ReadParameters(
          "Accelerator", parameter_type_names_, storage, tokenizer, parameters);
      if (!type_name.ok()) {
        return type_name.status();
      }

      status = Accelerator(*type_name, parameters);
    } else if (**next == "ActiveTransform") {
      next = tokenizer.Next();
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
            "Invalid parameter to directive ActiveTransforms: '" + **next +
            "'");
      }

      status = ActiveTransform(transformation);
    } else if (**next == "ConcatTransform") {
      auto values =
          ReadFloatParameters("ConcatTransform", storage, tokenizer, 16);
      if (!values.ok()) {
        return values.status();
      }

      status = ConcatTransform(
          (*values)[0], (*values)[1], (*values)[2], (*values)[3], (*values)[4],
          (*values)[5], (*values)[6], (*values)[7], (*values)[8], (*values)[9],
          (*values)[10], (*values)[11], (*values)[12], (*values)[13],
          (*values)[14], (*values)[15]);
    } else if (**next == "CoordinateSystem") {
      auto name = ReadQuotedString("CoordinateSystem", tokenizer);
      if (!name.ok()) {
        return name.status();
      }

      status = CoordinateSystem(*name);
    } else if (**next == "CoordSysTransform") {
      auto name = ReadQuotedString("CoordSysTransform", tokenizer);
      if (!name.ok()) {
        return name.status();
      }

      status = CoordSysTransform(*name);
    } else if (**next == "Identity") {
      status = Identity();
    } else if (**next == "Include") {
      auto name = ReadQuotedString("Include", tokenizer);
      if (!name.ok()) {
        return name.status();
      }

      status = Include(*name);
    } else if (**next == "Import") {
      auto name = ReadQuotedString("Import", tokenizer);
      if (!name.ok()) {
        return name.status();
      }

      status = Import(*name);
    } else if (**next == "LookAt") {
      auto values = ReadFloatParameters("LookAt", storage, tokenizer, 9);
      if (!values.ok()) {
        return values.status();
      }

      status = LookAt((*values)[0], (*values)[1], (*values)[2], (*values)[3],
                      (*values)[4], (*values)[5], (*values)[6], (*values)[7],
                      (*values)[8]);
    } else if (**next == "ReverseOrientation") {
      status = ReverseOrientation();
    } else if (**next == "Rotate") {
      auto values = ReadFloatParameters("Rotate", storage, tokenizer, 4);
      if (!values.ok()) {
        return values.status();
      }

      status = Rotate((*values)[0], (*values)[1], (*values)[2], (*values)[3]);
    } else if (**next == "Scale") {
      auto values = ReadFloatParameters("Scale", storage, tokenizer, 3);
      if (!values.ok()) {
        return values.status();
      }

      status = Scale((*values)[0], (*values)[1], (*values)[2]);
    } else if (**next == "Transform") {
      auto values = ReadFloatParameters("Transform", storage, tokenizer, 16);
      if (!values.ok()) {
        return values.status();
      }

      status =
          Transform((*values)[0], (*values)[1], (*values)[2], (*values)[3],
                    (*values)[4], (*values)[5], (*values)[6], (*values)[7],
                    (*values)[8], (*values)[9], (*values)[10], (*values)[11],
                    (*values)[12], (*values)[13], (*values)[14], (*values)[15]);
    } else if (**next == "TransformTimes") {
      auto values =
          ReadFloatParameters("TransformTimes", storage, tokenizer, 2);
      if (!values.ok()) {
        return values.status();
      }

      status = TransformTimes((*values)[0], (*values)[1]);
    } else if (**next == "Translate") {
      auto values = ReadFloatParameters("Translate", storage, tokenizer, 3);
      if (!values.ok()) {
        return values.status();
      }

      status = Translate((*values)[0], (*values)[1], (*values)[2]);
    } else if (**next == "WorldBegin") {
      status = WorldBegin();
    } else if (**next == "WorldEnd") {
      status = WorldEnd();
    } else {
      return absl::UnimplementedError(**next);
    }

    if (!status.ok()) {
      return status;
    }

    parameters.clear();
    storage.Clear();
  }

  return absl::OkStatus();
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

std::optional<absl::string_view> TryRemoveString(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name) {
  std::optional<absl::string_view> result;
  TryRemoveValue<ParameterType::STRING>(parameters, parameter_name, result)
      .IgnoreError();
  return result;
}

}  // namespace pbrt_proto