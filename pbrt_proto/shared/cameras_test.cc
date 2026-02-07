#include "pbrt_proto/shared/cameras.h"

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

TEST(OrthographicCameraV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  OrthographicCamera actual;
  RemoveOrthographicCameraV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(OrthographicCameraV1, WithData) {
  std::vector<double> shutteropen = {1.0};
  Parameter shutteropen_parameter{.directive = "",
                                  .type = ParameterType::FLOAT,
                                  .type_name = "",
                                  .values = absl::MakeSpan(shutteropen)};

  std::vector<double> shutterclose = {2.0};
  Parameter shutterclose_parameter{.directive = "",
                                   .type = ParameterType::FLOAT,
                                   .type_name = "",
                                   .values = absl::MakeSpan(shutterclose)};

  std::vector<double> focaldistance = {3.0};
  Parameter focaldistance_parameter{.directive = "",
                                    .type = ParameterType::FLOAT,
                                    .type_name = "",
                                    .values = absl::MakeSpan(focaldistance)};

  std::vector<double> frameaspectratio = {4.0};
  Parameter frameaspectratio_parameter{
      .directive = "",
      .type = ParameterType::FLOAT,
      .type_name = "",
      .values = absl::MakeSpan(frameaspectratio)};

  std::vector<double> screenwindow = {5.0, 6.0, 7.0, 8.0};
  Parameter screenwindow_parameter{.directive = "",
                                   .type = ParameterType::FLOAT,
                                   .type_name = "",
                                   .values = absl::MakeSpan(screenwindow)};

  std::vector<double> hither = {9.0};
  Parameter hither_parameter{.directive = "",
                             .type = ParameterType::FLOAT,
                             .type_name = "",
                             .values = absl::MakeSpan(hither)};

  std::vector<double> yon = {10.0};
  Parameter yon_parameter{.directive = "",
                          .type = ParameterType::FLOAT,
                          .type_name = "",
                          .values = absl::MakeSpan(yon)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"shutteropen", shutteropen_parameter},
      {"shutterclose", shutterclose_parameter},
      {"focaldistance", focaldistance_parameter},
      {"frameaspectratio", frameaspectratio_parameter},
      {"screenwindow", screenwindow_parameter},
      {"hither", hither_parameter},
      {"yon", yon_parameter}};

  OrthographicCamera actual;
  RemoveOrthographicCameraV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                shutteropen: 1.0
                shutterclose: 2.0
                focaldistance: 3.0
                frameaspectratio: 4.0
                screenwindow { x_min: 5.0 x_max: 6.0 y_min: 7.0 y_max: 8.0 }
                hither: 9.0
                yon: 10.0
              )pb"));
}

TEST(OrthographicCameraV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  OrthographicCamera actual;
  RemoveOrthographicCameraV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(OrthographicCameraV2, WithData) {
  std::vector<double> shutteropen = {1.0};
  Parameter shutteropen_parameter{.directive = "",
                                  .type = ParameterType::FLOAT,
                                  .type_name = "",
                                  .values = absl::MakeSpan(shutteropen)};

  std::vector<double> shutterclose = {2.0};
  Parameter shutterclose_parameter{.directive = "",
                                   .type = ParameterType::FLOAT,
                                   .type_name = "",
                                   .values = absl::MakeSpan(shutterclose)};

  std::vector<double> focaldistance = {3.0};
  Parameter focaldistance_parameter{.directive = "",
                                    .type = ParameterType::FLOAT,
                                    .type_name = "",
                                    .values = absl::MakeSpan(focaldistance)};

  std::vector<double> frameaspectratio = {4.0};
  Parameter frameaspectratio_parameter{
      .directive = "",
      .type = ParameterType::FLOAT,
      .type_name = "",
      .values = absl::MakeSpan(frameaspectratio)};

  std::vector<double> screenwindow = {5.0, 6.0, 7.0, 8.0};
  Parameter screenwindow_parameter{.directive = "",
                                   .type = ParameterType::FLOAT,
                                   .type_name = "",
                                   .values = absl::MakeSpan(screenwindow)};

  std::vector<double> hither = {9.0};
  Parameter hither_parameter{.directive = "",
                             .type = ParameterType::FLOAT,
                             .type_name = "",
                             .values = absl::MakeSpan(hither)};

  std::vector<double> yon = {10.0};
  Parameter yon_parameter{.directive = "",
                          .type = ParameterType::FLOAT,
                          .type_name = "",
                          .values = absl::MakeSpan(yon)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"shutteropen", shutteropen_parameter},
      {"shutterclose", shutterclose_parameter},
      {"focaldistance", focaldistance_parameter},
      {"frameaspectratio", frameaspectratio_parameter},
      {"screenwindow", screenwindow_parameter},
      {"hither", hither_parameter},
      {"yon", yon_parameter}};

  OrthographicCamera actual;
  RemoveOrthographicCameraV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                shutteropen: 1.0
                shutterclose: 2.0
                focaldistance: 3.0
                frameaspectratio: 4.0
                screenwindow { x_min: 5.0 x_max: 6.0 y_min: 7.0 y_max: 8.0 }
              )pb"));
}

