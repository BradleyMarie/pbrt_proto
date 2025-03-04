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
using ::testing::FieldsAre;
using ::testing::Pair;
using ::testing::Return;
using ::testing::VariantWith;

static const absl::flat_hash_map<absl::string_view, ParameterType>
    parameter_type_names = {
        {
            "blackbody1",
            ParameterType::BLACKBODY_V1,
        },
        {
            "blackbody2",
            ParameterType::BLACKBODY_V2,
        },
        {
            "bool",
            ParameterType::BOOL,
        },
        {
            "float",
            ParameterType::FLOAT,
        },
        {
            "integer",
            ParameterType::INTEGER,
        },
        {
            "normal3",
            ParameterType::NORMAL3,
        },
        {
            "point2",
            ParameterType::POINT2,
        },
        {
            "point3",
            ParameterType::POINT3,
        },
        {
            "rgb",
            ParameterType::RGB,
        },
        {
            "spectrum",
            ParameterType::SPECTRUM,
        },
        {"string", ParameterType::STRING},
        {"texture", ParameterType::STRING},
        {
            "vector2",
            ParameterType::VECTOR2,
        },
        {
            "vector3",
            ParameterType::VECTOR3,
        },
        {
            "xyz",
            ParameterType::XYZ,
        },
        {
            "zzz",
            ParameterType::FLOAT,
        },
};

class MockParser final : public Parser {
 public:
  MockParser() : Parser(parameter_type_names) {}

