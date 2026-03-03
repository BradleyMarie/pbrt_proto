#include "pbrt_proto/shared/light_sources.h"

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

TEST(RemoveDistantLightSourceV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  DistantLightSource actual;
  RemoveDistantLightSourceV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveDistantLightSourceV1, WithData) {
  std::vector<std::array<double, 2>> l = {{1.0, 2.0}};
  Parameter l_parameter{.directive = "",
                        .type = ParameterType::BLACKBODY_V1,
                        .type_name = "",
                        .values = absl::MakeSpan(l)};

  std::vector<std::array<double, 3>> from = {{3.0, 4.0, 5.0}};
  Parameter from_parameter{.directive = "",
                           .type = ParameterType::POINT3,
                           .type_name = "",
                           .values = absl::MakeSpan(from)};

  std::vector<std::array<double, 3>> to = {{6.0, 7.0, 8.0}};
  Parameter to_parameter{.directive = "",
                         .type = ParameterType::POINT3,
                         .type_name = "",
                         .values = absl::MakeSpan(to)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"L", l_parameter},
      {"from", from_parameter},
      {"to", to_parameter},
  };

  DistantLightSource actual;
  RemoveDistantLightSourceV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                L { blackbody_spectrum { temperature: 1.0 scale: 2.0 } }
                from { x: 3.0 y: 4.0 z: 5.0 }
                to { x: 6.0 y: 7.0 z: 8.0 }
              )pb"));
}

TEST(RemoveDistantLightSourceV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  DistantLightSource actual;
  RemoveDistantLightSourceV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveDistantLightSourceV2, WithData) {
  std::vector<std::array<double, 2>> l = {{1.0, 2.0}};
  Parameter l_parameter{.directive = "",
                        .type = ParameterType::BLACKBODY_V1,
                        .type_name = "",
                        .values = absl::MakeSpan(l)};

  std::vector<std::array<double, 3>> from = {{3.0, 4.0, 5.0}};
  Parameter from_parameter{.directive = "",
                           .type = ParameterType::POINT3,
                           .type_name = "",
                           .values = absl::MakeSpan(from)};

  std::vector<std::array<double, 3>> to = {{6.0, 7.0, 8.0}};
  Parameter to_parameter{.directive = "",
                         .type = ParameterType::POINT3,
                         .type_name = "",
                         .values = absl::MakeSpan(to)};

  std::vector<std::array<double, 2>> scale = {{9.0, 10.0}};
  Parameter scale_parameter{.directive = "",
                            .type = ParameterType::BLACKBODY_V1,
                            .type_name = "",
                            .values = absl::MakeSpan(scale)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"L", l_parameter},
      {"from", from_parameter},
      {"to", to_parameter},
      {"scale", scale_parameter},
  };

  DistantLightSource actual;
  RemoveDistantLightSourceV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                L { blackbody_spectrum { temperature: 1.0 scale: 2.0 } }
                from { x: 3.0 y: 4.0 z: 5.0 }
                to { x: 6.0 y: 7.0 z: 8.0 }
                scale { blackbody_spectrum { temperature: 9.0 scale: 10.0 } }
              )pb"));
}

TEST(RemoveDistantLightSourceV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  DistantLightSource actual;
  RemoveDistantLightSourceV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveDistantLightSourceV3, WithData) {
  std::vector<double> l = {1.0};
  Parameter l_parameter{.directive = "",
                        .type = ParameterType::BLACKBODY_V2,
                        .type_name = "",
                        .values = absl::MakeSpan(l)};

  std::vector<std::array<double, 3>> from = {{3.0, 4.0, 5.0}};
  Parameter from_parameter{.directive = "",
                           .type = ParameterType::POINT3,
                           .type_name = "",
                           .values = absl::MakeSpan(from)};

  std::vector<std::array<double, 3>> to = {{6.0, 7.0, 8.0}};
  Parameter to_parameter{.directive = "",
                         .type = ParameterType::POINT3,
                         .type_name = "",
                         .values = absl::MakeSpan(to)};

  std::vector<double> scale = {10.0};
  Parameter scale_parameter{.directive = "",
                            .type = ParameterType::FLOAT,
                            .type_name = "",
                            .values = absl::MakeSpan(scale)};

  std::vector<double> illuminance = {9.0};
  Parameter illuminance_parameter{.directive = "",
                                  .type = ParameterType::FLOAT,
                                  .type_name = "",
                                  .values = absl::MakeSpan(illuminance)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"L", l_parameter},         {"from", from_parameter},
      {"to", to_parameter},       {"illuminance", illuminance_parameter},
      {"scale", scale_parameter},
  };

  DistantLightSource actual;
  RemoveDistantLightSourceV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                L { blackbody_spectrum { temperature: 1.0 } }
                from { x: 3.0 y: 4.0 z: 5.0 }
                to { x: 6.0 y: 7.0 z: 8.0 }
                scale { constant_spectrum: 10.0 }
                illuminance: 9.0
              )pb"));
}

