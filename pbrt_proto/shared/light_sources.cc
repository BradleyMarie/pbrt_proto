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
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, T& output) {
  if (std::optional<absl::string_view> filename = TryRemoveString(
          parameters, pbrt_version >= 4 ? "filename" : "mapname");
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
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, T& output) {
  return TryRemoveSpectrum(parameters, pbrt_version, "I",
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
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, T& output) {
  return TryRemoveSpectrum(parameters, pbrt_version, "L",
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
absl::Status RemoveScale(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, T& output) {
  if (pbrt_version <= 3) {
    return TryRemoveSpectrumV1(parameters, "scale",
                               std::bind(&T::mutable_scale, &output));
  } else {
    if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
        scale) {
      output.mutable_scale()->set_constant_spectrum(*scale);
    }
  }

  return absl::OkStatus();
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

}  // namespace

absl::Status RemoveDistantLightSource(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, DistantLightSource& output) {
  RemoveFrom(parameters, output);
  RemoveTo(parameters, output);

  if (pbrt_version >= 2) {
    if (absl::Status status = RemoveScale(parameters, pbrt_version, output);
        !status.ok()) {
      return status;
    }
  }

  if (pbrt_version >= 4) {
    RemoveIlluminance(parameters, output);
  }

  return RemoveL(parameters, pbrt_version, output);
}

absl::Status RemoveGoniometricLightSource(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, GoniometricLightSource& output) {
  RemoveFilename(parameters, pbrt_version, output);

  if (pbrt_version >= 2) {
    if (absl::Status status = RemoveScale(parameters, pbrt_version, output);
        !status.ok()) {
      return status;
    }
  }

  if (pbrt_version >= 4) {
    RemovePower(parameters, output);
  }

  return RemoveI(parameters, pbrt_version, output);
}

absl::Status RemoveInfiniteLightSource(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, InfiniteLightSource& output) {
  RemoveFilename(parameters, pbrt_version, output);

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

  if (pbrt_version >= 2) {
    if (absl::Status status = RemoveScale(parameters, pbrt_version, output);
        !status.ok()) {
      return status;
    }
  }

  if (pbrt_version >= 4) {
    RemoveIlluminance(parameters, output);
  }

  return RemoveL(parameters, pbrt_version, output);
}

absl::Status RemovePointLightSource(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, PointLightSource& output) {
  RemoveFrom(parameters, output);

  if (pbrt_version >= 2) {
    if (absl::Status status = RemoveScale(parameters, pbrt_version, output);
        !status.ok()) {
      return status;
    }
  }

  if (pbrt_version >= 4) {
    RemovePower(parameters, output);
  }

  return RemoveI(parameters, pbrt_version, output);
}

absl::Status RemoveProjectionLightSource(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, ProjectionLightSource& output) {
  RemoveFilename(parameters, pbrt_version, output);

  if (std::optional<double> fov = TryRemoveFloat(parameters, "fov"); fov) {
    if (pbrt_version <= 3) {
      *fov *= 2.0;
    }

    output.set_fov(*fov);
  }

  if (pbrt_version >= 2) {
    if (absl::Status status = RemoveScale(parameters, pbrt_version, output);
        !status.ok()) {
      return status;
    }
  }

  if (pbrt_version >= 4) {
    RemovePower(parameters, output);
  }

  return RemoveI(parameters, pbrt_version, output);
}

absl::Status RemoveSpotLightSource(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, SpotLightSource& output) {
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

  if (pbrt_version >= 2) {
    if (absl::Status status = RemoveScale(parameters, pbrt_version, output);
        !status.ok()) {
      return status;
    }
  }

  if (pbrt_version >= 4) {
    RemovePower(parameters, output);
  }

  return RemoveI(parameters, pbrt_version, output);
}

}  // namespace pbrt_proto
