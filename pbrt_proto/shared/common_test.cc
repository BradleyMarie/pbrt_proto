#include "pbrt_proto/shared/common.h"

#include <array>
#include <functional>
#include <sstream>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "absl/strings/string_view.h"
#include "googlemock/include/gmock/gmock.h"
#include "googletest/include/gtest/gtest.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/common_test.pb.h"
#include "pbrt_proto/shared/parser.h"
#include "pbrt_proto/testing/proto_matchers.h"

namespace pbrt_proto {
namespace {

using ::google::protobuf::EqualsProto;
using ::testing::Eq;
using ::testing::Optional;

TEST(TryRemoveSpectrumV1, NotFound) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  TestParameterProto actual;
  TryRemoveSpectrumV1(
      parameters, "spectrum",
      std::bind(&TestParameterProto::mutable_spectrum, &actual));
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(TryRemoveSpectrumV1, Blackbody) {
  std::vector<std::array<double, 2>> values = {{1.0, 2.0}};
  Parameter parameter{.directive = "",
                      .type = ParameterType::BLACKBODY_V1,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"spectrum", parameter}};

  TestParameterProto actual;
  TryRemoveSpectrumV1(
      parameters, "spectrum",
      std::bind(&TestParameterProto::mutable_spectrum, &actual));
  EXPECT_THAT(actual, EqualsProto(R"pb(
                spectrum { blackbody_spectrum { temperature: 1.0 scale: 2.0 } }
              )pb"));
}

TEST(TryRemoveSpectrumV1, Rgb) {
  std::vector<std::array<double, 3>> values = {{1.0, 2.0, 3.0}};
  Parameter parameter{.directive = "",
                      .type = ParameterType::RGB,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"spectrum", parameter}};

  TestParameterProto actual;
  TryRemoveSpectrumV1(
      parameters, "spectrum",
      std::bind(&TestParameterProto::mutable_spectrum, &actual));
  EXPECT_THAT(actual, EqualsProto(R"pb(
                spectrum { rgb_spectrum { r: 1.0 g: 2.0 b: 3.0 } }
              )pb"));
}

TEST(TryRemoveSpectrumV1, Xyz) {
  std::vector<std::array<double, 3>> values = {{1.0, 2.0, 3.0}};
  Parameter parameter{.directive = "",
                      .type = ParameterType::XYZ,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"spectrum", parameter}};

  TestParameterProto actual;
  TryRemoveSpectrumV1(
      parameters, "spectrum",
      std::bind(&TestParameterProto::mutable_spectrum, &actual));
  EXPECT_THAT(actual, EqualsProto(R"pb(
                spectrum { xyz_spectrum { x: 1.0 y: 2.0 z: 3.0 } }
              )pb"));
}

TEST(TryRemoveSpectrumV1, Sampled) {
  std::vector<std::array<double, 2>> values = {{1.0, 2.0}};
  Parameter parameter{.directive = "",
                      .type = ParameterType::SPECTRUM,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"spectrum", parameter}};

  TestParameterProto actual;
  TryRemoveSpectrumV1(
      parameters, "spectrum",
      std::bind(&TestParameterProto::mutable_spectrum, &actual));
  EXPECT_THAT(
      actual, EqualsProto(R"pb(
        spectrum {
          sampled_spectrum { samples { wavelength: 1.0 intensity: 2.0 } }
        }
      )pb"));
}

TEST(TryRemoveSpectrumV1, SpectrumFile) {
  std::vector<absl::string_view> values = {"abc"};
  Parameter parameter{.directive = "",
                      .type = ParameterType::SPECTRUM,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"spectrum", parameter}};

  TestParameterProto actual;
  TryRemoveSpectrumV1(
      parameters, "spectrum",
      std::bind(&TestParameterProto::mutable_spectrum, &actual));
  EXPECT_THAT(actual, EqualsProto(R"pb(
                spectrum { sampled_spectrum_filename: "abc" }
              )pb"));
}

TEST(TryRemoveSpectrumV2, NotFound) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  TestParameterProto actual;
  TryRemoveSpectrumV2(
      parameters, "spectrum",
      std::bind(&TestParameterProto::mutable_spectrum, &actual));
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(TryRemoveSpectrumV2, Blackbody) {
  std::vector<double> values = {1.0};
  Parameter parameter{.directive = "",
                      .type = ParameterType::BLACKBODY_V2,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"spectrum", parameter}};

