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
  EXPECT_THAT(Convert(stream), StatusIs(absl::StatusCode::kInvalidArgument,
                                        "Unrecognized directive: 'Abc'"));
}

TEST(Convert, Empty) {
  std::stringstream stream;
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto("")));
}

TEST(Convert, TryRemoveFloatTextureNoMatch) {
  std::stringstream stream(
      "Texture \"name\" \"float\" \"constant\" \"integer e\" 1");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(R"pb(directives {
                                                               float_texture {
                                                                 name: "name"
                                                                 constant {}
                                                               }
                                                             })pb")));
}

TEST(Convert, TryRemoveFloatTextureFindsFloat) {
  std::stringstream stream(
      "Texture \"name\" \"float\" \"constant\" \"float value\" 1.0");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  R"pb(directives {
                         float_texture {
                           name: "name"
                           constant { value { float_value: 1.0 } }
                         }
                       })pb")));
}

TEST(Convert, TryRemoveFloatTextureFindsTexture) {
  std::stringstream stream(
      "Texture \"name\" \"float\" \"constant\" \"texture value\" \"value\"");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  R"pb(directives {
                         float_texture {
                           name: "name"
                           constant { value { float_texture_name: "value" } }
                         }
                       })pb")));
}

TEST(Convert, TryRemoveFloatTextureFindsIgnoresOther) {
  std::stringstream stream(
      "Texture \"name\" \"float\" \"constant\" \"integer value\" 1");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(R"pb(directives {
                                                               float_texture {
                                                                 name: "name"
                                                                 constant {}
                                                               }
                                                             })pb")));
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

TEST(Convert, CameraUnknown) {
  std::stringstream stream("Camera \"unknown\"");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives { camera {} })pb")));
}

TEST(Convert, CameraEnvironment) {
  std::stringstream stream(
      "Camera \"environment\" \"float shutteropen\" 1.0 \"float shutterclose\" "
      "2.0");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(
          R"pb(directives {
                 camera { environment { shutteropen: 1.0 shutterclose: 2.0 } }
               })pb")));
}

TEST(Convert, CameraOrthographic) {
  std::stringstream stream(
      "Camera \"orthographic\" \"float lensradius\" 3.0 \"float "
      "frameaspectratio\" 4.0 \"float screenwindow\" [1.0 2.0 3.0 4.0]  "
      "\"float shutteropen\" 1.0 \"float shutterclose\" 2.0");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives {
                                              camera {
                                                orthographic {
                                                  lensradius: 3.0
                                                  frameaspectratio: 4.0
                                                  screenwindow {
                                                    x_min: 1.0
                                                    x_max: 2.0
                                                    y_min: 3.0
                                                    y_max: 4.0
                                                  }
                                                  shutteropen: 1.0
                                                  shutterclose: 2.0
                                                }
                                              }
                                            })pb")));
}

TEST(Convert, CameraOrthographicBadWindow) {
  std::stringstream stream(
      "Camera \"orthographic\" \"float lensradius\" 3.0 \"float "
      "frameaspectratio\" 4.0 \"float screenwindow\" [1.0 2.0 3.0]  "
      "\"float shutteropen\" 1.0 \"float shutterclose\" 2.0");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives {
                                              camera {
                                                orthographic {
                                                  lensradius: 3.0
                                                  frameaspectratio: 4.0
                                                  shutteropen: 1.0
                                                  shutterclose: 2.0
                                                }
                                              }
                                            })pb")));
}

TEST(Convert, CameraPerspective) {
  std::stringstream stream(
      "Camera \"perspective\" \"float lensradius\" 3.0 \"float "
      "frameaspectratio\" 4.0 \"float screenwindow\" [1.0 2.0 3.0 4.0] \"float "
      "fov\" 60.0 \"float halffov\" 30.0 \"float shutteropen\" 1.0 \"float "
      "shutterclose\" 2.0");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives {
                                              camera {
                                                perspective {
                                                  lensradius: 3.0
                                                  frameaspectratio: 4.0
                                                  screenwindow {
                                                    x_min: 1.0
                                                    x_max: 2.0
                                                    y_min: 3.0
                                                    y_max: 4.0
                                                  }
                                                  fov: 60
                                                  halffov: 30
                                                  shutteropen: 1.0
                                                  shutterclose: 2.0
                                                }
                                              }
                                            })pb")));
}