TEST(RemovePerspectiveCameraV1, WithData) {
  std::vector<double> shutteropen = {1.0};
  Parameter shutteropen_parameter{.directive = "",
                                  .type = ParameterType::FLOAT,
                                  .type_name = "",
                                  .values = absl::MakeSpan(shutteropen)};

  std::vector<double> shutterclose = {2.0};
  Parameter shutterclose_parameter{.directive = "",
                                   .type = ParameterType::FLOAT,
                                   .type_name = "",
                                   .values = absl::MakeSpan(shutterclose)};

  std::vector<double> fov = {3.0};
  Parameter fov_parameter{.directive = "",
                          .type = ParameterType::FLOAT,
                          .type_name = "",
                          .values = absl::MakeSpan(fov)};

  std::vector<double> focaldistance = {4.0};
  Parameter focaldistance_parameter{.directive = "",
                                    .type = ParameterType::FLOAT,
                                    .type_name = "",
                                    .values = absl::MakeSpan(focaldistance)};

  std::vector<double> frameaspectratio = {5.0};
  Parameter frameaspectratio_parameter{
      .directive = "",
      .type = ParameterType::FLOAT,
      .type_name = "",
      .values = absl::MakeSpan(frameaspectratio)};

  std::vector<double> screenwindow = {6.0, 7.0, 8.0, 9.0};
  Parameter screenwindow_parameter{.directive = "",
                                   .type = ParameterType::FLOAT,
                                   .type_name = "",
                                   .values = absl::MakeSpan(screenwindow)};

  std::vector<double> hither = {10.0};
  Parameter hither_parameter{.directive = "",
                             .type = ParameterType::FLOAT,
                             .type_name = "",
                             .values = absl::MakeSpan(hither)};

  std::vector<double> yon = {11.0};
  Parameter yon_parameter{.directive = "",
                          .type = ParameterType::FLOAT,
                          .type_name = "",
                          .values = absl::MakeSpan(yon)};

  std::vector<double> halffov = {6.0};
  Parameter halffov_parameter{.directive = "",
                              .type = ParameterType::FLOAT,
                              .type_name = "",
                              .values = absl::MakeSpan(halffov)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"shutteropen", shutteropen_parameter},
      {"shutterclose", shutterclose_parameter},
      {"fov", fov_parameter},
      {"focaldistance", focaldistance_parameter},
      {"frameaspectratio", frameaspectratio_parameter},
      {"screenwindow", screenwindow_parameter},
      {"hither", hither_parameter},
      {"yon", yon_parameter},
      {"halffov", halffov_parameter}};

  PerspectiveCamera actual;
  RemovePerspectiveCameraV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                shutteropen: 1.0
                shutterclose: 2.0
                fov: 3.0
                focaldistance: 4.0
                frameaspectratio: 5.0
                screenwindow { x_min: 6.0 x_max: 7.0 y_min: 8.0 y_max: 9.0 }
                hither: 10.0
                yon: 11.0
              )pb"));
}