  TestParameterProto actual;
  TryRemoveSpectrumV2(
      parameters, "spectrum",
      std::bind(&TestParameterProto::mutable_spectrum, &actual));
  EXPECT_THAT(actual, EqualsProto(R"pb(
                spectrum { blackbody_spectrum { temperature: 1.0 } }
              )pb"));
}

TEST(TryRemoveSpectrumV2, Rgb) {
  std::vector<std::array<double, 3>> values = {{1.0, 2.0, 3.0}};
  Parameter parameter{.directive = "",
                      .type = ParameterType::RGB,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"spectrum", parameter}};

  TestParameterProto actual;
  TryRemoveSpectrumV2(
      parameters, "spectrum",
      std::bind(&TestParameterProto::mutable_spectrum, &actual));
  EXPECT_THAT(actual, EqualsProto(R"pb(
                spectrum { rgb_spectrum { r: 1.0 g: 2.0 b: 3.0 } }
              )pb"));
}

TEST(TryRemoveSpectrumV2, Xyz) {
  std::vector<std::array<double, 3>> values = {{1.0, 2.0, 3.0}};
  Parameter parameter{.directive = "",
                      .type = ParameterType::XYZ,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"spectrum", parameter}};

  TestParameterProto actual;
  TryRemoveSpectrumV2(
      parameters, "spectrum",
      std::bind(&TestParameterProto::mutable_spectrum, &actual));
  EXPECT_THAT(actual, EqualsProto(R"pb(
                spectrum { xyz_spectrum { x: 1.0 y: 2.0 z: 3.0 } }
              )pb"));
}

TEST(TryRemoveSpectrumV2, Sampled) {
  std::vector<std::array<double, 2>> values = {{1.0, 2.0}};
  Parameter parameter{.directive = "",
                      .type = ParameterType::SPECTRUM,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"spectrum", parameter}};

  TestParameterProto actual;
  TryRemoveSpectrumV2(
      parameters, "spectrum",
      std::bind(&TestParameterProto::mutable_spectrum, &actual));
  EXPECT_THAT(
      actual, EqualsProto(R"pb(
        spectrum {
          sampled_spectrum { samples { wavelength: 1.0 intensity: 2.0 } }
        }
      )pb"));
}

TEST(TryRemoveSpectrumV2, SpectrumFile) {
  std::vector<absl::string_view> values = {"abc"};
  Parameter parameter{.directive = "",
                      .type = ParameterType::SPECTRUM,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"spectrum", parameter}};

  TestParameterProto actual;
  TryRemoveSpectrumV2(
      parameters, "spectrum",
      std::bind(&TestParameterProto::mutable_spectrum, &actual));
  EXPECT_THAT(actual, EqualsProto(R"pb(
                spectrum { sampled_spectrum_filename: "abc" }
              )pb"));
}

TEST(TryRemoveFloatTexture, NotFound) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  TestParameterProto actual;
  TryRemoveFloatTexture(
      parameters, "float_parameter",
      std::bind(&TestParameterProto::mutable_float_parameter, &actual));
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(TryRemoveFloatTexture, FloatValue) {
  std::vector<double> values = {1.0};
  Parameter parameter{.directive = "",
                      .type = ParameterType::FLOAT,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"float_parameter", parameter}};

  TestParameterProto actual;
  TryRemoveFloatTexture(
      parameters, "float_parameter",
      std::bind(&TestParameterProto::mutable_float_parameter, &actual));
  EXPECT_THAT(actual, EqualsProto(R"pb(
                float_parameter { float_value: 1.0 }
              )pb"));
}

TEST(TryRemoveFloatTexture, FloatTexture) {
  std::vector<absl::string_view> values = {"abc"};
  Parameter parameter{.directive = "",
                      .type = ParameterType::TEXTURE,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"float_parameter", parameter}};

