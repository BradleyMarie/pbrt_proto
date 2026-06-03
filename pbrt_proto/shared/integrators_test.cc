#include "pbrt_proto/shared/integrators.h"

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

TEST(RemoveAmbientOcclusionIntegratorV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  AmbientOcclusionIntegrator actual;
  RemoveAmbientOcclusionIntegratorV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveAmbientOcclusionIntegratorV2, WithData) {
  std::vector<double> maxdist = {0.1};
  Parameter maxdist_parameter{.directive = "",
                              .type = ParameterType::FLOAT,
                              .type_name = "",
                              .values = absl::MakeSpan(maxdist)};

  std::vector<int32_t> nsamples = {2};
  Parameter nsamples_parameter{.directive = "",
                               .type = ParameterType::INTEGER,
                               .type_name = "",
                               .values = absl::MakeSpan(nsamples)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"maxdist", maxdist_parameter},
      {"nsamples", nsamples_parameter},
  };

  AmbientOcclusionIntegrator actual;
  RemoveAmbientOcclusionIntegratorV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                maxdistance: 0.1 nsamples: 2
              )pb"));
}

TEST(RemoveAmbientOcclusionIntegratorV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  AmbientOcclusionIntegrator actual;
  RemoveAmbientOcclusionIntegratorV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveAmbientOcclusionIntegratorV3, WithData) {
  std::vector<int32_t> nsamples = {1};
  Parameter nsamples_parameter{.directive = "",
                               .type = ParameterType::INTEGER,
                               .type_name = "",
                               .values = absl::MakeSpan(nsamples)};

  bool cossample[] = {true};
  Parameter cossample_parameter{.directive = "",
                                .type = ParameterType::BOOL,
                                .type_name = "",
                                .values = absl::MakeSpan(cossample)};

  std::vector<int32_t> pixelbounds = {2, 3, 4, 5};
  Parameter pixelbounds_parameter{.directive = "",
                                  .type = ParameterType::INTEGER,
                                  .type_name = "",
                                  .values = absl::MakeSpan(pixelbounds)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"nsamples", nsamples_parameter},
      {"cossample", cossample_parameter},
      {"pixelbounds", pixelbounds_parameter},
  };

  AmbientOcclusionIntegrator actual;
  RemoveAmbientOcclusionIntegratorV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                nsamples: 1
                cossample: true
                pixelbounds { x_min: 2 x_max: 3 y_min: 4 y_max: 5 }
              )pb"));
}

TEST(RemoveBdptIntegratorV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  BdptIntegrator actual;
  RemoveBdptIntegratorV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveBdptIntegratorV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  BdptIntegrator actual;
  RemoveBdptIntegratorV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveBdptIntegratorV3, WithData) {
  std::vector<int32_t> maxdepth = {1};
  Parameter maxdepth_parameter{.directive = "",
                               .type = ParameterType::INTEGER,
                               .type_name = "",
                               .values = absl::MakeSpan(maxdepth)};

  std::vector<int32_t> pixelbounds = {2, 3, 4, 5};
  Parameter pixelbounds_parameter{.directive = "",
                                  .type = ParameterType::INTEGER,
                                  .type_name = "",
                                  .values = absl::MakeSpan(pixelbounds)};

  std::vector<absl::string_view> lightsamplestrategy = {"unknown"};
  Parameter lightsamplestrategy_parameter{
      .directive = "",
      .type = ParameterType::STRING,
      .type_name = "",
      .values = absl::MakeSpan(lightsamplestrategy)};

  bool visualizestrategies[] = {true};
  Parameter visualizestrategies_parameter{
      .directive = "",
      .type = ParameterType::BOOL,
      .type_name = "",
      .values = absl::MakeSpan(visualizestrategies)};

  bool visualizeweights[] = {true};
  Parameter visualizeweights_parameter{
      .directive = "",
      .type = ParameterType::BOOL,
      .type_name = "",
      .values = absl::MakeSpan(visualizeweights)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"maxdepth", maxdepth_parameter},
      {"pixelbounds", pixelbounds_parameter},
      {"lightsamplestrategy", lightsamplestrategy_parameter},
      {"visualizestrategies", visualizestrategies_parameter},
      {"visualizeweights", visualizeweights_parameter},
  };

  BdptIntegrator actual;
  RemoveBdptIntegratorV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                maxdepth: 1
                pixelbounds { x_min: 2 x_max: 3 y_min: 4 y_max: 5 }
                lightsampler: BVH
                visualizestrategies: true
                visualizeweights: true
              )pb"));
}

TEST(RemoveBdptIntegratorV3, Uniform) {
  std::vector<absl::string_view> lightsamplestrategy = {"uniform"};
  Parameter lightsamplestrategy_parameter{
      .directive = "",
      .type = ParameterType::STRING,
      .type_name = "",
      .values = absl::MakeSpan(lightsamplestrategy)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"lightsamplestrategy", lightsamplestrategy_parameter},
  };

  BdptIntegrator actual;
  RemoveBdptIntegratorV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                lightsampler: UNIFORM
              )pb"));
}

TEST(RemoveBdptIntegratorV3, Power) {
  std::vector<absl::string_view> lightsamplestrategy = {"power"};
  Parameter lightsamplestrategy_parameter{
      .directive = "",
      .type = ParameterType::STRING,
      .type_name = "",
      .values = absl::MakeSpan(lightsamplestrategy)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"lightsamplestrategy", lightsamplestrategy_parameter},
  };

  BdptIntegrator actual;
  RemoveBdptIntegratorV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                lightsampler: POWER
              )pb"));
}

TEST(RemoveBdptIntegratorV3, Spatial) {
  std::vector<absl::string_view> lightsamplestrategy = {"spatial"};
  Parameter lightsamplestrategy_parameter{
      .directive = "",
      .type = ParameterType::STRING,
      .type_name = "",
      .values = absl::MakeSpan(lightsamplestrategy)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"lightsamplestrategy", lightsamplestrategy_parameter},
  };

  BdptIntegrator actual;
  RemoveBdptIntegratorV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                lightsampler: BVH
              )pb"));
}