TEST(Convert, CameraPerspectiveBadWindow) {
  std::stringstream stream(
      "Camera \"perspective\" \"float lensradius\" 3.0 \"float "
      "frameaspectratio\" 4.0 \"float screenwindow\" [1.0 2.0 3.0] \"float "
      "fov\" 60.0 \"float halffov\" 30.0 \"float shutteropen\" 1.0 \"float "
      "shutterclose\" 2.0");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives {
                                              camera {
                                                perspective {
                                                  lensradius: 3.0
                                                  frameaspectratio: 4.0
                                                  fov: 60
                                                  halffov: 30
                                                  shutteropen: 1.0
                                                  shutterclose: 2.0
                                                }
                                              }
                                            })pb")));
}

TEST(Convert, CameraRealistic) {
  std::stringstream stream(
      "Camera \"realistic\" \"string lensfile\" \"lens.file\" \"float "
      "aperturediameter\" 4.0 \"float focaldistance\" 2.0 \"bool "
      "simpleweighting\" \"false\" \"float shutteropen\" 1.0 \"float "
      "shutterclose\" 2.0");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives {
                                              camera {
                                                realistic {
                                                  lensfile: "lens.file"
                                                  aperturediameter: 4.0
                                                  focaldistance: 2.0
                                                  simpleweighting: false
                                                  shutteropen: 1.0
                                                  shutterclose: 2.0
                                                }
                                              }
                                            })pb")));
}

TEST(Convert, ConcatTransform) {
  std::stringstream stream(
      "ConcatTransform [1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16]");
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

TEST(Convert, FilterUnknown) {
  std::stringstream stream("Filter \"unknown\"");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives { filter {} })pb")));
}

TEST(Convert, FilterBox) {
  std::stringstream stream(
      "Filter \"box\" \"float xwidth\" 1.0 \"float ywidth\" 2.0");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(R"pb(directives {
                                      filter { box { xwidth: 1.0 ywidth: 2.0 } }
                                    })pb")));
}

TEST(Convert, FilterGaussian) {
  std::stringstream stream(
      "Filter \"gaussian\" \"float xwidth\" 1.0 \"float ywidth\" 2.0 \"float "
      "alpha\" 3.0");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(
          R"pb(directives {
                 filter { gaussian { xwidth: 1.0 ywidth: 2.0 alpha: 3.0 } }
               })pb")));
}

TEST(Convert, FilterMitchell) {
  std::stringstream stream(
      "Filter \"mitchell\" \"float xwidth\" 1.0 \"float ywidth\" 2.0 \"float "
      "B\" 3.0 \"float C\" 4.0");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(
          R"pb(directives {
                 filter { mitchell { xwidth: 1.0 ywidth: 2.0 B: 3.0 C: 4.0 } }
               })pb")));
}

TEST(Convert, FilterSinc) {
  std::stringstream stream(
      "Filter \"sinc\" \"float xwidth\" 1.0 \"float ywidth\" 2.0 \"float tau\" "
      "3.0");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  R"pb(directives {
                         filter { sinc { xwidth: 1.0 ywidth: 2.0 tau: 3.0 } }
                       })pb")));
}

TEST(Convert, FilterTriangle) {
  std::stringstream stream(
      "Filter \"triangle\" \"float xwidth\" 1.0 \"float ywidth\" 3.0");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  R"pb(directives {
                         filter { triangle { xwidth: 1.0 ywidth: 3.0 } }
                       })pb")));
}

TEST(Convert, FloatTextureUnknown) {
  std::stringstream stream("Texture \"name\" \"float\" \"unknown\"");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives {
                                              float_texture { name: "name" }
                                            })pb")));
}