TEST(RemoveGoniometricLightSourceV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  GoniometricLightSource actual;
  RemoveGoniometricLightSourceV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveGoniometricLightSourceV1, WithData) {
  std::vector<std::array<double, 2>> i = {{1.0, 2.0}};
  Parameter i_parameter{.directive = "",
                        .type = ParameterType::BLACKBODY_V1,
                        .type_name = "",
                        .values = absl::MakeSpan(i)};

  std::vector<absl::string_view> mapname = {"file"};
  Parameter mapname_parameter{.directive = "",
                              .type = ParameterType::STRING,
                              .type_name = "",
                              .values = absl::MakeSpan(mapname)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"I", i_parameter},
      {"mapname", mapname_parameter},
  };

  GoniometricLightSource actual;
  RemoveGoniometricLightSourceV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                I { blackbody_spectrum { temperature: 1.0 scale: 2.0 } }
                filename: "file"
              )pb"));
}

TEST(RemoveGoniometricLightSourceV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  GoniometricLightSource actual;
  RemoveGoniometricLightSourceV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveGoniometricLightSourceV2, WithData) {
  std::vector<std::array<double, 2>> i = {{1.0, 2.0}};
  Parameter i_parameter{.directive = "",
                        .type = ParameterType::BLACKBODY_V1,
                        .type_name = "",
                        .values = absl::MakeSpan(i)};

  std::vector<absl::string_view> mapname = {"file"};
  Parameter mapname_parameter{.directive = "",
                              .type = ParameterType::STRING,
                              .type_name = "",
                              .values = absl::MakeSpan(mapname)};

  std::vector<std::array<double, 2>> scale = {{3.0, 4.0}};
  Parameter scale_parameter{.directive = "",
                            .type = ParameterType::BLACKBODY_V1,
                            .type_name = "",
                            .values = absl::MakeSpan(scale)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"I", i_parameter},
      {"mapname", mapname_parameter},
      {"scale", scale_parameter},
  };

  GoniometricLightSource actual;
  RemoveGoniometricLightSourceV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                I { blackbody_spectrum { temperature: 1.0 scale: 2.0 } }
                filename: "file"
                scale { blackbody_spectrum { temperature: 3.0 scale: 4.0 } }
              )pb"));
}

TEST(RemoveGoniometricLightSourceV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  GoniometricLightSource actual;
  RemoveGoniometricLightSourceV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveGoniometricLightSourceV3, WithData) {
  std::vector<double> i = {1.0};
  Parameter i_parameter{.directive = "",
                        .type = ParameterType::BLACKBODY_V2,
                        .type_name = "",
                        .values = absl::MakeSpan(i)};

  std::vector<absl::string_view> filename = {"file"};
  Parameter filename_parameter{.directive = "",
                               .type = ParameterType::STRING,
                               .type_name = "",
                               .values = absl::MakeSpan(filename)};

  std::vector<double> scale = {3.0};
  Parameter scale_parameter{.directive = "",
                            .type = ParameterType::FLOAT,
                            .type_name = "",
                            .values = absl::MakeSpan(scale)};

  std::vector<double> power = {5.0};
  Parameter power_parameter{.directive = "",
                            .type = ParameterType::FLOAT,
                            .type_name = "",
                            .values = absl::MakeSpan(power)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"I", i_parameter},
      {"filename", filename_parameter},
      {"scale", scale_parameter},
      {"power", power_parameter},
  };

  GoniometricLightSource actual;
  RemoveGoniometricLightSourceV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                I { blackbody_spectrum { temperature: 1.0 } }
                filename: "file"
                scale { constant_spectrum: 3.0 }
                power: 5.0
              )pb"));
}