  TestParameterProto actual;
  TryRemoveFloatTexture(
      parameters, "float_parameter",
      std::bind(&TestParameterProto::mutable_float_parameter, &actual));
  EXPECT_THAT(actual, EqualsProto(R"pb(
                float_parameter { float_texture_name: "abc" }
              )pb"));
}

TEST(TryRemoveSpectrumTextureV1, NotFound) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  TestParameterProto actual;
  TryRemoveSpectrumTextureV1(
      parameters, "spectrum_parameter",
      std::bind(&TestParameterProto::mutable_spectrum_parameter, &actual));
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(TryRemoveSpectrumTextureV2, NotFound) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  TestParameterProto actual;
  TryRemoveSpectrumTextureV2(
      parameters, "spectrum_parameter",
      std::bind(&TestParameterProto::mutable_spectrum_parameter, &actual));
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(TryRemoveSpectrumTextureV1, Blackbody) {
  std::vector<std::array<double, 2>> values = {{1.0, 2.0}};
  Parameter parameter{.directive = "",
                      .type = ParameterType::BLACKBODY_V1,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"spectrum_parameter", parameter}};

  TestParameterProto actual;
  TryRemoveSpectrumTextureV1(
      parameters, "spectrum_parameter",
      std::bind(&TestParameterProto::mutable_spectrum_parameter, &actual));
  EXPECT_THAT(actual, EqualsProto(R"pb(
                spectrum_parameter {
                  blackbody_spectrum { temperature: 1.0 scale: 2.0 }
                }
              )pb"));
}

TEST(TryRemoveSpectrumTextureV1, Rgb) {
  std::vector<std::array<double, 3>> values = {{1.0, 2.0, 3.0}};
  Parameter parameter{.directive = "",
                      .type = ParameterType::RGB,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"spectrum_parameter", parameter}};

  TestParameterProto actual;
  TryRemoveSpectrumTextureV1(
      parameters, "spectrum_parameter",
      std::bind(&TestParameterProto::mutable_spectrum_parameter, &actual));
  EXPECT_THAT(actual, EqualsProto(R"pb(
                spectrum_parameter { rgb_spectrum { r: 1.0 g: 2.0 b: 3.0 } }
              )pb"));
}

TEST(TryRemoveSpectrumTextureV1, Xyz) {
  std::vector<std::array<double, 3>> values = {{1.0, 2.0, 3.0}};
  Parameter parameter{.directive = "",
                      .type = ParameterType::XYZ,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"spectrum_parameter", parameter}};

  TestParameterProto actual;
  TryRemoveSpectrumTextureV1(
      parameters, "spectrum_parameter",
      std::bind(&TestParameterProto::mutable_spectrum_parameter, &actual));
  EXPECT_THAT(actual, EqualsProto(R"pb(
                spectrum_parameter { xyz_spectrum { x: 1.0 y: 2.0 z: 3.0 } }
              )pb"));
}

TEST(TryRemoveSpectrumTextureV1, Sampled) {
  std::vector<std::array<double, 2>> values = {{1.0, 2.0}};
  Parameter parameter{.directive = "",
                      .type = ParameterType::SPECTRUM,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"spectrum_parameter", parameter}};

  TestParameterProto actual;
  TryRemoveSpectrumTextureV1(
      parameters, "spectrum_parameter",
      std::bind(&TestParameterProto::mutable_spectrum_parameter, &actual));
  EXPECT_THAT(
      actual, EqualsProto(R"pb(
        spectrum_parameter {
          sampled_spectrum { samples { wavelength: 1.0 intensity: 2.0 } }
        }
      )pb"));
}

TEST(TryRemoveSpectrumTextureV1, SpectrumFile) {
  std::vector<absl::string_view> values = {"abc"};
  Parameter parameter{.directive = "",
                      .type = ParameterType::SPECTRUM,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"spectrum_parameter", parameter}};