TEST(RemovePerspectiveCameraV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  PerspectiveCamera actual;
  RemovePerspectiveCameraV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemovePerspectiveCameraV2, WithData) {
  std::vector<double> shutteropen = {1.0};
  Parameter shutteropen_parameter{.directive = "",
                                  .type = ParameterType::FLOAT,
                                  .type_name = "",
                                  .values = absl::MakeSpan(shutteropen)};

  std::vector<double> shutterclose = {2.0};
  Parameter shutterclose_parameter{.directive = "",
                                   .type = ParameterType::FLOAT,
                                   .type_name = "",
                                   .values = absl::MakeSpan(shutterclose)};

  std::vector<double> fov = {3.0};
  Parameter fov_parameter{.directive = "",
                          .type = ParameterType::FLOAT,
                          .type_name = "",
                          .values = absl::MakeSpan(fov)};

  std::vector<double> focaldistance = {4.0};
  Parameter focaldistance_parameter{.directive = "",
                                    .type = ParameterType::FLOAT,
                                    .type_name = "",
                                    .values = absl::MakeSpan(focaldistance)};

  std::vector<double> frameaspectratio = {5.0};
  Parameter frameaspectratio_parameter{
      .directive = "",
      .type = ParameterType::FLOAT,
      .type_name = "",
      .values = absl::MakeSpan(frameaspectratio)};

  std::vector<double> screenwindow = {6.0, 7.0, 8.0, 9.0};
  Parameter screenwindow_parameter{.directive = "",
                                   .type = ParameterType::FLOAT,
                                   .type_name = "",
                                   .values = absl::MakeSpan(screenwindow)};

  std::vector<double> hither = {10.0};
  Parameter hither_parameter{.directive = "",
                             .type = ParameterType::FLOAT,
                             .type_name = "",
                             .values = absl::MakeSpan(hither)};

  std::vector<double> yon = {11.0};
  Parameter yon_parameter{.directive = "",
                          .type = ParameterType::FLOAT,
                          .type_name = "",
                          .values = absl::MakeSpan(yon)};

  std::vector<double> halffov = {6.0};
  Parameter halffov_parameter{.directive = "",
                              .type = ParameterType::FLOAT,
                              .type_name = "",
                              .values = absl::MakeSpan(halffov)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"shutteropen", shutteropen_parameter},
      {"shutterclose", shutterclose_parameter},
      {"fov", fov_parameter},
      {"focaldistance", focaldistance_parameter},
      {"frameaspectratio", frameaspectratio_parameter},
      {"screenwindow", screenwindow_parameter},
      {"hither", hither_parameter},
      {"yon", yon_parameter},
      {"halffov", halffov_parameter}};

  PerspectiveCamera actual;
  RemovePerspectiveCameraV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                shutteropen: 1.0
                shutterclose: 2.0
                fov: 12.0
                focaldistance: 4.0
                frameaspectratio: 5.0
                screenwindow { x_min: 6.0 x_max: 7.0 y_min: 8.0 y_max: 9.0 }
              )pb"));
}