TEST(RemoveInfiniteLightSourceV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  InfiniteLightSource actual;
  RemoveInfiniteLightSourceV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveInfiniteLightSourceV1, WithData) {
  std::vector<std::array<double, 2>> l = {{1.0, 2.0}};
  Parameter l_parameter{.directive = "",
                        .type = ParameterType::BLACKBODY_V1,
                        .type_name = "",
                        .values = absl::MakeSpan(l)};

  std::vector<absl::string_view> mapname = {"file"};
  Parameter mapname_parameter{.directive = "",
                              .type = ParameterType::STRING,
                              .type_name = "",
                              .values = absl::MakeSpan(mapname)};

  std::vector<int32_t> nsamples = {3};
  Parameter nsamples_parameter{.directive = "",
                               .type = ParameterType::INTEGER,
                               .type_name = "",
                               .values = absl::MakeSpan(nsamples)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"L", l_parameter},
      {"mapname", mapname_parameter},
      {"nsamples", nsamples_parameter},
  };

  InfiniteLightSource actual;
  RemoveInfiniteLightSourceV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                L { blackbody_spectrum { temperature: 1.0 scale: 2.0 } }
                filename: "file"
                samples: 3
              )pb"));
}

TEST(RemoveInfiniteLightSourceV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  InfiniteLightSource actual;
  RemoveInfiniteLightSourceV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveInfiniteLightSourceV2, WithData) {
  std::vector<std::array<double, 2>> l = {{1.0, 2.0}};
  Parameter l_parameter{.directive = "",
                        .type = ParameterType::BLACKBODY_V1,
                        .type_name = "",
                        .values = absl::MakeSpan(l)};

  std::vector<absl::string_view> mapname = {"file"};
  Parameter mapname_parameter{.directive = "",
                              .type = ParameterType::STRING,
                              .type_name = "",
                              .values = absl::MakeSpan(mapname)};

  std::vector<int32_t> nsamples = {3};
  Parameter nsamples_parameter{.directive = "",
                               .type = ParameterType::INTEGER,
                               .type_name = "",
                               .values = absl::MakeSpan(nsamples)};

  std::vector<std::array<double, 2>> scale = {{4.0, 5.0}};
  Parameter scale_parameter{.directive = "",
                            .type = ParameterType::BLACKBODY_V1,
                            .type_name = "",
                            .values = absl::MakeSpan(scale)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"L", l_parameter},
      {"mapname", mapname_parameter},
      {"nsamples", nsamples_parameter},
      {"scale", scale_parameter},
  };

  InfiniteLightSource actual;
  RemoveInfiniteLightSourceV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                L { blackbody_spectrum { temperature: 1.0 scale: 2.0 } }
                filename: "file"
                samples: 3
                scale { blackbody_spectrum { temperature: 4.0 scale: 5.0 } }
              )pb"));
}

TEST(RemoveInfiniteLightSourceV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  InfiniteLightSource actual;
  RemoveInfiniteLightSourceV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveInfiniteLightSourceV3, NSamples) {
  std::vector<int32_t> nsamples = {3};
  Parameter nsamples_parameter{.directive = "",
                               .type = ParameterType::INTEGER,
                               .type_name = "",
                               .values = absl::MakeSpan(nsamples)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"nsamples", nsamples_parameter},
  };

  InfiniteLightSource actual;
  RemoveInfiniteLightSourceV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                samples: 3
              )pb"));
}