  TestParameterProto actual;
  TryRemoveSpectrumTextureV1(
      parameters, "spectrum_parameter",
      std::bind(&TestParameterProto::mutable_spectrum_parameter, &actual));
  EXPECT_THAT(actual, EqualsProto(R"pb(
                spectrum_parameter { sampled_spectrum_filename: "abc" }
              )pb"));
}

TEST(TryRemoveSpectrumTextureV1, SpectrumTexture) {
  std::vector<absl::string_view> values = {"abc"};
  Parameter parameter{.directive = "",
                      .type = ParameterType::TEXTURE,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"spectrum_parameter", parameter}};

  TestParameterProto actual;
  TryRemoveSpectrumTextureV1(
      parameters, "spectrum_parameter",
      std::bind(&TestParameterProto::mutable_spectrum_parameter, &actual));
  EXPECT_THAT(actual, EqualsProto(R"pb(
                spectrum_parameter { spectrum_texture_name: "abc" }
              )pb"));
}

TEST(TryRemoveSpectrumTextureV2, Blackbody) {
  std::vector<double> values = {1.0};
  Parameter parameter{.directive = "",
                      .type = ParameterType::BLACKBODY_V2,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"spectrum_parameter", parameter}};

  TestParameterProto actual;
  TryRemoveSpectrumTextureV2(
      parameters, "spectrum_parameter",
      std::bind(&TestParameterProto::mutable_spectrum_parameter, &actual));
  EXPECT_THAT(actual, EqualsProto(R"pb(
                spectrum_parameter { blackbody_spectrum { temperature: 1.0 } }
              )pb"));
}

TEST(TryRemoveSpectrumTextureV2, Rgb) {
  std::vector<std::array<double, 3>> values = {{1.0, 2.0, 3.0}};
  Parameter parameter{.directive = "",
                      .type = ParameterType::RGB,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"spectrum_parameter", parameter}};

  TestParameterProto actual;
  TryRemoveSpectrumTextureV2(
      parameters, "spectrum_parameter",
      std::bind(&TestParameterProto::mutable_spectrum_parameter, &actual));
  EXPECT_THAT(actual, EqualsProto(R"pb(
                spectrum_parameter { rgb_spectrum { r: 1.0 g: 2.0 b: 3.0 } }
              )pb"));
}

TEST(TryRemoveSpectrumTextureV2, Xyz) {
  std::vector<std::array<double, 3>> values = {{1.0, 2.0, 3.0}};
  Parameter parameter{.directive = "",
                      .type = ParameterType::XYZ,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"spectrum_parameter", parameter}};

  TestParameterProto actual;
  TryRemoveSpectrumTextureV2(
      parameters, "spectrum_parameter",
      std::bind(&TestParameterProto::mutable_spectrum_parameter, &actual));
  EXPECT_THAT(actual, EqualsProto(R"pb(
                spectrum_parameter { xyz_spectrum { x: 1.0 y: 2.0 z: 3.0 } }
              )pb"));
}

TEST(TryRemoveSpectrumTextureV2, Sampled) {
  std::vector<std::array<double, 2>> values = {{1.0, 2.0}};
  Parameter parameter{.directive = "",
                      .type = ParameterType::SPECTRUM,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"spectrum_parameter", parameter}};

  TestParameterProto actual;
  TryRemoveSpectrumTextureV2(
      parameters, "spectrum_parameter",
      std::bind(&TestParameterProto::mutable_spectrum_parameter, &actual));
  EXPECT_THAT(
      actual, EqualsProto(R"pb(
        spectrum_parameter {
          sampled_spectrum { samples { wavelength: 1.0 intensity: 2.0 } }
        }
      )pb"));
}

TEST(TryRemoveSpectrumTextureV2, SpectrumFile) {
  std::vector<absl::string_view> values = {"abc"};
  Parameter parameter{.directive = "",
                      .type = ParameterType::SPECTRUM,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"spectrum_parameter", parameter}};

