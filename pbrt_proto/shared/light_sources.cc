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
void RemoveI(absl::flat_hash_map<absl::string_view, Parameter>& parameters,
             T& output, bool pbrt_v4) {
  if (!pbrt_v4) {
    TryRemoveSpectrumV1(parameters, "I", std::bind(&T::mutable_i, &output));
  } else {
    TryRemoveSpectrumV2(parameters, "I", std::bind(&T::mutable_i, &output));
  }
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
void RemoveL(absl::flat_hash_map<absl::string_view, Parameter>& parameters,
             T& output, bool pbrt_v4) {
  if (!pbrt_v4) {
    TryRemoveSpectrumV1(parameters, "L", std::bind(&T::mutable_l, &output));
  } else {
    TryRemoveSpectrumV2(parameters, "L", std::bind(&T::mutable_l, &output));
  }
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
void RemoveScaleV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters, T& output) {
  TryRemoveSpectrumV1(parameters, "scale",
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

void RemoveDistantLightSource(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DistantLightSource& output, bool pbrt_v4) {
  RemoveL(parameters, output, pbrt_v4);
  RemoveFrom(parameters, output);
  RemoveTo(parameters, output);
}

void RemoveGoniometricLightSource(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    GoniometricLightSource& output, bool pbrt_v4) {
  RemoveI(parameters, output, pbrt_v4);
  RemoveFilename(parameters, output, pbrt_v4);
}

void RemoveInfiniteLightSource(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    InfiniteLightSource& output, bool pbrt_v4) {
  RemoveL(parameters, output, pbrt_v4);
  RemoveFilename(parameters, output, pbrt_v4);
}

void RemovePointLightSource(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PointLightSource& output, bool pbrt_v4) {
  RemoveI(parameters, output, pbrt_v4);
  RemoveFrom(parameters, output);
}

void RemoveProjectionLightSource(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ProjectionLightSource& output, bool pbrt_v4) {
  RemoveI(parameters, output, pbrt_v4);
  RemoveFilename(parameters, output, pbrt_v4);

  if (std::optional<double> fov = TryRemoveFloat(parameters, "fov"); fov) {
    if (!pbrt_v4) {
      *fov *= 2.0;
    }

    output.set_fov(*fov);
  }
}

void RemoveSpotLightSource(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SpotLightSource& output, bool pbrt_v4) {
  RemoveI(parameters, output, pbrt_v4);
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
}

}  // namespace

void RemoveDistantLightSourceV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DistantLightSource& output) {
  RemoveDistantLightSource(parameters, output, /*pbrt_v4=*/false);
}

void RemoveDistantLightSourceV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DistantLightSource& output) {
  RemoveDistantLightSource(parameters, output, /*pbrt_v4=*/false);
  RemoveScaleV1(parameters, output);
}

void RemoveDistantLightSourceV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DistantLightSource& output) {
  RemoveDistantLightSource(parameters, output, /*pbrt_v4=*/true);
  RemoveScaleV2(parameters, output);
  RemoveIlluminance(parameters, output);
}

void RemoveGoniometricLightSourceV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    GoniometricLightSource& output) {
  RemoveGoniometricLightSource(parameters, output, /*pbrt_v4=*/false);
}

void RemoveGoniometricLightSourceV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    GoniometricLightSource& output) {
  RemoveGoniometricLightSource(parameters, output, /*pbrt_v4=*/false);
  RemoveScaleV1(parameters, output);
}

void RemoveGoniometricLightSourceV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    GoniometricLightSource& output) {
  RemoveGoniometricLightSource(parameters, output, /*pbrt_v4=*/true);
  RemoveScaleV2(parameters, output);
  RemovePower(parameters, output);
}

void RemoveInfiniteLightSourceV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    InfiniteLightSource& output) {
  RemoveInfiniteLightSource(parameters, output, /*pbrt_v4=*/false);

  if (std::optional<int32_t> samples = TryRemoveInteger(parameters, "nsamples");
      samples) {
    output.set_samples(std::max(0, *samples));
  }
}

void RemoveInfiniteLightSourceV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    InfiniteLightSource& output) {
  RemoveInfiniteLightSourceV1(parameters, output);
  RemoveScaleV1(parameters, output);
}

void RemoveInfiniteLightSourceV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    InfiniteLightSource& output) {
  RemoveInfiniteLightSourceV2(parameters, output);

  if (std::optional<int32_t> samples = TryRemoveInteger(parameters, "samples");
      samples) {
    output.set_samples(std::max(0, *samples));
  }
}

void RemoveInfiniteLightSourceV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    InfiniteLightSource& output) {
  RemoveInfiniteLightSource(parameters, output, /*pbrt_v4=*/true);
  RemoveScaleV2(parameters, output);
  RemoveIlluminance(parameters, output);
}

void RemovePointLightSourceV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PointLightSource& output) {
  RemovePointLightSource(parameters, output, /*pbrt_v4=*/false);
}

void RemovePointLightSourceV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PointLightSource& output) {
  RemovePointLightSource(parameters, output, /*pbrt_v4=*/false);
  RemoveScaleV1(parameters, output);
}

void RemovePointLightSourceV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PointLightSource& output) {
  RemovePointLightSource(parameters, output, /*pbrt_v4=*/true);
  RemoveScaleV2(parameters, output);
  RemovePower(parameters, output);
}

void RemoveProjectionLightSourceV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ProjectionLightSource& output) {
  RemoveProjectionLightSource(parameters, output, /*pbrt_v4=*/false);
}

void RemoveProjectionLightSourceV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ProjectionLightSource& output) {
  RemoveProjectionLightSource(parameters, output, /*pbrt_v4=*/false);
  RemoveScaleV1(parameters, output);
}

void RemoveProjectionLightSourceV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ProjectionLightSource& output) {
  RemoveProjectionLightSource(parameters, output, /*pbrt_v4=*/true);
  RemoveScaleV2(parameters, output);
  RemovePower(parameters, output);
}

void RemoveSpotLightSourceV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SpotLightSource& output) {
  RemoveSpotLightSource(parameters, output, /*pbrt_v4=*/false);
}

void RemoveSpotLightSourceV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SpotLightSource& output) {
  RemoveSpotLightSource(parameters, output, /*pbrt_v4=*/false);
  RemoveScaleV1(parameters, output);
}

void RemoveSpotLightSourceV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SpotLightSource& output) {
  RemoveSpotLightSource(parameters, output, /*pbrt_v4=*/true);
  RemoveScaleV2(parameters, output);
  RemovePower(parameters, output);
}

}  // namespace pbrt_proto
