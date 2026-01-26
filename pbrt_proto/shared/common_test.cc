#include "pbrt_proto/shared/common.h"

#include <array>
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
              Optional(CheckerboardAntialiasing::NONE));
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