  TestParameterProto actual;
  TryRemoveSpectrumTextureV2(
      parameters, "spectrum_parameter",
      std::bind(&TestParameterProto::mutable_spectrum_parameter, &actual));
  EXPECT_THAT(actual, EqualsProto(R"pb(
                spectrum_parameter { sampled_spectrum_filename: "abc" }
              )pb"));
}

TEST(TryRemoveSpectrumTextureV2, SpectrumTexture) {
  std::vector<absl::string_view> values = {"abc"};
  Parameter parameter{.directive = "",
                      .type = ParameterType::TEXTURE,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"spectrum_parameter", parameter}};

  TestParameterProto actual;
  TryRemoveSpectrumTextureV2(
      parameters, "spectrum_parameter",
      std::bind(&TestParameterProto::mutable_spectrum_parameter, &actual));
  EXPECT_THAT(actual, EqualsProto(R"pb(
                spectrum_parameter { spectrum_texture_name: "abc" }
              )pb"));
}

TEST(TryRemoveAaMode, NoValues) {
  std::vector<absl::string_view> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::STRING,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"aamode", parameter}};

  EXPECT_THAT(TryRemoveAaMode(parameters), Eq(std::nullopt));
}

TEST(TryRemoveAaMode, UnknownValue) {
  std::vector<absl::string_view> values = {"unknown"};
  Parameter parameter{.directive = "",
                      .type = ParameterType::STRING,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"aamode", parameter}};

  EXPECT_THAT(TryRemoveAaMode(parameters), Eq(std::nullopt));
}

TEST(TryRemoveAaMode, ClosedForm) {
  std::vector<absl::string_view> values = {"closedform"};
  Parameter parameter{.directive = "",
                      .type = ParameterType::STRING,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"aamode", parameter}};

  EXPECT_THAT(TryRemoveAaMode(parameters),
              Optional(CheckerboardAntialiasing::CLOSEDFORM));
}

TEST(TryRemoveAaMode, None) {
  std::vector<absl::string_view> values = {"none"};
  Parameter parameter{.directive = "",
                      .type = ParameterType::STRING,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"aamode", parameter}};

  EXPECT_THAT(TryRemoveAaMode(parameters),
              Optional(CheckerboardAntialiasing::NONE_AA));
}

TEST(TryRemoveMapping, NoValues) {
  std::vector<absl::string_view> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::STRING,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"mapping", parameter}};

  EXPECT_THAT(TryRemoveMapping(parameters), Eq(std::nullopt));
}

TEST(TryRemoveMapping, UnknownValue) {
  std::vector<absl::string_view> values = {"unknown"};
  Parameter parameter{.directive = "",
                      .type = ParameterType::STRING,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"mapping", parameter}};

  EXPECT_THAT(TryRemoveMapping(parameters), Eq(std::nullopt));
}

TEST(TryRemoveMapping, UV) {
  std::vector<absl::string_view> values = {"uv"};
  Parameter parameter{.directive = "",
                      .type = ParameterType::STRING,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"mapping", parameter}};

  EXPECT_THAT(TryRemoveMapping(parameters), Optional(TextureMapping::UV));
}

TEST(TryRemoveMapping, Spherical) {
  std::vector<absl::string_view> values = {"spherical"};
  Parameter parameter{.directive = "",
                      .type = ParameterType::STRING,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"mapping", parameter}};

  EXPECT_THAT(TryRemoveMapping(parameters),
              Optional(TextureMapping::SPHERICAL));
}

TEST(TryRemoveMapping, Planar) {
  std::vector<absl::string_view> values = {"planar"};
  Parameter parameter{.directive = "",
                      .type = ParameterType::STRING,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"mapping", parameter}};

  EXPECT_THAT(TryRemoveMapping(parameters), Optional(TextureMapping::PLANAR));
}

TEST(TryRemoveMapping, Cylindrical) {
  std::vector<absl::string_view> values = {"cylindrical"};
  Parameter parameter{.directive = "",
                      .type = ParameterType::STRING,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"mapping", parameter}};

  EXPECT_THAT(TryRemoveMapping(parameters),
              Optional(TextureMapping::CYLINDRICAL));
}

TEST(TryRemoveWrap, NoValues) {
  std::vector<absl::string_view> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::STRING,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"wrap", parameter}};

  EXPECT_THAT(TryRemoveWrap(parameters), Eq(std::nullopt));
}

