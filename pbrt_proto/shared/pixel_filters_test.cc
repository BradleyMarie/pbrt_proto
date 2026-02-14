#include "pbrt_proto/shared/pixel_filters.h"

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

TEST(RemoveBoxPixelFilterV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  BoxPixelFilter actual;
  RemoveBoxPixelFilterV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveBoxPixelFilterV1, WithData) {
  std::vector<double> xwidth = {1.0};
  Parameter xwidth_parameter{.directive = "",
                             .type = ParameterType::FLOAT,
                             .type_name = "",
                             .values = absl::MakeSpan(xwidth)};

  std::vector<double> ywidth = {2.0};
  Parameter ywidth_parameter{.directive = "",
                             .type = ParameterType::FLOAT,
                             .type_name = "",
                             .values = absl::MakeSpan(ywidth)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"xwidth", xwidth_parameter}, {"ywidth", ywidth_parameter}};

  BoxPixelFilter actual;
  RemoveBoxPixelFilterV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                xradius: 1.0 yradius: 2.0
              )pb"));
}

TEST(RemoveBoxPixelFilterV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  BoxPixelFilter actual;
  RemoveBoxPixelFilterV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveBoxPixelFilterV2, WithData) {
  std::vector<double> xradius = {1.0};
  Parameter xradius_parameter{.directive = "",
                              .type = ParameterType::FLOAT,
                              .type_name = "",
                              .values = absl::MakeSpan(xradius)};

  std::vector<double> yradius = {2.0};
  Parameter yradius_parameter{.directive = "",
                              .type = ParameterType::FLOAT,
                              .type_name = "",
                              .values = absl::MakeSpan(yradius)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"xradius", xradius_parameter}, {"yradius", yradius_parameter}};

  BoxPixelFilter actual;
  RemoveBoxPixelFilterV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                xradius: 1.0 yradius: 2.0
              )pb"));
}

TEST(RemoveGaussianPixelFilterV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  GaussianPixelFilter actual;
  RemoveGaussianPixelFilterV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveGaussianPixelFilterV1, WithData) {
  std::vector<double> xwidth = {1.0};
  Parameter xwidth_parameter{.directive = "",
                             .type = ParameterType::FLOAT,
                             .type_name = "",
                             .values = absl::MakeSpan(xwidth)};

  std::vector<double> ywidth = {2.0};
  Parameter ywidth_parameter{.directive = "",
                             .type = ParameterType::FLOAT,
                             .type_name = "",
                             .values = absl::MakeSpan(ywidth)};

  std::vector<double> alpha = {8.0};
  Parameter alpha_parameter{.directive = "",
                            .type = ParameterType::FLOAT,
                            .type_name = "",
                            .values = absl::MakeSpan(alpha)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"xwidth", xwidth_parameter},
      {"ywidth", ywidth_parameter},
      {"alpha", alpha_parameter}};

  GaussianPixelFilter actual;
  RemoveGaussianPixelFilterV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                xradius: 1.0 yradius: 2.0 sigma: 0.25
              )pb"));
}

TEST(RemoveGaussianPixelFilterV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  GaussianPixelFilter actual;
  RemoveGaussianPixelFilterV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveGaussianPixelFilterV2, WithData) {
  std::vector<double> xradius = {1.0};
  Parameter xradius_parameter{.directive = "",
                              .type = ParameterType::FLOAT,
                              .type_name = "",
                              .values = absl::MakeSpan(xradius)};

  std::vector<double> yradius = {2.0};
  Parameter yradius_parameter{.directive = "",
                              .type = ParameterType::FLOAT,
                              .type_name = "",
                              .values = absl::MakeSpan(yradius)};

  std::vector<double> sigma = {3.0};
  Parameter sigma_parameter{.directive = "",
                            .type = ParameterType::FLOAT,
                            .type_name = "",
                            .values = absl::MakeSpan(sigma)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"xradius", xradius_parameter},
      {"yradius", yradius_parameter},
      {"sigma", sigma_parameter}};

  GaussianPixelFilter actual;
  RemoveGaussianPixelFilterV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                xradius: 1.0 yradius: 2.0 sigma: 3.0
              )pb"));
}

TEST(RemoveLanczosPixelFilterV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  LanczosPixelFilter actual;
  RemoveLanczosPixelFilterV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveLanczosPixelFilterV1, WithData) {
  std::vector<double> xwidth = {1.0};
  Parameter xwidth_parameter{.directive = "",
                             .type = ParameterType::FLOAT,
                             .type_name = "",
                             .values = absl::MakeSpan(xwidth)};

  std::vector<double> ywidth = {2.0};
  Parameter ywidth_parameter{.directive = "",
                             .type = ParameterType::FLOAT,
                             .type_name = "",
                             .values = absl::MakeSpan(ywidth)};

  std::vector<double> tau = {3.0};
  Parameter tau_parameter{.directive = "",
                          .type = ParameterType::FLOAT,
                          .type_name = "",
                          .values = absl::MakeSpan(tau)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"xwidth", xwidth_parameter},
      {"ywidth", ywidth_parameter},
      {"tau", tau_parameter}};

  LanczosPixelFilter actual;
  RemoveLanczosPixelFilterV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                xradius: 1.0 yradius: 2.0 tau: 3.0
              )pb"));
}

