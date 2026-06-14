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
absl::Status RemoveColor(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, T& output) {
  return TryRemoveSpectrumTexture(parameters, "color",
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
               int pbrt_version, T& output) {
  if (pbrt_version >= 3) {
    RemoveEta(parameters, output);
  }

  if (!output.has_eta()) {
    RemoveIndex(parameters, output);
  }
}

template <typename T>
absl::Status RemoveKd(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, T& output) {
  return TryRemoveSpectrumTexture(parameters, "Kd",
                                  std::bind(&T::mutable_kd, &output));
}

template <typename T>
absl::Status RemoveKr(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, T& output) {
  return TryRemoveSpectrumTexture(parameters, "Kr",
                                  std::bind(&T::mutable_kr, &output));
}

template <typename T>
absl::Status RemoveKs(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, T& output) {
  return TryRemoveSpectrumTexture(parameters, "Ks",
                                  std::bind(&T::mutable_ks, &output));
}

template <typename T>
absl::Status RemoveKt(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, T& output) {
  return TryRemoveSpectrumTexture(parameters, "Kt",
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
absl::Status RemoveSigmaA(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, T& output) {
  return TryRemoveSpectrumTexture(parameters, "sigma_a",
                                  std::bind(&T::mutable_sigma_a, &output));
}

template <typename T>
absl::Status RemoveSigmaS(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, T& output) {
  return TryRemoveSpectrumTexture(parameters, "sigma_s",
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

absl::Status RemoveBuiltInMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, BuiltInMaterial& output) {
  RemoveBumpmap(parameters, output);
  return absl::OkStatus();
}

absl::Status RemoveDisneyMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, DisneyMaterial& output) {
  RemoveRoughness(parameters, output);
  RemoveEta(parameters, output);
  RemoveBumpmap(parameters, output);

  if (std::optional<bool> thin = TryRemoveBool(parameters, "thin"); thin) {
    output.set_thin(*thin);
  }

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

  if (absl::Status status = RemoveColor(parameters, pbrt_version, output);
      !status.ok()) {
    return status;
  }

  return TryRemoveSpectrumTexture(
      parameters, "scatterdistance",
      std::bind(&DisneyMaterial::mutable_scatterdistance, &output));
}

absl::Status RemoveGlassMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, GlassMaterial& output) {
  RemoveEta(parameters, pbrt_version, output);
  RemoveBumpmap(parameters, output);

  if (absl::Status status = RemoveKr(parameters, pbrt_version, output);
      !status.ok()) {
    return status;
  }

  if (absl::Status status = RemoveKt(parameters, pbrt_version, output);
      !status.ok()) {
    return status;
  }

  if (pbrt_version >= 3) {
    RemoveUVRoughness(parameters, output);
    RemoveRemapRoughness(parameters, output);
  }

  return absl::OkStatus();
}

absl::Status RemoveHairMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, HairMaterial& output) {
  RemoveEta(parameters, output);

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

  if (absl::Status status = RemoveColor(parameters, pbrt_version, output);
      !status.ok()) {
    return status;
  }

  return RemoveSigmaA(parameters, pbrt_version, output);
}

absl::Status RemoveKdSubsurfaceMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, KdSubsurfaceMaterial& output) {
  RemoveBumpmap(parameters, output);

  TryRemoveFloatTexture(
      parameters, "meanfreepath",
      std::bind(&KdSubsurfaceMaterial::mutable_meanfreepath, &output));

  if (absl::Status status = RemoveKd(parameters, pbrt_version, output);
      !status.ok()) {
    return status;
  }

  if (absl::Status status = RemoveKr(parameters, pbrt_version, output);
      !status.ok()) {
    return status;
  }

  if (pbrt_version <= 2) {
    RemoveIndex(parameters, output);
  }

  if (pbrt_version >= 3) {
    RemoveEta(parameters, output);
    RemoveUVRoughness(parameters, output);
    RemoveRemapRoughness(parameters, output);

    if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
        scale) {
      output.set_scale(*scale);
    }

    if (std::optional<double> g = TryRemoveFloat(parameters, "g"); g) {
      output.set_g(*g);
    }

    if (absl::Status status = RemoveKt(parameters, pbrt_version, output);
        !status.ok()) {
      return status;
    }

    if (absl::Status status = TryRemoveSpectrumTexture(
            parameters, "mfp",
            std::bind(&KdSubsurfaceMaterial::mutable_mfp, &output));
        !status.ok()) {
      return status;
    }
  }

  return absl::OkStatus();
}

absl::Status RemoveMatteMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, MatteMaterial& output) {
  RemoveSigma(parameters, output);
  RemoveBumpmap(parameters, output);
  return RemoveKd(parameters, pbrt_version, output);
}

absl::Status RemoveMeasuredFourierMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, MeasuredFourierMaterial& output) {
  RemoveBumpmap(parameters, output);

  if (std::optional<absl::string_view> filename =
          TryRemoveString(parameters, "bsdffile");
      filename) {
    output.set_filename(*filename);
  }

  return absl::OkStatus();
}

absl::Status RemoveMeasuredMerlMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, MeasuredMerlMaterial& output) {
  RemoveBumpmap(parameters, output);

  if (std::optional<absl::string_view> filename =
          TryRemoveString(parameters, "filename");
      filename) {
    output.set_filename(*filename);
  }

  return absl::OkStatus();
}