TEST(TryRemoveWrap, UnknownValue) {
  std::vector<absl::string_view> values = {"unknown"};
  Parameter parameter{.directive = "",
                      .type = ParameterType::STRING,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"wrap", parameter}};

  EXPECT_THAT(TryRemoveWrap(parameters), Eq(std::nullopt));
}

TEST(TryRemoveWrap, Black) {
  std::vector<absl::string_view> values = {"black"};
  Parameter parameter{.directive = "",
                      .type = ParameterType::STRING,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"wrap", parameter}};

  EXPECT_THAT(TryRemoveWrap(parameters), Optional(ImageWrap::BLACK));
}

TEST(TryRemoveWrap, Clamp) {
  std::vector<absl::string_view> values = {"clamp"};
  Parameter parameter{.directive = "",
                      .type = ParameterType::STRING,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"wrap", parameter}};

  EXPECT_THAT(TryRemoveWrap(parameters), Optional(ImageWrap::CLAMP));
}

TEST(TryRemoveWrap, Repeat) {
  std::vector<absl::string_view> values = {"repeat"};
  Parameter parameter{.directive = "",
                      .type = ParameterType::STRING,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"wrap", parameter}};

  EXPECT_THAT(TryRemoveWrap(parameters), Optional(ImageWrap::REPEAT));
}

TEST(TryRemoveUVParameters, None) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters = {};

  TestUVTexture to_fill;
  TryRemoveUVParameters(parameters, to_fill);

  EXPECT_THAT(to_fill, EqualsProto(R"pb()pb"));
}

TEST(TryRemoveUVParameters, All) {
  std::vector<absl::string_view> mapping_values = {"uv"};
  Parameter mapping_parameter{.directive = "",
                              .type = ParameterType::STRING,
                              .type_name = "",
                              .values = absl::MakeSpan(mapping_values)};

  std::vector<double> uscale_values = {1.0};
  Parameter uscale_parameter{.directive = "",
                             .type = ParameterType::FLOAT,
                             .type_name = "",
                             .values = absl::MakeSpan(uscale_values)};

  std::vector<double> vscale_values = {2.0};
  Parameter vscale_parameter{.directive = "",
                             .type = ParameterType::FLOAT,
                             .type_name = "",
                             .values = absl::MakeSpan(vscale_values)};

  std::vector<double> udelta_values = {3.0};
  Parameter udelta_parameter{.directive = "",
                             .type = ParameterType::FLOAT,
                             .type_name = "",
                             .values = absl::MakeSpan(udelta_values)};

  std::vector<double> vdelta_values = {4.0};
  Parameter vdelta_parameter{.directive = "",
                             .type = ParameterType::FLOAT,
                             .type_name = "",
                             .values = absl::MakeSpan(vdelta_values)};

  std::vector<std::array<double, 3>> v1_values = {{5.0, 6.0, 7.0}};
  Parameter v1_parameter{.directive = "",
                         .type = ParameterType::VECTOR3,
                         .type_name = "",
                         .values = absl::MakeSpan(v1_values)};

  std::vector<std::array<double, 3>> v2_values = {{8.0, 9.0, 10.0}};
  Parameter v2_parameter{.directive = "",
                         .type = ParameterType::VECTOR3,
                         .type_name = "",
                         .values = absl::MakeSpan(v2_values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"mapping", mapping_parameter}, {"uscale", uscale_parameter},
      {"vscale", vscale_parameter},   {"udelta", udelta_parameter},
      {"vdelta", vdelta_parameter},   {"v1", v1_parameter},
      {"v2", v2_parameter},
  };

  TestUVTexture to_fill;
  TryRemoveUVParameters(parameters, to_fill);

  EXPECT_THAT(to_fill, EqualsProto(R"pb(
                mapping: UV
                uscale: 1.0
                vscale: 2.0
                udelta: 3.0
                vdelta: 4.0
                v1 { x: 5.0 y: 6.0 z: 7.0 }
                v2 { x: 8.0 y: 9.0 z: 10.0 }
              )pb"));
}

}  // namespace
}  // namespace pbrt_proto
