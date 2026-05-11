#include "pbrt_proto/shared/materials.h"

#include <functional>

#include "absl/container/flat_hash_map.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/common.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {
namespace {

template <typename T>
void RemoveEta(absl::flat_hash_map<absl::string_view, Parameter>& parameters,
               T& output, bool pbrt_v3) {
  if (pbrt_v3) {
    TryRemoveFloatTexture(parameters, "eta",
                          std::bind(&T::mutable_eta, &output));
  }

  if (!output.has_eta()) {
    TryRemoveFloatTexture(parameters, "index",
                          std::bind(&T::mutable_eta, &output));
  }
}

template <typename T>
void RemoveUVRoughness(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters, T& output) {
  TryRemoveFloatTexture(parameters, "uroughness",
                        std::bind(&T::mutable_uroughness, &output));
  TryRemoveFloatTexture(parameters, "vroughness",
                        std::bind(&T::mutable_vroughness, &output));

  if (std::optional<bool> remaproughness =
          TryRemoveBool(parameters, "remaproughness");
      remaproughness) {
    output.set_remaproughness(*remaproughness);
  }
}

void RemoveGlassMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    GlassMaterial& output, bool pbrt_v3) {
  RemoveGlassMaterialV1(parameters, output);
  RemoveEta(parameters, output, pbrt_v3);
}

void RemoveKdSubsurfaceMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    KdSubsurfaceMaterial& output, bool pbrt_v3) {
  TryRemoveSpectrumTextureV1(
      parameters, "Kd", std::bind(&KdSubsurfaceMaterial::mutable_kd, &output));
  TryRemoveSpectrumTextureV1(
      parameters, "Kr", std::bind(&KdSubsurfaceMaterial::mutable_kr, &output));

  if (pbrt_v3) {
    if (std::optional<double> eta = TryRemoveFloat(parameters, "eta"); eta) {
      output.mutable_eta()->set_float_value(*eta);
    }

    TryRemoveSpectrumTextureV1(
        parameters, "mfp",
        std::bind(&KdSubsurfaceMaterial::mutable_mfp, &output));
  } else {
    TryRemoveFloatTexture(
        parameters, "index",
        std::bind(&KdSubsurfaceMaterial::mutable_eta, &output));
    TryRemoveFloatTexture(
        parameters, "meanfreepath",
        std::bind(&KdSubsurfaceMaterial::mutable_meanfreepath, &output));
  }

  TryRemoveFloatTexture(
      parameters, "bumpmap",
      std::bind(&KdSubsurfaceMaterial::mutable_bumpmap, &output));
}

void RemoveUberMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    UberMaterial& output, bool pbrt_v3) {
  RemoveUberMaterialV1(parameters, output);
  TryRemoveSpectrumTextureV1(parameters, "Kt",
                             std::bind(&UberMaterial::mutable_kt, &output));
  RemoveEta(parameters, output, pbrt_v3);
}

}  // namespace

void RemoveBuiltInMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    BuiltInMaterial& output) {
  TryRemoveFloatTexture(parameters, "bumpmap",
                        std::bind(&BuiltInMaterial::mutable_bumpmap, &output));
}

void RemoveDisneyMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DisneyMaterial& output) {
  TryRemoveFloatTexture(parameters, "roughness",
                        std::bind(&DisneyMaterial::mutable_roughness, &output));
  TryRemoveFloatTexture(parameters, "eta",
                        std::bind(&DisneyMaterial::mutable_eta, &output));
  TryRemoveFloatTexture(parameters, "bumpmap",
                        std::bind(&DisneyMaterial::mutable_bumpmap, &output));
  TryRemoveSpectrumTextureV1(
      parameters, "color", std::bind(&DisneyMaterial::mutable_color, &output));
  TryRemoveSpectrumTextureV1(
      parameters, "scatterdistance",
      std::bind(&DisneyMaterial::mutable_scatterdistance, &output));
  TryRemoveFloatTexture(
      parameters, "anisotropic",
      std::bind(&DisneyMaterial::mutable_anisotropic, &output));
  TryRemoveFloatTexture(parameters, "clearcoat",
                        std::bind(&DisneyMaterial::mutable_clearcoat, &output));
  TryRemoveFloatTexture(
      parameters, "clearcoatgloss",
      std::bind(&DisneyMaterial::mutable_clearcoatgloss, &output));
  TryRemoveFloatTexture(parameters, "difftrans",
                        std::bind(&DisneyMaterial::mutable_difftrans, &output));
  TryRemoveFloatTexture(parameters, "flatness",
                        std::bind(&DisneyMaterial::mutable_flatness, &output));
  TryRemoveFloatTexture(parameters, "metallic",
                        std::bind(&DisneyMaterial::mutable_metallic, &output));
  TryRemoveFloatTexture(parameters, "spectrans",
                        std::bind(&DisneyMaterial::mutable_spectrans, &output));
  TryRemoveFloatTexture(
      parameters, "speculartint",
      std::bind(&DisneyMaterial::mutable_speculartint, &output));
  TryRemoveFloatTexture(parameters, "sheen",
                        std::bind(&DisneyMaterial::mutable_sheen, &output));
  TryRemoveFloatTexture(parameters, "sheentint",
                        std::bind(&DisneyMaterial::mutable_sheentint, &output));

  if (std::optional<bool> thin = TryRemoveBool(parameters, "thin"); thin) {
    output.set_thin(*thin);
  }
}

