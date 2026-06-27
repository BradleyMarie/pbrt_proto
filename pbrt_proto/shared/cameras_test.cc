#include "pbrt_proto/shared/cameras.h"

#include <array>
#include <cstdint>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "absl/status/status_matchers.h"
#include "absl/strings/string_view.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
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
  EXPECT_TRUE(
      RemoveOrthographicCamera(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(OrthographicCameraV1, WithData) {
  std::vector<double> shutteropen = {1.0};
  Parameter shutteropen_parameter{/*directive=*/"Camera",
                                  /*type=*/ParameterType::FLOAT,
                                  /*type_name=*/"",
                                  /*values=*/absl::MakeSpan(shutteropen)};

  std::vector<double> shutterclose = {2.0};
  Parameter shutterclose_parameter{/*directive=*/"Camera",
                                   /*type=*/ParameterType::FLOAT,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(shutterclose)};

  std::vector<double> focaldistance = {3.0};
  Parameter focaldistance_parameter{/*directive=*/"Camera",
                                    /*type=*/ParameterType::FLOAT,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(focaldistance)};

  std::vector<double> frameaspectratio = {4.0};
  Parameter frameaspectratio_parameter{
      /*directive=*/"Camera",
      /*type=*/ParameterType::FLOAT,
      /*type_name=*/"",
      /*values=*/absl::MakeSpan(frameaspectratio)};

  std::vector<double> screenwindow = {5.0, 6.0, 7.0, 8.0};
  Parameter screenwindow_parameter{/*directive=*/"Camera",
                                   /*type=*/ParameterType::FLOAT,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(screenwindow)};

  std::vector<double> hither = {9.0};
  Parameter hither_parameter{/*directive=*/"Camera",
                             /*type=*/ParameterType::FLOAT,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(hither)};

  std::vector<double> yon = {10.0};
  Parameter yon_parameter{/*directive=*/"Camera",
                          /*type=*/ParameterType::FLOAT,
                          /*type_name=*/"",
                          /*values=*/absl::MakeSpan(yon)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"shutteropen", shutteropen_parameter},
      {"shutterclose", shutterclose_parameter},
      {"focaldistance", focaldistance_parameter},
      {"frameaspectratio", frameaspectratio_parameter},
      {"screenwindow", screenwindow_parameter},
      {"hither", hither_parameter},
      {"yon", yon_parameter}};

  OrthographicCamera actual;
  EXPECT_TRUE(
      RemoveOrthographicCamera(parameters, /*pbrt_version=*/1, actual).ok());
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
  EXPECT_TRUE(
      RemoveOrthographicCamera(parameters, /*pbrt_version=*/2, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(OrthographicCameraV2, WithData) {
  std::vector<double> shutteropen = {1.0};
  Parameter shutteropen_parameter{/*directive=*/"Camera",
                                  /*type=*/ParameterType::FLOAT,
                                  /*type_name=*/"",
                                  /*values=*/absl::MakeSpan(shutteropen)};

  std::vector<double> shutterclose = {2.0};
  Parameter shutterclose_parameter{/*directive=*/"Camera",
                                   /*type=*/ParameterType::FLOAT,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(shutterclose)};

  std::vector<double> focaldistance = {3.0};
  Parameter focaldistance_parameter{/*directive=*/"Camera",
                                    /*type=*/ParameterType::FLOAT,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(focaldistance)};

  std::vector<double> frameaspectratio = {4.0};
  Parameter frameaspectratio_parameter{
      /*directive=*/"Camera",
      /*type=*/ParameterType::FLOAT,
      /*type_name=*/"",
      /*values=*/absl::MakeSpan(frameaspectratio)};

  std::vector<double> screenwindow = {5.0, 6.0, 7.0, 8.0};
  Parameter screenwindow_parameter{/*directive=*/"Camera",
                                   /*type=*/ParameterType::FLOAT,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(screenwindow)};

  std::vector<double> hither = {9.0};
  Parameter hither_parameter{/*directive=*/"Camera",
                             /*type=*/ParameterType::FLOAT,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(hither)};

  std::vector<double> yon = {10.0};
  Parameter yon_parameter{/*directive=*/"Camera",
                          /*type=*/ParameterType::FLOAT,
                          /*type_name=*/"",
                          /*values=*/absl::MakeSpan(yon)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"shutteropen", shutteropen_parameter},
      {"shutterclose", shutterclose_parameter},
      {"focaldistance", focaldistance_parameter},
      {"frameaspectratio", frameaspectratio_parameter},
      {"screenwindow", screenwindow_parameter},
      {"hither", hither_parameter},
      {"yon", yon_parameter}};

  OrthographicCamera actual;
  EXPECT_TRUE(
      RemoveOrthographicCamera(parameters, /*pbrt_version=*/2, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                shutteropen: 1.0
                shutterclose: 2.0
                focaldistance: 3.0
                frameaspectratio: 4.0
                screenwindow { x_min: 5.0 x_max: 6.0 y_min: 7.0 y_max: 8.0 }
              )pb"));
}

TEST(OrthographicCameraV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  OrthographicCamera actual;
  EXPECT_TRUE(
      RemoveOrthographicCamera(parameters, /*pbrt_version=*/3, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(OrthographicCameraV3, WithData) {
  std::vector<double> shutteropen = {1.0};
  Parameter shutteropen_parameter{/*directive=*/"Camera",
                                  /*type=*/ParameterType::FLOAT,
                                  /*type_name=*/"",
                                  /*values=*/absl::MakeSpan(shutteropen)};

  std::vector<double> shutterclose = {2.0};
  Parameter shutterclose_parameter{/*directive=*/"Camera",
                                   /*type=*/ParameterType::FLOAT,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(shutterclose)};

  std::vector<double> focaldistance = {3.0};
  Parameter focaldistance_parameter{/*directive=*/"Camera",
                                    /*type=*/ParameterType::FLOAT,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(focaldistance)};

  std::vector<double> frameaspectratio = {4.0};
  Parameter frameaspectratio_parameter{
      /*directive=*/"Camera",
      /*type=*/ParameterType::FLOAT,
      /*type_name=*/"",
      /*values=*/absl::MakeSpan(frameaspectratio)};

  std::vector<double> screenwindow = {5.0, 6.0, 7.0, 8.0};
  Parameter screenwindow_parameter{/*directive=*/"Camera",
                                   /*type=*/ParameterType::FLOAT,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(screenwindow)};

  std::vector<double> hither = {9.0};
  Parameter hither_parameter{/*directive=*/"Camera",
                             /*type=*/ParameterType::FLOAT,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(hither)};

  std::vector<double> yon = {10.0};
  Parameter yon_parameter{/*directive=*/"Camera",
                          /*type=*/ParameterType::FLOAT,
                          /*type_name=*/"",
                          /*values=*/absl::MakeSpan(yon)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"shutteropen", shutteropen_parameter},
      {"shutterclose", shutterclose_parameter},
      {"focaldistance", focaldistance_parameter},
      {"frameaspectratio", frameaspectratio_parameter},
      {"screenwindow", screenwindow_parameter},
      {"hither", hither_parameter},
      {"yon", yon_parameter}};

  OrthographicCamera actual;
  EXPECT_TRUE(
      RemoveOrthographicCamera(parameters, /*pbrt_version=*/3, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                shutteropen: 1.0
                shutterclose: 2.0
                focaldistance: 3.0
                frameaspectratio: 4.0
                screenwindow { x_min: 5.0 x_max: 6.0 y_min: 7.0 y_max: 8.0 }
              )pb"));
}

TEST(OrthographicCameraV4, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  OrthographicCamera actual;
  EXPECT_TRUE(
      RemoveOrthographicCamera(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(OrthographicCameraV4, WithData) {
  std::vector<double> shutteropen = {1.0};
  Parameter shutteropen_parameter{/*directive=*/"Camera",
                                  /*type=*/ParameterType::FLOAT,
                                  /*type_name=*/"",
                                  /*values=*/absl::MakeSpan(shutteropen)};

  std::vector<double> shutterclose = {2.0};
  Parameter shutterclose_parameter{/*directive=*/"Camera",
                                   /*type=*/ParameterType::FLOAT,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(shutterclose)};

  std::vector<double> focaldistance = {3.0};
  Parameter focaldistance_parameter{/*directive=*/"Camera",
                                    /*type=*/ParameterType::FLOAT,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(focaldistance)};

  std::vector<double> frameaspectratio = {4.0};
  Parameter frameaspectratio_parameter{
      /*directive=*/"Camera",
      /*type=*/ParameterType::FLOAT,
      /*type_name=*/"",
      /*values=*/absl::MakeSpan(frameaspectratio)};

  std::vector<double> screenwindow = {5.0, 6.0, 7.0, 8.0};
  Parameter screenwindow_parameter{/*directive=*/"Camera",
                                   /*type=*/ParameterType::FLOAT,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(screenwindow)};

  std::vector<double> hither = {9.0};
  Parameter hither_parameter{/*directive=*/"Camera",
                             /*type=*/ParameterType::FLOAT,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(hither)};

  std::vector<double> yon = {10.0};
  Parameter yon_parameter{/*directive=*/"Camera",
                          /*type=*/ParameterType::FLOAT,
                          /*type_name=*/"",
                          /*values=*/absl::MakeSpan(yon)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"shutteropen", shutteropen_parameter},
      {"shutterclose", shutterclose_parameter},
      {"focaldistance", focaldistance_parameter},
      {"frameaspectratio", frameaspectratio_parameter},
      {"screenwindow", screenwindow_parameter},
      {"hither", hither_parameter},
      {"yon", yon_parameter}};

  OrthographicCamera actual;
  EXPECT_TRUE(
      RemoveOrthographicCamera(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                shutteropen: 1.0
                shutterclose: 2.0
                focaldistance: 3.0
                frameaspectratio: 4.0
                screenwindow { x_min: 5.0 x_max: 6.0 y_min: 7.0 y_max: 8.0 }
              )pb"));
}

TEST(RemovePerspectiveCameraV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  PerspectiveCamera actual;
  EXPECT_TRUE(
      RemovePerspectiveCamera(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemovePerspectiveCameraV1, WithData) {
  std::vector<double> shutteropen = {1.0};
  Parameter shutteropen_parameter{/*directive=*/"Camera",
                                  /*type=*/ParameterType::FLOAT,
                                  /*type_name=*/"",
                                  /*values=*/absl::MakeSpan(shutteropen)};

  std::vector<double> shutterclose = {2.0};
  Parameter shutterclose_parameter{/*directive=*/"Camera",
                                   /*type=*/ParameterType::FLOAT,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(shutterclose)};

  std::vector<double> fov = {3.0};
  Parameter fov_parameter{/*directive=*/"Camera",
                          /*type=*/ParameterType::FLOAT,
                          /*type_name=*/"",
                          /*values=*/absl::MakeSpan(fov)};

  std::vector<double> focaldistance = {4.0};
  Parameter focaldistance_parameter{/*directive=*/"Camera",
                                    /*type=*/ParameterType::FLOAT,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(focaldistance)};

  std::vector<double> frameaspectratio = {5.0};
  Parameter frameaspectratio_parameter{
      /*directive=*/"Camera",
      /*type=*/ParameterType::FLOAT,
      /*type_name=*/"",
      /*values=*/absl::MakeSpan(frameaspectratio)};

  std::vector<double> screenwindow = {6.0, 7.0, 8.0, 9.0};
  Parameter screenwindow_parameter{/*directive=*/"Camera",
                                   /*type=*/ParameterType::FLOAT,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(screenwindow)};

  std::vector<double> hither = {10.0};
  Parameter hither_parameter{/*directive=*/"Camera",
                             /*type=*/ParameterType::FLOAT,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(hither)};

  std::vector<double> yon = {11.0};
  Parameter yon_parameter{/*directive=*/"Camera",
                          /*type=*/ParameterType::FLOAT,
                          /*type_name=*/"",
                          /*values=*/absl::MakeSpan(yon)};

  std::vector<double> halffov = {6.0};
  Parameter halffov_parameter{/*directive=*/"Camera",
                              /*type=*/ParameterType::FLOAT,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(halffov)};

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
  EXPECT_TRUE(
      RemovePerspectiveCamera(parameters, /*pbrt_version=*/1, actual).ok());
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

TEST(RemovePerspectiveCameraV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  PerspectiveCamera actual;
  EXPECT_TRUE(
      RemovePerspectiveCamera(parameters, /*pbrt_version=*/2, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemovePerspectiveCameraV2, WithData) {
  std::vector<double> shutteropen = {1.0};
  Parameter shutteropen_parameter{/*directive=*/"Camera",
                                  /*type=*/ParameterType::FLOAT,
                                  /*type_name=*/"",
                                  /*values=*/absl::MakeSpan(shutteropen)};

  std::vector<double> shutterclose = {2.0};
  Parameter shutterclose_parameter{/*directive=*/"Camera",
                                   /*type=*/ParameterType::FLOAT,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(shutterclose)};

  std::vector<double> fov = {3.0};
  Parameter fov_parameter{/*directive=*/"Camera",
                          /*type=*/ParameterType::FLOAT,
                          /*type_name=*/"",
                          /*values=*/absl::MakeSpan(fov)};

  std::vector<double> focaldistance = {4.0};
  Parameter focaldistance_parameter{/*directive=*/"Camera",
                                    /*type=*/ParameterType::FLOAT,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(focaldistance)};

  std::vector<double> frameaspectratio = {5.0};
  Parameter frameaspectratio_parameter{
      /*directive=*/"Camera",
      /*type=*/ParameterType::FLOAT,
      /*type_name=*/"",
      /*values=*/absl::MakeSpan(frameaspectratio)};

  std::vector<double> screenwindow = {6.0, 7.0, 8.0, 9.0};
  Parameter screenwindow_parameter{/*directive=*/"Camera",
                                   /*type=*/ParameterType::FLOAT,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(screenwindow)};

  std::vector<double> hither = {10.0};
  Parameter hither_parameter{/*directive=*/"Camera",
                             /*type=*/ParameterType::FLOAT,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(hither)};

  std::vector<double> yon = {11.0};
  Parameter yon_parameter{/*directive=*/"Camera",
                          /*type=*/ParameterType::FLOAT,
                          /*type_name=*/"",
                          /*values=*/absl::MakeSpan(yon)};

  std::vector<double> halffov = {6.0};
  Parameter halffov_parameter{/*directive=*/"Camera",
                              /*type=*/ParameterType::FLOAT,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(halffov)};

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
  EXPECT_TRUE(
      RemovePerspectiveCamera(parameters, /*pbrt_version=*/2, actual).ok());
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
  Parameter halffov_parameter{/*directive=*/"Camera",
                              /*type=*/ParameterType::FLOAT,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(halffov)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"halffov", halffov_parameter}};

  PerspectiveCamera actual;
  EXPECT_TRUE(
      RemovePerspectiveCamera(parameters, /*pbrt_version=*/2, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemovePerspectiveCameraV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  PerspectiveCamera actual;
  EXPECT_TRUE(
      RemovePerspectiveCamera(parameters, /*pbrt_version=*/3, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemovePerspectiveCameraV3, WithData) {
  std::vector<double> shutteropen = {1.0};
  Parameter shutteropen_parameter{/*directive=*/"Camera",
                                  /*type=*/ParameterType::FLOAT,
                                  /*type_name=*/"",
                                  /*values=*/absl::MakeSpan(shutteropen)};

  std::vector<double> shutterclose = {2.0};
  Parameter shutterclose_parameter{/*directive=*/"Camera",
                                   /*type=*/ParameterType::FLOAT,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(shutterclose)};

  std::vector<double> fov = {3.0};
  Parameter fov_parameter{/*directive=*/"Camera",
                          /*type=*/ParameterType::FLOAT,
                          /*type_name=*/"",
                          /*values=*/absl::MakeSpan(fov)};

  std::vector<double> focaldistance = {4.0};
  Parameter focaldistance_parameter{/*directive=*/"Camera",
                                    /*type=*/ParameterType::FLOAT,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(focaldistance)};

  std::vector<double> frameaspectratio = {5.0};
  Parameter frameaspectratio_parameter{
      /*directive=*/"Camera",
      /*type=*/ParameterType::FLOAT,
      /*type_name=*/"",
      /*values=*/absl::MakeSpan(frameaspectratio)};

  std::vector<double> screenwindow = {6.0, 7.0, 8.0, 9.0};
  Parameter screenwindow_parameter{/*directive=*/"Camera",
                                   /*type=*/ParameterType::FLOAT,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(screenwindow)};

  std::vector<double> hither = {10.0};
  Parameter hither_parameter{/*directive=*/"Camera",
                             /*type=*/ParameterType::FLOAT,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(hither)};

  std::vector<double> yon = {11.0};
  Parameter yon_parameter{/*directive=*/"Camera",
                          /*type=*/ParameterType::FLOAT,
                          /*type_name=*/"",
                          /*values=*/absl::MakeSpan(yon)};

  std::vector<double> halffov = {6.0};
  Parameter halffov_parameter{/*directive=*/"Camera",
                              /*type=*/ParameterType::FLOAT,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(halffov)};

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
  EXPECT_TRUE(
      RemovePerspectiveCamera(parameters, /*pbrt_version=*/3, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                shutteropen: 1.0
                shutterclose: 2.0
                fov: 12.0
                focaldistance: 4.0
                frameaspectratio: 5.0
                screenwindow { x_min: 6.0 x_max: 7.0 y_min: 8.0 y_max: 9.0 }
              )pb"));
}

TEST(RemovePerspectiveCameraV3, ZeroHalfFov) {
  std::vector<double> halffov = {0.0};
  Parameter halffov_parameter{/*directive=*/"Camera",
                              /*type=*/ParameterType::FLOAT,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(halffov)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"halffov", halffov_parameter}};

  PerspectiveCamera actual;
  EXPECT_TRUE(
      RemovePerspectiveCamera(parameters, /*pbrt_version=*/3, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemovePerspectiveCameraV4, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  PerspectiveCamera actual;
  EXPECT_TRUE(
      RemovePerspectiveCamera(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemovePerspectiveCameraV4, WithData) {
  std::vector<double> shutteropen = {1.0};
  Parameter shutteropen_parameter{/*directive=*/"Camera",
                                  /*type=*/ParameterType::FLOAT,
                                  /*type_name=*/"",
                                  /*values=*/absl::MakeSpan(shutteropen)};

  std::vector<double> shutterclose = {2.0};
  Parameter shutterclose_parameter{/*directive=*/"Camera",
                                   /*type=*/ParameterType::FLOAT,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(shutterclose)};

  std::vector<double> fov = {3.0};
  Parameter fov_parameter{/*directive=*/"Camera",
                          /*type=*/ParameterType::FLOAT,
                          /*type_name=*/"",
                          /*values=*/absl::MakeSpan(fov)};

  std::vector<double> focaldistance = {4.0};
  Parameter focaldistance_parameter{/*directive=*/"Camera",
                                    /*type=*/ParameterType::FLOAT,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(focaldistance)};

  std::vector<double> frameaspectratio = {5.0};
  Parameter frameaspectratio_parameter{
      /*directive=*/"Camera",
      /*type=*/ParameterType::FLOAT,
      /*type_name=*/"",
      /*values=*/absl::MakeSpan(frameaspectratio)};

  std::vector<double> screenwindow = {6.0, 7.0, 8.0, 9.0};
  Parameter screenwindow_parameter{/*directive=*/"Camera",
                                   /*type=*/ParameterType::FLOAT,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(screenwindow)};

  std::vector<double> hither = {10.0};
  Parameter hither_parameter{/*directive=*/"Camera",
                             /*type=*/ParameterType::FLOAT,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(hither)};

  std::vector<double> yon = {11.0};
  Parameter yon_parameter{/*directive=*/"Camera",
                          /*type=*/ParameterType::FLOAT,
                          /*type_name=*/"",
                          /*values=*/absl::MakeSpan(yon)};

  std::vector<double> halffov = {6.0};
  Parameter halffov_parameter{/*directive=*/"Camera",
                              /*type=*/ParameterType::FLOAT,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(halffov)};

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
  EXPECT_TRUE(
      RemovePerspectiveCamera(parameters, /*pbrt_version=*/4, actual).ok());
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
  EXPECT_TRUE(
      RemoveSphericalCamera(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveSphericalCameraV1, WithData) {
  std::vector<double> shutteropen = {1.0};
  Parameter shutteropen_parameter{/*directive=*/"Camera",
                                  /*type=*/ParameterType::FLOAT,
                                  /*type_name=*/"",
                                  /*values=*/absl::MakeSpan(shutteropen)};

  std::vector<double> shutterclose = {2.0};
  Parameter shutterclose_parameter{/*directive=*/"Camera",
                                   /*type=*/ParameterType::FLOAT,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(shutterclose)};

  std::vector<double> lensradius = {3.0};
  Parameter lensradius_parameter{/*directive=*/"Camera",
                                 /*type=*/ParameterType::FLOAT,
                                 /*type_name=*/"",
                                 /*values=*/absl::MakeSpan(lensradius)};

  std::vector<double> focaldistance = {4.0};
  Parameter focaldistance_parameter{/*directive=*/"Camera",
                                    /*type=*/ParameterType::FLOAT,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(focaldistance)};

  std::vector<double> frameaspectratio = {5.0};
  Parameter frameaspectratio_parameter{
      /*directive=*/"Camera",
      /*type=*/ParameterType::FLOAT,
      /*type_name=*/"",
      /*values=*/absl::MakeSpan(frameaspectratio)};

  std::vector<double> screenwindow = {6.0, 7.0, 8.0, 9.0};
  Parameter screenwindow_parameter{/*directive=*/"Camera",
                                   /*type=*/ParameterType::FLOAT,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(screenwindow)};

  std::vector<double> hither = {10.0};
  Parameter hither_parameter{/*directive=*/"Camera",
                             /*type=*/ParameterType::FLOAT,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(hither)};

  std::vector<double> yon = {11.0};
  Parameter yon_parameter{/*directive=*/"Camera",
                          /*type=*/ParameterType::FLOAT,
                          /*type_name=*/"",
                          /*values=*/absl::MakeSpan(yon)};

  std::vector<absl::string_view> mapping = {"equalarea"};
  Parameter mapping_parameter{/*directive=*/"Camera",
                              /*type=*/ParameterType::FLOAT,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(mapping)};

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
  EXPECT_TRUE(
      RemoveSphericalCamera(parameters, /*pbrt_version=*/1, actual).ok());
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
  EXPECT_TRUE(
      RemoveSphericalCamera(parameters, /*pbrt_version=*/2, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveSphericalCameraV2, WithData) {
  std::vector<double> shutteropen = {1.0};
  Parameter shutteropen_parameter{/*directive=*/"Camera",
                                  /*type=*/ParameterType::FLOAT,
                                  /*type_name=*/"",
                                  /*values=*/absl::MakeSpan(shutteropen)};

  std::vector<double> shutterclose = {2.0};
  Parameter shutterclose_parameter{/*directive=*/"Camera",
                                   /*type=*/ParameterType::FLOAT,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(shutterclose)};

  std::vector<double> lensradius = {3.0};
  Parameter lensradius_parameter{/*directive=*/"Camera",
                                 /*type=*/ParameterType::FLOAT,
                                 /*type_name=*/"",
                                 /*values=*/absl::MakeSpan(lensradius)};

  std::vector<double> focaldistance = {4.0};
  Parameter focaldistance_parameter{/*directive=*/"Camera",
                                    /*type=*/ParameterType::FLOAT,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(focaldistance)};

  std::vector<double> frameaspectratio = {5.0};
  Parameter frameaspectratio_parameter{
      /*directive=*/"Camera",
      /*type=*/ParameterType::FLOAT,
      /*type_name=*/"",
      /*values=*/absl::MakeSpan(frameaspectratio)};

  std::vector<double> screenwindow = {6.0, 7.0, 8.0, 9.0};
  Parameter screenwindow_parameter{/*directive=*/"Camera",
                                   /*type=*/ParameterType::FLOAT,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(screenwindow)};

  std::vector<double> hither = {10.0};
  Parameter hither_parameter{/*directive=*/"Camera",
                             /*type=*/ParameterType::FLOAT,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(hither)};

  std::vector<double> yon = {11.0};
  Parameter yon_parameter{/*directive=*/"Camera",
                          /*type=*/ParameterType::FLOAT,
                          /*type_name=*/"",
                          /*values=*/absl::MakeSpan(yon)};

  std::vector<absl::string_view> mapping = {"equalarea"};
  Parameter mapping_parameter{/*directive=*/"Camera",
                              /*type=*/ParameterType::FLOAT,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(mapping)};

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
  EXPECT_TRUE(
      RemoveSphericalCamera(parameters, /*pbrt_version=*/2, actual).ok());
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
  EXPECT_TRUE(
      RemoveSphericalCamera(parameters, /*pbrt_version=*/3, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveSphericalCameraV3, WithData) {
  std::vector<double> shutteropen = {1.0};
  Parameter shutteropen_parameter{/*directive=*/"Camera",
                                  /*type=*/ParameterType::FLOAT,
                                  /*type_name=*/"",
                                  /*values=*/absl::MakeSpan(shutteropen)};

  std::vector<double> shutterclose = {2.0};
  Parameter shutterclose_parameter{/*directive=*/"Camera",
                                   /*type=*/ParameterType::FLOAT,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(shutterclose)};

  std::vector<double> lensradius = {3.0};
  Parameter lensradius_parameter{/*directive=*/"Camera",
                                 /*type=*/ParameterType::FLOAT,
                                 /*type_name=*/"",
                                 /*values=*/absl::MakeSpan(lensradius)};

  std::vector<double> focaldistance = {4.0};
  Parameter focaldistance_parameter{/*directive=*/"Camera",
                                    /*type=*/ParameterType::FLOAT,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(focaldistance)};

  std::vector<double> frameaspectratio = {5.0};
  Parameter frameaspectratio_parameter{
      /*directive=*/"Camera",
      /*type=*/ParameterType::FLOAT,
      /*type_name=*/"",
      /*values=*/absl::MakeSpan(frameaspectratio)};

  std::vector<double> screenwindow = {6.0, 7.0, 8.0, 9.0};
  Parameter screenwindow_parameter{/*directive=*/"Camera",
                                   /*type=*/ParameterType::FLOAT,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(screenwindow)};

  std::vector<double> hither = {10.0};
  Parameter hither_parameter{/*directive=*/"Camera",
                             /*type=*/ParameterType::FLOAT,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(hither)};

  std::vector<double> yon = {11.0};
  Parameter yon_parameter{/*directive=*/"Camera",
                          /*type=*/ParameterType::FLOAT,
                          /*type_name=*/"",
                          /*values=*/absl::MakeSpan(yon)};

  std::vector<absl::string_view> mapping = {"equalarea"};
  Parameter mapping_parameter{/*directive=*/"Camera",
                              /*type=*/ParameterType::FLOAT,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(mapping)};

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
  EXPECT_TRUE(
      RemoveSphericalCamera(parameters, /*pbrt_version=*/3, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                shutteropen: 1.0
                shutterclose: 2.0
                lensradius: 3.0
                focaldistance: 4.0
                frameaspectratio: 5.0
                screenwindow { x_min: 6.0 x_max: 7.0 y_min: 8.0 y_max: 9.0 }
              )pb"));
}

TEST(RemoveSphericalCameraV4, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  SphericalCamera actual;
  EXPECT_TRUE(
      RemoveSphericalCamera(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveSphericalCameraV4, WithData) {
  std::vector<double> shutteropen = {1.0};
  Parameter shutteropen_parameter{/*directive=*/"Camera",
                                  /*type=*/ParameterType::FLOAT,
                                  /*type_name=*/"",
                                  /*values=*/absl::MakeSpan(shutteropen)};

  std::vector<double> shutterclose = {2.0};
  Parameter shutterclose_parameter{/*directive=*/"Camera",
                                   /*type=*/ParameterType::FLOAT,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(shutterclose)};

  std::vector<double> lensradius = {3.0};
  Parameter lensradius_parameter{/*directive=*/"Camera",
                                 /*type=*/ParameterType::FLOAT,
                                 /*type_name=*/"",
                                 /*values=*/absl::MakeSpan(lensradius)};

  std::vector<double> focaldistance = {4.0};
  Parameter focaldistance_parameter{/*directive=*/"Camera",
                                    /*type=*/ParameterType::FLOAT,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(focaldistance)};

  std::vector<double> frameaspectratio = {5.0};
  Parameter frameaspectratio_parameter{
      /*directive=*/"Camera",
      /*type=*/ParameterType::FLOAT,
      /*type_name=*/"",
      /*values=*/absl::MakeSpan(frameaspectratio)};

  std::vector<double> screenwindow = {6.0, 7.0, 8.0, 9.0};
  Parameter screenwindow_parameter{/*directive=*/"Camera",
                                   /*type=*/ParameterType::FLOAT,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(screenwindow)};

  std::vector<double> hither = {10.0};
  Parameter hither_parameter{/*directive=*/"Camera",
                             /*type=*/ParameterType::FLOAT,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(hither)};

  std::vector<double> yon = {11.0};
  Parameter yon_parameter{/*directive=*/"Camera",
                          /*type=*/ParameterType::FLOAT,
                          /*type_name=*/"",
                          /*values=*/absl::MakeSpan(yon)};

  std::vector<absl::string_view> mapping = {"equalarea"};
  Parameter mapping_parameter{/*directive=*/"Camera",
                              /*type=*/ParameterType::STRING,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(mapping)};

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
  EXPECT_TRUE(
      RemoveSphericalCamera(parameters, /*pbrt_version=*/4, actual).ok());
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

TEST(RemoveSphericalCameraV4, Equirectangular) {
  std::vector<absl::string_view> mapping = {"equirectangular"};
  Parameter mapping_parameter{/*directive=*/"Camera",
                              /*type=*/ParameterType::STRING,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(mapping)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"mapping", mapping_parameter}};

  SphericalCamera actual;
  EXPECT_TRUE(
      RemoveSphericalCamera(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                mapping: EQUIRECTANGULAR
              )pb"));
}

TEST(RemoveSphericalCameraV4, InvalidMapping) {
  std::vector<absl::string_view> mapping = {"bad"};
  Parameter mapping_parameter{/*directive=*/"Camera",
                              /*type=*/ParameterType::STRING,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(mapping)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"mapping", mapping_parameter}};

  SphericalCamera actual;
  EXPECT_THAT(RemoveSphericalCamera(parameters, /*pbrt_version=*/4, actual),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Unsupported value for 'spherical' Camera parameter "
                       "'mapping' in PBRTv4: \"bad\""));
}

TEST(RemoveRealisticCameraV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  RealisticCamera actual;
  EXPECT_TRUE(
      RemoveRealisticCamera(parameters, /*pbrt_version=*/3, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveRealisticCameraV3, WithData) {
  std::vector<double> shutteropen = {1.0};
  Parameter shutteropen_parameter{/*directive=*/"Camera",
                                  /*type=*/ParameterType::FLOAT,
                                  /*type_name=*/"",
                                  /*values=*/absl::MakeSpan(shutteropen)};

  std::vector<double> shutterclose = {2.0};
  Parameter shutterclose_parameter{/*directive=*/"Camera",
                                   /*type=*/ParameterType::FLOAT,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(shutterclose)};

  std::vector<absl::string_view> lensfile = {"abc"};
  Parameter lensfile_parameter{/*directive=*/"Camera",
                               /*type=*/ParameterType::STRING,
                               /*type_name=*/"",
                               /*values=*/absl::MakeSpan(lensfile)};

  std::vector<double> aperturediameter = {3.0};
  Parameter aperturediameter_parameter{
      /*directive=*/"Camera",
      /*type=*/ParameterType::FLOAT,
      /*type_name=*/"",
      /*values=*/absl::MakeSpan(aperturediameter)};

  std::vector<absl::string_view> aperture = {"def"};
  Parameter aperture_parameter{/*directive=*/"Camera",
                               /*type=*/ParameterType::STRING,
                               /*type_name=*/"",
                               /*values=*/absl::MakeSpan(aperture)};

  std::vector<double> focusdistance = {4.0};
  Parameter focusdistance_parameter{/*directive=*/"Camera",
                                    /*type=*/ParameterType::FLOAT,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(focusdistance)};

  bool simpleweighting[] = {true};
  Parameter simpleweighting_parameter{
      /*directive=*/"Camera",
      /*type=*/ParameterType::BOOL,
      /*type_name=*/"",
      /*values=*/absl::MakeSpan(simpleweighting)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"shutteropen", shutteropen_parameter},
      {"shutterclose", shutterclose_parameter},
      {"lensfile", lensfile_parameter},
      {"aperturediameter", aperturediameter_parameter},
      {"aperture", aperture_parameter},
      {"focusdistance", focusdistance_parameter},
      {"simpleweighting", simpleweighting_parameter}};

  RealisticCamera actual;
  EXPECT_TRUE(
      RemoveRealisticCamera(parameters, /*pbrt_version=*/3, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                shutteropen: 1.0
                shutterclose: 2.0
                lensfile: "abc"
                aperturediameter: 3.0
                focusdistance: 4.0
                simpleweighting: true
              )pb"));
}

TEST(RemoveRealisticCameraV4, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  RealisticCamera actual;
  EXPECT_TRUE(
      RemoveRealisticCamera(parameters, /*pbrt_version=*/4, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveRealisticCameraV4, WithData) {
  std::vector<double> shutteropen = {1.0};
  Parameter shutteropen_parameter{/*directive=*/"Camera",
                                  /*type=*/ParameterType::FLOAT,
                                  /*type_name=*/"",
                                  /*values=*/absl::MakeSpan(shutteropen)};

  std::vector<double> shutterclose = {2.0};
  Parameter shutterclose_parameter{/*directive=*/"Camera",
                                   /*type=*/ParameterType::FLOAT,
                                   /*type_name=*/"",
                                   /*values=*/absl::MakeSpan(shutterclose)};

  std::vector<absl::string_view> lensfile = {"abc"};
  Parameter lensfile_parameter{/*directive=*/"Camera",
                               /*type=*/ParameterType::STRING,
                               /*type_name=*/"",
                               /*values=*/absl::MakeSpan(lensfile)};

  std::vector<double> aperturediameter = {3.0};
  Parameter aperturediameter_parameter{
      /*directive=*/"Camera",
      /*type=*/ParameterType::FLOAT,
      /*type_name=*/"",
      /*values=*/absl::MakeSpan(aperturediameter)};

  std::vector<absl::string_view> aperture = {"def"};
  Parameter aperture_parameter{/*directive=*/"Camera",
                               /*type=*/ParameterType::STRING,
                               /*type_name=*/"",
                               /*values=*/absl::MakeSpan(aperture)};

  std::vector<double> focusdistance = {4.0};
  Parameter focusdistance_parameter{/*directive=*/"Camera",
                                    /*type=*/ParameterType::FLOAT,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(focusdistance)};

  bool simpleweighting[] = {true};
  Parameter simpleweighting_parameter{
      /*directive=*/"Camera",
      /*type=*/ParameterType::BOOL,
      /*type_name=*/"",
      /*values=*/absl::MakeSpan(simpleweighting)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"shutteropen", shutteropen_parameter},
      {"shutterclose", shutterclose_parameter},
      {"lensfile", lensfile_parameter},
      {"aperturediameter", aperturediameter_parameter},
      {"aperture", aperture_parameter},
      {"focusdistance", focusdistance_parameter},
      {"simpleweighting", simpleweighting_parameter}};

  RealisticCamera actual;
  EXPECT_TRUE(
      RemoveRealisticCamera(parameters, /*pbrt_version=*/4, actual).ok());
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