absl::Status RemoveMetalMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, MetalMaterial& output) {
  RemoveRoughness(parameters, output);
  RemoveBumpmap(parameters, output);

  if (absl::Status status = TryRemoveSpectrumTexture(
          parameters, "eta", std::bind(&MetalMaterial::mutable_eta, &output));
      !status.ok()) {
    return status;
  }

  if (absl::Status status = TryRemoveSpectrumTexture(
          parameters, "k", std::bind(&MetalMaterial::mutable_k, &output));
      !status.ok()) {
    return status;
  }

  if (pbrt_version >= 3) {
    RemoveUVRoughness(parameters, output);
    RemoveRemapRoughness(parameters, output);
  }

  return absl::OkStatus();
}

absl::Status RemoveMirrorMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, MirrorMaterial& output) {
  RemoveBumpmap(parameters, output);
  return RemoveKr(parameters, pbrt_version, output);
}

absl::Status RemoveMixMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, MixMaterial& output) {
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

  if (absl::Status status = RemoveKd(parameters, pbrt_version, output);
      !status.ok()) {
    return status;
  }

  return TryRemoveSpectrumTexture(
      parameters, "amount", std::bind(&MixMaterial::mutable_amount, &output));
}

absl::Status RemovePlasticMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, PlasticMaterial& output) {
  RemoveRoughness(parameters, output);
  RemoveBumpmap(parameters, output);

  if (absl::Status status = RemoveKd(parameters, pbrt_version, output);
      !status.ok()) {
    return status;
  }

  if (absl::Status status = RemoveKs(parameters, pbrt_version, output);
      !status.ok()) {
    return status;
  }

  if (pbrt_version >= 3) {
    RemoveRemapRoughness(parameters, output);
  }

  return absl::OkStatus();
}

absl::Status RemoveShinyMetalMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, ShinyMetalMaterial& output) {
  RemoveRoughness(parameters, output);
  RemoveBumpmap(parameters, output);

  if (absl::Status status = RemoveKs(parameters, pbrt_version, output);
      !status.ok()) {
    return status;
  }

  if (absl::Status status = RemoveKr(parameters, pbrt_version, output);
      !status.ok()) {
    return status;
  }

  return absl::OkStatus();
}

absl::Status RemoveSubstrateMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, SubstrateMaterial& output) {
  RemoveUVRoughness(parameters, output);
  RemoveBumpmap(parameters, output);

  if (absl::Status status = RemoveKs(parameters, pbrt_version, output);
      !status.ok()) {
    return status;
  }

  if (absl::Status status = RemoveKd(parameters, pbrt_version, output);
      !status.ok()) {
    return status;
  }

  if (pbrt_version >= 3) {
    RemoveRemapRoughness(parameters, output);
  }

  return absl::OkStatus();
}

absl::Status RemoveSubsurfaceMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, SubsurfaceMaterial& output) {
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

  if (absl::Status status = RemoveKr(parameters, pbrt_version, output);
      !status.ok()) {
    return status;
  }

  if (absl::Status status = RemoveKt(parameters, pbrt_version, output);
      !status.ok()) {
    return status;
  }

  if (absl::Status status = RemoveSigmaA(parameters, pbrt_version, output);
      !status.ok()) {
    return status;
  }

  if (absl::Status status = RemoveSigmaS(parameters, pbrt_version, output);
      !status.ok()) {
    return status;
  }

  return absl::OkStatus();
}

absl::Status RemoveTranslucentMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, TranslucentMaterial& output) {
  RemoveRoughness(parameters, output);
  RemoveBumpmap(parameters, output);

  if (absl::Status status = TryRemoveSpectrumTexture(
          parameters, "reflect",
          std::bind(&TranslucentMaterial::mutable_reflect, &output));
      !status.ok()) {
    return status;
  }

  if (absl::Status status = TryRemoveSpectrumTexture(
          parameters, "transmit",
          std::bind(&TranslucentMaterial::mutable_transmit, &output));
      !status.ok()) {
    return status;
  }

  if (absl::Status status = RemoveKd(parameters, pbrt_version, output);
      !status.ok()) {
    return status;
  }

  if (absl::Status status = RemoveKs(parameters, pbrt_version, output);
      !status.ok()) {
    return status;
  }

  if (pbrt_version >= 3) {
    RemoveRemapRoughness(parameters, output);
  }

  return absl::OkStatus();
}

absl::Status RemoveUberMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, UberMaterial& output) {
  RemoveRoughness(parameters, output);
  RemoveBumpmap(parameters, output);

  if (absl::Status status = RemoveKd(parameters, pbrt_version, output);
      !status.ok()) {
    return status;
  }

  if (absl::Status status = RemoveKr(parameters, pbrt_version, output);
      !status.ok()) {
    return status;
  }

  if (absl::Status status = RemoveKs(parameters, pbrt_version, output);
      !status.ok()) {
    return status;
  }

  if (absl::Status status = TryRemoveSpectrumTexture(
          parameters, "opacity",
          std::bind(&UberMaterial::mutable_opacity, &output));
      !status.ok()) {
    return status;
  }

  if (pbrt_version >= 2) {
    RemoveEta(parameters, pbrt_version, output);

    if (absl::Status status = RemoveKt(parameters, pbrt_version, output);
        !status.ok()) {
      return status;
    }
  }

  if (pbrt_version >= 3) {
    RemoveUVRoughness(parameters, output);
    RemoveRemapRoughness(parameters, output);
  }

  return absl::OkStatus();
}

}  // namespace pbrt_proto
