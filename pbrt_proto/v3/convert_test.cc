#include "pbrt_proto/v3/convert.h"

#include <sstream>

#include "absl/status/status_matchers.h"
#include "googlemock/include/gmock/gmock.h"
#include "googletest/include/gtest/gtest.h"
#include "pbrt_proto/testing/proto_matchers.h"
#include "pbrt_proto/v3/pbrt.pb.h"

namespace pbrt_proto::v3 {
namespace {

using ::absl_testing::IsOkAndHolds;
using ::absl_testing::StatusIs;
using ::google::protobuf::EqualsProto;

TEST(Convert, Error) {
  std::stringstream stream("Abc");
  EXPECT_THAT(Convert(stream),
              StatusIs(absl::StatusCode::kUnimplemented, "Abc"));
}

TEST(Convert, Empty) {
  std::stringstream stream;
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto("")));
}

TEST(Convert, ActiveTransformAll) {
  std::stringstream stream("ActiveTransform All");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  "directives { active_transform { active: ALL } }")));
}

TEST(Convert, ActiveTransformStart) {
  std::stringstream stream("ActiveTransform StartTime");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  "directives { active_transform { active: START_TIME } }")));
}

TEST(Convert, ActiveTransformEnd) {
  std::stringstream stream("ActiveTransform EndTime");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  "directives { active_transform { active: END_TIME } }")));
}

TEST(Convert, ConcatTransform) {
  std::stringstream stream(
      "ConcatTransform 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives {
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
                                            })pb")));
}

TEST(Convert, CoordinateSystem) {
  std::stringstream stream("CoordinateSystem \"a\"");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives {
                                              coordinate_system { name: "a" }
                                            })pb")));
}

TEST(Convert, CoordSysTransform) {
  std::stringstream stream("CoordSysTransform \"a\"");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives {
                                              coord_sys_transform { name: "a" }
                                            })pb")));
}

TEST(Convert, Identity) {
  std::stringstream stream("Identity");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives { identity {} })pb")));
}

TEST(Convert, Include) {
  std::stringstream stream("Include \"a\"");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(R"pb(directives { include { path: "a" } })pb")));
}

TEST(Convert, Import) {
  std::stringstream stream("Import \"a\"");
  EXPECT_THAT(Convert(stream),
              StatusIs(absl::StatusCode::kUnimplemented,
                       "Directive 'Import' is not supported in pbrt-v3"));
}

TEST(Convert, LookAt) {
  std::stringstream stream("LookAt 1 2 3 4 5 6 7 8 9");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(R"pb(directives {
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
                                                             })pb")));
}

TEST(Convert, ReverseOrientation) {
  std::stringstream stream("ReverseOrientation");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(
                  EqualsProto(R"pb(directives { reverse_orientation {} })pb")));
}

TEST(Convert, Rotate) {
  std::stringstream stream("Rotate 1 2 3 4");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives {
                                              rotate { angle: 1 x: 2 y: 3 z: 4 }
                                            })pb")));
}

TEST(Convert, Scale) {
  std::stringstream stream("Scale 1 2 3");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives {
                                              scale { x: 1 y: 2 z: 3 }
                                            })pb")));
}

TEST(Convert, Transform) {
  std::stringstream stream("Transform 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(R"pb(directives {
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
                                                             })pb")));
}

TEST(Convert, TransformTimes) {
  std::stringstream stream("TransformTimes 1 2");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  R"pb(directives {
                         transform_times { start_time: 1 end_time: 2 }
                       })pb")));
}

TEST(Convert, Translate) {
  std::stringstream stream("Translate 1 2 3");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives {
                                              translate { x: 1 y: 2 z: 3 }
                                            })pb")));
}

TEST(Convert, WorldBegin) {
  std::stringstream stream("WorldBegin");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(R"pb(directives {
                                                               world_begin {}
                                                             })pb")));
}

TEST(Convert, WorldEnd) {
  std::stringstream stream("WorldEnd");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives { world_end {} })pb")));
}

TEST(Convert, MultipleDirectives) {
  std::stringstream stream("WorldBegin WorldEnd");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives { world_begin {} }
                                            directives { world_end {} })pb")));
}

}  // namespace
}  // namespace pbrt_proto::v3