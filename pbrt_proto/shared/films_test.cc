#include "pbrt_proto/shared/films.h"

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

TEST(RemoveRgbFilmV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  RgbFilm actual;
  RemoveRgbFilmV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveRgbFilmV1, WithData) {
  std::vector<absl::string_view> filename = {"file"};
  Parameter filename_parameter{.directive = "",
                               .type = ParameterType::STRING,
                               .type_name = "",
                               .values = absl::MakeSpan(filename)};

  std::vector<int32_t> xresolution = {1};
  Parameter xresolution_parameter{.directive = "",
                                  .type = ParameterType::INTEGER,
                                  .type_name = "",
                                  .values = absl::MakeSpan(xresolution)};

  std::vector<int32_t> yresolution = {2};
  Parameter yresolution_parameter{.directive = "",
                                  .type = ParameterType::INTEGER,
                                  .type_name = "",
                                  .values = absl::MakeSpan(yresolution)};

  std::vector<double> cropwindow = {3.0, 4.0, 5.0, 6.0};
  Parameter cropwindow_parameter{.directive = "",
                                 .type = ParameterType::FLOAT,
                                 .type_name = "",
                                 .values = absl::MakeSpan(cropwindow)};

  bool premultiplyalpha[] = {true};
  Parameter premultiplyalpha_parameter{
      .directive = "",
      .type = ParameterType::BOOL,
      .type_name = "",
      .values = absl::MakeSpan(premultiplyalpha)};

  std::vector<int32_t> writefrequency = {7};
  Parameter writefrequency_parameter{.directive = "",
                                     .type = ParameterType::INTEGER,
                                     .type_name = "",
                                     .values = absl::MakeSpan(writefrequency)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"filename", filename_parameter},
      {"xresolution", xresolution_parameter},
      {"yresolution", yresolution_parameter},
      {"cropwindow", cropwindow_parameter},
      {"premultiplyalpha", premultiplyalpha_parameter},
      {"writefrequency", writefrequency_parameter}};

  RgbFilm actual;
  RemoveRgbFilmV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                filename: "file"
                xresolution: 1
                yresolution: 2
                cropwindow { x_min: 3.0 x_max: 4.0 y_min: 5.0 y_max: 6.0 }
                premultiplyalpha: true
                writefrequency: 7
              )pb"));
}

TEST(RemoveRgbFilmV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  RgbFilm actual;
  RemoveRgbFilmV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveRgbFilmV2, WithData) {
  std::vector<absl::string_view> filename = {"file"};
  Parameter filename_parameter{.directive = "",
                               .type = ParameterType::STRING,
                               .type_name = "",
                               .values = absl::MakeSpan(filename)};

  std::vector<int32_t> xresolution = {1};
  Parameter xresolution_parameter{.directive = "",
                                  .type = ParameterType::INTEGER,
                                  .type_name = "",
                                  .values = absl::MakeSpan(xresolution)};

  std::vector<int32_t> yresolution = {2};
  Parameter yresolution_parameter{.directive = "",
                                  .type = ParameterType::INTEGER,
                                  .type_name = "",
                                  .values = absl::MakeSpan(yresolution)};

  std::vector<double> cropwindow = {3.0, 4.0, 5.0, 6.0};
  Parameter cropwindow_parameter{.directive = "",
                                 .type = ParameterType::FLOAT,
                                 .type_name = "",
                                 .values = absl::MakeSpan(cropwindow)};

  bool display[] = {true};
  Parameter display_parameter{.directive = "",
                              .type = ParameterType::BOOL,
                              .type_name = "",
                              .values = absl::MakeSpan(display)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"filename", filename_parameter},
      {"xresolution", xresolution_parameter},
      {"yresolution", yresolution_parameter},
      {"cropwindow", cropwindow_parameter},
      {"display", display_parameter}};

  RgbFilm actual;
  RemoveRgbFilmV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                filename: "file"
                xresolution: 1
                yresolution: 2
                cropwindow { x_min: 3.0 x_max: 4.0 y_min: 5.0 y_max: 6.0 }
                display: true
              )pb"));
}