TEST(Convert, FloatTextureBilerp) {
  std::stringstream stream(
      "Texture \"name\" \"float\" \"bilerp\" \"float v00\" 1.0 \"float v01\" "
      "2.0 \"float v10\" 3.0 \"float v11\" 4.0 \"string mapping\" \"uv\" "
      "\"float uscale\" 5.0 \"float vscale\" 6.0 \"float udelta\" 7.0 \"float "
      "vdelta\" 8.0 \"vector v1\" [1.0 2 3.0] \"vector v2\" [4.0 5.0 6.0]");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          float_texture {
                                            name: "name"
                                            bilerp {
                                              v00 { float_value: 1.0 }
                                              v01 { float_value: 2.0 }
                                              v10 { float_value: 3.0 }
                                              v11 { float_value: 4.0 }
                                              mapping: UV
                                              uscale: 5.0
                                              vscale: 6.0
                                              udelta: 7.0
                                              vdelta: 8.0
                                              v1 { x: 1.0 y: 2.0 z: 3.0 }
                                              v2 { x: 4.0 y: 5.0 z: 6.0 }
                                            }
                                          }
                                        })pb")));
}

TEST(Convert, FloatTextureCheckerboard2DNone) {
  std::stringstream stream(
      "Texture \"name\" \"float\" \"checkerboard\" \"integer dimensions\" 2 "
      "\"float tex1\" 1.0 \"float tex2\" 2.0 \"string aamode\" \"none\" "
      "\"string mapping\" \"uv\" \"float uscale\" 5.0 \"float vscale\" 6.0 "
      "\"float udelta\" 7.0 \"float vdelta\" 8.0 \"vector v1\" [1.0 2 3.0] "
      "\"vector v2\" [4.0 5.0 6.0]");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          float_texture {
                                            name: "name"
                                            checkerboard2d {
                                              tex1 { float_value: 1.0 }
                                              tex2 { float_value: 2.0 }
                                              aamode: NONE
                                              mapping: UV
                                              uscale: 5.0
                                              vscale: 6.0
                                              udelta: 7.0
                                              vdelta: 8.0
                                              v1 { x: 1.0 y: 2.0 z: 3.0 }
                                              v2 { x: 4.0 y: 5.0 z: 6.0 }
                                            }
                                          }
                                        })pb")));
}

TEST(Convert, FloatTextureCheckerboard2DClosedForm) {
  std::stringstream stream(
      "Texture \"name\" \"float\" \"checkerboard\" \"integer dimensions\" 2 "
      "\"float tex1\" 1.0 \"float tex2\" 2.0 \"string aamode\" \"closedform\" "
      "\"string mapping\" \"uv\" \"float uscale\" 5.0 \"float vscale\" 6.0 "
      "\"float udelta\" 7.0 \"float vdelta\" 8.0 \"vector v1\" [1.0 2 3.0] "
      "\"vector v2\" [4.0 5.0 6.0]");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          float_texture {
                                            name: "name"
                                            checkerboard2d {
                                              tex1 { float_value: 1.0 }
                                              tex2 { float_value: 2.0 }
                                              aamode: CLOSEDFORM
                                              mapping: UV
                                              uscale: 5.0
                                              vscale: 6.0
                                              udelta: 7.0
                                              vdelta: 8.0
                                              v1 { x: 1.0 y: 2.0 z: 3.0 }
                                              v2 { x: 4.0 y: 5.0 z: 6.0 }
                                            }
                                          }
                                        })pb")));
}

TEST(Convert, FloatTextureCheckerboard3D) {
  std::stringstream stream(
      "Texture \"name\" \"float\" \"checkerboard\" \"integer dimensions\" 3 "
      "\"float tex1\" 1.0 \"float tex2\" 2.0");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          float_texture {
                                            name: "name"
                                            checkerboard3d {
                                              tex1 { float_value: 1.0 }
                                              tex2 { float_value: 2.0 }
                                            }
                                          }
                                        })pb")));
}

TEST(Convert, FloatTextureCheckerboardDots) {
  std::stringstream stream(
      "Texture \"name\" \"float\" \"dots\" \"float inside\" 1.0 \"float "
      "outside\" 2.0 \"string mapping\" \"uv\" \"float uscale\" 5.0 \"float "
      "vscale\" 6.0 \"float udelta\" 7.0 \"float vdelta\" 8.0 \"vector v1\" "
      "[1.0 2 3.0] \"vector v2\" [4.0 5.0 6.0]");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          float_texture {
                                            name: "name"
                                            dots {
                                              inside { float_value: 1.0 }
                                              outside { float_value: 2.0 }
                                              mapping: UV
                                              uscale: 5.0
                                              vscale: 6.0
                                              udelta: 7.0
                                              vdelta: 8.0
                                              v1 { x: 1.0 y: 2.0 z: 3.0 }
                                              v2 { x: 4.0 y: 5.0 z: 6.0 }
                                            }
                                          }
                                        })pb")));
}

