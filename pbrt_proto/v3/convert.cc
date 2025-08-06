#include "pbrt_proto/v3/convert.h"

#include <array>
#include <functional>
#include <variant>

#include "absl/functional/function_ref.h"
#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "absl/types/span.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto::v3 {
namespace {

Integrator::LightSampleStrategy ParseLightSampleStrategy(
    absl::string_view lightsamplestrategy) {
  if (lightsamplestrategy == "power") {
    return Integrator::POWER;
  } else if (lightsamplestrategy == "uniform") {
    return Integrator::UNIFORM;
  }
  return Integrator::SPATIAL;
}

std::optional<ImageWrapping> TryRemoveImageWrapping(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  if (std::optional<absl::string_view> wrap =
          TryRemoveString(parameters, "wrap");
      wrap) {
    if (*wrap == "black") {
      return ImageWrapping::BLACK;
    } else if (*wrap == "clamp") {
      return ImageWrapping::CLAMP;
    } else if (*wrap == "repeat") {
      return ImageWrapping::REPEAT;
    }
  }

  return std::nullopt;
}

std::optional<AntialiasingMode> TryRemoveAntialiasingMode(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  if (std::optional<absl::string_view> aamode =
          TryRemoveString(parameters, "aamode");
      aamode) {
    if (*aamode == "closedform") {
      return AntialiasingMode::CLOSEDFORM;
    } else if (*aamode == "none") {
      return AntialiasingMode::NONE;
    }
  }

  return std::nullopt;
}

void TryRemoveSpectrum(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name,
    absl::FunctionRef<Spectrum*()> get_output) {
  if (std::optional<std::array<double, 2>> value =
          TryRemoveBlackbodyV1(parameters, parameter_name);
      value) {
    get_output()->mutable_blackbody_spectrum()->set_temperature((*value)[0]);
    get_output()->mutable_blackbody_spectrum()->set_scale((*value)[1]);
  } else if (std::optional<std::array<double, 3>> value =
                 TryRemoveRgb(parameters, parameter_name);
             value) {
    get_output()->mutable_rgb_spectrum()->set_r((*value)[0]);
    get_output()->mutable_rgb_spectrum()->set_g((*value)[1]);
    get_output()->mutable_rgb_spectrum()->set_b((*value)[2]);
  } else if (std::optional<std::array<double, 3>> value =
                 TryRemoveXyz(parameters, parameter_name);
             value) {
    get_output()->mutable_xyz_spectrum()->set_x((*value)[0]);
    get_output()->mutable_xyz_spectrum()->set_y((*value)[1]);
    get_output()->mutable_xyz_spectrum()->set_z((*value)[2]);
  } else if (std::optional<absl::Span<std::array<double, 2>>> values =
                 TryRemoveSpectralSamples(parameters, parameter_name);
             values) {
    for (const auto [wavelength, intensity] : *values) {
      auto& sample = *get_output()->mutable_sampled_spectrum()->add_samples();
      sample.set_wavelength(wavelength);
      sample.set_intensity(intensity);
    }
  } else if (std::optional<absl::string_view> value =
                 TryRemoveSpectrumFilename(parameters, parameter_name);
             value) {
    get_output()->set_sampled_spectrum_filename(*value);
  }
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

bool TryRemoveSpectrumTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name,
    absl::FunctionRef<SpectrumTextureParameter*()> get_output) {
  if (std::optional<std::array<double, 2>> value =
          TryRemoveBlackbodyV1(parameters, parameter_name);
      value) {
    get_output()->mutable_blackbody_spectrum()->set_temperature((*value)[0]);
    get_output()->mutable_blackbody_spectrum()->set_scale((*value)[1]);
    return true;
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

template <typename T>
void TryRemoveUVParameters(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters, T& output) {
  if (std::optional<absl::string_view> mapping =
          TryRemoveString(parameters, "mapping");
      mapping) {
    if (*mapping == "uv") {
      output.set_mapping(Mapping2D::UV);
    } else if (*mapping == "spherical") {
      output.set_mapping(Mapping2D::SPHERICAL);
    } else if (*mapping == "cylindrical") {
      output.set_mapping(Mapping2D::CYLINDRICAL);
    } else if (*mapping == "planar") {
      output.set_mapping(Mapping2D::PLANAR);
    }
  }

  if (std::optional<double> uscale = TryRemoveFloat(parameters, "uscale");
      uscale) {
    output.set_uscale(*uscale);
  }

  if (std::optional<double> vscale = TryRemoveFloat(parameters, "vscale");
      vscale) {
    output.set_vscale(*vscale);
  }

  if (std::optional<double> udelta = TryRemoveFloat(parameters, "udelta");
      udelta) {
    output.set_udelta(*udelta);
  }

  if (std::optional<double> vdelta = TryRemoveFloat(parameters, "vdelta");
      vdelta) {
    output.set_vdelta(*vdelta);
  }

  if (std::optional<std::array<double, 3>> v1 =
          TryRemoveVector3(parameters, "v1");
      v1) {
    output.mutable_v1()->set_x((*v1)[0]);
    output.mutable_v1()->set_y((*v1)[1]);
    output.mutable_v1()->set_z((*v1)[2]);
  }

  if (std::optional<std::array<double, 3>> v2 =
          TryRemoveVector3(parameters, "v2");
      v2) {
    output.mutable_v2()->set_x((*v2)[0]);
    output.mutable_v2()->set_y((*v2)[1]);
    output.mutable_v2()->set_z((*v2)[2]);
  }
}

static const absl::flat_hash_map<absl::string_view, MeasuredSubsurface>
    kSubsurfaces = {
        {"Apple", MeasuredSubsurface::APPLE},
        {"Chicken1", MeasuredSubsurface::CHICKEN1},
        {"Chicken2", MeasuredSubsurface::CHICKEN2},
        {"Cream", MeasuredSubsurface::CREAM},
        {"Ketchup", MeasuredSubsurface::KETCHUP},
        {"Marble", MeasuredSubsurface::MARBLE},
        {"Potato", MeasuredSubsurface::POTATO},
        {"Skimmilk", MeasuredSubsurface::SKIMMILK},
        {"Skin1", MeasuredSubsurface::SKIN1},
        {"Skin2", MeasuredSubsurface::SKIN2},
        {"Spectralon", MeasuredSubsurface::SPECTRALON},
        {"Wholemilk", MeasuredSubsurface::WHOLEMILK},
        {"Lowfat Milk", MeasuredSubsurface::LOWFAT_MILK},
        {"Reduced Milk", MeasuredSubsurface::REDUCED_MILK},
        {"Regular Milk", MeasuredSubsurface::REGULAR_MILK},
        {"Espresso", MeasuredSubsurface::ESPRESSO},
        {"Mint Mocha Coffee", MeasuredSubsurface::MINT_MOCHA_COFFEE},
        {"Lowfat Soy Milk", MeasuredSubsurface::LOWFAT_SOY_MILK},
        {"Regular Soy Milk", MeasuredSubsurface::REGULAR_SOY_MILK},
        {"Lowfat Chocolate Milk", MeasuredSubsurface::LOWFAT_CHOCOLATE_MILK},
        {"Regular Chocolate Milk", MeasuredSubsurface::REGULAR_CHOCOLATE_MILK},
        {"Coke", MeasuredSubsurface::COKE},
        {"Pepsi", MeasuredSubsurface::PEPSI},
        {"Sprite", MeasuredSubsurface::SPRITE},
        {"Gatorade", MeasuredSubsurface::GATORADE},
        {"Chardonnay", MeasuredSubsurface::CHARDONNAY},
        {"White Zinfandel", MeasuredSubsurface::WHITE_ZINFANDEL},
        {"Merlot", MeasuredSubsurface::MERLOT},
        {"Budweiser Beer", MeasuredSubsurface::BUDWEISER_BEER},
        {"Coors Light Beer", MeasuredSubsurface::COORS_LIGHT_BEER},
        {"Clorox", MeasuredSubsurface::CLOROX},
        {"Apple Juice", MeasuredSubsurface::APPLE_JUICE},
        {"Cranberry Juice", MeasuredSubsurface::CRANBERRY_JUICE},
        {"Grape Juice", MeasuredSubsurface::GRAPE_JUICE},
        {"Ruby Grapefruit Juice", MeasuredSubsurface::RUBY_GRAPEFRUIT_JUICE},
        {"White Grapefruit Juice", MeasuredSubsurface::WHITE_GRAPEFRUIT_JUICE},
        {"Shampoo", MeasuredSubsurface::SHAMPOO},
        {"Strawberry Shampoo", MeasuredSubsurface::STRAWBERRY_SHAMPOO},
        {"Head & Shoulders Shampoo",
         MeasuredSubsurface::HEAD_AND_SHOULDERS_SHAMPOO},
        {"Lemon Tea Powder", MeasuredSubsurface::LEMON_TEA_POWDER},
        {"Orange Powder", MeasuredSubsurface::ORANGE_POWDER},
        {"Pink Lemonade Powder", MeasuredSubsurface::PINK_LEMONADE_POWDER},
        {"Cappuccino Powder", MeasuredSubsurface::CAPPUCCINO_POWDER},
        {"Salt Powder", MeasuredSubsurface::SALT_POWDER},
        {"Sugar Powder", MeasuredSubsurface::SUGAR_POWDER},
        {"Suisse Mocha Powder", MeasuredSubsurface::SUISSE_MOCHA_POWDER},
        {"Pacific Ocean Surface Water",
         MeasuredSubsurface::PACIFIC_OCEAN_SURFACE_WATER},
};

Material ParseMaterial(
    absl::string_view material_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  Material material;

  if (material_type == "" || material_type == "none") {
    // Do Nothing
  } else if (material_type == "disney") {
    auto& disney = *material.mutable_disney();
    TryRemoveSpectrumTexture(
        parameters, "color",
        std::bind(&Material::Disney::mutable_color, &disney));
    TryRemoveFloatTexture(
        parameters, "anisotropic",
        std::bind(&Material::Disney::mutable_anisotropic, &disney));
    TryRemoveFloatTexture(
        parameters, "clearcoat",
        std::bind(&Material::Disney::mutable_clearcoat, &disney));
    TryRemoveFloatTexture(
        parameters, "clearcoatgloss",
        std::bind(&Material::Disney::mutable_clearcoatgloss, &disney));
    TryRemoveFloatTexture(parameters, "eta",
                          std::bind(&Material::Disney::mutable_eta, &disney));
    TryRemoveFloatTexture(
        parameters, "metallic",
        std::bind(&Material::Disney::mutable_metallic, &disney));
    TryRemoveFloatTexture(
        parameters, "roughness",
        std::bind(&Material::Disney::mutable_roughness, &disney));
    TryRemoveSpectrumTexture(
        parameters, "scatterdistance",
        std::bind(&Material::Disney::mutable_scatterdistance, &disney));
    TryRemoveFloatTexture(parameters, "sheen",
                          std::bind(&Material::Disney::mutable_sheen, &disney));
    TryRemoveFloatTexture(
        parameters, "sheentint",
        std::bind(&Material::Disney::mutable_sheentint, &disney));
    TryRemoveFloatTexture(
        parameters, "spectrans",
        std::bind(&Material::Disney::mutable_spectrans, &disney));
    TryRemoveFloatTexture(
        parameters, "speculartint",
        std::bind(&Material::Disney::mutable_speculartint, &disney));

    if (std::optional<bool> thin = TryRemoveBool(parameters, "thin"); thin) {
      disney.set_thin(*thin);
    }

    TryRemoveFloatTexture(
        parameters, "difftrans",
        std::bind(&Material::Disney::mutable_difftrans, &disney));
    TryRemoveFloatTexture(
        parameters, "flatness",
        std::bind(&Material::Disney::mutable_flatness, &disney));
    TryRemoveFloatTexture(
        parameters, "bumpmap",
        std::bind(&Material::Disney::mutable_bumpmap, &disney));
  } else if (material_type == "fourier") {
    auto& fourier = *material.mutable_fourier();

    if (std::optional<absl::string_view> bsdffile =
            TryRemoveString(parameters, "bsdffile");
        bsdffile) {
      fourier.set_bsdffile(*bsdffile);
    }

    TryRemoveFloatTexture(
        parameters, "bumpmap",
        std::bind(&Material::Fourier::mutable_bumpmap, &fourier));
  } else if (material_type == "glass") {
    auto& glass = *material.mutable_glass();
    TryRemoveSpectrumTexture(parameters, "Kr",
                             std::bind(&Material::Glass::mutable_kr, &glass));
    TryRemoveSpectrumTexture(parameters, "Kt",
                             std::bind(&Material::Glass::mutable_kt, &glass));
    TryRemoveFloatTexture(parameters, "eta",
                          std::bind(&Material::Glass::mutable_eta, &glass));

    if (!glass.has_eta()) {
      TryRemoveFloatTexture(parameters, "index",
                            std::bind(&Material::Glass::mutable_eta, &glass));
    }

    TryRemoveFloatTexture(
        parameters, "uroughness",
        std::bind(&Material::Glass::mutable_uroughness, &glass));
    TryRemoveFloatTexture(
        parameters, "vroughness",
        std::bind(&Material::Glass::mutable_vroughness, &glass));

    if (std::optional<bool> remaproughness =
            TryRemoveBool(parameters, "remaproughness");
        remaproughness) {
      glass.set_remaproughness(*remaproughness);
    }

    TryRemoveFloatTexture(parameters, "bumpmap",
                          std::bind(&Material::Glass::mutable_bumpmap, &glass));
  } else if (material_type == "hair") {
    auto& hair = *material.mutable_hair();
    TryRemoveSpectrumTexture(
        parameters, "sigma_a",
        std::bind(&Material::Hair::mutable_sigma_a, &hair));
    TryRemoveSpectrumTexture(parameters, "color",
                             std::bind(&Material::Hair::mutable_color, &hair));
    TryRemoveFloatTexture(parameters, "eumelanin",
                          std::bind(&Material::Hair::mutable_eumelanin, &hair));
    TryRemoveFloatTexture(
        parameters, "pheomelanin",
        std::bind(&Material::Hair::mutable_pheomelanin, &hair));
    TryRemoveFloatTexture(parameters, "eta",
                          std::bind(&Material::Hair::mutable_eta, &hair));
    TryRemoveFloatTexture(parameters, "beta_m",
                          std::bind(&Material::Hair::mutable_beta_m, &hair));
    TryRemoveFloatTexture(parameters, "beta_n",
                          std::bind(&Material::Hair::mutable_beta_n, &hair));
    TryRemoveFloatTexture(parameters, "alpha",
                          std::bind(&Material::Hair::mutable_alpha, &hair));
    TryRemoveFloatTexture(parameters, "bumpmap",
                          std::bind(&Material::Hair::mutable_bumpmap, &hair));
  } else if (material_type == "kdsubsurface") {
    auto& kdsubsurface = *material.mutable_kdsubsurface();
    TryRemoveSpectrumTexture(
        parameters, "Kd",
        std::bind(&Material::KdSubsurface::mutable_kd, &kdsubsurface));
    TryRemoveSpectrumTexture(
        parameters, "Kr",
        std::bind(&Material::KdSubsurface::mutable_kr, &kdsubsurface));
    TryRemoveSpectrumTexture(
        parameters, "Kt",
        std::bind(&Material::KdSubsurface::mutable_kt, &kdsubsurface));
    TryRemoveSpectrumTexture(
        parameters, "mfp",
        std::bind(&Material::KdSubsurface::mutable_mfp, &kdsubsurface));
    TryRemoveFloatTexture(
        parameters, "uroughness",
        std::bind(&Material::KdSubsurface::mutable_uroughness, &kdsubsurface));
    TryRemoveFloatTexture(
        parameters, "vroughness",
        std::bind(&Material::KdSubsurface::mutable_vroughness, &kdsubsurface));

    if (std::optional<double> eta = TryRemoveFloat(parameters, "eta"); eta) {
      kdsubsurface.set_eta(*eta);
    }

    if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
        scale) {
      kdsubsurface.set_scale(*scale);
    }

    if (std::optional<double> g = TryRemoveFloat(parameters, "g"); g) {
      kdsubsurface.set_g(*g);
    }

    if (std::optional<bool> remaproughness =
            TryRemoveBool(parameters, "remaproughness");
        remaproughness) {
      kdsubsurface.set_remaproughness(*remaproughness);
    }

    TryRemoveFloatTexture(
        parameters, "bumpmap",
        std::bind(&Material::KdSubsurface::mutable_bumpmap, &kdsubsurface));
  } else if (material_type == "metal") {
    auto& metal = *material.mutable_metal();
    TryRemoveSpectrumTexture(parameters, "eta",
                             std::bind(&Material::Metal::mutable_eta, &metal));
    TryRemoveSpectrumTexture(parameters, "k",
                             std::bind(&Material::Metal::mutable_k, &metal));
    TryRemoveFloatTexture(
        parameters, "roughness",
        std::bind(&Material::Metal::mutable_roughness, &metal));
    TryRemoveFloatTexture(
        parameters, "uroughness",
        std::bind(&Material::Metal::mutable_uroughness, &metal));
    TryRemoveFloatTexture(
        parameters, "vroughness",
        std::bind(&Material::Metal::mutable_vroughness, &metal));

    if (std::optional<bool> remaproughness =
            TryRemoveBool(parameters, "remaproughness");
        remaproughness) {
      metal.set_remaproughness(*remaproughness);
    }

    TryRemoveFloatTexture(parameters, "bumpmap",
                          std::bind(&Material::Metal::mutable_bumpmap, &metal));
  } else if (material_type == "mirror") {
    auto& mirror = *material.mutable_mirror();
    TryRemoveSpectrumTexture(parameters, "Kr",
                             std::bind(&Material::Mirror::mutable_kr, &mirror));
    TryRemoveFloatTexture(
        parameters, "bumpmap",
        std::bind(&Material::Mirror::mutable_bumpmap, &mirror));
  } else if (material_type == "mix") {
    auto& mix = *material.mutable_mix();
    TryRemoveFloatTexture(parameters, "sigma",
                          std::bind(&Material::Mix::mutable_sigma, &mix));
    TryRemoveFloatTexture(parameters, "bumpmap",
                          std::bind(&Material::Mix::mutable_bumpmap, &mix));
    TryRemoveSpectrumTexture(parameters, "amount",
                             std::bind(&Material::Mix::mutable_amount, &mix));
    TryRemoveSpectrumTexture(parameters, "Kd",
                             std::bind(&Material::Mix::mutable_kd, &mix));

    if (std::optional<absl::string_view> namedmaterial1 =
            TryRemoveString(parameters, "namedmaterial1");
        namedmaterial1) {
      mix.set_namedmaterial1(*namedmaterial1);
    }

    if (std::optional<absl::string_view> namedmaterial2 =
            TryRemoveString(parameters, "namedmaterial2");
        namedmaterial2) {
      mix.set_namedmaterial2(*namedmaterial2);
    }
  } else if (material_type == "plastic") {
    auto& plastic = *material.mutable_plastic();
    TryRemoveSpectrumTexture(
        parameters, "Kd", std::bind(&Material::Plastic::mutable_kd, &plastic));
    TryRemoveSpectrumTexture(
        parameters, "Ks", std::bind(&Material::Plastic::mutable_ks, &plastic));
    TryRemoveFloatTexture(
        parameters, "roughness",
        std::bind(&Material::Plastic::mutable_roughness, &plastic));

    if (std::optional<bool> remaproughness =
            TryRemoveBool(parameters, "remaproughness");
        remaproughness) {
      plastic.set_remaproughness(*remaproughness);
    }

    TryRemoveFloatTexture(
        parameters, "bumpmap",
        std::bind(&Material::Plastic::mutable_bumpmap, &plastic));
  } else if (material_type == "substrate") {
    auto& substrate = *material.mutable_substrate();
    TryRemoveSpectrumTexture(
        parameters, "Kd",
        std::bind(&Material::Substrate::mutable_kd, &substrate));
    TryRemoveSpectrumTexture(
        parameters, "Ks",
        std::bind(&Material::Substrate::mutable_ks, &substrate));
    TryRemoveFloatTexture(
        parameters, "uroughness",
        std::bind(&Material::Substrate::mutable_uroughness, &substrate));
    TryRemoveFloatTexture(
        parameters, "vroughness",
        std::bind(&Material::Substrate::mutable_vroughness, &substrate));

    if (std::optional<bool> remaproughness =
            TryRemoveBool(parameters, "remaproughness");
        remaproughness) {
      substrate.set_remaproughness(*remaproughness);
    }

    TryRemoveFloatTexture(
        parameters, "bumpmap",
        std::bind(&Material::Substrate::mutable_bumpmap, &substrate));
  } else if (material_type == "subsurface") {
    auto& subsurface = *material.mutable_subsurface();

    if (std::optional<absl::string_view> name =
            TryRemoveString(parameters, "name");
        name) {
      auto iter = kSubsurfaces.find(*name);
      if (iter != kSubsurfaces.end()) {
        subsurface.set_name(iter->second);
      }
    }

    TryRemoveSpectrumTexture(
        parameters, "sigma_a",
        std::bind(&Material::Subsurface::mutable_sigma_a, &subsurface));
    TryRemoveSpectrumTexture(
        parameters, "sigma_s",
        std::bind(&Material::Subsurface::mutable_sigma_s, &subsurface));

    if (std::optional<double> g = TryRemoveFloat(parameters, "g"); g) {
      subsurface.set_g(*g);
    }

    if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
        scale) {
      subsurface.set_scale(*scale);
    }

    if (std::optional<double> eta = TryRemoveFloat(parameters, "eta"); eta) {
      subsurface.set_eta(*eta);
    }

    TryRemoveSpectrumTexture(
        parameters, "Kr",
        std::bind(&Material::Subsurface::mutable_kr, &subsurface));
    TryRemoveSpectrumTexture(
        parameters, "Kt",
        std::bind(&Material::Subsurface::mutable_kt, &subsurface));
    TryRemoveFloatTexture(
        parameters, "uroughness",
        std::bind(&Material::Subsurface::mutable_uroughness, &subsurface));
    TryRemoveFloatTexture(
        parameters, "vroughness",
        std::bind(&Material::Subsurface::mutable_vroughness, &subsurface));

    if (std::optional<bool> remaproughness =
            TryRemoveBool(parameters, "remaproughness");
        remaproughness) {
      subsurface.set_remaproughness(*remaproughness);
    }

    TryRemoveFloatTexture(
        parameters, "bumpmap",
        std::bind(&Material::Subsurface::mutable_bumpmap, &subsurface));
  } else if (material_type == "translucent") {
    auto& translucent = *material.mutable_translucent();
    TryRemoveSpectrumTexture(
        parameters, "Kd",
        std::bind(&Material::Translucent::mutable_kd, &translucent));
    TryRemoveSpectrumTexture(
        parameters, "Ks",
        std::bind(&Material::Translucent::mutable_ks, &translucent));
    TryRemoveSpectrumTexture(
        parameters, "reflect",
        std::bind(&Material::Translucent::mutable_reflect, &translucent));
    TryRemoveSpectrumTexture(
        parameters, "transmit",
        std::bind(&Material::Translucent::mutable_transmit, &translucent));

    TryRemoveFloatTexture(
        parameters, "roughness",
        std::bind(&Material::Translucent::mutable_roughness, &translucent));

    if (std::optional<bool> remaproughness =
            TryRemoveBool(parameters, "remaproughness");
        remaproughness) {
      translucent.set_remaproughness(*remaproughness);
    }

    TryRemoveFloatTexture(
        parameters, "bumpmap",
        std::bind(&Material::Translucent::mutable_bumpmap, &translucent));
  } else if (material_type == "uber") {
    auto& uber = *material.mutable_uber();
    TryRemoveSpectrumTexture(parameters, "Kd",
                             std::bind(&Material::Uber::mutable_kd, &uber));
    TryRemoveSpectrumTexture(parameters, "Ks",
                             std::bind(&Material::Uber::mutable_ks, &uber));
    TryRemoveSpectrumTexture(parameters, "Kr",
                             std::bind(&Material::Uber::mutable_kr, &uber));
    TryRemoveSpectrumTexture(parameters, "Kt",
                             std::bind(&Material::Uber::mutable_kt, &uber));
    TryRemoveSpectrumTexture(
        parameters, "opacity",
        std::bind(&Material::Uber::mutable_opacity, &uber));

    TryRemoveFloatTexture(parameters, "roughness",
                          std::bind(&Material::Uber::mutable_roughness, &uber));
    TryRemoveFloatTexture(
        parameters, "uroughness",
        std::bind(&Material::Uber::mutable_uroughness, &uber));
    TryRemoveFloatTexture(
        parameters, "vroughness",
        std::bind(&Material::Uber::mutable_vroughness, &uber));

    TryRemoveFloatTexture(parameters, "eta",
                          std::bind(&Material::Uber::mutable_eta, &uber));
    if (!uber.has_eta()) {
      TryRemoveFloatTexture(parameters, "index",
                            std::bind(&Material::Uber::mutable_eta, &uber));
    }

    if (std::optional<bool> remaproughness =
            TryRemoveBool(parameters, "remaproughness");
        remaproughness) {
      uber.set_remaproughness(*remaproughness);
    }

    TryRemoveFloatTexture(parameters, "bumpmap",
                          std::bind(&Material::Uber::mutable_bumpmap, &uber));
  } else {
    if (material_type != "matte") {
      std::cerr << "Unrecognized Material type: \"" << material_type << "\""
                << std::endl;
    }

    auto& matte = *material.mutable_matte();
    TryRemoveSpectrumTexture(parameters, "Kd",
                             std::bind(&Material::Matte::mutable_kd, &matte));
    TryRemoveFloatTexture(parameters, "sigma",
                          std::bind(&Material::Matte::mutable_sigma, &matte));
    TryRemoveFloatTexture(parameters, "bumpmap",
                          std::bind(&Material::Matte::mutable_bumpmap, &matte));
  }

  return material;
}

static const absl::flat_hash_map<absl::string_view, ParameterType>
    parameter_type_names = {
        {
            "blackbody",
            ParameterType::BLACKBODY_V1,
        },
        {
            "bool",
            ParameterType::BOOL,
        },
        {
            "color",
            ParameterType::RGB,
        },
        {
            "float",
            ParameterType::FLOAT,
        },
        {
            "integer",
            ParameterType::INTEGER,
        },
        {
            "normal",
            ParameterType::NORMAL3,
        },
        {
            "normal3",
            ParameterType::NORMAL3,
        },
        {
            "point",
            ParameterType::POINT3,
        },
        {
            "point2",
            ParameterType::POINT2,
        },
        {
            "point3",
            ParameterType::POINT3,
        },
        {
            "rgb",
            ParameterType::RGB,
        },
        {
            "spectrum",
            ParameterType::SPECTRUM,
        },
        {"string", ParameterType::STRING},
        {"texture", ParameterType::TEXTURE},
        {
            "vector",
            ParameterType::VECTOR3,
        },
        {
            "vector2",
            ParameterType::VECTOR2,
        },
        {
            "vector3",
            ParameterType::VECTOR3,
        },
        {
            "xyz",
            ParameterType::XYZ,
        },
};

class ParserV3 final : public Parser {
 public:
  ParserV3(PbrtProto& output) noexcept
      : Parser(parameter_type_names), output_(output) {}