TEST(RemoveInfiniteLightSourceV3, WithData) {
  std::vector<std::array<double, 2>> l = {{1.0, 2.0}};
  Parameter l_parameter{.directive = "",
                        .type = ParameterType::BLACKBODY_V1,
                        .type_name = "",
                        .values = absl::MakeSpan(l)};

  std::vector<absl::string_view> mapname = {"file"};
  Parameter mapname_parameter{.directive = "",
                              .type = ParameterType::STRING,
                              .type_name = "",
                              .values = absl::MakeSpan(mapname)};

  std::vector<int32_t> samples = {3};
  Parameter samples_parameter{.directive = "",
                              .type = ParameterType::INTEGER,
                              .type_name = "",
                              .values = absl::MakeSpan(samples)};

  std::vector<std::array<double, 2>> scale = {{4.0, 5.0}};
  Parameter scale_parameter{.directive = "",
                            .type = ParameterType::BLACKBODY_V1,
                            .type_name = "",
                            .values = absl::MakeSpan(scale)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"L", l_parameter},
      {"mapname", mapname_parameter},
      {"samples", samples_parameter},
      {"scale", scale_parameter},
  };

  InfiniteLightSource actual;
  RemoveInfiniteLightSourceV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                L { blackbody_spectrum { temperature: 1.0 scale: 2.0 } }
                filename: "file"
                samples: 3
                scale { blackbody_spectrum { temperature: 4.0 scale: 5.0 } }
              )pb"));
}

TEST(RemoveInfiniteLightSourceV4, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  InfiniteLightSource actual;
  RemoveInfiniteLightSourceV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveInfiniteLightSourceV4, WithData) {
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

  std::vector<double> scale = {4.0};
  Parameter scale_parameter{.directive = "",
                            .type = ParameterType::FLOAT,
                            .type_name = "",
                            .values = absl::MakeSpan(scale)};

  std::vector<double> illuminance = {6.0};
  Parameter illuminance_parameter{.directive = "",
                                  .type = ParameterType::FLOAT,
                                  .type_name = "",
                                  .values = absl::MakeSpan(illuminance)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"L", l_parameter},
      {"filename", filename_parameter},
      {"scale", scale_parameter},
      {"illuminance", illuminance_parameter},
  };

  InfiniteLightSource actual;
  RemoveInfiniteLightSourceV4(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                L { blackbody_spectrum { temperature: 1.0 } }
                filename: "file"
                scale { constant_spectrum: 4.0 }
                illuminance: 6.0
              )pb"));
}

TEST(RemovePointLightSourceV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  PointLightSource actual;
  RemovePointLightSourceV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemovePointLightSourceV1, WithData) {
  std::vector<std::array<double, 2>> i = {{1.0, 2.0}};
  Parameter i_parameter{.directive = "",
                        .type = ParameterType::BLACKBODY_V1,
                        .type_name = "",
                        .values = absl::MakeSpan(i)};

  std::vector<std::array<double, 3>> from = {{3.0, 4.0, 5.0}};
  Parameter from_parameter{.directive = "",
                           .type = ParameterType::POINT3,
                           .type_name = "",
                           .values = absl::MakeSpan(from)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"I", i_parameter},
      {"from", from_parameter},
  };

  PointLightSource actual;
  RemovePointLightSourceV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                I { blackbody_spectrum { temperature: 1.0 scale: 2.0 } }
                from { x: 3.0 y: 4.0 z: 5.0 }
              )pb"));
}

TEST(RemovePointLightSourceV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  PointLightSource actual;
  RemovePointLightSourceV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemovePointLightSourceV2, WithData) {
  std::vector<std::array<double, 2>> i = {{1.0, 2.0}};
  Parameter i_parameter{.directive = "",
                        .type = ParameterType::BLACKBODY_V1,
                        .type_name = "",
                        .values = absl::MakeSpan(i)};

  std::vector<std::array<double, 3>> from = {{3.0, 4.0, 5.0}};
  Parameter from_parameter{.directive = "",
                           .type = ParameterType::POINT3,
                           .type_name = "",
                           .values = absl::MakeSpan(from)};

  std::vector<std::array<double, 2>> scale = {{6.0, 7.0}};
  Parameter scale_parameter{.directive = "",
                            .type = ParameterType::BLACKBODY_V1,
                            .type_name = "",
                            .values = absl::MakeSpan(scale)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"I", i_parameter},
      {"from", from_parameter},
      {"scale", scale_parameter},
  };

  PointLightSource actual;
  RemovePointLightSourceV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                I { blackbody_spectrum { temperature: 1.0 scale: 2.0 } }
                from { x: 3.0 y: 4.0 z: 5.0 }
                scale { blackbody_spectrum { temperature: 6.0 scale: 7.0 } }
              )pb"));
}

