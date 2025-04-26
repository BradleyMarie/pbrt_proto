#include "pbrt_proto/shared/parser.h"

#include <array>
#include <cstdint>
#include <sstream>

#include "absl/container/inlined_vector.h"
#include "absl/status/status_matchers.h"
#include "googlemock/include/gmock/gmock.h"
#include "googletest/include/gtest/gtest.h"

namespace pbrt_proto {
namespace {

using ::absl_testing::IsOk;
using ::absl_testing::StatusIs;
using ::testing::Contains;
using ::testing::ElementsAre;
using ::testing::Eq;
using ::testing::FieldsAre;
using ::testing::IsEmpty;
using ::testing::Key;
using ::testing::Not;
using ::testing::Optional;
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
        {"texture", ParameterType::TEXTURE},
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
  MOCK_METHOD(absl::Status, AreaLightSource,
              (absl::string_view,
               (absl::flat_hash_map<absl::string_view, Parameter>&)),
              (override));
  MOCK_METHOD(absl::Status, AttributeBegin, (), (override));
  MOCK_METHOD(absl::Status, AttributeEnd, (), (override));
  MOCK_METHOD(absl::Status, Camera,
              (absl::string_view,
               (absl::flat_hash_map<absl::string_view, Parameter>&)),
              (override));
  MOCK_METHOD(absl::Status, ConcatTransform, ((const std::array<double, 16>&)),
              ());
  MOCK_METHOD(absl::Status, CoordinateSystem, (absl::string_view), (override));
  MOCK_METHOD(absl::Status, CoordSysTransform, (absl::string_view), (override));
  MOCK_METHOD(absl::Status, Film,
              (absl::string_view,
               (absl::flat_hash_map<absl::string_view, Parameter>&)),
              (override));
  MOCK_METHOD(absl::Status, Filter,
              (absl::string_view,
               (absl::flat_hash_map<absl::string_view, Parameter>&)),
              (override));
  MOCK_METHOD(absl::Status, FloatTexture,
              (absl::string_view, absl::string_view,
               (absl::flat_hash_map<absl::string_view, Parameter>&)),
              (override));
  MOCK_METHOD(absl::Status, Identity, (), ());
  MOCK_METHOD(absl::Status, Include, (absl::string_view), (override));
  MOCK_METHOD(absl::Status, Integrator,
              (absl::string_view,
               (absl::flat_hash_map<absl::string_view, Parameter>&)),
              (override));
  MOCK_METHOD(absl::Status, Import, (absl::string_view), (override));
  MOCK_METHOD(absl::Status, LightSource,
              (absl::string_view,
               (absl::flat_hash_map<absl::string_view, Parameter>&)),
              (override));
  MOCK_METHOD(absl::Status, LookAt,
              (double, double, double, double, double, double, double, double,
               double),
              (override));
  MOCK_METHOD(absl::Status, MakeNamedMaterial,
              (absl::string_view,
               (absl::flat_hash_map<absl::string_view, Parameter>&)),
              (override));
  MOCK_METHOD(absl::Status, Material,
              (absl::string_view,
               (absl::flat_hash_map<absl::string_view, Parameter>&)),
              (override));
  MOCK_METHOD(absl::Status, NamedMaterial, (absl::string_view), (override));
  MOCK_METHOD(absl::Status, ObjectBegin, (absl::string_view), (override));
  MOCK_METHOD(absl::Status, ObjectEnd, (), (override));
  MOCK_METHOD(absl::Status, ObjectInstance, (absl::string_view), (override));
  MOCK_METHOD(absl::Status, ReverseOrientation, (), (override));
  MOCK_METHOD(absl::Status, Rotate, (double, double, double, double),
              (override));
  MOCK_METHOD(absl::Status, Sampler,
              (absl::string_view,
               (absl::flat_hash_map<absl::string_view, Parameter>&)),
              (override));
  MOCK_METHOD(absl::Status, Scale, (double, double, double), (override));
  MOCK_METHOD(absl::Status, Shape,
              (absl::string_view,
               (absl::flat_hash_map<absl::string_view, Parameter>&)),
              (override));
  MOCK_METHOD(absl::Status, SpectrumTexture,
              (absl::string_view, absl::string_view,
               (absl::flat_hash_map<absl::string_view, Parameter>&)),
              (override));
  MOCK_METHOD(absl::Status, Transform, ((const std::array<double, 16>&)), ());
  MOCK_METHOD(absl::Status, TransformBegin, (), (override));
  MOCK_METHOD(absl::Status, TransformEnd, (), (override));
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
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Unrecognized directive: 'Abc'"));
}

TEST(Parser, UnparsableNumber) {
  std::stringstream stream("Translate 1a");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Invalid parameter to directive Translate: '1a'"));
}

TEST(Parser, NoArray) {
  std::stringstream stream("Transform");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(
          absl::StatusCode::kInvalidArgument,
          "Directive Transform requires an array of exactly 16 parameters"));
}

TEST(Parser, NoArrayStart) {
  std::stringstream stream("Transform 1");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(
          absl::StatusCode::kInvalidArgument,
          "Directive Transform requires an array of exactly 16 parameters"));
}

TEST(Parser, TooFewArrayParamters) {
  std::stringstream stream("Transform [1]");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(
          absl::StatusCode::kInvalidArgument,
          "Directive Transform requires an array of exactly 16 parameters"));
}

TEST(Parser, NoArrayEnding) {
  std::stringstream stream("Transform [1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(
          absl::StatusCode::kInvalidArgument,
          "Directive Transform requires an array of exactly 16 parameters"));
}

TEST(Parser, TooManyArrayParameters) {
  std::stringstream stream(
      "Transform [1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17]");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(
          absl::StatusCode::kInvalidArgument,
          "Directive Transform requires an array of exactly 16 parameters"));
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

TEST(Parser, MissingTypeParameter) {
  std::stringstream stream("Accelerator");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Missing type parameter to directive Accelerator"));
}

TEST(Parser, TypeParameterNotQuoted) {
  std::stringstream stream("Accelerator abc");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Unquoted type parameter to directive Accelerator: 'abc'"));
}

TEST(Parser, ParameterTooFewTokens) {
  std::stringstream stream("Accelerator \"aaa\" \"abc\"");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(
          absl::StatusCode::kInvalidArgument,
          "Invalid parameter declaration for directive Accelerator: \"abc\""));
}

TEST(Parser, ParameterTooManyTokens) {
  std::stringstream stream("Accelerator \"aaa\" \"abc abc abc\"");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Invalid parameter declaration for directive "
                       "Accelerator: \"abc abc abc\""));
}

TEST(Parser, ParameterTypeUnrecognized) {
  std::stringstream stream("Accelerator \"aaa\" \"abc abc\"");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Invalid parameter type for directive Accelerator: 'abc'"));
}

TEST(Parser, EndsWithNoParameters) {
  std::stringstream stream("Accelerator \"aaa\"");
  MockParser parser;
  EXPECT_CALL(parser, Accelerator("aaa", IsEmpty()))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Parser, ContinuesAfterNoParameters) {
  std::stringstream stream("Accelerator \"aaa\" WorldBegin");
  MockParser parser;
  EXPECT_CALL(parser, Accelerator("aaa", IsEmpty()))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_CALL(parser, WorldBegin()).WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Parser, SingularValue) {
  std::stringstream stream("Accelerator \"typename\" \"float aaa\" 1.0");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa",
              FieldsAre("Accelerator", ParameterType::FLOAT, "float",
                        VariantWith<absl::Span<double>>(ElementsAre(1)))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Parser, MissingLoopEnding) {
  std::stringstream stream("Accelerator \"typename\" \"float aaa\" [ 1.0");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Unterminated array in directive Accelerator for float "
                       "parameter: 'aaa'"));
}

TEST(Parser, MissingFloat) {
  std::stringstream stream("Accelerator \"typename\" \"float aaa\"");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Missing value for Accelerator float parameter: 'aaa'"));
}

TEST(Parser, InvalidFloat) {
  std::stringstream stream("Accelerator \"typename\" \"float aaa\" 1.f");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(
          absl::StatusCode::kInvalidArgument,
          "Failed to parse float value for Accelerator parameter aaa: '1.f'"));
}

TEST(Parser, MissingFloat2) {
  std::stringstream stream("Accelerator \"typename\" \"point2 aaa\"");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Missing value for Accelerator point2 parameter: 'aaa'"));
}

TEST(Parser, MissingFirstFloat2) {
  std::stringstream stream("Accelerator \"typename\" \"point2 aaa\"");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Missing value for Accelerator point2 parameter: 'aaa'"));
}

TEST(Parser, MissingSecondFloat2) {
  std::stringstream stream("Accelerator \"typename\" \"point2 aaa\" [1.0");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Missing value for Accelerator point2 parameter: 'aaa'"));
}

