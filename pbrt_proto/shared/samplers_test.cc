#include "pbrt_proto/shared/samplers.h"

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

TEST(RemoveBestCandidateSampler, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  BestCandidateSampler actual;
  RemoveBestCandidateSampler(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveBestCandidateSampler, WithData) {
  std::vector<int32_t> pixelsamples = {1};
  Parameter pixelsamples_parameter{.directive = "",
                                   .type = ParameterType::INTEGER,
                                   .type_name = "",
                                   .values = absl::MakeSpan(pixelsamples)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"pixelsamples", pixelsamples_parameter}};

  BestCandidateSampler actual;
  RemoveBestCandidateSampler(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                pixelsamples: 1
              )pb"));
}

TEST(RemoveHaltonSamplerV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  HaltonSampler actual;
  RemoveHaltonSamplerV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveHaltonSamplerV1, WithData) {
  std::vector<int32_t> pixelsamples = {1};
  Parameter pixelsamples_parameter{.directive = "",
                                   .type = ParameterType::INTEGER,
                                   .type_name = "",
                                   .values = absl::MakeSpan(pixelsamples)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"pixelsamples", pixelsamples_parameter}};

  HaltonSampler actual;
  RemoveHaltonSamplerV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                pixelsamples: 1
              )pb"));
}

TEST(RemoveHaltonSamplerV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  HaltonSampler actual;
  RemoveHaltonSamplerV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveHaltonSamplerV2, WithData) {
  std::vector<int32_t> pixelsamples = {1};
  Parameter pixelsamples_parameter{.directive = "",
                                   .type = ParameterType::INTEGER,
                                   .type_name = "",
                                   .values = absl::MakeSpan(pixelsamples)};

  bool samplepixelcenter[] = {true};
  Parameter samplepixelcenter_parameter{
      .directive = "",
      .type = ParameterType::BOOL,
      .type_name = "",
      .values = absl::MakeSpan(samplepixelcenter)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"pixelsamples", pixelsamples_parameter},
      {"samplepixelcenter", samplepixelcenter_parameter}};

  HaltonSampler actual;
  RemoveHaltonSamplerV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                pixelsamples: 1 samplepixelcenter: true
              )pb"));
}

TEST(RemoveHaltonSamplerV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  HaltonSampler actual;
  EXPECT_TRUE(RemoveHaltonSamplerV3(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveHaltonSamplerV3, WithData) {
  std::vector<int32_t> pixelsamples = {1};
  Parameter pixelsamples_parameter{.directive = "",
                                   .type = ParameterType::INTEGER,
                                   .type_name = "",
                                   .values = absl::MakeSpan(pixelsamples)};

  std::vector<int32_t> seed = {2};
  Parameter seed_parameter{.directive = "",
                           .type = ParameterType::INTEGER,
                           .type_name = "",
                           .values = absl::MakeSpan(seed)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"pixelsamples", pixelsamples_parameter}, {"seed", seed_parameter}};

  HaltonSampler actual;
  EXPECT_TRUE(RemoveHaltonSamplerV3(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                pixelsamples: 1 seed: 2
              )pb"));
}

TEST(RemoveIndependentSamplerV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  IndependentSampler actual;
  RemoveIndependentSamplerV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveIndependentSamplerV1, WithData) {
  std::vector<int32_t> pixelsamples = {1};
  Parameter pixelsamples_parameter{.directive = "",
                                   .type = ParameterType::INTEGER,
                                   .type_name = "",
                                   .values = absl::MakeSpan(pixelsamples)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"pixelsamples", pixelsamples_parameter}};

  IndependentSampler actual;
  RemoveIndependentSamplerV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                pixelsamples: 1
              )pb"));
}

TEST(RemoveIndependentSamplerV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  IndependentSampler actual;
  RemoveIndependentSamplerV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveIndependentSamplerV2, WithData) {
  std::vector<int32_t> pixelsamples = {1};
  Parameter pixelsamples_parameter{.directive = "",
                                   .type = ParameterType::INTEGER,
                                   .type_name = "",
                                   .values = absl::MakeSpan(pixelsamples)};

  std::vector<int32_t> seed = {2};
  Parameter seed_parameter{.directive = "",
                           .type = ParameterType::INTEGER,
                           .type_name = "",
                           .values = absl::MakeSpan(seed)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"pixelsamples", pixelsamples_parameter}, {"seed", seed_parameter}};

  IndependentSampler actual;
  RemoveIndependentSamplerV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                pixelsamples: 1 seed: 2
              )pb"));
}

