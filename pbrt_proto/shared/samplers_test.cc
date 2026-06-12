#include "pbrt_proto/shared/samplers.h"

#include <cstdint>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "absl/strings/string_view.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"
#include "pbrt_proto/testing/proto_matchers.h"

namespace pbrt_proto {
namespace {

using ::google::protobuf::EqualsProto;

TEST(RemoveAdaptiveSampler, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  AdaptiveSampler actual;
  EXPECT_TRUE(
      RemoveAdaptiveSampler(parameters, /*pbrt_version=*/2, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveAdaptiveSampler, Contrast) {
  std::vector<absl::string_view> method = {"contrast"};
  Parameter method_parameter{/*directive=*/"",
                             /*type=*/ParameterType::STRING,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(method)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"method", method_parameter}};

  AdaptiveSampler actual;
  EXPECT_TRUE(
      RemoveAdaptiveSampler(parameters, /*pbrt_version=*/2, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                method: CONTRAST
              )pb"));
}

TEST(RemoveAdaptiveSampler, ShapeId) {
  std::vector<absl::string_view> method = {"shapeid"};
  Parameter method_parameter{/*directive=*/"",
                             /*type=*/ParameterType::STRING,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(method)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"method", method_parameter}};

  AdaptiveSampler actual;
  EXPECT_TRUE(
      RemoveAdaptiveSampler(parameters, /*pbrt_version=*/2, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                method: SHAPEID
              )pb"));
}

TEST(RemoveAdaptiveSampler, Unknown) {
  std::vector<absl::string_view> method = {"unknown"};
  Parameter method_parameter{/*directive=*/"",
                             /*type=*/ParameterType::STRING,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(method)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"method", method_parameter}};

  AdaptiveSampler actual;
  EXPECT_TRUE(
      RemoveAdaptiveSampler(parameters, /*pbrt_version=*/2, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                method: CONTRAST
              )pb"));
}

TEST(RemoveAdaptiveSampler, WithData) {
  std::vector<int32_t> minsamples = {1};
  Parameter minsamples_parameter{/*directive=*/"",
                                 /*type=*/ParameterType::INTEGER,
                                 /*type_name=*/"",
                                 /*values=*/absl::MakeSpan(minsamples)};

  std::vector<int32_t> maxsamples = {2};
  Parameter maxsamples_parameter{/*directive=*/"",
                                 /*type=*/ParameterType::INTEGER,
                                 /*type_name=*/"",
                                 /*values=*/absl::MakeSpan(maxsamples)};

  std::vector<absl::string_view> method = {"contrast"};
  Parameter method_parameter{/*directive=*/"",
                             /*type=*/ParameterType::STRING,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(method)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"minsamples", minsamples_parameter},
      {"maxsamples", maxsamples_parameter},
      {"method", method_parameter}};

  AdaptiveSampler actual;
  EXPECT_TRUE(
      RemoveAdaptiveSampler(parameters, /*pbrt_version=*/2, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(
                minsamples: 1 maxsamples: 2 method: CONTRAST
              )pb"));
}

TEST(RemoveBestCandidateSampler, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  BestCandidateSampler actual;
  EXPECT_TRUE(
      RemoveBestCandidateSampler(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveBestCandidateSampler, WithData) {
  std::vector<int32_t> pixelsamples = {1};
  Parameter pixelsamples_parameter{/*directive=*/"",
                                   /*type=*/ParameterType::INTEGER,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(pixelsamples)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"pixelsamples", pixelsamples_parameter}};

  BestCandidateSampler actual;
  EXPECT_TRUE(
      RemoveBestCandidateSampler(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                pixelsamples: 1
              )pb"));
}

TEST(RemoveHaltonSamplerV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  HaltonSampler actual;
  EXPECT_TRUE(RemoveHaltonSampler(parameters, /*pbrt_version=*/2, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveHaltonSamplerV2, WithData) {
  std::vector<int32_t> pixelsamples = {1};
  Parameter pixelsamples_parameter{/*directive=*/"",
                                   /*type=*/ParameterType::INTEGER,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(pixelsamples)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"pixelsamples", pixelsamples_parameter}};

  HaltonSampler actual;
  EXPECT_TRUE(RemoveHaltonSampler(parameters, /*pbrt_version=*/2, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                pixelsamples: 1
              )pb"));
}

TEST(RemoveHaltonSamplerV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  HaltonSampler actual;
  EXPECT_TRUE(RemoveHaltonSampler(parameters, /*pbrt_version=*/3, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveHaltonSamplerV3, WithData) {
  std::vector<int32_t> pixelsamples = {1};
  Parameter pixelsamples_parameter{/*directive=*/"",
                                   /*type=*/ParameterType::INTEGER,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(pixelsamples)};

  bool samplepixelcenter[] = {true};
  Parameter samplepixelcenter_parameter{
      /*directive=*/"",
      /*type=*/ParameterType::BOOL,
      /*type_name=*/"",
      /*values=*/absl::MakeSpan(samplepixelcenter)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"pixelsamples", pixelsamples_parameter},
      {"samplepixelcenter", samplepixelcenter_parameter}};

  HaltonSampler actual;
  EXPECT_TRUE(RemoveHaltonSampler(parameters, /*pbrt_version=*/3, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                pixelsamples: 1 samplepixelcenter: true
              )pb"));
}

TEST(RemoveHaltonSamplerV4, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  HaltonSampler actual;
  EXPECT_TRUE(RemoveHaltonSampler(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveHaltonSamplerV4, WithData) {
  std::vector<int32_t> pixelsamples = {1};
  Parameter pixelsamples_parameter{/*directive=*/"",
                                   /*type=*/ParameterType::INTEGER,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(pixelsamples)};

  std::vector<int32_t> seed = {2};
  Parameter seed_parameter{/*directive=*/"",
                           /*type=*/ParameterType::INTEGER,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(seed)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"pixelsamples", pixelsamples_parameter}, {"seed", seed_parameter}};

  HaltonSampler actual;
  EXPECT_TRUE(RemoveHaltonSampler(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                pixelsamples: 1 seed: 2
              )pb"));
}

TEST(RemoveIndependentSamplerV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  IndependentSampler actual;
  EXPECT_TRUE(
      RemoveIndependentSampler(parameters, /*pbrt_version=*/2, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveIndependentSamplerV2, WithData) {
  std::vector<int32_t> pixelsamples = {1};
  Parameter pixelsamples_parameter{/*directive=*/"",
                                   /*type=*/ParameterType::INTEGER,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(pixelsamples)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"pixelsamples", pixelsamples_parameter}};

  IndependentSampler actual;
  EXPECT_TRUE(
      RemoveIndependentSampler(parameters, /*pbrt_version=*/2, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                pixelsamples: 1
              )pb"));
}

TEST(RemoveIndependentSamplerV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  IndependentSampler actual;
  EXPECT_TRUE(
      RemoveIndependentSampler(parameters, /*pbrt_version=*/3, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveIndependentSamplerV3, WithData) {
  std::vector<int32_t> pixelsamples = {1};
  Parameter pixelsamples_parameter{/*directive=*/"",
                                   /*type=*/ParameterType::INTEGER,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(pixelsamples)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"pixelsamples", pixelsamples_parameter}};

  IndependentSampler actual;
  EXPECT_TRUE(
      RemoveIndependentSampler(parameters, /*pbrt_version=*/3, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                pixelsamples: 1
              )pb"));
}

TEST(RemoveIndependentSamplerV4, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  IndependentSampler actual;
  EXPECT_TRUE(
      RemoveIndependentSampler(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveIndependentSamplerV4, WithData) {
  std::vector<int32_t> pixelsamples = {1};
  Parameter pixelsamples_parameter{/*directive=*/"",
                                   /*type=*/ParameterType::INTEGER,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(pixelsamples)};

  std::vector<int32_t> seed = {2};
  Parameter seed_parameter{/*directive=*/"",
                           /*type=*/ParameterType::INTEGER,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(seed)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"pixelsamples", pixelsamples_parameter}, {"seed", seed_parameter}};

  IndependentSampler actual;
  EXPECT_TRUE(
      RemoveIndependentSampler(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                pixelsamples: 1 seed: 2
              )pb"));
}

TEST(RemoveMaxMinDistSampler, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  MaxMinDistSampler actual;
  EXPECT_TRUE(
      RemoveMaxMinDistSampler(parameters, /*pbrt_version=*/3, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveMaxMinDistSampler, WithData) {
  std::vector<int32_t> pixelsamples = {1};
  Parameter pixelsamples_parameter{/*directive=*/"",
                                   /*type=*/ParameterType::INTEGER,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(pixelsamples)};

  std::vector<int32_t> dimensions = {2};
  Parameter dimensions_parameter{/*directive=*/"",
                                 /*type=*/ParameterType::INTEGER,
                                 /*type_name=*/"",
                                 /*values=*/absl::MakeSpan(dimensions)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"pixelsamples", pixelsamples_parameter},
      {"dimensions", dimensions_parameter}};

  MaxMinDistSampler actual;
  EXPECT_TRUE(
      RemoveMaxMinDistSampler(parameters, /*pbrt_version=*/3, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                pixelsamples: 1 dimensions: 2
              )pb"));
}

TEST(RemovePaddedSobolSampler, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  PaddedSobolSampler actual;
  EXPECT_TRUE(
      RemovePaddedSobolSampler(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemovePaddedSobolSampler, BadRandomization) {
  std::vector<absl::string_view> randomization = {"unknown"};
  Parameter randomization_parameter{/*directive=*/"",
                                    /*type=*/ParameterType::STRING,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(randomization)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"randomization", randomization_parameter}};

  PaddedSobolSampler actual;
  EXPECT_FALSE(
      RemovePaddedSobolSampler(parameters, /*pbrt_version=*/4, actual).ok());
}

TEST(RemovePaddedSobolSampler, NoneRandomization) {
  std::vector<absl::string_view> randomization = {"none"};
  Parameter randomization_parameter{/*directive=*/"",
                                    /*type=*/ParameterType::STRING,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(randomization)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"randomization", randomization_parameter}};

  PaddedSobolSampler actual;
  EXPECT_TRUE(
      RemovePaddedSobolSampler(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                randomization: NONE
              )pb"));
}

TEST(RemovePaddedSobolSampler, PermuteDigitsRandomization) {
  std::vector<absl::string_view> randomization = {"permutedigits"};
  Parameter randomization_parameter{/*directive=*/"",
                                    /*type=*/ParameterType::STRING,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(randomization)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"randomization", randomization_parameter}};

  PaddedSobolSampler actual;
  EXPECT_TRUE(
      RemovePaddedSobolSampler(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                randomization: PERMUTEDIGITS
              )pb"));
}

TEST(RemovePaddedSobolSampler, OwenRandomization) {
  std::vector<absl::string_view> randomization = {"owen"};
  Parameter randomization_parameter{/*directive=*/"",
                                    /*type=*/ParameterType::STRING,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(randomization)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"randomization", randomization_parameter}};

  PaddedSobolSampler actual;
  EXPECT_TRUE(
      RemovePaddedSobolSampler(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                randomization: OWEN
              )pb"));
}

TEST(RemovePaddedSobolSampler, FastOwenRandomization) {
  std::vector<absl::string_view> randomization = {"fastowen"};
  Parameter randomization_parameter{/*directive=*/"",
                                    /*type=*/ParameterType::STRING,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(randomization)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"randomization", randomization_parameter}};

  PaddedSobolSampler actual;
  EXPECT_TRUE(
      RemovePaddedSobolSampler(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                randomization: FASTOWEN
              )pb"));
}

TEST(RemovePaddedSobolSampler, WithData) {
  std::vector<int32_t> pixelsamples = {1};
  Parameter pixelsamples_parameter{/*directive=*/"",
                                   /*type=*/ParameterType::INTEGER,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(pixelsamples)};

  std::vector<int32_t> seed = {2};
  Parameter seed_parameter{/*directive=*/"",
                           /*type=*/ParameterType::INTEGER,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(seed)};

  std::vector<absl::string_view> randomization = {"fastowen"};
  Parameter randomization_parameter{/*directive=*/"",
                                    /*type=*/ParameterType::STRING,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(randomization)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"pixelsamples", pixelsamples_parameter},
      {"seed", seed_parameter},
      {"randomization", randomization_parameter}};

  PaddedSobolSampler actual;
  EXPECT_TRUE(
      RemovePaddedSobolSampler(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(
                pixelsamples: 1 seed: 2 randomization: FASTOWEN
              )pb"));
}

TEST(RemovePMJ02BNSampler, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  PMJ02BNSampler actual;
  EXPECT_TRUE(
      RemovePMJ02BNSampler(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemovePMJ02BNSampler, WithData) {
  std::vector<int32_t> pixelsamples = {1};
  Parameter pixelsamples_parameter{/*directive=*/"",
                                   /*type=*/ParameterType::INTEGER,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(pixelsamples)};

  std::vector<int32_t> seed = {2};
  Parameter seed_parameter{/*directive=*/"",
                           /*type=*/ParameterType::INTEGER,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(seed)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"pixelsamples", pixelsamples_parameter}, {"seed", seed_parameter}};

  PMJ02BNSampler actual;
  EXPECT_TRUE(
      RemovePMJ02BNSampler(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(pixelsamples: 1 seed: 2)pb"));
}

TEST(RemoveRandomSampler, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  RandomSampler actual;
  EXPECT_TRUE(RemoveRandomSampler(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveRandomSampler, WithData) {
  std::vector<int32_t> xsamples = {1};
  Parameter xsamples_parameter{/*directive=*/"",
                               /*type=*/ParameterType::INTEGER,
                               /*type_name=*/"",
                               /*values=*/absl::MakeSpan(xsamples)};

  std::vector<int32_t> ysamples = {2};
  Parameter ysamples_parameter{/*directive=*/"",
                               /*type=*/ParameterType::INTEGER,
                               /*type_name=*/"",
                               /*values=*/absl::MakeSpan(ysamples)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"xsamples", xsamples_parameter}, {"ysamples", ysamples_parameter}};

  RandomSampler actual;
  EXPECT_TRUE(RemoveRandomSampler(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                xsamples: 1 ysamples: 2
              )pb"));
}

TEST(RemoveSobolSamplerV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  SobolSampler actual;
  EXPECT_TRUE(RemoveSobolSampler(parameters, /*pbrt_version=*/3, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveSobolSamplerV3, WithData) {
  std::vector<int32_t> pixelsamples = {1};
  Parameter pixelsamples_parameter{/*directive=*/"",
                                   /*type=*/ParameterType::INTEGER,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(pixelsamples)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"pixelsamples", pixelsamples_parameter}};

  SobolSampler actual;
  EXPECT_TRUE(RemoveSobolSampler(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                pixelsamples: 1
              )pb"));
}

TEST(RemoveSobolSamplerV4, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  SobolSampler actual;
  EXPECT_TRUE(RemoveSobolSampler(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveSobolSamplerV4, BadRandomization) {
  std::vector<absl::string_view> randomization = {"unknown"};
  Parameter randomization_parameter{/*directive=*/"",
                                    /*type=*/ParameterType::STRING,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(randomization)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"randomization", randomization_parameter}};

  SobolSampler actual;
  EXPECT_FALSE(RemoveSobolSampler(parameters, /*pbrt_version=*/4, actual).ok());
}

TEST(RemoveSobolSamplerV4, NoneRandomization) {
  std::vector<absl::string_view> randomization = {"none"};
  Parameter randomization_parameter{/*directive=*/"",
                                    /*type=*/ParameterType::STRING,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(randomization)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"randomization", randomization_parameter}};

  SobolSampler actual;
  EXPECT_TRUE(RemoveSobolSampler(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                randomization: NONE
              )pb"));
}

TEST(RemoveSobolSamplerV4, PermuteDigitsRandomization) {
  std::vector<absl::string_view> randomization = {"permutedigits"};
  Parameter randomization_parameter{/*directive=*/"",
                                    /*type=*/ParameterType::STRING,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(randomization)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"randomization", randomization_parameter}};

  SobolSampler actual;
  EXPECT_TRUE(RemoveSobolSampler(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                randomization: PERMUTEDIGITS
              )pb"));
}

TEST(RemoveSobolSamplerV4, OwenRandomization) {
  std::vector<absl::string_view> randomization = {"owen"};
  Parameter randomization_parameter{/*directive=*/"",
                                    /*type=*/ParameterType::STRING,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(randomization)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"randomization", randomization_parameter}};

  SobolSampler actual;
  EXPECT_TRUE(RemoveSobolSampler(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                randomization: OWEN
              )pb"));
}

TEST(RemoveSobolSamplerV4, FastOwenRandomization) {
  std::vector<absl::string_view> randomization = {"fastowen"};
  Parameter randomization_parameter{/*directive=*/"",
                                    /*type=*/ParameterType::STRING,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(randomization)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"randomization", randomization_parameter}};

  SobolSampler actual;
  EXPECT_TRUE(RemoveSobolSampler(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                randomization: FASTOWEN
              )pb"));
}

TEST(RemoveSobolSamplerV4, WithData) {
  std::vector<int32_t> pixelsamples = {1};
  Parameter pixelsamples_parameter{/*directive=*/"",
                                   /*type=*/ParameterType::INTEGER,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(pixelsamples)};

  std::vector<int32_t> seed = {2};
  Parameter seed_parameter{/*directive=*/"",
                           /*type=*/ParameterType::INTEGER,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(seed)};

  std::vector<absl::string_view> randomization = {"fastowen"};
  Parameter randomization_parameter{/*directive=*/"",
                                    /*type=*/ParameterType::STRING,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(randomization)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"pixelsamples", pixelsamples_parameter},
      {"seed", seed_parameter},
      {"randomization", randomization_parameter}};

  SobolSampler actual;
  EXPECT_TRUE(RemoveSobolSampler(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(
                pixelsamples: 1 seed: 2 randomization: FASTOWEN
              )pb"));
}

TEST(RemoveStratifiedSamplerV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  StratifiedSampler actual;
  EXPECT_TRUE(
      RemoveStratifiedSampler(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveStratifiedSamplerV1, WithData) {
  std::vector<int32_t> xsamples = {1};
  Parameter xsamples_parameter{/*directive=*/"",
                               /*type=*/ParameterType::INTEGER,
                               /*type_name=*/"",
                               /*values=*/absl::MakeSpan(xsamples)};

  std::vector<int32_t> ysamples = {2};
  Parameter ysamples_parameter{/*directive=*/"",
                               /*type=*/ParameterType::INTEGER,
                               /*type_name=*/"",
                               /*values=*/absl::MakeSpan(ysamples)};

  bool jitter[] = {true};
  Parameter jitter_parameter{/*directive=*/"",
                             /*type=*/ParameterType::BOOL,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(jitter)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"xsamples", xsamples_parameter},
      {"ysamples", ysamples_parameter},
      {"jitter", jitter_parameter}};

  StratifiedSampler actual;
  EXPECT_TRUE(
      RemoveStratifiedSampler(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                xsamples: 1 ysamples: 2 jitter: true
              )pb"));
}

TEST(RemoveStratifiedSamplerV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  StratifiedSampler actual;
  EXPECT_TRUE(
      RemoveStratifiedSampler(parameters, /*pbrt_version=*/2, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveStratifiedSamplerV2, WithData) {
  std::vector<int32_t> xsamples = {1};
  Parameter xsamples_parameter{/*directive=*/"",
                               /*type=*/ParameterType::INTEGER,
                               /*type_name=*/"",
                               /*values=*/absl::MakeSpan(xsamples)};

  std::vector<int32_t> ysamples = {2};
  Parameter ysamples_parameter{/*directive=*/"",
                               /*type=*/ParameterType::INTEGER,
                               /*type_name=*/"",
                               /*values=*/absl::MakeSpan(ysamples)};

  bool jitter[] = {true};
  Parameter jitter_parameter{/*directive=*/"",
                             /*type=*/ParameterType::BOOL,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(jitter)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"xsamples", xsamples_parameter},
      {"ysamples", ysamples_parameter},
      {"jitter", jitter_parameter}};

  StratifiedSampler actual;
  EXPECT_TRUE(
      RemoveStratifiedSampler(parameters, /*pbrt_version=*/2, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                xsamples: 1 ysamples: 2 jitter: true
              )pb"));
}

TEST(RemoveStratifiedSamplerV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  StratifiedSampler actual;
  EXPECT_TRUE(
      RemoveStratifiedSampler(parameters, /*pbrt_version=*/3, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveStratifiedSamplerV3, WithData) {
  std::vector<int32_t> xsamples = {1};
  Parameter xsamples_parameter{/*directive=*/"",
                               /*type=*/ParameterType::INTEGER,
                               /*type_name=*/"",
                               /*values=*/absl::MakeSpan(xsamples)};

  std::vector<int32_t> ysamples = {2};
  Parameter ysamples_parameter{/*directive=*/"",
                               /*type=*/ParameterType::INTEGER,
                               /*type_name=*/"",
                               /*values=*/absl::MakeSpan(ysamples)};

  bool jitter[] = {true};
  Parameter jitter_parameter{/*directive=*/"",
                             /*type=*/ParameterType::BOOL,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(jitter)};

  std::vector<int32_t> dimensions = {3};
  Parameter dimensions_parameter{/*directive=*/"",
                                 /*type=*/ParameterType::INTEGER,
                                 /*type_name=*/"",
                                 /*values=*/absl::MakeSpan(dimensions)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"xsamples", xsamples_parameter},
      {"ysamples", ysamples_parameter},
      {"jitter", jitter_parameter},
      {"dimensions", dimensions_parameter}};

  StratifiedSampler actual;
  EXPECT_TRUE(
      RemoveStratifiedSampler(parameters, /*pbrt_version=*/3, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                xsamples: 1
                ysamples: 2
                jitter: true
                dimensions: 3
              )pb"));
}

TEST(RemoveStratifiedSamplerV4, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  StratifiedSampler actual;
  EXPECT_TRUE(
      RemoveStratifiedSampler(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveStratifiedSamplerV4, WithData) {
  std::vector<int32_t> xsamples = {1};
  Parameter xsamples_parameter{/*directive=*/"",
                               /*type=*/ParameterType::INTEGER,
                               /*type_name=*/"",
                               /*values=*/absl::MakeSpan(xsamples)};

  std::vector<int32_t> ysamples = {2};
  Parameter ysamples_parameter{/*directive=*/"",
                               /*type=*/ParameterType::INTEGER,
                               /*type_name=*/"",
                               /*values=*/absl::MakeSpan(ysamples)};

  bool jitter[] = {true};
  Parameter jitter_parameter{/*directive=*/"",
                             /*type=*/ParameterType::BOOL,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(jitter)};

  std::vector<int32_t> seed = {3};
  Parameter seed_parameter{/*directive=*/"",
                           /*type=*/ParameterType::INTEGER,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(seed)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"xsamples", xsamples_parameter},
      {"ysamples", ysamples_parameter},
      {"jitter", jitter_parameter},
      {"seed", seed_parameter}};

  StratifiedSampler actual;
  EXPECT_TRUE(
      RemoveStratifiedSampler(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(
                xsamples: 1 ysamples: 2 jitter: true seed: 3
              )pb"));
}

TEST(RemoveZeroTwoSequenceSamplerV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  ZeroTwoSequenceSampler actual;
  EXPECT_TRUE(
      RemoveZeroTwoSequenceSampler(parameters, /*pbrt_version=*/1, actual)
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveZeroTwoSequenceSamplerV1, WithData) {
  std::vector<int32_t> pixelsamples = {1};
  Parameter pixelsamples_parameter{/*directive=*/"",
                                   /*type=*/ParameterType::INTEGER,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(pixelsamples)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"pixelsamples", pixelsamples_parameter}};

  ZeroTwoSequenceSampler actual;
  EXPECT_TRUE(
      RemoveZeroTwoSequenceSampler(parameters, /*pbrt_version=*/1, actual)
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                pixelsamples: 1
              )pb"));
}

TEST(RemoveZeroTwoSequenceSamplerV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  ZeroTwoSequenceSampler actual;
  EXPECT_TRUE(
      RemoveZeroTwoSequenceSampler(parameters, /*pbrt_version=*/2, actual)
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveZeroTwoSequenceSamplerV2, WithData) {
  std::vector<int32_t> pixelsamples = {1};
  Parameter pixelsamples_parameter{/*directive=*/"",
                                   /*type=*/ParameterType::INTEGER,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(pixelsamples)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"pixelsamples", pixelsamples_parameter}};

  ZeroTwoSequenceSampler actual;
  EXPECT_TRUE(
      RemoveZeroTwoSequenceSampler(parameters, /*pbrt_version=*/2, actual)
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                pixelsamples: 1
              )pb"));
}

TEST(RemoveZeroTwoSequenceSamplerV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  ZeroTwoSequenceSampler actual;
  EXPECT_TRUE(
      RemoveZeroTwoSequenceSampler(parameters, /*pbrt_version=*/3, actual)
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveZeroTwoSequenceSamplerV3, WithData) {
  std::vector<int32_t> pixelsamples = {1};
  Parameter pixelsamples_parameter{/*directive=*/"",
                                   /*type=*/ParameterType::INTEGER,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(pixelsamples)};

  std::vector<int32_t> dimensions = {2};
  Parameter dimensions_parameter{/*directive=*/"",
                                 /*type=*/ParameterType::INTEGER,
                                 /*type_name=*/"",
                                 /*values=*/absl::MakeSpan(dimensions)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"pixelsamples", pixelsamples_parameter},
      {"dimensions", dimensions_parameter}};

  ZeroTwoSequenceSampler actual;
  EXPECT_TRUE(
      RemoveZeroTwoSequenceSampler(parameters, /*pbrt_version=*/3, actual)
          .ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                pixelsamples: 1 dimensions: 2
              )pb"));
}

TEST(RemoveZSobolSampler, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  ZSobolSampler actual;
  EXPECT_TRUE(RemoveZSobolSampler(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveZSobolSampler, BadRandomization) {
  std::vector<absl::string_view> randomization = {"unknown"};
  Parameter randomization_parameter{/*directive=*/"",
                                    /*type=*/ParameterType::STRING,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(randomization)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"randomization", randomization_parameter}};

  ZSobolSampler actual;
  EXPECT_FALSE(
      RemoveZSobolSampler(parameters, /*pbrt_version=*/4, actual).ok());
}

TEST(RemoveZSobolSampler, NoneRandomization) {
  std::vector<absl::string_view> randomization = {"none"};
  Parameter randomization_parameter{/*directive=*/"",
                                    /*type=*/ParameterType::STRING,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(randomization)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"randomization", randomization_parameter}};

  ZSobolSampler actual;
  EXPECT_TRUE(RemoveZSobolSampler(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                randomization: NONE
              )pb"));
}

TEST(RemoveZSobolSampler, PermuteDigitsRandomization) {
  std::vector<absl::string_view> randomization = {"permutedigits"};
  Parameter randomization_parameter{/*directive=*/"",
                                    /*type=*/ParameterType::STRING,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(randomization)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"randomization", randomization_parameter}};

  ZSobolSampler actual;
  EXPECT_TRUE(RemoveZSobolSampler(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                randomization: PERMUTEDIGITS
              )pb"));
}

TEST(RemoveZSobolSampler, OwenRandomization) {
  std::vector<absl::string_view> randomization = {"owen"};
  Parameter randomization_parameter{/*directive=*/"",
                                    /*type=*/ParameterType::STRING,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(randomization)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"randomization", randomization_parameter}};

  ZSobolSampler actual;
  EXPECT_TRUE(RemoveZSobolSampler(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                randomization: OWEN
              )pb"));
}

TEST(RemoveZSobolSampler, FastOwenRandomization) {
  std::vector<absl::string_view> randomization = {"fastowen"};
  Parameter randomization_parameter{/*directive=*/"",
                                    /*type=*/ParameterType::STRING,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(randomization)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"randomization", randomization_parameter}};

  ZSobolSampler actual;
  EXPECT_TRUE(RemoveZSobolSampler(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                randomization: FASTOWEN
              )pb"));
}

TEST(RemoveZSobolSampler, WithData) {
  std::vector<int32_t> pixelsamples = {1};
  Parameter pixelsamples_parameter{/*directive=*/"",
                                   /*type=*/ParameterType::INTEGER,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(pixelsamples)};

  std::vector<int32_t> seed = {2};
  Parameter seed_parameter{/*directive=*/"",
                           /*type=*/ParameterType::INTEGER,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(seed)};

  std::vector<absl::string_view> randomization = {"fastowen"};
  Parameter randomization_parameter{/*directive=*/"",
                                    /*type=*/ParameterType::STRING,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(randomization)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"pixelsamples", pixelsamples_parameter},
      {"seed", seed_parameter},
      {"randomization", randomization_parameter}};

  ZSobolSampler actual;
  EXPECT_TRUE(RemoveZSobolSampler(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(
                pixelsamples: 1 seed: 2 randomization: FASTOWEN
              )pb"));
}

}  // namespace
}  // namespace pbrt_proto
