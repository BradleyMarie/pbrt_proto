#include "pbrt_proto/shared/common.h"

#include <limits>

#include "absl/container/flat_hash_map.h"
#include "absl/functional/function_ref.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {

const absl::flat_hash_map<absl::string_view, ScatteringPreset::Type>
    kNamedScatteringPresets = {
        {"Apple", ScatteringPreset::APPLE},
        {"Chicken1", ScatteringPreset::CHICKEN1},
        {"Chicken2", ScatteringPreset::CHICKEN2},
        {"Cream", ScatteringPreset::CREAM},
        {"Ketchup", ScatteringPreset::KETCHUP},
        {"Marble", ScatteringPreset::MARBLE},
        {"Potato", ScatteringPreset::POTATO},
        {"Skimmilk", ScatteringPreset::SKIMMILK},
        {"Skin1", ScatteringPreset::SKIN1},
        {"Skin2", ScatteringPreset::SKIN2},
        {"Spectralon", ScatteringPreset::SPECTRALON},
        {"Wholemilk", ScatteringPreset::WHOLEMILK},
        {"Lowfat Milk", ScatteringPreset::LOWFAT_MILK},
        {"Reduced Milk", ScatteringPreset::REDUCED_MILK},
        {"Regular Milk", ScatteringPreset::REGULAR_MILK},
        {"Espresso", ScatteringPreset::ESPRESSO},
        {"Mint Mocha Coffee", ScatteringPreset::MINT_MOCHA_COFFEE},
        {"Lowfat Soy Milk", ScatteringPreset::LOWFAT_SOY_MILK},
        {"Regular Soy Milk", ScatteringPreset::REGULAR_SOY_MILK},
        {"Lowfat Chocolate Milk", ScatteringPreset::LOWFAT_CHOCOLATE_MILK},
        {"Regular Chocolate Milk", ScatteringPreset::REGULAR_CHOCOLATE_MILK},
        {"Coke", ScatteringPreset::COKE},
        {"Pepsi", ScatteringPreset::PEPSI},
        {"Sprite", ScatteringPreset::SPRITE},
        {"Gatorade", ScatteringPreset::GATORADE},
        {"Chardonnay", ScatteringPreset::CHARDONNAY},
        {"White Zinfandel", ScatteringPreset::WHITE_ZINFANDEL},
        {"Merlot", ScatteringPreset::MERLOT},
        {"Budweiser Beer", ScatteringPreset::BUDWEISER_BEER},
        {"Coors Light Beer", ScatteringPreset::COORS_LIGHT_BEER},
        {"Clorox", ScatteringPreset::CLOROX},
        {"Apple Juice", ScatteringPreset::APPLE_JUICE},
        {"Cranberry Juice", ScatteringPreset::CRANBERRY_JUICE},
        {"Grape Juice", ScatteringPreset::GRAPE_JUICE},
        {"Ruby Grapefruit Juice", ScatteringPreset::RUBY_GRAPEFRUIT_JUICE},
        {"White Grapefruit Juice", ScatteringPreset::WHITE_GRAPEFRUIT_JUICE},
        {"Shampoo", ScatteringPreset::SHAMPOO},
        {"Strawberry Shampoo", ScatteringPreset::STRAWBERRY_SHAMPOO},
        {"Head & Shoulders Shampoo",
         ScatteringPreset::HEAD_AND_SHOULDERS_SHAMPOO},
        {"Lemon Tea Powder", ScatteringPreset::LEMON_TEA_POWDER},
        {"Orange Powder", ScatteringPreset::ORANGE_POWDER},
        {"Pink Lemonade Powder", ScatteringPreset::PINK_LEMONADE_POWDER},
        {"Cappuccino Powder", ScatteringPreset::CAPPUCCINO_POWDER},
        {"Salt Powder", ScatteringPreset::SALT_POWDER},
        {"Sugar Powder", ScatteringPreset::SUGAR_POWDER},
        {"Suisse Mocha Powder", ScatteringPreset::SUISSE_MOCHA_POWDER},
        {"Pacific Ocean Surface Water",
         ScatteringPreset::PACIFIC_OCEAN_SURFACE_WATER},
};

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