 private:
  absl::Status Accelerator(
      absl::string_view accelerator_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status ActiveTransform(ActiveTransformation active) override;

  absl::Status AreaLightSource(
      absl::string_view area_light_source_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status AttributeBegin() override;

  absl::Status AttributeEnd() override;

  absl::Status Camera(
      absl::string_view camera_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status ConcatTransform(double m00, double m01, double m02, double m03,
                               double m10, double m11, double m12, double m13,
                               double m20, double m21, double m22, double m23,
                               double m30, double m31, double m32,
                               double m33) override;

  absl::Status CoordinateSystem(absl::string_view name) override;

  absl::Status CoordSysTransform(absl::string_view name) override;

  absl::Status Film(
      absl::string_view film_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status FloatTexture(
      absl::string_view float_texture_name,
      absl::string_view float_texture_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status Identity() override;

  absl::Status Include(absl::string_view path) override;

  absl::Status Integrator(
      absl::string_view integrator_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status Import(absl::string_view path) override;

  absl::Status LightSource(
      absl::string_view light_source_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status LookAt(double eye_x, double eye_y, double eye_z, double look_x,
                      double look_y, double look_z, double up_x, double up_y,
                      double up_z) override;

  absl::Status MakeNamedMaterial(
      absl::string_view material_name,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status MakeNamedMedium(
      absl::string_view medium_name,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status Material(
      absl::string_view material_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status MediumInterface(absl::string_view inside,
                               absl::string_view outside) override;

  absl::Status NamedMaterial(absl::string_view material) override;

  absl::Status ObjectBegin(absl::string_view path) override;

  absl::Status ObjectEnd() override;

  absl::Status ObjectInstance(absl::string_view path) override;

  absl::Status PixelFilter(
      absl::string_view filter_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status ReverseOrientation() override;

  absl::Status Rotate(double angle, double x, double y, double z) override;

  absl::Status Sampler(
      absl::string_view sampler_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status Scale(double x, double y, double z) override;

  absl::Status Shape(
      absl::string_view shape_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status SpectrumTexture(
      absl::string_view spectrum_texture_name,
      absl::string_view spectrum_texture_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status Transform(double m00, double m01, double m02, double m03,
                         double m10, double m11, double m12, double m13,
                         double m20, double m21, double m22, double m23,
                         double m30, double m31, double m32,
                         double m33) override;

  absl::Status TransformTimes(double start_time, double end_time) override;

  absl::Status TransformBegin() override;

  absl::Status TransformEnd() override;

  absl::Status Translate(double x, double y, double z) override;

  absl::Status WorldBegin() override;

  absl::Status WorldEnd() override;

  PbrtProto& output_;
};

absl::Status ParserV3::Accelerator(
    absl::string_view accelerator_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& accelerator = *output_.add_directives()->mutable_accelerator();

  if (accelerator_type == "bvh") {
    auto& bvh = *accelerator.mutable_bvh();

    if (std::optional<int32_t> maxnodeprims =
            TryRemoveInteger(parameters, "maxnodeprims");
        maxnodeprims.has_value()) {
      bvh.set_maxnodeprims(*maxnodeprims);
    }

    if (std::optional<absl::string_view> splitmethod =
            TryRemoveString(parameters, "splitmethod");
        splitmethod.has_value()) {
      if (*splitmethod == "sah") {
        bvh.set_splitmethod(Accelerator::Bvh::SAH);
      } else if (*splitmethod == "middle") {
        bvh.set_splitmethod(Accelerator::Bvh::MIDDLE);
      } else if (*splitmethod == "equal") {
        bvh.set_splitmethod(Accelerator::Bvh::EQUAL);
      } else if (*splitmethod == "hlbvh") {
        bvh.set_splitmethod(Accelerator::Bvh::HLBVH);
      }
    }
  } else if (accelerator_type == "kdtree") {
    auto& kdtree = *accelerator.mutable_kdtree();

    if (std::optional<double> emptybonus =
            TryRemoveFloat(parameters, "emptybonus");
        emptybonus.has_value()) {
      kdtree.set_emptybonus(*emptybonus);
    }

    if (std::optional<int32_t> intersectcost =
            TryRemoveInteger(parameters, "intersectcost");
        intersectcost.has_value()) {
      kdtree.set_intersectcost(*intersectcost);
    }

    if (std::optional<int32_t> maxdepth =
            TryRemoveInteger(parameters, "maxdepth");
        maxdepth.has_value()) {
      kdtree.set_maxdepth(*maxdepth);
    }

    if (std::optional<int32_t> maxprims =
            TryRemoveInteger(parameters, "maxprims");
        maxprims.has_value()) {
      kdtree.set_maxprims(*maxprims);
    }

    if (std::optional<int32_t> traversalcost =
            TryRemoveInteger(parameters, "traversalcost");
        traversalcost.has_value()) {
      kdtree.set_traversalcost(*traversalcost);
    }
  } else {
    std::cerr << "Unrecognized Accelerator type: \"" << accelerator_type << "\""
              << std::endl;
  }

  return absl::OkStatus();
}

absl::Status ParserV3::ActiveTransform(ActiveTransformation active) {
  auto& active_transform =
      *output_.add_directives()->mutable_active_transform();
  switch (active) {
    case ActiveTransformation::ALL:
      active_transform.set_active(ActiveTransform::ALL);
      break;
    case ActiveTransformation::START_TIME:
      active_transform.set_active(ActiveTransform::START_TIME);
      break;
    case ActiveTransformation::END_TIME:
      active_transform.set_active(ActiveTransform::END_TIME);
      break;
  }
  return absl::OkStatus();
}

absl::Status ParserV3::AreaLightSource(
    absl::string_view area_light_source_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& area_light_source =
      *output_.add_directives()->mutable_area_light_source();

  if (area_light_source_type == "diffuse") {
    auto& diffuse = *area_light_source.mutable_diffuse();

    TryRemoveSpectrum(
        parameters, "L",
        std::bind(&AreaLightSource::Diffuse::mutable_l, &diffuse));

    if (std::optional<int32_t> samples =
            TryRemoveInteger(parameters, "nsamples");
        samples) {
      diffuse.set_samples(*samples);
    }

    if (std::optional<int32_t> samples =
            TryRemoveInteger(parameters, "samples");
        samples) {
      diffuse.set_samples(*samples);
    }

    if (std::optional<bool> twosided = TryRemoveBool(parameters, "twosided");
        twosided) {
      diffuse.set_twosided(*twosided);
    }

    TryRemoveSpectrum(
        parameters, "scale",
        std::bind(&AreaLightSource::Diffuse::mutable_scale, &diffuse));
  } else {
    std::cerr << "Unrecognized AreaLightSource type: \""
              << area_light_source_type << "\"" << std::endl;
  }

  return absl::OkStatus();
}

absl::Status ParserV3::AttributeBegin() {
  output_.add_directives()->mutable_attribute_begin();
  return absl::OkStatus();
}

absl::Status ParserV3::AttributeEnd() {
  output_.add_directives()->mutable_attribute_end();
  return absl::OkStatus();
}

absl::Status ParserV3::Camera(
    absl::string_view camera_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& camera = *output_.add_directives()->mutable_camera();

  if (camera_type == "environment") {
    auto& environment = *camera.mutable_environment();

    if (std::optional<double> lensradius =
            TryRemoveFloat(parameters, "lensradius");
        lensradius.has_value()) {
      environment.set_lensradius(*lensradius);
    }

    if (std::optional<double> focaldistance =
            TryRemoveFloat(parameters, "focaldistance");
        focaldistance.has_value()) {
      environment.set_focaldistance(*focaldistance);
    }

    if (std::optional<double> frameaspectratio =
            TryRemoveFloat(parameters, "frameaspectratio");
        frameaspectratio.has_value()) {
      environment.set_frameaspectratio(*frameaspectratio);
    }

    std::optional<absl::Span<double>> screenwindow;
    if (absl::Status status =
            TryRemoveFloats(parameters, "screenwindow", 4, screenwindow);
        status.ok() && screenwindow.has_value()) {
      environment.mutable_screenwindow()->set_x_min((*screenwindow)[0]);
      environment.mutable_screenwindow()->set_x_max((*screenwindow)[1]);
      environment.mutable_screenwindow()->set_y_min((*screenwindow)[2]);
      environment.mutable_screenwindow()->set_y_max((*screenwindow)[3]);
    }

    if (std::optional<double> shutteropen =
            TryRemoveFloat(parameters, "shutteropen");
        shutteropen.has_value()) {
      environment.set_shutteropen(*shutteropen);
    }

    if (std::optional<double> shutterclose =
            TryRemoveFloat(parameters, "shutterclose");
        shutterclose.has_value()) {
      environment.set_shutterclose(*shutterclose);
    }
  } else if (camera_type == "orthographic") {
    auto& orthographic = *camera.mutable_orthographic();

    if (std::optional<double> lensradius =
            TryRemoveFloat(parameters, "lensradius");
        lensradius.has_value()) {
      orthographic.set_lensradius(*lensradius);
    }

    if (std::optional<double> focaldistance =
            TryRemoveFloat(parameters, "focaldistance");
        focaldistance.has_value()) {
      orthographic.set_focaldistance(*focaldistance);
    }

    if (std::optional<double> frameaspectratio =
            TryRemoveFloat(parameters, "frameaspectratio");
        frameaspectratio.has_value()) {
      orthographic.set_frameaspectratio(*frameaspectratio);
    }

    std::optional<absl::Span<double>> screenwindow;
    if (absl::Status status =
            TryRemoveFloats(parameters, "screenwindow", 4, screenwindow);
        status.ok() && screenwindow.has_value()) {
      orthographic.mutable_screenwindow()->set_x_min((*screenwindow)[0]);
      orthographic.mutable_screenwindow()->set_x_max((*screenwindow)[1]);
      orthographic.mutable_screenwindow()->set_y_min((*screenwindow)[2]);
      orthographic.mutable_screenwindow()->set_y_max((*screenwindow)[3]);
    }

    if (std::optional<double> shutteropen =
            TryRemoveFloat(parameters, "shutteropen");
        shutteropen.has_value()) {
      orthographic.set_shutteropen(*shutteropen);
    }

    if (std::optional<double> shutterclose =
            TryRemoveFloat(parameters, "shutterclose");
        shutterclose.has_value()) {
      orthographic.set_shutterclose(*shutterclose);
    }
  } else if (camera_type == "perspective") {
    auto& perspective = *camera.mutable_perspective();

    if (std::optional<double> lensradius =
            TryRemoveFloat(parameters, "lensradius");
        lensradius.has_value()) {
      perspective.set_lensradius(*lensradius);
    }

    if (std::optional<double> focaldistance =
            TryRemoveFloat(parameters, "focaldistance");
        focaldistance.has_value()) {
      perspective.set_focaldistance(*focaldistance);
    }

    if (std::optional<double> frameaspectratio =
            TryRemoveFloat(parameters, "frameaspectratio");
        frameaspectratio.has_value()) {
      perspective.set_frameaspectratio(*frameaspectratio);
    }

    std::optional<absl::Span<double>> screenwindow;
    if (absl::Status status =
            TryRemoveFloats(parameters, "screenwindow", 4, screenwindow);
        status.ok() && screenwindow.has_value()) {
      perspective.mutable_screenwindow()->set_x_min((*screenwindow)[0]);
      perspective.mutable_screenwindow()->set_x_max((*screenwindow)[1]);
      perspective.mutable_screenwindow()->set_y_min((*screenwindow)[2]);
      perspective.mutable_screenwindow()->set_y_max((*screenwindow)[3]);
    }

    if (std::optional<double> fov = TryRemoveFloat(parameters, "fov");
        fov.has_value()) {
      perspective.set_fov(*fov);
    }

    if (std::optional<double> halffov = TryRemoveFloat(parameters, "halffov");
        halffov.has_value()) {
      perspective.set_halffov(*halffov);
    }

    if (std::optional<double> shutteropen =
            TryRemoveFloat(parameters, "shutteropen");
        shutteropen.has_value()) {
      perspective.set_shutteropen(*shutteropen);
    }

    if (std::optional<double> shutterclose =
            TryRemoveFloat(parameters, "shutterclose");
        shutterclose.has_value()) {
      perspective.set_shutterclose(*shutterclose);
    }
  } else if (camera_type == "realistic") {
    auto& realistic = *camera.mutable_realistic();

    if (std::optional<absl::string_view> lensfile =
            TryRemoveString(parameters, "lensfile");
        lensfile.has_value()) {
      realistic.set_lensfile(*lensfile);
    }

    if (std::optional<double> aperturediameter =
            TryRemoveFloat(parameters, "aperturediameter");
        aperturediameter.has_value()) {
      realistic.set_aperturediameter(*aperturediameter);
    }

    if (std::optional<double> focusdistance =
            TryRemoveFloat(parameters, "focusdistance");
        focusdistance.has_value()) {
      realistic.set_focusdistance(*focusdistance);
    }

    if (std::optional<bool> simpleweighting =
            TryRemoveBool(parameters, "simpleweighting");
        simpleweighting.has_value()) {
      realistic.set_simpleweighting(*simpleweighting);
    }

    if (std::optional<double> shutteropen =
            TryRemoveFloat(parameters, "shutteropen");
        shutteropen.has_value()) {
      realistic.set_shutteropen(*shutteropen);
    }

    if (std::optional<double> shutterclose =
            TryRemoveFloat(parameters, "shutterclose");
        shutterclose.has_value()) {
      realistic.set_shutterclose(*shutterclose);
    }
  } else {
    std::cerr << "Unrecognized Camera type: \"" << camera_type << "\""
              << std::endl;
  }

  return absl::OkStatus();
}

absl::Status ParserV3::ConcatTransform(double m00, double m01, double m02,
                                       double m03, double m10, double m11,
                                       double m12, double m13, double m20,
                                       double m21, double m22, double m23,
                                       double m30, double m31, double m32,
                                       double m33) {
  auto& concat_transform =
      *output_.add_directives()->mutable_concat_transform();
  concat_transform.set_m00(m00);
  concat_transform.set_m01(m01);
  concat_transform.set_m02(m02);
  concat_transform.set_m03(m03);
  concat_transform.set_m10(m10);
  concat_transform.set_m11(m11);
  concat_transform.set_m12(m12);
  concat_transform.set_m13(m13);
  concat_transform.set_m20(m20);
  concat_transform.set_m21(m21);
  concat_transform.set_m22(m22);
  concat_transform.set_m23(m23);
  concat_transform.set_m30(m30);
  concat_transform.set_m31(m31);
  concat_transform.set_m32(m32);
  concat_transform.set_m33(m33);
  return absl::OkStatus();
}

absl::Status ParserV3::CoordinateSystem(absl::string_view name) {
  output_.add_directives()->mutable_coordinate_system()->set_name(name);
  return absl::OkStatus();
}

absl::Status ParserV3::CoordSysTransform(absl::string_view name) {
  output_.add_directives()->mutable_coord_sys_transform()->set_name(name);
  return absl::OkStatus();
}

absl::Status ParserV3::Film(
    absl::string_view film_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& film = *output_.add_directives()->mutable_film();

  if (film_type == "image") {
    auto& image = *film.mutable_image();

    if (std::optional<int32_t> xresolution =
            TryRemoveInteger(parameters, "xresolution");
        xresolution.has_value()) {
      image.set_xresolution(*xresolution);
    }

    if (std::optional<int32_t> yresolution =
            TryRemoveInteger(parameters, "yresolution");
        yresolution.has_value()) {
      image.set_yresolution(*yresolution);
    }

    std::optional<absl::Span<double>> cropwindow;
    if (absl::Status status =
            TryRemoveFloats(parameters, "cropwindow", 4, cropwindow);
        !status.ok()) {
      return status;
    } else if (cropwindow.has_value()) {
      image.mutable_cropwindow()->set_x_min((*cropwindow)[0]);
      image.mutable_cropwindow()->set_x_max((*cropwindow)[1]);
      image.mutable_cropwindow()->set_y_min((*cropwindow)[2]);
      image.mutable_cropwindow()->set_y_max((*cropwindow)[3]);
    }

    if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
        scale.has_value()) {
      image.set_scale(*scale);
    }

    if (std::optional<double> maxsampleluminance =
            TryRemoveFloat(parameters, "maxsampleluminance");
        maxsampleluminance.has_value()) {
      image.set_maxsampleluminance(*maxsampleluminance);
    }

    if (std::optional<double> diagonal = TryRemoveFloat(parameters, "diagonal");
        diagonal.has_value()) {
      image.set_diagonal(*diagonal);
    }

    if (std::optional<absl::string_view> filename =
            TryRemoveString(parameters, "filename");
        filename.has_value()) {
      image.set_filename(*filename);
    }
  } else {
    std::cerr << "Unrecognized Film type: \"" << film_type << "\"" << std::endl;
  }

  return absl::OkStatus();
}

absl::Status ParserV3::FloatTexture(
    absl::string_view float_texture_name, absl::string_view float_texture_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& float_texture = *output_.add_directives()->mutable_float_texture();

  float_texture.set_name(float_texture_name);

  if (float_texture_type == "bilerp") {
    auto& bilerp = *float_texture.mutable_bilerp();

    TryRemoveFloatTexture(
        parameters, "v00",
        std::bind(&FloatTexture::Bilerp::mutable_v00, &bilerp));
    TryRemoveFloatTexture(
        parameters, "v01",
        std::bind(&FloatTexture::Bilerp::mutable_v01, &bilerp));
    TryRemoveFloatTexture(
        parameters, "v10",
        std::bind(&FloatTexture::Bilerp::mutable_v10, &bilerp));
    TryRemoveFloatTexture(
        parameters, "v11",
        std::bind(&FloatTexture::Bilerp::mutable_v11, &bilerp));
    TryRemoveUVParameters(parameters, bilerp);
  } else if (float_texture_type == "checkerboard") {
    int32_t dimensions = TryRemoveInteger(parameters, "dimensions").value_or(2);
    if (dimensions == 2) {
      auto& checkerboard = *float_texture.mutable_checkerboard2d();

      if (std::optional<AntialiasingMode> aamode =
              TryRemoveAntialiasingMode(parameters);
          aamode) {
        checkerboard.set_aamode(*aamode);
      }

      TryRemoveFloatTexture(
          parameters, "tex1",
          std::bind(&FloatTexture::Checkerboard2D::mutable_tex1,
                    &checkerboard));
      TryRemoveFloatTexture(
          parameters, "tex2",
          std::bind(&FloatTexture::Checkerboard2D::mutable_tex2,
                    &checkerboard));
      TryRemoveUVParameters(parameters, checkerboard);
    } else {
      auto& checkerboard = *float_texture.mutable_checkerboard3d();

      TryRemoveFloatTexture(
          parameters, "tex1",
          std::bind(&FloatTexture::Checkerboard3D::mutable_tex1,
                    &checkerboard));
      TryRemoveFloatTexture(
          parameters, "tex2",
          std::bind(&FloatTexture::Checkerboard3D::mutable_tex2,
                    &checkerboard));
    }
  } else if (float_texture_type == "constant") {
    auto& constant = *float_texture.mutable_constant();

    if (std::optional<double> value = TryRemoveFloat(parameters, "value");
        value) {
      constant.set_value(*value);
    }
  } else if (float_texture_type == "dots") {
    auto& dots = *float_texture.mutable_dots();

    TryRemoveFloatTexture(
        parameters, "inside",
        std::bind(&FloatTexture::Dots::mutable_inside, &dots));
    TryRemoveFloatTexture(
        parameters, "outside",
        std::bind(&FloatTexture::Dots::mutable_outside, &dots));
    TryRemoveUVParameters(parameters, dots);
  } else if (float_texture_type == "fbm") {
    auto& fbm = *float_texture.mutable_fbm();

    if (std::optional<int32_t> octaves =
            TryRemoveInteger(parameters, "octaves");
        octaves) {
      fbm.set_octaves(*octaves);
    }

    if (std::optional<double> roughness =
            TryRemoveFloat(parameters, "roughness");
        roughness) {
      fbm.set_roughness(*roughness);
    }
  } else if (float_texture_type == "imagemap") {
    auto& imagemap = *float_texture.mutable_imagemap();

    if (std::optional<bool> gamma = TryRemoveBool(parameters, "gamma"); gamma) {
      imagemap.set_gamma(*gamma);
    }

    if (std::optional<absl::string_view> filename =
            TryRemoveString(parameters, "filename");
        filename) {
      imagemap.set_filename(*filename);
    }

    if (std::optional<double> maxanisotropy =
            TryRemoveFloat(parameters, "maxanisotropy");
        maxanisotropy) {
      imagemap.set_maxanisotropy(*maxanisotropy);
    }

    if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
        scale) {
      imagemap.set_scale(*scale);
    }

    if (std::optional<bool> trilinear = TryRemoveBool(parameters, "trilinear");
        trilinear) {
      imagemap.set_trilinear(*trilinear);
    }

    if (std::optional<ImageWrapping> wrap = TryRemoveImageWrapping(parameters);
        wrap) {
      imagemap.set_wrap(*wrap);
    }

    TryRemoveUVParameters(parameters, imagemap);
  } else if (float_texture_type == "marble") {
    float_texture.mutable_marble();
  } else if (float_texture_type == "mix") {
    auto& mix = *float_texture.mutable_mix();

    TryRemoveFloatTexture(parameters, "amount",
                          std::bind(&FloatTexture::Mix::mutable_amount, &mix));
    TryRemoveFloatTexture(parameters, "tex1",
                          std::bind(&FloatTexture::Mix::mutable_tex1, &mix));
    TryRemoveFloatTexture(parameters, "tex2",
                          std::bind(&FloatTexture::Mix::mutable_tex2, &mix));
  } else if (float_texture_type == "ptex") {
    auto& ptex = *float_texture.mutable_ptex();

    if (std::optional<absl::string_view> filename =
            TryRemoveString(parameters, "filename");
        filename) {
      ptex.set_filename(*filename);
    }

    if (std::optional<double> gamma = TryRemoveFloat(parameters, "gamma");
        gamma) {
      ptex.set_gamma(*gamma);
    }
  } else if (float_texture_type == "scale") {
    auto& scale = *float_texture.mutable_scale();

    TryRemoveFloatTexture(
        parameters, "tex1",
        std::bind(&FloatTexture::Scale::mutable_tex1, &scale));
    TryRemoveFloatTexture(
        parameters, "tex2",
        std::bind(&FloatTexture::Scale::mutable_tex2, &scale));
  } else if (float_texture_type == "windy") {
    float_texture.mutable_windy();
  } else if (float_texture_type == "wrinkled") {
    auto& wrinkled = *float_texture.mutable_wrinkled();

    if (std::optional<int32_t> octaves =
            TryRemoveInteger(parameters, "octaves");
        octaves) {
      wrinkled.set_octaves(*octaves);
    }

    if (std::optional<double> roughness =
            TryRemoveFloat(parameters, "roughness");
        roughness) {
      wrinkled.set_roughness(*roughness);
    }
  } else {
    std::cerr << "Unrecognized Texture type: \"" << float_texture_type << "\""
              << std::endl;
  }

  return absl::OkStatus();
}

absl::Status ParserV3::Identity() {
  output_.add_directives()->mutable_identity();
  return absl::OkStatus();
}

absl::Status ParserV3::Include(absl::string_view path) {
  output_.add_directives()->mutable_include()->set_path(path);
  return absl::OkStatus();
}

absl::Status ParserV3::Integrator(
    absl::string_view integrator_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& integrator = *output_.add_directives()->mutable_integrator();

  if (integrator_type == "ambientocclusion") {
    auto& ambientocclusion = *integrator.mutable_ambientocclusion();

    if (std::optional<bool> cossample = TryRemoveBool(parameters, "cossample");
        cossample.has_value()) {
      ambientocclusion.set_cossample(*cossample);
    }

    if (std::optional<int32_t> nsamples =
            TryRemoveInteger(parameters, "nsamples");
        nsamples.has_value()) {
      ambientocclusion.set_nsamples(*nsamples);
    }

    std::optional<absl::Span<int32_t>> pixelbounds;
    if (absl::Status status =
            TryRemoveIntegers(parameters, "pixelbounds", 4, pixelbounds);
        status.ok() && pixelbounds.has_value()) {
      ambientocclusion.mutable_pixelbounds()->set_x_min((*pixelbounds)[0]);
      ambientocclusion.mutable_pixelbounds()->set_x_max((*pixelbounds)[1]);
      ambientocclusion.mutable_pixelbounds()->set_y_min((*pixelbounds)[2]);
      ambientocclusion.mutable_pixelbounds()->set_y_max((*pixelbounds)[3]);
    }
  } else if (integrator_type == "bdpt") {
    auto& bdpt = *integrator.mutable_bdpt();

    if (std::optional<int32_t> maxdepth =
            TryRemoveInteger(parameters, "maxdepth");
        maxdepth.has_value()) {
      bdpt.set_maxdepth(*maxdepth);
    }

    if (std::optional<bool> visualizestrategies =
            TryRemoveBool(parameters, "visualizestrategies");
        visualizestrategies.has_value()) {
      bdpt.set_visualizestrategies(*visualizestrategies);
    }

    if (std::optional<bool> visualizeweights =
            TryRemoveBool(parameters, "visualizeweights");
        visualizeweights.has_value()) {
      bdpt.set_visualizeweights(*visualizeweights);
    }

    if (std::optional<absl::string_view> lightsamplestrategy =
            TryRemoveString(parameters, "lightsamplestrategy");
        lightsamplestrategy.has_value()) {
      bdpt.set_lightsamplestrategy(
          ParseLightSampleStrategy(*lightsamplestrategy));
    }

    std::optional<absl::Span<int32_t>> pixelbounds;
    if (absl::Status status =
            TryRemoveIntegers(parameters, "pixelbounds", 4, pixelbounds);
        status.ok() && pixelbounds.has_value()) {
      bdpt.mutable_pixelbounds()->set_x_min((*pixelbounds)[0]);
      bdpt.mutable_pixelbounds()->set_x_max((*pixelbounds)[1]);
      bdpt.mutable_pixelbounds()->set_y_min((*pixelbounds)[2]);
      bdpt.mutable_pixelbounds()->set_y_max((*pixelbounds)[3]);
    }
  } else if (integrator_type == "directlighting") {
    auto& directlighting = *integrator.mutable_directlighting();

    if (std::optional<int32_t> maxdepth =
            TryRemoveInteger(parameters, "maxdepth");
        maxdepth.has_value()) {
      directlighting.set_maxdepth(*maxdepth);
    }

    if (std::optional<absl::string_view> strategy =
            TryRemoveString(parameters, "strategy");
        strategy.has_value()) {
      if (*strategy == "one") {
        directlighting.set_strategy(Integrator::DirectLighting::ONE);
      } else {
        directlighting.set_strategy(Integrator::DirectLighting::ALL);
      }
    }

    std::optional<absl::Span<int32_t>> pixelbounds;
    if (absl::Status status =
            TryRemoveIntegers(parameters, "pixelbounds", 4, pixelbounds);
        status.ok() && pixelbounds.has_value()) {
      directlighting.mutable_pixelbounds()->set_x_min((*pixelbounds)[0]);
      directlighting.mutable_pixelbounds()->set_x_max((*pixelbounds)[1]);
      directlighting.mutable_pixelbounds()->set_y_min((*pixelbounds)[2]);
      directlighting.mutable_pixelbounds()->set_y_max((*pixelbounds)[3]);
    }
  } else if (integrator_type == "mlt") {
    auto& mlt = *integrator.mutable_mlt();

    if (std::optional<int32_t> maxdepth =
            TryRemoveInteger(parameters, "maxdepth");
        maxdepth.has_value()) {
      mlt.set_maxdepth(*maxdepth);
    }

    if (std::optional<int32_t> bootstrapsamples =
            TryRemoveInteger(parameters, "bootstrapsamples");
        bootstrapsamples.has_value()) {
      mlt.set_bootstrapsamples(*bootstrapsamples);
    }

    if (std::optional<int32_t> chains = TryRemoveInteger(parameters, "chains");
        chains.has_value()) {
      mlt.set_chains(*chains);
    }

    if (std::optional<int32_t> mutationsperpixel =
            TryRemoveInteger(parameters, "mutationsperpixel");
        mutationsperpixel.has_value()) {
      mlt.set_mutationsperpixel(*mutationsperpixel);
    }

    if (std::optional<double> largestepprobability =
            TryRemoveFloat(parameters, "largestepprobability");
        largestepprobability.has_value()) {
      mlt.set_largestepprobability(*largestepprobability);
    }

    if (std::optional<double> sigma = TryRemoveFloat(parameters, "sigma");
        sigma.has_value()) {
      mlt.set_sigma(*sigma);
    }
  } else if (integrator_type == "path") {
    auto& path = *integrator.mutable_path();

    if (std::optional<int32_t> maxdepth =
            TryRemoveInteger(parameters, "maxdepth");
        maxdepth.has_value()) {
      path.set_maxdepth(*maxdepth);
    }

    if (std::optional<double> rrthreshold =
            TryRemoveFloat(parameters, "rrthreshold");
        rrthreshold.has_value()) {
      path.set_rrthreshold(*rrthreshold);
    }

    if (std::optional<absl::string_view> lightsamplestrategy =
            TryRemoveString(parameters, "lightsamplestrategy");
        lightsamplestrategy.has_value()) {
      path.set_lightsamplestrategy(
          ParseLightSampleStrategy(*lightsamplestrategy));
    }

    std::optional<absl::Span<int32_t>> pixelbounds;
    if (absl::Status status =
            TryRemoveIntegers(parameters, "pixelbounds", 4, pixelbounds);
        status.ok() && pixelbounds.has_value()) {
      path.mutable_pixelbounds()->set_x_min((*pixelbounds)[0]);
      path.mutable_pixelbounds()->set_x_max((*pixelbounds)[1]);
      path.mutable_pixelbounds()->set_y_min((*pixelbounds)[2]);
      path.mutable_pixelbounds()->set_y_max((*pixelbounds)[3]);
    }
  } else if (integrator_type == "sppm") {
    auto& sppm = *integrator.mutable_sppm();

    if (std::optional<int32_t> maxdepth =
            TryRemoveInteger(parameters, "maxdepth");
        maxdepth.has_value()) {
      sppm.set_maxdepth(*maxdepth);
    }

    if (std::optional<int32_t> numiterations =
            TryRemoveInteger(parameters, "numiterations");
        numiterations.has_value()) {
      sppm.set_numiterations(*numiterations);
    }

    if (std::optional<int32_t> photonsperiteration =
            TryRemoveInteger(parameters, "photonsperiteration");
        photonsperiteration.has_value()) {
      sppm.set_photonsperiteration(*photonsperiteration);
    }

    if (std::optional<int32_t> imagewritefrequency =
            TryRemoveInteger(parameters, "imagewritefrequency");
        imagewritefrequency.has_value()) {
      sppm.set_imagewritefrequency(*imagewritefrequency);
    }

    if (std::optional<double> radius = TryRemoveFloat(parameters, "radius");
        radius.has_value()) {
      sppm.set_radius(*radius);
    }
  } else if (integrator_type == "volpath") {
    auto& volpath = *integrator.mutable_volpath();

    if (std::optional<int32_t> maxdepth =
            TryRemoveInteger(parameters, "maxdepth");
        maxdepth.has_value()) {
      volpath.set_maxdepth(*maxdepth);
    }

    if (std::optional<double> rrthreshold =
            TryRemoveFloat(parameters, "rrthreshold");
        rrthreshold.has_value()) {
      volpath.set_rrthreshold(*rrthreshold);
    }

    if (std::optional<absl::string_view> lightsamplestrategy =
            TryRemoveString(parameters, "lightsamplestrategy");
        lightsamplestrategy.has_value()) {
      volpath.set_lightsamplestrategy(
          ParseLightSampleStrategy(*lightsamplestrategy));
    }

    std::optional<absl::Span<int32_t>> pixelbounds;
    if (absl::Status status =
            TryRemoveIntegers(parameters, "pixelbounds", 4, pixelbounds);
        status.ok() && pixelbounds.has_value()) {
      volpath.mutable_pixelbounds()->set_x_min((*pixelbounds)[0]);
      volpath.mutable_pixelbounds()->set_x_max((*pixelbounds)[1]);
      volpath.mutable_pixelbounds()->set_y_min((*pixelbounds)[2]);
      volpath.mutable_pixelbounds()->set_y_max((*pixelbounds)[3]);
    }
  } else if (integrator_type == "whitted") {
    auto& whitted = *integrator.mutable_whitted();

    if (std::optional<int32_t> maxdepth =
            TryRemoveInteger(parameters, "maxdepth");
        maxdepth.has_value()) {
      whitted.set_maxdepth(*maxdepth);
    }

    std::optional<absl::Span<int32_t>> pixelbounds;
    if (absl::Status status =
            TryRemoveIntegers(parameters, "pixelbounds", 4, pixelbounds);
        status.ok() && pixelbounds.has_value()) {
      whitted.mutable_pixelbounds()->set_x_min((*pixelbounds)[0]);
      whitted.mutable_pixelbounds()->set_x_max((*pixelbounds)[1]);
      whitted.mutable_pixelbounds()->set_y_min((*pixelbounds)[2]);
      whitted.mutable_pixelbounds()->set_y_max((*pixelbounds)[3]);
    }
  } else {
    std::cerr << "Unrecognized Integrator type: \"" << integrator_type << "\""
              << std::endl;
  }

  return absl::OkStatus();
}

absl::Status ParserV3::Import(absl::string_view path) {
  return absl::UnimplementedError(
      "Directive 'Import' is not supported in pbrt-v3");
}

absl::Status ParserV3::LightSource(
    absl::string_view light_source_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& light_source = *output_.add_directives()->mutable_light_source();

  if (light_source_type == "distant") {
    auto& distant = *light_source.mutable_distant();

    TryRemoveSpectrum(parameters, "L",
                      std::bind(&LightSource::Distant::mutable_l, &distant));

    if (std::optional<std::array<double, 3>> from =
            TryRemovePoint3(parameters, "from");
        from) {
      distant.mutable_from()->set_x((*from)[0]);
      distant.mutable_from()->set_y((*from)[1]);
      distant.mutable_from()->set_z((*from)[2]);
    }

    if (std::optional<std::array<double, 3>> to =
            TryRemovePoint3(parameters, "to");
        to) {
      distant.mutable_to()->set_x((*to)[0]);
      distant.mutable_to()->set_y((*to)[1]);
      distant.mutable_to()->set_z((*to)[2]);
    }

    TryRemoveSpectrum(
        parameters, "scale",
        std::bind(&LightSource::Distant::mutable_scale, &distant));
  } else if (light_source_type == "goniometric") {
    auto& goniometric = *light_source.mutable_goniometric();

    TryRemoveSpectrum(
        parameters, "I",
        std::bind(&LightSource::Goniometric::mutable_i, &goniometric));

    if (std::optional<absl::string_view> mapname =
            TryRemoveString(parameters, "mapname");
        mapname) {
      goniometric.set_mapname(*mapname);
    }

    TryRemoveSpectrum(
        parameters, "scale",
        std::bind(&LightSource::Goniometric::mutable_scale, &goniometric));
  } else if (light_source_type == "infinite") {
    auto& infinite = *light_source.mutable_infinite();

    TryRemoveSpectrum(parameters, "L",
                      std::bind(&LightSource::Infinite::mutable_l, &infinite));

    if (std::optional<int32_t> samples =
            TryRemoveInteger(parameters, "nsamples");
        samples) {
      infinite.set_samples(*samples);
    }

    if (std::optional<int32_t> samples =
            TryRemoveInteger(parameters, "samples");
        samples) {
      infinite.set_samples(*samples);
    }

    if (std::optional<absl::string_view> mapname =
            TryRemoveString(parameters, "mapname");
        mapname) {
      infinite.set_mapname(*mapname);
    }

    TryRemoveSpectrum(
        parameters, "scale",
        std::bind(&LightSource::Infinite::mutable_scale, &infinite));
  } else if (light_source_type == "point") {
    auto& point = *light_source.mutable_point();

    TryRemoveSpectrum(parameters, "I",
                      std::bind(&LightSource::Point::mutable_i, &point));

    if (std::optional<std::array<double, 3>> from =
            TryRemovePoint3(parameters, "from");
        from) {
      point.mutable_from()->set_x((*from)[0]);
      point.mutable_from()->set_y((*from)[1]);
      point.mutable_from()->set_z((*from)[2]);
    }

    TryRemoveSpectrum(parameters, "scale",
                      std::bind(&LightSource::Point::mutable_scale, &point));
  } else if (light_source_type == "projection") {
    auto& projection = *light_source.mutable_projection();

    TryRemoveSpectrum(
        parameters, "I",
        std::bind(&LightSource::Projection::mutable_i, &projection));

    if (std::optional<double> fov = TryRemoveFloat(parameters, "fov"); fov) {
      projection.set_fov(*fov);
    }

    if (std::optional<absl::string_view> mapname =
            TryRemoveString(parameters, "mapname");
        mapname) {
      projection.set_mapname(*mapname);
    }

    TryRemoveSpectrum(
        parameters, "scale",
        std::bind(&LightSource::Projection::mutable_scale, &projection));
  } else if (light_source_type == "spot") {
    auto& spot = *light_source.mutable_spot();

    TryRemoveSpectrum(parameters, "I",
                      std::bind(&LightSource::Spot::mutable_i, &spot));

    if (std::optional<std::array<double, 3>> from =
            TryRemovePoint3(parameters, "from");
        from) {
      spot.mutable_from()->set_x((*from)[0]);
      spot.mutable_from()->set_y((*from)[1]);
      spot.mutable_from()->set_z((*from)[2]);
    }

    if (std::optional<std::array<double, 3>> to =
            TryRemovePoint3(parameters, "to");
        to) {
      spot.mutable_to()->set_x((*to)[0]);
      spot.mutable_to()->set_y((*to)[1]);
      spot.mutable_to()->set_z((*to)[2]);
    }

    if (std::optional<double> coneangle =
            TryRemoveFloat(parameters, "coneangle");
        coneangle) {
      spot.set_coneangle(*coneangle);
    }

    if (std::optional<double> conedeltaangle =
            TryRemoveFloat(parameters, "conedeltaangle");
        conedeltaangle) {
      spot.set_conedeltaangle(*conedeltaangle);
    }

    TryRemoveSpectrum(parameters, "scale",
                      std::bind(&LightSource::Spot::mutable_scale, &spot));
  } else {
    std::cerr << "Unrecognized LightSource type: \"" << light_source_type
              << "\"" << std::endl;
  }

  return absl::OkStatus();
}

absl::Status ParserV3::LookAt(double eye_x, double eye_y, double eye_z,
                              double look_x, double look_y, double look_z,
                              double up_x, double up_y, double up_z) {
  auto& look_at = *output_.add_directives()->mutable_look_at();
  look_at.set_eye_x(eye_x);
  look_at.set_eye_y(eye_y);
  look_at.set_eye_z(eye_z);
  look_at.set_look_x(look_x);
  look_at.set_look_y(look_y);
  look_at.set_look_z(look_z);
  look_at.set_up_x(up_x);
  look_at.set_up_y(up_y);
  look_at.set_up_z(up_z);
  return absl::OkStatus();
}

absl::Status ParserV3::MakeNamedMaterial(
    absl::string_view material_name,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& make_named_material =
      *output_.add_directives()->mutable_make_named_material();

  make_named_material.set_name(material_name);
  *make_named_material.mutable_material() = ParseMaterial(
      TryRemoveString(parameters, "type").value_or(""), parameters);

  return absl::OkStatus();
}

absl::Status ParserV3::MakeNamedMedium(
    absl::string_view medium_name,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& make_named_medium =
      *output_.add_directives()->mutable_make_named_medium();

  make_named_medium.set_name(medium_name);

  absl::string_view medium_type =
      TryRemoveString(parameters, "type").value_or("");
  if (medium_type == "homogeneous") {
    auto& homogeneous = *make_named_medium.mutable_homogeneous();

    if (std::optional<absl::string_view> preset =
            TryRemoveString(parameters, "preset");
        preset.has_value()) {
      auto iter = kSubsurfaces.find(*preset);
      if (iter != kSubsurfaces.end()) {
        homogeneous.set_preset(iter->second);
      }
    }

    TryRemoveSpectrum(parameters, "sigma_a",
                      std::bind(&MakeNamedMedium::Homogeneous::mutable_sigma_a,
                                &homogeneous));
    TryRemoveSpectrum(parameters, "sigma_s",
                      std::bind(&MakeNamedMedium::Homogeneous::mutable_sigma_s,
                                &homogeneous));

    if (std::optional<double> g = TryRemoveFloat(parameters, "g");
        g.has_value()) {
      homogeneous.set_g(*g);
    }

    if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
        scale.has_value()) {
      homogeneous.set_scale(*scale);
    }
  } else if (medium_type == "heterogeneous") {
    auto& heterogeneous = *make_named_medium.mutable_heterogeneous();

    if (std::optional<absl::string_view> preset =
            TryRemoveString(parameters, "preset");
        preset.has_value()) {
      auto iter = kSubsurfaces.find(*preset);
      if (iter != kSubsurfaces.end()) {
        heterogeneous.set_preset(iter->second);
      }
    }

    TryRemoveSpectrum(
        parameters, "sigma_a",
        std::bind(&MakeNamedMedium::Heterogeneous::mutable_sigma_a,
                  &heterogeneous));
    TryRemoveSpectrum(
        parameters, "sigma_s",
        std::bind(&MakeNamedMedium::Heterogeneous::mutable_sigma_s,
                  &heterogeneous));

    if (std::optional<double> g = TryRemoveFloat(parameters, "g");
        g.has_value()) {
      heterogeneous.set_g(*g);
    }

    if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
        scale.has_value()) {
      heterogeneous.set_scale(*scale);
    }

    if (std::optional<std::array<double, 3>> p0 =
            TryRemovePoint3(parameters, "p0");
        p0.has_value()) {
      heterogeneous.mutable_p0()->set_x((*p0)[0]);
      heterogeneous.mutable_p0()->set_y((*p0)[1]);
      heterogeneous.mutable_p0()->set_z((*p0)[2]);
    }

    if (std::optional<std::array<double, 3>> p1 =
            TryRemovePoint3(parameters, "p1");
        p1.has_value()) {
      heterogeneous.mutable_p1()->set_x((*p1)[0]);
      heterogeneous.mutable_p1()->set_y((*p1)[1]);
      heterogeneous.mutable_p1()->set_z((*p1)[2]);
    }

    if (std::optional<int32_t> nx = TryRemoveInteger(parameters, "nx");
        nx.has_value()) {
      heterogeneous.set_nx(*nx);
    }

    if (std::optional<int32_t> ny = TryRemoveInteger(parameters, "ny");
        ny.has_value()) {
      heterogeneous.set_ny(*ny);
    }

    if (std::optional<int32_t> nz = TryRemoveInteger(parameters, "nz");
        nz.has_value()) {
      heterogeneous.set_nz(*nz);
    }

    int64_t required_size = static_cast<int64_t>(heterogeneous.nx()) *
                            static_cast<int64_t>(heterogeneous.ny());
    if (required_size < std::numeric_limits<int32_t>::max()) {
      required_size *= static_cast<int64_t>(heterogeneous.nz());
    }

    if (required_size > std::numeric_limits<int32_t>::max()) {
      return absl::InvalidArgumentError(
          "Heterogeneous medium is too large to be stored in a 1D proto array");
    }

    if (std::optional<absl::Span<double>> density =
            TryRemoveFloats(parameters, "density");
        density.has_value() &&
        density->size() == static_cast<size_t>(required_size)) {
      heterogeneous.mutable_density()->Add(density->begin(), density->end());
    } else {
      std::cerr << "Invalid or missing required heterogeneous MakeNamedMedia "
                   "parameter: 'density'"
                << std::endl;
      make_named_medium.clear_heterogeneous();
    }
  } else {
    std::cerr << "Unrecognized MakeNamedMedium type: \"" << medium_type << "\""
              << std::endl;
  }

  return absl::OkStatus();
}

absl::Status ParserV3::Material(
    absl::string_view material_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  *output_.add_directives()->mutable_material() =
      ParseMaterial(material_type, parameters);
  return absl::OkStatus();
}

absl::Status ParserV3::MediumInterface(absl::string_view inside,
                                       absl::string_view outside) {
  auto& material_interface =
      *output_.add_directives()->mutable_medium_interface();
  material_interface.set_inside(inside);
  material_interface.set_outside(outside);
  return absl::OkStatus();
}

absl::Status ParserV3::NamedMaterial(absl::string_view name) {
  output_.add_directives()->mutable_named_material()->set_name(name);
  return absl::OkStatus();
}

absl::Status ParserV3::ObjectBegin(absl::string_view name) {
  output_.add_directives()->mutable_object_begin()->set_name(name);
  return absl::OkStatus();
}

absl::Status ParserV3::ObjectEnd() {
  output_.add_directives()->mutable_object_end();
  return absl::OkStatus();
}

absl::Status ParserV3::ObjectInstance(absl::string_view name) {
  output_.add_directives()->mutable_object_instance()->set_name(name);
  return absl::OkStatus();
}

absl::Status ParserV3::PixelFilter(
    absl::string_view pixel_filter_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& pixel_filter = *output_.add_directives()->mutable_pixel_filter();

  if (pixel_filter_type == "box") {
    auto& box = *pixel_filter.mutable_box();

    if (std::optional<double> xwidth = TryRemoveFloat(parameters, "xwidth");
        xwidth.has_value()) {
      box.set_xwidth(*xwidth);
    }

    if (std::optional<double> ywidth = TryRemoveFloat(parameters, "ywidth");
        ywidth.has_value()) {
      box.set_ywidth(*ywidth);
    }
  } else if (pixel_filter_type == "gaussian") {
    auto& gaussian = *pixel_filter.mutable_gaussian();

    if (std::optional<double> xwidth = TryRemoveFloat(parameters, "xwidth");
        xwidth.has_value()) {
      gaussian.set_xwidth(*xwidth);
    }

    if (std::optional<double> ywidth = TryRemoveFloat(parameters, "ywidth");
        ywidth.has_value()) {
      gaussian.set_ywidth(*ywidth);
    }

    if (std::optional<double> alpha = TryRemoveFloat(parameters, "alpha");
        alpha.has_value()) {
      gaussian.set_alpha(*alpha);
    }
  } else if (pixel_filter_type == "mitchell") {
    auto& mitchell = *pixel_filter.mutable_mitchell();

    if (std::optional<double> xwidth = TryRemoveFloat(parameters, "xwidth");
        xwidth.has_value()) {
      mitchell.set_xwidth(*xwidth);
    }

    if (std::optional<double> ywidth = TryRemoveFloat(parameters, "ywidth");
        ywidth.has_value()) {
      mitchell.set_ywidth(*ywidth);
    }

    if (std::optional<double> B = TryRemoveFloat(parameters, "B");
        B.has_value()) {
      mitchell.set_b(*B);
    }

    if (std::optional<double> C = TryRemoveFloat(parameters, "C");
        C.has_value()) {
      mitchell.set_c(*C);
    }
  } else if (pixel_filter_type == "sinc") {
    auto& sinc = *pixel_filter.mutable_sinc();

    if (std::optional<double> xwidth = TryRemoveFloat(parameters, "xwidth");
        xwidth.has_value()) {
      sinc.set_xwidth(*xwidth);
    }

    if (std::optional<double> ywidth = TryRemoveFloat(parameters, "ywidth");
        ywidth.has_value()) {
      sinc.set_ywidth(*ywidth);
    }

    if (std::optional<double> tau = TryRemoveFloat(parameters, "tau");
        tau.has_value()) {
      sinc.set_tau(*tau);
    }
  } else if (pixel_filter_type == "triangle") {
    auto& triangle = *pixel_filter.mutable_triangle();

    if (std::optional<double> xwidth = TryRemoveFloat(parameters, "xwidth");
        xwidth.has_value()) {
      triangle.set_xwidth(*xwidth);
    }

    if (std::optional<double> ywidth = TryRemoveFloat(parameters, "ywidth");
        ywidth.has_value()) {
      triangle.set_ywidth(*ywidth);
    }
  } else {
    std::cerr << "Unrecognized PixelFilter type: \"" << pixel_filter_type
              << "\"" << std::endl;
  }

  return absl::OkStatus();
}

absl::Status ParserV3::ReverseOrientation() {
  output_.add_directives()->mutable_reverse_orientation();
  return absl::OkStatus();
}

absl::Status ParserV3::Rotate(double angle, double x, double y, double z) {
  auto& rotate = *output_.add_directives()->mutable_rotate();
  rotate.set_angle(angle);
  rotate.set_x(x);
  rotate.set_y(y);
  rotate.set_z(z);
  return absl::OkStatus();
}

absl::Status ParserV3::Scale(double x, double y, double z) {
  auto& scale = *output_.add_directives()->mutable_scale();
  scale.set_x(x);
  scale.set_y(y);
  scale.set_z(z);
  return absl::OkStatus();
}

absl::Status ParserV3::Sampler(
    absl::string_view sampler_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& sampler = *output_.add_directives()->mutable_sampler();

  if (sampler_type == "halton") {
    auto& halton = *sampler.mutable_halton();

    if (std::optional<int32_t> pixelsamples =
            TryRemoveInteger(parameters, "pixelsamples");
        pixelsamples.has_value()) {
      halton.set_pixelsamples(*pixelsamples);
    }

    if (std::optional<bool> samplepixelcenter =
            TryRemoveBool(parameters, "samplepixelcenter");
        samplepixelcenter.has_value()) {
      halton.set_samplepixelcenter(*samplepixelcenter);
    }
  } else if (sampler_type == "maxmindist") {
    auto& maxmindist = *sampler.mutable_maxmindist();

    if (std::optional<int32_t> pixelsamples =
            TryRemoveInteger(parameters, "pixelsamples");
        pixelsamples.has_value()) {
      maxmindist.set_pixelsamples(*pixelsamples);
    }

    if (std::optional<int32_t> dimensions =
            TryRemoveInteger(parameters, "dimensions");
        dimensions.has_value()) {
      maxmindist.set_dimensions(*dimensions);
    }
  } else if (sampler_type == "random") {
    auto& random = *sampler.mutable_random();

    if (std::optional<int32_t> pixelsamples =
            TryRemoveInteger(parameters, "pixelsamples");
        pixelsamples.has_value()) {
      random.set_pixelsamples(*pixelsamples);
    }
  } else if (sampler_type == "sobol") {
    auto& sobol = *sampler.mutable_sobol();

    if (std::optional<int32_t> pixelsamples =
            TryRemoveInteger(parameters, "pixelsamples");
        pixelsamples.has_value()) {
      sobol.set_pixelsamples(*pixelsamples);
    }
  } else if (sampler_type == "stratified") {
    auto& stratified = *sampler.mutable_stratified();

    if (std::optional<bool> jitter = TryRemoveBool(parameters, "jitter");
        jitter.has_value()) {
      stratified.set_jitter(*jitter);
    }

    if (std::optional<int32_t> xsamples =
            TryRemoveInteger(parameters, "xsamples");
        xsamples.has_value()) {
      stratified.set_xsamples(*xsamples);
    }

    if (std::optional<int32_t> ysamples =
            TryRemoveInteger(parameters, "ysamples");
        ysamples.has_value()) {
      stratified.set_ysamples(*ysamples);
    }

    if (std::optional<int32_t> dimensions =
            TryRemoveInteger(parameters, "dimensions");
        dimensions.has_value()) {
      stratified.set_dimensions(*dimensions);
    }
  } else if (sampler_type == "02sequence" || sampler_type == "lowdiscrepancy") {
    auto& zerotwosequence = *sampler.mutable_zerotwosequence();

    if (std::optional<int32_t> pixelsamples =
            TryRemoveInteger(parameters, "pixelsamples");
        pixelsamples.has_value()) {
      zerotwosequence.set_pixelsamples(*pixelsamples);
    }

    if (std::optional<int32_t> dimensions =
            TryRemoveInteger(parameters, "dimensions");
        dimensions.has_value()) {
      zerotwosequence.set_dimensions(*dimensions);
    }
  } else {
    std::cerr << "Unrecognized Sampler type: \"" << sampler_type << "\""
              << std::endl;
  }

  return absl::OkStatus();
}

absl::Status ParserV3::Shape(
    absl::string_view shape_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& shape = *output_.add_directives()->mutable_shape();

  if (shape_type == "cone") {
    auto& cone = *shape.mutable_cone();

    if (std::optional<double> radius = TryRemoveFloat(parameters, "radius");
        radius.has_value()) {
      cone.set_radius(*radius);
    }

    if (std::optional<double> height = TryRemoveFloat(parameters, "height");
        height.has_value()) {
      cone.set_height(*height);
    }

    if (std::optional<double> phimax = TryRemoveFloat(parameters, "phimax");
        phimax.has_value()) {
      cone.set_phimax(*phimax);
    }
  } else if (shape_type == "curve") {
    auto& curve = *shape.mutable_curve();

    if (std::optional<double> width = TryRemoveFloat(parameters, "width");
        width.has_value()) {
      curve.set_width(*width);
    }

    if (std::optional<double> width0 = TryRemoveFloat(parameters, "width0");
        width0.has_value()) {
      curve.set_width0(*width0);
    }

    if (std::optional<double> width1 = TryRemoveFloat(parameters, "width1");
        width1.has_value()) {
      curve.set_width1(*width1);
    }

    if (std::optional<int32_t> degree = TryRemoveInteger(parameters, "degree");
        degree.has_value()) {
      curve.set_degree(*degree);
    }

    if (std::optional<absl::string_view> basis =
            TryRemoveString(parameters, "basis");
        basis.has_value()) {
      if (*basis == "bezier") {
        curve.set_basis(Shape::Curve::BEZIER);
      } else if (*basis == "bspline") {
        curve.set_basis(Shape::Curve::BSPLINE);
      } else {
        std::cerr
            << "Unrecognized value for 'curve' Shape parameter 'basis': \""
            << *basis << "\"" << std::endl;

        shape.clear_curve();
        return absl::OkStatus();
      }
    }

    if (std::optional<absl::Span<std::array<double, 3>>> p =
            TryRemovePoint3s(parameters, "P");
        p.has_value()) {
      for (const auto& src : *p) {
        auto& dest = *curve.add_p();
        dest.set_x(src[0]);
        dest.set_y(src[1]);
        dest.set_z(src[2]);
      }
    }

    if (std::optional<absl::string_view> type =
            TryRemoveString(parameters, "type");
        type.has_value()) {
      if (*type == "cylinder") {
        curve.set_type(Shape::Curve::CYLINDER);
      } else if (*type == "flat") {
        curve.set_type(Shape::Curve::FLAT);
      } else if (*type == "ribbon") {
        curve.set_type(Shape::Curve::RIBBON);
      } else {
        std::cerr << "Unrecognized value for 'curve' Shape parameter 'type': \""
                  << *type << "\"" << std::endl;
        curve.set_type(Shape::Curve::CYLINDER);
      }
    }

    if (std::optional<absl::Span<std::array<double, 3>>> n =
            TryRemoveNormals(parameters, "N");
        n.has_value()) {
      for (const auto& src : *n) {
        auto& dest = *curve.add_n();
        dest.set_x(src[0]);
        dest.set_y(src[1]);
        dest.set_z(src[2]);
      }
    }

    if (std::optional<int32_t> splitdepth =
            TryRemoveInteger(parameters, "splitdepth");
        splitdepth.has_value()) {
      curve.set_splitdepth(*splitdepth);
    }
  } else if (shape_type == "cylinder") {
    auto& cylinder = *shape.mutable_cylinder();

    if (std::optional<double> radius = TryRemoveFloat(parameters, "radius");
        radius.has_value()) {
      cylinder.set_radius(*radius);
    }

    if (std::optional<double> zmin = TryRemoveFloat(parameters, "zmin");
        zmin.has_value()) {
      cylinder.set_zmin(*zmin);
    }

    if (std::optional<double> zmax = TryRemoveFloat(parameters, "zmax");
        zmax.has_value()) {
      cylinder.set_zmax(*zmax);
    }

    if (std::optional<double> phimax = TryRemoveFloat(parameters, "phimax");
        phimax.has_value()) {
      cylinder.set_phimax(*phimax);
    }
  } else if (shape_type == "disk") {
    auto& disk = *shape.mutable_disk();

    if (std::optional<double> height = TryRemoveFloat(parameters, "height");
        height.has_value()) {
      disk.set_height(*height);
    }

    if (std::optional<double> radius = TryRemoveFloat(parameters, "radius");
        radius.has_value()) {
      disk.set_radius(*radius);
    }

    if (std::optional<double> innerradius =
            TryRemoveFloat(parameters, "innerradius");
        innerradius.has_value()) {
      disk.set_innerradius(*innerradius);
    }

    if (std::optional<double> phimax = TryRemoveFloat(parameters, "phimax");
        phimax.has_value()) {
      disk.set_phimax(*phimax);
    }
  } else if (shape_type == "heightfield") {
    auto& heightfield = *shape.mutable_heightfield();

    if (std::optional<int32_t> nu = TryRemoveInteger(parameters, "nu");
        nu.has_value()) {
      heightfield.set_nu(*nu);
    } else {
      return absl::InvalidArgumentError(
          "Missing required heightfield Shape parameter: 'nu'");
    }

    if (std::optional<int32_t> nv = TryRemoveInteger(parameters, "nv");
        nv.has_value()) {
      heightfield.set_nv(*nv);
    } else {
      return absl::InvalidArgumentError(
          "Missing required heightfield Shape parameter: 'nv'");
    }

    int64_t required_size = static_cast<int64_t>(heightfield.nu()) *
                            static_cast<int64_t>(heightfield.nv());
    if (required_size > std::numeric_limits<int32_t>::max()) {
      return absl::InvalidArgumentError(
          "Heighfield shape is too large to be stored in a 1D proto array");
    }

    std::optional<absl::Span<double>> pz;
    if (absl::Status status = TryRemoveFloats(
            parameters, "Pz", static_cast<size_t>(required_size), pz);
        status.ok() && pz.has_value()) {
      heightfield.mutable_pz()->Add(pz->begin(), pz->end());
    } else {
      return absl::InvalidArgumentError(
          "Missing or invalid heightfield Shape parameter: 'Pz'");
    }
  } else if (shape_type == "hyperboloid") {
    auto& hyperboloid = *shape.mutable_hyperboloid();

    if (std::optional<std::array<double, 3>> p1 =
            TryRemovePoint3(parameters, "p1");
        p1.has_value()) {
      hyperboloid.mutable_p1()->set_x((*p1)[0]);
      hyperboloid.mutable_p1()->set_y((*p1)[1]);
      hyperboloid.mutable_p1()->set_z((*p1)[2]);
    }

    if (std::optional<std::array<double, 3>> p2 =
            TryRemovePoint3(parameters, "p2");
        p2.has_value()) {
      hyperboloid.mutable_p2()->set_x((*p2)[0]);
      hyperboloid.mutable_p2()->set_y((*p2)[1]);
      hyperboloid.mutable_p2()->set_z((*p2)[2]);
    }

    if (std::optional<double> phimax = TryRemoveFloat(parameters, "phimax");
        phimax.has_value()) {
      hyperboloid.set_phimax(*phimax);
    }
  } else if (shape_type == "loopsubdiv") {
    auto& loopsubdiv = *shape.mutable_loopsubdiv();

    if (std::optional<int32_t> nlevels =
            TryRemoveInteger(parameters, "nlevels");
        nlevels.has_value()) {
      loopsubdiv.set_levels(*nlevels);
    }

    if (std::optional<int32_t> levels = TryRemoveInteger(parameters, "levels");
        levels.has_value()) {
      loopsubdiv.set_levels(*levels);
    }

    if (std::optional<absl::Span<int32_t>> indices =
            TryRemoveIntegers(parameters, "indices");
        indices.has_value()) {
      indices->remove_suffix(indices->size() % 3);
      for (size_t i = 0; i < indices->size() / 3u; i++) {
        auto& face = *loopsubdiv.add_indices();
        face.set_v0((*indices)[3u * i + 0]);
        face.set_v1((*indices)[3u * i + 1]);
        face.set_v2((*indices)[3u * i + 2]);
      }
    } else {
      std::cerr << "Missing required loopsubdiv Shape parameter: 'indices'"
                << std::endl;
      shape.clear_loopsubdiv();
      return absl::OkStatus();
    }

    if (std::optional<absl::Span<std::array<double, 3>>> p =
            TryRemovePoint3s(parameters, "P");
        p.has_value()) {
      p->remove_suffix(p->size() % 3);
      for (const auto& src : *p) {
        auto& dest = *loopsubdiv.add_p();
        dest.set_x(src[0]);
        dest.set_y(src[1]);
        dest.set_z(src[2]);
      }
    } else {
      std::cerr << "Missing required loopsubdiv Shape parameter: 'P'"
                << std::endl;
      shape.clear_loopsubdiv();
      return absl::OkStatus();
    }
  } else if (shape_type == "nurbs") {
    auto& nurbs = *shape.mutable_nurbs();

    if (std::optional<int32_t> nu = TryRemoveInteger(parameters, "nu");
        nu.has_value()) {
      nurbs.set_nu(*nu);
    } else {
      std::cerr << "Missing required nurbs Shape parameter: 'nu'" << std::endl;
      shape.clear_nurbs();
      return absl::OkStatus();
    }

    if (std::optional<int32_t> nv = TryRemoveInteger(parameters, "nv");
        nv.has_value()) {
      nurbs.set_nv(*nv);
    } else {
      std::cerr << "Missing required nurbs Shape parameter: 'nv'" << std::endl;
      shape.clear_nurbs();
      return absl::OkStatus();
    }

    if (std::optional<int32_t> uorder = TryRemoveInteger(parameters, "uorder");
        uorder.has_value()) {
      nurbs.set_uorder(*uorder);
    } else {
      std::cerr << "Missing required nurbs Shape parameter: 'uorder'"
                << std::endl;
      shape.clear_nurbs();
      return absl::OkStatus();
    }

    if (std::optional<int32_t> vorder = TryRemoveInteger(parameters, "vorder");
        vorder.has_value()) {
      nurbs.set_vorder(*vorder);
    } else {
      std::cerr << "Missing required nurbs Shape parameter: 'vorder'"
                << std::endl;
      shape.clear_nurbs();
      return absl::OkStatus();
    }

    std::optional<absl::Span<double>> uknots;
    if (absl::Status status =
            TryRemoveFloats(parameters, "uknots",
                            static_cast<uint32_t>(nurbs.nu()) +
                                static_cast<uint32_t>(nurbs.uorder()),
                            uknots);
        status.ok() && uknots.has_value()) {
      nurbs.mutable_uknots()->Add(uknots->begin(), uknots->end());
    } else {
      std::cerr << "Missing required nurbs Shape parameter: 'uknots'"
                << std::endl;
      shape.clear_nurbs();
      return absl::OkStatus();
    }

    std::optional<absl::Span<double>> vknots;
    if (absl::Status status =
            TryRemoveFloats(parameters, "vknots",
                            static_cast<uint32_t>(nurbs.nv()) +
                                static_cast<uint32_t>(nurbs.vorder()),
                            vknots);
        status.ok() && vknots.has_value()) {
      nurbs.mutable_vknots()->Add(vknots->begin(), vknots->end());
    } else {
      std::cerr << "Missing required nurbs Shape parameter: 'vknots'"
                << std::endl;
      shape.clear_nurbs();
      return absl::OkStatus();
    }

    if (std::optional<double> v0 = TryRemoveFloat(parameters, "v0");
        v0.has_value()) {
      nurbs.set_v0(*v0);
    }

    if (std::optional<double> v1 = TryRemoveFloat(parameters, "v1");
        v1.has_value()) {
      nurbs.set_v1(*v1);
    }

    if (std::optional<double> u0 = TryRemoveFloat(parameters, "u0");
        u0.has_value()) {
      nurbs.set_u0(*u0);
    }

    if (std::optional<double> u1 = TryRemoveFloat(parameters, "u1");
        u1.has_value()) {
      nurbs.set_u1(*u1);
    }

    int64_t required_points =
        static_cast<int64_t>(nurbs.nu()) * static_cast<int64_t>(nurbs.nv());
    if (required_points > std::numeric_limits<int32_t>::max()) {
      return absl::InvalidArgumentError(
          "Nurbs shape has too many points to be stored in a 1D proto array");
    }

    size_t num_points = 0;
    if (std::optional<absl::Span<std::array<double, 3>>> p =
            TryRemovePoint3s(parameters, "P");
        p.has_value()) {
      for (auto& src : *p) {
        auto& dest = *nurbs.add_p();
        dest.set_x(src[0]);
        dest.set_y(src[1]);
        dest.set_z(src[2]);
        num_points += 1;
      }
    } else if (std::optional<absl::Span<double>> pw =
                   TryRemoveFloats(parameters, "Pw");
               pw.has_value() && pw->size() % 4u == 0u) {
      for (size_t i = 0; i < pw->size() / 4; i++) {
        auto& dest = *nurbs.add_pw();
        dest.mutable_p()->set_x((*pw)[4 * i + 0]);
        dest.mutable_p()->set_y((*pw)[4 * i + 1]);
        dest.mutable_p()->set_z((*pw)[4 * i + 2]);
        dest.set_weight((*pw)[4 * i + 3]);
        num_points += 1;
      }
    } else {
      std::cerr
          << "Invalid or missing required nurbs Shape parameter: 'P' or 'Pw'"
          << std::endl;
      shape.clear_nurbs();
      return absl::OkStatus();
    }

    if (num_points != static_cast<size_t>(required_points)) {
      std::cerr
          << "Invalid or missing required nurbs Shape parameter: 'P' or 'Pw'"
          << std::endl;
      shape.clear_nurbs();
      return absl::OkStatus();
    }
  } else if (shape_type == "paraboloid") {
    auto& paraboloid = *shape.mutable_paraboloid();

    if (std::optional<double> radius = TryRemoveFloat(parameters, "radius");
        radius.has_value()) {
      paraboloid.set_radius(*radius);
    }

    if (std::optional<double> zmin = TryRemoveFloat(parameters, "zmin");
        zmin.has_value()) {
      paraboloid.set_zmin(*zmin);
    }

    if (std::optional<double> zmax = TryRemoveFloat(parameters, "zmax");
        zmax.has_value()) {
      paraboloid.set_zmax(*zmax);
    }

    if (std::optional<double> phimax = TryRemoveFloat(parameters, "phimax");
        phimax.has_value()) {
      paraboloid.set_phimax(*phimax);
    }
  } else if (shape_type == "plymesh") {
    auto& plymesh = *shape.mutable_plymesh();

    if (std::optional<absl::string_view> filename =
            TryRemoveString(parameters, "filename");
        filename) {
      plymesh.set_filename(*filename);
    }

    TryRemoveFloatTexture(parameters, "alpha",
                          std::bind(&Shape::PlyMesh::mutable_alpha, &plymesh));

    TryRemoveFloatTexture(
        parameters, "shadowalpha",
        std::bind(&Shape::PlyMesh::mutable_shadowalpha, &plymesh));
  } else if (shape_type == "sphere") {
    auto& sphere = *shape.mutable_sphere();

    if (std::optional<double> radius = TryRemoveFloat(parameters, "radius");
        radius.has_value()) {
      sphere.set_radius(*radius);
    }

    if (std::optional<double> zmin = TryRemoveFloat(parameters, "zmin");
        zmin.has_value()) {
      sphere.set_zmin(*zmin);
    }

    if (std::optional<double> zmax = TryRemoveFloat(parameters, "zmax");
        zmax.has_value()) {
      sphere.set_zmax(*zmax);
    }

    if (std::optional<double> phimax = TryRemoveFloat(parameters, "phimax");
        phimax.has_value()) {
      sphere.set_phimax(*phimax);
    }
  } else if (shape_type == "trianglemesh") {
    auto& trianglemesh = *shape.mutable_trianglemesh();

    if (std::optional<absl::Span<std::array<double, 3>>> p =
            TryRemovePoint3s(parameters, "P");
        p.has_value()) {
      for (const auto& src : *p) {
        auto& dest = *trianglemesh.add_p();
        dest.set_x(src[0]);
        dest.set_y(src[1]);
        dest.set_z(src[2]);
      }
    } else {
      std::cerr
          << "Invalid or missing required trianglemesh Shape parameter: 'P'"
          << std::endl;
      shape.clear_trianglemesh();
      return absl::OkStatus();
    }

    if (std::optional<absl::Span<int32_t>> indices =
            TryRemoveIntegers(parameters, "indices");
        indices.has_value()) {
      indices->remove_suffix(indices->size() % 3);
      for (size_t i = 0; i < indices->size() / 3; i++) {
        auto& dest = *trianglemesh.add_indices();
        dest.set_v0((*indices)[3u * i + 0]);
        dest.set_v1((*indices)[3u * i + 1]);
        dest.set_v2((*indices)[3u * i + 2]);
      }
    } else {
      std::cerr << "Invalid or missing required trianglemesh Shape parameter: "
                   "'indices'"
                << std::endl;
      shape.clear_trianglemesh();
      return absl::OkStatus();
    }

    if (std::optional<absl::Span<std::array<double, 3>>> n =
            TryRemoveNormals(parameters, "N");
        n.has_value()) {
      for (const auto& src : *n) {
        auto& dest = *trianglemesh.add_n();
        dest.set_x(src[0]);
        dest.set_y(src[1]);
        dest.set_z(src[2]);
      }
    }

    if (std::optional<absl::Span<std::array<double, 3>>> s =
            TryRemoveVector3s(parameters, "S");
        s.has_value()) {
      for (const auto& src : *s) {
        auto& dest = *trianglemesh.add_s();
        dest.set_x(src[0]);
        dest.set_y(src[1]);
        dest.set_z(src[2]);
      }
    }

    if (std::optional<absl::Span<int32_t>> faceIndices =
            TryRemoveIntegers(parameters, "faceIndices");
        faceIndices.has_value()) {
      trianglemesh.mutable_faceindices()->Add(faceIndices->begin(),
                                              faceIndices->end());
    }

    if (std::optional<absl::Span<std::array<double, 2>>> uv =
            TryRemovePoint2s(parameters, "uv");
        uv.has_value()) {
      for (const auto& src : *uv) {
        auto& dest = *trianglemesh.add_uv();
        dest.set_u(src[0]);
        dest.set_v(src[1]);
      }
    } else if (std::optional<absl::Span<std::array<double, 2>>> st =
                   TryRemovePoint2s(parameters, "st");
               st.has_value()) {
      for (const auto& src : *st) {
        auto& dest = *trianglemesh.add_uv();
        dest.set_u(src[0]);
        dest.set_v(src[1]);
      }
    } else if (std::optional<absl::Span<double>> uv =
                   TryRemoveFloats(parameters, "uv");
               uv.has_value()) {
      uv->remove_suffix(uv->size() % 2);
      for (size_t i = 0; i < uv->size() / 2; i++) {
        auto& dest = *trianglemesh.add_uv();
        dest.set_u((*uv)[2 * i + 0]);
        dest.set_v((*uv)[2 * i + 1]);
      }
    } else if (std::optional<absl::Span<double>> st =
                   TryRemoveFloats(parameters, "st");
               st.has_value()) {
      st->remove_suffix(st->size() % 2);
      for (size_t i = 0; i < st->size() / 2; i++) {
        auto& dest = *trianglemesh.add_uv();
        dest.set_u((*st)[2 * i + 0]);
        dest.set_v((*st)[2 * i + 1]);
      }
    }

    TryRemoveFloatTexture(
        parameters, "alpha",
        std::bind(&Shape::TriangleMesh::mutable_alpha, &trianglemesh));

    TryRemoveFloatTexture(
        parameters, "shadowalpha",
        std::bind(&Shape::TriangleMesh::mutable_shadowalpha, &trianglemesh));
  } else {
    std::cerr << "Unrecognized Shape type: \"" << shape_type << "\""
              << std::endl;
    return absl::OkStatus();
  }

  auto& overrides = *shape.mutable_overrides();

  bool overrides_populated = false;
  if (std::optional<bool> remaproughness =
          TryRemoveBool(parameters, "remaproughness");
      remaproughness.has_value()) {
    overrides.set_remaproughness(*remaproughness);
    overrides_populated = true;
  }

  if (std::optional<bool> thin = TryRemoveBool(parameters, "thin");
      thin.has_value()) {
    overrides.set_thin(*thin);
    overrides_populated = true;
  }

  if (std::optional<double> g = TryRemoveFloat(parameters, "g");
      g.has_value()) {
    overrides.set_g(*g);
    overrides_populated = true;
  }

  if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
      scale.has_value()) {
    overrides.set_scale(*scale);
    overrides_populated = true;
  }

  overrides_populated |= TryRemoveFloatTexture(
      parameters, "alpha",
      std::bind(&Shape::MaterialOverrides::mutable_alpha, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "anisotropic",
      std::bind(&Shape::MaterialOverrides::mutable_anisotropic, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "beta_m",
      std::bind(&Shape::MaterialOverrides::mutable_beta_m, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "beta_n",
      std::bind(&Shape::MaterialOverrides::mutable_beta_n, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "bumpmap",
      std::bind(&Shape::MaterialOverrides::mutable_bumpmap, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "clearcoat",
      std::bind(&Shape::MaterialOverrides::mutable_clearcoat, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "clearcoatgloss",
      std::bind(&Shape::MaterialOverrides::mutable_clearcoatgloss, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "difftrans",
      std::bind(&Shape::MaterialOverrides::mutable_difftrans, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "eumelanin",
      std::bind(&Shape::MaterialOverrides::mutable_eumelanin, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "flatness",
      std::bind(&Shape::MaterialOverrides::mutable_flatness, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "metallic",
      std::bind(&Shape::MaterialOverrides::mutable_metallic, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "pheomelanin",
      std::bind(&Shape::MaterialOverrides::mutable_pheomelanin, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "roughness",
      std::bind(&Shape::MaterialOverrides::mutable_roughness, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "sheen",
      std::bind(&Shape::MaterialOverrides::mutable_sheen, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "sheentint",
      std::bind(&Shape::MaterialOverrides::mutable_sheentint, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "sigma",
      std::bind(&Shape::MaterialOverrides::mutable_sigma, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "spectrans",
      std::bind(&Shape::MaterialOverrides::mutable_spectrans, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "speculartint",
      std::bind(&Shape::MaterialOverrides::mutable_speculartint, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "uroughness",
      std::bind(&Shape::MaterialOverrides::mutable_uroughness, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "vroughness",
      std::bind(&Shape::MaterialOverrides::mutable_vroughness, &overrides));

  if (std::optional<absl::string_view> name =
          TryRemoveString(parameters, "name");
      name.has_value()) {
    auto iter = kSubsurfaces.find(*name);
    if (iter != kSubsurfaces.end()) {
      overrides.set_name(iter->second);
      overrides_populated = true;
    }
  }

  overrides_populated |= TryRemoveSpectrumTexture(
      parameters, "amount",
      std::bind(&Shape::MaterialOverrides::mutable_amount, &overrides));
  overrides_populated |= TryRemoveSpectrumTexture(
      parameters, "color",
      std::bind(&Shape::MaterialOverrides::mutable_color, &overrides));
  overrides_populated |= TryRemoveSpectrumTexture(
      parameters, "k",
      std::bind(&Shape::MaterialOverrides::mutable_k, &overrides));
  overrides_populated |= TryRemoveSpectrumTexture(
      parameters, "Kd",
      std::bind(&Shape::MaterialOverrides::mutable_kd, &overrides));
  overrides_populated |= TryRemoveSpectrumTexture(
      parameters, "Kr",
      std::bind(&Shape::MaterialOverrides::mutable_kr, &overrides));
  overrides_populated |= TryRemoveSpectrumTexture(
      parameters, "Ks",
      std::bind(&Shape::MaterialOverrides::mutable_ks, &overrides));
  overrides_populated |= TryRemoveSpectrumTexture(
      parameters, "Kt",
      std::bind(&Shape::MaterialOverrides::mutable_kt, &overrides));
  overrides_populated |= TryRemoveSpectrumTexture(
      parameters, "mfp",
      std::bind(&Shape::MaterialOverrides::mutable_mfp, &overrides));
  overrides_populated |= TryRemoveSpectrumTexture(
      parameters, "opacity",
      std::bind(&Shape::MaterialOverrides::mutable_opacity, &overrides));
  overrides_populated |= TryRemoveSpectrumTexture(
      parameters, "reflect",
      std::bind(&Shape::MaterialOverrides::mutable_reflect, &overrides));
  overrides_populated |= TryRemoveSpectrumTexture(
      parameters, "scatterdistance",
      std::bind(&Shape::MaterialOverrides::mutable_scatterdistance,
                &overrides));
  overrides_populated |= TryRemoveSpectrumTexture(
      parameters, "sigma_a",
      std::bind(&Shape::MaterialOverrides::mutable_sigma_a, &overrides));
  overrides_populated |= TryRemoveSpectrumTexture(
      parameters, "sigma_s",
      std::bind(&Shape::MaterialOverrides::mutable_sigma_s, &overrides));
  overrides_populated |= TryRemoveSpectrumTexture(
      parameters, "transmit",
      std::bind(&Shape::MaterialOverrides::mutable_transmit, &overrides));

  if (std::optional<absl::string_view> bsdffile =
          TryRemoveString(parameters, "bsdffile");
      bsdffile.has_value()) {
    overrides.set_bsdffile(*bsdffile);
    overrides_populated = true;
  }

  if (std::optional<absl::string_view> namedmaterial1 =
          TryRemoveString(parameters, "namedmaterial1");
      namedmaterial1.has_value()) {
    overrides.set_namedmaterial1(*namedmaterial1);
    overrides_populated = true;
  }

  if (std::optional<absl::string_view> namedmaterial2 =
          TryRemoveString(parameters, "namedmaterial2");
      namedmaterial2.has_value()) {
    overrides.set_namedmaterial2(*namedmaterial2);
    overrides_populated = true;
  }

  if (std::optional<double> eta = TryRemoveFloat(parameters, "eta");
      eta.has_value()) {
    overrides.set_eta_as_value(*eta);
    overrides.mutable_eta_as_float_texture()->set_float_value(*eta);
    overrides_populated = true;
  } else if (std::optional<absl::string_view> eta =
                 TryRemoveTexture(parameters, "eta");
             eta.has_value()) {
    overrides.mutable_eta_as_float_texture()->set_float_texture_name(*eta);
    overrides.mutable_eta_as_spectrum_texture()->set_spectrum_texture_name(
        *eta);
    overrides_populated = true;
  } else {
    overrides_populated |= TryRemoveSpectrumTexture(
        parameters, "eta",
        std::bind(&Shape::MaterialOverrides::mutable_eta_as_spectrum_texture,
                  &overrides));
  }

  if (!overrides_populated) {
    shape.clear_overrides();
  }

  return absl::OkStatus();
}

absl::Status ParserV3::SpectrumTexture(
    absl::string_view spectrum_texture_name,
    absl::string_view spectrum_texture_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& spectrum_texture =
      *output_.add_directives()->mutable_spectrum_texture();

  spectrum_texture.set_name(spectrum_texture_name);

  if (spectrum_texture_type == "bilerp") {
    auto& bilerp = *spectrum_texture.mutable_bilerp();

    TryRemoveSpectrumTexture(
        parameters, "v00",
        std::bind(&SpectrumTexture::Bilerp::mutable_v00, &bilerp));
    TryRemoveSpectrumTexture(
        parameters, "v01",
        std::bind(&SpectrumTexture::Bilerp::mutable_v01, &bilerp));
    TryRemoveSpectrumTexture(
        parameters, "v10",
        std::bind(&SpectrumTexture::Bilerp::mutable_v10, &bilerp));
    TryRemoveSpectrumTexture(
        parameters, "v11",
        std::bind(&SpectrumTexture::Bilerp::mutable_v11, &bilerp));
    TryRemoveUVParameters(parameters, bilerp);
  } else if (spectrum_texture_type == "checkerboard") {
    int32_t dimensions = TryRemoveInteger(parameters, "dimensions").value_or(2);
    if (dimensions == 2) {
      auto& checkerboard = *spectrum_texture.mutable_checkerboard2d();

      if (std::optional<AntialiasingMode> aamode =
              TryRemoveAntialiasingMode(parameters);
          aamode) {
        checkerboard.set_aamode(*aamode);
      }

      TryRemoveSpectrumTexture(
          parameters, "tex1",
          std::bind(&SpectrumTexture::Checkerboard2D::mutable_tex1,
                    &checkerboard));
      TryRemoveSpectrumTexture(
          parameters, "tex2",
          std::bind(&SpectrumTexture::Checkerboard2D::mutable_tex2,
                    &checkerboard));
      TryRemoveUVParameters(parameters, checkerboard);
    } else {
      auto& checkerboard = *spectrum_texture.mutable_checkerboard3d();

      TryRemoveSpectrumTexture(
          parameters, "tex1",
          std::bind(&SpectrumTexture::Checkerboard3D::mutable_tex1,
                    &checkerboard));
      TryRemoveSpectrumTexture(
          parameters, "tex2",
          std::bind(&SpectrumTexture::Checkerboard3D::mutable_tex2,
                    &checkerboard));
    }
  } else if (spectrum_texture_type == "constant") {
    auto& constant = *spectrum_texture.mutable_constant();

    TryRemoveSpectrum(
        parameters, "value",
        std::bind(&SpectrumTexture::Constant::mutable_value, &constant));
  } else if (spectrum_texture_type == "dots") {
    auto& dots = *spectrum_texture.mutable_dots();

    TryRemoveSpectrumTexture(
        parameters, "inside",
        std::bind(&SpectrumTexture::Dots::mutable_inside, &dots));
    TryRemoveSpectrumTexture(
        parameters, "outside",
        std::bind(&SpectrumTexture::Dots::mutable_outside, &dots));
    TryRemoveUVParameters(parameters, dots);
  } else if (spectrum_texture_type == "fbm") {
    auto& fbm = *spectrum_texture.mutable_fbm();

    if (std::optional<int32_t> octaves =
            TryRemoveInteger(parameters, "octaves");
        octaves) {
      fbm.set_octaves(*octaves);
    }

    if (std::optional<double> roughness =
            TryRemoveFloat(parameters, "roughness");
        roughness) {
      fbm.set_roughness(*roughness);
    }
  } else if (spectrum_texture_type == "imagemap") {
    auto& imagemap = *spectrum_texture.mutable_imagemap();

    if (std::optional<bool> gamma = TryRemoveBool(parameters, "gamma"); gamma) {
      imagemap.set_gamma(*gamma);
    }

    if (std::optional<absl::string_view> filename =
            TryRemoveString(parameters, "filename");
        filename) {
      imagemap.set_filename(*filename);
    }

    if (std::optional<double> maxanisotropy =
            TryRemoveFloat(parameters, "maxanisotropy");
        maxanisotropy) {
      imagemap.set_maxanisotropy(*maxanisotropy);
    }

    if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
        scale) {
      imagemap.set_scale(*scale);
    }

    if (std::optional<bool> trilinear = TryRemoveBool(parameters, "trilinear");
        trilinear) {
      imagemap.set_trilinear(*trilinear);
    }

    if (std::optional<ImageWrapping> wrap = TryRemoveImageWrapping(parameters);
        wrap) {
      imagemap.set_wrap(*wrap);
    }

    TryRemoveUVParameters(parameters, imagemap);
  } else if (spectrum_texture_type == "marble") {
    auto& marble = *spectrum_texture.mutable_marble();

    if (std::optional<int32_t> octaves =
            TryRemoveInteger(parameters, "octaves");
        octaves) {
      marble.set_octaves(*octaves);
    }

    if (std::optional<double> roughness =
            TryRemoveFloat(parameters, "roughness");
        roughness) {
      marble.set_roughness(*roughness);
    }

    if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
        scale) {
      marble.set_scale(*scale);
    }

    if (std::optional<double> variation =
            TryRemoveFloat(parameters, "variation");
        variation) {
      marble.set_variation(*variation);
    }
  } else if (spectrum_texture_type == "mix") {
    auto& mix = *spectrum_texture.mutable_mix();

    TryRemoveFloatTexture(
        parameters, "amount",
        std::bind(&SpectrumTexture::Mix::mutable_amount, &mix));
    TryRemoveSpectrumTexture(
        parameters, "tex1",
        std::bind(&SpectrumTexture::Mix::mutable_tex1, &mix));
    TryRemoveSpectrumTexture(
        parameters, "tex2",
        std::bind(&SpectrumTexture::Mix::mutable_tex2, &mix));
  } else if (spectrum_texture_type == "ptex") {
    auto& ptex = *spectrum_texture.mutable_ptex();

    if (std::optional<absl::string_view> filename =
            TryRemoveString(parameters, "filename");
        filename) {
      ptex.set_filename(*filename);
    }

    if (std::optional<double> gamma = TryRemoveFloat(parameters, "gamma");
        gamma) {
      ptex.set_gamma(*gamma);
    }
  } else if (spectrum_texture_type == "scale") {
    auto& scale = *spectrum_texture.mutable_scale();

    TryRemoveSpectrumTexture(
        parameters, "tex1",
        std::bind(&SpectrumTexture::Scale::mutable_tex1, &scale));
    TryRemoveSpectrumTexture(
        parameters, "tex2",
        std::bind(&SpectrumTexture::Scale::mutable_tex2, &scale));
  } else if (spectrum_texture_type == "uv") {
    auto& uv = *spectrum_texture.mutable_uv();
    TryRemoveUVParameters(parameters, uv);
  } else if (spectrum_texture_type == "windy") {
    spectrum_texture.mutable_windy();
  } else if (spectrum_texture_type == "wrinkled") {
    auto& wrinkled = *spectrum_texture.mutable_wrinkled();

    if (std::optional<int32_t> octaves =
            TryRemoveInteger(parameters, "octaves");
        octaves) {
      wrinkled.set_octaves(*octaves);
    }

    if (std::optional<double> roughness =
            TryRemoveFloat(parameters, "roughness");
        roughness) {
      wrinkled.set_roughness(*roughness);
    }
  } else {
    std::cerr << "Unrecognized Texture type: \"" << spectrum_texture_type
              << "\"" << std::endl;
  }

  return absl::OkStatus();
}

absl::Status ParserV3::Transform(double m00, double m01, double m02, double m03,
                                 double m10, double m11, double m12, double m13,
                                 double m20, double m21, double m22, double m23,
                                 double m30, double m31, double m32,
                                 double m33) {
  auto& transform = *output_.add_directives()->mutable_transform();
  transform.set_m00(m00);
  transform.set_m01(m01);
  transform.set_m02(m02);
  transform.set_m03(m03);
  transform.set_m10(m10);
  transform.set_m11(m11);
  transform.set_m12(m12);
  transform.set_m13(m13);
  transform.set_m20(m20);
  transform.set_m21(m21);
  transform.set_m22(m22);
  transform.set_m23(m23);
  transform.set_m30(m30);
  transform.set_m31(m31);
  transform.set_m32(m32);
  transform.set_m33(m33);
  return absl::OkStatus();
}

absl::Status ParserV3::TransformTimes(double start_time, double end_time) {
  auto& transform_times = *output_.add_directives()->mutable_transform_times();
  transform_times.set_start_time(start_time);
  transform_times.set_end_time(end_time);
  return absl::OkStatus();
}

absl::Status ParserV3::TransformBegin() {
  output_.add_directives()->mutable_transform_begin();
  return absl::OkStatus();
}

absl::Status ParserV3::TransformEnd() {
  output_.add_directives()->mutable_transform_end();
  return absl::OkStatus();
}

absl::Status ParserV3::Translate(double x, double y, double z) {
  auto& translate = *output_.add_directives()->mutable_translate();
  translate.set_x(x);
  translate.set_y(y);
  translate.set_z(z);
  return absl::OkStatus();
}

absl::Status ParserV3::WorldBegin() {
  output_.add_directives()->mutable_world_begin();
  return absl::OkStatus();
}

absl::Status ParserV3::WorldEnd() {
  output_.add_directives()->mutable_world_end();
  return absl::OkStatus();
}

}  // namespace

absl::Status Convert(std::istream& input, PbrtProto& output) {
  return ParserV3(output).ReadFrom(input);
}

absl::StatusOr<PbrtProto> Convert(std::istream& input) {
  PbrtProto output;
  if (absl::Status error = Convert(input, output); !error.ok()) {
    return error;
  }
  return output;
}

}  // namespace pbrt_proto::v3
