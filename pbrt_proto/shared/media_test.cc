#include "pbrt_proto/shared/media.h"

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

TEST(RemoveCloudMediumV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  CloudMedium actual;
  RemoveCloudMediumV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveCloudMediumV1, WithData) {
  std::vector<absl::string_view> sigma_a = {"a"};
  Parameter sigma_a_parameter{.directive = "",
                              .type = ParameterType::SPECTRUM,
                              .type_name = "",
                              .values = absl::MakeSpan(sigma_a)};

  std::vector<absl::string_view> sigma_s = {"b"};
  Parameter sigma_s_parameter{.directive = "",
                              .type = ParameterType::SPECTRUM,
                              .type_name = "",
                              .values = absl::MakeSpan(sigma_s)};

  std::vector<double> g = {5.0};
  Parameter g_parameter{.directive = "",
                        .type = ParameterType::FLOAT,
                        .type_name = "",
                        .values = absl::MakeSpan(g)};

  std::vector<std::array<double, 3>> p0 = {{6.0, 7.0, 8.0}};
  Parameter p0_parameter{.directive = "",
                         .type = ParameterType::POINT3,
                         .type_name = "",
                         .values = absl::MakeSpan(p0)};

  std::vector<std::array<double, 3>> p1 = {{9.0, 10.0, 11.0}};
  Parameter p1_parameter{.directive = "",
                         .type = ParameterType::POINT3,
                         .type_name = "",
                         .values = absl::MakeSpan(p1)};

  std::vector<double> density = {12.0};
  Parameter density_parameter{.directive = "",
                              .type = ParameterType::FLOAT,
                              .type_name = "",
                              .values = absl::MakeSpan(density)};

  std::vector<double> frequency = {13.0};
  Parameter frequency_parameter{.directive = "",
                                .type = ParameterType::FLOAT,
                                .type_name = "",
                                .values = absl::MakeSpan(frequency)};

  std::vector<double> wispiness = {14.0};
  Parameter wispiness_parameter{.directive = "",
                                .type = ParameterType::FLOAT,
                                .type_name = "",
                                .values = absl::MakeSpan(wispiness)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"sigma_a", sigma_a_parameter},
      {"sigma_s", sigma_s_parameter},
      {"g", g_parameter},
      {"p0", p0_parameter},
      {"p1", p1_parameter},
      {"density", density_parameter},
      {"frequency", frequency_parameter},
      {"wispiness", wispiness_parameter}};

  CloudMedium actual;
  RemoveCloudMediumV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                sigma_a { sampled_spectrum_filename: "a" }
                sigma_s { sampled_spectrum_filename: "b" }
                g: 5.0
                p0 { x: 6.0 y: 7.0 z: 8.0 }
                p1 { x: 9.0 y: 10.0 z: 11.0 }
                density: 12.0
                frequency: 13.0
                wispiness: 14.0
              )pb"));
}

TEST(RemoveHomogeneousMediumV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  HomogeneousMedium actual;
  RemoveHomogeneousMediumV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveHomogeneousMediumV1, WithData) {
  std::vector<absl::string_view> sigma_a = {"a"};
  Parameter sigma_a_parameter{.directive = "",
                              .type = ParameterType::SPECTRUM,
                              .type_name = "",
                              .values = absl::MakeSpan(sigma_a)};

  std::vector<absl::string_view> sigma_s = {"b"};
  Parameter sigma_s_parameter{.directive = "",
                              .type = ParameterType::SPECTRUM,
                              .type_name = "",
                              .values = absl::MakeSpan(sigma_s)};

  std::vector<double> g = {5.0};
  Parameter g_parameter{.directive = "",
                        .type = ParameterType::FLOAT,
                        .type_name = "",
                        .values = absl::MakeSpan(g)};

  std::vector<double> scale = {6.0};
  Parameter scale_parameter{.directive = "",
                            .type = ParameterType::FLOAT,
                            .type_name = "",
                            .values = absl::MakeSpan(scale)};

  std::vector<absl::string_view> preset = {"Apple"};
  Parameter preset_parameter{.directive = "",
                             .type = ParameterType::STRING,
                             .type_name = "",
                             .values = absl::MakeSpan(preset)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"sigma_a", sigma_a_parameter},
      {"sigma_s", sigma_s_parameter},
      {"g", g_parameter},
      {"scale", scale_parameter},
      {"preset", preset_parameter}};

  HomogeneousMedium actual;
  RemoveHomogeneousMediumV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                sigma_a { sampled_spectrum_filename: "a" }
                sigma_s { sampled_spectrum_filename: "b" }
                g: 5.0
                scale: 6.0
                preset: APPLE
              )pb"));
}

