#include "pbrt_proto/shared/textures.h"

#include <array>
#include <cstdint>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "absl/status/status_matchers.h"
#include "absl/strings/string_view.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"
#include "pbrt_proto/testing/proto_matchers.h"

namespace pbrt_proto {
namespace {

using ::absl_testing::StatusIs;
using ::google::protobuf::EqualsProto;

TEST(AaMode, UnknownValue) {
  std::vector<absl::string_view> values = {"unknown"};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::STRING,
                      /*type_name=*/"",
                      /*values=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"aamode", parameter}};

  Checkerboard2DFloatTexture actual;
  EXPECT_TRUE(
      RemoveCheckerboard2DFloatTexture(parameters, /*pbrt_version=*/1, actual)
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                aamode: CLOSEDFORM
              )pb"));
}

TEST(AaMode, ClosedForm) {
  std::vector<absl::string_view> values = {"closedform"};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::STRING,
                      /*type_name=*/"",
                      /*values=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"aamode", parameter}};

  Checkerboard2DFloatTexture actual;
  EXPECT_TRUE(
      RemoveCheckerboard2DFloatTexture(parameters, /*pbrt_version=*/1, actual)
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                aamode: CLOSEDFORM
              )pb"));
}

TEST(AaMode, None) {
  std::vector<absl::string_view> values = {"none"};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::STRING,
                      /*type_name=*/"",
                      /*values=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"aamode", parameter}};

  Checkerboard2DFloatTexture actual;
  EXPECT_TRUE(
      RemoveCheckerboard2DFloatTexture(parameters, /*pbrt_version=*/1, actual)
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                aamode: DISABLED
              )pb"));
}

TEST(AaMode, Supersample) {
  std::vector<absl::string_view> values = {"supersample"};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::STRING,
                      /*type_name=*/"",
                      /*values=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"aamode", parameter}};

  Checkerboard2DFloatTexture actual;
  EXPECT_TRUE(
      RemoveCheckerboard2DFloatTexture(parameters, /*pbrt_version=*/1, actual)
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                aamode: SUPERSAMPLE
              )pb"));
}

TEST(AaMode, SupersampleV2) {
  std::vector<absl::string_view> values = {"supersample"};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::STRING,
                      /*type_name=*/"",
                      /*values=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"aamode", parameter}};

  Checkerboard2DFloatTexture actual;
  EXPECT_TRUE(
      RemoveCheckerboard2DFloatTexture(parameters, /*pbrt_version=*/2, actual)
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                aamode: CLOSEDFORM
              )pb"));
}

TEST(Encoding, UnknownValue) {
  std::vector<absl::string_view> values = {"unknown"};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::STRING,
                      /*type_name=*/"",
                      /*values=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"encoding", parameter}};

  ImageMapFloatTexture actual;
  EXPECT_THAT(
      RemoveImageMapFloatTexture(parameters, /*pbrt_version=*/4, actual),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Invalid imagemap Texture parameter : 'encoding'"));
}

TEST(Encoding, SRGB) {
  std::vector<absl::string_view> values = {"sRGB"};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::STRING,
                      /*type_name=*/"",
                      /*values=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"encoding", parameter}};

  ImageMapFloatTexture actual;
  EXPECT_TRUE(
      RemoveImageMapFloatTexture(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                encoding: SRGB
              )pb"));
}

TEST(Encoding, Linear) {
  std::vector<absl::string_view> values = {"linear"};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::STRING,
                      /*type_name=*/"",
                      /*values=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"encoding", parameter}};

  ImageMapFloatTexture actual;
  EXPECT_TRUE(
      RemoveImageMapFloatTexture(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                encoding: LINEAR
              )pb"));
}

TEST(Encoding, Gamma) {
  std::vector<absl::string_view> values = {"gamma 2.2"};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::STRING,
                      /*type_name=*/"",
                      /*values=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"encoding", parameter}};

  ImageMapFloatTexture actual;
  EXPECT_TRUE(
      RemoveImageMapFloatTexture(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                encoding: GAMMA gamma: 2.2
              )pb"));
}

TEST(Filter, UnknownValue) {
  std::vector<absl::string_view> values = {"unknown"};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::STRING,
                      /*type_name=*/"",
                      /*values=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"filter", parameter}};

  ImageMapFloatTexture actual;
  EXPECT_TRUE(
      RemoveImageMapFloatTexture(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(Filter, Bilinear) {
  std::vector<absl::string_view> values = {"bilinear"};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::STRING,
                      /*type_name=*/"",
                      /*values=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"filter", parameter}};

  ImageMapFloatTexture actual;
  EXPECT_TRUE(
      RemoveImageMapFloatTexture(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                filter: BILINEAR
              )pb"));
}

TEST(Filter, Trilinear) {
  std::vector<absl::string_view> values = {"trilinear"};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::STRING,
                      /*type_name=*/"",
                      /*values=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"filter", parameter}};

  ImageMapFloatTexture actual;
  EXPECT_TRUE(
      RemoveImageMapFloatTexture(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                filter: TRILINEAR
              )pb"));
}

TEST(Filter, EWA) {
  std::vector<absl::string_view> values = {"ewa"};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::STRING,
                      /*type_name=*/"",
                      /*values=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"filter", parameter}};

  ImageMapFloatTexture actual;
  EXPECT_TRUE(
      RemoveImageMapFloatTexture(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                filter: EWA
              )pb"));
}

TEST(Filter, Point) {
  std::vector<absl::string_view> values = {"point"};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::STRING,
                      /*type_name=*/"",
                      /*values=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"filter", parameter}};

  ImageMapFloatTexture actual;
  EXPECT_TRUE(
      RemoveImageMapFloatTexture(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                filter: POINT
              )pb"));
}

TEST(Mapping, UnknownValue) {
  std::vector<absl::string_view> values = {"unknown"};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::STRING,
                      /*type_name=*/"",
                      /*values=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"mapping", parameter}};

  BilerpFloatTexture actual;
  EXPECT_TRUE(
      RemoveBilerpFloatTexture(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                mapping: UV
              )pb"));
}

TEST(Mapping, UV) {
  std::vector<absl::string_view> values = {"uv"};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::STRING,
                      /*type_name=*/"",
                      /*values=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"mapping", parameter}};

  BilerpFloatTexture actual;
  EXPECT_TRUE(
      RemoveBilerpFloatTexture(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                mapping: UV
              )pb"));
}

TEST(Mapping, Spherical) {
  std::vector<absl::string_view> values = {"spherical"};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::STRING,
                      /*type_name=*/"",
                      /*values=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"mapping", parameter}};

  BilerpFloatTexture actual;
  EXPECT_TRUE(
      RemoveBilerpFloatTexture(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                mapping: SPHERICAL
              )pb"));
}

TEST(Mapping, Planar) {
  std::vector<absl::string_view> values = {"planar"};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::STRING,
                      /*type_name=*/"",
                      /*values=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"mapping", parameter}};

  BilerpFloatTexture actual;
  EXPECT_TRUE(
      RemoveBilerpFloatTexture(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                mapping: PLANAR
              )pb"));
}

TEST(Mapping, Cylindrical) {
  std::vector<absl::string_view> values = {"cylindrical"};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::STRING,
                      /*type_name=*/"",
                      /*values=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"mapping", parameter}};

  BilerpFloatTexture actual;
  EXPECT_TRUE(
      RemoveBilerpFloatTexture(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                mapping: CYLINDRICAL
              )pb"));
}

TEST(Wrap, UnknownValue) {
  std::vector<absl::string_view> values = {"unknown"};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::STRING,
                      /*type_name=*/"",
                      /*values=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"wrap", parameter}};

  ImageMapFloatTexture actual;
  EXPECT_TRUE(
      RemoveImageMapFloatTexture(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                wrap: REPEAT
              )pb"));
}

TEST(Wrap, Black) {
  std::vector<absl::string_view> values = {"black"};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::STRING,
                      /*type_name=*/"",
                      /*values=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"wrap", parameter}};

  ImageMapFloatTexture actual;
  EXPECT_TRUE(
      RemoveImageMapFloatTexture(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                wrap: BLACK
              )pb"));
}

TEST(Wrap, Clamp) {
  std::vector<absl::string_view> values = {"clamp"};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::STRING,
                      /*type_name=*/"",
                      /*values=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"wrap", parameter}};

  ImageMapFloatTexture actual;
  EXPECT_TRUE(
      RemoveImageMapFloatTexture(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                wrap: CLAMP
              )pb"));
}

TEST(Wrap, Repeat) {
  std::vector<absl::string_view> values = {"repeat"};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::STRING,
                      /*type_name=*/"",
                      /*values=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"wrap", parameter}};

  ImageMapFloatTexture actual;
  EXPECT_TRUE(
      RemoveImageMapFloatTexture(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                wrap: REPEAT
              )pb"));
}

TEST(Mapping, UvParameters) {
  std::vector<absl::string_view> mapping = {"uv"};
  Parameter mapping_parameter{/*directive=*/"",
                              /*type=*/ParameterType::STRING,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(mapping)};

  std::vector<double> uscale = {1.0};
  Parameter uscale_parameter{/*directive=*/"",
                             /*type=*/ParameterType::FLOAT,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(uscale)};

  std::vector<double> vscale = {2.0};
  Parameter vscale_parameter{/*directive=*/"",
                             /*type=*/ParameterType::FLOAT,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(vscale)};

  std::vector<double> udelta = {3.0};
  Parameter udelta_parameter{/*directive=*/"",
                             /*type=*/ParameterType::FLOAT,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(udelta)};

  std::vector<double> vdelta = {4.0};
  Parameter vdelta_parameter{/*directive=*/"",
                             /*type=*/ParameterType::FLOAT,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(vdelta)};

  std::vector<std::array<double, 3>> v1 = {{5.0, 6.0, 7.0}};
  Parameter v1_parameter{/*directive=*/"",
                         /*type=*/ParameterType::VECTOR3,
                         /*type_name=*/"",
                         /*values=*/absl::MakeSpan(v1)};

  std::vector<std::array<double, 3>> v2 = {{8.0, 9.0, 10.0}};
  Parameter v2_parameter{/*directive=*/"",
                         /*type=*/ParameterType::VECTOR3,
                         /*type_name=*/"",
                         /*values=*/absl::MakeSpan(v2)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"mapping", mapping_parameter},
      {"uscale", uscale_parameter},
      {"vscale", vscale_parameter},
      {"udelta", udelta_parameter},
      {"vdelta", vdelta_parameter},
      {"v1", v1_parameter},
      {"v2", v2_parameter}};

  BilerpFloatTexture actual;
  EXPECT_TRUE(
      RemoveBilerpFloatTexture(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                mapping: UV
                uscale: 1.0
                vscale: 2.0
                udelta: 3.0
                vdelta: 4.0
                v1 { x: 5.0 y: 6.0 z: 7.0 }
                v2 { x: 8.0 y: 9.0 z: 10.0 }
              )pb"));
}

