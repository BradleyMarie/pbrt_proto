#include "pbrt_proto/shared/area_light_sources.h"

#include <algorithm>
#include <functional>

#include "absl/container/flat_hash_map.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/common.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {
namespace {

void RemoveDiffuseAreaLightSource(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    bool allow_scale, bool allow_samples, bool allow_twosided,
    DiffuseAreaLightSourceV1& output) {
  TryRemoveSpectrumV1(parameters, "L",
                      std::bind(&DiffuseAreaLightSourceV1::mutable_l, &output));

  if (std::optional<int32_t> samples = TryRemoveInteger(parameters, "nsamples");
      samples) {
    output.set_samples(std::max(0, *samples));
  }

  if (allow_scale) {
    TryRemoveSpectrumV1(
        parameters, "scale",
        std::bind(&DiffuseAreaLightSourceV1::mutable_scale, &output));
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
}

}  // namespace

void RemoveDiffuseAreaLightSourceV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DiffuseAreaLightSourceV1& output) {
  RemoveDiffuseAreaLightSource(parameters, /*allow_scale=*/false,
                               /*allow_samples=*/false,
                               /*allow_twosided=*/false, output);
}

void RemoveDiffuseAreaLightSourceV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DiffuseAreaLightSourceV1& output) {
  RemoveDiffuseAreaLightSource(parameters, /*allow_scale=*/true,
                               /*allow_samples=*/false,
                               /*allow_twosided=*/false, output);
}

void RemoveDiffuseAreaLightSourceV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DiffuseAreaLightSourceV1& output) {
  RemoveDiffuseAreaLightSource(parameters, /*allow_scale=*/true,
                               /*allow_samples=*/true,
                               /*allow_twosided=*/true, output);
}

absl::Status RemoveDiffuseAreaLightSourceV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DiffuseAreaLightSourceV2& output) {
  TryRemoveSpectrumV2(parameters, "L",
                      std::bind(&DiffuseAreaLightSourceV2::mutable_l, &output));

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
    output.set_scale(*scale);
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