TEST(RemoveDebugIntegratorV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  DebugIntegrator actual;
  RemoveDebugIntegratorV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveDebugIntegratorV1, FirstWithData) {
  std::vector<absl::string_view> red = {"zero"};
  Parameter red_parameter{.directive = "",
                          .type = ParameterType::STRING,
                          .type_name = "",
                          .values = absl::MakeSpan(red)};

  std::vector<absl::string_view> green = {"one"};
  Parameter green_parameter{.directive = "",
                            .type = ParameterType::STRING,
                            .type_name = "",
                            .values = absl::MakeSpan(green)};

  std::vector<absl::string_view> blue = {"hit"};
  Parameter blue_parameter{.directive = "",
                           .type = ParameterType::STRING,
                           .type_name = "",
                           .values = absl::MakeSpan(blue)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"red", red_parameter},
      {"green", green_parameter},
      {"blue", blue_parameter},
  };

  DebugIntegrator actual;
  RemoveDebugIntegratorV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                red: ZERO green: ONE blue: HIT
              )pb"));
}

TEST(RemoveDebugIntegratorV1, SecondWithData) {
  std::vector<absl::string_view> red = {"nx"};
  Parameter red_parameter{.directive = "",
                          .type = ParameterType::STRING,
                          .type_name = "",
                          .values = absl::MakeSpan(red)};

  std::vector<absl::string_view> green = {"ny"};
  Parameter green_parameter{.directive = "",
                            .type = ParameterType::STRING,
                            .type_name = "",
                            .values = absl::MakeSpan(green)};

  std::vector<absl::string_view> blue = {"nz"};
  Parameter blue_parameter{.directive = "",
                           .type = ParameterType::STRING,
                           .type_name = "",
                           .values = absl::MakeSpan(blue)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"red", red_parameter},
      {"green", green_parameter},
      {"blue", blue_parameter},
  };

  DebugIntegrator actual;
  RemoveDebugIntegratorV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                red: NX green: NY blue: NZ
              )pb"));
}

TEST(RemoveDebugIntegratorV1, ThirdWithData) {
  std::vector<absl::string_view> red = {"snx"};
  Parameter red_parameter{.directive = "",
                          .type = ParameterType::STRING,
                          .type_name = "",
                          .values = absl::MakeSpan(red)};

  std::vector<absl::string_view> green = {"sny"};
  Parameter green_parameter{.directive = "",
                            .type = ParameterType::STRING,
                            .type_name = "",
                            .values = absl::MakeSpan(green)};

  std::vector<absl::string_view> blue = {"snz"};
  Parameter blue_parameter{.directive = "",
                           .type = ParameterType::STRING,
                           .type_name = "",
                           .values = absl::MakeSpan(blue)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"red", red_parameter},
      {"green", green_parameter},
      {"blue", blue_parameter},
  };

  DebugIntegrator actual;
  RemoveDebugIntegratorV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                red: SNX green: SNY blue: SNZ
              )pb"));
}

TEST(RemoveDebugIntegratorV1, FourthWithData) {
  std::vector<absl::string_view> red = {"u"};
  Parameter red_parameter{.directive = "",
                          .type = ParameterType::STRING,
                          .type_name = "",
                          .values = absl::MakeSpan(red)};

  std::vector<absl::string_view> green = {"v"};
  Parameter green_parameter{.directive = "",
                            .type = ParameterType::STRING,
                            .type_name = "",
                            .values = absl::MakeSpan(green)};

  std::vector<absl::string_view> blue = {"x"};
  Parameter blue_parameter{.directive = "",
                           .type = ParameterType::STRING,
                           .type_name = "",
                           .values = absl::MakeSpan(blue)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"red", red_parameter},
      {"green", green_parameter},
      {"blue", blue_parameter},
  };

  DebugIntegrator actual;
  RemoveDebugIntegratorV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                red: U green: V blue: ZERO
              )pb"));
}

TEST(RemoveDiffusePrtIntegratorV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  DiffusePrtIntegrator actual;
  RemoveDiffusePrtIntegratorV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveDiffusePrtIntegratorV2, WithData) {
  std::vector<int32_t> lmax = {1};
  Parameter lmax_parameter{.directive = "",
                           .type = ParameterType::INTEGER,
                           .type_name = "",
                           .values = absl::MakeSpan(lmax)};

  std::vector<int32_t> nsamples = {2};
  Parameter nsamples_parameter{.directive = "",
                               .type = ParameterType::INTEGER,
                               .type_name = "",
                               .values = absl::MakeSpan(nsamples)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"lmax", lmax_parameter},
      {"nsamples", nsamples_parameter},
  };

  DiffusePrtIntegrator actual;
  RemoveDiffusePrtIntegratorV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                lmax: 1 nsamples: 2
              )pb"));
}

TEST(RemoveDipoleSubsurfaceIntegratorV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  DipoleSubsurfaceIntegrator actual;
  RemoveDipoleSubsurfaceIntegratorV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveDipoleSubsurfaceIntegratorV2, WithData) {
  std::vector<int32_t> maxdepth = {1};
  Parameter maxdepth_parameter{.directive = "",
                               .type = ParameterType::INTEGER,
                               .type_name = "",
                               .values = absl::MakeSpan(maxdepth)};

  std::vector<double> maxerror = {0.2};
  Parameter maxerror_parameter{.directive = "",
                               .type = ParameterType::FLOAT,
                               .type_name = "",
                               .values = absl::MakeSpan(maxerror)};

  std::vector<double> minsampledistance = {0.3};
  Parameter minsampledistance_parameter{
      .directive = "",
      .type = ParameterType::FLOAT,
      .type_name = "",
      .values = absl::MakeSpan(minsampledistance)};

  std::vector<absl::string_view> pointsfile = {"file"};
  Parameter pointsfile_parameter{.directive = "",
                                 .type = ParameterType::STRING,
                                 .type_name = "",
                                 .values = absl::MakeSpan(pointsfile)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"maxdepth", maxdepth_parameter},
      {"maxerror", maxerror_parameter},
      {"minsampledistance", minsampledistance_parameter},
      {"pointsfile", pointsfile_parameter},
  };

  DipoleSubsurfaceIntegrator actual;
  RemoveDipoleSubsurfaceIntegratorV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                maxdepth: 1
                maxerror: 0.2
                minsampledistance: 0.3
                pointsfile: "file"
              )pb"));
}

