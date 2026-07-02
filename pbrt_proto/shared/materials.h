#ifndef _PBRT_PROTO_SHARED_MATERIALS_
#define _PBRT_PROTO_SHARED_MATERIALS_

#include "absl/container/flat_hash_map.h"
#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {

absl::Status RemoveBluePaintMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, BluePaintMaterial& output);

absl::Status RemoveBrushedMetalMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, BrushedMetalMaterial& output);

absl::Status RemoveClayMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, ClayMaterial& output);

absl::Status RemoveConductorMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, ConductorMaterial& output);

absl::Status RemoveDielectricMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, DielectricMaterial& output);

absl::Status RemoveDiffuseMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, DiffuseMaterial& output);

absl::Status RemoveDisneyMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, DisneyMaterial& output);

absl::Status RemoveFeltMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, FeltMaterial& output);

absl::Status RemoveHairMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, HairMaterial& output);

absl::Status RemoveKdSubsurfaceMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, KdSubsurfaceMaterial& output);

absl::Status RemoveMeasuredFourierMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, MeasuredFourierMaterial& output);

absl::Status RemoveMeasuredMerlMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, MeasuredMerlMaterial& output);

absl::Status RemoveMirrorMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, MirrorMaterial& output);

absl::Status RemoveMixMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, MixMaterial& output);

absl::Status RemovePlasticMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, PlasticMaterial& output);

absl::Status RemovePrimerMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, PrimerMaterial& output);

absl::Status RemoveShinyMetalMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, ShinyMetalMaterial& output);

absl::Status RemoveSkinMaterial(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, SkinMaterial& output);

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
