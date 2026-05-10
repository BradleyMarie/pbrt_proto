#include "pbrt_proto/shared/materials.h"

#include <functional>

#include "absl/container/flat_hash_map.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/common.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {

void RemoveBuiltInMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    BuiltInMaterial& output) {
  TryRemoveFloatTexture(parameters, "bumpmap",
                        std::bind(&BuiltInMaterial::mutable_bumpmap, &output));
}

void RemoveMirrorMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    MirrorMaterial& output) {
  TryRemoveSpectrumTextureV1(parameters, "Kr",
                             std::bind(&MirrorMaterial::mutable_kr, &output));
  TryRemoveFloatTexture(parameters, "bumpmap",
                        std::bind(&MirrorMaterial::mutable_bumpmap, &output));
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
  RemoveUberMaterialV1(parameters, output);
  TryRemoveSpectrumTextureV1(parameters, "Kt",
                             std::bind(&UberMaterial::mutable_kt, &output));
  TryRemoveFloatTexture(parameters, "index",
                        std::bind(&UberMaterial::mutable_eta, &output));
}

void RemoveUberMaterialV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    UberMaterial& output) {
  RemoveUberMaterialV1(parameters, output);

  TryRemoveSpectrumTextureV1(parameters, "Kt",
                             std::bind(&UberMaterial::mutable_kt, &output));

  TryRemoveFloatTexture(parameters, "eta",
                        std::bind(&UberMaterial::mutable_eta, &output));
  if (!output.has_eta()) {
    TryRemoveFloatTexture(parameters, "index",
                          std::bind(&UberMaterial::mutable_eta, &output));
  }

  TryRemoveFloatTexture(parameters, "uroughness",
                        std::bind(&UberMaterial::mutable_uroughness, &output));
  TryRemoveFloatTexture(parameters, "vroughness",
                        std::bind(&UberMaterial::mutable_vroughness, &output));

  if (std::optional<bool> remaproughness =
          TryRemoveBool(parameters, "remaproughness");
      remaproughness) {
    output.set_remaproughness(*remaproughness);
  }
}

}  // namespace pbrt_proto
