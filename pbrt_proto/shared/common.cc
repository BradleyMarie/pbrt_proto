#include "pbrt_proto/shared/common.h"

#include <optional>

#include "absl/container/flat_hash_map.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {

std::optional<CheckerboardAntialiasing> TryRemoveAaMode(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  if (std::optional<absl::string_view> aamode =
          TryRemoveString(parameters, "aamode");
      aamode) {
    if (*aamode == "closedform") {
      return CheckerboardAntialiasing::CLOSEDFORM;
    } else if (*aamode == "none") {
      return CheckerboardAntialiasing::NONE;
    }
  }

  return std::nullopt;
}

std::optional<TextureMapping> TryRemoveMapping(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  if (std::optional<absl::string_view> mapping =
          TryRemoveString(parameters, "mapping");
      mapping) {
    if (*mapping == "uv") {
      return TextureMapping::UV;
    } else if (*mapping == "spherical") {
      return TextureMapping::SPHERICAL;
    } else if (*mapping == "cylindrical") {
      return TextureMapping::CYLINDRICAL;
    } else if (*mapping == "planar") {
      return TextureMapping::PLANAR;
    }
  }

  return std::nullopt;
}

std::optional<ImageWrap> TryRemoveWrap(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  if (std::optional<absl::string_view> wrap =
          TryRemoveString(parameters, "wrap");
      wrap) {
    if (*wrap == "black") {
      return ImageWrap::BLACK;
    } else if (*wrap == "clamp") {
      return ImageWrap::CLAMP;
    } else if (*wrap == "repeat") {
      return ImageWrap::REPEAT;
    }
  }

  return std::nullopt;
}

}  // namespace pbrt_proto