TEST(Convert, FloatTextureFbm) {
  std::stringstream stream(
      "Texture \"name\" \"float\" \"fbm\" \"integer octaves\" 1 \"float "
      "roughness\" 2.0");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          float_texture {
                                            name: "name"
                                            fbm { octaves: 1 roughness: 2.0 }
                                          }
                                        })pb")));
}

TEST(Convert, FloatTextureConstant) {
  std::stringstream stream(
      "Texture \"name\" \"float\" \"constant\" \"float value\" 1.0");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  R"pb(directives {
                         float_texture {
                           name: "name"
                           constant { value { float_value: 1.0 } }
                         }
                       })pb")));
}

TEST(Convert, FloatTextureImageMapBlack) {
  std::stringstream stream(
      "Texture \"name\" \"float\" \"imagemap\" \"string filename\" \"a\" "
      "\"string wrap\" \"black\" \"float maxanisotropy\" 1.0 \"bool "
      "trilinear\" \"true\" \"float scale\" 2.0 \"bool gamma\" \"true\" "
      "\"string mapping\" \"uv\" \"float uscale\" 5.0 \"float vscale\" 6.0 "
      "\"float udelta\" 7.0 \"float vdelta\" 8.0 \"vector v1\" [1.0 2 3.0] "
      "\"vector v2\" [4.0 5.0 6.0]");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          float_texture {
                                            name: "name"
                                            imagemap {
                                              filename: "a"
                                              wrap: BLACK
                                              maxanisotropy: 1.0
                                              trilinear: true
                                              scale: 2.0
                                              gamma: true
                                              mapping: UV
                                              uscale: 5.0
                                              vscale: 6.0
                                              udelta: 7.0
                                              vdelta: 8.0
                                              v1 { x: 1.0 y: 2.0 z: 3.0 }
                                              v2 { x: 4.0 y: 5.0 z: 6.0 }
                                            }
                                          }
                                        })pb")));
}

TEST(Convert, FloatTextureImageMapClamp) {
  std::stringstream stream(
      "Texture \"name\" \"float\" \"imagemap\" \"string filename\" \"a\" "
      "\"string wrap\" \"clamp\" \"float maxanisotropy\" 1.0 \"bool "
      "trilinear\" \"true\" \"float scale\" 2.0 \"bool gamma\" \"true\" "
      "\"string mapping\" \"uv\" \"float uscale\" 5.0 \"float vscale\" 6.0 "
      "\"float udelta\" 7.0 \"float vdelta\" 8.0 \"vector v1\" [1.0 2 3.0] "
      "\"vector v2\" [4.0 5.0 6.0]");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          float_texture {
                                            name: "name"
                                            imagemap {
                                              filename: "a"
                                              wrap: CLAMP
                                              maxanisotropy: 1.0
                                              trilinear: true
                                              scale: 2.0
                                              gamma: true
                                              mapping: UV
                                              uscale: 5.0
                                              vscale: 6.0
                                              udelta: 7.0
                                              vdelta: 8.0
                                              v1 { x: 1.0 y: 2.0 z: 3.0 }
                                              v2 { x: 4.0 y: 5.0 z: 6.0 }
                                            }
                                          }
                                        })pb")));
}

