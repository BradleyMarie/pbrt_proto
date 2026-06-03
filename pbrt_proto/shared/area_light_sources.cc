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
namespace {

absl::Status RemoveDiffuseAreaLightSource(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    bool allow_scale, bool allow_samples, bool allow_twosided,
    DiffuseAreaLightSource& output) {
  if (absl::Status status = TryRemoveSpectrumV1(
          parameters, "L",
          std::bind(&DiffuseAreaLightSource::mutable_l, &output));
      !status.ok()) {
    return status;
  }

  if (std::optional<int32_t> samples = TryRemoveInteger(parameters, "nsamples");
      samples) {
    output.set_samples(std::max(0, *samples));
  }

  if (allow_scale) {
    if (absl::Status status = TryRemoveSpectrumV1(
            parameters, "scale",
            std::bind(&DiffuseAreaLightSource::mutable_scale, &output));
        !status.ok()) {
      return status;
    }
  }

  if (allow_samples) {
    if (std::optional<int32_t> samples =
            TryRemoveInteger(parameters, "samples");
        samples) {
      output.set_samples(std::max(0, *samples));
    }
  }

  if (allow_twosided) {
    if (std::optional<bool> twosided = TryRemoveBool(parameters, "twosided");
        twosided) {
      output.set_twosided(*twosided);
    }
  }

  return absl::OkStatus();
}

}  // namespace

absl::Status RemoveDiffuseAreaLightSourceV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DiffuseAreaLightSource& output) {
  return RemoveDiffuseAreaLightSource(parameters, /*allow_scale=*/false,
                                      /*allow_samples=*/false,
                                      /*allow_twosided=*/false, output);
}

absl::Status RemoveDiffuseAreaLightSourceV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DiffuseAreaLightSource& output) {
  return RemoveDiffuseAreaLightSource(parameters, /*allow_scale=*/true,
                                      /*allow_samples=*/false,
                                      /*allow_twosided=*/false, output);
}

absl::Status RemoveDiffuseAreaLightSourceV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DiffuseAreaLightSource& output) {
  return RemoveDiffuseAreaLightSource(parameters, /*allow_scale=*/true,
                                      /*allow_samples=*/true,
                                      /*allow_twosided=*/true, output);
}

absl::Status RemoveDiffuseAreaLightSourceV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DiffuseAreaLightSource& output) {
  if (absl::Status status = TryRemoveSpectrumV2(
          parameters, "L",
          std::bind(&DiffuseAreaLightSource::mutable_l, &output));
      !status.ok()) {
    return status;
  }

  if (std::optional<absl::string_view> filename =
          TryRemoveString(parameters, "filename");
      filename) {
    if (output.has_l()) {
      return absl::InvalidArgumentError(
          "A diffuse AreaLightSource may not specify both 'L' and 'filename'");
    }

    output.set_filename(*filename);
  }

  if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
      scale) {
    output.mutable_scale()->set_constant_spectrum(*scale);
  }

  if (std::optional<double> power = TryRemoveFloat(parameters, "power");
      power) {
    output.set_power(*power);
  }

  if (std::optional<bool> twosided = TryRemoveBool(parameters, "twosided");
      twosided) {
    output.set_twosided(*twosided);
  }

  return absl::OkStatus();
}

}  // namespace pbrt_proto
