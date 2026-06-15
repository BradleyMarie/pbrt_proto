#include "pbrt_proto/shared/proto_parser.h"

#include <sstream>
#include <string>

#include "absl/container/flat_hash_map.h"
#include "absl/status/status.h"
#include "absl/status/status_matchers.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"
#include "pbrt_proto/shared/proto_parser.pb.h"
#include "pbrt_proto/testing/proto_matchers.h"

namespace pbrt_proto {
namespace {

using ::absl_testing::StatusIs;
using ::google::protobuf::EqualsProto;
using ::testing::_;
using ::testing::Return;

static const absl::flat_hash_map<absl::string_view, ParameterType>
    kSupportedTypes = {};

template <int PbrtVersion>
class TestProtoParser final : public ProtoParser<PbrtProto, PbrtVersion> {
 public:
  TestProtoParser(PbrtProto& output) noexcept
      : ProtoParser<PbrtProto, PbrtVersion>(kSupportedTypes, output) {}

  MOCK_METHOD(absl::Status, Material,
              (absl::string_view,
               (absl::flat_hash_map<absl::string_view, Parameter>&),
               pbrt_proto::Material& output),
              (override));

 private:
  absl::Status Accelerator(
      absl::string_view accelerator_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override {
    return absl::UnimplementedError("N/A");
  }

  absl::Status AreaLightSource(
      absl::string_view area_light_source_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override {
    return absl::UnimplementedError("N/A");
  }

  absl::Status Camera(
      absl::string_view camera_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override {
    return absl::UnimplementedError("N/A");
  }

  absl::Status Film(
      absl::string_view film_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override {
    return absl::UnimplementedError("N/A");
  }

  absl::Status FloatTexture(
      absl::string_view float_texture_name,
      absl::string_view float_texture_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override {
    return absl::UnimplementedError("N/A");
  }

  absl::Status LightSource(
      absl::string_view light_source_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override {
    return absl::UnimplementedError("N/A");
  }

  absl::Status PixelFilter(
      absl::string_view pixel_filter_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override {
    return absl::UnimplementedError("N/A");
  }

  absl::Status Sampler(
      absl::string_view sampler_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override {
    return absl::UnimplementedError("N/A");
  }

  absl::Status Shape(
      absl::string_view shape_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override {
    return absl::UnimplementedError("N/A");
  }

  absl::Status SpectrumTexture(
      absl::string_view spectrum_texture_name,
      absl::string_view spectrum_texture_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override {
    return absl::UnimplementedError("N/A");
  }
};

template <int PbrtVersion>
absl::Status Convert(absl::string_view input, PbrtProto& output) {
  std::string as_string(input);
  std::istringstream as_stream(as_string);
  TestProtoParser<PbrtVersion> parser(output);
  return parser.ReadFrom(as_stream);
}

TEST(ActiveTransform, V1) {
  absl::string_view directive = R"pbrt(ActiveTransform All)pbrt";

  PbrtProto actual;
  EXPECT_THAT(
      Convert<1>(directive, actual),
      StatusIs(absl::StatusCode::kUnimplemented,
               "Directive 'ActiveTransform' is not supported in pbrt-v1"));
}

TEST(ActiveTransform, All) {
  absl::string_view directive = R"pbrt(ActiveTransform All)pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert<2>(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         active_transform { active: ALL }
                                       })pb"));
}

TEST(ActiveTransform, StartTime) {
  absl::string_view directive = R"pbrt(ActiveTransform StartTime)pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert<2>(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         active_transform { active: START_TIME }
                                       })pb"));
}

TEST(ActiveTransform, EndTime) {
  absl::string_view directive = R"pbrt(ActiveTransform EndTime)pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert<2>(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         active_transform { active: END_TIME }
                                       })pb"));
}

TEST(AttributeBegin, V1) {
  absl::string_view directive = R"pbrt(AttributeBegin)pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert<1>(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives { attribute_begin {} })pb"));
}

TEST(AttributeEnd, V1) {
  absl::string_view directive = R"pbrt(AttributeEnd)pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert<1>(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives { attribute_end {} })pb"));
}

TEST(ConcatTransform, V1) {
  absl::string_view directive =
      R"pbrt(ConcatTransform [1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16])pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert<1>(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         concat_transform {
                                           m00: 1
                                           m01: 2
                                           m02: 3
                                           m03: 4
                                           m10: 5
                                           m11: 6
                                           m12: 7
                                           m13: 8
                                           m20: 9
                                           m21: 10
                                           m22: 11
                                           m23: 12
                                           m30: 13
                                           m31: 14
                                           m32: 15
                                           m33: 16
                                         }
                                       })pb"));
}

TEST(CoordinateSystem, V1) {
  absl::string_view directive = R"pbrt(CoordinateSystem "a")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert<1>(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         coordinate_system { name: "a" }
                                       })pb"));
}

TEST(CoordSysTransform, V1) {
  absl::string_view directive = R"pbrt(CoordSysTransform "a")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert<1>(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         coord_sys_transform { name: "a" }
                                       })pb"));
}