TEST(Convert, FloatTextureImageMapRepeat) {
  std::stringstream stream(
      "Texture \"name\" \"float\" \"imagemap\" \"string filename\" \"a\" "
      "\"string wrap\" \"repeat\" \"float maxanisotropy\" 1.0 \"bool "
      "trilinear\" \"true\" \"float scale\" 2.0 \"bool gamma\" \"true\" "
      "\"string mapping\" \"uv\" \"float uscale\" 5.0 \"float vscale\" 6.0 "
      "\"float udelta\" 7.0 \"float vdelta\" 8.0 \"vector v1\" [1.0 2 3.0] "
      "\"vector v2\" [4.0 5.0 6.0]");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          float_texture {
                                            name: "name"
                                            imagemap {
                                              filename: "a"
                                              wrap: REPEAT
                                              maxanisotropy: 1.0
                                              trilinear: true
                                              scale: 2.0
                                              gamma: true
                                              mapping: UV
                                              uscale: 5.0
                                              vscale: 6.0
                                              udelta: 7.0
                                              vdelta: 8.0
                                              v1 { x: 1.0 y: 2.0 z: 3.0 }
                                              v2 { x: 4.0 y: 5.0 z: 6.0 }
                                            }
                                          }
                                        })pb")));
}

TEST(Convert, FloatTextureMarble) {
  std::stringstream stream(
      "Texture \"name\" \"float\" \"marble\" \"integer octaves\" 1 \"float "
      "roughness\" 2.0 \"float scale\" 3.0 \"float variation\" 4.0");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          float_texture {
                                            name: "name"
                                            marble {
                                              octaves: 1
                                              roughness: 2.0
                                              scale: 3.0
                                              variation: 4.0
                                            }
                                          }
                                        })pb")));
}

TEST(Convert, FloatTextureMix) {
  std::stringstream stream(
      "Texture \"name\" \"float\" \"mix\" \"float tex1\" 1.0 \"float tex2\" "
      "2.0 \"float amount\" 3.0");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          float_texture {
                                            name: "name"
                                            mix {
                                              tex1 { float_value: 1.0 }
                                              tex2 { float_value: 2.0 }
                                              amount { float_value: 3.0 }
                                            }
                                          }
                                        })pb")));
}

TEST(Convert, FloatTexturePtex) {
  std::stringstream stream(
      "Texture \"name\" \"float\" \"ptex\" \"string filename\" \"a\" \"float "
      "gamma\" 2.0");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          float_texture {
                                            name: "name"
                                            ptex { filename: "a" gamma: 2.0 }
                                          }
                                        })pb")));
}

TEST(Convert, FloatTextureScale) {
  std::stringstream stream(
      "Texture \"name\" \"float\" \"scale\" \"float tex1\" 1.0 \"float tex2\" "
      "2.0");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          float_texture {
                                            name: "name"
                                            scale {
                                              tex1 { float_value: 1.0 }
                                              tex2 { float_value: 2.0 }
                                            }
                                          }
                                        })pb")));
}

TEST(Convert, FloatTextureWindy) {
  std::stringstream stream("Texture \"name\" \"float\" \"windy\"");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          float_texture {
                                            name: "name"
                                            windy {}
                                          }
                                        })pb")));
}

TEST(Convert, FloatTextureWrinkled) {
  std::stringstream stream(
      "Texture \"name\" \"float\" \"wrinkled\" \"integer octaves\" 1 \"float "
      "roughness\" 2.0");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  R"pb(directives {
                         float_texture {
                           name: "name"
                           wrinkled { octaves: 1 roughness: 2.0 }
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

TEST(Convert, IntegratorUnknown) {
  std::stringstream stream("Integrator \"unknown\"");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives { integrator {} })pb")));
}

TEST(Convert, IntegratorSpatialLighting) {
  std::stringstream stream(
      "Integrator \"path\" \"string lightsamplestrategy\" \"spatial\"");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  R"pb(directives {
                         integrator { path { lightsamplestrategy: SPATIAL } }
                       })pb")));
}

TEST(Convert, IntegratorPowerLighting) {
  std::stringstream stream(
      "Integrator \"path\" \"string lightsamplestrategy\" \"power\"");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  R"pb(directives {
                         integrator { path { lightsamplestrategy: POWER } }
                       })pb")));
}

TEST(Convert, IntegratorUniformLighting) {
  std::stringstream stream(
      "Integrator \"path\" \"string lightsamplestrategy\" \"uniform\"");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  R"pb(directives {
                         integrator { path { lightsamplestrategy: UNIFORM } }
                       })pb")));
}

TEST(Convert, IntegratorUnknownLighting) {
  std::stringstream stream(
      "Integrator \"path\" \"string lightsamplestrategy\" \"unknown\"");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  R"pb(directives {
                         integrator { path { lightsamplestrategy: SPATIAL } }
                       })pb")));
}

