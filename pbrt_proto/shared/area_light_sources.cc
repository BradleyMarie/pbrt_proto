#include "pbrt_proto/shared/area_light_sources.h"

#include <algorithm>
#include <functional>

#include "absl/container/flat_hash_map.h"
#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/common.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {

absl::Status RemoveDiffuseAreaLightSource(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, DiffuseAreaLightSource& output) {
  if (absl::Status status = TryRemoveSpectrum(
          parameters, "L",
          std::bind(&DiffuseAreaLightSource::mutable_l, &output));
      !status.ok()) {
    return status;
  }

  if (pbrt_version >= 4) {
    if (std::optional<absl::string_view> filename =
            TryRemoveString(parameters, "filename");
        filename) {
      if (output.has_l()) {
        return absl::InvalidArgumentError(
            "A diffuse AreaLightSource may not specify both 'L' and "
            "'filename'");
      }

      output.set_filename(*filename);
    }

    if (std::optional<double> power = TryRemoveFloat(parameters, "power");
        power) {
      output.set_power(*power);
    }
  }

  if (pbrt_version >= 3) {
    if (std::optional<bool> twosided = TryRemoveBool(parameters, "twosided");
        twosided) {
      output.set_twosided(*twosided);
    }
  }

  if (pbrt_version >= 2 && pbrt_version <= 3) {
    if (absl::Status status = TryRemoveSpectrum(
            parameters, "scale",
            std::bind(&DiffuseAreaLightSource::mutable_scale, &output));
        !status.ok()) {
      return status;
    }
  } else if (pbrt_version >= 4) {
    if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
        scale) {
      output.mutable_scale()->set_constant_spectrum(*scale);
    }
  }

  if (pbrt_version <= 3) {
    if (std::optional<int32_t> samples =
            TryRemoveInteger(parameters, "nsamples");
        samples) {
      output.set_samples(std::max(0, *samples));
    }
  }

  if (pbrt_version == 3) {
    if (std::optional<int32_t> samples =
            TryRemoveInteger(parameters, "samples");
        samples) {
      output.set_samples(std::max(0, *samples));
    }
  }

  return absl::OkStatus();
}

}  // namespace pbrt_proto
