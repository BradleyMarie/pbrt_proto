#include "pbrt_proto/shared/light_sources.h"

#include <algorithm>
#include <functional>

#include "absl/container/flat_hash_map.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/common.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {
namespace {

template <typename T>
void RemoveFilename(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters, T& output,
    bool pbrt_v4) {
  if (std::optional<absl::string_view> filename =
          TryRemoveString(parameters, pbrt_v4 ? "filename" : "mapname");
      filename) {
    output.set_filename(*filename);
  }
}

template <typename T>
void RemoveFrom(absl::flat_hash_map<absl::string_view, Parameter>& parameters,
                T& output) {
  if (std::optional<std::array<double, 3>> from =
          TryRemovePoint3(parameters, "from");
      from) {
    output.mutable_from()->set_x((*from)[0]);
    output.mutable_from()->set_y((*from)[1]);
    output.mutable_from()->set_z((*from)[2]);
  }
}

template <typename T>
absl::Status RemoveI(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters, T& output,
    bool pbrt_v4) {
  if (!pbrt_v4) {
    return TryRemoveSpectrumV1(parameters, "I",
                               std::bind(&T::mutable_i, &output));
  }

  return TryRemoveSpectrumV2(parameters, "I",
                             std::bind(&T::mutable_i, &output));
}

template <typename T>
void RemoveIlluminance(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters, T& output) {
  if (std::optional<double> illuminance =
          TryRemoveFloat(parameters, "illuminance");
      illuminance) {
    output.set_illuminance(*illuminance);
  }
}

template <typename T>
absl::Status RemoveL(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters, T& output,
    bool pbrt_v4) {
  if (!pbrt_v4) {
    return TryRemoveSpectrumV1(parameters, "L",
                               std::bind(&T::mutable_l, &output));
  }

  return TryRemoveSpectrumV2(parameters, "L",
                             std::bind(&T::mutable_l, &output));
}

template <typename T>
void RemovePower(absl::flat_hash_map<absl::string_view, Parameter>& parameters,
                 T& output) {
  if (std::optional<double> power = TryRemoveFloat(parameters, "power");
      power) {
    output.set_power(*power);
  }
}

template <typename T>
absl::Status RemoveScaleV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters, T& output) {
  return TryRemoveSpectrumV1(parameters, "scale",
                             std::bind(&T::mutable_scale, &output));
}

template <typename T>
void RemoveScaleV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters, T& output) {
  if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
      scale) {
    output.mutable_scale()->set_constant_spectrum(*scale);
  }
}

template <typename T>
void RemoveTo(absl::flat_hash_map<absl::string_view, Parameter>& parameters,
              T& output) {
  if (std::optional<std::array<double, 3>> to =
          TryRemovePoint3(parameters, "to");
      to) {
    output.mutable_to()->set_x((*to)[0]);
    output.mutable_to()->set_y((*to)[1]);
    output.mutable_to()->set_z((*to)[2]);
  }
}

absl::Status RemoveDistantLightSource(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DistantLightSource& output, bool pbrt_v4) {
  RemoveFrom(parameters, output);
  RemoveTo(parameters, output);
  return RemoveL(parameters, output, pbrt_v4);
}

absl::Status RemoveGoniometricLightSource(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    GoniometricLightSource& output, bool pbrt_v4) {
  RemoveFilename(parameters, output, pbrt_v4);
  return RemoveI(parameters, output, pbrt_v4);
}

absl::Status RemoveInfiniteLightSource(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    InfiniteLightSource& output, bool pbrt_v4) {
  RemoveFilename(parameters, output, pbrt_v4);
  return RemoveL(parameters, output, pbrt_v4);
}

absl::Status RemovePointLightSource(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PointLightSource& output, bool pbrt_v4) {
  RemoveFrom(parameters, output);
  return RemoveI(parameters, output, pbrt_v4);
}

absl::Status RemoveProjectionLightSource(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ProjectionLightSource& output, bool pbrt_v4) {
  RemoveFilename(parameters, output, pbrt_v4);

  if (std::optional<double> fov = TryRemoveFloat(parameters, "fov"); fov) {
    if (!pbrt_v4) {
      *fov *= 2.0;
    }

    output.set_fov(*fov);
  }

  return RemoveI(parameters, output, pbrt_v4);
}

absl::Status RemoveSpotLightSource(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SpotLightSource& output, bool pbrt_v4) {
  RemoveFrom(parameters, output);
  RemoveTo(parameters, output);

  if (std::optional<double> coneangle = TryRemoveFloat(parameters, "coneangle");
      coneangle) {
    output.set_coneangle(*coneangle);
  }

  if (std::optional<double> conedeltaangle =
          TryRemoveFloat(parameters, "conedeltaangle");
      conedeltaangle) {
    output.set_conedeltaangle(*conedeltaangle);
  }

  return RemoveI(parameters, output, pbrt_v4);
}

}  // namespace