TEST(Convert, IntegratorAmbientOcclusion) {
  std::stringstream stream(
      "Integrator \"ambientocclusion\" \"bool cossample\" \"false\" \"integer "
      "nsamples\" 2 \"integer pixelbounds\" [1 2 3 4]");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  R"pb(directives {
                         integrator {
                           ambientocclusion {
                             cossample: false
                             nsamples: 2
                             pixelbounds { x_min: 1 x_max: 2 y_min: 3 y_max: 4 }
                           }
                         }
                       })pb")));
}

TEST(Convert, IntegratorBdpt) {
  std::stringstream stream(
      "Integrator \"bdpt\" \"integer maxdepth\" 1 \"bool visualizestrategies\" "
      "\"true\" \"bool visualizeweights\" \"true\" \"string "
      "lightsamplestrategy\" \"uniform\" \"integer pixelbounds\" [1 2 3 4]");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  R"pb(directives {
                         integrator {
                           bdpt {
                             maxdepth: 1
                             visualizestrategies: true
                             visualizeweights: true
                             lightsamplestrategy: UNIFORM
                             pixelbounds { x_min: 1 x_max: 2 y_min: 3 y_max: 4 }
                           }
                         }
                       })pb")));
}

TEST(Convert, IntegratorBdptUnknown) {
  std::stringstream stream(
      "Integrator \"bdpt\" \"integer maxdepth\" 1 \"bool visualizestrategies\" "
      "\"true\" \"bool visualizeweights\" \"true\" \"string "
      "lightsamplestrategy\" \"xxx\" \"integer pixelbounds\" [1 2 3 4]");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  R"pb(directives {
                         integrator {
                           bdpt {
                             maxdepth: 1
                             visualizestrategies: true
                             visualizeweights: true
                             lightsamplestrategy: SPATIAL
                             pixelbounds { x_min: 1 x_max: 2 y_min: 3 y_max: 4 }
                           }
                         }
                       })pb")));
}

TEST(Convert, IntegratorDirectLightingOne) {
  std::stringstream stream(
      "Integrator \"directlighting\" \"integer maxdepth\" 1 \"string "
      "strategy\" \"one\" \"integer pixelbounds\" [1 2 3 4]");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  R"pb(directives {
                         integrator {
                           directlighting {
                             maxdepth: 1
                             strategy: ONE
                             pixelbounds { x_min: 1 x_max: 2 y_min: 3 y_max: 4 }
                           }
                         }
                       })pb")));
}

TEST(Convert, IntegratorDirectLightingAll) {
  std::stringstream stream(
      "Integrator \"directlighting\" \"integer maxdepth\" 1 \"string "
      "strategy\" \"all\" \"integer pixelbounds\" [1 2 3 4]");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  R"pb(directives {
                         integrator {
                           directlighting {
                             maxdepth: 1
                             strategy: ALL
                             pixelbounds { x_min: 1 x_max: 2 y_min: 3 y_max: 4 }
                           }
                         }
                       })pb")));
}

TEST(Convert, IntegratorDirectLightingUnknown) {
  std::stringstream stream(
      "Integrator \"directlighting\" \"integer maxdepth\" 1 \"string "
      "strategy\" \"xxx\" \"integer pixelbounds\" [1 2 3 4]");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  R"pb(directives {
                         integrator {
                           directlighting {
                             maxdepth: 1
                             strategy: ALL
                             pixelbounds { x_min: 1 x_max: 2 y_min: 3 y_max: 4 }
                           }
                         }
                       })pb")));
}

TEST(Convert, IntegratorDirectLightingBadPixelBounds) {
  std::stringstream stream(
      "Integrator \"directlighting\" \"integer maxdepth\" 1 \"string "
      "strategy\" \"all\" \"integer pixelbounds\" [1 2 3]");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(
          R"pb(directives {
                 integrator { directlighting { maxdepth: 1 strategy: ALL } }
               })pb")));
}

