#include "pbrt_proto/shared/parser.h"

#include <array>
#include <sstream>

#include "absl/status/status_matchers.h"
#include "googlemock/include/gmock/gmock.h"
#include "googletest/include/gtest/gtest.h"

namespace pbrt_proto {
namespace {

using ::absl_testing::IsOk;
using ::absl_testing::StatusIs;
using ::testing::ElementsAre;
using ::testing::Return;

class MockParser final : public Parser {
 public:
  MOCK_METHOD(absl::Status, ActiveTransform, (ActiveTransformation),
              (override));
  MOCK_METHOD(absl::Status, ConcatTransform, ((const std::array<double, 16>&)),
              ());
  MOCK_METHOD(absl::Status, CoordinateSystem, (absl::string_view), (override));
  MOCK_METHOD(absl::Status, CoordSysTransform, (absl::string_view), (override));
  MOCK_METHOD(absl::Status, Identity, (), ());
  MOCK_METHOD(absl::Status, Include, (absl::string_view), (override));
  MOCK_METHOD(absl::Status, Import, (absl::string_view), (override));
  MOCK_METHOD(absl::Status, LookAt,
              (double, double, double, double, double, double, double, double,
               double),
              (override));
  MOCK_METHOD(absl::Status, ReverseOrientation, (), (override));
  MOCK_METHOD(absl::Status, Rotate, (double, double, double, double),
              (override));
  MOCK_METHOD(absl::Status, Scale, (double, double, double), (override));
  MOCK_METHOD(absl::Status, Transform, ((const std::array<double, 16>&)), ());
  MOCK_METHOD(absl::Status, TransformTimes, (double, double), (override));
  MOCK_METHOD(absl::Status, Translate, (double x, double y, double z),
              (override));
  MOCK_METHOD(absl::Status, WorldBegin, (), (override));
  MOCK_METHOD(absl::Status, WorldEnd, (), (override));

 private:
  absl::Status ConcatTransform(double m00, double m01, double m02, double m03,
                               double m10, double m11, double m12, double m13,
                               double m20, double m21, double m22, double m23,
                               double m30, double m31, double m32,
                               double m33) override {
    return ConcatTransform({m00, m01, m02, m03, m10, m11, m12, m13, m20, m21,
                            m22, m23, m30, m31, m32, m33});
  }

  absl::Status Transform(double m00, double m01, double m02, double m03,
                         double m10, double m11, double m12, double m13,
                         double m20, double m21, double m22, double m23,
                         double m30, double m31, double m32, double m33) {
    return Transform({m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22,
                      m23, m30, m31, m32, m33});
  }
};

TEST(Parser, Empty) {
  std::stringstream stream;
  EXPECT_THAT(MockParser().ReadFrom(stream), IsOk());
}

TEST(Parser, UnknownDirective) {
  std::stringstream stream("Abc");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kUnimplemented, "Abc"));
}

TEST(ActiveTransform, MissingParameter) {
  std::stringstream stream("ActiveTransform");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Missing parameter to directive ActiveTransform"));
}

TEST(ActiveTransform, InvalidParameter) {
  std::stringstream stream("ActiveTransform A");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Invalid parameter to directive ActiveTransforms: 'A'"));
}

TEST(ActiveTransform, All) {
  std::stringstream stream("ActiveTransform All");
  MockParser parser;
  EXPECT_CALL(parser, ActiveTransform(ActiveTransformation::ALL))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(ActiveTransform, StartTime) {
  std::stringstream stream("ActiveTransform StartTime");
  MockParser parser;
  EXPECT_CALL(parser, ActiveTransform(ActiveTransformation::START_TIME))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(ActiveTransform, EndTime) {
  std::stringstream stream("ActiveTransform EndTime");
  MockParser parser;
  EXPECT_CALL(parser, ActiveTransform(ActiveTransformation::END_TIME))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(ActiveTransform, Fails) {
  std::stringstream stream("ActiveTransform All");
  MockParser parser;
  EXPECT_CALL(parser, ActiveTransform(ActiveTransformation::ALL))
      .WillOnce(Return(absl::UnknownError("")));
  EXPECT_THAT(parser.ReadFrom(stream),
              StatusIs(absl::StatusCode::kUnknown, ""));
}

TEST(ConcatTransform, MissingParameter) {
  std::stringstream stream("ConcatTransform");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Directive ConcatTransform requires exactly 16 parameters"));
}

TEST(ConcatTransform, MissingParameters) {
  std::stringstream stream(
      "ConcatTransform 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Directive ConcatTransform requires exactly 16 parameters"));
}

TEST(ConcatTransform, Unparsable) {
  std::stringstream stream("ConcatTransform 1a");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Invalid parameter to directive ConcatTransform: '1a'"));
}

TEST(ConcatTransform, Succeeds) {
  std::stringstream stream(
      "ConcatTransform 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16");
  MockParser parser;
  EXPECT_CALL(parser, ConcatTransform(ElementsAre(
                          1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0,
                          11.0, 12.0, 13.0, 14.0, 15.0, 16.0)))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(ConcatTransform, Fails) {
  std::stringstream stream(
      "ConcatTransform 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16");
  MockParser parser;
  EXPECT_CALL(parser, ConcatTransform(ElementsAre(
                          1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0,
                          11.0, 12.0, 13.0, 14.0, 15.0, 16.0)))
      .WillOnce(Return(absl::UnknownError("")));
  EXPECT_THAT(parser.ReadFrom(stream),
              StatusIs(absl::StatusCode::kUnknown, ""));
}

}  // namespace
}  // namespace pbrt_proto