TEST(Identity, V1) {
  absl::string_view directive = R"pbrt(Identity)pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert<1>(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives { identity {} })pb"));
}

TEST(Include, V1) {
  absl::string_view directive = R"pbrt(Include "a")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert<1>(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { include { path: "a" } })pb"));
}

TEST(Import, V1) {
  absl::string_view directive = R"pbrt(Import "a")pbrt";

  PbrtProto actual;
  EXPECT_THAT(Convert<1>(directive, actual),
              StatusIs(absl::StatusCode::kUnimplemented,
                       "Directive 'Import' is not supported in pbrt-v1"));
}

TEST(Import, V4) {
  absl::string_view directive = R"pbrt(Import "a")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert<4>(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { import { path: "a" } })pb"));
}

TEST(LookAt, V1) {
  absl::string_view directive = R"pbrt(LookAt 1 2 3 4 5 6 7 8 9)pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert<1>(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         look_at {
                                           eye_x: 1
                                           eye_y: 2
                                           eye_z: 3
                                           look_x: 4
                                           look_y: 5
                                           look_z: 6
                                           up_x: 7
                                           up_y: 8
                                           up_z: 9
                                         }
                                       })pb"));
}

TEST(MakeNamedMaterial, V1) {
  absl::string_view directive = R"pbrt(MakeNamedMaterial "a")pbrt";

  PbrtProto actual;
  EXPECT_THAT(
      Convert<1>(directive, actual),
      StatusIs(absl::StatusCode::kUnimplemented,
               "Directive 'MakeNamedMaterial' is not supported in pbrt-v1"));
}

TEST(MakeNamedMaterial, V2) {
  absl::string_view directive = R"pbrt(MakeNamedMaterial "a")pbrt";
  std::string as_string(directive);
  std::istringstream as_stream(as_string);

  PbrtProto actual;
  TestProtoParser<2> parser(actual);
  EXPECT_CALL(parser, Material("", _, _)).WillOnce(Return(absl::OkStatus()));

  EXPECT_TRUE(parser.ReadFrom(as_stream).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         make_named_material {
                                           name: "a"
                                           material {}
                                         }
                                       })pb"));
}

TEST(MakeNamedMedium, V1) {
  absl::string_view directive = R"pbrt(MakeNamedMedium "a")pbrt";

  PbrtProto actual;
  EXPECT_THAT(
      Convert<1>(directive, actual),
      StatusIs(absl::StatusCode::kUnimplemented,
               "Directive 'MakeNamedMedium' is not supported in pbrt-v1"));
}

TEST(Material, V1) {
  absl::string_view directive = R"pbrt(Material "a")pbrt";
  std::string as_string(directive);
  std::istringstream as_stream(as_string);

  PbrtProto actual;
  TestProtoParser<1> parser(actual);
  EXPECT_CALL(parser, Material("a", _, _)).WillOnce(Return(absl::OkStatus()));

  EXPECT_TRUE(parser.ReadFrom(as_stream).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives { material {} })pb"));
}

TEST(MediumInterface, V1) {
  absl::string_view directive = R"pbrt(MediumInterface "a" "b")pbrt";

  PbrtProto actual;
  EXPECT_THAT(
      Convert<1>(directive, actual),
      StatusIs(absl::StatusCode::kUnimplemented,
               "Directive 'MediumInterface' is not supported in pbrt-v1"));
}

TEST(MediumInterface, V3) {
  absl::string_view directive = R"pbrt(MediumInterface "a" "b")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert<3>(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives {
                                 medium_interface { inside: "a" outside: "b" }
                               })pb"));
}

TEST(NamedMaterial, V1) {
  absl::string_view directive = R"pbrt(NamedMaterial "a")pbrt";

  PbrtProto actual;
  EXPECT_THAT(
      Convert<1>(directive, actual),
      StatusIs(absl::StatusCode::kUnimplemented,
               "Directive 'NamedMaterial' is not supported in pbrt-v1"));
}

TEST(ObjectBegin, V1) {
  absl::string_view directive = R"pbrt(ObjectBegin "a")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert<1>(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { object_begin { name: "a" } })pb"));
}

TEST(ObjectEnd, V1) {
  absl::string_view directive = R"pbrt(ObjectEnd)pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert<1>(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives { object_end {} })pb"));
}

TEST(ObjectInstance, V1) {
  absl::string_view directive = R"pbrt(ObjectInstance "a")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert<1>(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         object_instance { name: "a" }
                                       })pb"));
}

TEST(Renderer, V1) {
  absl::string_view directive = R"pbrt(Renderer "a")pbrt";

  PbrtProto actual;
  EXPECT_THAT(Convert<1>(directive, actual),
              StatusIs(absl::StatusCode::kUnimplemented,
                       "Directive 'Renderer' is not supported in pbrt-v1"));
}

TEST(ReverseOrientation, V1) {
  absl::string_view directive = R"pbrt(ReverseOrientation)pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert<1>(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { reverse_orientation {} })pb"));
}

