#include "pbrt_proto/shared/common.h"

#include <limits>

#include "absl/container/flat_hash_map.h"
#include "absl/functional/function_ref.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {

absl::Status TryRemoveSpectrum(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name,
    absl::FunctionRef<Spectrum*()> get_output) {
  if (std::optional<std::array<double, 2>> value =
          TryRemoveBlackbodyV1(parameters, parameter_name);
      value) {
    get_output()->mutable_blackbody_spectrum()->set_temperature((*value)[0]);
    get_output()->mutable_blackbody_spectrum()->set_scale((*value)[1]);
    return absl::OkStatus();
  }

  if (std::optional<double> value =
          TryRemoveBlackbodyV2(parameters, parameter_name);
      value) {
    get_output()->mutable_blackbody_spectrum()->set_temperature(*value);
    return absl::OkStatus();
  }

  if (std::optional<std::array<double, 3>> value =
          TryRemoveRgb(parameters, parameter_name);
      value) {
    get_output()->mutable_rgb_spectrum()->set_r((*value)[0]);
    get_output()->mutable_rgb_spectrum()->set_g((*value)[1]);
    get_output()->mutable_rgb_spectrum()->set_b((*value)[2]);
    return absl::OkStatus();
  }

  if (std::optional<std::array<double, 3>> value =
          TryRemoveXyz(parameters, parameter_name);
      value) {
    get_output()->mutable_xyz_spectrum()->set_x((*value)[0]);
    get_output()->mutable_xyz_spectrum()->set_y((*value)[1]);
    get_output()->mutable_xyz_spectrum()->set_z((*value)[2]);
    return absl::OkStatus();
  }

  if (std::optional<absl::Span<std::array<double, 2>>> values =
          TryRemoveSpectralSamples(parameters, parameter_name);
      values) {
    if (values->size() > std::numeric_limits<int>::max()) {
      return absl::ResourceExhaustedError(
          "Spectrum has too many samples to be stored in a 1D proto array");
    }

    for (const auto [wavelength, intensity] : *values) {
      auto& sample = *get_output()->mutable_sampled_spectrum()->add_samples();
      sample.set_wavelength(wavelength);
      sample.set_intensity(intensity);
    }

    return absl::OkStatus();
  }

  if (std::optional<absl::string_view> value =
          TryRemoveSpectrumFilename(parameters, parameter_name);
      value) {
    get_output()->set_sampled_spectrum_filename(*value);
    return absl::OkStatus();
  }

  return absl::OkStatus();
}

void TryRemoveFloatTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name,
    absl::FunctionRef<FloatTextureParameter*()> get_output) {
  if (std::optional<double> value = TryRemoveFloat(parameters, parameter_name);
      value) {
    get_output()->set_float_value(*value);
  } else if (std::optional<absl::string_view> texture_name =
                 TryRemoveTexture(parameters, parameter_name);
             texture_name) {
    get_output()->set_float_texture_name(*texture_name);
  }
}

absl::Status TryRemoveSpectrumTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name,
    absl::FunctionRef<SpectrumTextureParameter*()> get_output) {
  if (std::optional<std::array<double, 2>> value =
          TryRemoveBlackbodyV1(parameters, parameter_name);
      value) {
    get_output()->mutable_blackbody_spectrum()->set_temperature((*value)[0]);
    get_output()->mutable_blackbody_spectrum()->set_scale((*value)[1]);
    return absl::OkStatus();
  }

  if (std::optional<double> value =
          TryRemoveBlackbodyV2(parameters, parameter_name);
      value) {
    get_output()->mutable_blackbody_spectrum()->set_temperature(*value);
    return absl::OkStatus();
  }

  if (std::optional<std::array<double, 3>> value =
          TryRemoveRgb(parameters, parameter_name);
      value) {
    get_output()->mutable_rgb_spectrum()->set_r((*value)[0]);
    get_output()->mutable_rgb_spectrum()->set_g((*value)[1]);
    get_output()->mutable_rgb_spectrum()->set_b((*value)[2]);
    return absl::OkStatus();
  }

  if (std::optional<std::array<double, 3>> value =
          TryRemoveXyz(parameters, parameter_name);
      value) {
    get_output()->mutable_xyz_spectrum()->set_x((*value)[0]);
    get_output()->mutable_xyz_spectrum()->set_y((*value)[1]);
    get_output()->mutable_xyz_spectrum()->set_z((*value)[2]);
    return absl::OkStatus();
  }

  if (std::optional<absl::Span<std::array<double, 2>>> values =
          TryRemoveSpectralSamples(parameters, parameter_name);
      values) {
    if (values->size() > std::numeric_limits<int>::max()) {
      return absl::ResourceExhaustedError(
          "Spectrum has too many samples to be stored in a 1D proto array");
    }

    for (const auto [wavelength, intensity] : *values) {
      auto& sample = *get_output()->mutable_sampled_spectrum()->add_samples();
      sample.set_wavelength(wavelength);
      sample.set_intensity(intensity);
    }
    return absl::OkStatus();
  }

  if (std::optional<absl::string_view> value =
          TryRemoveSpectrumFilename(parameters, parameter_name);
      value) {
    get_output()->set_sampled_spectrum_filename(*value);
    return absl::OkStatus();
  }

  if (std::optional<absl::string_view> value =
          TryRemoveTexture(parameters, parameter_name);
      value) {
    get_output()->set_spectrum_texture_name(*value);
    return absl::OkStatus();
  }

  return absl::OkStatus();
}

}  // namespace pbrt_proto