TEST(RemovePaddedSobolSampler, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  PaddedSobolSampler actual;
  EXPECT_TRUE(RemovePaddedSobolSampler(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemovePaddedSobolSampler, BadRandomization) {
  std::vector<absl::string_view> randomization = {"unknown"};
  Parameter randomization_parameter{.directive = "",
                                    .type = ParameterType::STRING,
                                    .type_name = "",
                                    .values = absl::MakeSpan(randomization)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"randomization", randomization_parameter}};

  PaddedSobolSampler actual;
  EXPECT_FALSE(RemovePaddedSobolSampler(parameters, actual).ok());
}

TEST(RemovePaddedSobolSampler, NoneRandomization) {
  std::vector<absl::string_view> randomization = {"none"};
  Parameter randomization_parameter{.directive = "",
                                    .type = ParameterType::STRING,
                                    .type_name = "",
                                    .values = absl::MakeSpan(randomization)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"randomization", randomization_parameter}};

  PaddedSobolSampler actual;
  EXPECT_TRUE(RemovePaddedSobolSampler(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                randomization: NONE
              )pb"));
}

TEST(RemovePaddedSobolSampler, PermuteDigitsRandomization) {
  std::vector<absl::string_view> randomization = {"permutedigits"};
  Parameter randomization_parameter{.directive = "",
                                    .type = ParameterType::STRING,
                                    .type_name = "",
                                    .values = absl::MakeSpan(randomization)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"randomization", randomization_parameter}};

  PaddedSobolSampler actual;
  EXPECT_TRUE(RemovePaddedSobolSampler(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                randomization: PERMUTEDIGITS
              )pb"));
}

TEST(RemovePaddedSobolSampler, OwenRandomization) {
  std::vector<absl::string_view> randomization = {"owen"};
  Parameter randomization_parameter{.directive = "",
                                    .type = ParameterType::STRING,
                                    .type_name = "",
                                    .values = absl::MakeSpan(randomization)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"randomization", randomization_parameter}};

  PaddedSobolSampler actual;
  EXPECT_TRUE(RemovePaddedSobolSampler(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                randomization: OWEN
              )pb"));
}

TEST(RemovePaddedSobolSampler, FastOwenRandomization) {
  std::vector<absl::string_view> randomization = {"fastowen"};
  Parameter randomization_parameter{.directive = "",
                                    .type = ParameterType::STRING,
                                    .type_name = "",
                                    .values = absl::MakeSpan(randomization)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"randomization", randomization_parameter}};

  PaddedSobolSampler actual;
  EXPECT_TRUE(RemovePaddedSobolSampler(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                randomization: FASTOWEN
              )pb"));
}

TEST(RemovePaddedSobolSampler, WithData) {
  std::vector<int32_t> pixelsamples = {1};
  Parameter pixelsamples_parameter{.directive = "",
                                   .type = ParameterType::INTEGER,
                                   .type_name = "",
                                   .values = absl::MakeSpan(pixelsamples)};

  std::vector<int32_t> seed = {2};
  Parameter seed_parameter{.directive = "",
                           .type = ParameterType::INTEGER,
                           .type_name = "",
                           .values = absl::MakeSpan(seed)};

  std::vector<absl::string_view> randomization = {"fastowen"};
  Parameter randomization_parameter{.directive = "",
                                    .type = ParameterType::STRING,
                                    .type_name = "",
                                    .values = absl::MakeSpan(randomization)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"pixelsamples", pixelsamples_parameter},
      {"seed", seed_parameter},
      {"randomization", randomization_parameter}};

  PaddedSobolSampler actual;
  EXPECT_TRUE(RemovePaddedSobolSampler(parameters, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(
                pixelsamples: 1 seed: 2 randomization: FASTOWEN
              )pb"));
}

TEST(RemovePMJ02BNSampler, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  PMJ02BNSampler actual;
  RemovePMJ02BNSampler(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemovePMJ02BNSampler, WithData) {
  std::vector<int32_t> pixelsamples = {1};
  Parameter pixelsamples_parameter{.directive = "",
                                   .type = ParameterType::INTEGER,
                                   .type_name = "",
                                   .values = absl::MakeSpan(pixelsamples)};

  std::vector<int32_t> seed = {2};
  Parameter seed_parameter{.directive = "",
                           .type = ParameterType::INTEGER,
                           .type_name = "",
                           .values = absl::MakeSpan(seed)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"pixelsamples", pixelsamples_parameter}, {"seed", seed_parameter}};

  PMJ02BNSampler actual;
  RemovePMJ02BNSampler(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(pixelsamples: 1 seed: 2)pb"));
}

TEST(RemoveSobolSamplerV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  SobolSampler actual;
  RemoveSobolSamplerV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveSobolSamplerV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  SobolSampler actual;
  EXPECT_TRUE(RemoveSobolSamplerV2(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveSobolSamplerV2, BadRandomization) {
  std::vector<absl::string_view> randomization = {"unknown"};
  Parameter randomization_parameter{.directive = "",
                                    .type = ParameterType::STRING,
                                    .type_name = "",
                                    .values = absl::MakeSpan(randomization)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"randomization", randomization_parameter}};

  SobolSampler actual;
  EXPECT_FALSE(RemoveSobolSamplerV2(parameters, actual).ok());
}

TEST(RemoveSobolSamplerV2, NoneRandomization) {
  std::vector<absl::string_view> randomization = {"none"};
  Parameter randomization_parameter{.directive = "",
                                    .type = ParameterType::STRING,
                                    .type_name = "",
                                    .values = absl::MakeSpan(randomization)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"randomization", randomization_parameter}};

  SobolSampler actual;
  EXPECT_TRUE(RemoveSobolSamplerV2(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                randomization: NONE
              )pb"));
}

TEST(RemoveSobolSamplerV2, PermuteDigitsRandomization) {
  std::vector<absl::string_view> randomization = {"permutedigits"};
  Parameter randomization_parameter{.directive = "",
                                    .type = ParameterType::STRING,
                                    .type_name = "",
                                    .values = absl::MakeSpan(randomization)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"randomization", randomization_parameter}};

  SobolSampler actual;
  EXPECT_TRUE(RemoveSobolSamplerV2(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                randomization: PERMUTEDIGITS
              )pb"));
}

TEST(RemoveSobolSamplerV2, OwenRandomization) {
  std::vector<absl::string_view> randomization = {"owen"};
  Parameter randomization_parameter{.directive = "",
                                    .type = ParameterType::STRING,
                                    .type_name = "",
                                    .values = absl::MakeSpan(randomization)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"randomization", randomization_parameter}};

  SobolSampler actual;
  EXPECT_TRUE(RemoveSobolSamplerV2(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                randomization: OWEN
              )pb"));
}

TEST(RemoveSobolSamplerV2, FastOwenRandomization) {
  std::vector<absl::string_view> randomization = {"fastowen"};
  Parameter randomization_parameter{.directive = "",
                                    .type = ParameterType::STRING,
                                    .type_name = "",
                                    .values = absl::MakeSpan(randomization)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"randomization", randomization_parameter}};

  SobolSampler actual;
  EXPECT_TRUE(RemoveSobolSamplerV2(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                randomization: FASTOWEN
              )pb"));
}

TEST(RemoveSobolSamplerV2, WithData) {
  std::vector<int32_t> pixelsamples = {1};
  Parameter pixelsamples_parameter{.directive = "",
                                   .type = ParameterType::INTEGER,
                                   .type_name = "",
                                   .values = absl::MakeSpan(pixelsamples)};

  std::vector<int32_t> seed = {2};
  Parameter seed_parameter{.directive = "",
                           .type = ParameterType::INTEGER,
                           .type_name = "",
                           .values = absl::MakeSpan(seed)};

  std::vector<absl::string_view> randomization = {"fastowen"};
  Parameter randomization_parameter{.directive = "",
                                    .type = ParameterType::STRING,
                                    .type_name = "",
                                    .values = absl::MakeSpan(randomization)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"pixelsamples", pixelsamples_parameter},
      {"seed", seed_parameter},
      {"randomization", randomization_parameter}};

  SobolSampler actual;
  EXPECT_TRUE(RemoveSobolSamplerV2(parameters, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(
                pixelsamples: 1 seed: 2 randomization: FASTOWEN
              )pb"));
}

TEST(RemoveStratifiedSamplerV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  StratifiedSampler actual;
  RemoveStratifiedSamplerV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveStratifiedSamplerV1, WithData) {
  std::vector<int32_t> xsamples = {1};
  Parameter xsamples_parameter{.directive = "",
                               .type = ParameterType::INTEGER,
                               .type_name = "",
                               .values = absl::MakeSpan(xsamples)};

  std::vector<int32_t> ysamples = {2};
  Parameter ysamples_parameter{.directive = "",
                               .type = ParameterType::INTEGER,
                               .type_name = "",
                               .values = absl::MakeSpan(ysamples)};

  bool jitter[] = {true};
  Parameter jitter_parameter{.directive = "",
                             .type = ParameterType::BOOL,
                             .type_name = "",
                             .values = absl::MakeSpan(jitter)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"xsamples", xsamples_parameter},
      {"ysamples", ysamples_parameter},
      {"jitter", jitter_parameter}};

  StratifiedSampler actual;
  RemoveStratifiedSamplerV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                xsamples: 1 ysamples: 2 jitter: true
              )pb"));
}

TEST(RemoveStratifiedSamplerV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  StratifiedSampler actual;
  RemoveStratifiedSamplerV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveStratifiedSamplerV2, WithData) {
  std::vector<int32_t> xsamples = {1};
  Parameter xsamples_parameter{.directive = "",
                               .type = ParameterType::INTEGER,
                               .type_name = "",
                               .values = absl::MakeSpan(xsamples)};

  std::vector<int32_t> ysamples = {2};
  Parameter ysamples_parameter{.directive = "",
                               .type = ParameterType::INTEGER,
                               .type_name = "",
                               .values = absl::MakeSpan(ysamples)};

  bool jitter[] = {true};
  Parameter jitter_parameter{.directive = "",
                             .type = ParameterType::BOOL,
                             .type_name = "",
                             .values = absl::MakeSpan(jitter)};

  std::vector<int32_t> dimensions = {3};
  Parameter dimensions_parameter{.directive = "",
                                 .type = ParameterType::INTEGER,
                                 .type_name = "",
                                 .values = absl::MakeSpan(dimensions)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"xsamples", xsamples_parameter},
      {"ysamples", ysamples_parameter},
      {"jitter", jitter_parameter},
      {"dimensions", dimensions_parameter}};

  StratifiedSampler actual;
  RemoveStratifiedSamplerV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                xsamples: 1
                ysamples: 2
                jitter: true
                dimensions: 3
              )pb"));
}

TEST(RemoveStratifiedSamplerV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  StratifiedSampler actual;
  RemoveStratifiedSamplerV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveStratifiedSamplerV3, WithData) {
  std::vector<int32_t> xsamples = {1};
  Parameter xsamples_parameter{.directive = "",
                               .type = ParameterType::INTEGER,
                               .type_name = "",
                               .values = absl::MakeSpan(xsamples)};

  std::vector<int32_t> ysamples = {2};
  Parameter ysamples_parameter{.directive = "",
                               .type = ParameterType::INTEGER,
                               .type_name = "",
                               .values = absl::MakeSpan(ysamples)};

  bool jitter[] = {true};
  Parameter jitter_parameter{.directive = "",
                             .type = ParameterType::BOOL,
                             .type_name = "",
                             .values = absl::MakeSpan(jitter)};

  std::vector<int32_t> seed = {3};
  Parameter seed_parameter{.directive = "",
                           .type = ParameterType::INTEGER,
                           .type_name = "",
                           .values = absl::MakeSpan(seed)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"xsamples", xsamples_parameter},
      {"ysamples", ysamples_parameter},
      {"jitter", jitter_parameter},
      {"seed", seed_parameter}};

  StratifiedSampler actual;
  RemoveStratifiedSamplerV3(parameters, actual);
  EXPECT_THAT(actual,
              EqualsProto(R"pb(
                xsamples: 1 ysamples: 2 jitter: true seed: 3
              )pb"));
}

TEST(RemoveZeroTwoSequenceSamplerV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  ZeroTwoSequenceSampler actual;
  RemoveZeroTwoSequenceSamplerV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveZeroTwoSequenceSamplerV1, WithData) {
  std::vector<int32_t> pixelsamples = {1};
  Parameter pixelsamples_parameter{.directive = "",
                                   .type = ParameterType::INTEGER,
                                   .type_name = "",
                                   .values = absl::MakeSpan(pixelsamples)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"pixelsamples", pixelsamples_parameter}};

  ZeroTwoSequenceSampler actual;
  RemoveZeroTwoSequenceSamplerV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                pixelsamples: 1
              )pb"));
}

