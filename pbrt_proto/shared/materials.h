#ifndef _PBRT_PROTO_SHARED_MATERIALS_
#define _PBRT_PROTO_SHARED_MATERIALS_

#include "absl/container/flat_hash_map.h"
#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {

absl::Status RemoveBuiltInMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, BuiltInMaterial& output);

absl::Status RemoveDisneyMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, DisneyMaterial& output);

absl::Status RemoveGlassMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, GlassMaterial& output);

absl::Status RemoveHairMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, HairMaterial& output);

absl::Status RemoveKdSubsurfaceMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, KdSubsurfaceMaterial& output);

absl::Status RemoveMatteMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, MatteMaterial& output);

absl::Status RemoveMeasuredFourierMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, MeasuredFourierMaterial& output);

absl::Status RemoveMeasuredMerlMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, MeasuredMerlMaterial& output);

absl::Status RemoveMetalMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, MetalMaterial& output);

absl::Status RemoveMirrorMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, MirrorMaterial& output);

absl::Status RemoveMixMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, MixMaterial& output);

absl::Status RemovePlasticMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, PlasticMaterial& output);

absl::Status RemoveShinyMetalMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, ShinyMetalMaterial& output);

absl::Status RemoveSubstrateMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, SubstrateMaterial& output);

absl::Status RemoveSubsurfaceMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, SubsurfaceMaterial& output);

absl::Status RemoveTranslucentMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, TranslucentMaterial& output);

absl::Status RemoveUberMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, UberMaterial& output);

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_MATERIALS_
