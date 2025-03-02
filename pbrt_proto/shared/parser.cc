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

bool ParseValue(absl::string_view str, double& out) {
  return absl::SimpleAtod(str, &out);
}

template <typename T>
absl::StatusOr<absl::InlinedVector<T, 16>> ReadNumericParameters(
    absl::string_view directive, Tokenizer& tokenizer, size_t num_to_read) {
  absl::InlinedVector<T, 16> results;
  for (size_t i = 0; i < num_to_read; i++) {
    auto next = tokenizer.Next();
    if (!next.ok()) {
      return next.status();
    }

    if (!*next) {
      return absl::InvalidArgumentError(
          absl::StrCat("Directive ", directive, " requires exactly ",
                       num_to_read, " parameters"));
    }

    T value;
    if (!ParseValue(**next, value)) {
      return absl::InvalidArgumentError(absl::StrCat(
          "Invalid parameter to directive ", directive, ": '", **next, "'"));
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
          ReadNumericParameters<double>("ConcatTransform", tokenizer, 16);
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
      auto values = ReadNumericParameters<double>("LookAt", tokenizer, 9);
      if (!values.ok()) {
        return values.status();
      }

      status = LookAt((*values)[0], (*values)[1], (*values)[2], (*values)[3],
                      (*values)[4], (*values)[5], (*values)[6], (*values)[7],
                      (*values)[8]);
    } else if (**next == "ReverseOrientation") {
      status = ReverseOrientation();
    } else if (**next == "Rotate") {
      auto values = ReadNumericParameters<double>("Rotate", tokenizer, 4);
      if (!values.ok()) {
        return values.status();
      }

      status = Rotate((*values)[0], (*values)[1], (*values)[2], (*values)[3]);
    } else if (**next == "Scale") {
      auto values = ReadNumericParameters<double>("Scale", tokenizer, 3);
      if (!values.ok()) {
        return values.status();
      }

      status = Scale((*values)[0], (*values)[1], (*values)[2]);
    } else if (**next == "Transform") {
      auto values = ReadNumericParameters<double>("Transform", tokenizer, 16);
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
          ReadNumericParameters<double>("TransformTimes", tokenizer, 2);
      if (!values.ok()) {
        return values.status();
      }

      status = TransformTimes((*values)[0], (*values)[1]);
    } else if (**next == "Translate") {
      auto values = ReadNumericParameters<double>("Translate", tokenizer, 3);
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
  }

  return absl::OkStatus();
}

}  // namespace pbrt_proto