TEST(RemoveDirectLightingIntegratorV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  DirectLightingIntegrator actual;
  RemoveDirectLightingIntegratorV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveDirectLightingIntegratorV1, WithData) {
  std::vector<int32_t> maxdepth = {1};
  Parameter maxdepth_parameter{.directive = "",
                               .type = ParameterType::INTEGER,
                               .type_name = "",
                               .values = absl::MakeSpan(maxdepth)};

  std::vector<absl::string_view> strategy = {"all"};
  Parameter strategy_parameter{.directive = "",
                               .type = ParameterType::STRING,
                               .type_name = "",
                               .values = absl::MakeSpan(strategy)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"maxdepth", maxdepth_parameter}, {"strategy", strategy_parameter}};

  DirectLightingIntegrator actual;
  RemoveDirectLightingIntegratorV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                maxdepth: 1 strategy: ALL
              )pb"));
}

TEST(RemoveDirectLightingIntegratorV1, All) {
  std::vector<absl::string_view> strategy = {"all"};
  Parameter strategy_parameter{.directive = "",
                               .type = ParameterType::STRING,
                               .type_name = "",
                               .values = absl::MakeSpan(strategy)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"strategy", strategy_parameter}};

  DirectLightingIntegrator actual;
  RemoveDirectLightingIntegratorV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                strategy: ALL
              )pb"));
}

TEST(RemoveDirectLightingIntegratorV1, One) {
  std::vector<absl::string_view> strategy = {"one"};
  Parameter strategy_parameter{.directive = "",
                               .type = ParameterType::STRING,
                               .type_name = "",
                               .values = absl::MakeSpan(strategy)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"strategy", strategy_parameter}};

  DirectLightingIntegrator actual;
  RemoveDirectLightingIntegratorV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                strategy: ONE
              )pb"));
}

TEST(RemoveDirectLightingIntegratorV1, Weighted) {
  std::vector<absl::string_view> strategy = {"weighted"};
  Parameter strategy_parameter{.directive = "",
                               .type = ParameterType::STRING,
                               .type_name = "",
                               .values = absl::MakeSpan(strategy)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"strategy", strategy_parameter}};

  DirectLightingIntegrator actual;
  RemoveDirectLightingIntegratorV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                strategy: WEIGHTED
              )pb"));
}

TEST(RemoveDirectLightingIntegratorV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  DirectLightingIntegrator actual;
  RemoveDirectLightingIntegratorV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveDirectLightingIntegratorV2, WithData) {
  std::vector<int32_t> maxdepth = {1};
  Parameter maxdepth_parameter{.directive = "",
                               .type = ParameterType::INTEGER,
                               .type_name = "",
                               .values = absl::MakeSpan(maxdepth)};

  std::vector<absl::string_view> strategy = {"all"};
  Parameter strategy_parameter{.directive = "",
                               .type = ParameterType::STRING,
                               .type_name = "",
                               .values = absl::MakeSpan(strategy)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"maxdepth", maxdepth_parameter}, {"strategy", strategy_parameter}};

  DirectLightingIntegrator actual;
  RemoveDirectLightingIntegratorV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                maxdepth: 1 strategy: ALL
              )pb"));
}

TEST(RemoveDirectLightingIntegratorV2, All) {
  std::vector<absl::string_view> strategy = {"all"};
  Parameter strategy_parameter{.directive = "",
                               .type = ParameterType::STRING,
                               .type_name = "",
                               .values = absl::MakeSpan(strategy)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"strategy", strategy_parameter}};

  DirectLightingIntegrator actual;
  RemoveDirectLightingIntegratorV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                strategy: ALL
              )pb"));
}

TEST(RemoveDirectLightingIntegratorV2, One) {
  std::vector<absl::string_view> strategy = {"one"};
  Parameter strategy_parameter{.directive = "",
                               .type = ParameterType::STRING,
                               .type_name = "",
                               .values = absl::MakeSpan(strategy)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"strategy", strategy_parameter}};

  DirectLightingIntegrator actual;
  RemoveDirectLightingIntegratorV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                strategy: ONE
              )pb"));
}

TEST(RemoveDirectLightingIntegratorV2, Weighted) {
  std::vector<absl::string_view> strategy = {"weighted"};
  Parameter strategy_parameter{.directive = "",
                               .type = ParameterType::STRING,
                               .type_name = "",
                               .values = absl::MakeSpan(strategy)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"strategy", strategy_parameter}};

  DirectLightingIntegrator actual;
  RemoveDirectLightingIntegratorV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                strategy: ALL
              )pb"));
}

TEST(RemoveDirectLightingIntegratorV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  DirectLightingIntegrator actual;
  RemoveDirectLightingIntegratorV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveDirectLightingIntegratorV3, WithData) {
  std::vector<int32_t> maxdepth = {1};
  Parameter maxdepth_parameter{.directive = "",
                               .type = ParameterType::INTEGER,
                               .type_name = "",
                               .values = absl::MakeSpan(maxdepth)};

  std::vector<absl::string_view> strategy = {"all"};
  Parameter strategy_parameter{.directive = "",
                               .type = ParameterType::STRING,
                               .type_name = "",
                               .values = absl::MakeSpan(strategy)};

  std::vector<int32_t> pixelbounds = {2, 3, 4, 5};
  Parameter pixelbounds_parameter{.directive = "",
                                  .type = ParameterType::INTEGER,
                                  .type_name = "",
                                  .values = absl::MakeSpan(pixelbounds)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"maxdepth", maxdepth_parameter},
      {"strategy", strategy_parameter},
      {"pixelbounds", pixelbounds_parameter}};

  DirectLightingIntegrator actual;
  RemoveDirectLightingIntegratorV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                maxdepth: 1
                strategy: ALL
                pixelbounds { x_min: 2 x_max: 3 y_min: 4 y_max: 5 }
              )pb"));
}

TEST(RemoveDirectLightingIntegratorV3, All) {
  std::vector<absl::string_view> strategy = {"all"};
  Parameter strategy_parameter{.directive = "",
                               .type = ParameterType::STRING,
                               .type_name = "",
                               .values = absl::MakeSpan(strategy)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"strategy", strategy_parameter}};

  DirectLightingIntegrator actual;
  RemoveDirectLightingIntegratorV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                strategy: ALL
              )pb"));
}

TEST(RemoveDirectLightingIntegratorV3, One) {
  std::vector<absl::string_view> strategy = {"one"};
  Parameter strategy_parameter{.directive = "",
                               .type = ParameterType::STRING,
                               .type_name = "",
                               .values = absl::MakeSpan(strategy)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"strategy", strategy_parameter}};

  DirectLightingIntegrator actual;
  RemoveDirectLightingIntegratorV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                strategy: ONE
              )pb"));
}