TEST(RemoveRgbFilmV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  RgbFilm actual;
  RemoveRgbFilmV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveRgbFilmV3, WithData) {
  std::vector<absl::string_view> filename = {"file"};
  Parameter filename_parameter{.directive = "",
                               .type = ParameterType::STRING,
                               .type_name = "",
                               .values = absl::MakeSpan(filename)};

  std::vector<int32_t> xresolution = {1};
  Parameter xresolution_parameter{.directive = "",
                                  .type = ParameterType::INTEGER,
                                  .type_name = "",
                                  .values = absl::MakeSpan(xresolution)};

  std::vector<int32_t> yresolution = {2};
  Parameter yresolution_parameter{.directive = "",
                                  .type = ParameterType::INTEGER,
                                  .type_name = "",
                                  .values = absl::MakeSpan(yresolution)};

  std::vector<double> cropwindow = {3.0, 4.0, 5.0, 6.0};
  Parameter cropwindow_parameter{.directive = "",
                                 .type = ParameterType::FLOAT,
                                 .type_name = "",
                                 .values = absl::MakeSpan(cropwindow)};

  std::vector<double> diagonal = {7.0};
  Parameter diagonal_parameter{.directive = "",
                               .type = ParameterType::FLOAT,
                               .type_name = "",
                               .values = absl::MakeSpan(diagonal)};

  std::vector<double> scale = {8.0};
  Parameter scale_parameter{.directive = "",
                            .type = ParameterType::FLOAT,
                            .type_name = "",
                            .values = absl::MakeSpan(scale)};

  std::vector<double> maxsampleluminance = {9.0};
  Parameter maxsampleluminance_parameter{
      .directive = "",
      .type = ParameterType::FLOAT,
      .type_name = "",
      .values = absl::MakeSpan(maxsampleluminance)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"filename", filename_parameter},
      {"xresolution", xresolution_parameter},
      {"yresolution", yresolution_parameter},
      {"cropwindow", cropwindow_parameter},
      {"diagonal", diagonal_parameter},
      {"scale", scale_parameter},
      {"maxsampleluminance", maxsampleluminance_parameter}};

  RgbFilm actual;
  RemoveRgbFilmV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                filename: "file"
                xresolution: 1
                yresolution: 2
                cropwindow { x_min: 3.0 x_max: 4.0 y_min: 5.0 y_max: 6.0 }
                diagonal: 7.0
                scale: 8.0
                maxsampleluminance: 9.0
              )pb"));
}

TEST(RemoveRgbFilmV4, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  RgbFilm actual;
  ASSERT_TRUE(RemoveRgbFilmV4(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveRgbFilmV4, CropwindowOverrides) {
  std::vector<int32_t> pixelbounds = {1, 2, 7, 8};
  Parameter pixelbounds_parameter{.directive = "",
                                  .type = ParameterType::INTEGER,
                                  .type_name = "",
                                  .values = absl::MakeSpan(pixelbounds)};

  std::vector<double> cropwindow = {3.0, 4.0, 5.0, 6.0};
  Parameter cropwindow_parameter{.directive = "",
                                 .type = ParameterType::FLOAT,
                                 .type_name = "",
                                 .values = absl::MakeSpan(cropwindow)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"pixelbounds", pixelbounds_parameter},
      {"cropwindow", cropwindow_parameter}};

  RgbFilm actual;
  ASSERT_TRUE(RemoveRgbFilmV4(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                cropwindow { x_min: 3.0 x_max: 4.0 y_min: 5.0 y_max: 6.0 }
              )pb"));
}

TEST(RemoveRgbFilmV4, BadSensor) {
  std::vector<absl::string_view> filename = {"invalid"};
  Parameter filename_parameter{.directive = "",
                               .type = ParameterType::STRING,
                               .type_name = "",
                               .values = absl::MakeSpan(filename)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"sensor", filename_parameter}};

  RgbFilm actual;
  EXPECT_THAT(RemoveRgbFilmV4(parameters, actual),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "A Film specified an invalid 'sensor'"));
}