  MOCK_METHOD(absl::Status, Accelerator,
              (absl::string_view,
               (absl::flat_hash_map<absl::string_view, Parameter>&)),
              (override));
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

TEST(Parser, UnparsableNumber) {
  std::stringstream stream("ConcatTransform 1a");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Invalid parameter to directive ConcatTransform: '1a'"));
}

TEST(Parser, MissingQuotedString) {
  std::stringstream stream("CoordSysTransform");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Missing parameter to directive CoordSysTransform"));
}

TEST(Parser, NotQuotedString) {
  std::stringstream stream("CoordSysTransform 1");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Invalid parameter to directive CoordSysTransform: '1'"));
}

TEST(Parser, Blackbody1) {
  std::stringstream stream("Accelerator \"typename\" \"blackbody1 aaa\" [1 2]");
  MockParser parser;

  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa", FieldsAre(ParameterType::BLACKBODY_V1, "blackbody1",
                               VariantWith<absl::Span<std::array<double, 2>>>(
                                   ElementsAre(ElementsAre(1.0, 2.0))))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Parser, Blackbody2) {
  std::stringstream stream("Accelerator \"typename\" \"blackbody2 aaa\"[1] ");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa",
              FieldsAre(ParameterType::BLACKBODY_V2, "blackbody2",
                        VariantWith<absl::Span<double>>(ElementsAre(1.0)))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
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

TEST(ConcatTransform, MissingParameters) {
  std::stringstream stream(
      "ConcatTransform 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Directive ConcatTransform requires exactly 16 parameters"));
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

TEST(CoordinateSystem, Invalid) {
  std::stringstream stream("CoordinateSystem a");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Invalid parameter to directive CoordinateSystem: 'a'"));
}

TEST(CoordinateSystem, Succeeds) {
  std::stringstream stream("CoordinateSystem \"a\"");
  MockParser parser;
  EXPECT_CALL(parser, CoordinateSystem("a")).WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(CoordinateSystem, Fails) {
  std::stringstream stream("CoordinateSystem \"a\"");
  MockParser parser;
  EXPECT_CALL(parser, CoordinateSystem("a"))
      .WillOnce(Return(absl::UnknownError("")));
  EXPECT_THAT(parser.ReadFrom(stream),
              StatusIs(absl::StatusCode::kUnknown, ""));
}

TEST(CoordSysTransform, Invalid) {
  std::stringstream stream("CoordSysTransform a");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Invalid parameter to directive CoordSysTransform: 'a'"));
}

TEST(CoordSysTransform, Succeeds) {
  std::stringstream stream("CoordSysTransform \"a\"");
  MockParser parser;
  EXPECT_CALL(parser, CoordSysTransform("a"))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(CoordSysTransform, Fails) {
  std::stringstream stream("CoordSysTransform \"a\"");
  MockParser parser;
  EXPECT_CALL(parser, CoordSysTransform("a"))
      .WillOnce(Return(absl::UnknownError("")));
  EXPECT_THAT(parser.ReadFrom(stream),
              StatusIs(absl::StatusCode::kUnknown, ""));
}

TEST(Identity, Succeeds) {
  std::stringstream stream("Identity");
  MockParser parser;
  EXPECT_CALL(parser, Identity()).WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Identity, Fails) {
  std::stringstream stream("Identity");
  MockParser parser;
  EXPECT_CALL(parser, Identity()).WillOnce(Return(absl::UnknownError("")));
  EXPECT_THAT(parser.ReadFrom(stream),
              StatusIs(absl::StatusCode::kUnknown, ""));
}

TEST(Include, Invalid) {
  std::stringstream stream("Include a");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Invalid parameter to directive Include: 'a'"));
}

TEST(Include, Succeeds) {
  std::stringstream stream("Include \"a\"");
  MockParser parser;
  EXPECT_CALL(parser, Include("a")).WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Include, Fails) {
  std::stringstream stream("Include \"a\"");
  MockParser parser;
  EXPECT_CALL(parser, Include("a")).WillOnce(Return(absl::UnknownError("")));
  EXPECT_THAT(parser.ReadFrom(stream),
              StatusIs(absl::StatusCode::kUnknown, ""));
}

TEST(Import, Invalid) {
  std::stringstream stream("Import a");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Invalid parameter to directive Import: 'a'"));
}

TEST(Import, Succeeds) {
  std::stringstream stream("Import \"a\"");
  MockParser parser;
  EXPECT_CALL(parser, Import("a")).WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Import, Fails) {
  std::stringstream stream("Import \"a\"");
  MockParser parser;
  EXPECT_CALL(parser, Import("a")).WillOnce(Return(absl::UnknownError("")));
  EXPECT_THAT(parser.ReadFrom(stream),
              StatusIs(absl::StatusCode::kUnknown, ""));
}

TEST(LookAt, MissingParameters) {
  std::stringstream stream("LookAt 1 2 3 4 5 6 7 8");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Directive LookAt requires exactly 9 parameters"));
}

TEST(LookAt, Succeeds) {
  std::stringstream stream("LookAt 1 2 3 4 5 6 7 8 9");
  MockParser parser;
  EXPECT_CALL(parser, LookAt(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(LookAt, Fails) {
  std::stringstream stream("LookAt 1 2 3 4 5 6 7 8 9");
  MockParser parser;
  EXPECT_CALL(parser, LookAt(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0))
      .WillOnce(Return(absl::UnknownError("")));
  EXPECT_THAT(parser.ReadFrom(stream),
              StatusIs(absl::StatusCode::kUnknown, ""));
}

TEST(ReverseOrientation, Succeeds) {
  std::stringstream stream("ReverseOrientation");
  MockParser parser;
  EXPECT_CALL(parser, ReverseOrientation()).WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(ReverseOrientation, Fails) {
  std::stringstream stream("ReverseOrientation");
  MockParser parser;
  EXPECT_CALL(parser, ReverseOrientation())
      .WillOnce(Return(absl::UnknownError("")));
  EXPECT_THAT(parser.ReadFrom(stream),
              StatusIs(absl::StatusCode::kUnknown, ""));
}

TEST(Rotate, MissingParameters) {
  std::stringstream stream("Rotate 1 2 3");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Directive Rotate requires exactly 4 parameters"));
}

TEST(Rotate, Succeeds) {
  std::stringstream stream("Rotate 1 2 3 4");
  MockParser parser;
  EXPECT_CALL(parser, Rotate(1.0, 2.0, 3.0, 4.0))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Rotate, Fails) {
  std::stringstream stream("Rotate 1 2 3 4");
  MockParser parser;
  EXPECT_CALL(parser, Rotate(1.0, 2.0, 3.0, 4.0))
      .WillOnce(Return(absl::UnknownError("")));
  EXPECT_THAT(parser.ReadFrom(stream),
              StatusIs(absl::StatusCode::kUnknown, ""));
}

TEST(Scale, MissingParameters) {
  std::stringstream stream("Scale 1 2");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Directive Scale requires exactly 3 parameters"));
}

TEST(Scale, Succeeds) {
  std::stringstream stream("Scale 1 2 3");
  MockParser parser;
  EXPECT_CALL(parser, Scale(1.0, 2.0, 3.0)).WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Scale, Fails) {
  std::stringstream stream("Scale 1 2 3");
  MockParser parser;
  EXPECT_CALL(parser, Scale(1.0, 2.0, 3.0))
      .WillOnce(Return(absl::UnknownError("")));
  EXPECT_THAT(parser.ReadFrom(stream),
              StatusIs(absl::StatusCode::kUnknown, ""));
}

TEST(Transform, MissingParameters) {
  std::stringstream stream("Transform 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Directive Transform requires exactly 16 parameters"));
}

TEST(Transform, Succeeds) {
  std::stringstream stream("Transform 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16");
  MockParser parser;
  EXPECT_CALL(parser,
              Transform(ElementsAre(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0,
                                    10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0)))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Transform, Fails) {
  std::stringstream stream("Transform 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16");
  MockParser parser;
  EXPECT_CALL(parser,
              Transform(ElementsAre(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0,
                                    10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0)))
      .WillOnce(Return(absl::UnknownError("")));
  EXPECT_THAT(parser.ReadFrom(stream),
              StatusIs(absl::StatusCode::kUnknown, ""));
}

TEST(TransformTimes, MissingParameters) {
  std::stringstream stream("TransformTimes 1");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Directive TransformTimes requires exactly 2 parameters"));
}

TEST(TransformTimes, Succeeds) {
  std::stringstream stream("TransformTimes 1 2");
  MockParser parser;
  EXPECT_CALL(parser, TransformTimes(1.0, 2.0))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(TransformTimes, Fails) {
  std::stringstream stream("TransformTimes 1 2");
  MockParser parser;
  EXPECT_CALL(parser, TransformTimes(1.0, 2.0))
      .WillOnce(Return(absl::UnknownError("")));
  EXPECT_THAT(parser.ReadFrom(stream),
              StatusIs(absl::StatusCode::kUnknown, ""));
}

TEST(Translate, MissingParameters) {
  std::stringstream stream("Translate 1 2");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Directive Translate requires exactly 3 parameters"));
}

TEST(Translate, Succeeds) {
  std::stringstream stream("Translate 1 2 3");
  MockParser parser;
  EXPECT_CALL(parser, Translate(1.0, 2.0, 3.0))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Translate, Fails) {
  std::stringstream stream("Translate 1 2 3");
  MockParser parser;
  EXPECT_CALL(parser, Translate(1.0, 2.0, 3.0))
      .WillOnce(Return(absl::UnknownError("")));
  EXPECT_THAT(parser.ReadFrom(stream),
              StatusIs(absl::StatusCode::kUnknown, ""));
}

TEST(WorldBegin, Succeeds) {
  std::stringstream stream("WorldBegin");
  MockParser parser;
  EXPECT_CALL(parser, WorldBegin()).WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(WorldBegin, Fails) {
  std::stringstream stream("WorldBegin");
  MockParser parser;
  EXPECT_CALL(parser, WorldBegin()).WillOnce(Return(absl::UnknownError("")));
  EXPECT_THAT(parser.ReadFrom(stream),
              StatusIs(absl::StatusCode::kUnknown, ""));
}

TEST(WorldEnd, Succeeds) {
  std::stringstream stream("WorldEnd");
  MockParser parser;
  EXPECT_CALL(parser, WorldEnd()).WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(WorldEnd, Fails) {
  std::stringstream stream("WorldEnd");
  MockParser parser;
  EXPECT_CALL(parser, WorldEnd()).WillOnce(Return(absl::UnknownError("")));
  EXPECT_THAT(parser.ReadFrom(stream),
              StatusIs(absl::StatusCode::kUnknown, ""));
}

}  // namespace
}  // namespace pbrt_proto