TEST(RemoveDirectLightingIntegratorV3, Weighted) {
  std::vector<absl::string_view> strategy = {"weighted"};
  Parameter strategy_parameter{.directive = "",
                               .type = ParameterType::STRING,
                               .type_name = "",
                               .values = absl::MakeSpan(strategy)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"strategy", strategy_parameter}};

  DirectLightingIntegrator actual;
  RemoveDirectLightingIntegratorV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                strategy: ALL
              )pb"));
}

TEST(RemoveGlossyPrtIntegratorV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  GlossyPrtIntegrator actual;
  EXPECT_TRUE(RemoveGlossyPrtIntegratorV2(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveGlossyPrtIntegratorV2, WithData) {
  std::vector<int32_t> lmax = {1};
  Parameter lmax_parameter{.directive = "",
                           .type = ParameterType::INTEGER,
                           .type_name = "",
                           .values = absl::MakeSpan(lmax)};

  std::vector<int32_t> nsamples = {2};
  Parameter nsamples_parameter{.directive = "",
                               .type = ParameterType::INTEGER,
                               .type_name = "",
                               .values = absl::MakeSpan(nsamples)};

  std::vector<double> roughness = {0.3};
  Parameter roughness_parameter{.directive = "",
                                .type = ParameterType::FLOAT,
                                .type_name = "",
                                .values = absl::MakeSpan(roughness)};

  std::vector<absl::string_view> kd = {"kd"};
  Parameter kd_parameter{.directive = "",
                         .type = ParameterType::SPECTRUM,
                         .type_name = "",
                         .values = absl::MakeSpan(kd)};

  std::vector<absl::string_view> ks = {"ks"};
  Parameter ks_parameter{.directive = "",
                         .type = ParameterType::SPECTRUM,
                         .type_name = "",
                         .values = absl::MakeSpan(ks)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"lmax", lmax_parameter},
      {"nsamples", nsamples_parameter},
      {"roughness", roughness_parameter},
      {"Kd", kd_parameter},
      {"Ks", ks_parameter},
  };

  GlossyPrtIntegrator actual;
  EXPECT_TRUE(RemoveGlossyPrtIntegratorV2(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                lmax: 1
                nsamples: 2
                roughness: 0.3
                Kd { sampled_spectrum_filename: "kd" }
                Ks { sampled_spectrum_filename: "ks" }
              )pb"));
}

TEST(RemoveIgiIntegratorV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  IgiIntegrator actual;
  RemoveIgiIntegratorV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveIgiIntegratorV1, WithData) {
  std::vector<int32_t> nlights = {1};
  Parameter nlights_parameter{.directive = "",
                              .type = ParameterType::INTEGER,
                              .type_name = "",
                              .values = absl::MakeSpan(nlights)};

  std::vector<int32_t> nsets = {2};
  Parameter nsets_parameter{.directive = "",
                            .type = ParameterType::INTEGER,
                            .type_name = "",
                            .values = absl::MakeSpan(nsets)};

  std::vector<double> rrthreshold = {0.3};
  Parameter rrthreshold_parameter{.directive = "",
                                  .type = ParameterType::FLOAT,
                                  .type_name = "",
                                  .values = absl::MakeSpan(rrthreshold)};

  std::vector<double> indirectscale = {0.4};
  Parameter indirectscale_parameter{.directive = "",
                                    .type = ParameterType::FLOAT,
                                    .type_name = "",
                                    .values = absl::MakeSpan(indirectscale)};

  std::vector<double> mindist = {0.5};
  Parameter mindist_parameter{.directive = "",
                              .type = ParameterType::FLOAT,
                              .type_name = "",
                              .values = absl::MakeSpan(mindist)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"nlights", nlights_parameter},
      {"nsets", nsets_parameter},
      {"rrthreshold", rrthreshold_parameter},
      {"indirectscale", indirectscale_parameter},
      {"mindist", mindist_parameter}};

  IgiIntegrator actual;
  RemoveIgiIntegratorV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                nlights: 1
                nsets: 2
                rrthreshold: 0.3
                indirectscale: 0.4
                mindist: 0.5
              )pb"));
}

TEST(RemoveIgiIntegratorV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  IgiIntegrator actual;
  RemoveIgiIntegratorV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveIgiIntegratorV2, WithData) {
  std::vector<int32_t> maxdepth = {1};
  Parameter maxdepth_parameter{.directive = "",
                               .type = ParameterType::INTEGER,
                               .type_name = "",
                               .values = absl::MakeSpan(maxdepth)};

  std::vector<int32_t> nlights = {2};
  Parameter nlights_parameter{.directive = "",
                              .type = ParameterType::INTEGER,
                              .type_name = "",
                              .values = absl::MakeSpan(nlights)};

  std::vector<int32_t> nsets = {3};
  Parameter nsets_parameter{.directive = "",
                            .type = ParameterType::INTEGER,
                            .type_name = "",
                            .values = absl::MakeSpan(nsets)};

  std::vector<double> rrthreshold = {0.4};
  Parameter rrthreshold_parameter{.directive = "",
                                  .type = ParameterType::FLOAT,
                                  .type_name = "",
                                  .values = absl::MakeSpan(rrthreshold)};

  std::vector<double> glimit = {0.5};
  Parameter glimit_parameter{.directive = "",
                             .type = ParameterType::FLOAT,
                             .type_name = "",
                             .values = absl::MakeSpan(glimit)};

  std::vector<int32_t> gathersamples = {6};
  Parameter gathersamples_parameter{.directive = "",
                                    .type = ParameterType::INTEGER,
                                    .type_name = "",
                                    .values = absl::MakeSpan(gathersamples)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"maxdepth", maxdepth_parameter},
      {"nlights", nlights_parameter},
      {"nsets", nsets_parameter},
      {"rrthreshold", rrthreshold_parameter},
      {"glimit", glimit_parameter},
      {"gathersamples", gathersamples_parameter}};

  IgiIntegrator actual;
  RemoveIgiIntegratorV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                maxdepth: 1
                nlights: 2
                nsets: 3
                rrthreshold: 0.4
                glimit: 0.5
                gathersamples: 6
              )pb"));
}

