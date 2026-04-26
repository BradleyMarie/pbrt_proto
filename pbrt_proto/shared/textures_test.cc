#include "pbrt_proto/shared/textures.h"

#include <cstdint>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "absl/strings/string_view.h"
#include "googlemock/include/gmock/gmock.h"
#include "googletest/include/gtest/gtest.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"
#include "pbrt_proto/testing/proto_matchers.h"

namespace pbrt_proto {
namespace {

using ::google::protobuf::EqualsProto;

TEST(RemoveBilerpFloatTextureV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  BilerpFloatTexture actual;
  RemoveBilerpFloatTextureV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveBilerpSpectrumTextureV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  BilerpSpectrumTexture actual;
  RemoveBilerpSpectrumTextureV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveCheckerboard2DFloatTextureV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  Checkerboard2DFloatTexture actual;
  RemoveCheckerboard2DFloatTextureV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveCheckerboard2DFloatTextureV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  Checkerboard2DFloatTexture actual;
  RemoveCheckerboard2DFloatTextureV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveCheckerboard2DFloatTextureV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  Checkerboard2DFloatTexture actual;
  RemoveCheckerboard2DFloatTextureV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveCheckerboard2DSpectrumTextureV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  Checkerboard2DSpectrumTexture actual;
  RemoveCheckerboard2DSpectrumTextureV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveCheckerboard2DSpectrumTextureV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  Checkerboard2DSpectrumTexture actual;
  RemoveCheckerboard2DSpectrumTextureV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveCheckerboard2DSpectrumTextureV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  Checkerboard2DSpectrumTexture actual;
  RemoveCheckerboard2DSpectrumTextureV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveCheckerboard3DFloatTextureV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  Checkerboard3DFloatTexture actual;
  RemoveCheckerboard3DFloatTextureV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveCheckerboard3DSpectrumTextureV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  Checkerboard3DSpectrumTexture actual;
  RemoveCheckerboard3DSpectrumTextureV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveConstantFloatTextureV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  ConstantFloatTexture actual;
  RemoveConstantFloatTextureV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveConstantSpectrumTextureV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  ConstantSpectrumTexture actual;
  RemoveConstantSpectrumTextureV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveDirectionMixFloatTextureV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  DirectionMixFloatTexture actual;
  RemoveDirectionMixFloatTextureV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveDirectionMixSpectrumTextureV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  DirectionMixSpectrumTexture actual;
  RemoveDirectionMixSpectrumTextureV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveDotsFloatTextureV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  DotsFloatTexture actual;
  RemoveDotsFloatTextureV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveDotsSpectrumTextureV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  DotsSpectrumTexture actual;
  RemoveDotsSpectrumTextureV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveFBmFloatTextureV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  FBmFloatTexture actual;
  RemoveFBmFloatTextureV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveFBmSpectrumTextureV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  FBmSpectrumTexture actual;
  RemoveFBmSpectrumTextureV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveImageMapFloatTextureV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  ImageMapFloatTexture actual;
  RemoveImageMapFloatTextureV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveImageMapFloatTextureV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  ImageMapFloatTexture actual;
  RemoveImageMapFloatTextureV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveImageMapFloatTextureV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  ImageMapFloatTexture actual;
  RemoveImageMapFloatTextureV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveImageMapFloatTextureV4, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  ImageMapFloatTexture actual;
  EXPECT_TRUE(RemoveImageMapFloatTextureV4(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveImageMapSpectrumTextureV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  ImageMapSpectrumTexture actual;
  RemoveImageMapSpectrumTextureV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveImageMapSpectrumTextureV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  ImageMapSpectrumTexture actual;
  RemoveImageMapSpectrumTextureV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveImageMapSpectrumTextureV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  ImageMapSpectrumTexture actual;
  RemoveImageMapSpectrumTextureV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveImageMapSpectrumTextureV4, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  ImageMapSpectrumTexture actual;
  EXPECT_TRUE(RemoveImageMapSpectrumTextureV4(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveMarbleSpectrumTextureV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  MarbleSpectrumTexture actual;
  RemoveMarbleSpectrumTextureV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveMixFloatTextureV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  MixFloatTexture actual;
  RemoveMixFloatTextureV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveMixSpectrumTextureV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  MixSpectrumTexture actual;
  RemoveMixSpectrumTextureV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemovePtexFloatTextureV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  PtexFloatTexture actual;
  RemovePtexFloatTextureV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemovePtexFloatTextureV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  PtexFloatTexture actual;
  EXPECT_TRUE(RemovePtexFloatTextureV2(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemovePtexSpectrumTextureV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  PtexSpectrumTexture actual;
  RemovePtexSpectrumTextureV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemovePtexSpectrumTextureV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  PtexSpectrumTexture actual;
  EXPECT_TRUE(RemovePtexSpectrumTextureV2(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveScaleFloatTextureV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  ScaleFloatTexture actual;
  RemoveScaleFloatTextureV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveScaleSpectrumTextureV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  ScaleSpectrumTexture actual;
  RemoveScaleSpectrumTextureV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveWrinkledFloatTextureV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  WrinkledFloatTexture actual;
  RemoveWrinkledFloatTextureV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveWrinkledSpectrumTextureV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  WrinkledSpectrumTexture actual;
  RemoveWrinkledSpectrumTextureV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveUvSpectrumTextureV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  UvSpectrumTexture actual;
  RemoveUvSpectrumTextureV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

}  // namespace
}  // namespace pbrt_proto