TEST(RemovePerspectiveCameraV2, ZeroHalfFov) {
  std::vector<double> halffov = {0.0};
  Parameter halffov_parameter{.directive = "",
                              .type = ParameterType::FLOAT,
                              .type_name = "",
                              .values = absl::MakeSpan(halffov)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"halffov", halffov_parameter}};

  PerspectiveCamera actual;
  RemovePerspectiveCameraV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemovePerspectiveCameraV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  PerspectiveCamera actual;
  RemovePerspectiveCameraV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemovePerspectiveCameraV3, WithData) {
  std::vector<double> shutteropen = {1.0};
  Parameter shutteropen_parameter{.directive = "",
                                  .type = ParameterType::FLOAT,
                                  .type_name = "",
                                  .values = absl::MakeSpan(shutteropen)};

  std::vector<double> shutterclose = {2.0};
  Parameter shutterclose_parameter{.directive = "",
                                   .type = ParameterType::FLOAT,
                                   .type_name = "",
                                   .values = absl::MakeSpan(shutterclose)};

  std::vector<double> fov = {3.0};
  Parameter fov_parameter{.directive = "",
                          .type = ParameterType::FLOAT,
                          .type_name = "",
                          .values = absl::MakeSpan(fov)};

  std::vector<double> focaldistance = {4.0};
  Parameter focaldistance_parameter{.directive = "",
                                    .type = ParameterType::FLOAT,
                                    .type_name = "",
                                    .values = absl::MakeSpan(focaldistance)};

  std::vector<double> frameaspectratio = {5.0};
  Parameter frameaspectratio_parameter{
      .directive = "",
      .type = ParameterType::FLOAT,
      .type_name = "",
      .values = absl::MakeSpan(frameaspectratio)};

  std::vector<double> screenwindow = {6.0, 7.0, 8.0, 9.0};
  Parameter screenwindow_parameter{.directive = "",
                                   .type = ParameterType::FLOAT,
                                   .type_name = "",
                                   .values = absl::MakeSpan(screenwindow)};

  std::vector<double> hither = {10.0};
  Parameter hither_parameter{.directive = "",
                             .type = ParameterType::FLOAT,
                             .type_name = "",
                             .values = absl::MakeSpan(hither)};

  std::vector<double> yon = {11.0};
  Parameter yon_parameter{.directive = "",
                          .type = ParameterType::FLOAT,
                          .type_name = "",
                          .values = absl::MakeSpan(yon)};

  std::vector<double> halffov = {6.0};
  Parameter halffov_parameter{.directive = "",
                              .type = ParameterType::FLOAT,
                              .type_name = "",
                              .values = absl::MakeSpan(halffov)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"shutteropen", shutteropen_parameter},
      {"shutterclose", shutterclose_parameter},
      {"fov", fov_parameter},
      {"focaldistance", focaldistance_parameter},
      {"frameaspectratio", frameaspectratio_parameter},
      {"screenwindow", screenwindow_parameter},
      {"hither", hither_parameter},
      {"yon", yon_parameter},
      {"halffov", halffov_parameter}};

  PerspectiveCamera actual;
  RemovePerspectiveCameraV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                shutteropen: 1.0
                shutterclose: 2.0
                fov: 3.0
                focaldistance: 4.0
                frameaspectratio: 5.0
                screenwindow { x_min: 6.0 x_max: 7.0 y_min: 8.0 y_max: 9.0 }
              )pb"));
}

TEST(RemoveSphericalCameraV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  SphericalCamera actual;
  RemoveSphericalCameraV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveSphericalCameraV1, WithData) {
  std::vector<double> shutteropen = {1.0};
  Parameter shutteropen_parameter{.directive = "",
                                  .type = ParameterType::FLOAT,
                                  .type_name = "",
                                  .values = absl::MakeSpan(shutteropen)};

  std::vector<double> shutterclose = {2.0};
  Parameter shutterclose_parameter{.directive = "",
                                   .type = ParameterType::FLOAT,
                                   .type_name = "",
                                   .values = absl::MakeSpan(shutterclose)};

  std::vector<double> lensradius = {3.0};
  Parameter lensradius_parameter{.directive = "",
                                 .type = ParameterType::FLOAT,
                                 .type_name = "",
                                 .values = absl::MakeSpan(lensradius)};

  std::vector<double> focaldistance = {4.0};
  Parameter focaldistance_parameter{.directive = "",
                                    .type = ParameterType::FLOAT,
                                    .type_name = "",
                                    .values = absl::MakeSpan(focaldistance)};

  std::vector<double> frameaspectratio = {5.0};
  Parameter frameaspectratio_parameter{
      .directive = "",
      .type = ParameterType::FLOAT,
      .type_name = "",
      .values = absl::MakeSpan(frameaspectratio)};

  std::vector<double> screenwindow = {6.0, 7.0, 8.0, 9.0};
  Parameter screenwindow_parameter{.directive = "",
                                   .type = ParameterType::FLOAT,
                                   .type_name = "",
                                   .values = absl::MakeSpan(screenwindow)};

  std::vector<double> hither = {10.0};
  Parameter hither_parameter{.directive = "",
                             .type = ParameterType::FLOAT,
                             .type_name = "",
                             .values = absl::MakeSpan(hither)};

  std::vector<double> yon = {11.0};
  Parameter yon_parameter{.directive = "",
                          .type = ParameterType::FLOAT,
                          .type_name = "",
                          .values = absl::MakeSpan(yon)};

  std::vector<absl::string_view> mapping = {"equalarea"};
  Parameter mapping_parameter{.directive = "",
                              .type = ParameterType::FLOAT,
                              .type_name = "",
                              .values = absl::MakeSpan(mapping)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"shutteropen", shutteropen_parameter},
      {"shutterclose", shutterclose_parameter},
      {"lensradius", lensradius_parameter},
      {"focaldistance", focaldistance_parameter},
      {"frameaspectratio", frameaspectratio_parameter},
      {"screenwindow", screenwindow_parameter},
      {"hither", hither_parameter},
      {"yon", yon_parameter},
      {"mapping", mapping_parameter}};

  SphericalCamera actual;
  RemoveSphericalCameraV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                shutteropen: 1.0
                shutterclose: 2.0
                lensradius: 3.0
                focaldistance: 4.0
                frameaspectratio: 5.0
                screenwindow { x_min: 6.0 x_max: 7.0 y_min: 8.0 y_max: 9.0 }
                hither: 10.0
                yon: 11.0
              )pb"));
}

