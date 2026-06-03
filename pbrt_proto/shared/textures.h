#ifndef _PBRT_PROTO_SHARED_TEXTURES_
#define _PBRT_PROTO_SHARED_TEXTURES_

#include "absl/container/flat_hash_map.h"
#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {

void RemoveBilerpFloatTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    BilerpFloatTexture& output);

void RemoveBilerpSpectrumTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    BilerpSpectrumTexture& output);

void RemoveCheckerboard2DFloatTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    Checkerboard2DFloatTexture& output);

void RemoveCheckerboard2DFloatTextureV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    Checkerboard2DFloatTexture& output);

void RemoveCheckerboard2DFloatTextureV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    Checkerboard2DFloatTexture& output);

void RemoveCheckerboard2DSpectrumTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    Checkerboard2DSpectrumTexture& output);

void RemoveCheckerboard2DSpectrumTextureV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    Checkerboard2DSpectrumTexture& output);

void RemoveCheckerboard2DSpectrumTextureV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    Checkerboard2DSpectrumTexture& output);

void RemoveCheckerboard3DFloatTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    Checkerboard3DFloatTexture& output);

void RemoveCheckerboard3DSpectrumTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    Checkerboard3DSpectrumTexture& output);

void RemoveConstantFloatTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ConstantFloatTexture& output);

absl::Status RemoveConstantSpectrumTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ConstantSpectrumTexture& output);

void RemoveDirectionMixFloatTextureV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DirectionMixFloatTexture& output);

void RemoveDirectionMixSpectrumTextureV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DirectionMixSpectrumTexture& output);

void RemoveDotsFloatTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DotsFloatTexture& output);

void RemoveDotsSpectrumTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DotsSpectrumTexture& output);

void RemoveFBmFloatTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    FBmFloatTexture& output);

void RemoveFBmSpectrumTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    FBmSpectrumTexture& output);

void RemoveImageMapFloatTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ImageMapFloatTexture& output);

void RemoveImageMapFloatTextureV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ImageMapFloatTexture& output);

void RemoveImageMapFloatTextureV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ImageMapFloatTexture& output);

absl::Status RemoveImageMapFloatTextureV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ImageMapFloatTexture& output);

void RemoveImageMapSpectrumTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ImageMapSpectrumTexture& output);

void RemoveImageMapSpectrumTextureV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ImageMapSpectrumTexture& output);

void RemoveImageMapSpectrumTextureV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ImageMapSpectrumTexture& output);

absl::Status RemoveImageMapSpectrumTextureV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ImageMapSpectrumTexture& output);

void RemoveMarbleSpectrumTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    MarbleSpectrumTexture& output);

void RemoveMixFloatTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    MixFloatTexture& output);

void RemoveMixSpectrumTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    MixSpectrumTexture& output);

void RemovePtexFloatTextureV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PtexFloatTexture& output);

absl::Status RemovePtexFloatTextureV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PtexFloatTexture& output);

void RemovePtexSpectrumTextureV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PtexSpectrumTexture& output);

absl::Status RemovePtexSpectrumTextureV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PtexSpectrumTexture& output);

void RemoveScaleFloatTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ScaleFloatTexture& output);

void RemoveScaleSpectrumTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ScaleSpectrumTexture& output);

void RemoveWrinkledFloatTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    WrinkledFloatTexture& output);

void RemoveWrinkledSpectrumTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    WrinkledSpectrumTexture& output);

void RemoveUvSpectrumTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    UvSpectrumTexture& output);

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_TEXTURES_