TEST(RemoveIrradianceCacheIntegratorV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  IrradianceCacheIntegrator actual;
  RemoveIrradianceCacheIntegratorV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveIrradianceCacheIntegratorV1, WithData) {
  std::vector<double> minweight = {0.1};
  Parameter minweight_parameter{.directive = "",
                                .type = ParameterType::FLOAT,
                                .type_name = "",
                                .values = absl::MakeSpan(minweight)};

  std::vector<double> minpixelspacing = {0.2};
  Parameter minpixelspacing_parameter{
      .directive = "",
      .type = ParameterType::FLOAT,
      .type_name = "",
      .values = absl::MakeSpan(minpixelspacing)};

  std::vector<double> maxpixelspacing = {0.3};
  Parameter maxpixelspacing_parameter{
      .directive = "",
      .type = ParameterType::FLOAT,
      .type_name = "",
      .values = absl::MakeSpan(maxpixelspacing)};

  std::vector<double> maxangledifference = {0.4};
  Parameter maxangledifference_parameter{
      .directive = "",
      .type = ParameterType::FLOAT,
      .type_name = "",
      .values = absl::MakeSpan(maxangledifference)};

  std::vector<int32_t> maxspeculardepth = {5};
  Parameter maxspeculardepth_parameter{
      .directive = "",
      .type = ParameterType::INTEGER,
      .type_name = "",
      .values = absl::MakeSpan(maxspeculardepth)};

  std::vector<int32_t> maxindirectdepth = {6};
  Parameter maxindirectdepth_parameter{
      .directive = "",
      .type = ParameterType::INTEGER,
      .type_name = "",
      .values = absl::MakeSpan(maxindirectdepth)};

  std::vector<int32_t> nsamples = {7};
  Parameter nsamples_parameter{.directive = "",
                               .type = ParameterType::INTEGER,
                               .type_name = "",
                               .values = absl::MakeSpan(nsamples)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"minweight", minweight_parameter},
      {"minpixelspacing", minpixelspacing_parameter},
      {"maxpixelspacing", maxpixelspacing_parameter},
      {"maxangledifference", maxangledifference_parameter},
      {"maxspeculardepth", maxspeculardepth_parameter},
      {"maxindirectdepth", maxindirectdepth_parameter},
      {"nsamples", nsamples_parameter}};

  IrradianceCacheIntegrator actual;
  RemoveIrradianceCacheIntegratorV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                minweight: 0.1
                minpixelspacing: 0.2
                maxpixelspacing: 0.3
                maxangledifference: 0.4
                maxspeculardepth: 5
                maxindirectdepth: 6
                nsamples: 7
              )pb"));
}

TEST(RemoveMltIntegratorV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  MltIntegrator actual;
  RemoveMltIntegratorV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveMltIntegratorV3, WithData) {
  std::vector<int32_t> maxdepth = {1};
  Parameter maxdepth_parameter{.directive = "",
                               .type = ParameterType::INTEGER,
                               .type_name = "",
                               .values = absl::MakeSpan(maxdepth)};

  std::vector<int32_t> bootstrapsamples = {2};
  Parameter bootstrapsamples_parameter{
      .directive = "",
      .type = ParameterType::INTEGER,
      .type_name = "",
      .values = absl::MakeSpan(bootstrapsamples)};

  std::vector<int32_t> chains = {3};
  Parameter chains_parameter{.directive = "",
                             .type = ParameterType::INTEGER,
                             .type_name = "",
                             .values = absl::MakeSpan(chains)};

  std::vector<int32_t> mutationsperpixel = {4};
  Parameter mutationsperpixel_parameter{
      .directive = "",
      .type = ParameterType::INTEGER,
      .type_name = "",
      .values = absl::MakeSpan(mutationsperpixel)};

  std::vector<double> largestepprobability = {0.5};
  Parameter largestepprobability_parameter{
      .directive = "",
      .type = ParameterType::FLOAT,
      .type_name = "",
      .values = absl::MakeSpan(largestepprobability)};

  std::vector<double> sigma = {0.6};
  Parameter sigma_parameter{.directive = "",
                            .type = ParameterType::FLOAT,
                            .type_name = "",
                            .values = absl::MakeSpan(sigma)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"maxdepth", maxdepth_parameter},
      {"bootstrapsamples", bootstrapsamples_parameter},
      {"chains", chains_parameter},
      {"mutationsperpixel", mutationsperpixel_parameter},
      {"largestepprobability", largestepprobability_parameter},
      {"sigma", sigma_parameter}};

  MltIntegrator actual;
  RemoveMltIntegratorV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                maxdepth: 1
                bootstrapsamples: 2
                chains: 3
                mutationsperpixel: 4
                largestepprobability: 0.5
                sigma: 0.6
              )pb"));
}

TEST(RemovePathIntegratorV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  PathIntegrator actual;
  RemovePathIntegratorV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemovePathIntegratorV1, WithData) {
  std::vector<int32_t> maxdepth = {1};
  Parameter maxdepth_parameter{.directive = "",
                               .type = ParameterType::INTEGER,
                               .type_name = "",
                               .values = absl::MakeSpan(maxdepth)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"maxdepth", maxdepth_parameter}};

  PathIntegrator actual;
  RemovePathIntegratorV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                maxdepth: 1
              )pb"));
}

TEST(RemovePathIntegratorV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  PathIntegrator actual;
  RemovePathIntegratorV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemovePathIntegratorV3, WithData) {
  std::vector<int32_t> maxdepth = {1};
  Parameter maxdepth_parameter{.directive = "",
                               .type = ParameterType::INTEGER,
                               .type_name = "",
                               .values = absl::MakeSpan(maxdepth)};

  std::vector<double> rrthreshold = {0.2};
  Parameter rrthreshold_parameter{.directive = "",
                                  .type = ParameterType::FLOAT,
                                  .type_name = "",
                                  .values = absl::MakeSpan(rrthreshold)};

  std::vector<int32_t> pixelbounds = {3, 4, 5, 6};
  Parameter pixelbounds_parameter{.directive = "",
                                  .type = ParameterType::INTEGER,
                                  .type_name = "",
                                  .values = absl::MakeSpan(pixelbounds)};

  std::vector<absl::string_view> lightsamplestrategy = {"unknown"};
  Parameter lightsamplestrategy_parameter{
      .directive = "",
      .type = ParameterType::STRING,
      .type_name = "",
      .values = absl::MakeSpan(lightsamplestrategy)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"maxdepth", maxdepth_parameter},
      {"rrthreshold", rrthreshold_parameter},
      {"pixelbounds", pixelbounds_parameter},
      {"lightsamplestrategy", lightsamplestrategy_parameter}};

  PathIntegrator actual;
  RemovePathIntegratorV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                maxdepth: 1
                rrthreshold: 0.2
                pixelbounds { x_min: 3 x_max: 4 y_min: 5 y_max: 6 }
                lightsampler: BVH
              )pb"));
}