TEST(RemoveBilerpFloatTexture, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  BilerpFloatTexture actual;
  EXPECT_TRUE(
      RemoveBilerpFloatTexture(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveBilerpFloatTexture, WithData) {
  std::vector<absl::string_view> v00 = {"v00"};
  Parameter v00_parameter{/*directive=*/"",
                          /*type=*/ParameterType::TEXTURE,
                          /*type_name=*/"",
                          /*values=*/absl::MakeSpan(v00)};

  std::vector<absl::string_view> v01 = {"v01"};
  Parameter v01_parameter{/*directive=*/"",
                          /*type=*/ParameterType::TEXTURE,
                          /*type_name=*/"",
                          /*values=*/absl::MakeSpan(v01)};

  std::vector<absl::string_view> v10 = {"v10"};
  Parameter v10_parameter{/*directive=*/"",
                          /*type=*/ParameterType::TEXTURE,
                          /*type_name=*/"",
                          /*values=*/absl::MakeSpan(v10)};

  std::vector<absl::string_view> v11 = {"v11"};
  Parameter v11_parameter{/*directive=*/"",
                          /*type=*/ParameterType::TEXTURE,
                          /*type_name=*/"",
                          /*values=*/absl::MakeSpan(v11)};

  std::vector<absl::string_view> mapping = {"uv"};
  Parameter mapping_parameter{/*directive=*/"",
                              /*type=*/ParameterType::STRING,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(mapping)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"v00", v00_parameter},
      {"v01", v01_parameter},
      {"v10", v10_parameter},
      {"v11", v11_parameter},
      {"mapping", mapping_parameter}};

  BilerpFloatTexture actual;
  EXPECT_TRUE(
      RemoveBilerpFloatTexture(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                v00 { float_texture_name: "v00" }
                v01 { float_texture_name: "v01" }
                v10 { float_texture_name: "v10" }
                v11 { float_texture_name: "v11" }
                mapping: UV
              )pb"));
}

TEST(RemoveBilerpSpectrumTexture, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  BilerpSpectrumTexture actual;
  EXPECT_TRUE(
      RemoveBilerpSpectrumTexture(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveBilerpSpectrumTexture, WithData) {
  std::vector<absl::string_view> v00 = {"v00"};
  Parameter v00_parameter{/*directive=*/"",
                          /*type=*/ParameterType::TEXTURE,
                          /*type_name=*/"",
                          /*values=*/absl::MakeSpan(v00)};

  std::vector<absl::string_view> v01 = {"v01"};
  Parameter v01_parameter{/*directive=*/"",
                          /*type=*/ParameterType::TEXTURE,
                          /*type_name=*/"",
                          /*values=*/absl::MakeSpan(v01)};

  std::vector<absl::string_view> v10 = {"v10"};
  Parameter v10_parameter{/*directive=*/"",
                          /*type=*/ParameterType::TEXTURE,
                          /*type_name=*/"",
                          /*values=*/absl::MakeSpan(v10)};

  std::vector<absl::string_view> v11 = {"v11"};
  Parameter v11_parameter{/*directive=*/"",
                          /*type=*/ParameterType::TEXTURE,
                          /*type_name=*/"",
                          /*values=*/absl::MakeSpan(v11)};

  std::vector<absl::string_view> mapping = {"uv"};
  Parameter mapping_parameter{/*directive=*/"",
                              /*type=*/ParameterType::STRING,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(mapping)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"v00", v00_parameter},
      {"v01", v01_parameter},
      {"v10", v10_parameter},
      {"v11", v11_parameter},
      {"mapping", mapping_parameter}};

  BilerpSpectrumTexture actual;
  EXPECT_TRUE(
      RemoveBilerpSpectrumTexture(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                v00 { spectrum_texture_name: "v00" }
                v01 { spectrum_texture_name: "v01" }
                v10 { spectrum_texture_name: "v10" }
                v11 { spectrum_texture_name: "v11" }
                mapping: UV
              )pb"));
}

TEST(RemoveCheckerboard2DFloatTextureV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  Checkerboard2DFloatTexture actual;
  EXPECT_TRUE(
      RemoveCheckerboard2DFloatTexture(parameters, /*pbrt_version=*/1, actual)
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveCheckerboard2DFloatTextureV1, WithData) {
  std::vector<absl::string_view> tex1 = {"tex1"};
  Parameter tex1_parameter{/*directive=*/"",
                           /*type=*/ParameterType::TEXTURE,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(tex1)};

  std::vector<absl::string_view> tex2 = {"tex2"};
  Parameter tex2_parameter{/*directive=*/"",
                           /*type=*/ParameterType::TEXTURE,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(tex2)};

  std::vector<absl::string_view> aamode = {"supersample"};
  Parameter aamode_parameter{/*directive=*/"",
                             /*type=*/ParameterType::STRING,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(aamode)};

  std::vector<absl::string_view> mapping = {"uv"};
  Parameter mapping_parameter{/*directive=*/"",
                              /*type=*/ParameterType::STRING,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(mapping)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"tex1", tex1_parameter},
      {"tex2", tex2_parameter},
      {"aamode", aamode_parameter},
      {"mapping", mapping_parameter}};

  Checkerboard2DFloatTexture actual;
  EXPECT_TRUE(
      RemoveCheckerboard2DFloatTexture(parameters, /*pbrt_version=*/1, actual)
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                tex1 { float_texture_name: "tex1" }
                tex2 { float_texture_name: "tex2" }
                aamode: SUPERSAMPLE
                mapping: UV
              )pb"));
}

TEST(RemoveCheckerboard2DFloatTextureV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  Checkerboard2DFloatTexture actual;
  EXPECT_TRUE(
      RemoveCheckerboard2DFloatTexture(parameters, /*pbrt_version=*/2, actual)
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveCheckerboard2DFloatTextureV2, WithData) {
  std::vector<absl::string_view> tex1 = {"tex1"};
  Parameter tex1_parameter{/*directive=*/"",
                           /*type=*/ParameterType::TEXTURE,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(tex1)};

  std::vector<absl::string_view> tex2 = {"tex2"};
  Parameter tex2_parameter{/*directive=*/"",
                           /*type=*/ParameterType::TEXTURE,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(tex2)};

  std::vector<absl::string_view> aamode = {"closedform"};
  Parameter aamode_parameter{/*directive=*/"",
                             /*type=*/ParameterType::STRING,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(aamode)};

  std::vector<absl::string_view> mapping = {"uv"};
  Parameter mapping_parameter{/*directive=*/"",
                              /*type=*/ParameterType::STRING,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(mapping)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"tex1", tex1_parameter},
      {"tex2", tex2_parameter},
      {"aamode", aamode_parameter},
      {"mapping", mapping_parameter}};

  Checkerboard2DFloatTexture actual;
  EXPECT_TRUE(
      RemoveCheckerboard2DFloatTexture(parameters, /*pbrt_version=*/2, actual)
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                tex1 { float_texture_name: "tex1" }
                tex2 { float_texture_name: "tex2" }
                aamode: CLOSEDFORM
                mapping: UV
              )pb"));
}

TEST(RemoveCheckerboard2DFloatTextureV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  Checkerboard2DFloatTexture actual;
  EXPECT_TRUE(
      RemoveCheckerboard2DFloatTexture(parameters, /*pbrt_version=*/3, actual)
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveCheckerboard2DFloatTextureV3, WithData) {
  std::vector<absl::string_view> tex1 = {"tex1"};
  Parameter tex1_parameter{/*directive=*/"",
                           /*type=*/ParameterType::TEXTURE,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(tex1)};

  std::vector<absl::string_view> tex2 = {"tex2"};
  Parameter tex2_parameter{/*directive=*/"",
                           /*type=*/ParameterType::TEXTURE,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(tex2)};

  std::vector<absl::string_view> aamode = {"closedform"};
  Parameter aamode_parameter{/*directive=*/"",
                             /*type=*/ParameterType::STRING,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(aamode)};

  std::vector<absl::string_view> mapping = {"uv"};
  Parameter mapping_parameter{/*directive=*/"",
                              /*type=*/ParameterType::STRING,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(mapping)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"tex1", tex1_parameter},
      {"tex2", tex2_parameter},
      {"aamode", aamode_parameter},
      {"mapping", mapping_parameter}};

  Checkerboard2DFloatTexture actual;
  EXPECT_TRUE(
      RemoveCheckerboard2DFloatTexture(parameters, /*pbrt_version=*/3, actual)
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                tex1 { float_texture_name: "tex1" }
                tex2 { float_texture_name: "tex2" }
                aamode: CLOSEDFORM
                mapping: UV
              )pb"));
}

TEST(RemoveCheckerboard2DFloatTextureV4, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  Checkerboard2DFloatTexture actual;
  EXPECT_TRUE(
      RemoveCheckerboard2DFloatTexture(parameters, /*pbrt_version=*/4, actual)
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveCheckerboard2DFloatTextureV4, WithData) {
  std::vector<absl::string_view> tex1 = {"tex1"};
  Parameter tex1_parameter{/*directive=*/"",
                           /*type=*/ParameterType::TEXTURE,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(tex1)};

  std::vector<absl::string_view> tex2 = {"tex2"};
  Parameter tex2_parameter{/*directive=*/"",
                           /*type=*/ParameterType::TEXTURE,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(tex2)};

  std::vector<absl::string_view> mapping = {"uv"};
  Parameter mapping_parameter{/*directive=*/"",
                              /*type=*/ParameterType::STRING,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(mapping)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"tex1", tex1_parameter},
      {"tex2", tex2_parameter},
      {"mapping", mapping_parameter}};

  Checkerboard2DFloatTexture actual;
  EXPECT_TRUE(
      RemoveCheckerboard2DFloatTexture(parameters, /*pbrt_version=*/4, actual)
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                tex1 { float_texture_name: "tex1" }
                tex2 { float_texture_name: "tex2" }
                mapping: UV
              )pb"));
}

TEST(RemoveCheckerboard2DSpectrumTextureV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  Checkerboard2DSpectrumTexture actual;
  EXPECT_TRUE(RemoveCheckerboard2DSpectrumTexture(parameters,
                                                  /*pbrt_version=*/1, actual)
                  .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveCheckerboard2DSpectrumTextureV1, WithData) {
  std::vector<absl::string_view> tex1 = {"tex1"};
  Parameter tex1_parameter{/*directive=*/"",
                           /*type=*/ParameterType::TEXTURE,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(tex1)};

  std::vector<absl::string_view> tex2 = {"tex2"};
  Parameter tex2_parameter{/*directive=*/"",
                           /*type=*/ParameterType::TEXTURE,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(tex2)};

  std::vector<absl::string_view> aamode = {"supersample"};
  Parameter aamode_parameter{/*directive=*/"",
                             /*type=*/ParameterType::STRING,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(aamode)};

  std::vector<absl::string_view> mapping = {"uv"};
  Parameter mapping_parameter{/*directive=*/"",
                              /*type=*/ParameterType::STRING,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(mapping)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"tex1", tex1_parameter},
      {"tex2", tex2_parameter},
      {"aamode", aamode_parameter},
      {"mapping", mapping_parameter}};

  Checkerboard2DSpectrumTexture actual;
  EXPECT_TRUE(RemoveCheckerboard2DSpectrumTexture(parameters,
                                                  /*pbrt_version=*/1, actual)
                  .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                tex1 { spectrum_texture_name: "tex1" }
                tex2 { spectrum_texture_name: "tex2" }
                aamode: SUPERSAMPLE
                mapping: UV
              )pb"));
}

TEST(RemoveCheckerboard2DSpectrumTextureV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  Checkerboard2DSpectrumTexture actual;
  EXPECT_TRUE(RemoveCheckerboard2DSpectrumTexture(parameters,
                                                  /*pbrt_version=*/2, actual)
                  .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveCheckerboard2DSpectrumTextureV2, WithData) {
  std::vector<absl::string_view> tex1 = {"tex1"};
  Parameter tex1_parameter{/*directive=*/"",
                           /*type=*/ParameterType::TEXTURE,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(tex1)};

  std::vector<absl::string_view> tex2 = {"tex2"};
  Parameter tex2_parameter{/*directive=*/"",
                           /*type=*/ParameterType::TEXTURE,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(tex2)};

  std::vector<absl::string_view> aamode = {"closedform"};
  Parameter aamode_parameter{/*directive=*/"",
                             /*type=*/ParameterType::STRING,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(aamode)};

  std::vector<absl::string_view> mapping = {"uv"};
  Parameter mapping_parameter{/*directive=*/"",
                              /*type=*/ParameterType::STRING,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(mapping)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"tex1", tex1_parameter},
      {"tex2", tex2_parameter},
      {"aamode", aamode_parameter},
      {"mapping", mapping_parameter}};

  Checkerboard2DSpectrumTexture actual;
  EXPECT_TRUE(RemoveCheckerboard2DSpectrumTexture(parameters,
                                                  /*pbrt_version=*/2, actual)
                  .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                tex1 { spectrum_texture_name: "tex1" }
                tex2 { spectrum_texture_name: "tex2" }
                aamode: CLOSEDFORM
                mapping: UV
              )pb"));
}

TEST(RemoveCheckerboard2DSpectrumTextureV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  Checkerboard2DSpectrumTexture actual;
  EXPECT_TRUE(RemoveCheckerboard2DSpectrumTexture(parameters,
                                                  /*pbrt_version=*/3, actual)
                  .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveCheckerboard2DSpectrumTextureV3, WithData) {
  std::vector<absl::string_view> tex1 = {"tex1"};
  Parameter tex1_parameter{/*directive=*/"",
                           /*type=*/ParameterType::TEXTURE,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(tex1)};

  std::vector<absl::string_view> tex2 = {"tex2"};
  Parameter tex2_parameter{/*directive=*/"",
                           /*type=*/ParameterType::TEXTURE,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(tex2)};

  std::vector<absl::string_view> aamode = {"closedform"};
  Parameter aamode_parameter{/*directive=*/"",
                             /*type=*/ParameterType::STRING,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(aamode)};

  std::vector<absl::string_view> mapping = {"uv"};
  Parameter mapping_parameter{/*directive=*/"",
                              /*type=*/ParameterType::STRING,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(mapping)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"tex1", tex1_parameter},
      {"tex2", tex2_parameter},
      {"aamode", aamode_parameter},
      {"mapping", mapping_parameter}};

  Checkerboard2DSpectrumTexture actual;
  EXPECT_TRUE(RemoveCheckerboard2DSpectrumTexture(parameters,
                                                  /*pbrt_version=*/3, actual)
                  .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                tex1 { spectrum_texture_name: "tex1" }
                tex2 { spectrum_texture_name: "tex2" }
                aamode: CLOSEDFORM
                mapping: UV
              )pb"));
}

TEST(RemoveCheckerboard2DSpectrumTextureV4, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  Checkerboard2DSpectrumTexture actual;
  EXPECT_TRUE(RemoveCheckerboard2DSpectrumTexture(parameters,
                                                  /*pbrt_version=*/4, actual)
                  .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveCheckerboard2DSpectrumTextureV4, WithData) {
  std::vector<absl::string_view> tex1 = {"tex1"};
  Parameter tex1_parameter{/*directive=*/"",
                           /*type=*/ParameterType::TEXTURE,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(tex1)};

  std::vector<absl::string_view> tex2 = {"tex2"};
  Parameter tex2_parameter{/*directive=*/"",
                           /*type=*/ParameterType::TEXTURE,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(tex2)};

  std::vector<absl::string_view> mapping = {"uv"};
  Parameter mapping_parameter{/*directive=*/"",
                              /*type=*/ParameterType::STRING,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(mapping)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"tex1", tex1_parameter},
      {"tex2", tex2_parameter},
      {"mapping", mapping_parameter}};

  Checkerboard2DSpectrumTexture actual;
  EXPECT_TRUE(RemoveCheckerboard2DSpectrumTexture(parameters,
                                                  /*pbrt_version=*/4, actual)
                  .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                tex1 { spectrum_texture_name: "tex1" }
                tex2 { spectrum_texture_name: "tex2" }
                mapping: UV
              )pb"));
}

TEST(RemoveCheckerboard3DFloatTextureV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  Checkerboard3DFloatTexture actual;
  EXPECT_TRUE(
      RemoveCheckerboard3DFloatTexture(parameters, /*pbrt_version=*/1, actual)
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveCheckerboard3DFloatTextureV1, WithData) {
  std::vector<absl::string_view> tex1 = {"tex1"};
  Parameter tex1_parameter{/*directive=*/"",
                           /*type=*/ParameterType::TEXTURE,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(tex1)};

  std::vector<absl::string_view> tex2 = {"tex2"};
  Parameter tex2_parameter{/*directive=*/"",
                           /*type=*/ParameterType::TEXTURE,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(tex2)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"tex1", tex1_parameter}, {"tex2", tex2_parameter}};

  Checkerboard3DFloatTexture actual;
  EXPECT_TRUE(
      RemoveCheckerboard3DFloatTexture(parameters, /*pbrt_version=*/1, actual)
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                tex1 { float_texture_name: "tex1" }
                tex2 { float_texture_name: "tex2" }
              )pb"));
}

TEST(RemoveCheckerboard3DSpectrumTexture, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  Checkerboard3DSpectrumTexture actual;
  EXPECT_TRUE(RemoveCheckerboard3DSpectrumTexture(parameters,
                                                  /*pbrt_version=*/1, actual)
                  .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveCheckerboard3DSpectrumTexture, WithData) {
  std::vector<absl::string_view> tex1 = {"tex1"};
  Parameter tex1_parameter{/*directive=*/"",
                           /*type=*/ParameterType::TEXTURE,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(tex1)};

  std::vector<absl::string_view> tex2 = {"tex2"};
  Parameter tex2_parameter{/*directive=*/"",
                           /*type=*/ParameterType::TEXTURE,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(tex2)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"tex1", tex1_parameter}, {"tex2", tex2_parameter}};

  Checkerboard3DSpectrumTexture actual;
  EXPECT_TRUE(RemoveCheckerboard3DSpectrumTexture(parameters,
                                                  /*pbrt_version=*/1, actual)
                  .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                tex1 { spectrum_texture_name: "tex1" }
                tex2 { spectrum_texture_name: "tex2" }
              )pb"));
}

TEST(RemoveConstantFloatTexture, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  ConstantFloatTexture actual;
  EXPECT_TRUE(
      RemoveConstantFloatTexture(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveConstantFloatTexture, WithData) {
  std::vector<double> value = {1.0};
  Parameter value_parameter{/*directive=*/"",
                            /*type=*/ParameterType::FLOAT,
                            /*type_name=*/"",
                            /*values=*/absl::MakeSpan(value)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"value", value_parameter}};

  ConstantFloatTexture actual;
  EXPECT_TRUE(
      RemoveConstantFloatTexture(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                value: 1.0
              )pb"));
}

TEST(RemoveConstantSpectrumTexture, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  ConstantSpectrumTexture actual;
  EXPECT_TRUE(
      RemoveConstantSpectrumTexture(parameters, /*pbrt_version=*/1, actual)
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveConstantSpectrumTexture, WithData) {
  std::vector<absl::string_view> value = {"value"};
  Parameter value_parameter{/*directive=*/"",
                            /*type=*/ParameterType::SPECTRUM,
                            /*type_name=*/"",
                            /*values=*/absl::MakeSpan(value)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"value", value_parameter}};

  ConstantSpectrumTexture actual;
  EXPECT_TRUE(
      RemoveConstantSpectrumTexture(parameters, /*pbrt_version=*/1, actual)
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                value { sampled_spectrum_filename: "value" }
              )pb"));
}

TEST(RemoveDirectionMixFloatTexture, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  DirectionMixFloatTexture actual;
  EXPECT_TRUE(
      RemoveDirectionMixFloatTexture(parameters, /*pbrt_version=*/4, actual)
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveDirectionMixFloatTexture, WithData) {
  std::vector<absl::string_view> tex1 = {"tex1"};
  Parameter tex1_parameter{/*directive=*/"",
                           /*type=*/ParameterType::TEXTURE,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(tex1)};

  std::vector<absl::string_view> tex2 = {"tex2"};
  Parameter tex2_parameter{/*directive=*/"",
                           /*type=*/ParameterType::TEXTURE,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(tex2)};

  std::vector<std::array<double, 3>> dir = {{1.0, 2.0, 3.0}};
  Parameter dir_parameter{/*directive=*/"",
                          /*type=*/ParameterType::VECTOR3,
                          /*type_name=*/"",
                          /*values=*/absl::MakeSpan(dir)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"tex1", tex1_parameter},
      {"tex2", tex2_parameter},
      {"dir", dir_parameter}};

  DirectionMixFloatTexture actual;
  EXPECT_TRUE(
      RemoveDirectionMixFloatTexture(parameters, /*pbrt_version=*/4, actual)
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                tex1 { float_texture_name: "tex1" }
                tex2 { float_texture_name: "tex2" }
                dir { x: 1.0 y: 2.0 z: 3.0 }
              )pb"));
}

TEST(RemoveDirectionMixSpectrumTexture, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  DirectionMixSpectrumTexture actual;
  EXPECT_TRUE(
      RemoveDirectionMixSpectrumTexture(parameters, /*pbrt_version=*/4, actual)
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveDirectionMixSpectrumTexture, WithData) {
  std::vector<absl::string_view> tex1 = {"tex1"};
  Parameter tex1_parameter{/*directive=*/"",
                           /*type=*/ParameterType::TEXTURE,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(tex1)};

  std::vector<absl::string_view> tex2 = {"tex2"};
  Parameter tex2_parameter{/*directive=*/"",
                           /*type=*/ParameterType::TEXTURE,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(tex2)};

  std::vector<std::array<double, 3>> dir = {{1.0, 2.0, 3.0}};
  Parameter dir_parameter{/*directive=*/"",
                          /*type=*/ParameterType::VECTOR3,
                          /*type_name=*/"",
                          /*values=*/absl::MakeSpan(dir)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"tex1", tex1_parameter},
      {"tex2", tex2_parameter},
      {"dir", dir_parameter}};

  DirectionMixSpectrumTexture actual;
  EXPECT_TRUE(
      RemoveDirectionMixSpectrumTexture(parameters, /*pbrt_version=*/4, actual)
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                tex1 { spectrum_texture_name: "tex1" }
                tex2 { spectrum_texture_name: "tex2" }
                dir { x: 1.0 y: 2.0 z: 3.0 }
              )pb"));
}

TEST(RemoveDotsFloatTexture, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  DotsFloatTexture actual;
  EXPECT_TRUE(
      RemoveDotsFloatTexture(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveDotsFloatTexture, WithData) {
  std::vector<absl::string_view> inside = {"inside"};
  Parameter inside_parameter{/*directive=*/"",
                             /*type=*/ParameterType::TEXTURE,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(inside)};

  std::vector<absl::string_view> outside = {"outside"};
  Parameter outside_parameter{/*directive=*/"",
                              /*type=*/ParameterType::TEXTURE,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(outside)};

  std::vector<absl::string_view> mapping = {"uv"};
  Parameter mapping_parameter{/*directive=*/"",
                              /*type=*/ParameterType::STRING,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(mapping)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"inside", inside_parameter},
      {"outside", outside_parameter},
      {"mapping", mapping_parameter}};

  DotsFloatTexture actual;
  EXPECT_TRUE(
      RemoveDotsFloatTexture(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                inside { float_texture_name: "inside" }
                outside { float_texture_name: "outside" }
                mapping: UV
              )pb"));
}

TEST(RemoveDotsSpectrumTexture, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  DotsSpectrumTexture actual;
  EXPECT_TRUE(
      RemoveDotsSpectrumTexture(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveDotsSpectrumTexture, WithData) {
  std::vector<absl::string_view> inside = {"inside"};
  Parameter inside_parameter{/*directive=*/"",
                             /*type=*/ParameterType::TEXTURE,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(inside)};

  std::vector<absl::string_view> outside = {"outside"};
  Parameter outside_parameter{/*directive=*/"",
                              /*type=*/ParameterType::TEXTURE,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(outside)};

  std::vector<absl::string_view> mapping = {"uv"};
  Parameter mapping_parameter{/*directive=*/"",
                              /*type=*/ParameterType::STRING,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(mapping)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"inside", inside_parameter},
      {"outside", outside_parameter},
      {"mapping", mapping_parameter}};

  DotsSpectrumTexture actual;
  EXPECT_TRUE(
      RemoveDotsSpectrumTexture(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                inside { spectrum_texture_name: "inside" }
                outside { spectrum_texture_name: "outside" }
                mapping: UV
              )pb"));
}

TEST(RemoveFBmFloatTexture, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  FBmFloatTexture actual;
  EXPECT_TRUE(
      RemoveFBmFloatTexture(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveFBmFloatTexture, WithData) {
  std::vector<int32_t> octaves = {1};
  Parameter octaves_parameter{/*directive=*/"",
                              /*type=*/ParameterType::INTEGER,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(octaves)};

  std::vector<double> roughness = {2.0};
  Parameter roughness_parameter{/*directive=*/"",
                                /*type=*/ParameterType::FLOAT,
                                /*type_name=*/"",
                                /*values=*/absl::MakeSpan(roughness)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"octaves", octaves_parameter}, {"roughness", roughness_parameter}};

  FBmFloatTexture actual;
  EXPECT_TRUE(
      RemoveFBmFloatTexture(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                octaves: 1 roughness: 2.0
              )pb"));
}

TEST(RemoveFBmSpectrumTexture, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  FBmSpectrumTexture actual;
  EXPECT_TRUE(
      RemoveFBmSpectrumTexture(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveFBmSpectrumTexture, WithData) {
  std::vector<int32_t> octaves = {1};
  Parameter octaves_parameter{/*directive=*/"",
                              /*type=*/ParameterType::INTEGER,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(octaves)};

  std::vector<double> roughness = {2.0};
  Parameter roughness_parameter{/*directive=*/"",
                                /*type=*/ParameterType::FLOAT,
                                /*type_name=*/"",
                                /*values=*/absl::MakeSpan(roughness)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"octaves", octaves_parameter}, {"roughness", roughness_parameter}};

  FBmSpectrumTexture actual;
  EXPECT_TRUE(
      RemoveFBmSpectrumTexture(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                octaves: 1 roughness: 2.0
              )pb"));
}

TEST(RemoveImageMapFloatTextureV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  ImageMapFloatTexture actual;
  EXPECT_TRUE(
      RemoveImageMapFloatTexture(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveImageMapFloatTextureV1, WithData) {
  std::vector<absl::string_view> filename = {"filename"};
  Parameter filename_parameter{/*directive=*/"",
                               /*type=*/ParameterType::STRING,
                               /*type_name=*/"",
                               /*values=*/absl::MakeSpan(filename)};

  std::vector<absl::string_view> wrap = {"black"};
  Parameter wrap_parameter{/*directive=*/"",
                           /*type=*/ParameterType::STRING,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(wrap)};

  bool trilinear[] = {true};
  Parameter trilinear_parameter{/*directive=*/"",
                                /*type=*/ParameterType::BOOL,
                                /*type_name=*/"",
                                /*values=*/absl::MakeSpan(trilinear)};

  std::vector<double> maxanisotropy = {1.0};
  Parameter maxanisotropy_parameter{/*directive=*/"",
                                    /*type=*/ParameterType::FLOAT,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(maxanisotropy)};

  std::vector<absl::string_view> mapping = {"uv"};
  Parameter mapping_parameter{/*directive=*/"",
                              /*type=*/ParameterType::STRING,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(mapping)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"filename", filename_parameter},
      {"wrap", wrap_parameter},
      {"trilinear", trilinear_parameter},
      {"maxanisotropy", maxanisotropy_parameter},
      {"mapping", mapping_parameter}};

  ImageMapFloatTexture actual;
  EXPECT_TRUE(
      RemoveImageMapFloatTexture(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                filename: "filename"
                wrap: BLACK
                filter: TRILINEAR
                maxanisotropy: 1.0
                mapping: UV
              )pb"));
}

TEST(RemoveImageMapFloatTextureV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  ImageMapFloatTexture actual;
  EXPECT_TRUE(
      RemoveImageMapFloatTexture(parameters, /*pbrt_version=*/2, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveImageMapFloatTextureV2, WithData) {
  std::vector<absl::string_view> filename = {"filename"};
  Parameter filename_parameter{/*directive=*/"",
                               /*type=*/ParameterType::STRING,
                               /*type_name=*/"",
                               /*values=*/absl::MakeSpan(filename)};

  std::vector<absl::string_view> wrap = {"black"};
  Parameter wrap_parameter{/*directive=*/"",
                           /*type=*/ParameterType::STRING,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(wrap)};

  bool trilinear[] = {true};
  Parameter trilinear_parameter{/*directive=*/"",
                                /*type=*/ParameterType::BOOL,
                                /*type_name=*/"",
                                /*values=*/absl::MakeSpan(trilinear)};

  std::vector<double> maxanisotropy = {1.0};
  Parameter maxanisotropy_parameter{/*directive=*/"",
                                    /*type=*/ParameterType::FLOAT,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(maxanisotropy)};

  std::vector<absl::string_view> mapping = {"uv"};
  Parameter mapping_parameter{/*directive=*/"",
                              /*type=*/ParameterType::STRING,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(mapping)};

  std::vector<double> scale = {2.0};
  Parameter scale_parameter{/*directive=*/"",
                            /*type=*/ParameterType::FLOAT,
                            /*type_name=*/"",
                            /*values=*/absl::MakeSpan(scale)};

  std::vector<double> gamma = {3.0};
  Parameter gamma_parameter{/*directive=*/"",
                            /*type=*/ParameterType::FLOAT,
                            /*type_name=*/"",
                            /*values=*/absl::MakeSpan(gamma)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"filename", filename_parameter},
      {"wrap", wrap_parameter},
      {"trilinear", trilinear_parameter},
      {"maxanisotropy", maxanisotropy_parameter},
      {"mapping", mapping_parameter},
      {"scale", scale_parameter},
      {"gamma", gamma_parameter}};

  ImageMapFloatTexture actual;
  EXPECT_TRUE(
      RemoveImageMapFloatTexture(parameters, /*pbrt_version=*/2, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                filename: "filename"
                wrap: BLACK
                filter: TRILINEAR
                maxanisotropy: 1.0
                mapping: UV
                scale: 2.0
                gamma: 3.0
              )pb"));
}

TEST(RemoveImageMapFloatTextureV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  ImageMapFloatTexture actual;
  EXPECT_TRUE(
      RemoveImageMapFloatTexture(parameters, /*pbrt_version=*/3, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveImageMapFloatTextureV3, WithData) {
  std::vector<absl::string_view> filename = {"filename"};
  Parameter filename_parameter{/*directive=*/"",
                               /*type=*/ParameterType::STRING,
                               /*type_name=*/"",
                               /*values=*/absl::MakeSpan(filename)};

  std::vector<absl::string_view> wrap = {"black"};
  Parameter wrap_parameter{/*directive=*/"",
                           /*type=*/ParameterType::STRING,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(wrap)};

  bool trilinear[] = {true};
  Parameter trilinear_parameter{/*directive=*/"",
                                /*type=*/ParameterType::BOOL,
                                /*type_name=*/"",
                                /*values=*/absl::MakeSpan(trilinear)};

  std::vector<double> maxanisotropy = {1.0};
  Parameter maxanisotropy_parameter{/*directive=*/"",
                                    /*type=*/ParameterType::FLOAT,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(maxanisotropy)};

  std::vector<absl::string_view> mapping = {"uv"};
  Parameter mapping_parameter{/*directive=*/"",
                              /*type=*/ParameterType::STRING,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(mapping)};

  std::vector<double> scale = {2.0};
  Parameter scale_parameter{/*directive=*/"",
                            /*type=*/ParameterType::FLOAT,
                            /*type_name=*/"",
                            /*values=*/absl::MakeSpan(scale)};

  bool gamma[] = {true};
  Parameter gamma_parameter{/*directive=*/"",
                            /*type=*/ParameterType::BOOL,
                            /*type_name=*/"",
                            /*values=*/absl::MakeSpan(gamma)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"filename", filename_parameter},
      {"wrap", wrap_parameter},
      {"trilinear", trilinear_parameter},
      {"maxanisotropy", maxanisotropy_parameter},
      {"mapping", mapping_parameter},
      {"scale", scale_parameter},
      {"gamma", gamma_parameter}};

  ImageMapFloatTexture actual;
  EXPECT_TRUE(
      RemoveImageMapFloatTexture(parameters, /*pbrt_version=*/3, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                filename: "filename"
                wrap: BLACK
                filter: TRILINEAR
                maxanisotropy: 1.0
                mapping: UV
                scale: 2.0
                encoding: GAMMA
              )pb"));
}

TEST(RemoveImageMapFloatTextureV4, Empty) {
  std::vector<absl::string_view> filename = {"filename"};
  Parameter filename_parameter{/*directive=*/"",
                               /*type=*/ParameterType::STRING,
                               /*type_name=*/"",
                               /*values=*/absl::MakeSpan(filename)};

  std::vector<absl::string_view> wrap = {"black"};
  Parameter wrap_parameter{/*directive=*/"",
                           /*type=*/ParameterType::STRING,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(wrap)};

  std::vector<absl::string_view> filter = {"bilinear"};
  Parameter filter_parameter{/*directive=*/"",
                             /*type=*/ParameterType::STRING,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(filter)};

  std::vector<double> maxanisotropy = {1.0};
  Parameter maxanisotropy_parameter{/*directive=*/"",
                                    /*type=*/ParameterType::FLOAT,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(maxanisotropy)};

  std::vector<absl::string_view> mapping = {"uv"};
  Parameter mapping_parameter{/*directive=*/"",
                              /*type=*/ParameterType::STRING,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(mapping)};

  std::vector<double> scale = {2.0};
  Parameter scale_parameter{/*directive=*/"",
                            /*type=*/ParameterType::FLOAT,
                            /*type_name=*/"",
                            /*values=*/absl::MakeSpan(scale)};

  std::vector<absl::string_view> encoding = {"gamma 2.2"};
  Parameter encoding_parameter{/*directive=*/"",
                               /*type=*/ParameterType::STRING,
                               /*type_name=*/"",
                               /*values=*/absl::MakeSpan(encoding)};

  bool invert[] = {true};
  Parameter invert_parameter{/*directive=*/"",
                             /*type=*/ParameterType::BOOL,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(invert)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"filename", filename_parameter},
      {"wrap", wrap_parameter},
      {"filter", filter_parameter},
      {"maxanisotropy", maxanisotropy_parameter},
      {"mapping", mapping_parameter},
      {"scale", scale_parameter},
      {"encoding", encoding_parameter},
      {"invert", invert_parameter}};

  ImageMapFloatTexture actual;
  EXPECT_TRUE(
      RemoveImageMapFloatTexture(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                filename: "filename"
                wrap: BLACK
                filter: BILINEAR
                maxanisotropy: 1.0
                mapping: UV
                scale: 2.0
                encoding: GAMMA
                gamma: 2.2
                invert: true
              )pb"));
}

TEST(RemoveImageMapFloatTextureV4, WithData) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  ImageMapFloatTexture actual;
  EXPECT_TRUE(
      RemoveImageMapFloatTexture(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveImageMapSpectrumTextureV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  ImageMapSpectrumTexture actual;
  EXPECT_TRUE(
      RemoveImageMapSpectrumTexture(parameters, /*pbrt_version=*/1, actual)
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveImageMapSpectrumTextureV1, WithData) {
  std::vector<absl::string_view> filename = {"filename"};
  Parameter filename_parameter{/*directive=*/"",
                               /*type=*/ParameterType::STRING,
                               /*type_name=*/"",
                               /*values=*/absl::MakeSpan(filename)};

  std::vector<absl::string_view> wrap = {"black"};
  Parameter wrap_parameter{/*directive=*/"",
                           /*type=*/ParameterType::STRING,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(wrap)};

  bool trilinear[] = {true};
  Parameter trilinear_parameter{/*directive=*/"",
                                /*type=*/ParameterType::BOOL,
                                /*type_name=*/"",
                                /*values=*/absl::MakeSpan(trilinear)};

  std::vector<double> maxanisotropy = {1.0};
  Parameter maxanisotropy_parameter{/*directive=*/"",
                                    /*type=*/ParameterType::FLOAT,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(maxanisotropy)};

  std::vector<absl::string_view> mapping = {"uv"};
  Parameter mapping_parameter{/*directive=*/"",
                              /*type=*/ParameterType::STRING,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(mapping)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"filename", filename_parameter},
      {"wrap", wrap_parameter},
      {"trilinear", trilinear_parameter},
      {"maxanisotropy", maxanisotropy_parameter},
      {"mapping", mapping_parameter}};

  ImageMapSpectrumTexture actual;
  EXPECT_TRUE(
      RemoveImageMapSpectrumTexture(parameters, /*pbrt_version=*/1, actual)
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                filename: "filename"
                wrap: BLACK
                filter: TRILINEAR
                maxanisotropy: 1.0
                mapping: UV
              )pb"));
}

TEST(RemoveImageMapSpectrumTextureV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  ImageMapSpectrumTexture actual;
  EXPECT_TRUE(
      RemoveImageMapSpectrumTexture(parameters, /*pbrt_version=*/2, actual)
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveImageMapSpectrumTextureV2, WithData) {
  std::vector<absl::string_view> filename = {"filename"};
  Parameter filename_parameter{/*directive=*/"",
                               /*type=*/ParameterType::STRING,
                               /*type_name=*/"",
                               /*values=*/absl::MakeSpan(filename)};

  std::vector<absl::string_view> wrap = {"black"};
  Parameter wrap_parameter{/*directive=*/"",
                           /*type=*/ParameterType::STRING,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(wrap)};

  bool trilinear[] = {true};
  Parameter trilinear_parameter{/*directive=*/"",
                                /*type=*/ParameterType::BOOL,
                                /*type_name=*/"",
                                /*values=*/absl::MakeSpan(trilinear)};

  std::vector<double> maxanisotropy = {1.0};
  Parameter maxanisotropy_parameter{/*directive=*/"",
                                    /*type=*/ParameterType::FLOAT,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(maxanisotropy)};

  std::vector<absl::string_view> mapping = {"uv"};
  Parameter mapping_parameter{/*directive=*/"",
                              /*type=*/ParameterType::STRING,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(mapping)};

  std::vector<double> scale = {2.0};
  Parameter scale_parameter{/*directive=*/"",
                            /*type=*/ParameterType::FLOAT,
                            /*type_name=*/"",
                            /*values=*/absl::MakeSpan(scale)};

  std::vector<double> gamma = {3.0};
  Parameter gamma_parameter{/*directive=*/"",
                            /*type=*/ParameterType::FLOAT,
                            /*type_name=*/"",
                            /*values=*/absl::MakeSpan(gamma)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"filename", filename_parameter},
      {"wrap", wrap_parameter},
      {"trilinear", trilinear_parameter},
      {"maxanisotropy", maxanisotropy_parameter},
      {"mapping", mapping_parameter},
      {"scale", scale_parameter},
      {"gamma", gamma_parameter}};

  ImageMapSpectrumTexture actual;
  EXPECT_TRUE(
      RemoveImageMapSpectrumTexture(parameters, /*pbrt_version=*/2, actual)
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                filename: "filename"
                wrap: BLACK
                filter: TRILINEAR
                maxanisotropy: 1.0
                mapping: UV
                scale: 2.0
                gamma: 3.0
              )pb"));
}

TEST(RemoveImageMapSpectrumTextureV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  ImageMapSpectrumTexture actual;
  EXPECT_TRUE(
      RemoveImageMapSpectrumTexture(parameters, /*pbrt_version=*/3, actual)
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveImageMapSpectrumTextureV3, WithData) {
  std::vector<absl::string_view> filename = {"filename"};
  Parameter filename_parameter{/*directive=*/"",
                               /*type=*/ParameterType::STRING,
                               /*type_name=*/"",
                               /*values=*/absl::MakeSpan(filename)};

  std::vector<absl::string_view> wrap = {"black"};
  Parameter wrap_parameter{/*directive=*/"",
                           /*type=*/ParameterType::STRING,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(wrap)};

  bool trilinear[] = {true};
  Parameter trilinear_parameter{/*directive=*/"",
                                /*type=*/ParameterType::BOOL,
                                /*type_name=*/"",
                                /*values=*/absl::MakeSpan(trilinear)};

  std::vector<double> maxanisotropy = {1.0};
  Parameter maxanisotropy_parameter{/*directive=*/"",
                                    /*type=*/ParameterType::FLOAT,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(maxanisotropy)};

  std::vector<absl::string_view> mapping = {"uv"};
  Parameter mapping_parameter{/*directive=*/"",
                              /*type=*/ParameterType::STRING,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(mapping)};

  std::vector<double> scale = {2.0};
  Parameter scale_parameter{/*directive=*/"",
                            /*type=*/ParameterType::FLOAT,
                            /*type_name=*/"",
                            /*values=*/absl::MakeSpan(scale)};

  bool gamma[] = {true};
  Parameter gamma_parameter{/*directive=*/"",
                            /*type=*/ParameterType::BOOL,
                            /*type_name=*/"",
                            /*values=*/absl::MakeSpan(gamma)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"filename", filename_parameter},
      {"wrap", wrap_parameter},
      {"trilinear", trilinear_parameter},
      {"maxanisotropy", maxanisotropy_parameter},
      {"mapping", mapping_parameter},
      {"scale", scale_parameter},
      {"gamma", gamma_parameter}};

  ImageMapSpectrumTexture actual;
  EXPECT_TRUE(
      RemoveImageMapSpectrumTexture(parameters, /*pbrt_version=*/3, actual)
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                filename: "filename"
                wrap: BLACK
                filter: TRILINEAR
                maxanisotropy: 1.0
                mapping: UV
                scale: 2.0
                encoding: GAMMA
              )pb"));
}

TEST(RemoveImageMapSpectrumTextureV4, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  ImageMapSpectrumTexture actual;
  EXPECT_TRUE(
      RemoveImageMapSpectrumTexture(parameters, /*pbrt_version=*/4, actual)
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveImageMapSpectrumTextureV4, WithData) {
  std::vector<absl::string_view> filename = {"filename"};
  Parameter filename_parameter{/*directive=*/"",
                               /*type=*/ParameterType::STRING,
                               /*type_name=*/"",
                               /*values=*/absl::MakeSpan(filename)};

  std::vector<absl::string_view> wrap = {"black"};
  Parameter wrap_parameter{/*directive=*/"",
                           /*type=*/ParameterType::STRING,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(wrap)};

  std::vector<absl::string_view> filter = {"bilinear"};
  Parameter filter_parameter{/*directive=*/"",
                             /*type=*/ParameterType::STRING,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(filter)};

  std::vector<double> maxanisotropy = {1.0};
  Parameter maxanisotropy_parameter{/*directive=*/"",
                                    /*type=*/ParameterType::FLOAT,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(maxanisotropy)};

  std::vector<absl::string_view> mapping = {"uv"};
  Parameter mapping_parameter{/*directive=*/"",
                              /*type=*/ParameterType::STRING,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(mapping)};

  std::vector<double> scale = {2.0};
  Parameter scale_parameter{/*directive=*/"",
                            /*type=*/ParameterType::FLOAT,
                            /*type_name=*/"",
                            /*values=*/absl::MakeSpan(scale)};

  std::vector<absl::string_view> encoding = {"gamma 2.2"};
  Parameter encoding_parameter{/*directive=*/"",
                               /*type=*/ParameterType::STRING,
                               /*type_name=*/"",
                               /*values=*/absl::MakeSpan(encoding)};

  bool invert[] = {true};
  Parameter invert_parameter{/*directive=*/"",
                             /*type=*/ParameterType::BOOL,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(invert)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"filename", filename_parameter},
      {"wrap", wrap_parameter},
      {"filter", filter_parameter},
      {"maxanisotropy", maxanisotropy_parameter},
      {"mapping", mapping_parameter},
      {"scale", scale_parameter},
      {"encoding", encoding_parameter},
      {"invert", invert_parameter}};

  ImageMapSpectrumTexture actual;
  EXPECT_TRUE(
      RemoveImageMapSpectrumTexture(parameters, /*pbrt_version=*/4, actual)
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                filename: "filename"
                wrap: BLACK
                filter: BILINEAR
                maxanisotropy: 1.0
                mapping: UV
                scale: 2.0
                encoding: GAMMA
                gamma: 2.2
                invert: true
              )pb"));
}

TEST(RemoveMarbleSpectrumTexture, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  MarbleSpectrumTexture actual;
  EXPECT_TRUE(
      RemoveMarbleSpectrumTexture(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveMarbleSpectrumTexture, WithData) {
  std::vector<int32_t> octaves = {1};
  Parameter octaves_parameter{/*directive=*/"",
                              /*type=*/ParameterType::INTEGER,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(octaves)};

  std::vector<double> roughness = {2.0};
  Parameter roughness_parameter{/*directive=*/"",
                                /*type=*/ParameterType::FLOAT,
                                /*type_name=*/"",
                                /*values=*/absl::MakeSpan(roughness)};

  std::vector<double> scale = {3.0};
  Parameter scale_parameter{/*directive=*/"",
                            /*type=*/ParameterType::FLOAT,
                            /*type_name=*/"",
                            /*values=*/absl::MakeSpan(scale)};

  std::vector<double> variation = {4.0};
  Parameter variation_parameter{/*directive=*/"",
                                /*type=*/ParameterType::FLOAT,
                                /*type_name=*/"",
                                /*values=*/absl::MakeSpan(variation)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"octaves", octaves_parameter},
      {"roughness", roughness_parameter},
      {"scale", scale_parameter},
      {"variation", variation_parameter}};

  MarbleSpectrumTexture actual;
  EXPECT_TRUE(
      RemoveMarbleSpectrumTexture(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                octaves: 1
                roughness: 2.0
                scale: 3.0
                variation: 4.0
              )pb"));
}

TEST(RemoveMixFloatTexture, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  MixFloatTexture actual;
  EXPECT_TRUE(
      RemoveMixFloatTexture(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveMixFloatTexture, WithData) {
  std::vector<absl::string_view> tex1 = {"tex1"};
  Parameter tex1_parameter{/*directive=*/"",
                           /*type=*/ParameterType::TEXTURE,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(tex1)};

  std::vector<absl::string_view> tex2 = {"tex2"};
  Parameter tex2_parameter{/*directive=*/"",
                           /*type=*/ParameterType::TEXTURE,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(tex2)};

  std::vector<double> amount = {1.0};
  Parameter amount_parameter{/*directive=*/"",
                             /*type=*/ParameterType::FLOAT,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(amount)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"tex1", tex1_parameter},
      {"tex2", tex2_parameter},
      {"amount", amount_parameter}};

  MixFloatTexture actual;
  EXPECT_TRUE(
      RemoveMixFloatTexture(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                tex1 { float_texture_name: "tex1" }
                tex2 { float_texture_name: "tex2" }
                amount { float_value: 1.0 }
              )pb"));
}

TEST(RemoveMixSpectrumTexture, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  MixSpectrumTexture actual;
  EXPECT_TRUE(
      RemoveMixSpectrumTexture(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveMixSpectrumTexture, WithData) {
  std::vector<absl::string_view> tex1 = {"tex1"};
  Parameter tex1_parameter{/*directive=*/"",
                           /*type=*/ParameterType::TEXTURE,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(tex1)};

  std::vector<absl::string_view> tex2 = {"tex2"};
  Parameter tex2_parameter{/*directive=*/"",
                           /*type=*/ParameterType::TEXTURE,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(tex2)};

  std::vector<double> amount = {1.0};
  Parameter amount_parameter{/*directive=*/"",
                             /*type=*/ParameterType::FLOAT,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(amount)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"tex1", tex1_parameter},
      {"tex2", tex2_parameter},
      {"amount", amount_parameter}};

  MixSpectrumTexture actual;
  EXPECT_TRUE(
      RemoveMixSpectrumTexture(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                tex1 { spectrum_texture_name: "tex1" }
                tex2 { spectrum_texture_name: "tex2" }
                amount { float_value: 1.0 }
              )pb"));
}

TEST(RemovePtexFloatTextureV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  PtexFloatTexture actual;
  EXPECT_TRUE(
      RemovePtexFloatTexture(parameters, /*pbrt_version=*/3, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemovePtexFloatTextureV3, WithData) {
  std::vector<absl::string_view> filename = {"filename"};
  Parameter filename_parameter{/*directive=*/"",
                               /*type=*/ParameterType::STRING,
                               /*type_name=*/"",
                               /*values=*/absl::MakeSpan(filename)};

  std::vector<double> gamma = {1.0};
  Parameter gamma_parameter{/*directive=*/"",
                            /*type=*/ParameterType::FLOAT,
                            /*type_name=*/"",
                            /*values=*/absl::MakeSpan(gamma)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"filename", filename_parameter}, {"gamma", gamma_parameter}};

  PtexFloatTexture actual;
  EXPECT_TRUE(
      RemovePtexFloatTexture(parameters, /*pbrt_version=*/3, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                filename: "filename" gamma: 1.0
              )pb"));
}

TEST(RemovePtexFloatTextureV4, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  PtexFloatTexture actual;
  EXPECT_TRUE(
      RemovePtexFloatTexture(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemovePtexFloatTextureV4, WithData) {
  std::vector<absl::string_view> filename = {"filename"};
  Parameter filename_parameter{/*directive=*/"",
                               /*type=*/ParameterType::STRING,
                               /*type_name=*/"",
                               /*values=*/absl::MakeSpan(filename)};

  std::vector<double> scale = {1.0};
  Parameter scale_parameter{/*directive=*/"",
                            /*type=*/ParameterType::FLOAT,
                            /*type_name=*/"",
                            /*values=*/absl::MakeSpan(scale)};

  std::vector<absl::string_view> encoding = {"gamma 2.2"};
  Parameter encoding_parameter{/*directive=*/"",
                               /*type=*/ParameterType::STRING,
                               /*type_name=*/"",
                               /*values=*/absl::MakeSpan(encoding)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"filename", filename_parameter},
      {"scale", scale_parameter},
      {"scale", scale_parameter},
      {"encoding", encoding_parameter}};

  PtexFloatTexture actual;
  EXPECT_TRUE(
      RemovePtexFloatTexture(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                filename: "filename"
                scale: 1.0
                encoding: GAMMA
                gamma: 2.2
              )pb"));
}

TEST(RemovePtexSpectrumTextureV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  PtexSpectrumTexture actual;
  EXPECT_TRUE(
      RemovePtexSpectrumTexture(parameters, /*pbrt_version=*/3, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemovePtexSpectrumTextureV3, WithData) {
  std::vector<absl::string_view> filename = {"filename"};
  Parameter filename_parameter{/*directive=*/"",
                               /*type=*/ParameterType::STRING,
                               /*type_name=*/"",
                               /*values=*/absl::MakeSpan(filename)};

  std::vector<double> gamma = {1.0};
  Parameter gamma_parameter{/*directive=*/"",
                            /*type=*/ParameterType::FLOAT,
                            /*type_name=*/"",
                            /*values=*/absl::MakeSpan(gamma)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"filename", filename_parameter}, {"gamma", gamma_parameter}};

  PtexSpectrumTexture actual;
  EXPECT_TRUE(
      RemovePtexSpectrumTexture(parameters, /*pbrt_version=*/3, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                filename: "filename" gamma: 1.0
              )pb"));
}

TEST(RemovePtexSpectrumTextureV4, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  PtexSpectrumTexture actual;
  EXPECT_TRUE(
      RemovePtexSpectrumTexture(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemovePtexSpectrumTextureV4, WithData) {
  std::vector<absl::string_view> filename = {"filename"};
  Parameter filename_parameter{/*directive=*/"",
                               /*type=*/ParameterType::STRING,
                               /*type_name=*/"",
                               /*values=*/absl::MakeSpan(filename)};

  std::vector<double> scale = {1.0};
  Parameter scale_parameter{/*directive=*/"",
                            /*type=*/ParameterType::FLOAT,
                            /*type_name=*/"",
                            /*values=*/absl::MakeSpan(scale)};

  std::vector<absl::string_view> encoding = {"gamma 2.2"};
  Parameter encoding_parameter{/*directive=*/"",
                               /*type=*/ParameterType::STRING,
                               /*type_name=*/"",
                               /*values=*/absl::MakeSpan(encoding)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"filename", filename_parameter},
      {"scale", scale_parameter},
      {"scale", scale_parameter},
      {"encoding", encoding_parameter}};

  PtexSpectrumTexture actual;
  EXPECT_TRUE(
      RemovePtexSpectrumTexture(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                filename: "filename"
                scale: 1.0
                encoding: GAMMA
                gamma: 2.2
              )pb"));
}

TEST(RemoveScaleFloatTexture, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  ScaleFloatTexture actual;
  EXPECT_TRUE(
      RemoveScaleFloatTexture(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveScaleFloatTexture, WithData) {
  std::vector<absl::string_view> tex1 = {"tex1"};
  Parameter tex1_parameter{/*directive=*/"",
                           /*type=*/ParameterType::TEXTURE,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(tex1)};

  std::vector<absl::string_view> tex2 = {"tex2"};
  Parameter tex2_parameter{/*directive=*/"",
                           /*type=*/ParameterType::TEXTURE,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(tex2)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"tex1", tex1_parameter}, {"tex2", tex2_parameter}};

  ScaleFloatTexture actual;
  EXPECT_TRUE(
      RemoveScaleFloatTexture(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                tex1 { float_texture_name: "tex1" }
                tex2 { float_texture_name: "tex2" }
              )pb"));
}

TEST(RemoveScaleSpectrumTexture, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  ScaleSpectrumTexture actual;
  EXPECT_TRUE(
      RemoveScaleSpectrumTexture(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveScaleSpectrumTexture, WithData) {
  std::vector<absl::string_view> tex1 = {"tex1"};
  Parameter tex1_parameter{/*directive=*/"",
                           /*type=*/ParameterType::TEXTURE,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(tex1)};

  std::vector<absl::string_view> tex2 = {"tex2"};
  Parameter tex2_parameter{/*directive=*/"",
                           /*type=*/ParameterType::TEXTURE,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(tex2)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"tex1", tex1_parameter}, {"tex2", tex2_parameter}};

  ScaleSpectrumTexture actual;
  EXPECT_TRUE(
      RemoveScaleSpectrumTexture(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                tex1 { spectrum_texture_name: "tex1" }
                tex2 { spectrum_texture_name: "tex2" }
              )pb"));
}

TEST(RemoveWrinkledFloatTexture, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  WrinkledFloatTexture actual;
  EXPECT_TRUE(
      RemoveWrinkledFloatTexture(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveWrinkledFloatTexture, WithData) {
  std::vector<int32_t> octaves = {1};
  Parameter octaves_parameter{/*directive=*/"",
                              /*type=*/ParameterType::INTEGER,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(octaves)};

  std::vector<double> roughness = {2.0};
  Parameter roughness_parameter{/*directive=*/"",
                                /*type=*/ParameterType::FLOAT,
                                /*type_name=*/"",
                                /*values=*/absl::MakeSpan(roughness)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"octaves", octaves_parameter}, {"roughness", roughness_parameter}};

  WrinkledFloatTexture actual;
  EXPECT_TRUE(
      RemoveWrinkledFloatTexture(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                octaves: 1 roughness: 2.0
              )pb"));
}

TEST(RemoveWrinkledSpectrumTexture, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  WrinkledSpectrumTexture actual;
  EXPECT_TRUE(
      RemoveWrinkledSpectrumTexture(parameters, /*pbrt_version=*/1, actual)
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveWrinkledSpectrumTexture, WithData) {
  std::vector<int32_t> octaves = {1};
  Parameter octaves_parameter{/*directive=*/"",
                              /*type=*/ParameterType::INTEGER,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(octaves)};

  std::vector<double> roughness = {2.0};
  Parameter roughness_parameter{/*directive=*/"",
                                /*type=*/ParameterType::FLOAT,
                                /*type_name=*/"",
                                /*values=*/absl::MakeSpan(roughness)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"octaves", octaves_parameter}, {"roughness", roughness_parameter}};

  WrinkledSpectrumTexture actual;
  EXPECT_TRUE(
      RemoveWrinkledSpectrumTexture(parameters, /*pbrt_version=*/1, actual)
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                octaves: 1 roughness: 2.0
              )pb"));
}

TEST(RemoveUvSpectrumTexture, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  UvSpectrumTexture actual;
  EXPECT_TRUE(
      RemoveUvSpectrumTexture(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveUvSpectrumTexture, WithData) {
  std::vector<absl::string_view> mapping = {"uv"};
  Parameter mapping_parameter{/*directive=*/"",
                              /*type=*/ParameterType::STRING,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(mapping)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"mapping", mapping_parameter}};

  UvSpectrumTexture actual;
  EXPECT_TRUE(
      RemoveUvSpectrumTexture(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                mapping: UV
              )pb"));
}

}  // namespace
}  // namespace pbrt_proto