TEST(RemoveHomogeneousMediumV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  HomogeneousMedium actual;
  RemoveHomogeneousMediumV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveHomogeneousMediumV2, WithData) {
  std::vector<absl::string_view> sigma_a = {"a"};
  Parameter sigma_a_parameter{.directive = "",
                              .type = ParameterType::SPECTRUM,
                              .type_name = "",
                              .values = absl::MakeSpan(sigma_a)};

  std::vector<absl::string_view> sigma_s = {"b"};
  Parameter sigma_s_parameter{.directive = "",
                              .type = ParameterType::SPECTRUM,
                              .type_name = "",
                              .values = absl::MakeSpan(sigma_s)};

  std::vector<double> g = {5.0};
  Parameter g_parameter{.directive = "",
                        .type = ParameterType::FLOAT,
                        .type_name = "",
                        .values = absl::MakeSpan(g)};

  std::vector<double> scale = {6.0};
  Parameter scale_parameter{.directive = "",
                            .type = ParameterType::FLOAT,
                            .type_name = "",
                            .values = absl::MakeSpan(scale)};

  std::vector<absl::string_view> preset = {"Apple"};
  Parameter preset_parameter{.directive = "",
                             .type = ParameterType::STRING,
                             .type_name = "",
                             .values = absl::MakeSpan(preset)};

  std::vector<absl::string_view> le = {"c"};
  Parameter le_parameter{.directive = "",
                         .type = ParameterType::SPECTRUM,
                         .type_name = "",
                         .values = absl::MakeSpan(le)};

  std::vector<double> lescale = {7.0};
  Parameter lescale_parameter{.directive = "",
                              .type = ParameterType::FLOAT,
                              .type_name = "",
                              .values = absl::MakeSpan(lescale)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"sigma_a", sigma_a_parameter},
      {"sigma_s", sigma_s_parameter},
      {"g", g_parameter},
      {"scale", scale_parameter},
      {"preset", preset_parameter},
      {"Le", le_parameter},
      {"Lescale", lescale_parameter}};

  HomogeneousMedium actual;
  RemoveHomogeneousMediumV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                sigma_a { sampled_spectrum_filename: "a" }
                sigma_s { sampled_spectrum_filename: "b" }
                g: 5.0
                scale: 6.0
                preset: APPLE
                Le { sampled_spectrum_filename: "c" }
                Lescale: 7.0
              )pb"));
}

TEST(RemoveNanoVdbMediumV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  NanoVdbMedium actual;
  RemoveNanoVdbMediumV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveNanoVdbMediumV1, WithData) {
  std::vector<absl::string_view> sigma_a = {"a"};
  Parameter sigma_a_parameter{.directive = "",
                              .type = ParameterType::SPECTRUM,
                              .type_name = "",
                              .values = absl::MakeSpan(sigma_a)};

  std::vector<absl::string_view> sigma_s = {"b"};
  Parameter sigma_s_parameter{.directive = "",
                              .type = ParameterType::SPECTRUM,
                              .type_name = "",
                              .values = absl::MakeSpan(sigma_s)};

  std::vector<double> g = {5.0};
  Parameter g_parameter{.directive = "",
                        .type = ParameterType::FLOAT,
                        .type_name = "",
                        .values = absl::MakeSpan(g)};

  std::vector<double> scale = {6.0};
  Parameter scale_parameter{.directive = "",
                            .type = ParameterType::FLOAT,
                            .type_name = "",
                            .values = absl::MakeSpan(scale)};

  std::vector<double> lescale = {7.0};
  Parameter lescale_parameter{.directive = "",
                              .type = ParameterType::FLOAT,
                              .type_name = "",
                              .values = absl::MakeSpan(lescale)};

  std::vector<double> temperatureoffset = {8.0};
  Parameter temperatureoffset_parameter{
      .directive = "",
      .type = ParameterType::FLOAT,
      .type_name = "",
      .values = absl::MakeSpan(temperatureoffset)};

  std::vector<double> temperaturescale = {9.0};
  Parameter temperaturescale_parameter{
      .directive = "",
      .type = ParameterType::FLOAT,
      .type_name = "",
      .values = absl::MakeSpan(temperaturescale)};

  std::vector<absl::string_view> filename = {"f"};
  Parameter filename_parameter{.directive = "",
                               .type = ParameterType::STRING,
                               .type_name = "",
                               .values = absl::MakeSpan(filename)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"sigma_a", sigma_a_parameter},
      {"sigma_s", sigma_s_parameter},
      {"g", g_parameter},
      {"scale", scale_parameter},
      {"Lescale", lescale_parameter},
      {"temperatureoffset", temperatureoffset_parameter},
      {"temperaturescale", temperaturescale_parameter},
      {"filename", filename_parameter}};

  NanoVdbMedium actual;
  RemoveNanoVdbMediumV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                sigma_a { sampled_spectrum_filename: "a" }
                sigma_s { sampled_spectrum_filename: "b" }
                g: 5.0
                scale: 6.0
                Lescale: 7.0
                temperatureoffset: 8.0
                temperaturescale: 9.0
                filename: "f"
              )pb"));
}