TEST(Rotate, V1) {
  absl::string_view directive = R"pbrt(Rotate 1 2 3 4)pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert<1>(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         rotate { angle: 1 x: 2 y: 3 z: 4 }
                                       })pb"));
}

TEST(Scale, V1) {
  absl::string_view directive = R"pbrt(Scale 1 2 3)pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert<1>(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { scale { x: 1 y: 2 z: 3 } })pb"));
}

TEST(SearchPath, V1) {
  absl::string_view directive = R"pbrt(SearchPath "a")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert<1>(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { search_path { path: "a" } })pb"));
}

TEST(SearchPath, V2) {
  absl::string_view directive = R"pbrt(SearchPath "a")pbrt";

  PbrtProto actual;
  EXPECT_THAT(Convert<2>(directive, actual),
              StatusIs(absl::StatusCode::kUnimplemented,
                       "Directive 'SearchPath' is not supported in pbrt-v2"));
}

TEST(SurfaceIntegrator, V1) {
  absl::string_view directive = R"pbrt(SurfaceIntegrator "a")pbrt";

  PbrtProto actual;
  EXPECT_THAT(
      Convert<1>(directive, actual),
      StatusIs(absl::StatusCode::kUnimplemented,
               "Directive 'SurfaceIntegrator' is not supported in pbrt-v1"));
}

TEST(Transform, V1) {
  absl::string_view directive =
      R"pbrt(Transform [1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16])pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert<1>(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         transform {
                                           m00: 1
                                           m01: 2
                                           m02: 3
                                           m03: 4
                                           m10: 5
                                           m11: 6
                                           m12: 7
                                           m13: 8
                                           m20: 9
                                           m21: 10
                                           m22: 11
                                           m23: 12
                                           m30: 13
                                           m31: 14
                                           m32: 15
                                           m33: 16
                                         }
                                       })pb"));
}

TEST(TransformBegin, V1) {
  absl::string_view directive = R"pbrt(TransformBegin)pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert<1>(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives { transform_begin {} })pb"));
}

TEST(TransformBegin, V4) {
  absl::string_view directive = R"pbrt(TransformBegin)pbrt";

  PbrtProto actual;
  EXPECT_THAT(
      Convert<4>(directive, actual),
      StatusIs(absl::StatusCode::kUnimplemented,
               "Directive 'TransformBegin' is not supported in pbrt-v4"));
}

TEST(TransformEnd, V1) {
  absl::string_view directive = R"pbrt(TransformEnd)pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert<1>(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives { transform_end {} })pb"));
}

TEST(TransformEnd, V4) {
  absl::string_view directive = R"pbrt(TransformEnd)pbrt";

  PbrtProto actual;
  EXPECT_THAT(Convert<4>(directive, actual),
              StatusIs(absl::StatusCode::kUnimplemented,
                       "Directive 'TransformEnd' is not supported in pbrt-v4"));
}

TEST(TransformTimes, V1) {
  absl::string_view directive = R"pbrt(TransformTimes 1 2)pbrt";

  PbrtProto actual;
  EXPECT_THAT(
      Convert<1>(directive, actual),
      StatusIs(absl::StatusCode::kUnimplemented,
               "Directive 'TransformTimes' is not supported in pbrt-v1"));
}

TEST(TransformTimes, V2) {
  absl::string_view directive = R"pbrt(TransformTimes 1 2)pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert<2>(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives {
                                 transform_times { start_time: 1 end_time: 2 }
                               })pb"));
}

TEST(STranslatecale, V1) {
  absl::string_view directive = R"pbrt(Translate 1 2 3)pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert<1>(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         translate { x: 1 y: 2 z: 3 }
                                       })pb"));
}

TEST(Volume, V2) {
  absl::string_view directive = R"pbrt(Volume "a")pbrt";

  PbrtProto actual;
  EXPECT_THAT(Convert<1>(directive, actual),
              StatusIs(absl::StatusCode::kUnimplemented,
                       "Directive 'Volume' is not supported in pbrt-v1"));
}

TEST(VolumeIntegrator, V2) {
  absl::string_view directive = R"pbrt(VolumeIntegrator "a")pbrt";

  PbrtProto actual;
  EXPECT_THAT(
      Convert<1>(directive, actual),
      StatusIs(absl::StatusCode::kUnimplemented,
               "Directive 'VolumeIntegrator' is not supported in pbrt-v1"));
}

TEST(WorldBegin, V1) {
  absl::string_view directive = R"pbrt(WorldBegin)pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert<1>(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives { world_begin {} })pb"));
}

TEST(WorldEnd, V1) {
  absl::string_view directive = R"pbrt(WorldEnd)pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert<1>(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives { world_end {} })pb"));
}

TEST(WorldEnd, V4) {
  absl::string_view directive = R"pbrt(WorldEnd)pbrt";

  PbrtProto actual;
  EXPECT_THAT(Convert<4>(directive, actual),
              StatusIs(absl::StatusCode::kUnimplemented,
                       "Directive 'WorldEnd' is not supported in pbrt-v4"));
}

}  // namespace
}  // namespace pbrt_proto
