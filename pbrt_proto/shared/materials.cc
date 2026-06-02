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
void RemoveBumpmap(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters, T& output) {
  TryRemoveFloatTexture(parameters, "bumpmap",
                        std::bind(&T::mutable_bumpmap, &output));
}

template <typename T>
void RemoveColor(absl::flat_hash_map<absl::string_view, Parameter>& parameters,
                 T& output) {
  TryRemoveSpectrumTextureV1(parameters, "color",
                             std::bind(&T::mutable_color, &output));
}

template <typename T>
void RemoveEta(absl::flat_hash_map<absl::string_view, Parameter>& parameters,
               T& output) {
  TryRemoveFloatTexture(parameters, "eta", std::bind(&T::mutable_eta, &output));
}

template <typename T>
void RemoveIndex(absl::flat_hash_map<absl::string_view, Parameter>& parameters,
                 T& output) {
  TryRemoveFloatTexture(parameters, "index",
                        std::bind(&T::mutable_eta, &output));
}

template <typename T>
void RemoveEta(absl::flat_hash_map<absl::string_view, Parameter>& parameters,
               T& output, bool pbrt_v3) {
  if (pbrt_v3) {
    RemoveEta(parameters, output);
  }

  if (!output.has_eta()) {
    RemoveIndex(parameters, output);
  }
}

template <typename T>
void RemoveKd(absl::flat_hash_map<absl::string_view, Parameter>& parameters,
              T& output) {
  TryRemoveSpectrumTextureV1(parameters, "Kd",
                             std::bind(&T::mutable_kd, &output));
}

template <typename T>
void RemoveKr(absl::flat_hash_map<absl::string_view, Parameter>& parameters,
              T& output) {
  TryRemoveSpectrumTextureV1(parameters, "Kr",
                             std::bind(&T::mutable_kr, &output));
}

template <typename T>
void RemoveKs(absl::flat_hash_map<absl::string_view, Parameter>& parameters,
              T& output) {
  TryRemoveSpectrumTextureV1(parameters, "Ks",
                             std::bind(&T::mutable_ks, &output));
}

template <typename T>
void RemoveKt(absl::flat_hash_map<absl::string_view, Parameter>& parameters,
              T& output) {
  TryRemoveSpectrumTextureV1(parameters, "Kt",
                             std::bind(&T::mutable_kt, &output));
}

template <typename T>
void RemoveRoughness(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters, T& output) {
  TryRemoveFloatTexture(parameters, "roughness",
                        std::bind(&T::mutable_roughness, &output));
}

template <typename T>
void RemoveSigma(absl::flat_hash_map<absl::string_view, Parameter>& parameters,
                 T& output) {
  TryRemoveFloatTexture(parameters, "sigma",
                        std::bind(&T::mutable_sigma, &output));
}

template <typename T>
void RemoveSigmaA(absl::flat_hash_map<absl::string_view, Parameter>& parameters,
                  T& output) {
  TryRemoveSpectrumTextureV1(parameters, "sigma_a",
                             std::bind(&T::mutable_sigma_a, &output));
}

template <typename T>
void RemoveSigmaS(absl::flat_hash_map<absl::string_view, Parameter>& parameters,
                  T& output) {
  TryRemoveSpectrumTextureV1(parameters, "sigma_s",
                             std::bind(&T::mutable_sigma_s, &output));
}

template <typename T>
void RemoveUVRoughness(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters, T& output) {
  TryRemoveFloatTexture(parameters, "uroughness",
                        std::bind(&T::mutable_uroughness, &output));
  TryRemoveFloatTexture(parameters, "vroughness",
                        std::bind(&T::mutable_vroughness, &output));
}

template <typename T>
void RemoveRemapRoughness(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters, T& output) {
  if (std::optional<bool> remaproughness =
          TryRemoveBool(parameters, "remaproughness");
      remaproughness) {
    output.set_remaproughness(*remaproughness);
  }
}

}  // namespace

void RemoveBuiltInMaterialV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    BuiltInMaterial& output) {
  RemoveBumpmap(parameters, output);
}

void RemoveDisneyMaterialV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DisneyMaterial& output) {
  RemoveRoughness(parameters, output);
  RemoveEta(parameters, output);
  RemoveColor(parameters, output);
  RemoveBumpmap(parameters, output);

  if (std::optional<bool> thin = TryRemoveBool(parameters, "thin"); thin) {
    output.set_thin(*thin);
  }

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
}

void RemoveGlassMaterialV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    GlassMaterial& output) {
  RemoveKr(parameters, output);
  RemoveKt(parameters, output);
  RemoveEta(parameters, output, /*pbrt_v3=*/false);
  RemoveBumpmap(parameters, output);
}

void RemoveGlassMaterialV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    GlassMaterial& output) {
  RemoveKr(parameters, output);
  RemoveKt(parameters, output);
  RemoveEta(parameters, output, /*pbrt_v3=*/true);
  RemoveBumpmap(parameters, output);
  RemoveUVRoughness(parameters, output);
  RemoveRemapRoughness(parameters, output);
}

void RemoveHairMaterialV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    HairMaterial& output) {
  RemoveEta(parameters, output);
  RemoveColor(parameters, output);
  RemoveSigmaA(parameters, output);

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

void RemoveKdSubsurfaceMaterialV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    KdSubsurfaceMaterial& output) {
  RemoveKd(parameters, output);
  RemoveKr(parameters, output);
  RemoveIndex(parameters, output);
  RemoveBumpmap(parameters, output);

  TryRemoveFloatTexture(
      parameters, "meanfreepath",
      std::bind(&KdSubsurfaceMaterial::mutable_meanfreepath, &output));
}