TEST(Convert, IntegratorMlt) {
  std::stringstream stream(
      "Integrator \"mlt\" \"integer maxdepth\" 1 \"integer bootstrapsamples\" "
      "2 \"integer chains\" 3 \"integer mutationsperpixel\" 4 \"float "
      "largestepprobability\" 5.0 \"float sigma\" 6.0");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives {
                                              integrator {
                                                mlt {
                                                  maxdepth: 1
                                                  bootstrapsamples: 2
                                                  chains: 3
                                                  mutationsperpixel: 4
                                                  largestepprobability: 5.0
                                                  sigma: 6.0
                                                }
                                              }
                                            })pb")));
}

TEST(Convert, IntegratorPath) {
  std::stringstream stream(
      "Integrator \"path\" \"integer maxdepth\" 1 \"float rrthreshold\" 2.0 "
      "\"string lightsamplestrategy\" \"uniform\" \"integer pixelbounds\" [1 2 "
      "3 4]");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  R"pb(directives {
                         integrator {
                           path {
                             maxdepth: 1
                             rrthreshold: 2.0
                             lightsamplestrategy: UNIFORM
                             pixelbounds { x_min: 1 x_max: 2 y_min: 3 y_max: 4 }
                           }
                         }
                       })pb")));
}

TEST(Convert, IntegratorPathBadPixelBounds) {
  std::stringstream stream(
      "Integrator \"path\" \"integer maxdepth\" 1 \"float rrthreshold\" 2.0 "
      "\"string lightsamplestrategy\" \"uniform\" \"integer pixelbounds\" [1 2 "
      "3]");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          integrator {
                                            path {
                                              maxdepth: 1
                                              rrthreshold: 2.0
                                              lightsamplestrategy: UNIFORM
                                            }
                                          }
                                        })pb")));
}

TEST(Convert, IntegratorPathUnknown) {
  std::stringstream stream(
      "Integrator \"path\" \"integer maxdepth\" 1 \"float rrthreshold\" 2.0 "
      "\"string lightsamplestrategy\" \"xxx\" \"integer pixelbounds\" [1 2 "
      "3 4]");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  R"pb(directives {
                         integrator {
                           path {
                             maxdepth: 1
                             rrthreshold: 2.0
                             lightsamplestrategy: SPATIAL
                             pixelbounds { x_min: 1 x_max: 2 y_min: 3 y_max: 4 }
                           }
                         }
                       })pb")));
}

TEST(Convert, IntegratorSppm) {
  std::stringstream stream(
      "Integrator \"sppm\" \"integer maxdepth\" 1 \"integer numiterations\" 2 "
      "\"integer photonsperiteration\" 3 \"integer imagewritefrequency\" 4 "
      "\"float radius\" 5.0");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives {
                                              integrator {
                                                sppm {
                                                  maxdepth: 1
                                                  numiterations: 2
                                                  photonsperiteration: 3
                                                  imagewritefrequency: 4
                                                  radius: 5.0
                                                }
                                              }
                                            })pb")));
}

TEST(Convert, IntegratorVolPath) {
  std::stringstream stream(
      "Integrator \"volpath\" \"integer maxdepth\" 1 \"float rrthreshold\" 2.0 "
      "\"string lightsamplestrategy\" \"uniform\" \"integer pixelbounds\" [1 2 "
      "3 4]");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  R"pb(directives {
                         integrator {
                           volpath {
                             maxdepth: 1
                             rrthreshold: 2.0
                             lightsamplestrategy: UNIFORM
                             pixelbounds { x_min: 1 x_max: 2 y_min: 3 y_max: 4 }
                           }
                         }
                       })pb")));
}

TEST(Convert, IntegratorVolPathBadPixelBounds) {
  std::stringstream stream(
      "Integrator \"volpath\" \"integer maxdepth\" 1 \"float rrthreshold\" 2.0 "
      "\"string lightsamplestrategy\" \"uniform\" \"integer pixelbounds\" [1 2 "
      "3]");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          integrator {
                                            volpath {
                                              maxdepth: 1
                                              rrthreshold: 2.0
                                              lightsamplestrategy: UNIFORM
                                            }
                                          }
                                        })pb")));
}