TEST(RemoveRgbFilmV4, WithData) {
  std::vector<absl::string_view> filename = {"file"};
  Parameter filename_parameter{.directive = "",
                               .type = ParameterType::STRING,
                               .type_name = "",
                               .values = absl::MakeSpan(filename)};

  std::vector<int32_t> xresolution = {1};
  Parameter xresolution_parameter{.directive = "",
                                  .type = ParameterType::INTEGER,
                                  .type_name = "",
                                  .values = absl::MakeSpan(xresolution)};

  std::vector<int32_t> yresolution = {2};
  Parameter yresolution_parameter{.directive = "",
                                  .type = ParameterType::INTEGER,
                                  .type_name = "",
                                  .values = absl::MakeSpan(yresolution)};

  std::vector<int32_t> pixelbounds = {3, 4, 5, 6};
  Parameter pixelbounds_parameter{.directive = "",
                                  .type = ParameterType::INTEGER,
                                  .type_name = "",
                                  .values = absl::MakeSpan(pixelbounds)};

  std::vector<double> diagonal = {7.0};
  Parameter diagonal_parameter{.directive = "",
                               .type = ParameterType::FLOAT,
                               .type_name = "",
                               .values = absl::MakeSpan(diagonal)};

  std::vector<double> iso = {8.0};
  Parameter iso_parameter{.directive = "",
                          .type = ParameterType::FLOAT,
                          .type_name = "",
                          .values = absl::MakeSpan(iso)};

  std::vector<double> whitebalance = {9.0};
  Parameter whitebalance_parameter{.directive = "",
                                   .type = ParameterType::FLOAT,
                                   .type_name = "",
                                   .values = absl::MakeSpan(whitebalance)};

  std::vector<double> maxcomponentvalue = {10.0};
  Parameter maxcomponentvalue_parameter{
      .directive = "",
      .type = ParameterType::FLOAT,
      .type_name = "",
      .values = absl::MakeSpan(maxcomponentvalue)};

  std::vector<absl::string_view> sensor = {"cie_1931"};
  Parameter sensor_parameter{.directive = "",
                             .type = ParameterType::STRING,
                             .type_name = "",
                             .values = absl::MakeSpan(sensor)};

  bool savefp16[] = {true};
  Parameter savefp16_parameter{.directive = "",
                               .type = ParameterType::BOOL,
                               .type_name = "",
                               .values = absl::MakeSpan(savefp16)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"filename", filename_parameter},
      {"xresolution", xresolution_parameter},
      {"yresolution", yresolution_parameter},
      {"pixelbounds", pixelbounds_parameter},
      {"diagonal", diagonal_parameter},
      {"iso", iso_parameter},
      {"whitebalance", whitebalance_parameter},
      {"maxcomponentvalue", maxcomponentvalue_parameter},
      {"sensor", sensor_parameter},
      {"savefp16", savefp16_parameter}};

  RgbFilm actual;
  ASSERT_TRUE(RemoveRgbFilmV4(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                filename: "file"
                xresolution: 1
                yresolution: 2
                pixelbounds { x_min: 3 x_max: 4 y_min: 5 y_max: 6 }
                diagonal: 7.0
                iso: 8.0
                whitebalance: 9.0
                maxcomponentvalue: 10.0
                sensor: CIE_1931
                savefp16: true
              )pb"));
}

TEST(RemoveGBufferFilmV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  GBufferFilm actual;
  ASSERT_TRUE(RemoveGBufferFilmV1(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveGBufferFilmV1, CropwindowOverrides) {
  std::vector<int32_t> pixelbounds = {1, 2, 7, 8};
  Parameter pixelbounds_parameter{.directive = "",
                                  .type = ParameterType::INTEGER,
                                  .type_name = "",
                                  .values = absl::MakeSpan(pixelbounds)};

  std::vector<double> cropwindow = {3.0, 4.0, 5.0, 6.0};
  Parameter cropwindow_parameter{.directive = "",
                                 .type = ParameterType::FLOAT,
                                 .type_name = "",
                                 .values = absl::MakeSpan(cropwindow)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"pixelbounds", pixelbounds_parameter},
      {"cropwindow", cropwindow_parameter}};

  GBufferFilm actual;
  ASSERT_TRUE(RemoveGBufferFilmV1(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                cropwindow { x_min: 3.0 x_max: 4.0 y_min: 5.0 y_max: 6.0 }
              )pb"));
}

TEST(RemoveGBufferFilmV1, BadSensor) {
  std::vector<absl::string_view> filename = {"invalid"};
  Parameter filename_parameter{.directive = "",
                               .type = ParameterType::STRING,
                               .type_name = "",
                               .values = absl::MakeSpan(filename)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"sensor", filename_parameter}};

  GBufferFilm actual;
  EXPECT_THAT(RemoveGBufferFilmV1(parameters, actual),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "A Film specified an invalid 'sensor'"));
}

TEST(RemoveGBufferFilmV1, BadCoordinateSystem) {
  std::vector<absl::string_view> filename = {"invalid"};
  Parameter filename_parameter{.directive = "",
                               .type = ParameterType::STRING,
                               .type_name = "",
                               .values = absl::MakeSpan(filename)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"coordinatesystem", filename_parameter}};

  GBufferFilm actual;
  EXPECT_THAT(
      RemoveGBufferFilmV1(parameters, actual),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "A gbuffer Film specified an invalid 'coordinatesystem'"));
}