TEST(RemoveRgbGridMediumV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  RgbGridMedium actual;
  EXPECT_TRUE(RemoveRgbGridMediumV1(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveRgbGridMediumV1, WithData) {
  std::vector<std::array<double, 3>> sigma_a = {{1.0, 2.0, 3.0}};
  Parameter sigma_a_parameter{.directive = "",
                              .type = ParameterType::RGB,
                              .type_name = "",
                              .values = absl::MakeSpan(sigma_a)};

  std::vector<std::array<double, 3>> sigma_s = {{4.0, 5.0, 6.0}};
  Parameter sigma_s_parameter{.directive = "",
                              .type = ParameterType::RGB,
                              .type_name = "",
                              .values = absl::MakeSpan(sigma_s)};

  std::vector<double> g = {7.0};
  Parameter g_parameter{.directive = "",
                        .type = ParameterType::FLOAT,
                        .type_name = "",
                        .values = absl::MakeSpan(g)};

  std::vector<double> lescale = {8.0};
  Parameter lescale_parameter{.directive = "",
                              .type = ParameterType::FLOAT,
                              .type_name = "",
                              .values = absl::MakeSpan(lescale)};

  std::vector<std::array<double, 3>> p0 = {{9.0, 10.0, 11.0}};
  Parameter p0_parameter{.directive = "",
                         .type = ParameterType::POINT3,
                         .type_name = "",
                         .values = absl::MakeSpan(p0)};

  std::vector<std::array<double, 3>> p1 = {{12.0, 13.0, 14.0}};
  Parameter p1_parameter{.directive = "",
                         .type = ParameterType::POINT3,
                         .type_name = "",
                         .values = absl::MakeSpan(p1)};

  std::vector<double> scale = {15.0};
  Parameter scale_parameter{.directive = "",
                            .type = ParameterType::FLOAT,
                            .type_name = "",
                            .values = absl::MakeSpan(scale)};

  std::vector<int> nx = {16};
  Parameter nx_parameter{.directive = "",
                         .type = ParameterType::INTEGER,
                         .type_name = "",
                         .values = absl::MakeSpan(nx)};

  std::vector<int> ny = {17};
  Parameter ny_parameter{.directive = "",
                         .type = ParameterType::INTEGER,
                         .type_name = "",
                         .values = absl::MakeSpan(ny)};

  std::vector<int> nz = {18};
  Parameter nz_parameter{.directive = "",
                         .type = ParameterType::INTEGER,
                         .type_name = "",
                         .values = absl::MakeSpan(nz)};

  std::vector<std::array<double, 3>> le = {{19.0, 20.0, 21.0}};
  Parameter le_parameter{.directive = "",
                         .type = ParameterType::RGB,
                         .type_name = "",
                         .values = absl::MakeSpan(le)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"sigma_a", sigma_a_parameter},
      {"sigma_s", sigma_s_parameter},
      {"scale", scale_parameter},
      {"Lescale", lescale_parameter},
      {"g", g_parameter},
      {"p0", p0_parameter},
      {"p1", p1_parameter},
      {"nx", nx_parameter},
      {"ny", ny_parameter},
      {"nz", nz_parameter},
      {"Le", le_parameter}};

  RgbGridMedium actual;
  EXPECT_TRUE(RemoveRgbGridMediumV1(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                sigma_a { r: 1.0 g: 2.0 b: 3.0 }
                sigma_s { r: 4.0 g: 5.0 b: 6.0 }
                g: 7.0
                Lescale: 8.0
                p0 { x: 9.0 y: 10.0 z: 11.0 }
                p1 { x: 12.0 y: 13.0 z: 14.0 }
                scale: 15.0
                nx: 16
                ny: 17
                nz: 18
                Le { r: 19.0 g: 20.0 b: 21.0 }
              )pb"));
}

TEST(RemoveUniformGridMediumV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  UniformGridMedium actual;
  EXPECT_TRUE(RemoveUniformGridMediumV1(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveUniformGridMediumV1, WithData) {
  std::vector<absl::string_view> sigma_a = {"a"};
  Parameter sigma_a_parameter{.directive = "",
                              .type = ParameterType::SPECTRUM,
                              .type_name = "",
                              .values = absl::MakeSpan(sigma_a)};

  std::vector<absl::string_view> sigma_s = {"b"};
  Parameter sigma_s_parameter{.directive = "",
                              .type = ParameterType::SPECTRUM,
                              .type_name = "",
                              .values = absl::MakeSpan(sigma_s)};

  std::vector<double> g = {7.0};
  Parameter g_parameter{.directive = "",
                        .type = ParameterType::FLOAT,
                        .type_name = "",
                        .values = absl::MakeSpan(g)};

  std::vector<std::array<double, 3>> p0 = {{9.0, 10.0, 11.0}};
  Parameter p0_parameter{.directive = "",
                         .type = ParameterType::POINT3,
                         .type_name = "",
                         .values = absl::MakeSpan(p0)};

  std::vector<std::array<double, 3>> p1 = {{12.0, 13.0, 14.0}};
  Parameter p1_parameter{.directive = "",
                         .type = ParameterType::POINT3,
                         .type_name = "",
                         .values = absl::MakeSpan(p1)};

  std::vector<double> scale = {15.0};
  Parameter scale_parameter{.directive = "",
                            .type = ParameterType::FLOAT,
                            .type_name = "",
                            .values = absl::MakeSpan(scale)};

  std::vector<int> nx = {16};
  Parameter nx_parameter{.directive = "",
                         .type = ParameterType::INTEGER,
                         .type_name = "",
                         .values = absl::MakeSpan(nx)};

  std::vector<int> ny = {17};
  Parameter ny_parameter{.directive = "",
                         .type = ParameterType::INTEGER,
                         .type_name = "",
                         .values = absl::MakeSpan(ny)};

  std::vector<int> nz = {18};
  Parameter nz_parameter{.directive = "",
                         .type = ParameterType::INTEGER,
                         .type_name = "",
                         .values = absl::MakeSpan(nz)};

  std::vector<absl::string_view> preset = {"Apple"};
  Parameter preset_parameter{.directive = "",
                             .type = ParameterType::STRING,
                             .type_name = "",
                             .values = absl::MakeSpan(preset)};

  std::vector<double> density = {19.0};
  Parameter density_parameter{.directive = "",
                              .type = ParameterType::FLOAT,
                              .type_name = "",
                              .values = absl::MakeSpan(density)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"sigma_a", sigma_a_parameter},
      {"sigma_s", sigma_s_parameter},
      {"scale", scale_parameter},
      {"preset", preset_parameter},
      {"g", g_parameter},
      {"p0", p0_parameter},
      {"p1", p1_parameter},
      {"nx", nx_parameter},
      {"ny", ny_parameter},
      {"nz", nz_parameter},
      {"density", density_parameter}};

  UniformGridMedium actual;
  EXPECT_TRUE(RemoveUniformGridMediumV1(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                sigma_a { sampled_spectrum_filename: "a" }
                sigma_s { sampled_spectrum_filename: "b" }
                g: 7.0
                p0 { x: 9.0 y: 10.0 z: 11.0 }
                p1 { x: 12.0 y: 13.0 z: 14.0 }
                preset: APPLE
                scale: 15.0
                nx: 16
                ny: 17
                nz: 18
                density: 19.0
              )pb"));
}

TEST(RemoveUniformGridMediumV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  UniformGridMedium actual;
  EXPECT_TRUE(RemoveUniformGridMediumV2(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveUniformGridMediumV2, WithData) {
  std::vector<absl::string_view> sigma_a = {"a"};
  Parameter sigma_a_parameter{.directive = "",
                              .type = ParameterType::SPECTRUM,
                              .type_name = "",
                              .values = absl::MakeSpan(sigma_a)};

  std::vector<absl::string_view> sigma_s = {"b"};
  Parameter sigma_s_parameter{.directive = "",
                              .type = ParameterType::SPECTRUM,
                              .type_name = "",
                              .values = absl::MakeSpan(sigma_s)};

  std::vector<double> g = {7.0};
  Parameter g_parameter{.directive = "",
                        .type = ParameterType::FLOAT,
                        .type_name = "",
                        .values = absl::MakeSpan(g)};

  std::vector<double> lescale = {8.0};
  Parameter lescale_parameter{.directive = "",
                              .type = ParameterType::FLOAT,
                              .type_name = "",
                              .values = absl::MakeSpan(lescale)};

  std::vector<std::array<double, 3>> p0 = {{9.0, 10.0, 11.0}};
  Parameter p0_parameter{.directive = "",
                         .type = ParameterType::POINT3,
                         .type_name = "",
                         .values = absl::MakeSpan(p0)};

  std::vector<std::array<double, 3>> p1 = {{12.0, 13.0, 14.0}};
  Parameter p1_parameter{.directive = "",
                         .type = ParameterType::POINT3,
                         .type_name = "",
                         .values = absl::MakeSpan(p1)};

  std::vector<double> scale = {15.0};
  Parameter scale_parameter{.directive = "",
                            .type = ParameterType::FLOAT,
                            .type_name = "",
                            .values = absl::MakeSpan(scale)};

  std::vector<int> nx = {16};
  Parameter nx_parameter{.directive = "",
                         .type = ParameterType::INTEGER,
                         .type_name = "",
                         .values = absl::MakeSpan(nx)};

  std::vector<int> ny = {17};
  Parameter ny_parameter{.directive = "",
                         .type = ParameterType::INTEGER,
                         .type_name = "",
                         .values = absl::MakeSpan(ny)};

  std::vector<int> nz = {18};
  Parameter nz_parameter{.directive = "",
                         .type = ParameterType::INTEGER,
                         .type_name = "",
                         .values = absl::MakeSpan(nz)};

  std::vector<absl::string_view> preset = {"Apple"};
  Parameter preset_parameter{.directive = "",
                             .type = ParameterType::STRING,
                             .type_name = "",
                             .values = absl::MakeSpan(preset)};

  std::vector<double> density = {19.0};
  Parameter density_parameter{.directive = "",
                              .type = ParameterType::FLOAT,
                              .type_name = "",
                              .values = absl::MakeSpan(density)};

  std::vector<double> temperatureoffset = {20.0};
  Parameter temperatureoffset_parameter{
      .directive = "",
      .type = ParameterType::FLOAT,
      .type_name = "",
      .values = absl::MakeSpan(temperatureoffset)};

  std::vector<double> temperaturescale = {21};
  Parameter temperaturescale_parameter{
      .directive = "",
      .type = ParameterType::FLOAT,
      .type_name = "",
      .values = absl::MakeSpan(temperaturescale)};

  std::vector<double> temperature = {22};
  Parameter temperature_parameter{.directive = "",
                                  .type = ParameterType::FLOAT,
                                  .type_name = "",
                                  .values = absl::MakeSpan(temperature)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"sigma_a", sigma_a_parameter},
      {"sigma_s", sigma_s_parameter},
      {"scale", scale_parameter},
      {"preset", preset_parameter},
      {"Lescale", lescale_parameter},
      {"g", g_parameter},
      {"p0", p0_parameter},
      {"p1", p1_parameter},
      {"nx", nx_parameter},
      {"ny", ny_parameter},
      {"nz", nz_parameter},
      {"density", density_parameter},
      {"temperatureoffset", temperatureoffset_parameter},
      {"temperaturescale", temperaturescale_parameter},
      {"temperature", temperature_parameter}};

  UniformGridMedium actual;
  EXPECT_TRUE(RemoveUniformGridMediumV2(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                sigma_a { sampled_spectrum_filename: "a" }
                sigma_s { sampled_spectrum_filename: "b" }
                g: 7.0
                Lescale: 8.0
                p0 { x: 9.0 y: 10.0 z: 11.0 }
                p1 { x: 12.0 y: 13.0 z: 14.0 }
                preset: APPLE
                scale: 15.0
                nx: 16
                ny: 17
                nz: 18
                density: 19.0
                temperatureoffset: 20.0
                temperaturescale: 21.0
                temperature: 22.0
              )pb"));
}

}  // namespace
}  // namespace pbrt_proto