TEST(RemovePathIntegratorV3, Uniform) {
  std::vector<absl::string_view> lightsamplestrategy = {"uniform"};
  Parameter lightsamplestrategy_parameter{
      .directive = "",
      .type = ParameterType::STRING,
      .type_name = "",
      .values = absl::MakeSpan(lightsamplestrategy)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"lightsamplestrategy", lightsamplestrategy_parameter},
  };

  PathIntegrator actual;
  RemovePathIntegratorV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                lightsampler: UNIFORM
              )pb"));
}

TEST(RemovePathIntegratorV3, Power) {
  std::vector<absl::string_view> lightsamplestrategy = {"power"};
  Parameter lightsamplestrategy_parameter{
      .directive = "",
      .type = ParameterType::STRING,
      .type_name = "",
      .values = absl::MakeSpan(lightsamplestrategy)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"lightsamplestrategy", lightsamplestrategy_parameter},
  };

  PathIntegrator actual;
  RemovePathIntegratorV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                lightsampler: POWER
              )pb"));
}

TEST(RemovePathIntegratorV3, Spatial) {
  std::vector<absl::string_view> lightsamplestrategy = {"spatial"};
  Parameter lightsamplestrategy_parameter{
      .directive = "",
      .type = ParameterType::STRING,
      .type_name = "",
      .values = absl::MakeSpan(lightsamplestrategy)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"lightsamplestrategy", lightsamplestrategy_parameter},
  };

  PathIntegrator actual;
  RemovePathIntegratorV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                lightsampler: BVH
              )pb"));
}

TEST(RemovePhotonMapIntegratorV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  PhotonMapIntegrator actual;
  RemovePhotonMapIntegratorV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemovePhotonMapIntegratorV1, WithData) {
  std::vector<int32_t> causticphotons = {1};
  Parameter causticphotons_parameter{.directive = "",
                                     .type = ParameterType::INTEGER,
                                     .type_name = "",
                                     .values = absl::MakeSpan(causticphotons)};

  std::vector<int32_t> directphotons = {2};
  Parameter directphotons_parameter{.directive = "",
                                    .type = ParameterType::INTEGER,
                                    .type_name = "",
                                    .values = absl::MakeSpan(directphotons)};

  std::vector<int32_t> indirectphotons = {3};
  Parameter indirectphotons_parameter{
      .directive = "",
      .type = ParameterType::INTEGER,
      .type_name = "",
      .values = absl::MakeSpan(indirectphotons)};

  std::vector<int32_t> maxdepth = {4};
  Parameter maxdepth_parameter{.directive = "",
                               .type = ParameterType::INTEGER,
                               .type_name = "",
                               .values = absl::MakeSpan(maxdepth)};

  std::vector<int32_t> finalgathersamples = {5};
  Parameter finalgathersamples_parameter{
      .directive = "",
      .type = ParameterType::INTEGER,
      .type_name = "",
      .values = absl::MakeSpan(finalgathersamples)};

  std::vector<double> maxdist = {0.6};
  Parameter maxdist_parameter{.directive = "",
                              .type = ParameterType::FLOAT,
                              .type_name = "",
                              .values = absl::MakeSpan(maxdist)};

  bool finalgather[] = {true};
  Parameter finalgather_parameter{.directive = "",
                                  .type = ParameterType::BOOL,
                                  .type_name = "",
                                  .values = absl::MakeSpan(finalgather)};

  bool directwithphotons[] = {true};
  Parameter directwithphotons_parameter{
      .directive = "",
      .type = ParameterType::BOOL,
      .type_name = "",
      .values = absl::MakeSpan(directwithphotons)};

  std::vector<double> rrthreshold = {0.7};
  Parameter rrthreshold_parameter{.directive = "",
                                  .type = ParameterType::FLOAT,
                                  .type_name = "",
                                  .values = absl::MakeSpan(rrthreshold)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"causticphotons", causticphotons_parameter},
      {"directphotons", directphotons_parameter},
      {"indirectphotons", indirectphotons_parameter},
      {"maxdepth", maxdepth_parameter},
      {"finalgathersamples", finalgathersamples_parameter},
      {"maxdist", maxdist_parameter},
      {"finalgather", finalgather_parameter},
      {"directwithphotons", directwithphotons_parameter},
      {"rrthreshold", rrthreshold_parameter}};

  PhotonMapIntegrator actual;
  RemovePhotonMapIntegratorV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                causticphotons: 1
                directphotons: 2
                indirectphotons: 3
                maxdepth: 4
                finalgathersamples: 5
                maxdist: 0.6
                finalgather: true
                directwithphotons: true
                rrthreshold: 0.7
              )pb"));
}