void RemoveGlassMaterialV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    GlassMaterial& output) {
  TryRemoveSpectrumTextureV1(parameters, "Kr",
                             std::bind(&GlassMaterial::mutable_kr, &output));
  TryRemoveSpectrumTextureV1(parameters, "Kt",
                             std::bind(&GlassMaterial::mutable_kt, &output));
  TryRemoveFloatTexture(parameters, "index",
                        std::bind(&GlassMaterial::mutable_eta, &output));
  TryRemoveFloatTexture(parameters, "bumpmap",
                        std::bind(&GlassMaterial::mutable_bumpmap, &output));
}

void RemoveGlassMaterialV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    GlassMaterial& output) {
  RemoveGlassMaterial(parameters, output, /*pbrt_v3=*/true);
  RemoveUVRoughness(parameters, output);
}

void RemoveHairMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    HairMaterial& output) {
  TryRemoveFloatTexture(parameters, "eta",
                        std::bind(&HairMaterial::mutable_eta, &output));
  TryRemoveSpectrumTextureV1(
      parameters, "sigma_a",
      std::bind(&HairMaterial::mutable_sigma_a, &output));
  TryRemoveSpectrumTextureV1(parameters, "color",
                             std::bind(&HairMaterial::mutable_color, &output));
  TryRemoveFloatTexture(parameters, "eumelanin",
                        std::bind(&HairMaterial::mutable_eumelanin, &output));
  TryRemoveFloatTexture(parameters, "pheomelanin",
                        std::bind(&HairMaterial::mutable_pheomelanin, &output));
  TryRemoveFloatTexture(parameters, "beta_m",
                        std::bind(&HairMaterial::mutable_beta_m, &output));
  TryRemoveFloatTexture(parameters, "beta_n",
                        std::bind(&HairMaterial::mutable_beta_n, &output));
  TryRemoveFloatTexture(parameters, "alpha",
                        std::bind(&HairMaterial::mutable_alpha, &output));
}

void RemoveKdSubsurfaceMaterialV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    KdSubsurfaceMaterial& output) {
  RemoveKdSubsurfaceMaterial(parameters, output, /*pbrt_v3=*/false);
}

void RemoveKdSubsurfaceMaterialV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    KdSubsurfaceMaterial& output) {
  RemoveKdSubsurfaceMaterial(parameters, output, /*pbrt_v3=*/true);

  TryRemoveSpectrumTextureV1(
      parameters, "Kt", std::bind(&KdSubsurfaceMaterial::mutable_kt, &output));

  TryRemoveFloatTexture(
      parameters, "uroughness",
      std::bind(&KdSubsurfaceMaterial::mutable_uroughness, &output));

  TryRemoveFloatTexture(
      parameters, "vroughness",
      std::bind(&KdSubsurfaceMaterial::mutable_vroughness, &output));

  if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
      scale) {
    output.set_scale(*scale);
  }

  if (std::optional<double> g = TryRemoveFloat(parameters, "g"); g) {
    output.set_g(*g);
  }

  if (std::optional<bool> remaproughness =
          TryRemoveBool(parameters, "remaproughness");
      remaproughness) {
    output.set_remaproughness(*remaproughness);
  }
}

void RemoveMatteMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    MatteMaterial& output) {
  TryRemoveSpectrumTextureV1(parameters, "Kd",
                             std::bind(&MatteMaterial::mutable_kd, &output));
  TryRemoveFloatTexture(parameters, "sigma",
                        std::bind(&MatteMaterial::mutable_sigma, &output));
  TryRemoveFloatTexture(parameters, "bumpmap",
                        std::bind(&MatteMaterial::mutable_bumpmap, &output));
}

void RemoveMeasuredFourierMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    MeasuredFourierMaterial& output) {
  TryRemoveFloatTexture(
      parameters, "bumpmap",
      std::bind(&MeasuredFourierMaterial::mutable_bumpmap, &output));

  if (std::optional<absl::string_view> filename =
          TryRemoveString(parameters, "bsdffile");
      filename) {
    output.set_filename(*filename);
  }
}

void RemoveMeasuredMerlMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    MeasuredMerlMaterial& output) {
  TryRemoveFloatTexture(
      parameters, "bumpmap",
      std::bind(&MeasuredMerlMaterial::mutable_bumpmap, &output));

  if (std::optional<absl::string_view> filename =
          TryRemoveString(parameters, "filename");
      filename) {
    output.set_filename(*filename);
  }
}

void RemoveMetalMaterialV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    MetalMaterial& output) {
  TryRemoveFloatTexture(parameters, "roughness",
                        std::bind(&MetalMaterial::mutable_roughness, &output));
  TryRemoveFloatTexture(parameters, "bumpmap",
                        std::bind(&MetalMaterial::mutable_bumpmap, &output));
  TryRemoveSpectrumTextureV1(parameters, "eta",
                             std::bind(&MetalMaterial::mutable_eta, &output));
  TryRemoveSpectrumTextureV1(parameters, "k",
                             std::bind(&MetalMaterial::mutable_k, &output));
}

void RemoveMetalMaterialV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    MetalMaterial& output) {
  RemoveMetalMaterialV1(parameters, output);
  RemoveUVRoughness(parameters, output);
}

void RemoveMirrorMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    MirrorMaterial& output) {
  TryRemoveSpectrumTextureV1(parameters, "Kr",
                             std::bind(&MirrorMaterial::mutable_kr, &output));
  TryRemoveFloatTexture(parameters, "bumpmap",
                        std::bind(&MirrorMaterial::mutable_bumpmap, &output));
}

void RemoveMixMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    MixMaterial& output) {
  TryRemoveSpectrumTextureV1(parameters, "Kd",
                             std::bind(&MixMaterial::mutable_kd, &output));
  TryRemoveFloatTexture(parameters, "sigma",
                        std::bind(&MixMaterial::mutable_sigma, &output));
  TryRemoveFloatTexture(parameters, "bumpmap",
                        std::bind(&MixMaterial::mutable_bumpmap, &output));
  TryRemoveSpectrumTextureV1(parameters, "amount",
                             std::bind(&MixMaterial::mutable_amount, &output));

  if (std::optional<absl::string_view> namedmaterial1 =
          TryRemoveString(parameters, "namedmaterial1");
      namedmaterial1) {
    output.set_namedmaterial1(*namedmaterial1);
  }

  if (std::optional<absl::string_view> namedmaterial2 =
          TryRemoveString(parameters, "namedmaterial2");
      namedmaterial2) {
    output.set_namedmaterial2(*namedmaterial2);
  }
}

void RemovePlasticMaterialV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PlasticMaterial& output) {
  TryRemoveSpectrumTextureV1(parameters, "Kd",
                             std::bind(&PlasticMaterial::mutable_kd, &output));
  TryRemoveSpectrumTextureV1(parameters, "Ks",
                             std::bind(&PlasticMaterial::mutable_ks, &output));
  TryRemoveFloatTexture(
      parameters, "roughness",
      std::bind(&PlasticMaterial::mutable_roughness, &output));
  TryRemoveFloatTexture(parameters, "bumpmap",
                        std::bind(&PlasticMaterial::mutable_bumpmap, &output));
}

void RemovePlasticMaterialV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PlasticMaterial& output) {
  RemovePlasticMaterialV1(parameters, output);

  if (std::optional<bool> remaproughness =
          TryRemoveBool(parameters, "remaproughness");
      remaproughness) {
    output.set_remaproughness(*remaproughness);
  }
}

void RemoveShinyMetalMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ShinyMetalMaterial& output) {
  TryRemoveSpectrumTextureV1(
      parameters, "Ks", std::bind(&ShinyMetalMaterial::mutable_ks, &output));
  TryRemoveSpectrumTextureV1(
      parameters, "Kr", std::bind(&ShinyMetalMaterial::mutable_kr, &output));
  TryRemoveFloatTexture(
      parameters, "roughness",
      std::bind(&ShinyMetalMaterial::mutable_roughness, &output));
  TryRemoveFloatTexture(
      parameters, "bumpmap",
      std::bind(&ShinyMetalMaterial::mutable_bumpmap, &output));
}

void RemoveSubstrateMaterialV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SubstrateMaterial& output) {
  TryRemoveSpectrumTextureV1(
      parameters, "Kd", std::bind(&SubstrateMaterial::mutable_kd, &output));
  TryRemoveSpectrumTextureV1(
      parameters, "Ks", std::bind(&SubstrateMaterial::mutable_ks, &output));
  TryRemoveFloatTexture(
      parameters, "bumpmap",
      std::bind(&SubstrateMaterial::mutable_bumpmap, &output));
  TryRemoveFloatTexture(
      parameters, "uroughness",
      std::bind(&SubstrateMaterial::mutable_uroughness, &output));
  TryRemoveFloatTexture(
      parameters, "vroughness",
      std::bind(&SubstrateMaterial::mutable_vroughness, &output));
}