TEST(RemoveLanczosPixelFilterV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  LanczosPixelFilter actual;
  RemoveLanczosPixelFilterV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveLanczosPixelFilterV2, WithData) {
  std::vector<double> xradius = {1.0};
  Parameter xradius_parameter{.directive = "",
                              .type = ParameterType::FLOAT,
                              .type_name = "",
                              .values = absl::MakeSpan(xradius)};

  std::vector<double> yradius = {2.0};
  Parameter yradius_parameter{.directive = "",
                              .type = ParameterType::FLOAT,
                              .type_name = "",
                              .values = absl::MakeSpan(yradius)};

  std::vector<double> tau = {3.0};
  Parameter tau_parameter{.directive = "",
                          .type = ParameterType::FLOAT,
                          .type_name = "",
                          .values = absl::MakeSpan(tau)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"xradius", xradius_parameter},
      {"yradius", yradius_parameter},
      {"tau", tau_parameter}};

  LanczosPixelFilter actual;
  RemoveLanczosPixelFilterV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                xradius: 1.0 yradius: 2.0 tau: 3.0
              )pb"));
}

TEST(RemoveMitchellPixelFilterV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  MitchellPixelFilter actual;
  RemoveMitchellPixelFilterV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveMitchellPixelFilterV1, WithData) {
  std::vector<double> xwidth = {1.0};
  Parameter xwidth_parameter{.directive = "",
                             .type = ParameterType::FLOAT,
                             .type_name = "",
                             .values = absl::MakeSpan(xwidth)};

  std::vector<double> ywidth = {2.0};
  Parameter ywidth_parameter{.directive = "",
                             .type = ParameterType::FLOAT,
                             .type_name = "",
                             .values = absl::MakeSpan(ywidth)};

  std::vector<double> b = {3.0};
  Parameter b_parameter{.directive = "",
                        .type = ParameterType::FLOAT,
                        .type_name = "",
                        .values = absl::MakeSpan(b)};

  std::vector<double> c = {4.0};
  Parameter c_parameter{.directive = "",
                        .type = ParameterType::FLOAT,
                        .type_name = "",
                        .values = absl::MakeSpan(c)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"xwidth", xwidth_parameter},
      {"ywidth", ywidth_parameter},
      {"B", b_parameter},
      {"C", c_parameter}};

  MitchellPixelFilter actual;
  RemoveMitchellPixelFilterV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                xradius: 1.0 yradius: 2.0 B: 3.0 C: 4.0
              )pb"));
}

TEST(RemoveMitchellPixelFilterV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  MitchellPixelFilter actual;
  RemoveMitchellPixelFilterV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveMitchellPixelFilterV2, WithData) {
  std::vector<double> xradius = {1.0};
  Parameter xradius_parameter{.directive = "",
                              .type = ParameterType::FLOAT,
                              .type_name = "",
                              .values = absl::MakeSpan(xradius)};

  std::vector<double> yradius = {2.0};
  Parameter yradius_parameter{.directive = "",
                              .type = ParameterType::FLOAT,
                              .type_name = "",
                              .values = absl::MakeSpan(yradius)};

  std::vector<double> b = {3.0};
  Parameter b_parameter{.directive = "",
                        .type = ParameterType::FLOAT,
                        .type_name = "",
                        .values = absl::MakeSpan(b)};

  std::vector<double> c = {4.0};
  Parameter c_parameter{.directive = "",
                        .type = ParameterType::FLOAT,
                        .type_name = "",
                        .values = absl::MakeSpan(c)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"xradius", xradius_parameter},
      {"yradius", yradius_parameter},
      {"B", b_parameter},
      {"C", c_parameter}};

  MitchellPixelFilter actual;
  RemoveMitchellPixelFilterV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                xradius: 1.0 yradius: 2.0 B: 3.0 C: 4.0
              )pb"));
}

TEST(RemoveTrianglePixelFilterV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  TrianglePixelFilter actual;
  RemoveTrianglePixelFilterV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveTrianglePixelFilterV1, WithData) {
  std::vector<double> xwidth = {1.0};
  Parameter xwidth_parameter{.directive = "",
                             .type = ParameterType::FLOAT,
                             .type_name = "",
                             .values = absl::MakeSpan(xwidth)};

  std::vector<double> ywidth = {2.0};
  Parameter ywidth_parameter{.directive = "",
                             .type = ParameterType::FLOAT,
                             .type_name = "",
                             .values = absl::MakeSpan(ywidth)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"xwidth", xwidth_parameter}, {"ywidth", ywidth_parameter}};

  TrianglePixelFilter actual;
  RemoveTrianglePixelFilterV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                xradius: 1.0 yradius: 2.0
              )pb"));
}

TEST(RemoveTrianglePixelFilterV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  TrianglePixelFilter actual;
  RemoveTrianglePixelFilterV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveTrianglePixelFilterV2, WithData) {
  std::vector<double> xradius = {1.0};
  Parameter xradius_parameter{.directive = "",
                              .type = ParameterType::FLOAT,
                              .type_name = "",
                              .values = absl::MakeSpan(xradius)};

  std::vector<double> yradius = {2.0};
  Parameter yradius_parameter{.directive = "",
                              .type = ParameterType::FLOAT,
                              .type_name = "",
                              .values = absl::MakeSpan(yradius)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"xradius", xradius_parameter}, {"yradius", yradius_parameter}};

  TrianglePixelFilter actual;
  RemoveTrianglePixelFilterV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                xradius: 1.0 yradius: 2.0
              )pb"));
}

}  // namespace
}  // namespace pbrt_proto
