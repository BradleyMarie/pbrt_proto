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

TEST(Convert, AcceleratorUnknown) {
  std::stringstream stream("Accelerator \"unknown\"");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(R"pb(directives {
                                                               accelerator {}
                                                             })pb")));
}

TEST(Convert, AcceleratorBvhEmpty) {
  std::stringstream stream("Accelerator \"bvh\"");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(
                  EqualsProto(R"pb(directives { accelerator { bvh {} } })pb")));
}

TEST(Convert, AcceleratorBvhSAH) {
  std::stringstream stream(
      "Accelerator \"bvh\" \"integer maxnodeprims\" 5 \"string splitmethod\" "
      "\"sah\"");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(
          R"pb(directives {
                 accelerator { bvh { maxnodeprims: 5 splitmethod: SAH } }
               })pb")));
}

TEST(Convert, AcceleratorBvhMiddle) {
  std::stringstream stream(
      "Accelerator \"bvh\" \"integer maxnodeprims\" 5 \"string splitmethod\" "
      "\"middle\"");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(
          R"pb(directives {
                 accelerator { bvh { maxnodeprims: 5 splitmethod: MIDDLE } }
               })pb")));
}

TEST(Convert, AcceleratorBvhEqual) {
  std::stringstream stream(
      "Accelerator \"bvh\" \"integer maxnodeprims\" 5 \"string splitmethod\" "
      "\"equal\"");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(
          R"pb(directives {
                 accelerator { bvh { maxnodeprims: 5 splitmethod: EQUAL } }
               })pb")));
}

TEST(Convert, AcceleratorBvhHlbvh) {
  std::stringstream stream(
      "Accelerator \"bvh\" \"integer maxnodeprims\" 5 \"string splitmethod\" "
      "\"hlbvh\"");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(
          R"pb(directives {
                 accelerator { bvh { maxnodeprims: 5 splitmethod: HLBVH } }
               })pb")));
}

TEST(Convert, AcceleratorBvhBad) {
  std::stringstream stream(
      "Accelerator \"bvh\" \"integer maxnodeprims\" 5 \"string splitmethod\" "
      "\"invalid\"");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(R"pb(directives {
                                      accelerator { bvh { maxnodeprims: 5 } }
                                    })pb")));
}

TEST(Convert, AcceleratorKdTreeEmpty) {
  std::stringstream stream("Accelerator \"kdtree\"");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives {
                                              accelerator { kdtree {} }
                                            })pb")));
}

TEST(Convert, AcceleratorKdTree) {
  std::stringstream stream(
      "Accelerator \"kdtree\" \"integer intersectcost\" 1 \"integer "
      "traversalcost\" 2 \"float emptybonus\" 3 \"integer maxprims\" 4 "
      "\"integer maxdepth\" 5");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives {
                                              accelerator {
                                                kdtree {
                                                  intersectcost: 1
                                                  traversalcost: 2
                                                  emptybonus: 3
                                                  maxprims: 4
                                                  maxdepth: 5
                                                }
                                              }
                                            })pb")));
}

TEST(Convert, ActiveTransformAll) {
  std::stringstream stream("ActiveTransform All");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  "directives { active_transform { active: ALL } }")));
}

TEST(Convert, ActiveTransformStart) {
  std::stringstream stream("ActiveTransform StartTime");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(R"pb(directives {
                                      active_transform { active: START_TIME }
                                    })pb")));
}

TEST(Convert, ActiveTransformEnd) {
  std::stringstream stream("ActiveTransform EndTime");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(R"pb(directives {
                                      active_transform { active: END_TIME }
                                    })pb")));
}

TEST(Convert, AttributeBegin) {
  std::stringstream stream("AttributeBegin");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(R"pb(directives { attribute_begin {} })pb")));
}

TEST(Convert, AttributeEnd) {
  std::stringstream stream("AttributeEnd");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(R"pb(directives {
                                                               attribute_end {}
                                                             })pb")));
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

TEST(Convert, FilmUnknown) {
  std::stringstream stream("Film \"unknown\"");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives { film {} })pb")));
}

TEST(Convert, FilmImageBadCropWindow) {
  std::stringstream stream("Film \"image\" \"float cropwindow\" [1 2 3]");
  EXPECT_THAT(
      Convert(stream),
      StatusIs(
          absl::StatusCode::kInvalidArgument,
          "Invalid number of values for Film float parameter: 'cropwindow'"));
}

TEST(Convert, FilmImage) {
  std::stringstream stream(
      "Film \"image\" \"integer xresolution\" 200 \"integer yresolution\" 300 "
      "\"float cropwindow\" [1 2 3 4] \"float scale\" 2.0 \"float "
      "maxsampleluminance\" 3.0 \"float diagonal\" 4.0 \"string filename\" "
      "\"out.png\"");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(
          R"pb(directives {
                 film {
                   image {
                     xresolution: 200
                     yresolution: 300
                     cropwindow { x_min: 1.0 x_max: 2.0 y_min: 3.0 y_max: 4.0 }
                     scale: 2.0
                     maxsampleluminance: 3.0
                     diagonal: 4.0
                     filename: "out.png"
                   }
                 }
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

TEST(Convert, TransformBegin) {
  std::stringstream stream("TransformBegin");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(R"pb(directives { transform_begin {} })pb")));
}

TEST(Convert, TransformEnd) {
  std::stringstream stream("TransformEnd");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(R"pb(directives {
                                                               transform_end {}
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