TEST(RemoveSphericalCameraV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  SphericalCamera actual;
  RemoveSphericalCameraV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveSphericalCameraV2, WithData) {
  std::vector<double> shutteropen = {1.0};
  Parameter shutteropen_parameter{.directive = "",
                                  .type = ParameterType::FLOAT,
                                  .type_name = "",
                                  .values = absl::MakeSpan(shutteropen)};

  std::vector<double> shutterclose = {2.0};
  Parameter shutterclose_parameter{.directive = "",
                                   .type = ParameterType::FLOAT,
                                   .type_name = "",
                                   .values = absl::MakeSpan(shutterclose)};

  std::vector<double> lensradius = {3.0};
  Parameter lensradius_parameter{.directive = "",
                                 .type = ParameterType::FLOAT,
                                 .type_name = "",
                                 .values = absl::MakeSpan(lensradius)};

  std::vector<double> focaldistance = {4.0};
  Parameter focaldistance_parameter{.directive = "",
                                    .type = ParameterType::FLOAT,
                                    .type_name = "",
                                    .values = absl::MakeSpan(focaldistance)};

  std::vector<double> frameaspectratio = {5.0};
  Parameter frameaspectratio_parameter{
      .directive = "",
      .type = ParameterType::FLOAT,
      .type_name = "",
      .values = absl::MakeSpan(frameaspectratio)};

  std::vector<double> screenwindow = {6.0, 7.0, 8.0, 9.0};
  Parameter screenwindow_parameter{.directive = "",
                                   .type = ParameterType::FLOAT,
                                   .type_name = "",
                                   .values = absl::MakeSpan(screenwindow)};

  std::vector<double> hither = {10.0};
  Parameter hither_parameter{.directive = "",
                             .type = ParameterType::FLOAT,
                             .type_name = "",
                             .values = absl::MakeSpan(hither)};

  std::vector<double> yon = {11.0};
  Parameter yon_parameter{.directive = "",
                          .type = ParameterType::FLOAT,
                          .type_name = "",
                          .values = absl::MakeSpan(yon)};

  std::vector<absl::string_view> mapping = {"equalarea"};
  Parameter mapping_parameter{.directive = "",
                              .type = ParameterType::FLOAT,
                              .type_name = "",
                              .values = absl::MakeSpan(mapping)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"shutteropen", shutteropen_parameter},
      {"shutterclose", shutterclose_parameter},
      {"lensradius", lensradius_parameter},
      {"focaldistance", focaldistance_parameter},
      {"frameaspectratio", frameaspectratio_parameter},
      {"screenwindow", screenwindow_parameter},
      {"hither", hither_parameter},
      {"yon", yon_parameter},
      {"mapping", mapping_parameter}};

  SphericalCamera actual;
  RemoveSphericalCameraV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                shutteropen: 1.0
                shutterclose: 2.0
                lensradius: 3.0
                focaldistance: 4.0
                frameaspectratio: 5.0
                screenwindow { x_min: 6.0 x_max: 7.0 y_min: 8.0 y_max: 9.0 }
              )pb"));
}

