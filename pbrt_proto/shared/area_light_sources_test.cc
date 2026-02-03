#include "pbrt_proto/shared/area_light_sources.h"

#include <array>
#include <cstdint>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "absl/status/status_matchers.h"
#include "absl/strings/string_view.h"
#include "googlemock/include/gmock/gmock.h"
#include "googletest/include/gtest/gtest.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"
#include "pbrt_proto/testing/proto_matchers.h"

namespace pbrt_proto {
namespace {

using ::absl_testing::StatusIs;
using ::google::protobuf::EqualsProto;

TEST(RemoveDiffuseAreaLightSourceV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  DiffuseAreaLightSourceV1 actual;
  RemoveDiffuseAreaLightSourceV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveDiffuseAreaLightSourceV1, WithValues) {
  std::vector<std::array<double, 2>> l = {{1.0, 2.0}};
  Parameter l_parameter{.directive = "",
                        .type = ParameterType::BLACKBODY_V1,
                        .type_name = "",
                        .values = absl::MakeSpan(l)};

  std::vector<int32_t> nsamples = {3};
  Parameter nsamples_parameter{.directive = "",
                               .type = ParameterType::INTEGER,
                               .type_name = "",
                               .values = absl::MakeSpan(nsamples)};

  std::vector<int32_t> samples = {4};
  Parameter samples_parameter{.directive = "",
                              .type = ParameterType::INTEGER,
                              .type_name = "",
                              .values = absl::MakeSpan(samples)};

  std::vector<double> scale = {5.0};
  Parameter scale_parameter{.directive = "",
                            .type = ParameterType::FLOAT,
                            .type_name = "",
                            .values = absl::MakeSpan(scale)};

  bool twosided[] = {true};
  Parameter twosided_parameter{.directive = "",
                               .type = ParameterType::BOOL,
                               .type_name = "",
                               .values = absl::MakeSpan(twosided)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"L", l_parameter},
      {"nsamples", nsamples_parameter},
      {"samples", samples_parameter},
      {"scale", scale_parameter},
      {"twosided", twosided_parameter}};

  DiffuseAreaLightSourceV1 actual;
  RemoveDiffuseAreaLightSourceV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                L { blackbody_spectrum { temperature: 1.0 scale: 2.0 } }
                samples: 3
              )pb"));
}

TEST(RemoveDiffuseAreaLightSourceV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  DiffuseAreaLightSourceV1 actual;
  RemoveDiffuseAreaLightSourceV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveDiffuseAreaLightSourceV2, WithValues) {
  std::vector<std::array<double, 2>> l = {{1.0, 2.0}};
  Parameter l_parameter{.directive = "",
                        .type = ParameterType::BLACKBODY_V1,
                        .type_name = "",
                        .values = absl::MakeSpan(l)};

  std::vector<int32_t> nsamples = {3};
  Parameter nsamples_parameter{.directive = "",
                               .type = ParameterType::INTEGER,
                               .type_name = "",
                               .values = absl::MakeSpan(nsamples)};

  std::vector<int32_t> samples = {4};
  Parameter samples_parameter{.directive = "",
                              .type = ParameterType::INTEGER,
                              .type_name = "",
                              .values = absl::MakeSpan(samples)};

  std::vector<std::array<double, 2>> scale = {{5.0, 6.0}};
  Parameter scale_parameter{.directive = "",
                            .type = ParameterType::BLACKBODY_V1,
                            .type_name = "",
                            .values = absl::MakeSpan(scale)};

  bool twosided[] = {true};
  Parameter twosided_parameter{.directive = "",
                               .type = ParameterType::BOOL,
                               .type_name = "",
                               .values = absl::MakeSpan(twosided)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"L", l_parameter},
      {"nsamples", nsamples_parameter},
      {"samples", samples_parameter},
      {"scale", scale_parameter},
      {"twosided", twosided_parameter}};

  DiffuseAreaLightSourceV1 actual;
  RemoveDiffuseAreaLightSourceV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                L { blackbody_spectrum { temperature: 1.0 scale: 2.0 } }
                samples: 3
                scale { blackbody_spectrum { temperature: 5.0 scale: 6.0 } }
              )pb"));
}