TEST(RemovePointLightSourceV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  PointLightSource actual;
  RemovePointLightSourceV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemovePointLightSourceV3, WithData) {
  std::vector<double> i = {1.0};
  Parameter i_parameter{.directive = "",
                        .type = ParameterType::BLACKBODY_V2,
                        .type_name = "",
                        .values = absl::MakeSpan(i)};

  std::vector<std::array<double, 3>> from = {{3.0, 4.0, 5.0}};
  Parameter from_parameter{.directive = "",
                           .type = ParameterType::POINT3,
                           .type_name = "",
                           .values = absl::MakeSpan(from)};

  std::vector<double> scale = {6.0};
  Parameter scale_parameter{.directive = "",
                            .type = ParameterType::FLOAT,
                            .type_name = "",
                            .values = absl::MakeSpan(scale)};

  std::vector<double> power = {8.0};
  Parameter power_parameter{.directive = "",
                            .type = ParameterType::FLOAT,
                            .type_name = "",
                            .values = absl::MakeSpan(power)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"I", i_parameter},
      {"from", from_parameter},
      {"scale", scale_parameter},
      {"power", power_parameter},
  };

  PointLightSource actual;
  RemovePointLightSourceV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                I { blackbody_spectrum { temperature: 1.0 } }
                from { x: 3.0 y: 4.0 z: 5.0 }
                scale { constant_spectrum: 6.0 }
                power: 8.0
              )pb"));
}

TEST(RemoveProjectionLightSourceV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  ProjectionLightSource actual;
  RemoveProjectionLightSourceV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveProjectionLightSourceV1, WithData) {
  std::vector<std::array<double, 2>> i = {{1.0, 2.0}};
  Parameter i_parameter{.directive = "",
                        .type = ParameterType::BLACKBODY_V1,
                        .type_name = "",
                        .values = absl::MakeSpan(i)};

  std::vector<absl::string_view> mapname = {"file"};
  Parameter mapname_parameter{.directive = "",
                              .type = ParameterType::STRING,
                              .type_name = "",
                              .values = absl::MakeSpan(mapname)};

  std::vector<double> fov = {10.0};
  Parameter fov_parameter{.directive = "",
                          .type = ParameterType::FLOAT,
                          .type_name = "",
                          .values = absl::MakeSpan(fov)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"I", i_parameter},
      {"mapname", mapname_parameter},
      {"fov", fov_parameter},
  };

  ProjectionLightSource actual;
  RemoveProjectionLightSourceV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                I { blackbody_spectrum { temperature: 1.0 scale: 2.0 } }
                filename: "file"
                fov: 20.0
              )pb"));
}

TEST(RemoveProjectionLightSourceV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  ProjectionLightSource actual;
  RemoveProjectionLightSourceV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveProjectionLightSourceV2, WithData) {
  std::vector<std::array<double, 2>> i = {{1.0, 2.0}};
  Parameter i_parameter{.directive = "",
                        .type = ParameterType::BLACKBODY_V1,
                        .type_name = "",
                        .values = absl::MakeSpan(i)};

  std::vector<absl::string_view> mapname = {"file"};
  Parameter mapname_parameter{.directive = "",
                              .type = ParameterType::STRING,
                              .type_name = "",
                              .values = absl::MakeSpan(mapname)};

  std::vector<double> fov = {10.0};
  Parameter fov_parameter{.directive = "",
                          .type = ParameterType::FLOAT,
                          .type_name = "",
                          .values = absl::MakeSpan(fov)};

  std::vector<std::array<double, 2>> scale = {{3.0, 4.0}};
  Parameter scale_parameter{.directive = "",
                            .type = ParameterType::BLACKBODY_V1,
                            .type_name = "",
                            .values = absl::MakeSpan(scale)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"I", i_parameter},
      {"mapname", mapname_parameter},
      {"fov", fov_parameter},
      {"scale", scale_parameter},
  };

  ProjectionLightSource actual;
  RemoveProjectionLightSourceV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                I { blackbody_spectrum { temperature: 1.0 scale: 2.0 } }
                filename: "file"
                fov: 20.0
                scale { blackbody_spectrum { temperature: 3.0 scale: 4.0 } }
              )pb"));
}