// TODO: Make this compatible with all PBRT versions
TEST(Parser, MissingSecondFloat2Terminated) {
  std::stringstream stream("Accelerator \"typename\" \"point2 aaa\" [1.0]");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Missing value for Accelerator point2 parameter: 'aaa'"));
}

// TODO: Make this compatible with all PBRT versions
TEST(Parser, UnboundFloat2MissingOne) {
  std::stringstream stream("Accelerator \"typename\" \"point2 aaa\" 1.0");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Missing value for Accelerator point2 parameter: 'aaa'"));
}

TEST(Parser, UnboundFloat2) {
  std::stringstream stream("Accelerator \"typename\" \"point2 aaa\" 1.0 2.0");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Missing value for Accelerator point2 parameter: 'aaa'"));
}

TEST(Parser, MissingFloat3) {
  std::stringstream stream("Accelerator \"typename\" \"point3 aaa\"");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Missing value for Accelerator point3 parameter: 'aaa'"));
}

TEST(Parser, MissingFirstFloat3) {
  std::stringstream stream("Accelerator \"typename\" \"point3 aaa\"");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Missing value for Accelerator point3 parameter: 'aaa'"));
}

TEST(Parser, MissingSecondFloat3) {
  std::stringstream stream("Accelerator \"typename\" \"point3 aaa\" [1.0");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Missing value for Accelerator point3 parameter: 'aaa'"));
}

// TODO: Make this compatible with all PBRT versions
TEST(Parser, MissingSecondFloat3Terminated) {
  std::stringstream stream("Accelerator \"typename\" \"point3 aaa\" [1.0]");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Missing value for Accelerator point3 parameter: 'aaa'"));
}

TEST(Parser, MissingThirdFloat3) {
  std::stringstream stream("Accelerator \"typename\" \"point3 aaa\" [1.0 2.0");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Missing value for Accelerator point3 parameter: 'aaa'"));
}

// TODO: Make this compatible with all PBRT versions
TEST(Parser, MissingThirdFloat3Terminated) {
  std::stringstream stream("Accelerator \"typename\" \"point3 aaa\" [1.0 2.0]");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Missing value for Accelerator point3 parameter: 'aaa'"));
}

// TODO: Make this compatible with all PBRT versions
TEST(Parser, UnboundFloat3MissingTwo) {
  std::stringstream stream("Accelerator \"typename\" \"point3 aaa\" 1.0");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Missing value for Accelerator point3 parameter: 'aaa'"));
}

TEST(Parser, UnboundFloat3MissingOne) {
  std::stringstream stream("Accelerator \"typename\" \"point3 aaa\" 1.0 2.0");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Missing value for Accelerator point3 parameter: 'aaa'"));
}

TEST(Parser, UnboundFloat3) {
  std::stringstream stream(
      "Accelerator \"typename\" \"point3 aaa\" 1.0 2.0 3.0");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Missing value for Accelerator point3 parameter: 'aaa'"));
}

TEST(Parser, MissingInteger) {
  std::stringstream stream("Accelerator \"typename\" \"integer aaa\"");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Missing value for Accelerator integer parameter: 'aaa'"));
}

TEST(Parser, InvalidInteger) {
  std::stringstream stream("Accelerator \"typename\" \"integer aaa\" 1.0");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Failed to parse integer value for Accelerator "
                       "parameter aaa: '1.0'"));
}

TEST(Parser, MissingString) {
  std::stringstream stream("Accelerator \"typename\" \"string aaa\"");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Missing value for Accelerator string parameter: 'aaa'"));
}

TEST(Parser, InvalidString) {
  std::stringstream stream("Accelerator \"typename\" \"string aaa\" 1");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Unquoted string value for Accelerator parameter aaa: '1'"));
}

TEST(Parser, MissingBool) {
  std::stringstream stream("Accelerator \"typename\" \"bool aaa\"");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Missing value for Accelerator bool parameter: 'aaa'"));
}

TEST(Parser, InvalidBool) {
  std::stringstream stream("Accelerator \"typename\" \"bool aaa\" \"1\"");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Failed to parse bool value for Accelerator "
                       "parameter aaa: \"1\""));
}