TEST(RemoveSphericalCameraV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  SphericalCamera actual;
  ASSERT_TRUE(RemoveSphericalCameraV3(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveSphericalCameraV3, WithData) {
  std::vector<double> shutteropen = {1.0};
  Parameter shutteropen_parameter{.directive = "",
                                  .type = ParameterType::FLOAT,
                                  .type_name = "",
                                  .values = absl::MakeSpan(shutteropen)};

  std::vector<double> shutterclose = {2.0};
  Parameter shutterclose_parameter{.directive = "",
                                   .type = ParameterType::FLOAT,
                                   .type_name = "",
                                   .values = absl::MakeSpan(shutterclose)};

  std::vector<double> lensradius = {3.0};
  Parameter lensradius_parameter{.directive = "",
                                 .type = ParameterType::FLOAT,
                                 .type_name = "",
                                 .values = absl::MakeSpan(lensradius)};

  std::vector<double> focaldistance = {4.0};
  Parameter focaldistance_parameter{.directive = "",
                                    .type = ParameterType::FLOAT,
                                    .type_name = "",
                                    .values = absl::MakeSpan(focaldistance)};

  std::vector<double> frameaspectratio = {5.0};
  Parameter frameaspectratio_parameter{
      .directive = "",
      .type = ParameterType::FLOAT,
      .type_name = "",
      .values = absl::MakeSpan(frameaspectratio)};

  std::vector<double> screenwindow = {6.0, 7.0, 8.0, 9.0};
  Parameter screenwindow_parameter{.directive = "",
                                   .type = ParameterType::FLOAT,
                                   .type_name = "",
                                   .values = absl::MakeSpan(screenwindow)};

  std::vector<double> hither = {10.0};
  Parameter hither_parameter{.directive = "",
                             .type = ParameterType::FLOAT,
                             .type_name = "",
                             .values = absl::MakeSpan(hither)};

  std::vector<double> yon = {11.0};
  Parameter yon_parameter{.directive = "",
                          .type = ParameterType::FLOAT,
                          .type_name = "",
                          .values = absl::MakeSpan(yon)};

  std::vector<absl::string_view> mapping = {"equalarea"};
  Parameter mapping_parameter{.directive = "",
                              .type = ParameterType::STRING,
                              .type_name = "",
                              .values = absl::MakeSpan(mapping)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"shutteropen", shutteropen_parameter},
      {"shutterclose", shutterclose_parameter},
      {"lensradius", lensradius_parameter},
      {"focaldistance", focaldistance_parameter},
      {"frameaspectratio", frameaspectratio_parameter},
      {"screenwindow", screenwindow_parameter},
      {"hither", hither_parameter},
      {"yon", yon_parameter},
      {"mapping", mapping_parameter}};

  SphericalCamera actual;
  ASSERT_TRUE(RemoveSphericalCameraV3(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                shutteropen: 1.0
                shutterclose: 2.0
                lensradius: 3.0
                focaldistance: 4.0
                frameaspectratio: 5.0
                screenwindow { x_min: 6.0 x_max: 7.0 y_min: 8.0 y_max: 9.0 }
                mapping: EQUALAREA
              )pb"));
}

TEST(RemoveSphericalCameraV3, Equirectangular) {
  std::vector<absl::string_view> mapping = {"equirectangular"};
  Parameter mapping_parameter{.directive = "",
                              .type = ParameterType::STRING,
                              .type_name = "",
                              .values = absl::MakeSpan(mapping)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"mapping", mapping_parameter}};

  SphericalCamera actual;
  ASSERT_TRUE(RemoveSphericalCameraV3(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                mapping: EQUIRECTANGULAR
              )pb"));
}

TEST(RemoveSphericalCameraV3, InvalidMapping) {
  std::vector<absl::string_view> mapping = {"bad"};
  Parameter mapping_parameter{.directive = "",
                              .type = ParameterType::STRING,
                              .type_name = "",
                              .values = absl::MakeSpan(mapping)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"mapping", mapping_parameter}};

  SphericalCamera actual;
  EXPECT_THAT(RemoveSphericalCameraV3(parameters, actual),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "A spherical Camera specified an invalid 'mapping'"));
}

