#include "pbrt_proto/shared/common.h"

#include <array>
#include <functional>
#include <sstream>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "absl/strings/string_view.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/common_test.pb.h"
#include "pbrt_proto/shared/parser.h"
#include "pbrt_proto/testing/proto_matchers.h"

namespace pbrt_proto {
namespace {

using ::google::protobuf::EqualsProto;
using ::testing::Eq;
using ::testing::Optional;

TEST(TryRemoveSpectrum, NotFound) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  TestParameterProto actual;
  EXPECT_TRUE(TryRemoveSpectrum(
                  parameters, "spectrum",
                  std::bind(&TestParameterProto::mutable_spectrum, &actual))
                  .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(TryRemoveSpectrum, BlackbodyV1) {
  std::vector<std::array<double, 2>> values = {{1.0, 2.0}};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::BLACKBODY_V1,
                      /*type_name=*/"",
                      /*values=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"spectrum", parameter}};

  TestParameterProto actual;
  EXPECT_TRUE(TryRemoveSpectrum(
                  parameters, "spectrum",
                  std::bind(&TestParameterProto::mutable_spectrum, &actual))
                  .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                spectrum { blackbody_spectrum { temperature: 1.0 scale: 2.0 } }
              )pb"));
}

TEST(TryRemoveSpectrum, BlackbodyV2) {
  std::vector<double> values = {1.0};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::BLACKBODY_V2,
                      /*type_name=*/"",
                      /*values=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"spectrum", parameter}};

  TestParameterProto actual;
  EXPECT_TRUE(TryRemoveSpectrum(
                  parameters, "spectrum",
                  std::bind(&TestParameterProto::mutable_spectrum, &actual))
                  .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                spectrum { blackbody_spectrum { temperature: 1.0 } }
              )pb"));
}

TEST(TryRemoveSpectrum, Rgb) {
  std::vector<std::array<double, 3>> values = {{1.0, 2.0, 3.0}};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::RGB,
                      /*type_name=*/"",
                      /*values=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"spectrum", parameter}};

  TestParameterProto actual;
  EXPECT_TRUE(TryRemoveSpectrum(
                  parameters, "spectrum",
                  std::bind(&TestParameterProto::mutable_spectrum, &actual))
                  .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                spectrum { rgb_spectrum { r: 1.0 g: 2.0 b: 3.0 } }
              )pb"));
}

TEST(TryRemoveSpectrum, Xyz) {
  std::vector<std::array<double, 3>> values = {{1.0, 2.0, 3.0}};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::XYZ,
                      /*type_name=*/"",
                      /*values=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"spectrum", parameter}};

  TestParameterProto actual;
  EXPECT_TRUE(TryRemoveSpectrum(
                  parameters, "spectrum",
                  std::bind(&TestParameterProto::mutable_spectrum, &actual))
                  .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                spectrum { xyz_spectrum { x: 1.0 y: 2.0 z: 3.0 } }
              )pb"));
}

TEST(TryRemoveSpectrum, Sampled) {
  std::vector<std::array<double, 2>> values = {{1.0, 2.0}};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::SPECTRUM,
                      /*type_name=*/"",
                      /*values=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"spectrum", parameter}};

  TestParameterProto actual;
  EXPECT_TRUE(TryRemoveSpectrum(
                  parameters, "spectrum",
                  std::bind(&TestParameterProto::mutable_spectrum, &actual))
                  .ok());
  EXPECT_THAT(
      actual, EqualsProto(R"pb(
        spectrum {
          sampled_spectrum { samples { wavelength: 1.0 intensity: 2.0 } }
        }
      )pb"));
}

TEST(TryRemoveSpectrum, SpectrumFile) {
  std::vector<absl::string_view> values = {"abc"};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::SPECTRUM,
                      /*type_name=*/"",
                      /*values=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"spectrum", parameter}};

  TestParameterProto actual;
  EXPECT_TRUE(TryRemoveSpectrum(
                  parameters, "spectrum",
                  std::bind(&TestParameterProto::mutable_spectrum, &actual))
                  .ok());
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
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::FLOAT,
                      /*type_name=*/"",
                      /*values=*/absl::MakeSpan(values)};

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
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::TEXTURE,
                      /*type_name=*/"",
                      /*values=*/absl::MakeSpan(values)};

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

TEST(TryRemoveSpectrumTexture, NotFound) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  TestParameterProto actual;
  EXPECT_TRUE(
      TryRemoveSpectrumTexture(
          parameters, "spectrum_parameter",
          std::bind(&TestParameterProto::mutable_spectrum_parameter, &actual))
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(TryRemoveSpectrumTexture, BlackbodyV2) {
  std::vector<double> values = {1.0};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::BLACKBODY_V2,
                      /*type_name=*/"",
                      /*values=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"spectrum_parameter", parameter}};

  TestParameterProto actual;
  EXPECT_TRUE(
      TryRemoveSpectrumTexture(
          parameters, "spectrum_parameter",
          std::bind(&TestParameterProto::mutable_spectrum_parameter, &actual))
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                spectrum_parameter { blackbody_spectrum { temperature: 1.0 } }
              )pb"));
}

TEST(TryRemoveSpectrumTexture, Rgb) {
  std::vector<std::array<double, 3>> values = {{1.0, 2.0, 3.0}};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::RGB,
                      /*type_name=*/"",
                      /*values=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"spectrum_parameter", parameter}};

  TestParameterProto actual;
  EXPECT_TRUE(
      TryRemoveSpectrumTexture(
          parameters, "spectrum_parameter",
          std::bind(&TestParameterProto::mutable_spectrum_parameter, &actual))
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                spectrum_parameter { rgb_spectrum { r: 1.0 g: 2.0 b: 3.0 } }
              )pb"));
}

TEST(TryRemoveSpectrumTexture, Xyz) {
  std::vector<std::array<double, 3>> values = {{1.0, 2.0, 3.0}};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::XYZ,
                      /*type_name=*/"",
                      /*values=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"spectrum_parameter", parameter}};

  TestParameterProto actual;
  EXPECT_TRUE(
      TryRemoveSpectrumTexture(
          parameters, "spectrum_parameter",
          std::bind(&TestParameterProto::mutable_spectrum_parameter, &actual))
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                spectrum_parameter { xyz_spectrum { x: 1.0 y: 2.0 z: 3.0 } }
              )pb"));
}

TEST(TryRemoveSpectrumTexture, Sampled) {
  std::vector<std::array<double, 2>> values = {{1.0, 2.0}};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::SPECTRUM,
                      /*type_name=*/"",
                      /*values=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"spectrum_parameter", parameter}};

  TestParameterProto actual;
  EXPECT_TRUE(
      TryRemoveSpectrumTexture(
          parameters, "spectrum_parameter",
          std::bind(&TestParameterProto::mutable_spectrum_parameter, &actual))
          .ok());
  EXPECT_THAT(
      actual, EqualsProto(R"pb(
        spectrum_parameter {
          sampled_spectrum { samples { wavelength: 1.0 intensity: 2.0 } }
        }
      )pb"));
}

TEST(TryRemoveSpectrumTexture, SpectrumFile) {
  std::vector<absl::string_view> values = {"abc"};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::SPECTRUM,
                      /*type_name=*/"",
                      /*values=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"spectrum_parameter", parameter}};

  TestParameterProto actual;
  EXPECT_TRUE(
      TryRemoveSpectrumTexture(
          parameters, "spectrum_parameter",
          std::bind(&TestParameterProto::mutable_spectrum_parameter, &actual))
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                spectrum_parameter { sampled_spectrum_filename: "abc" }
              )pb"));
}

TEST(TryRemoveSpectrumTexture, SpectrumTexture) {
  std::vector<absl::string_view> values = {"abc"};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::TEXTURE,
                      /*type_name=*/"",
                      /*values=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"spectrum_parameter", parameter}};

  TestParameterProto actual;
  EXPECT_TRUE(
      TryRemoveSpectrumTexture(
          parameters, "spectrum_parameter",
          std::bind(&TestParameterProto::mutable_spectrum_parameter, &actual))
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                spectrum_parameter { spectrum_texture_name: "abc" }
              )pb"));
}

}  // namespace
}  // namespace pbrt_proto
