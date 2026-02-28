#include "pbrt_proto/shared/common.h"

#include <optional>

#include "absl/container/flat_hash_map.h"
#include "absl/functional/function_ref.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {
namespace {

void TryRemoveSpectrum(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name, bool allow_scale,
    absl::FunctionRef<Spectrum*()> get_output) {
  if (allow_scale) {
    if (std::optional<std::array<double, 2>> value =
            TryRemoveBlackbodyV1(parameters, parameter_name);
        value) {
      get_output()->mutable_blackbody_spectrum()->set_temperature((*value)[0]);
      get_output()->mutable_blackbody_spectrum()->set_scale((*value)[1]);
      return;
    }
  } else {
    if (std::optional<double> value =
            TryRemoveBlackbodyV2(parameters, parameter_name);
        value) {
      get_output()->mutable_blackbody_spectrum()->set_temperature(*value);
      return;
    }
  }

  if (std::optional<std::array<double, 3>> value =
          TryRemoveRgb(parameters, parameter_name);
      value) {
    get_output()->mutable_rgb_spectrum()->set_r((*value)[0]);
    get_output()->mutable_rgb_spectrum()->set_g((*value)[1]);
    get_output()->mutable_rgb_spectrum()->set_b((*value)[2]);
    return;
  }

  if (std::optional<std::array<double, 3>> value =
          TryRemoveXyz(parameters, parameter_name);
      value) {
    get_output()->mutable_xyz_spectrum()->set_x((*value)[0]);
    get_output()->mutable_xyz_spectrum()->set_y((*value)[1]);
    get_output()->mutable_xyz_spectrum()->set_z((*value)[2]);
    return;
  }

  if (std::optional<absl::Span<std::array<double, 2>>> values =
          TryRemoveSpectralSamples(parameters, parameter_name);
      values) {
    for (const auto [wavelength, intensity] : *values) {
      auto& sample = *get_output()->mutable_sampled_spectrum()->add_samples();
      sample.set_wavelength(wavelength);
      sample.set_intensity(intensity);
    }

    return;
  }

  if (std::optional<absl::string_view> value =
          TryRemoveSpectrumFilename(parameters, parameter_name);
      value) {
    get_output()->set_sampled_spectrum_filename(*value);
    return;
  }
}

bool TryRemoveSpectrumTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name, bool allow_scale,
    absl::FunctionRef<SpectrumTextureParameter*()> get_output) {
  if (allow_scale) {
    if (std::optional<std::array<double, 2>> value =
            TryRemoveBlackbodyV1(parameters, parameter_name);
        value) {
      get_output()->mutable_blackbody_spectrum()->set_temperature((*value)[0]);
      get_output()->mutable_blackbody_spectrum()->set_scale((*value)[1]);
      return true;
    }
  } else {
    if (std::optional<double> value =
            TryRemoveBlackbodyV2(parameters, parameter_name);
        value) {
      get_output()->mutable_blackbody_spectrum()->set_temperature(*value);
      return true;
    }
  }

  if (std::optional<std::array<double, 3>> value =
          TryRemoveRgb(parameters, parameter_name);
      value) {
    get_output()->mutable_rgb_spectrum()->set_r((*value)[0]);
    get_output()->mutable_rgb_spectrum()->set_g((*value)[1]);
    get_output()->mutable_rgb_spectrum()->set_b((*value)[2]);
    return true;
  }

  if (std::optional<std::array<double, 3>> value =
          TryRemoveXyz(parameters, parameter_name);
      value) {
    get_output()->mutable_xyz_spectrum()->set_x((*value)[0]);
    get_output()->mutable_xyz_spectrum()->set_y((*value)[1]);
    get_output()->mutable_xyz_spectrum()->set_z((*value)[2]);
    return true;
  }

  if (std::optional<absl::Span<std::array<double, 2>>> values =
          TryRemoveSpectralSamples(parameters, parameter_name);
      values) {
    for (const auto [wavelength, intensity] : *values) {
      auto& sample = *get_output()->mutable_sampled_spectrum()->add_samples();
      sample.set_wavelength(wavelength);
      sample.set_intensity(intensity);
    }
    return true;
  }

  if (std::optional<absl::string_view> value =
          TryRemoveSpectrumFilename(parameters, parameter_name);
      value) {
    get_output()->set_sampled_spectrum_filename(*value);
    return true;
  }

  if (std::optional<absl::string_view> value =
          TryRemoveTexture(parameters, parameter_name);
      value) {
    get_output()->set_spectrum_texture_name(*value);
    return true;
  }

  return false;
}

}  // namespace

void TryRemoveSpectrumV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name,
    absl::FunctionRef<Spectrum*()> get_output) {
  TryRemoveSpectrum(parameters, parameter_name, /*allow_scale=*/true,
                    get_output);
}

void TryRemoveSpectrumV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name,
    absl::FunctionRef<Spectrum*()> get_output) {
  TryRemoveSpectrum(parameters, parameter_name, /*allow_scale=*/false,
                    get_output);
}

bool TryRemoveFloatTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name,
    absl::FunctionRef<FloatTextureParameter*()> get_output) {
  if (std::optional<double> value = TryRemoveFloat(parameters, parameter_name);
      value) {
    get_output()->set_float_value(*value);
    return true;
  }

  if (std::optional<absl::string_view> texture_name =
          TryRemoveTexture(parameters, parameter_name);
      texture_name) {
    get_output()->set_float_texture_name(*texture_name);
    return true;
  }

  return false;
}

bool TryRemoveSpectrumTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name,
    absl::FunctionRef<SpectrumTextureParameter*()> get_output) {
  return TryRemoveSpectrumTexture(parameters, parameter_name,
                                  /*allow_scale=*/true, get_output);
}

bool TryRemoveSpectrumTextureV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name,
    absl::FunctionRef<SpectrumTextureParameter*()> get_output) {
  return TryRemoveSpectrumTexture(parameters, parameter_name,
                                  /*allow_scale=*/false, get_output);
}

std::optional<CheckerboardAntialiasing> TryRemoveAaMode(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  if (std::optional<absl::string_view> aamode =
          TryRemoveString(parameters, "aamode");
      aamode) {
    if (*aamode == "closedform") {
      return CheckerboardAntialiasing::CLOSEDFORM;
    } else if (*aamode == "none") {
      return CheckerboardAntialiasing::NONE_AA;
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