void RemoveSubstrateMaterialV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SubstrateMaterial& output) {
  RemoveSubstrateMaterialV1(parameters, output);

  if (std::optional<bool> remaproughness =
          TryRemoveBool(parameters, "remaproughness");
      remaproughness) {
    output.set_remaproughness(*remaproughness);
  }
}

void RemoveSubsurfaceMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SubsurfaceMaterial& output) {
  TryRemoveSpectrumTextureV1(
      parameters, "Kr", std::bind(&SubsurfaceMaterial::mutable_kr, &output));
  TryRemoveSpectrumTextureV1(
      parameters, "Kt", std::bind(&SubsurfaceMaterial::mutable_kt, &output));
  TryRemoveFloatTexture(
      parameters, "bumpmap",
      std::bind(&SubsurfaceMaterial::mutable_bumpmap, &output));
  TryRemoveFloatTexture(
      parameters, "uroughness",
      std::bind(&SubsurfaceMaterial::mutable_uroughness, &output));
  TryRemoveFloatTexture(
      parameters, "vroughness",
      std::bind(&SubsurfaceMaterial::mutable_vroughness, &output));

  if (std::optional<bool> remaproughness =
          TryRemoveBool(parameters, "remaproughness");
      remaproughness) {
    output.set_remaproughness(*remaproughness);
  }

  if (std::optional<double> g = TryRemoveFloat(parameters, "g"); g) {
    output.set_g(*g);
  }

  if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
      scale) {
    output.set_scale(*scale);
  }

  if (std::optional<double> eta = TryRemoveFloat(parameters, "eta"); eta) {
    output.set_eta(*eta);
  }

  TryRemoveSpectrumTextureV1(
      parameters, "sigma_a",
      std::bind(&SubsurfaceMaterial::mutable_sigma_a, &output));

  TryRemoveSpectrumTextureV1(
      parameters, "sigma_s",
      std::bind(&SubsurfaceMaterial::mutable_sigma_s, &output));

  if (std::optional<absl::string_view> name =
          TryRemoveString(parameters, "name");
      name.has_value()) {
    auto iter = kNamedMeasuredScatteringPresets.find(*name);
    if (iter != kNamedMeasuredScatteringPresets.end()) {
      output.set_name(iter->second);
    }
  }
}

void RemoveTranslucentMaterialV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    TranslucentMaterial& output) {
  TryRemoveSpectrumTextureV1(
      parameters, "Kd", std::bind(&TranslucentMaterial::mutable_kd, &output));
  TryRemoveSpectrumTextureV1(
      parameters, "Ks", std::bind(&TranslucentMaterial::mutable_ks, &output));
  TryRemoveSpectrumTextureV1(
      parameters, "reflect",
      std::bind(&TranslucentMaterial::mutable_reflect, &output));
  TryRemoveSpectrumTextureV1(
      parameters, "transmit",
      std::bind(&TranslucentMaterial::mutable_transmit, &output));
  TryRemoveFloatTexture(
      parameters, "roughness",
      std::bind(&TranslucentMaterial::mutable_roughness, &output));
  TryRemoveFloatTexture(
      parameters, "bumpmap",
      std::bind(&TranslucentMaterial::mutable_bumpmap, &output));
}

void RemoveTranslucentMaterialV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    TranslucentMaterial& output) {
  RemoveTranslucentMaterialV1(parameters, output);

  if (std::optional<bool> remaproughness =
          TryRemoveBool(parameters, "remaproughness");
      remaproughness) {
    output.set_remaproughness(*remaproughness);
  }
}

void RemoveUberMaterialV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    UberMaterial& output) {
  TryRemoveSpectrumTextureV1(parameters, "Kd",
                             std::bind(&UberMaterial::mutable_kd, &output));
  TryRemoveSpectrumTextureV1(parameters, "Ks",
                             std::bind(&UberMaterial::mutable_ks, &output));
  TryRemoveSpectrumTextureV1(parameters, "Kr",
                             std::bind(&UberMaterial::mutable_kr, &output));
  TryRemoveSpectrumTextureV1(
      parameters, "opacity",
      std::bind(&UberMaterial::mutable_opacity, &output));
  TryRemoveFloatTexture(parameters, "roughness",
                        std::bind(&UberMaterial::mutable_roughness, &output));
  TryRemoveFloatTexture(parameters, "bumpmap",
                        std::bind(&UberMaterial::mutable_bumpmap, &output));
}

void RemoveUberMaterialV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    UberMaterial& output) {
  RemoveUberMaterial(parameters, output, /*pbrt_v3=*/false);
}

void RemoveUberMaterialV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    UberMaterial& output) {
  RemoveUberMaterial(parameters, output, /*pbrt_v3=*/true);
  RemoveUVRoughness(parameters, output);
}

}  // namespace pbrt_proto