TEST(RemoveDiffuseAreaLightSourceV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  DiffuseAreaLightSourceV1 actual;
  RemoveDiffuseAreaLightSourceV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveDiffuseAreaLightSourceV3, WithValues) {
  std::vector<std::array<double, 2>> l = {{1.0, 2.0}};
  Parameter l_parameter{.directive = "",
                        .type = ParameterType::BLACKBODY_V1,
                        .type_name = "",
                        .values = absl::MakeSpan(l)};

  std::vector<int32_t> nsamples = {3};
  Parameter nsamples_parameter{.directive = "",
                               .type = ParameterType::INTEGER,
                               .type_name = "",
                               .values = absl::MakeSpan(nsamples)};

  std::vector<int32_t> samples = {4};
  Parameter samples_parameter{.directive = "",
                              .type = ParameterType::INTEGER,
                              .type_name = "",
                              .values = absl::MakeSpan(samples)};

  std::vector<std::array<double, 2>> scale = {{5.0, 6.0}};
  Parameter scale_parameter{.directive = "",
                            .type = ParameterType::BLACKBODY_V1,
                            .type_name = "",
                            .values = absl::MakeSpan(scale)};

  bool twosided[] = {true};
  Parameter twosided_parameter{.directive = "",
                               .type = ParameterType::BOOL,
                               .type_name = "",
                               .values = absl::MakeSpan(twosided)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"L", l_parameter},
      {"nsamples", nsamples_parameter},
      {"samples", samples_parameter},
      {"scale", scale_parameter},
      {"twosided", twosided_parameter}};

  DiffuseAreaLightSourceV1 actual;
  RemoveDiffuseAreaLightSourceV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                L { blackbody_spectrum { temperature: 1.0 scale: 2.0 } }
                samples: 4
                scale { blackbody_spectrum { temperature: 5.0 scale: 6.0 } }
                twosided: true
              )pb"));
}

TEST(RemoveDiffuseAreaLightSourceV4, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  DiffuseAreaLightSourceV2 actual;
  ASSERT_TRUE(RemoveDiffuseAreaLightSourceV4(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveDiffuseAreaLightSourceV4, WithL) {
  std::vector<double> l = {1.0};
  Parameter l_parameter{.directive = "",
                        .type = ParameterType::BLACKBODY_V2,
                        .type_name = "",
                        .values = absl::MakeSpan(l)};

  std::vector<double> scale = {2.0};
  Parameter scale_parameter{.directive = "",
                            .type = ParameterType::FLOAT,
                            .type_name = "",
                            .values = absl::MakeSpan(scale)};

  std::vector<double> power = {3.0};
  Parameter power_parameter{.directive = "",
                            .type = ParameterType::FLOAT,
                            .type_name = "",
                            .values = absl::MakeSpan(power)};

  bool twosided[] = {true};
  Parameter twosided_parameter{.directive = "",
                               .type = ParameterType::BOOL,
                               .type_name = "",
                               .values = absl::MakeSpan(twosided)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"L", l_parameter},
      {"scale", scale_parameter},
      {"power", power_parameter},
      {"twosided", twosided_parameter}};

  DiffuseAreaLightSourceV2 actual;
  ASSERT_TRUE(RemoveDiffuseAreaLightSourceV4(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                L { blackbody_spectrum { temperature: 1.0 } }
                scale: 2.0
                power: 3.0
                twosided: true
              )pb"));
}

TEST(RemoveDiffuseAreaLightSourceV4, WithFilename) {
  std::vector<absl::string_view> filename = {"file"};
  Parameter filename_parameter{.directive = "",
                               .type = ParameterType::STRING,
                               .type_name = "",
                               .values = absl::MakeSpan(filename)};

  std::vector<double> scale = {2.0};
  Parameter scale_parameter{.directive = "",
                            .type = ParameterType::FLOAT,
                            .type_name = "",
                            .values = absl::MakeSpan(scale)};

  std::vector<double> power = {3.0};
  Parameter power_parameter{.directive = "",
                            .type = ParameterType::FLOAT,
                            .type_name = "",
                            .values = absl::MakeSpan(power)};

  bool twosided[] = {true};
  Parameter twosided_parameter{.directive = "",
                               .type = ParameterType::BOOL,
                               .type_name = "",
                               .values = absl::MakeSpan(twosided)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"filename", filename_parameter},
      {"scale", scale_parameter},
      {"power", power_parameter},
      {"twosided", twosided_parameter}};

  DiffuseAreaLightSourceV2 actual;
  ASSERT_TRUE(RemoveDiffuseAreaLightSourceV4(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                filename: "file"
                scale: 2.0
                power: 3.0
                twosided: true
              )pb"));
}

TEST(RemoveDiffuseAreaLightSourceV4, LAndFilename) {
  std::vector<double> l = {1.0};
  Parameter l_parameter{.directive = "",
                        .type = ParameterType::BLACKBODY_V2,
                        .type_name = "",
                        .values = absl::MakeSpan(l)};

  std::vector<absl::string_view> filename = {"file"};
  Parameter filename_parameter{.directive = "",
                               .type = ParameterType::STRING,
                               .type_name = "",
                               .values = absl::MakeSpan(filename)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"L", l_parameter}, {"filename", filename_parameter}};

  DiffuseAreaLightSourceV2 actual;
  EXPECT_THAT(
      RemoveDiffuseAreaLightSourceV4(parameters, actual),
      StatusIs(
          absl::StatusCode::kInvalidArgument,
          "A diffuse AreaLightSource may not specify both 'L' and 'filename'"));
}

}  // namespace
}  // namespace pbrt_proto