TEST(RemoveProjectionLightSourceV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  ProjectionLightSource actual;
  RemoveProjectionLightSourceV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveProjectionLightSourceV3, WithData) {
  std::vector<double> i = {1.0};
  Parameter i_parameter{.directive = "",
                        .type = ParameterType::BLACKBODY_V2,
                        .type_name = "",
                        .values = absl::MakeSpan(i)};

  std::vector<absl::string_view> filename = {"file"};
  Parameter filename_parameter{.directive = "",
                               .type = ParameterType::STRING,
                               .type_name = "",
                               .values = absl::MakeSpan(filename)};

  std::vector<double> fov = {10.0};
  Parameter fov_parameter{.directive = "",
                          .type = ParameterType::FLOAT,
                          .type_name = "",
                          .values = absl::MakeSpan(fov)};

  std::vector<double> scale = {3.0};
  Parameter scale_parameter{.directive = "",
                            .type = ParameterType::FLOAT,
                            .type_name = "",
                            .values = absl::MakeSpan(scale)};

  std::vector<double> power = {5.0};
  Parameter power_parameter{.directive = "",
                            .type = ParameterType::FLOAT,
                            .type_name = "",
                            .values = absl::MakeSpan(power)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"I", i_parameter},         {"filename", filename_parameter},
      {"fov", fov_parameter},     {"scale", scale_parameter},
      {"power", power_parameter},
  };

  ProjectionLightSource actual;
  RemoveProjectionLightSourceV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                I { blackbody_spectrum { temperature: 1.0 } }
                filename: "file"
                fov: 10.0
                scale { constant_spectrum: 3.0 }
                power: 5.0
              )pb"));
}

TEST(RemoveSpotLightSourceV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  SpotLightSource actual;
  RemoveSpotLightSourceV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveSpotLightSourceV1, WithData) {
  std::vector<std::array<double, 2>> i = {{1.0, 2.0}};
  Parameter i_parameter{.directive = "",
                        .type = ParameterType::BLACKBODY_V1,
                        .type_name = "",
                        .values = absl::MakeSpan(i)};

  std::vector<std::array<double, 3>> from = {{3.0, 4.0, 5.0}};
  Parameter from_parameter{.directive = "",
                           .type = ParameterType::POINT3,
                           .type_name = "",
                           .values = absl::MakeSpan(from)};

  std::vector<std::array<double, 3>> to = {{6.0, 7.0, 8.0}};
  Parameter to_parameter{.directive = "",
                         .type = ParameterType::POINT3,
                         .type_name = "",
                         .values = absl::MakeSpan(to)};

  std::vector<double> coneangle = {9.0};
  Parameter coneangle_parameter{.directive = "",
                                .type = ParameterType::FLOAT,
                                .type_name = "",
                                .values = absl::MakeSpan(coneangle)};

  std::vector<double> conedeltaangle = {10.0};
  Parameter conedeltaangle_parameter{.directive = "",
                                     .type = ParameterType::FLOAT,
                                     .type_name = "",
                                     .values = absl::MakeSpan(conedeltaangle)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"I", i_parameter},
      {"from", from_parameter},
      {"to", to_parameter},
      {"coneangle", coneangle_parameter},
      {"conedeltaangle", conedeltaangle_parameter},
  };

  SpotLightSource actual;
  RemoveSpotLightSourceV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                I { blackbody_spectrum { temperature: 1.0 scale: 2.0 } }
                from { x: 3.0 y: 4.0 z: 5.0 }
                to { x: 6.0 y: 7.0 z: 8.0 }
                coneangle: 9.0
                conedeltaangle: 10.0
              )pb"));
}

