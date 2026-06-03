#include "pbrt_proto/shared/common.h"

#include <limits>

#include "absl/container/flat_hash_map.h"
#include "absl/functional/function_ref.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {
namespace {

absl::Status TryRemoveSpectrum(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name, bool allow_scale,
    absl::FunctionRef<Spectrum*()> get_output) {
  if (allow_scale) {
    if (std::optional<std::array<double, 2>> value =
            TryRemoveBlackbodyV1(parameters, parameter_name);
        value) {
      get_output()->mutable_blackbody_spectrum()->set_temperature((*value)[0]);
      get_output()->mutable_blackbody_spectrum()->set_scale((*value)[1]);
      return absl::OkStatus();
    }
  } else {
    if (std::optional<double> value =
            TryRemoveBlackbodyV2(parameters, parameter_name);
        value) {
      get_output()->mutable_blackbody_spectrum()->set_temperature(*value);
      return absl::OkStatus();
    }
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

const absl::flat_hash_map<absl::string_view, MeasuredScatteringPreset>
    kNamedMeasuredScatteringPresets = {
        {"Apple", MeasuredScatteringPreset::APPLE},
        {"Chicken1", MeasuredScatteringPreset::CHICKEN1},
        {"Chicken2", MeasuredScatteringPreset::CHICKEN2},
        {"Cream", MeasuredScatteringPreset::CREAM},
        {"Ketchup", MeasuredScatteringPreset::KETCHUP},
        {"Marble", MeasuredScatteringPreset::MARBLE},
        {"Potato", MeasuredScatteringPreset::POTATO},
        {"Skimmilk", MeasuredScatteringPreset::SKIMMILK},
        {"Skin1", MeasuredScatteringPreset::SKIN1},
        {"Skin2", MeasuredScatteringPreset::SKIN2},
        {"Spectralon", MeasuredScatteringPreset::SPECTRALON},
        {"Wholemilk", MeasuredScatteringPreset::WHOLEMILK},
        {"Lowfat Milk", MeasuredScatteringPreset::LOWFAT_MILK},
        {"Reduced Milk", MeasuredScatteringPreset::REDUCED_MILK},
        {"Regular Milk", MeasuredScatteringPreset::REGULAR_MILK},
        {"Espresso", MeasuredScatteringPreset::ESPRESSO},
        {"Mint Mocha Coffee", MeasuredScatteringPreset::MINT_MOCHA_COFFEE},
        {"Lowfat Soy Milk", MeasuredScatteringPreset::LOWFAT_SOY_MILK},
        {"Regular Soy Milk", MeasuredScatteringPreset::REGULAR_SOY_MILK},
        {"Lowfat Chocolate Milk",
         MeasuredScatteringPreset::LOWFAT_CHOCOLATE_MILK},
        {"Regular Chocolate Milk",
         MeasuredScatteringPreset::REGULAR_CHOCOLATE_MILK},
        {"Coke", MeasuredScatteringPreset::COKE},
        {"Pepsi", MeasuredScatteringPreset::PEPSI},
        {"Sprite", MeasuredScatteringPreset::SPRITE},
        {"Gatorade", MeasuredScatteringPreset::GATORADE},
        {"Chardonnay", MeasuredScatteringPreset::CHARDONNAY},
        {"White Zinfandel", MeasuredScatteringPreset::WHITE_ZINFANDEL},
        {"Merlot", MeasuredScatteringPreset::MERLOT},
        {"Budweiser Beer", MeasuredScatteringPreset::BUDWEISER_BEER},
        {"Coors Light Beer", MeasuredScatteringPreset::COORS_LIGHT_BEER},
        {"Clorox", MeasuredScatteringPreset::CLOROX},
        {"Apple Juice", MeasuredScatteringPreset::APPLE_JUICE},
        {"Cranberry Juice", MeasuredScatteringPreset::CRANBERRY_JUICE},
        {"Grape Juice", MeasuredScatteringPreset::GRAPE_JUICE},
        {"Ruby Grapefruit Juice",
         MeasuredScatteringPreset::RUBY_GRAPEFRUIT_JUICE},
        {"White Grapefruit Juice",
         MeasuredScatteringPreset::WHITE_GRAPEFRUIT_JUICE},
        {"Shampoo", MeasuredScatteringPreset::SHAMPOO},
        {"Strawberry Shampoo", MeasuredScatteringPreset::STRAWBERRY_SHAMPOO},
        {"Head & Shoulders Shampoo",
         MeasuredScatteringPreset::HEAD_AND_SHOULDERS_SHAMPOO},
        {"Lemon Tea Powder", MeasuredScatteringPreset::LEMON_TEA_POWDER},
        {"Orange Powder", MeasuredScatteringPreset::ORANGE_POWDER},
        {"Pink Lemonade Powder",
         MeasuredScatteringPreset::PINK_LEMONADE_POWDER},
        {"Cappuccino Powder", MeasuredScatteringPreset::CAPPUCCINO_POWDER},
        {"Salt Powder", MeasuredScatteringPreset::SALT_POWDER},
        {"Sugar Powder", MeasuredScatteringPreset::SUGAR_POWDER},
        {"Suisse Mocha Powder", MeasuredScatteringPreset::SUISSE_MOCHA_POWDER},
        {"Pacific Ocean Surface Water",
         MeasuredScatteringPreset::PACIFIC_OCEAN_SURFACE_WATER},
};

absl::Status TryRemoveSpectrumV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name,
    absl::FunctionRef<Spectrum*()> get_output) {
  return TryRemoveSpectrum(parameters, parameter_name, /*allow_scale=*/true,
                           get_output);
}

absl::Status TryRemoveSpectrumV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name,
    absl::FunctionRef<Spectrum*()> get_output) {
  return TryRemoveSpectrum(parameters, parameter_name, /*allow_scale=*/false,
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

}  // namespace pbrt_proto
