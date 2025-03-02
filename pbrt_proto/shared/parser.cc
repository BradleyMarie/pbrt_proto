#include "pbrt_proto/shared/parser.h"

#include <istream>

#include "absl/container/flat_hash_map.h"
#include "absl/container/inlined_vector.h"
#include "absl/status/status.h"
#include "absl/strings/numbers.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/shared/tokenizer.h"

namespace pbrt_proto {
namespace {

absl::StatusOr<absl::string_view> ReadQuotedString(absl::string_view directive,
                                                   Tokenizer& tokenizer) {
  auto next = tokenizer.Next();
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

bool ParseValue(absl::string_view str, float& out) {
  return absl::SimpleAtof(str, &out);
}

bool ParseValue(absl::string_view str, double& out) {
  return absl::SimpleAtod(str, &out);
}

template <typename T>
absl::StatusOr<absl::InlinedVector<T, 16>> ReadNumericValues(
    absl::string_view directive, Tokenizer& tokenizer) {
  auto next = tokenizer.Next();
  if (!next.ok()) {
    return next.status();
  }

  if (!*next) {
    return absl::InvalidArgumentError(
        absl::StrCat("Missing parameter to directive ", directive));
  }

  absl::InlinedVector<T, 16> results;
  if (**next != "[") {
    for (;;) {
      next = tokenizer.Next();
      if (!next.ok()) {
        return next.status();
      }

      if (!*next) {
        return absl::InvalidArgumentError(
            absl::StrCat("Missing parameter to directive ", directive));
      }

      if (**next == "]") {
        break;
      }

      T value;
      if (!ParseValue(**next, value)) {
        return absl::InvalidArgumentError(absl::StrCat(
            "Invalid parameter to directive ", directive, ": ", **next));
      }

      results.push_back(value);
    }
  } else {
    T value;
    if (!ParseValue(**next, value)) {
      return absl::InvalidArgumentError(absl::StrCat(
          "Invalid parameter to directive ", directive, ": ", **next));
    }

    results.push_back(value);
  }

  return results;
}

}  // namespace

absl::Status Parser::ReadFrom(std::istream& stream) {
  Tokenizer tokenizer(stream);

  for (;;) {
    auto next = tokenizer.Next();
    if (!next.ok()) {
      return next.status();
    }

    if (!*next) {
      break;
    }

    absl::Status status;
    if (**next == "ActiveTransform") {
      next = tokenizer.Next();
      if (!next.ok()) {
        return next.status();
      }

      if (!*next) {
        return absl::InvalidArgumentError("Unexpected EOF");
      } else if (**next == "All") {
        status = ActiveTransform(ActiveTransformation::ALL);
      } else if (**next == "StartTime") {
        status = ActiveTransform(ActiveTransformation::START_TIME);
      } else if (**next == "EndTime") {
        status = ActiveTransform(ActiveTransformation::END_TIME);
      } else {
        return absl::InvalidArgumentError(
            "Invalid parameter to directive ActiveTransforms: '" + **next +
            "'");
      }
    } else if (**next == "ConcatTransform") {
      auto values = ReadNumericValues<double>("ConcatTransform", tokenizer);
      if (!values.ok()) {
        return values.status();
      }

      if (values->size() != 16) {
        return absl::InvalidArgumentError(
            "Directive ConcatTransform requires exactly 16 parameter values");
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
      auto values = ReadNumericValues<double>("LookAt", tokenizer);
      if (!values.ok()) {
        return values.status();
      }

      if (values->size() != 9) {
        return absl::InvalidArgumentError(
            "Directive LookAt requires exactly 9 parameter values");
      }

      status = LookAt((*values)[0], (*values)[1], (*values)[2], (*values)[3],
                      (*values)[4], (*values)[5], (*values)[6], (*values)[7],
                      (*values)[8]);
    } else if (**next == "ReverseOrientation") {
      status = ReverseOrientation();
    } else if (**next == "Rotate") {
      auto values = ReadNumericValues<double>("Rotate", tokenizer);
      if (!values.ok()) {
        return values.status();
      }

      if (values->size() != 4) {
        return absl::InvalidArgumentError(
            "Directive Rotate requires exactly 4 parameter values");
      }

      status = Rotate((*values)[0], (*values)[1], (*values)[2], (*values)[3]);
    } else if (**next == "Scale") {
      auto values = ReadNumericValues<double>("Scale", tokenizer);
      if (!values.ok()) {
        return values.status();
      }

      if (values->size() != 3) {
        return absl::InvalidArgumentError(
            "Directive Scale requires exactly 3 parameter values");
      }

      status = Scale((*values)[0], (*values)[1], (*values)[2]);
    } else if (**next == "Transform") {
      auto values = ReadNumericValues<double>("Transform", tokenizer);
      if (!values.ok()) {
        return values.status();
      }

      if (values->size() != 16) {
        return absl::InvalidArgumentError(
            "Directive Transform requires exactly 16 parameter values");
      }

      status =
          Transform((*values)[0], (*values)[1], (*values)[2], (*values)[3],
                    (*values)[4], (*values)[5], (*values)[6], (*values)[7],
                    (*values)[8], (*values)[9], (*values)[10], (*values)[11],
                    (*values)[12], (*values)[13], (*values)[14], (*values)[15]);
    } else if (**next == "TransformTimes") {
      auto values = ReadNumericValues<double>("TransformTimes", tokenizer);
      if (!values.ok()) {
        return values.status();
      }

      if (values->size() != 2) {
        return absl::InvalidArgumentError(
            "Directive TransformTimes requires exactly 2 parameter values");
      }

      status = TransformTimes((*values)[0], (*values)[1]);
    } else if (**next == "Translate") {
      auto values = ReadNumericValues<double>("Translate", tokenizer);
      if (!values.ok()) {
        return values.status();
      }

      if (values->size() != 3) {
        return absl::InvalidArgumentError(
            "Directive Translate requires exactly 3 parameter values");
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
  }

  return absl::OkStatus();
}

}  // namespace pbrt_proto