TEST(Parser, Alias) {
  std::stringstream stream("Accelerator \"typename\" \"zzz aaa\" [1.0]");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa",
              FieldsAre("Accelerator", ParameterType::FLOAT, "zzz",
                        VariantWith<absl::Span<double>>(ElementsAre(1)))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Parser, Blackbody1Empty) {
  std::stringstream stream("Accelerator \"typename\" \"blackbody1 aaa\" []");
  MockParser parser;

  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa",
              FieldsAre(
                  "Accelerator", ParameterType::BLACKBODY_V1, "blackbody1",
                  VariantWith<absl::Span<std::array<double, 2>>>(IsEmpty()))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

// TODO: Make this compatible with all PBRT versions
TEST(Parser, Blackbody1Incomplete) {
  std::stringstream stream("Accelerator \"typename\" \"blackbody1 aaa\" [1.0]");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Missing value for Accelerator blackbody1 parameter: 'aaa'"));
}

TEST(Parser, Blackbody1) {
  std::stringstream stream("Accelerator \"typename\" \"blackbody1 aaa\" [1 2]");
  MockParser parser;

  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa", FieldsAre("Accelerator", ParameterType::BLACKBODY_V1,
                               "blackbody1",
                               VariantWith<absl::Span<std::array<double, 2>>>(
                                   ElementsAre(ElementsAre(1.0, 2.0))))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Parser, Blackbody1Multiple) {
  std::stringstream stream(
      "Accelerator \"typename\" \"blackbody1 aaa\" [1 2 3 4]");
  MockParser parser;

  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa",
              FieldsAre(
                  "Accelerator", ParameterType::BLACKBODY_V1, "blackbody1",
                  VariantWith<absl::Span<std::array<double, 2>>>(ElementsAre(
                      ElementsAre(1.0, 2.0), ElementsAre(3.0, 4.0))))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Parser, Blackbody2Empty) {
  std::stringstream stream("Accelerator \"typename\" \"blackbody2 aaa\" []");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa", FieldsAre("Accelerator", ParameterType::BLACKBODY_V2,
                               "blackbody2",
                               VariantWith<absl::Span<double>>(IsEmpty()))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Parser, Blackbody2) {
  std::stringstream stream("Accelerator \"typename\" \"blackbody2 aaa\" 1");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa",
              FieldsAre("Accelerator", ParameterType::BLACKBODY_V2,
                        "blackbody2",
                        VariantWith<absl::Span<double>>(ElementsAre(1.0)))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Parser, Blackbody2Multiple) {
  std::stringstream stream("Accelerator \"typename\" \"blackbody2 aaa\" [1 2]");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa",
              FieldsAre(
                  "Accelerator", ParameterType::BLACKBODY_V2, "blackbody2",
                  VariantWith<absl::Span<double>>(ElementsAre(1.0, 2.0)))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Parser, BoolEmpty) {
  std::stringstream stream("Accelerator \"typename\" \"bool aaa\" []");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa", FieldsAre("Accelerator", ParameterType::BOOL, "bool",
                               VariantWith<absl::Span<bool>>(IsEmpty()))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Parser, Bool) {
  std::stringstream stream("Accelerator \"typename\" \"bool aaa\" \"true\"");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa",
              FieldsAre("Accelerator", ParameterType::BOOL, "bool",
                        VariantWith<absl::Span<bool>>(ElementsAre(true)))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Parser, BoolMultiple) {
  std::stringstream stream(
      "Accelerator \"typename\" \"bool aaa\" [\"true\" \"false\"]");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(
              Pair("aaa", FieldsAre("Accelerator", ParameterType::BOOL, "bool",
                                    VariantWith<absl::Span<bool>>(
                                        ElementsAre(true, false)))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Parser, FloatEmpty) {
  std::stringstream stream("Accelerator \"typename\" \"float aaa\" []");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa", FieldsAre("Accelerator", ParameterType::FLOAT, "float",
                               VariantWith<absl::Span<double>>(IsEmpty()))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Parser, Float) {
  std::stringstream stream("Accelerator \"typename\" \"float aaa\" [1.0]");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa",
              FieldsAre("Accelerator", ParameterType::FLOAT, "float",
                        VariantWith<absl::Span<double>>(ElementsAre(1)))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Parser, FloatMultiple) {
  std::stringstream stream("Accelerator \"typename\" \"float aaa\" [1.0 2.0]");
  MockParser parser;
  EXPECT_CALL(parser,
              Accelerator("typename",
                          ElementsAre(Pair(
                              "aaa", FieldsAre("Accelerator",
                                               ParameterType::FLOAT, "float",
                                               VariantWith<absl::Span<double>>(
                                                   ElementsAre(1.0, 2.0)))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Parser, IntegerEmpty) {
  std::stringstream stream("Accelerator \"typename\" \"integer aaa\" []");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa", FieldsAre("Accelerator", ParameterType::INTEGER, "integer",
                               VariantWith<absl::Span<int32_t>>(IsEmpty()))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Parser, Integer) {
  std::stringstream stream("Accelerator \"typename\" \"integer aaa\" [1]");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa",
              FieldsAre("Accelerator", ParameterType::INTEGER, "integer",
                        VariantWith<absl::Span<int32_t>>(ElementsAre(1.0)))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Parser, IntegerMultiple) {
  std::stringstream stream("Accelerator \"typename\" \"integer aaa\" [1 2]");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa",
              FieldsAre("Accelerator", ParameterType::INTEGER, "integer",
                        VariantWith<absl::Span<int32_t>>(ElementsAre(1, 2)))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Parser, NormalEmpty) {
  std::stringstream stream("Accelerator \"typename\" \"normal3 aaa\" []");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa", FieldsAre("Accelerator", ParameterType::NORMAL3, "normal3",
                               VariantWith<absl::Span<std::array<double, 3>>>(
                                   IsEmpty()))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

// TODO: Make this compatible with all PBRT versions
TEST(Parser, NormalIncompleteMissingTwo) {
  std::stringstream stream("Accelerator \"typename\" \"normal3 aaa\" 1.0");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Missing value for Accelerator normal3 parameter: 'aaa'"));
}

// TODO: Make this compatible with all PBRT versions
TEST(Parser, NormalIncompleteMissingOne) {
  std::stringstream stream(
      "Accelerator \"typename\" \"normal3 aaa\" [1.0 2.0]");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Missing value for Accelerator normal3 parameter: 'aaa'"));
}

TEST(Parser, Normal) {
  std::stringstream stream(
      "Accelerator \"typename\" \"normal3 aaa\" [1.0 2.0 3.0]");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa", FieldsAre("Accelerator", ParameterType::NORMAL3, "normal3",
                               VariantWith<absl::Span<std::array<double, 3>>>(
                                   ElementsAre(ElementsAre(1.0, 2.0, 3.0))))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

// TODO: Make this compatible with all PBRT versions
TEST(Parser, NormalFirstCompleteSecondIncomplete) {
  std::stringstream stream(
      "Accelerator \"typename\" \"normal3 aaa\" [1.0 2.0 3.0 4.0]");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Missing value for Accelerator normal3 parameter: 'aaa'"));
}

TEST(Parser, NormalMultiple) {
  std::stringstream stream(
      "Accelerator \"typename\" \"normal3 aaa\" [1.0 2.0 3.0 4.0 5.0 6.0]");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa", FieldsAre("Accelerator", ParameterType::NORMAL3, "normal3",
                               VariantWith<absl::Span<std::array<double, 3>>>(
                                   ElementsAre(ElementsAre(1.0, 2.0, 3.0),
                                               ElementsAre(4.0, 5.0, 6.0))))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Parser, Point2Empty) {
  std::stringstream stream("Accelerator \"typename\" \"point2 aaa\" []");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa", FieldsAre("Accelerator", ParameterType::POINT2, "point2",
                               VariantWith<absl::Span<std::array<double, 2>>>(
                                   IsEmpty()))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

// TODO: Make this compatible with all PBRT versions
TEST(Parser, Point2Incomplete) {
  std::stringstream stream("Accelerator \"typename\" \"point2 aaa\" 1.0");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Missing value for Accelerator point2 parameter: 'aaa'"));
}

TEST(Parser, Point2) {
  std::stringstream stream("Accelerator \"typename\" \"point2 aaa\" [1.0 2.0]");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa", FieldsAre("Accelerator", ParameterType::POINT2, "point2",
                               VariantWith<absl::Span<std::array<double, 2>>>(
                                   ElementsAre(ElementsAre(1.0, 2.0))))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

// TODO: Make this compatible with all PBRT versions
TEST(Parser, Point2FirstCompleteSecondIncomplete) {
  std::stringstream stream(
      "Accelerator \"typename\" \"point2 aaa\" [1.0 2.0 3.0]");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Missing value for Accelerator point2 parameter: 'aaa'"));
}

TEST(Parser, Point3Empty) {
  std::stringstream stream("Accelerator \"typename\" \"point3 aaa\" []");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa", FieldsAre("Accelerator", ParameterType::POINT3, "point3",
                               VariantWith<absl::Span<std::array<double, 3>>>(
                                   IsEmpty()))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

// TODO: Make this compatible with all PBRT versions
TEST(Parser, Point3IncompleteMissingTwo) {
  std::stringstream stream("Accelerator \"typename\" \"point3 aaa\" 1.0");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Missing value for Accelerator point3 parameter: 'aaa'"));
}

// TODO: Make this compatible with all PBRT versions
TEST(Parser, Point3IncompleteMissingOne) {
  std::stringstream stream("Accelerator \"typename\" \"point3 aaa\" [1.0 2.0]");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Missing value for Accelerator point3 parameter: 'aaa'"));
}

TEST(Parser, Point3) {
  std::stringstream stream(
      "Accelerator \"typename\" \"point3 aaa\" [1.0 2.0 3.0]");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa", FieldsAre("Accelerator", ParameterType::POINT3, "point3",
                               VariantWith<absl::Span<std::array<double, 3>>>(
                                   ElementsAre(ElementsAre(1.0, 2.0, 3.0))))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

// TODO: Make this compatible with all PBRT versions
TEST(Parser, Point3FirstCompleteSecondIncomplete) {
  std::stringstream stream(
      "Accelerator \"typename\" \"point3 aaa\" [1.0 2.0 3.0 4.0]");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Missing value for Accelerator point3 parameter: 'aaa'"));
}

TEST(Parser, Point3Multiple) {
  std::stringstream stream(
      "Accelerator \"typename\" \"point3 aaa\" [1.0 2.0 3.0 4.0 5.0 6.0]");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa", FieldsAre("Accelerator", ParameterType::POINT3, "point3",
                               VariantWith<absl::Span<std::array<double, 3>>>(
                                   ElementsAre(ElementsAre(1.0, 2.0, 3.0),
                                               ElementsAre(4.0, 5.0, 6.0))))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}
TEST(Parser, RgbEmpty) {
  std::stringstream stream("Accelerator \"typename\" \"rgb aaa\" []");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa", FieldsAre("Accelerator", ParameterType::RGB, "rgb",
                               VariantWith<absl::Span<std::array<double, 3>>>(
                                   IsEmpty()))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

// TODO: Make this compatible with all PBRT versions
TEST(Parser, RgbIncompleteMissingTwo) {
  std::stringstream stream("Accelerator \"typename\" \"rgb aaa\" 1.0");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Missing value for Accelerator rgb parameter: 'aaa'"));
}

// TODO: Make this compatible with all PBRT versions
TEST(Parser, RgbIncompleteMissingOne) {
  std::stringstream stream("Accelerator \"typename\" \"rgb aaa\" [1.0 2.0]");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Missing value for Accelerator rgb parameter: 'aaa'"));
}

TEST(Parser, Rgb) {
  std::stringstream stream(
      "Accelerator \"typename\" \"rgb aaa\" [1.0 2.0 3.0]");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa", FieldsAre("Accelerator", ParameterType::RGB, "rgb",
                               VariantWith<absl::Span<std::array<double, 3>>>(
                                   ElementsAre(ElementsAre(1.0, 2.0, 3.0))))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

// TODO: Make this compatible with all PBRT versions
TEST(Parser, RgbFirstCompleteSecondIncomplete) {
  std::stringstream stream(
      "Accelerator \"typename\" \"rgb aaa\" [1.0 2.0 3.0 4.0]");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Missing value for Accelerator rgb parameter: 'aaa'"));
}

TEST(Parser, RgbMultiple) {
  std::stringstream stream(
      "Accelerator \"typename\" \"rgb aaa\" [1.0 2.0 3.0 4.0 5.0 6.0]");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa", FieldsAre("Accelerator", ParameterType::RGB, "rgb",
                               VariantWith<absl::Span<std::array<double, 3>>>(
                                   ElementsAre(ElementsAre(1.0, 2.0, 3.0),
                                               ElementsAre(4.0, 5.0, 6.0))))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Parser, SpectrumMissing) {
  std::stringstream stream("Accelerator \"typename\" \"spectrum aaa\"");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Missing value for Accelerator spectrum parameter: 'aaa'"));
}

TEST(Parser, SpectrumUnterminatedArray) {
  std::stringstream stream("Accelerator \"typename\" \"spectrum aaa\" [");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Unterminated array in directive Accelerator for "
                       "spectrum parameter: 'aaa'"));
}

