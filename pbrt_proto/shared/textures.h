#ifndef _PBRT_PROTO_SHARED_TEXTURES_
#define _PBRT_PROTO_SHARED_TEXTURES_

#include "absl/container/flat_hash_map.h"
#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {

absl::Status RemoveBilerpFloatTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, BilerpFloatTexture& output);

absl::Status RemoveBilerpSpectrumTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, BilerpSpectrumTexture& output);

absl::Status RemoveCheckerboard2DFloatTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, Checkerboard2DFloatTexture& output);

absl::Status RemoveCheckerboard2DSpectrumTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, Checkerboard2DSpectrumTexture& output);

absl::Status RemoveCheckerboard3DFloatTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, Checkerboard3DFloatTexture& output);

absl::Status RemoveCheckerboard3DSpectrumTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, Checkerboard3DSpectrumTexture& output);

absl::Status RemoveConstantFloatTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, ConstantFloatTexture& output);

absl::Status RemoveConstantSpectrumTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, ConstantSpectrumTexture& output);

absl::Status RemoveDirectionMixFloatTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, DirectionMixFloatTexture& output);

absl::Status RemoveDirectionMixSpectrumTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, DirectionMixSpectrumTexture& output);

absl::Status RemoveDotsFloatTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, DotsFloatTexture& output);

absl::Status RemoveDotsSpectrumTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, DotsSpectrumTexture& output);

absl::Status RemoveFBmFloatTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, FBmFloatTexture& output);

absl::Status RemoveFBmSpectrumTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, FBmSpectrumTexture& output);

absl::Status RemoveImageMapFloatTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, ImageMapFloatTexture& output);

absl::Status RemoveImageMapSpectrumTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, ImageMapSpectrumTexture& output);

absl::Status RemoveMarbleSpectrumTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, MarbleSpectrumTexture& output);

absl::Status RemoveMixFloatTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, MixFloatTexture& output);

absl::Status RemoveMixSpectrumTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, MixSpectrumTexture& output);

absl::Status RemovePtexFloatTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, PtexFloatTexture& output);

absl::Status RemovePtexSpectrumTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, PtexSpectrumTexture& output);

absl::Status RemoveScaleFloatTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, ScaleFloatTexture& output);

absl::Status RemoveScaleSpectrumTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, ScaleSpectrumTexture& output);

absl::Status RemoveWrinkledFloatTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, WrinkledFloatTexture& output);

absl::Status RemoveWrinkledSpectrumTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, WrinkledSpectrumTexture& output);

absl::Status RemoveUvSpectrumTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, UvSpectrumTexture& output);

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_TEXTURES_