TEST(RemoveRealisticCameraV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  RealisticCamera actual;
  RemoveRealisticCameraV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveRealisticCameraV1, WithData) {
  std::vector<double> shutteropen = {1.0};
  Parameter shutteropen_parameter{.directive = "",
                                  .type = ParameterType::FLOAT,
                                  .type_name = "",
                                  .values = absl::MakeSpan(shutteropen)};

  std::vector<double> shutterclose = {2.0};
  Parameter shutterclose_parameter{.directive = "",
                                   .type = ParameterType::FLOAT,
                                   .type_name = "",
                                   .values = absl::MakeSpan(shutterclose)};

  std::vector<absl::string_view> lensfile = {"abc"};
  Parameter lensfile_parameter{.directive = "",
                               .type = ParameterType::STRING,
                               .type_name = "",
                               .values = absl::MakeSpan(lensfile)};

  std::vector<double> aperturediameter = {3.0};
  Parameter aperturediameter_parameter{
      .directive = "",
      .type = ParameterType::FLOAT,
      .type_name = "",
      .values = absl::MakeSpan(aperturediameter)};

  std::vector<absl::string_view> aperture = {"def"};
  Parameter aperture_parameter{.directive = "",
                               .type = ParameterType::STRING,
                               .type_name = "",
                               .values = absl::MakeSpan(aperture)};

  std::vector<double> focusdistance = {4.0};
  Parameter focusdistance_parameter{.directive = "",
                                    .type = ParameterType::FLOAT,
                                    .type_name = "",
                                    .values = absl::MakeSpan(focusdistance)};

  bool simpleweighting[] = {true};
  Parameter simpleweighting_parameter{
      .directive = "",
      .type = ParameterType::BOOL,
      .type_name = "",
      .values = absl::MakeSpan(simpleweighting)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"shutteropen", shutteropen_parameter},
      {"shutterclose", shutterclose_parameter},
      {"lensfile", lensfile_parameter},
      {"aperturediameter", aperturediameter_parameter},
      {"aperture", aperture_parameter},
      {"focusdistance", focusdistance_parameter},
      {"simpleweighting", simpleweighting_parameter}};

  RealisticCamera actual;
  RemoveRealisticCameraV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                shutteropen: 1.0
                shutterclose: 2.0
                lensfile: "abc"
                aperturediameter: 3.0
                focusdistance: 4.0
                simpleweighting: true
              )pb"));
}

TEST(RemoveRealisticCameraV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  RealisticCamera actual;
  RemoveRealisticCameraV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveRealisticCameraV2, WithData) {
  std::vector<double> shutteropen = {1.0};
  Parameter shutteropen_parameter{.directive = "",
                                  .type = ParameterType::FLOAT,
                                  .type_name = "",
                                  .values = absl::MakeSpan(shutteropen)};

  std::vector<double> shutterclose = {2.0};
  Parameter shutterclose_parameter{.directive = "",
                                   .type = ParameterType::FLOAT,
                                   .type_name = "",
                                   .values = absl::MakeSpan(shutterclose)};

  std::vector<absl::string_view> lensfile = {"abc"};
  Parameter lensfile_parameter{.directive = "",
                               .type = ParameterType::STRING,
                               .type_name = "",
                               .values = absl::MakeSpan(lensfile)};

  std::vector<double> aperturediameter = {3.0};
  Parameter aperturediameter_parameter{
      .directive = "",
      .type = ParameterType::FLOAT,
      .type_name = "",
      .values = absl::MakeSpan(aperturediameter)};

  std::vector<absl::string_view> aperture = {"def"};
  Parameter aperture_parameter{.directive = "",
                               .type = ParameterType::STRING,
                               .type_name = "",
                               .values = absl::MakeSpan(aperture)};

  std::vector<double> focusdistance = {4.0};
  Parameter focusdistance_parameter{.directive = "",
                                    .type = ParameterType::FLOAT,
                                    .type_name = "",
                                    .values = absl::MakeSpan(focusdistance)};

  bool simpleweighting[] = {true};
  Parameter simpleweighting_parameter{
      .directive = "",
      .type = ParameterType::BOOL,
      .type_name = "",
      .values = absl::MakeSpan(simpleweighting)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"shutteropen", shutteropen_parameter},
      {"shutterclose", shutterclose_parameter},
      {"lensfile", lensfile_parameter},
      {"aperturediameter", aperturediameter_parameter},
      {"aperture", aperture_parameter},
      {"focusdistance", focusdistance_parameter},
      {"simpleweighting", simpleweighting_parameter}};

  RealisticCamera actual;
  RemoveRealisticCameraV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                shutteropen: 1.0
                shutterclose: 2.0
                lensfile: "abc"
                aperturediameter: 3.0
                aperture: "def"
                focusdistance: 4.0
              )pb"));
}

}  // namespace
}  // namespace pbrt_proto