TEST(Parser, SpectrumSingularFloat) {
  std::stringstream stream("Accelerator \"typename\" \"spectrum aaa\" 1.0");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Non-array spectrum value for Accelerator parameter aaa "
                       "was not a string: '1.0'"));
}

TEST(Parser, SpectrumEmptyArray) {
  std::stringstream stream("Accelerator \"typename\" \"spectrum aaa\" []");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa",
              FieldsAre(
                  "Accelerator", ParameterType::SPECTRUM, "spectrum",
                  VariantWith<absl::Span<std::array<double, 2>>>(IsEmpty()))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Parser, SpectrumSamplesIncomplete) {
  std::stringstream stream("Accelerator \"typename\" \"spectrum aaa\" [1.0]");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Missing value for Accelerator spectrum parameter: 'aaa'"));
}

TEST(Parser, SpectrumSamples) {
  std::stringstream stream(
      "Accelerator \"typename\" \"spectrum aaa\" [1.0 2.0]");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(
              Pair("aaa",
                   FieldsAre("Accelerator", ParameterType::SPECTRUM, "spectrum",
                             VariantWith<absl::Span<std::array<double, 2>>>(
                                 ElementsAre(ElementsAre(1.0, 2.0))))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

// TODO: Make this compatible with all PBRT versions
TEST(Parser, SpectrumSamplesFirstCompleteSecondIncomplete) {
  std::stringstream stream(
      "Accelerator \"typename\" \"spectrum aaa\" [1.0 2.0 3.0]");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Missing value for Accelerator spectrum parameter: 'aaa'"));
}

TEST(Parser, SpectrumSamplesMultiple) {
  std::stringstream stream(
      "Accelerator \"typename\" \"spectrum aaa\" [1.0 2.0 3.0 4.0]");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa",
              FieldsAre(
                  "Accelerator", ParameterType::SPECTRUM, "spectrum",
                  VariantWith<absl::Span<std::array<double, 2>>>(ElementsAre(
                      ElementsAre(1.0, 2.0), ElementsAre(3.0, 4.0))))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Parser, SpectrumBySingularPath) {
  std::stringstream stream(
      "Accelerator \"typename\" \"spectrum aaa\" \"abcdefg\"");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(
              Pair("aaa",
                   FieldsAre("Accelerator", ParameterType::SPECTRUM, "spectrum",
                             VariantWith<absl::Span<absl::string_view>>(
                                 ElementsAre("abcdefg")))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Parser, SpectrumByPath) {
  std::stringstream stream(
      "Accelerator \"typename\" \"spectrum aaa\" [\"abcdefg\"]");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(
              Pair("aaa",
                   FieldsAre("Accelerator", ParameterType::SPECTRUM, "spectrum",
                             VariantWith<absl::Span<absl::string_view>>(
                                 ElementsAre("abcdefg")))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Parser, SpectrumByPathMultiple) {
  std::stringstream stream(
      "Accelerator \"typename\" \"spectrum aaa\" [\"abc\" \"defg\"]");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(
              Pair("aaa",
                   FieldsAre("Accelerator", ParameterType::SPECTRUM, "spectrum",
                             VariantWith<absl::Span<absl::string_view>>(
                                 ElementsAre("abc", "defg")))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Parser, StringEmpty) {
  std::stringstream stream("Accelerator \"typename\" \"string aaa\" []");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa", FieldsAre("Accelerator", ParameterType::STRING, "string",
                               VariantWith<absl::Span<absl::string_view>>(
                                   IsEmpty()))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Parser, String) {
  std::stringstream stream(
      "Accelerator \"typename\" \"string aaa\" \"abcdefg\"");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa", FieldsAre("Accelerator", ParameterType::STRING, "string",
                               VariantWith<absl::Span<absl::string_view>>(
                                   ElementsAre("abcdefg")))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Parser, StringMultiple) {
  std::stringstream stream(
      "Accelerator \"typename\" \"string aaa\" [\"abc\" \"defg\"]");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa", FieldsAre("Accelerator", ParameterType::STRING, "string",
                               VariantWith<absl::Span<absl::string_view>>(
                                   ElementsAre("abc", "defg")))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Parser, TextureEmpty) {
  std::stringstream stream("Accelerator \"typename\" \"texture aaa\" []");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa", FieldsAre("Accelerator", ParameterType::TEXTURE, "texture",
                               VariantWith<absl::Span<absl::string_view>>(
                                   IsEmpty()))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Parser, Texture) {
  std::stringstream stream(
      "Accelerator \"typename\" \"texture aaa\" \"abcdefg\"");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa", FieldsAre("Accelerator", ParameterType::TEXTURE, "texture",
                               VariantWith<absl::Span<absl::string_view>>(
                                   ElementsAre("abcdefg")))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Parser, TextureMultiple) {
  std::stringstream stream(
      "Accelerator \"typename\" \"texture aaa\" [\"abc\" \"defg\"]");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa", FieldsAre("Accelerator", ParameterType::TEXTURE, "texture",
                               VariantWith<absl::Span<absl::string_view>>(
                                   ElementsAre("abc", "defg")))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Parser, Vector2Empty) {
  std::stringstream stream("Accelerator \"typename\" \"vector2 aaa\" []");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa", FieldsAre("Accelerator", ParameterType::VECTOR2, "vector2",
                               VariantWith<absl::Span<std::array<double, 2>>>(
                                   IsEmpty()))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

// TODO: Make this compatible with all PBRT versions
TEST(Parser, Vector2Incomplete) {
  std::stringstream stream("Accelerator \"typename\" \"vector2 aaa\" 1.0");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Missing value for Accelerator vector2 parameter: 'aaa'"));
}

TEST(Parser, Vector2) {
  std::stringstream stream(
      "Accelerator \"typename\" \"vector2 aaa\" [1.0 2.0]");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa", FieldsAre("Accelerator", ParameterType::VECTOR2, "vector2",
                               VariantWith<absl::Span<std::array<double, 2>>>(
                                   ElementsAre(ElementsAre(1.0, 2.0))))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

// TODO: Make this compatible with all PBRT versions
TEST(Parser, Vector2FirstCompleteSecondIncomplete) {
  std::stringstream stream(
      "Accelerator \"typename\" \"vector2 aaa\" [1.0 2.0 3.0]");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Missing value for Accelerator vector2 parameter: 'aaa'"));
}

TEST(Parser, Vector3Empty) {
  std::stringstream stream("Accelerator \"typename\" \"vector3 aaa\" []");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa", FieldsAre("Accelerator", ParameterType::VECTOR3, "vector3",
                               VariantWith<absl::Span<std::array<double, 3>>>(
                                   IsEmpty()))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

// TODO: Make this compatible with all PBRT versions
TEST(Parser, Vector3IncompleteMissingTwo) {
  std::stringstream stream("Accelerator \"typename\" \"vector3 aaa\" 1.0");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Missing value for Accelerator vector3 parameter: 'aaa'"));
}

// TODO: Make this compatible with all PBRT versions
TEST(Parser, Vector3IncompleteMissingOne) {
  std::stringstream stream(
      "Accelerator \"typename\" \"vector3 aaa\" [1.0 2.0]");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Missing value for Accelerator vector3 parameter: 'aaa'"));
}

TEST(Parser, Vector3) {
  std::stringstream stream(
      "Accelerator \"typename\" \"vector3 aaa\" [1.0 2.0 3.0]");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa", FieldsAre("Accelerator", ParameterType::VECTOR3, "vector3",
                               VariantWith<absl::Span<std::array<double, 3>>>(
                                   ElementsAre(ElementsAre(1.0, 2.0, 3.0))))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

// TODO: Make this compatible with all PBRT versions
TEST(Parser, Vector3FirstCompleteSecondIncomplete) {
  std::stringstream stream(
      "Accelerator \"typename\" \"vector3 aaa\" [1.0 2.0 3.0 4.0]");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Missing value for Accelerator vector3 parameter: 'aaa'"));
}

TEST(Parser, Vector3Multiple) {
  std::stringstream stream(
      "Accelerator \"typename\" \"vector3 aaa\" [1.0 2.0 3.0 4.0 5.0 6.0]");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa", FieldsAre("Accelerator", ParameterType::VECTOR3, "vector3",
                               VariantWith<absl::Span<std::array<double, 3>>>(
                                   ElementsAre(ElementsAre(1.0, 2.0, 3.0),
                                               ElementsAre(4.0, 5.0, 6.0))))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Parser, XyzEmpty) {
  std::stringstream stream("Accelerator \"typename\" \"xyz aaa\" []");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa", FieldsAre("Accelerator", ParameterType::XYZ, "xyz",
                               VariantWith<absl::Span<std::array<double, 3>>>(
                                   IsEmpty()))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

// TODO: Make this compatible with all PBRT versions
TEST(Parser, XyzIncompleteMissingTwo) {
  std::stringstream stream("Accelerator \"typename\" \"xyz aaa\" 1.0");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Missing value for Accelerator xyz parameter: 'aaa'"));
}

// TODO: Make this compatible with all PBRT versions
TEST(Parser, XyzIncompleteMissingOne) {
  std::stringstream stream("Accelerator \"typename\" \"xyz aaa\" [1.0 2.0]");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Missing value for Accelerator xyz parameter: 'aaa'"));
}

TEST(Parser, Xyz) {
  std::stringstream stream(
      "Accelerator \"typename\" \"xyz aaa\" [1.0 2.0 3.0]");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa", FieldsAre("Accelerator", ParameterType::XYZ, "xyz",
                               VariantWith<absl::Span<std::array<double, 3>>>(
                                   ElementsAre(ElementsAre(1.0, 2.0, 3.0))))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

// TODO: Make this compatible with all PBRT versions
TEST(Parser, XyzFirstCompleteSecondIncomplete) {
  std::stringstream stream(
      "Accelerator \"typename\" \"xyz aaa\" [1.0 2.0 3.0 4.0]");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Missing value for Accelerator xyz parameter: 'aaa'"));
}

TEST(Parser, XyzMultiple) {
  std::stringstream stream(
      "Accelerator \"typename\" \"xyz aaa\" [1.0 2.0 3.0 4.0 5.0 6.0]");
  MockParser parser;
  EXPECT_CALL(
      parser,
      Accelerator(
          "typename",
          ElementsAre(Pair(
              "aaa", FieldsAre("Accelerator", ParameterType::XYZ, "xyz",
                               VariantWith<absl::Span<std::array<double, 3>>>(
                                   ElementsAre(ElementsAre(1.0, 2.0, 3.0),
                                               ElementsAre(4.0, 5.0, 6.0))))))))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Accelerator, Succeeds) {
  std::stringstream stream("Accelerator \"abc\"");
  MockParser parser;
  EXPECT_CALL(parser, Accelerator("abc", IsEmpty()))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Accelerator, MissingType) {
  std::stringstream stream("Accelerator");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Missing type parameter to directive Accelerator"));
}

TEST(Accelerator, Fails) {
  std::stringstream stream("Accelerator \"abc\"");
  MockParser parser;
  EXPECT_CALL(parser, Accelerator("abc", IsEmpty()))
      .WillOnce(Return(absl::UnknownError("")));
  EXPECT_THAT(parser.ReadFrom(stream),
              StatusIs(absl::StatusCode::kUnknown, ""));
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

TEST(AreaLightSource, Succeeds) {
  std::stringstream stream("AreaLightSource \"abc\"");
  MockParser parser;
  EXPECT_CALL(parser, AreaLightSource("abc", IsEmpty()))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(AreaLightSource, MissingType) {
  std::stringstream stream("AreaLightSource");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Missing type parameter to directive AreaLightSource"));
}

TEST(AreaLightSource, Fails) {
  std::stringstream stream("AreaLightSource \"abc\"");
  MockParser parser;
  EXPECT_CALL(parser, AreaLightSource("abc", IsEmpty()))
      .WillOnce(Return(absl::UnknownError("")));
  EXPECT_THAT(parser.ReadFrom(stream),
              StatusIs(absl::StatusCode::kUnknown, ""));
}

TEST(AttributeBegin, Succeeds) {
  std::stringstream stream("AttributeBegin");
  MockParser parser;
  EXPECT_CALL(parser, AttributeBegin()).WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(AttributeBegin, Fails) {
  std::stringstream stream("AttributeBegin");
  MockParser parser;
  EXPECT_CALL(parser, AttributeBegin())
      .WillOnce(Return(absl::UnknownError("")));
  EXPECT_THAT(parser.ReadFrom(stream),
              StatusIs(absl::StatusCode::kUnknown, ""));
}

TEST(AttributeEnd, Succeeds) {
  std::stringstream stream("AttributeEnd");
  MockParser parser;
  EXPECT_CALL(parser, AttributeEnd()).WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(AttributeEnd, Fails) {
  std::stringstream stream("AttributeEnd");
  MockParser parser;
  EXPECT_CALL(parser, AttributeEnd()).WillOnce(Return(absl::UnknownError("")));
  EXPECT_THAT(parser.ReadFrom(stream),
              StatusIs(absl::StatusCode::kUnknown, ""));
}

TEST(Camera, Succeeds) {
  std::stringstream stream("Camera \"abc\"");
  MockParser parser;
  EXPECT_CALL(parser, Camera("abc", IsEmpty()))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Camera, MissingType) {
  std::stringstream stream("Camera");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Missing type parameter to directive Camera"));
}

TEST(Camera, Fails) {
  std::stringstream stream("Camera \"abc\"");
  MockParser parser;
  EXPECT_CALL(parser, Camera("abc", IsEmpty()))
      .WillOnce(Return(absl::UnknownError("")));
  EXPECT_THAT(parser.ReadFrom(stream),
              StatusIs(absl::StatusCode::kUnknown, ""));
}

TEST(ConcatTransform, MissingParameters) {
  std::stringstream stream(
      "ConcatTransform [1 2 3 4 5 6 7 8 9 10 11 12 13 14 15]");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Directive ConcatTransform requires an array of exactly "
                       "16 parameters"));
}

TEST(ConcatTransform, Succeeds) {
  std::stringstream stream(
      "ConcatTransform [1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16]");
  MockParser parser;
  EXPECT_CALL(parser, ConcatTransform(ElementsAre(
                          1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0,
                          11.0, 12.0, 13.0, 14.0, 15.0, 16.0)))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(ConcatTransform, Fails) {
  std::stringstream stream(
      "ConcatTransform [1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16]");
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

TEST(Film, Succeeds) {
  std::stringstream stream("Film \"abc\"");
  MockParser parser;
  EXPECT_CALL(parser, Film("abc", IsEmpty()))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Film, MissingType) {
  std::stringstream stream("Film");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Missing type parameter to directive Film"));
}

TEST(Film, Fails) {
  std::stringstream stream("Film \"abc\"");
  MockParser parser;
  EXPECT_CALL(parser, Film("abc", IsEmpty()))
      .WillOnce(Return(absl::UnknownError("")));
  EXPECT_THAT(parser.ReadFrom(stream),
              StatusIs(absl::StatusCode::kUnknown, ""));
}

TEST(Filter, Succeeds) {
  std::stringstream stream("Filter \"abc\"");
  MockParser parser;
  EXPECT_CALL(parser, Filter("abc", IsEmpty()))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Filter, MissingType) {
  std::stringstream stream("Filter");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Missing type parameter to directive Filter"));
}

TEST(Filter, Fails) {
  std::stringstream stream("Filter \"abc\"");
  MockParser parser;
  EXPECT_CALL(parser, Filter("abc", IsEmpty()))
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

TEST(Integrator, Succeeds) {
  std::stringstream stream("Integrator \"abc\"");
  MockParser parser;
  EXPECT_CALL(parser, Integrator("abc", IsEmpty()))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Integrator, MissingType) {
  std::stringstream stream("Integrator");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Missing type parameter to directive Integrator"));
}

TEST(Integrator, Fails) {
  std::stringstream stream("Integrator \"abc\"");
  MockParser parser;
  EXPECT_CALL(parser, Integrator("abc", IsEmpty()))
      .WillOnce(Return(absl::UnknownError("")));
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

TEST(LightSource, Succeeds) {
  std::stringstream stream("LightSource \"abc\"");
  MockParser parser;
  EXPECT_CALL(parser, LightSource("abc", IsEmpty()))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(LightSource, MissingType) {
  std::stringstream stream("LightSource");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Missing type parameter to directive LightSource"));
}

TEST(LightSource, Fails) {
  std::stringstream stream("LightSource \"abc\"");
  MockParser parser;
  EXPECT_CALL(parser, LightSource("abc", IsEmpty()))
      .WillOnce(Return(absl::UnknownError("")));
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

TEST(MakeNamedMaterial, Succeeds) {
  std::stringstream stream("MakeNamedMaterial \"abc\"");
  MockParser parser;
  EXPECT_CALL(parser, MakeNamedMaterial("abc", IsEmpty()))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(MakeNamedMaterial, MissingType) {
  std::stringstream stream("MakeNamedMaterial");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(absl::StatusCode::kInvalidArgument,
               "Missing name parameter to directive MakeNamedMaterial"));
}

TEST(MakeNamedMaterial, Fails) {
  std::stringstream stream("MakeNamedMaterial \"abc\"");
  MockParser parser;
  EXPECT_CALL(parser, MakeNamedMaterial("abc", IsEmpty()))
      .WillOnce(Return(absl::UnknownError("")));
  EXPECT_THAT(parser.ReadFrom(stream),
              StatusIs(absl::StatusCode::kUnknown, ""));
}

TEST(Material, Succeeds) {
  std::stringstream stream("Material \"abc\"");
  MockParser parser;
  EXPECT_CALL(parser, Material("abc", IsEmpty()))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Material, MissingType) {
  std::stringstream stream("Material");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Missing type parameter to directive Material"));
}

TEST(Material, Fails) {
  std::stringstream stream("Material \"abc\"");
  MockParser parser;
  EXPECT_CALL(parser, Material("abc", IsEmpty()))
      .WillOnce(Return(absl::UnknownError("")));
  EXPECT_THAT(parser.ReadFrom(stream),
              StatusIs(absl::StatusCode::kUnknown, ""));
}

TEST(NamedMaterial, Invalid) {
  std::stringstream stream("NamedMaterial a");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Invalid parameter to directive NamedMaterial: 'a'"));
}

TEST(NamedMaterial, Succeeds) {
  std::stringstream stream("NamedMaterial \"a\"");
  MockParser parser;
  EXPECT_CALL(parser, NamedMaterial("a")).WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(NamedMaterial, Fails) {
  std::stringstream stream("NamedMaterial \"a\"");
  MockParser parser;
  EXPECT_CALL(parser, NamedMaterial("a"))
      .WillOnce(Return(absl::UnknownError("")));
  EXPECT_THAT(parser.ReadFrom(stream),
              StatusIs(absl::StatusCode::kUnknown, ""));
}

TEST(ObjectBegin, Invalid) {
  std::stringstream stream("ObjectBegin a");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Invalid parameter to directive ObjectBegin: 'a'"));
}

TEST(ObjectBegin, Succeeds) {
  std::stringstream stream("ObjectBegin \"a\"");
  MockParser parser;
  EXPECT_CALL(parser, ObjectBegin("a")).WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(ObjectBegin, Fails) {
  std::stringstream stream("ObjectBegin \"a\"");
  MockParser parser;
  EXPECT_CALL(parser, ObjectBegin("a"))
      .WillOnce(Return(absl::UnknownError("")));
  EXPECT_THAT(parser.ReadFrom(stream),
              StatusIs(absl::StatusCode::kUnknown, ""));
}

TEST(ObjectEnd, Succeeds) {
  std::stringstream stream("ObjectEnd");
  MockParser parser;
  EXPECT_CALL(parser, ObjectEnd()).WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(ObjectEnd, Fails) {
  std::stringstream stream("ObjectEnd");
  MockParser parser;
  EXPECT_CALL(parser, ObjectEnd()).WillOnce(Return(absl::UnknownError("")));
  EXPECT_THAT(parser.ReadFrom(stream),
              StatusIs(absl::StatusCode::kUnknown, ""));
}

TEST(ObjectInstance, Invalid) {
  std::stringstream stream("ObjectInstance a");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Invalid parameter to directive ObjectInstance: 'a'"));
}

TEST(ObjectInstance, Succeeds) {
  std::stringstream stream("ObjectInstance \"a\"");
  MockParser parser;
  EXPECT_CALL(parser, ObjectInstance("a")).WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(ObjectInstance, Fails) {
  std::stringstream stream("ObjectInstance \"a\"");
  MockParser parser;
  EXPECT_CALL(parser, ObjectInstance("a"))
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

TEST(Sampler, Succeeds) {
  std::stringstream stream("Sampler \"abc\"");
  MockParser parser;
  EXPECT_CALL(parser, Sampler("abc", IsEmpty()))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Sampler, MissingType) {
  std::stringstream stream("Sampler");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Missing type parameter to directive Sampler"));
}

TEST(Sampler, Fails) {
  std::stringstream stream("Sampler \"abc\"");
  MockParser parser;
  EXPECT_CALL(parser, Sampler("abc", IsEmpty()))
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

TEST(Shape, Succeeds) {
  std::stringstream stream("Shape \"abc\"");
  MockParser parser;
  EXPECT_CALL(parser, Shape("abc", IsEmpty()))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Shape, MissingType) {
  std::stringstream stream("Shape");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Missing type parameter to directive Shape"));
}

TEST(Shape, Fails) {
  std::stringstream stream("Shape \"abc\"");
  MockParser parser;
  EXPECT_CALL(parser, Shape("abc", IsEmpty()))
      .WillOnce(Return(absl::UnknownError("")));
  EXPECT_THAT(parser.ReadFrom(stream),
              StatusIs(absl::StatusCode::kUnknown, ""));
}

TEST(Texture, MissingAllParameters) {
  std::stringstream stream("Texture");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Missing parameter to directive Texture"));
}

TEST(Texture, MissingTextureType) {
  std::stringstream stream("Texture \"abc\"");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Missing parameter to directive Texture"));
}

TEST(Texture, MissingType) {
  std::stringstream stream("Texture \"abc\" \"float\"");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Missing type parameter to directive Texture"));
}

TEST(Texture, UnrecognizedTextureType) {
  std::stringstream stream("Texture \"abc\" \"def\" \"type\"");
  EXPECT_THAT(MockParser().ReadFrom(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Unrecgonized Texture type: \"def\""));
}

TEST(Texture, FloatSucceeds) {
  std::stringstream stream("Texture \"name\" \"float\" \"abc\"");
  MockParser parser;
  EXPECT_CALL(parser, FloatTexture("name", "abc", IsEmpty()))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Texture, ColorSucceeds) {
  std::stringstream stream("Texture \"name\" \"color\" \"abc\"");
  MockParser parser;
  EXPECT_CALL(parser, SpectrumTexture("name", "abc", IsEmpty()))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Texture, SpectrumSucceeds) {
  std::stringstream stream("Texture \"name\" \"spectrum\" \"abc\"");
  MockParser parser;
  EXPECT_CALL(parser, SpectrumTexture("name", "abc", IsEmpty()))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Texture, Fails) {
  std::stringstream stream("Texture \"name\" \"float\" \"type\"");
  MockParser parser;
  EXPECT_CALL(parser, FloatTexture("name", "type", IsEmpty()))
      .WillOnce(Return(absl::UnknownError("")));
  EXPECT_THAT(parser.ReadFrom(stream),
              StatusIs(absl::StatusCode::kUnknown, ""));
}

TEST(Transform, MissingParameters) {
  std::stringstream stream("Transform [1 2 3 4 5 6 7 8 9 10 11 12 13 14 15]");
  EXPECT_THAT(
      MockParser().ReadFrom(stream),
      StatusIs(
          absl::StatusCode::kInvalidArgument,
          "Directive Transform requires an array of exactly 16 parameters"));
}

TEST(Transform, Succeeds) {
  std::stringstream stream(
      "Transform [1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16]");
  MockParser parser;
  EXPECT_CALL(parser,
              Transform(ElementsAre(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0,
                                    10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0)))
      .WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(Transform, Fails) {
  std::stringstream stream(
      "Transform [1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16]");
  MockParser parser;
  EXPECT_CALL(parser,
              Transform(ElementsAre(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0,
                                    10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0)))
      .WillOnce(Return(absl::UnknownError("")));
  EXPECT_THAT(parser.ReadFrom(stream),
              StatusIs(absl::StatusCode::kUnknown, ""));
}

TEST(TransformBegin, Succeeds) {
  std::stringstream stream("TransformBegin");
  MockParser parser;
  EXPECT_CALL(parser, TransformBegin()).WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(TransformBegin, Fails) {
  std::stringstream stream("TransformBegin");
  MockParser parser;
  EXPECT_CALL(parser, TransformBegin())
      .WillOnce(Return(absl::UnknownError("")));
  EXPECT_THAT(parser.ReadFrom(stream),
              StatusIs(absl::StatusCode::kUnknown, ""));
}

TEST(TransformEnd, Succeeds) {
  std::stringstream stream("TransformEnd");
  MockParser parser;
  EXPECT_CALL(parser, TransformEnd()).WillOnce(Return(absl::OkStatus()));
  EXPECT_THAT(parser.ReadFrom(stream), IsOk());
}

TEST(TransformEnd, Fails) {
  std::stringstream stream("TransformEnd");
  MockParser parser;
  EXPECT_CALL(parser, TransformEnd()).WillOnce(Return(absl::UnknownError("")));
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

TEST(TryRemoveFloats, WrongType) {
  std::vector<double> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::BLACKBODY_V1,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  std::optional<absl::Span<double>> removed_values;
  EXPECT_THAT(TryRemoveFloats(parameters, "name", 4, removed_values), IsOk());
  EXPECT_THAT(removed_values, Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name")));
}

TEST(TryRemoveFloats, WrongStoredType) {
  std::vector<int32_t> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::FLOAT,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name1", parameter}};

  std::optional<absl::Span<double>> removed_values;
  EXPECT_THAT(TryRemoveFloats(parameters, "name1", 4, removed_values), IsOk());
  EXPECT_THAT(removed_values, Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name1")));
}

TEST(TryRemoveFloats, WrongName) {
  std::vector<double> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::FLOAT,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name1", parameter}};

  std::optional<absl::Span<double>> removed_values;
  EXPECT_THAT(TryRemoveFloats(parameters, "name2", 4, removed_values), IsOk());
  EXPECT_THAT(removed_values, Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name1")));
}

TEST(TryRemoveFloats, WrongCount) {
  std::vector<double> values = {1.0};
  Parameter parameter{.directive = "Accelerator",
                      .type = ParameterType::FLOAT,
                      .type_name = "float",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  std::optional<absl::Span<double>> removed_values;
  EXPECT_THAT(
      TryRemoveFloats(parameters, "name", 4, removed_values),
      StatusIs(
          absl::StatusCode::kInvalidArgument,
          "Invalid number of values for Accelerator float parameter: 'name'"));
  EXPECT_THAT(removed_values, Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name")));
}

TEST(TryRemoveFloats, Found) {
  std::vector<double> values = {1.0, 2.0, 3.0, 4.0};
  Parameter parameter{.directive = "",
                      .type = ParameterType::FLOAT,
                      .type_name = "aaa",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  std::optional<absl::Span<double>> removed_values;
  EXPECT_THAT(TryRemoveFloats(parameters, "name", 4, removed_values), IsOk());
  EXPECT_THAT(removed_values, Optional(ElementsAre(1.0, 2.0, 3.0, 4.0)));
  EXPECT_THAT(parameters, Not(Contains(Key("name"))));
}

TEST(TryRemoveIntegers, WrongType) {
  std::vector<int32_t> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::BLACKBODY_V1,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  std::optional<absl::Span<int32_t>> removed_values;
  EXPECT_THAT(TryRemoveIntegers(parameters, "name", 4, removed_values), IsOk());
  EXPECT_THAT(removed_values, Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name")));
}

TEST(TryRemoveIntegers, WrongStoredType) {
  std::vector<double> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::INTEGER,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  std::optional<absl::Span<int32_t>> removed_values;
  EXPECT_THAT(TryRemoveIntegers(parameters, "name", 4, removed_values), IsOk());
  EXPECT_THAT(removed_values, Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name")));
}

TEST(TryRemoveIntegers, WrongName) {
  std::vector<int32_t> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::INTEGER,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name1", parameter}};

  std::optional<absl::Span<int32_t>> removed_values;
  EXPECT_THAT(TryRemoveIntegers(parameters, "name2", 4, removed_values),
              IsOk());
  EXPECT_THAT(removed_values, Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name1")));
}

TEST(TryRemoveIntegers, WrongCount) {
  std::vector<int32_t> values = {1};
  Parameter parameter{.directive = "Accelerator",
                      .type = ParameterType::INTEGER,
                      .type_name = "integer",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  std::optional<absl::Span<int32_t>> removed_values;
  EXPECT_THAT(TryRemoveIntegers(parameters, "name", 4, removed_values),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Invalid number of values for Accelerator integer "
                       "parameter: 'name'"));
  EXPECT_THAT(removed_values, Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name")));
}

TEST(TryRemoveIntegers, Found) {
  std::vector<int32_t> values = {1, 2, 3, 4};
  Parameter parameter{.directive = "",
                      .type = ParameterType::INTEGER,
                      .type_name = "aaa",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  std::optional<absl::Span<int32_t>> removed_values;
  EXPECT_THAT(TryRemoveIntegers(parameters, "name", 4, removed_values), IsOk());
  EXPECT_THAT(removed_values, Optional(ElementsAre(1, 2, 3, 4)));
  EXPECT_THAT(parameters, Not(Contains(Key("name"))));
}

TEST(TryRemoveSpectralSamples, WrongType) {
  std::vector<std::array<double, 2>> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::BLACKBODY_V1,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  EXPECT_THAT(TryRemoveSpectralSamples(parameters, "name"), Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name")));
}

TEST(TryRemoveSpectralSamples, WrongStoredType) {
  std::vector<double> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::SPECTRUM,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  EXPECT_THAT(TryRemoveSpectralSamples(parameters, "name"), Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name")));
}

TEST(TryRemoveSpectralSamples, WrongName) {
  std::vector<std::array<double, 2>> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::SPECTRUM,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name1", parameter}};

  EXPECT_THAT(TryRemoveSpectralSamples(parameters, "name2"), Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name1")));
}

TEST(TryRemoveSpectralSamples, Found) {
  std::vector<std::array<double, 2>> values = {{1.0, 2.0}, {3.0, 4.0}};
  Parameter parameter{.directive = "",
                      .type = ParameterType::SPECTRUM,
                      .type_name = "aaa",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  EXPECT_THAT(
      TryRemoveSpectralSamples(parameters, "name"),
      Optional(ElementsAre(ElementsAre(1.0, 2.0), ElementsAre(3.0, 4.0))));
  EXPECT_THAT(parameters, Not(Contains(Key("name"))));
}

TEST(TryRemoveBlackbodyV1, WrongType) {
  std::vector<std::array<double, 2>> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::FLOAT,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  EXPECT_THAT(TryRemoveBlackbodyV1(parameters, "name"), Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name")));
}

TEST(TryRemoveBlackbodyV1, WrongStoredType) {
  std::vector<double> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::BLACKBODY_V1,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  EXPECT_THAT(TryRemoveBlackbodyV1(parameters, "name"), Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name")));
}

TEST(TryRemoveBlackbodyV1, WrongName) {
  std::vector<std::array<double, 2>> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::BLACKBODY_V1,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name1", parameter}};

  EXPECT_THAT(TryRemoveBlackbodyV1(parameters, "name2"), Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name1")));
}

TEST(TryRemoveBlackbodyV1, Found) {
  std::vector<std::array<double, 2>> values = {{1.0, 2.0}};
  Parameter parameter{.directive = "",
                      .type = ParameterType::BLACKBODY_V1,
                      .type_name = "aaa",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  EXPECT_THAT(TryRemoveBlackbodyV1(parameters, "name"),
              Optional(ElementsAre(1.0, 2.0)));
  EXPECT_THAT(parameters, Not(Contains(Key("name1"))));
}

TEST(TryRemoveBool, WrongType) {
  absl::InlinedVector<bool, 1> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::BLACKBODY_V1,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  EXPECT_THAT(TryRemoveBool(parameters, "name"), Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name")));
}

TEST(TryRemoveBool, WrongStoredType) {
  std::vector<std::array<double, 2>> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::BOOL,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  EXPECT_THAT(TryRemoveBool(parameters, "name"), Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name")));
}

TEST(TryRemoveBool, WrongName) {
  absl::InlinedVector<bool, 1> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::BOOL,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name1", parameter}};

  EXPECT_THAT(TryRemoveBool(parameters, "name2"), Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name1")));
}

TEST(TryRemoveBool, Found) {
  absl::InlinedVector<bool, 1> values = {true};
  Parameter parameter{.directive = "",
                      .type = ParameterType::BOOL,
                      .type_name = "aaa",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  EXPECT_THAT(TryRemoveBool(parameters, "name"), Optional(true));
  EXPECT_THAT(parameters, Not(Contains(Key("name1"))));
}

TEST(TryRemoveFloat, WrongType) {
  std::vector<double> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::BLACKBODY_V1,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  EXPECT_THAT(TryRemoveFloat(parameters, "name"), Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name")));
}

TEST(TryRemoveFloat, WrongStoredType) {
  std::vector<int32_t> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::FLOAT,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  EXPECT_THAT(TryRemoveFloat(parameters, "name"), Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name")));
}

TEST(TryRemoveFloat, WrongName) {
  std::vector<double> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::FLOAT,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name1", parameter}};

  EXPECT_THAT(TryRemoveFloat(parameters, "name2"), Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name1")));
}

TEST(TryRemoveFloat, Found) {
  std::vector<double> values = {1.0};
  Parameter parameter{.directive = "",
                      .type = ParameterType::FLOAT,
                      .type_name = "aaa",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  EXPECT_THAT(TryRemoveFloat(parameters, "name"), Optional(1.0));
  EXPECT_THAT(parameters, Not(Contains(Key("name1"))));
}

TEST(TryRemoveInteger, WrongType) {
  std::vector<int32_t> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::BLACKBODY_V1,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  EXPECT_THAT(TryRemoveInteger(parameters, "name"), Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name")));
}

TEST(TryRemoveInteger, WrongStoredType) {
  std::vector<double> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::INTEGER,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  EXPECT_THAT(TryRemoveInteger(parameters, "name"), Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name")));
}

TEST(TryRemoveInteger, WrongName) {
  std::vector<int32_t> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::INTEGER,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name1", parameter}};

  EXPECT_THAT(TryRemoveInteger(parameters, "name2"), Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name1")));
}

TEST(TryRemoveInteger, Found) {
  std::vector<int32_t> values = {1};
  Parameter parameter{.directive = "",
                      .type = ParameterType::INTEGER,
                      .type_name = "aaa",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  EXPECT_THAT(TryRemoveInteger(parameters, "name"), Optional(1));
  EXPECT_THAT(parameters, Not(Contains(Key("name1"))));
}

TEST(TryRemovePoint3, WrongType) {
  std::vector<std::array<double, 3>> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::FLOAT,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  EXPECT_THAT(TryRemovePoint3(parameters, "name"), Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name")));
}

TEST(TryRemovePoint3, WrongStoredType) {
  std::vector<std::array<double, 2>> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::POINT3,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  EXPECT_THAT(TryRemovePoint3(parameters, "name"), Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name")));
}

TEST(TryRemovePoint3, WrongName) {
  std::vector<std::array<double, 3>> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::POINT3,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name1", parameter}};

  EXPECT_THAT(TryRemovePoint3(parameters, "name2"), Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name1")));
}

TEST(TryRemovePoint3, Found) {
  std::vector<std::array<double, 3>> values = {{1.0, 2.0, 3.0}};
  Parameter parameter{.directive = "",
                      .type = ParameterType::POINT3,
                      .type_name = "aaa",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  EXPECT_THAT(TryRemovePoint3(parameters, "name"),
              Optional(ElementsAre(1.0, 2.0, 3.0)));
  EXPECT_THAT(parameters, Not(Contains(Key("name1"))));
}

TEST(TryRemoveRgb, WrongType) {
  std::vector<std::array<double, 3>> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::FLOAT,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  EXPECT_THAT(TryRemoveRgb(parameters, "name"), Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name")));
}

TEST(TryRemoveRgb, WrongStoredType) {
  std::vector<std::array<double, 2>> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::RGB,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  EXPECT_THAT(TryRemoveRgb(parameters, "name"), Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name")));
}

TEST(TryRemoveRgb, WrongName) {
  std::vector<std::array<double, 3>> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::RGB,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name1", parameter}};

  EXPECT_THAT(TryRemoveRgb(parameters, "name2"), Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name1")));
}

TEST(TryRemoveRgb, Found) {
  std::vector<std::array<double, 3>> values = {{1.0, 2.0, 3.0}};
  Parameter parameter{.directive = "",
                      .type = ParameterType::RGB,
                      .type_name = "aaa",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  EXPECT_THAT(TryRemoveRgb(parameters, "name"),
              Optional(ElementsAre(1.0, 2.0, 3.0)));
  EXPECT_THAT(parameters, Not(Contains(Key("name1"))));
}

TEST(TryRemoveString, WrongType) {
  std::vector<absl::string_view> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::BLACKBODY_V1,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  EXPECT_THAT(TryRemoveString(parameters, "name"), Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name")));
}

TEST(TryRemoveString, WrongStoredType) {
  std::vector<double> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::STRING,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  EXPECT_THAT(TryRemoveString(parameters, "name"), Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name")));
}

TEST(TryRemoveString, WrongName) {
  std::vector<absl::string_view> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::STRING,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name1", parameter}};

  EXPECT_THAT(TryRemoveString(parameters, "name2"), Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name1")));
}

TEST(TryRemoveString, Found) {
  std::vector<absl::string_view> values = {"abc"};
  Parameter parameter{.directive = "",
                      .type = ParameterType::STRING,
                      .type_name = "aaa",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  EXPECT_THAT(TryRemoveString(parameters, "name"), Optional(Eq("abc")));
  EXPECT_THAT(parameters, Not(Contains(Key("name1"))));
}

TEST(TryRemoveSpectrumFilename, WrongType) {
  std::vector<absl::string_view> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::BLACKBODY_V1,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  EXPECT_THAT(TryRemoveSpectrumFilename(parameters, "name"), Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name")));
}

TEST(TryRemoveSpectrumFilename, WrongStoredType) {
  std::vector<double> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::SPECTRUM,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  EXPECT_THAT(TryRemoveSpectrumFilename(parameters, "name"), Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name")));
}

TEST(TryRemoveSpectrumFilename, WrongName) {
  std::vector<absl::string_view> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::SPECTRUM,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name1", parameter}};

  EXPECT_THAT(TryRemoveSpectrumFilename(parameters, "name2"), Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name1")));
}

TEST(TryRemoveSpectrumFilename, Found) {
  std::vector<absl::string_view> values = {"abc"};
  Parameter parameter{.directive = "",
                      .type = ParameterType::SPECTRUM,
                      .type_name = "aaa",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  EXPECT_THAT(TryRemoveSpectrumFilename(parameters, "name"),
              Optional(Eq("abc")));
  EXPECT_THAT(parameters, Not(Contains(Key("name1"))));
}

TEST(TryRemoveTexture, WrongType) {
  std::vector<absl::string_view> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::BLACKBODY_V1,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  EXPECT_THAT(TryRemoveTexture(parameters, "name"), Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name")));
}

TEST(TryRemoveTexture, WrongStoredType) {
  std::vector<double> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::TEXTURE,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  EXPECT_THAT(TryRemoveTexture(parameters, "name"), Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name")));
}

TEST(TryRemoveTexture, WrongName) {
  std::vector<absl::string_view> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::TEXTURE,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name1", parameter}};

  EXPECT_THAT(TryRemoveTexture(parameters, "name2"), Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name1")));
}

TEST(TryRemoveTexture, Found) {
  std::vector<absl::string_view> values = {"abc"};
  Parameter parameter{.directive = "",
                      .type = ParameterType::TEXTURE,
                      .type_name = "aaa",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  EXPECT_THAT(TryRemoveTexture(parameters, "name"), Optional(Eq("abc")));
  EXPECT_THAT(parameters, Not(Contains(Key("name1"))));
}

TEST(TryRemoveVector3, WrongType) {
  std::vector<std::array<double, 3>> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::BLACKBODY_V1,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  EXPECT_THAT(TryRemoveVector3(parameters, "name"), Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name")));
}

TEST(TryRemoveVector3, WrongStoredType) {
  std::vector<std::array<double, 2>> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::VECTOR3,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  EXPECT_THAT(TryRemoveVector3(parameters, "name"), Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name")));
}

TEST(TryRemoveVector3, WrongName) {
  std::vector<std::array<double, 3>> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::VECTOR3,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name1", parameter}};

  EXPECT_THAT(TryRemoveVector3(parameters, "name2"), Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name1")));
}

TEST(TryRemoveVector3, Found) {
  std::vector<std::array<double, 3>> values = {{1.0, 2.0, 3.0}};
  Parameter parameter{.directive = "",
                      .type = ParameterType::VECTOR3,
                      .type_name = "aaa",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  EXPECT_THAT(TryRemoveVector3(parameters, "name"),
              Optional(ElementsAre(1.0, 2.0, 3.0)));
  EXPECT_THAT(parameters, Not(Contains(Key("name1"))));
}

TEST(TryRemoveXyz, WrongType) {
  std::vector<std::array<double, 3>> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::FLOAT,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  EXPECT_THAT(TryRemoveXyz(parameters, "name"), Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name")));
}

TEST(TryRemoveXyz, WrongStoredType) {
  std::vector<std::array<double, 2>> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::XYZ,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  EXPECT_THAT(TryRemoveXyz(parameters, "name"), Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name")));
}

TEST(TryRemoveXyz, WrongName) {
  std::vector<std::array<double, 3>> values;
  Parameter parameter{.directive = "",
                      .type = ParameterType::XYZ,
                      .type_name = "",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name1", parameter}};

  EXPECT_THAT(TryRemoveXyz(parameters, "name2"), Eq(std::nullopt));
  EXPECT_THAT(parameters, Contains(Key("name1")));
}

TEST(TryRemoveXyz, Found) {
  std::vector<std::array<double, 3>> values = {{1.0, 2.0, 3.0}};
  Parameter parameter{.directive = "",
                      .type = ParameterType::XYZ,
                      .type_name = "aaa",
                      .values = absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"name", parameter}};

  EXPECT_THAT(TryRemoveXyz(parameters, "name"),
              Optional(ElementsAre(1.0, 2.0, 3.0)));
  EXPECT_THAT(parameters, Not(Contains(Key("name1"))));
}

}  // namespace
}  // namespace pbrt_proto