absl::Status RemoveDistantLightSourceV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DistantLightSource& output) {
  return RemoveDistantLightSource(parameters, output, /*pbrt_v4=*/false);
}

absl::Status RemoveDistantLightSourceV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DistantLightSource& output) {
  if (absl::Status status =
          RemoveDistantLightSource(parameters, output, /*pbrt_v4=*/false);
      !status.ok()) {
    return status;
  }

  return RemoveScaleV1(parameters, output);
}

absl::Status RemoveDistantLightSourceV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DistantLightSource& output) {
  RemoveScaleV2(parameters, output);
  RemoveIlluminance(parameters, output);
  return RemoveDistantLightSource(parameters, output, /*pbrt_v4=*/true);
}

absl::Status RemoveGoniometricLightSourceV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    GoniometricLightSource& output) {
  return RemoveGoniometricLightSource(parameters, output, /*pbrt_v4=*/false);
}

absl::Status RemoveGoniometricLightSourceV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    GoniometricLightSource& output) {
  if (absl::Status status =
          RemoveGoniometricLightSource(parameters, output, /*pbrt_v4=*/false);
      !status.ok()) {
    return status;
  }

  return RemoveScaleV1(parameters, output);
}

absl::Status RemoveGoniometricLightSourceV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    GoniometricLightSource& output) {
  RemoveScaleV2(parameters, output);
  RemovePower(parameters, output);
  return RemoveGoniometricLightSource(parameters, output, /*pbrt_v4=*/true);
}

absl::Status RemoveInfiniteLightSourceV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    InfiniteLightSource& output) {
  if (std::optional<int32_t> samples = TryRemoveInteger(parameters, "nsamples");
      samples) {
    output.set_samples(std::max(0, *samples));
  }

  return RemoveInfiniteLightSource(parameters, output, /*pbrt_v4=*/false);
}

absl::Status RemoveInfiniteLightSourceV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    InfiniteLightSource& output) {
  if (absl::Status status = RemoveInfiniteLightSourceV1(parameters, output);
      !status.ok()) {
    return status;
  }

  return RemoveScaleV1(parameters, output);
}

absl::Status RemoveInfiniteLightSourceV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    InfiniteLightSource& output) {
  if (std::optional<int32_t> samples = TryRemoveInteger(parameters, "samples");
      samples) {
    output.set_samples(std::max(0, *samples));
  }

  return RemoveInfiniteLightSourceV2(parameters, output);
}

absl::Status RemoveInfiniteLightSourceV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    InfiniteLightSource& output) {
  RemoveScaleV2(parameters, output);
  RemoveIlluminance(parameters, output);
  return RemoveInfiniteLightSource(parameters, output, /*pbrt_v4=*/true);
}

absl::Status RemovePointLightSourceV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PointLightSource& output) {
  return RemovePointLightSource(parameters, output, /*pbrt_v4=*/false);
}

absl::Status RemovePointLightSourceV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PointLightSource& output) {
  if (absl::Status status =
          RemovePointLightSource(parameters, output, /*pbrt_v4=*/false);
      !status.ok()) {
    return status;
  }

  return RemoveScaleV1(parameters, output);
}

absl::Status RemovePointLightSourceV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PointLightSource& output) {
  RemoveScaleV2(parameters, output);
  RemovePower(parameters, output);
  return RemovePointLightSource(parameters, output, /*pbrt_v4=*/true);
}

absl::Status RemoveProjectionLightSourceV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ProjectionLightSource& output) {
  return RemoveProjectionLightSource(parameters, output, /*pbrt_v4=*/false);
}

absl::Status RemoveProjectionLightSourceV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ProjectionLightSource& output) {
  if (absl::Status status =
          RemoveProjectionLightSource(parameters, output, /*pbrt_v4=*/false);
      !status.ok()) {
    return status;
  }

  return RemoveScaleV1(parameters, output);
}

absl::Status RemoveProjectionLightSourceV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ProjectionLightSource& output) {
  RemoveScaleV2(parameters, output);
  RemovePower(parameters, output);
  return RemoveProjectionLightSource(parameters, output, /*pbrt_v4=*/true);
}

absl::Status RemoveSpotLightSourceV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SpotLightSource& output) {
  return RemoveSpotLightSource(parameters, output, /*pbrt_v4=*/false);
}

absl::Status RemoveSpotLightSourceV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SpotLightSource& output) {
  if (absl::Status status =
          RemoveSpotLightSource(parameters, output, /*pbrt_v4=*/false);
      !status.ok()) {
    return status;
  }

  return RemoveScaleV1(parameters, output);
}

absl::Status RemoveSpotLightSourceV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SpotLightSource& output) {
  RemoveScaleV2(parameters, output);
  RemovePower(parameters, output);
  return RemoveSpotLightSource(parameters, output, /*pbrt_v4=*/true);
}

}  // namespace pbrt_proto