TEST(Convert, IntegratorVolPathUnknown) {
  std::stringstream stream(
      "Integrator \"volpath\" \"integer maxdepth\" 1 \"float rrthreshold\" 2.0 "
      "\"string lightsamplestrategy\" \"xxx\" \"integer pixelbounds\" [1 2 "
      "3 4]");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  R"pb(directives {
                         integrator {
                           volpath {
                             maxdepth: 1
                             rrthreshold: 2.0
                             lightsamplestrategy: SPATIAL
                             pixelbounds { x_min: 1 x_max: 2 y_min: 3 y_max: 4 }
                           }
                         }
                       })pb")));
}

TEST(Convert, IntegratorWhitted) {
  std::stringstream stream(
      "Integrator \"whitted\" \"integer maxdepth\" 1 \"integer pixelbounds\" "
      "[1 2 3 4]");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  R"pb(directives {
                         integrator {
                           whitted {
                             maxdepth: 1
                             pixelbounds { x_min: 1 x_max: 2 y_min: 3 y_max: 4 }
                           }
                         }
                       })pb")));
}

TEST(Convert, IntegratorWhittedBadPixelBounds) {
  std::stringstream stream(
      "Integrator \"whitted\" \"integer maxdepth\" 1 \"integer pixelbounds\" "
      "[1 2 3]");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          integrator { whitted { maxdepth: 1 } }
                                        })pb")));
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

TEST(Convert, ObjectBegin) {
  std::stringstream stream("ObjectBegin \"a\"");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives {
                                              object_begin { name: "a" }
                                            })pb")));
}

TEST(Convert, ObjectEnd) {
  std::stringstream stream("ObjectEnd");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives { object_end {} })pb")));
}

TEST(Convert, ObjectInstance) {
  std::stringstream stream("ObjectInstance \"a\"");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives {
                                              object_instance { name: "a" }
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

TEST(Convert, SamplerUnknown) {
  std::stringstream stream("Sampler \"unknown\"");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives { sampler {} })pb")));
}

TEST(Convert, SamplerHalton) {
  std::stringstream stream(
      "Sampler \"halton\" \"integer pixelsamples\" 2 \"bool "
      "samplepixelcenter\" \"true\"");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(
          R"pb(directives {
                 sampler { halton { pixelsamples: 2 samplepixelcenter: true } }
               })pb")));
}

TEST(Convert, SamplerMaxMinDist) {
  std::stringstream stream(
      "Sampler \"maxmindist\" \"integer pixelsamples\" 2 \"integer "
      "dimensions\" 3");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(
          R"pb(directives {
                 sampler { maxmindist { pixelsamples: 2 dimensions: 3 } }
               })pb")));
}

TEST(Convert, SamplerRandom) {
  std::stringstream stream("Sampler \"random\" \"integer pixelsamples\" 2");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          sampler { random { pixelsamples: 2 } }
                                        })pb")));
}

TEST(Convert, SamplerSobol) {
  std::stringstream stream("Sampler \"sobol\" \"integer pixelsamples\" 2");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          sampler { sobol { pixelsamples: 2 } }
                                        })pb")));
}

TEST(Convert, SamplerStratified) {
  std::stringstream stream(
      "Sampler \"stratified\" \"bool jitter\" \"false\" \"integer xsamples\" 3 "
      "\"integer ysamples\" 5 \"integer dimensions\" 7");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          sampler {
                                            stratified {
                                              jitter: false
                                              xsamples: 3
                                              ysamples: 5
                                              dimensions: 7
                                            }
                                          }
                                        })pb")));
}

TEST(Convert, SamplerZeroTwoSequence) {
  std::stringstream stream(
      "Sampler \"02sequence\" \"integer pixelsamples\" 2 \"integer "
      "dimensions\" 3");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(
          R"pb(directives {
                 sampler { zerotwosequence { pixelsamples: 2 dimensions: 3 } }
               })pb")));
}

TEST(Convert, SamplerLowDiscrepancy) {
  std::stringstream stream(
      "Sampler \"lowdiscrepancy\" \"integer pixelsamples\" 2 \"integer "
      "dimensions\" 3");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(
          R"pb(directives {
                 sampler { zerotwosequence { pixelsamples: 2 dimensions: 3 } }
               })pb")));
}

TEST(Convert, Transform) {
  std::stringstream stream(
      "Transform [1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16]");
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