TEST(RemoveGBufferFilmV1, WithData) {
  std::vector<absl::string_view> filename = {"file"};
  Parameter filename_parameter{.directive = "",
                               .type = ParameterType::STRING,
                               .type_name = "",
                               .values = absl::MakeSpan(filename)};

  std::vector<int32_t> xresolution = {1};
  Parameter xresolution_parameter{.directive = "",
                                  .type = ParameterType::INTEGER,
                                  .type_name = "",
                                  .values = absl::MakeSpan(xresolution)};

  std::vector<int32_t> yresolution = {2};
  Parameter yresolution_parameter{.directive = "",
                                  .type = ParameterType::INTEGER,
                                  .type_name = "",
                                  .values = absl::MakeSpan(yresolution)};

  std::vector<int32_t> pixelbounds = {3, 4, 5, 6};
  Parameter pixelbounds_parameter{.directive = "",
                                  .type = ParameterType::INTEGER,
                                  .type_name = "",
                                  .values = absl::MakeSpan(pixelbounds)};

  std::vector<double> diagonal = {7.0};
  Parameter diagonal_parameter{.directive = "",
                               .type = ParameterType::FLOAT,
                               .type_name = "",
                               .values = absl::MakeSpan(diagonal)};

  std::vector<double> iso = {8.0};
  Parameter iso_parameter{.directive = "",
                          .type = ParameterType::FLOAT,
                          .type_name = "",
                          .values = absl::MakeSpan(iso)};

  std::vector<double> whitebalance = {9.0};
  Parameter whitebalance_parameter{.directive = "",
                                   .type = ParameterType::FLOAT,
                                   .type_name = "",
                                   .values = absl::MakeSpan(whitebalance)};

  std::vector<double> maxcomponentvalue = {10.0};
  Parameter maxcomponentvalue_parameter{
      .directive = "",
      .type = ParameterType::FLOAT,
      .type_name = "",
      .values = absl::MakeSpan(maxcomponentvalue)};

  std::vector<absl::string_view> sensor = {"cie_1931"};
  Parameter sensor_parameter{.directive = "",
                             .type = ParameterType::STRING,
                             .type_name = "",
                             .values = absl::MakeSpan(sensor)};

  bool savefp16[] = {true};
  Parameter savefp16_parameter{.directive = "",
                               .type = ParameterType::BOOL,
                               .type_name = "",
                               .values = absl::MakeSpan(savefp16)};

  std::vector<absl::string_view> coordinatesystem = {"camera"};
  Parameter coordinatesystem_parameter{
      .directive = "",
      .type = ParameterType::STRING,
      .type_name = "",
      .values = absl::MakeSpan(coordinatesystem)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"filename", filename_parameter},
      {"xresolution", xresolution_parameter},
      {"yresolution", yresolution_parameter},
      {"pixelbounds", pixelbounds_parameter},
      {"diagonal", diagonal_parameter},
      {"iso", iso_parameter},
      {"whitebalance", whitebalance_parameter},
      {"maxcomponentvalue", maxcomponentvalue_parameter},
      {"sensor", sensor_parameter},
      {"savefp16", savefp16_parameter},
      {"coordinatesystem", coordinatesystem_parameter}};

  GBufferFilm actual;
  ASSERT_TRUE(RemoveGBufferFilmV1(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                filename: "file"
                xresolution: 1
                yresolution: 2
                pixelbounds { x_min: 3 x_max: 4 y_min: 5 y_max: 6 }
                diagonal: 7.0
                iso: 8.0
                whitebalance: 9.0
                maxcomponentvalue: 10.0
                sensor: CIE_1931
                savefp16: true
                coordinatesystem: CAMERA
              )pb"));
}

TEST(RemoveSpectralFilmV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  SpectralFilm actual;
  ASSERT_TRUE(RemoveSpectralFilmV1(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveSpectralFilmV1, CropwindowOverrides) {
  std::vector<int32_t> pixelbounds = {1, 2, 7, 8};
  Parameter pixelbounds_parameter{.directive = "",
                                  .type = ParameterType::INTEGER,
                                  .type_name = "",
                                  .values = absl::MakeSpan(pixelbounds)};

  std::vector<double> cropwindow = {3.0, 4.0, 5.0, 6.0};
  Parameter cropwindow_parameter{.directive = "",
                                 .type = ParameterType::FLOAT,
                                 .type_name = "",
                                 .values = absl::MakeSpan(cropwindow)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"pixelbounds", pixelbounds_parameter},
      {"cropwindow", cropwindow_parameter}};

  SpectralFilm actual;
  ASSERT_TRUE(RemoveSpectralFilmV1(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                cropwindow { x_min: 3.0 x_max: 4.0 y_min: 5.0 y_max: 6.0 }
              )pb"));
}

TEST(RemoveSpectralFilmV1, BadSensor) {
  std::vector<absl::string_view> filename = {"invalid"};
  Parameter filename_parameter{.directive = "",
                               .type = ParameterType::STRING,
                               .type_name = "",
                               .values = absl::MakeSpan(filename)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"sensor", filename_parameter}};

  SpectralFilm actual;
  EXPECT_THAT(RemoveSpectralFilmV1(parameters, actual),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "A Film specified an invalid 'sensor'"));
}