TEST(RemoveExPhotonMapIntegratorV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  PhotonMapIntegrator actual;
  RemoveExPhotonMapIntegratorV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveExPhotonMapIntegratorV1, WithData) {
  std::vector<int32_t> causticphotons = {1};
  Parameter causticphotons_parameter{.directive = "",
                                     .type = ParameterType::INTEGER,
                                     .type_name = "",
                                     .values = absl::MakeSpan(causticphotons)};

  std::vector<int32_t> directphotons = {2};
  Parameter directphotons_parameter{.directive = "",
                                    .type = ParameterType::INTEGER,
                                    .type_name = "",
                                    .values = absl::MakeSpan(directphotons)};

  std::vector<int32_t> indirectphotons = {3};
  Parameter indirectphotons_parameter{
      .directive = "",
      .type = ParameterType::INTEGER,
      .type_name = "",
      .values = absl::MakeSpan(indirectphotons)};

  std::vector<int32_t> maxdepth = {4};
  Parameter maxdepth_parameter{.directive = "",
                               .type = ParameterType::INTEGER,
                               .type_name = "",
                               .values = absl::MakeSpan(maxdepth)};

  std::vector<int32_t> finalgathersamples = {5};
  Parameter finalgathersamples_parameter{
      .directive = "",
      .type = ParameterType::INTEGER,
      .type_name = "",
      .values = absl::MakeSpan(finalgathersamples)};

  std::vector<double> maxdist = {0.6};
  Parameter maxdist_parameter{.directive = "",
                              .type = ParameterType::FLOAT,
                              .type_name = "",
                              .values = absl::MakeSpan(maxdist)};

  bool finalgather[] = {true};
  Parameter finalgather_parameter{.directive = "",
                                  .type = ParameterType::BOOL,
                                  .type_name = "",
                                  .values = absl::MakeSpan(finalgather)};

  bool directwithphotons[] = {true};
  Parameter directwithphotons_parameter{
      .directive = "",
      .type = ParameterType::BOOL,
      .type_name = "",
      .values = absl::MakeSpan(directwithphotons)};

  std::vector<double> rrthreshold = {0.7};
  Parameter rrthreshold_parameter{.directive = "",
                                  .type = ParameterType::FLOAT,
                                  .type_name = "",
                                  .values = absl::MakeSpan(rrthreshold)};

  std::vector<double> gatherangle = {0.8};
  Parameter gatherangle_parameter{.directive = "",
                                  .type = ParameterType::FLOAT,
                                  .type_name = "",
                                  .values = absl::MakeSpan(gatherangle)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"causticphotons", causticphotons_parameter},
      {"directphotons", directphotons_parameter},
      {"indirectphotons", indirectphotons_parameter},
      {"maxdepth", maxdepth_parameter},
      {"finalgathersamples", finalgathersamples_parameter},
      {"maxdist", maxdist_parameter},
      {"finalgather", finalgather_parameter},
      {"directwithphotons", directwithphotons_parameter},
      {"rrthreshold", rrthreshold_parameter},
      {"gatherangle", gatherangle_parameter}};

  PhotonMapIntegrator actual;
  RemoveExPhotonMapIntegratorV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                causticphotons: 1
                directphotons: 2
                indirectphotons: 3
                maxdepth: 4
                finalgathersamples: 5
                maxdist: 0.6
                finalgather: true
                directwithphotons: true
                rrthreshold: 0.7
                gatherangle: 0.8
              )pb"));
}

TEST(RemovePhotonMapIntegratorV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  PhotonMapIntegrator actual;
  RemovePhotonMapIntegratorV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemovePhotonMapIntegratorV2, WithData) {
  std::vector<int32_t> causticphotons = {1};
  Parameter causticphotons_parameter{.directive = "",
                                     .type = ParameterType::INTEGER,
                                     .type_name = "",
                                     .values = absl::MakeSpan(causticphotons)};

  std::vector<int32_t> indirectphotons = {2};
  Parameter indirectphotons_parameter{
      .directive = "",
      .type = ParameterType::INTEGER,
      .type_name = "",
      .values = absl::MakeSpan(indirectphotons)};

  std::vector<int32_t> finalgathersamples = {3};
  Parameter finalgathersamples_parameter{
      .directive = "",
      .type = ParameterType::INTEGER,
      .type_name = "",
      .values = absl::MakeSpan(finalgathersamples)};

  std::vector<int32_t> maxspeculardepth = {4};
  Parameter maxspeculardepth_parameter{
      .directive = "",
      .type = ParameterType::INTEGER,
      .type_name = "",
      .values = absl::MakeSpan(maxspeculardepth)};

  std::vector<int32_t> maxphotondepth = {5};
  Parameter maxphotondepth_parameter{.directive = "",
                                     .type = ParameterType::INTEGER,
                                     .type_name = "",
                                     .values = absl::MakeSpan(maxphotondepth)};

  std::vector<double> maxdist = {0.6};
  Parameter maxdist_parameter{.directive = "",
                              .type = ParameterType::FLOAT,
                              .type_name = "",
                              .values = absl::MakeSpan(maxdist)};

  bool finalgather[] = {true};
  Parameter finalgather_parameter{.directive = "",
                                  .type = ParameterType::BOOL,
                                  .type_name = "",
                                  .values = absl::MakeSpan(finalgather)};

  std::vector<double> rrthreshold = {0.7};
  Parameter rrthreshold_parameter{.directive = "",
                                  .type = ParameterType::FLOAT,
                                  .type_name = "",
                                  .values = absl::MakeSpan(rrthreshold)};

  std::vector<double> gatherangle = {0.8};
  Parameter gatherangle_parameter{.directive = "",
                                  .type = ParameterType::FLOAT,
                                  .type_name = "",
                                  .values = absl::MakeSpan(gatherangle)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"causticphotons", causticphotons_parameter},
      {"indirectphotons", indirectphotons_parameter},
      {"finalgathersamples", finalgathersamples_parameter},
      {"maxspeculardepth", maxspeculardepth_parameter},
      {"maxphotondepth", maxphotondepth_parameter},
      {"maxdist", maxdist_parameter},
      {"finalgather", finalgather_parameter},
      {"rrthreshold", rrthreshold_parameter},
      {"gatherangle", gatherangle_parameter}};

  PhotonMapIntegrator actual;
  RemovePhotonMapIntegratorV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                causticphotons: 1
                indirectphotons: 2
                finalgathersamples: 3
                maxspeculardepth: 4
                maxphotondepth: 5
                maxdist: 0.6
                finalgather: true
                rrthreshold: 0.7
                gatherangle: 0.8
              )pb"));
}

TEST(RemoveSppmIntegratorV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  SppmIntegrator actual;
  RemoveSppmIntegratorV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveSppmIntegratorV3, WithData) {
  std::vector<int32_t> maxdepth = {1};
  Parameter maxdepth_parameter{.directive = "",
                               .type = ParameterType::INTEGER,
                               .type_name = "",
                               .values = absl::MakeSpan(maxdepth)};

  std::vector<int32_t> numiterations = {2};
  Parameter numiterations_parameter{.directive = "",
                                    .type = ParameterType::INTEGER,
                                    .type_name = "",
                                    .values = absl::MakeSpan(numiterations)};

  std::vector<int32_t> photonsperiteration = {3};
  Parameter photonsperiteration_parameter{
      .directive = "",
      .type = ParameterType::INTEGER,
      .type_name = "",
      .values = absl::MakeSpan(photonsperiteration)};

  std::vector<int32_t> imagewritefrequency = {4};
  Parameter imagewritefrequency_parameter{
      .directive = "",
      .type = ParameterType::INTEGER,
      .type_name = "",
      .values = absl::MakeSpan(imagewritefrequency)};

  std::vector<double> radius = {0.5};
  Parameter radius_parameter{.directive = "",
                             .type = ParameterType::FLOAT,
                             .type_name = "",
                             .values = absl::MakeSpan(radius)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"maxdepth", maxdepth_parameter},
      {"numiterations", numiterations_parameter},
      {"photonsperiteration", photonsperiteration_parameter},
      {"imagewritefrequency", imagewritefrequency_parameter},
      {"radius", radius_parameter}};

  SppmIntegrator actual;
  RemoveSppmIntegratorV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                maxdepth: 1
                numiterations: 2
                photonsperiteration: 3
                imagewritefrequency: 4
                radius: 0.5
              )pb"));
}