void RemoveKdSubsurfaceMaterialV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    KdSubsurfaceMaterial& output) {
  RemoveKd(parameters, output);
  RemoveKr(parameters, output);
  RemoveKt(parameters, output);
  RemoveEta(parameters, output);
  RemoveBumpmap(parameters, output);
  RemoveUVRoughness(parameters, output);
  RemoveRemapRoughness(parameters, output);

  if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
      scale) {
    output.set_scale(*scale);
  }

  if (std::optional<double> g = TryRemoveFloat(parameters, "g"); g) {
    output.set_g(*g);
  }

  TryRemoveSpectrumTextureV1(
      parameters, "mfp",
      std::bind(&KdSubsurfaceMaterial::mutable_mfp, &output));
}

void RemoveMatteMaterialV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    MatteMaterial& output) {
  RemoveKd(parameters, output);
  RemoveSigma(parameters, output);
  RemoveBumpmap(parameters, output);
}

void RemoveMeasuredFourierMaterialV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    MeasuredFourierMaterial& output) {
  RemoveBumpmap(parameters, output);

  if (std::optional<absl::string_view> filename =
          TryRemoveString(parameters, "bsdffile");
      filename) {
    output.set_filename(*filename);
  }
}

void RemoveMeasuredMerlMaterialV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    MeasuredMerlMaterial& output) {
  RemoveBumpmap(parameters, output);

  if (std::optional<absl::string_view> filename =
          TryRemoveString(parameters, "filename");
      filename) {
    output.set_filename(*filename);
  }
}

void RemoveMetalMaterialV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    MetalMaterial& output) {
  RemoveRoughness(parameters, output);
  RemoveBumpmap(parameters, output);

  TryRemoveSpectrumTextureV1(parameters, "eta",
                             std::bind(&MetalMaterial::mutable_eta, &output));
  TryRemoveSpectrumTextureV1(parameters, "k",
                             std::bind(&MetalMaterial::mutable_k, &output));
}

void RemoveMetalMaterialV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    MetalMaterial& output) {
  RemoveMetalMaterialV2(parameters, output);
  RemoveUVRoughness(parameters, output);
  RemoveRemapRoughness(parameters, output);
}

void RemoveMirrorMaterialV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    MirrorMaterial& output) {
  RemoveKr(parameters, output);
  RemoveBumpmap(parameters, output);
}

void RemoveMixMaterialV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    MixMaterial& output) {
  RemoveKd(parameters, output);
  RemoveSigma(parameters, output);
  RemoveBumpmap(parameters, output);

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

  TryRemoveSpectrumTextureV1(parameters, "amount",
                             std::bind(&MixMaterial::mutable_amount, &output));
}

void RemovePlasticMaterialV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PlasticMaterial& output) {
  RemoveKd(parameters, output);
  RemoveKs(parameters, output);
  RemoveRoughness(parameters, output);
  RemoveBumpmap(parameters, output);
}

void RemovePlasticMaterialV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PlasticMaterial& output) {
  RemovePlasticMaterialV1(parameters, output);
  RemoveRemapRoughness(parameters, output);
}

void RemoveShinyMetalMaterialV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ShinyMetalMaterial& output) {
  RemoveKs(parameters, output);
  RemoveKr(parameters, output);
  RemoveRoughness(parameters, output);
  RemoveBumpmap(parameters, output);
}

void RemoveSubstrateMaterialV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SubstrateMaterial& output) {
  RemoveKd(parameters, output);
  RemoveKs(parameters, output);
  RemoveUVRoughness(parameters, output);
  RemoveBumpmap(parameters, output);
}

void RemoveSubstrateMaterialV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SubstrateMaterial& output) {
  RemoveSubstrateMaterialV1(parameters, output);
  RemoveRemapRoughness(parameters, output);
}

void RemoveSubsurfaceMaterialV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SubsurfaceMaterial& output) {
  RemoveKr(parameters, output);
  RemoveKt(parameters, output);
  RemoveSigmaA(parameters, output);
  RemoveSigmaS(parameters, output);
  RemoveBumpmap(parameters, output);
  RemoveUVRoughness(parameters, output);
  RemoveRemapRoughness(parameters, output);

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
  RemoveKd(parameters, output);
  RemoveKs(parameters, output);
  RemoveRoughness(parameters, output);
  RemoveBumpmap(parameters, output);

  TryRemoveSpectrumTextureV1(
      parameters, "reflect",
      std::bind(&TranslucentMaterial::mutable_reflect, &output));
  TryRemoveSpectrumTextureV1(
      parameters, "transmit",
      std::bind(&TranslucentMaterial::mutable_transmit, &output));
}

void RemoveTranslucentMaterialV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    TranslucentMaterial& output) {
  RemoveTranslucentMaterialV1(parameters, output);
  RemoveRemapRoughness(parameters, output);
}

void RemoveUberMaterialV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    UberMaterial& output) {
  RemoveKd(parameters, output);
  RemoveKr(parameters, output);
  RemoveKs(parameters, output);
  RemoveRoughness(parameters, output);
  RemoveBumpmap(parameters, output);

  TryRemoveSpectrumTextureV1(
      parameters, "opacity",
      std::bind(&UberMaterial::mutable_opacity, &output));
}

void RemoveUberMaterialV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    UberMaterial& output) {
  RemoveUberMaterialV1(parameters, output);
  RemoveKt(parameters, output);
  RemoveEta(parameters, output, /*pbrt_v3=*/false);
}

void RemoveUberMaterialV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    UberMaterial& output) {
  RemoveUberMaterialV1(parameters, output);
  RemoveKt(parameters, output);
  RemoveEta(parameters, output, /*pbrt_v3=*/true);
  RemoveUVRoughness(parameters, output);
  RemoveRemapRoughness(parameters, output);
}

}  // namespace pbrt_proto