TEST(RemoveSpectralFilmV1, WithData) {
  std::vector<absl::string_view> filename = {"file"};
  Parameter filename_parameter{.directive = "",
                               .type = ParameterType::STRING,
                               .type_name = "",
                               .values = absl::MakeSpan(filename)};

  std::vector<int32_t> xresolution = {1};
  Parameter xresolution_parameter{.directive = "",
                                  .type = ParameterType::INTEGER,
                                  .type_name = "",
                                  .values = absl::MakeSpan(xresolution)};

  std::vector<int32_t> yresolution = {2};
  Parameter yresolution_parameter{.directive = "",
                                  .type = ParameterType::INTEGER,
                                  .type_name = "",
                                  .values = absl::MakeSpan(yresolution)};

  std::vector<int32_t> pixelbounds = {3, 4, 5, 6};
  Parameter pixelbounds_parameter{.directive = "",
                                  .type = ParameterType::INTEGER,
                                  .type_name = "",
                                  .values = absl::MakeSpan(pixelbounds)};

  std::vector<double> diagonal = {7.0};
  Parameter diagonal_parameter{.directive = "",
                               .type = ParameterType::FLOAT,
                               .type_name = "",
                               .values = absl::MakeSpan(diagonal)};

  std::vector<double> iso = {8.0};
  Parameter iso_parameter{.directive = "",
                          .type = ParameterType::FLOAT,
                          .type_name = "",
                          .values = absl::MakeSpan(iso)};

  std::vector<double> whitebalance = {9.0};
  Parameter whitebalance_parameter{.directive = "",
                                   .type = ParameterType::FLOAT,
                                   .type_name = "",
                                   .values = absl::MakeSpan(whitebalance)};

  std::vector<double> maxcomponentvalue = {10.0};
  Parameter maxcomponentvalue_parameter{
      .directive = "",
      .type = ParameterType::FLOAT,
      .type_name = "",
      .values = absl::MakeSpan(maxcomponentvalue)};

  std::vector<absl::string_view> sensor = {"cie_1931"};
  Parameter sensor_parameter{.directive = "",
                             .type = ParameterType::STRING,
                             .type_name = "",
                             .values = absl::MakeSpan(sensor)};

  bool savefp16[] = {true};
  Parameter savefp16_parameter{.directive = "",
                               .type = ParameterType::BOOL,
                               .type_name = "",
                               .values = absl::MakeSpan(savefp16)};

  std::vector<int32_t> nbuckets = {11};
  Parameter nbuckets_parameter{.directive = "",
                               .type = ParameterType::INTEGER,
                               .type_name = "",
                               .values = absl::MakeSpan(nbuckets)};

  std::vector<double> lambdamin = {12.0};
  Parameter lambdamin_parameter{.directive = "",
                                .type = ParameterType::FLOAT,
                                .type_name = "",
                                .values = absl::MakeSpan(lambdamin)};

  std::vector<double> lambdamax = {13.0};
  Parameter lambdamax_parameter{.directive = "",
                                .type = ParameterType::FLOAT,
                                .type_name = "",
                                .values = absl::MakeSpan(lambdamax)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"filename", filename_parameter},
      {"xresolution", xresolution_parameter},
      {"yresolution", yresolution_parameter},
      {"pixelbounds", pixelbounds_parameter},
      {"diagonal", diagonal_parameter},
      {"iso", iso_parameter},
      {"whitebalance", whitebalance_parameter},
      {"maxcomponentvalue", maxcomponentvalue_parameter},
      {"sensor", sensor_parameter},
      {"savefp16", savefp16_parameter},
      {"nbuckets", nbuckets_parameter},
      {"lambdamin", lambdamin_parameter},
      {"lambdamax", lambdamax_parameter}};

  SpectralFilm actual;
  ASSERT_TRUE(RemoveSpectralFilmV1(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                filename: "file"
                xresolution: 1
                yresolution: 2
                pixelbounds { x_min: 3 x_max: 4 y_min: 5 y_max: 6 }
                diagonal: 7.0
                iso: 8.0
                whitebalance: 9.0
                maxcomponentvalue: 10.0
                sensor: CIE_1931
                savefp16: true
                nbuckets: 11
                lambdamin: 12.0
                lambdamax: 13.0
              )pb"));
}

}  // namespace
}  // namespace pbrt_proto
