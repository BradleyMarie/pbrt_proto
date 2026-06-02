#ifndef _PBRT_PROTO_SHARED_MATERIALS_
#define _PBRT_PROTO_SHARED_MATERIALS_

#include "absl/container/flat_hash_map.h"
#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {

void RemoveBuiltInMaterialV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    BuiltInMaterial& output);

void RemoveDisneyMaterialV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DisneyMaterial& output);

void RemoveGlassMaterialV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    GlassMaterial& output);

void RemoveGlassMaterialV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    GlassMaterial& output);

void RemoveHairMaterialV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    HairMaterial& output);

void RemoveKdSubsurfaceMaterialV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    KdSubsurfaceMaterial& output);

void RemoveKdSubsurfaceMaterialV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    KdSubsurfaceMaterial& output);

void RemoveMatteMaterialV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    MatteMaterial& output);

void RemoveMeasuredFourierMaterialV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    MeasuredFourierMaterial& output);

void RemoveMeasuredMerlMaterialV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    MeasuredMerlMaterial& output);

void RemoveMetalMaterialV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    MetalMaterial& output);

void RemoveMetalMaterialV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    MetalMaterial& output);

void RemoveMirrorMaterialV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    MirrorMaterial& output);

void RemoveMixMaterialV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    MixMaterial& output);

void RemovePlasticMaterialV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PlasticMaterial& output);

void RemovePlasticMaterialV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PlasticMaterial& output);

void RemoveShinyMetalMaterialV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ShinyMetalMaterial& output);

void RemoveSubstrateMaterialV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SubstrateMaterial& output);

void RemoveSubstrateMaterialV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SubstrateMaterial& output);

void RemoveSubsurfaceMaterialV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SubsurfaceMaterial& output);

void RemoveTranslucentMaterialV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    TranslucentMaterial& output);

void RemoveTranslucentMaterialV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    TranslucentMaterial& output);

void RemoveUberMaterialV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    UberMaterial& output);

void RemoveUberMaterialV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    UberMaterial& output);

void RemoveUberMaterialV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    UberMaterial& output);

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_MATERIALS_