TEST(RemoveUseProbesIntegratorV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  UseProbesIntegrator actual;
  RemoveUseProbesIntegratorV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveUseProbesIntegratorV2, WithData) {
  std::vector<absl::string_view> filename = {"file"};
  Parameter filename_parameter{.directive = "",
                               .type = ParameterType::STRING,
                               .type_name = "",
                               .values = absl::MakeSpan(filename)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"filename", filename_parameter}};

  UseProbesIntegrator actual;
  RemoveUseProbesIntegratorV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                filename: "file"
              )pb"));
}

TEST(RemoveVolPathIntegratorV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  VolPathIntegrator actual;
  RemoveVolPathIntegratorV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveVolPathIntegratorV3, WithData) {
  std::vector<int32_t> maxdepth = {1};
  Parameter maxdepth_parameter{.directive = "",
                               .type = ParameterType::INTEGER,
                               .type_name = "",
                               .values = absl::MakeSpan(maxdepth)};

  std::vector<double> rrthreshold = {0.2};
  Parameter rrthreshold_parameter{.directive = "",
                                  .type = ParameterType::FLOAT,
                                  .type_name = "",
                                  .values = absl::MakeSpan(rrthreshold)};

  std::vector<int32_t> pixelbounds = {3, 4, 5, 6};
  Parameter pixelbounds_parameter{.directive = "",
                                  .type = ParameterType::INTEGER,
                                  .type_name = "",
                                  .values = absl::MakeSpan(pixelbounds)};

  std::vector<absl::string_view> lightsamplestrategy = {"unknown"};
  Parameter lightsamplestrategy_parameter{
      .directive = "",
      .type = ParameterType::STRING,
      .type_name = "",
      .values = absl::MakeSpan(lightsamplestrategy)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"maxdepth", maxdepth_parameter},
      {"rrthreshold", rrthreshold_parameter},
      {"pixelbounds", pixelbounds_parameter},
      {"lightsamplestrategy", lightsamplestrategy_parameter}};

  VolPathIntegrator actual;
  RemoveVolPathIntegratorV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                maxdepth: 1
                rrthreshold: 0.2
                pixelbounds { x_min: 3 x_max: 4 y_min: 5 y_max: 6 }
                lightsampler: BVH
              )pb"));
}

TEST(RemoveVolPathIntegratorV3, Uniform) {
  std::vector<absl::string_view> lightsamplestrategy = {"uniform"};
  Parameter lightsamplestrategy_parameter{
      .directive = "",
      .type = ParameterType::STRING,
      .type_name = "",
      .values = absl::MakeSpan(lightsamplestrategy)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"lightsamplestrategy", lightsamplestrategy_parameter},
  };

  VolPathIntegrator actual;
  RemoveVolPathIntegratorV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                lightsampler: UNIFORM
              )pb"));
}

TEST(RemoveVolPathIntegratorV3, Power) {
  std::vector<absl::string_view> lightsamplestrategy = {"power"};
  Parameter lightsamplestrategy_parameter{
      .directive = "",
      .type = ParameterType::STRING,
      .type_name = "",
      .values = absl::MakeSpan(lightsamplestrategy)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"lightsamplestrategy", lightsamplestrategy_parameter},
  };

  VolPathIntegrator actual;
  RemoveVolPathIntegratorV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                lightsampler: POWER
              )pb"));
}

TEST(RemoveVolPathIntegratorV3, Spatial) {
  std::vector<absl::string_view> lightsamplestrategy = {"spatial"};
  Parameter lightsamplestrategy_parameter{
      .directive = "",
      .type = ParameterType::STRING,
      .type_name = "",
      .values = absl::MakeSpan(lightsamplestrategy)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"lightsamplestrategy", lightsamplestrategy_parameter},
  };

  VolPathIntegrator actual;
  RemoveVolPathIntegratorV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                lightsampler: BVH
              )pb"));
}

TEST(RemoveWhittedIntegratorV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  WhittedIntegrator actual;
  RemoveWhittedIntegratorV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveWhittedIntegratorV1, WithData) {
  std::vector<int32_t> maxdepth = {1};
  Parameter maxdepth_parameter{.directive = "",
                               .type = ParameterType::INTEGER,
                               .type_name = "",
                               .values = absl::MakeSpan(maxdepth)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"maxdepth", maxdepth_parameter}};

  WhittedIntegrator actual;
  RemoveWhittedIntegratorV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                maxdepth: 1
              )pb"));
}

TEST(RemoveWhittedIntegratorV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  WhittedIntegrator actual;
  RemoveWhittedIntegratorV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveWhittedIntegratorV3, WithData) {
  std::vector<int32_t> maxdepth = {1};
  Parameter maxdepth_parameter{.directive = "",
                               .type = ParameterType::INTEGER,
                               .type_name = "",
                               .values = absl::MakeSpan(maxdepth)};

  std::vector<int32_t> pixelbounds = {3, 4, 5, 6};
  Parameter pixelbounds_parameter{.directive = "",
                                  .type = ParameterType::INTEGER,
                                  .type_name = "",
                                  .values = absl::MakeSpan(pixelbounds)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"maxdepth", maxdepth_parameter},
      {"pixelbounds", pixelbounds_parameter},
  };

  WhittedIntegrator actual;
  RemoveWhittedIntegratorV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                maxdepth: 1
                pixelbounds { x_min: 3 x_max: 4 y_min: 5 y_max: 6 }
              )pb"));
}

}  // namespace
}  // namespace pbrt_proto