TEST(RemoveZeroTwoSequenceSamplerV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  ZeroTwoSequenceSampler actual;
  RemoveZeroTwoSequenceSamplerV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveZeroTwoSequenceSamplerV2, WithData) {
  std::vector<int32_t> pixelsamples = {1};
  Parameter pixelsamples_parameter{.directive = "",
                                   .type = ParameterType::INTEGER,
                                   .type_name = "",
                                   .values = absl::MakeSpan(pixelsamples)};

  std::vector<int32_t> dimensions = {2};
  Parameter dimensions_parameter{.directive = "",
                                 .type = ParameterType::INTEGER,
                                 .type_name = "",
                                 .values = absl::MakeSpan(dimensions)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"pixelsamples", pixelsamples_parameter},
      {"dimensions", dimensions_parameter}};

  ZeroTwoSequenceSampler actual;
  RemoveZeroTwoSequenceSamplerV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                pixelsamples: 1 dimensions: 2
              )pb"));
}

TEST(RemoveZSobolSampler, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  ZSobolSampler actual;
  EXPECT_TRUE(RemoveZSobolSampler(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveZSobolSampler, BadRandomization) {
  std::vector<absl::string_view> randomization = {"unknown"};
  Parameter randomization_parameter{.directive = "",
                                    .type = ParameterType::STRING,
                                    .type_name = "",
                                    .values = absl::MakeSpan(randomization)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"randomization", randomization_parameter}};

  ZSobolSampler actual;
  EXPECT_FALSE(RemoveZSobolSampler(parameters, actual).ok());
}

TEST(RemoveZSobolSampler, NoneRandomization) {
  std::vector<absl::string_view> randomization = {"none"};
  Parameter randomization_parameter{.directive = "",
                                    .type = ParameterType::STRING,
                                    .type_name = "",
                                    .values = absl::MakeSpan(randomization)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"randomization", randomization_parameter}};

  ZSobolSampler actual;
  EXPECT_TRUE(RemoveZSobolSampler(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                randomization: NONE
              )pb"));
}

TEST(RemoveZSobolSampler, PermuteDigitsRandomization) {
  std::vector<absl::string_view> randomization = {"permutedigits"};
  Parameter randomization_parameter{.directive = "",
                                    .type = ParameterType::STRING,
                                    .type_name = "",
                                    .values = absl::MakeSpan(randomization)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"randomization", randomization_parameter}};

  ZSobolSampler actual;
  EXPECT_TRUE(RemoveZSobolSampler(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                randomization: PERMUTEDIGITS
              )pb"));
}

TEST(RemoveZSobolSampler, OwenRandomization) {
  std::vector<absl::string_view> randomization = {"owen"};
  Parameter randomization_parameter{.directive = "",
                                    .type = ParameterType::STRING,
                                    .type_name = "",
                                    .values = absl::MakeSpan(randomization)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"randomization", randomization_parameter}};

  ZSobolSampler actual;
  EXPECT_TRUE(RemoveZSobolSampler(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                randomization: OWEN
              )pb"));
}

TEST(RemoveZSobolSampler, FastOwenRandomization) {
  std::vector<absl::string_view> randomization = {"fastowen"};
  Parameter randomization_parameter{.directive = "",
                                    .type = ParameterType::STRING,
                                    .type_name = "",
                                    .values = absl::MakeSpan(randomization)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"randomization", randomization_parameter}};

  ZSobolSampler actual;
  EXPECT_TRUE(RemoveZSobolSampler(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                randomization: FASTOWEN
              )pb"));
}

TEST(RemoveZSobolSampler, WithData) {
  std::vector<int32_t> pixelsamples = {1};
  Parameter pixelsamples_parameter{.directive = "",
                                   .type = ParameterType::INTEGER,
                                   .type_name = "",
                                   .values = absl::MakeSpan(pixelsamples)};

  std::vector<int32_t> seed = {2};
  Parameter seed_parameter{.directive = "",
                           .type = ParameterType::INTEGER,
                           .type_name = "",
                           .values = absl::MakeSpan(seed)};

  std::vector<absl::string_view> randomization = {"fastowen"};
  Parameter randomization_parameter{.directive = "",
                                    .type = ParameterType::STRING,
                                    .type_name = "",
                                    .values = absl::MakeSpan(randomization)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"pixelsamples", pixelsamples_parameter},
      {"seed", seed_parameter},
      {"randomization", randomization_parameter}};

  ZSobolSampler actual;
  EXPECT_TRUE(RemoveZSobolSampler(parameters, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(
                pixelsamples: 1 seed: 2 randomization: FASTOWEN
              )pb"));
}

}  // namespace
}  // namespace pbrt_proto