TEST(RemoveSpotLightSourceV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  SpotLightSource actual;
  RemoveSpotLightSourceV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveSpotLightSourceV2, WithData) {
  std::vector<std::array<double, 2>> i = {{1.0, 2.0}};
  Parameter i_parameter{.directive = "",
                        .type = ParameterType::BLACKBODY_V1,
                        .type_name = "",
                        .values = absl::MakeSpan(i)};

  std::vector<std::array<double, 3>> from = {{3.0, 4.0, 5.0}};
  Parameter from_parameter{.directive = "",
                           .type = ParameterType::POINT3,
                           .type_name = "",
                           .values = absl::MakeSpan(from)};

  std::vector<std::array<double, 3>> to = {{6.0, 7.0, 8.0}};
  Parameter to_parameter{.directive = "",
                         .type = ParameterType::POINT3,
                         .type_name = "",
                         .values = absl::MakeSpan(to)};

  std::vector<double> coneangle = {9.0};
  Parameter coneangle_parameter{.directive = "",
                                .type = ParameterType::FLOAT,
                                .type_name = "",
                                .values = absl::MakeSpan(coneangle)};

  std::vector<double> conedeltaangle = {10.0};
  Parameter conedeltaangle_parameter{.directive = "",
                                     .type = ParameterType::FLOAT,
                                     .type_name = "",
                                     .values = absl::MakeSpan(conedeltaangle)};

  std::vector<std::array<double, 2>> scale = {{11.0, 12.0}};
  Parameter scale_parameter{.directive = "",
                            .type = ParameterType::BLACKBODY_V1,
                            .type_name = "",
                            .values = absl::MakeSpan(scale)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"I", i_parameter},
      {"from", from_parameter},
      {"to", to_parameter},
      {"coneangle", coneangle_parameter},
      {"conedeltaangle", conedeltaangle_parameter},
      {"scale", scale_parameter},
  };

  SpotLightSource actual;
  RemoveSpotLightSourceV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                I { blackbody_spectrum { temperature: 1.0 scale: 2.0 } }
                from { x: 3.0 y: 4.0 z: 5.0 }
                to { x: 6.0 y: 7.0 z: 8.0 }
                coneangle: 9.0
                conedeltaangle: 10.0
                scale { blackbody_spectrum { temperature: 11.0 scale: 12.0 } }
              )pb"));
}

TEST(RemoveSpotLightSourceV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  SpotLightSource actual;
  RemoveSpotLightSourceV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveSpotLightSourceV3, WithData) {
  std::vector<double> i = {1.0};
  Parameter i_parameter{.directive = "",
                        .type = ParameterType::BLACKBODY_V2,
                        .type_name = "",
                        .values = absl::MakeSpan(i)};

  std::vector<std::array<double, 3>> from = {{3.0, 4.0, 5.0}};
  Parameter from_parameter{.directive = "",
                           .type = ParameterType::POINT3,
                           .type_name = "",
                           .values = absl::MakeSpan(from)};

  std::vector<std::array<double, 3>> to = {{6.0, 7.0, 8.0}};
  Parameter to_parameter{.directive = "",
                         .type = ParameterType::POINT3,
                         .type_name = "",
                         .values = absl::MakeSpan(to)};

  std::vector<double> coneangle = {9.0};
  Parameter coneangle_parameter{.directive = "",
                                .type = ParameterType::FLOAT,
                                .type_name = "",
                                .values = absl::MakeSpan(coneangle)};

  std::vector<double> conedeltaangle = {10.0};
  Parameter conedeltaangle_parameter{.directive = "",
                                     .type = ParameterType::FLOAT,
                                     .type_name = "",
                                     .values = absl::MakeSpan(conedeltaangle)};

  std::vector<double> scale = {11.0};
  Parameter scale_parameter{.directive = "",
                            .type = ParameterType::FLOAT,
                            .type_name = "",
                            .values = absl::MakeSpan(scale)};

  std::vector<double> power = {12.0};
  Parameter power_parameter{.directive = "",
                            .type = ParameterType::FLOAT,
                            .type_name = "",
                            .values = absl::MakeSpan(power)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"I", i_parameter},
      {"from", from_parameter},
      {"to", to_parameter},
      {"coneangle", coneangle_parameter},
      {"conedeltaangle", conedeltaangle_parameter},
      {"scale", scale_parameter},
      {"power", power_parameter},
  };

  SpotLightSource actual;
  RemoveSpotLightSourceV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                I { blackbody_spectrum { temperature: 1.0 } }
                from { x: 3.0 y: 4.0 z: 5.0 }
                to { x: 6.0 y: 7.0 z: 8.0 }
                coneangle: 9.0
                conedeltaangle: 10.0
                scale { constant_spectrum: 11.0 }
                power: 12.0
              )pb"));
}

}  // namespace
}  // namespace pbrt_proto
