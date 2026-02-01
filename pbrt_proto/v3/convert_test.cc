#include "pbrt_proto/v3/convert.h"

#include <sstream>

#include "absl/status/status_matchers.h"
#include "googlemock/include/gmock/gmock.h"
#include "googletest/include/gtest/gtest.h"
#include "pbrt_proto/testing/proto_matchers.h"
#include "pbrt_proto/v3/v3.pb.h"

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

TEST(Convert, AreaLightSourceUnknown) {
  std::stringstream stream("AreaLightSource \"unknown\"");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(R"pb(directives { area_light_source {} })pb")));
}

TEST(Convert, AreaLightSourceDiffuse) {
  std::stringstream stream(
      "AreaLightSource \"diffuse\" \"blackbody L\" [1.0 2.0] \"bool twosided\" "
      "\"true\" \"integer samples\" 2 \"blackbody scale\" [11.0 12.0]");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(
          R"pb(directives {
                 area_light_source {
                   diffuse {
                     L { blackbody_spectrum { temperature: 1.0 scale: 2.0 } }
                     twosided: true
                     samples: 2
                     scale {
                       blackbody_spectrum { temperature: 11.0 scale: 12.0 }
                     }
                   }
                 }
               })pb")));
}

TEST(Convert, AreaLightSourceDiffuseNSamples) {
  std::stringstream stream(
      "AreaLightSource \"diffuse\" \"blackbody L\" [1.0 2.0] \"bool twosided\" "
      "\"true\" \"integer nsamples\" 2 \"blackbody scale\" [11.0 12.0]");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(
          R"pb(directives {
                 area_light_source {
                   diffuse {
                     L { blackbody_spectrum { temperature: 1.0 scale: 2.0 } }
                     twosided: true
                     samples: 2
                     scale {
                       blackbody_spectrum { temperature: 11.0 scale: 12.0 }
                     }
                   }
                 }
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
      "Camera \"environment\" \"float lensradius\" 3.0 \"float "
      "frameaspectratio\" 4.0 \"float screenwindow\" [1.0 2.0 3.0 4.0]  "
      "\"float shutteropen\" 1.0 \"float shutterclose\" 2.0");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives {
                                              camera {
                                                environment {
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

TEST(Convert, CameraEnvironmentBadWindow) {
  std::stringstream stream(
      "Camera \"environment\" \"float lensradius\" 3.0 \"float "
      "frameaspectratio\" 4.0 \"float screenwindow\" [1.0 2.0 3.0]  "
      "\"float shutteropen\" 1.0 \"float shutterclose\" 2.0");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives {
                                              camera {
                                                environment {
                                                  lensradius: 3.0
                                                  frameaspectratio: 4.0
                                                  shutteropen: 1.0
                                                  shutterclose: 2.0
                                                }
                                              }
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
      "aperturediameter\" 4.0 \"float focusdistance\" 2.0 \"bool "
      "simpleweighting\" \"false\" \"float shutteropen\" 1.0 \"float "
      "shutterclose\" 2.0");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives {
                                              camera {
                                                realistic {
                                                  lensfile: "lens.file"
                                                  aperturediameter: 4.0
                                                  focusdistance: 2.0
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
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          float_texture {
                                            name: "name"
                                            constant { value: 1.0 }
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
  std::stringstream stream("Texture \"name\" \"float\" \"marble\"");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          float_texture {
                                            name: "name"
                                            marble {}
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

TEST(Convert, LightSourceUnknown) {
  std::stringstream stream("LightSource \"unknown\"");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(R"pb(directives {
                                                               light_source {}
                                                             })pb")));
}

TEST(Convert, LightSourceDistant) {
  std::stringstream stream(
      "LightSource \"distant\" \"blackbody L\" [1.0 2.0] \"point from\" [3.0 "
      "4.0 5.0] \"point to\" [6.0 7.0 8.0] \"blackbody scale\" [9.0 10.0]");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(
          R"pb(directives {
                 light_source {
                   distant {
                     L { blackbody_spectrum { temperature: 1.0 scale: 2.0 } }
                     from { x: 3.0 y: 4.0 z: 5.0 }
                     to { x: 6.0 y: 7.0 z: 8.0 }
                     scale {
                       blackbody_spectrum { temperature: 9.0 scale: 10.0 }
                     }
                   }
                 }
               })pb")));
}

TEST(Convert, LightSourceGoniometric) {
  std::stringstream stream(
      "LightSource \"goniometric\" \"blackbody I\" [1.0 2.0] \"string "
      "mapname\" \"a\" \"blackbody scale\" [9.0 10.0]");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(
          R"pb(directives {
                 light_source {
                   goniometric {
                     I { blackbody_spectrum { temperature: 1.0 scale: 2.0 } }
                     mapname: "a"
                     scale {
                       blackbody_spectrum { temperature: 9.0 scale: 10.0 }
                     }
                   }
                 }
               })pb")));
}

TEST(Convert, LightSourceInfinite) {
  std::stringstream stream(
      "LightSource \"infinite\" \"blackbody L\" [1.0 2.0] \"integer samples\" "
      "3 \"string mapname\" \"a\" \"blackbody scale\" [9.0 10.0]");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(
          R"pb(directives {
                 light_source {
                   infinite {
                     L { blackbody_spectrum { temperature: 1.0 scale: 2.0 } }
                     samples: 3
                     mapname: "a"
                     scale {
                       blackbody_spectrum { temperature: 9.0 scale: 10.0 }
                     }
                   }
                 }
               })pb")));
}

TEST(Convert, LightSourceInfiniteNSamples) {
  std::stringstream stream(
      "LightSource \"infinite\" \"blackbody L\" [1.0 2.0] \"integer nsamples\" "
      "3 \"string mapname\" \"a\" \"blackbody scale\" [9.0 10.0]");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(
          R"pb(directives {
                 light_source {
                   infinite {
                     L { blackbody_spectrum { temperature: 1.0 scale: 2.0 } }
                     samples: 3
                     mapname: "a"
                     scale {
                       blackbody_spectrum { temperature: 9.0 scale: 10.0 }
                     }
                   }
                 }
               })pb")));
}

TEST(Convert, LightSourcePoint) {
  std::stringstream stream(
      "LightSource \"point\" \"blackbody I\" [1.0 2.0] \"point from\" [3.0 4.0 "
      "5.0] \"blackbody scale\" [9.0 10.0]");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(
          R"pb(directives {
                 light_source {
                   point {
                     I { blackbody_spectrum { temperature: 1.0 scale: 2.0 } }
                     from { x: 3.0 y: 4.0 z: 5.0 }
                     scale {
                       blackbody_spectrum { temperature: 9.0 scale: 10.0 }
                     }
                   }
                 }
               })pb")));
}

TEST(Convert, LightSourceProjection) {
  std::stringstream stream(
      "LightSource \"projection\" \"blackbody I\" [1.0 2.0] \"float fov\" 3.0 "
      "\"string mapname\" \"a\" \"blackbody scale\" [9.0 10.0]");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(
          R"pb(directives {
                 light_source {
                   projection {
                     I { blackbody_spectrum { temperature: 1.0 scale: 2.0 } }
                     fov: 3.0
                     mapname: "a"
                     scale {
                       blackbody_spectrum { temperature: 9.0 scale: 10.0 }
                     }
                   }
                 }
               })pb")));
}

TEST(Convert, LightSourceSpot) {
  std::stringstream stream(
      "LightSource \"spot\" \"blackbody I\" [1.0 2.0] \"point from\" [3.0 4.0 "
      "5.0] \"point to\" [6.0 7.0 8.0] \"float coneangle\" 9.0 \"float "
      "conedeltaangle\" 10.0 \"blackbody scale\" [11.0 12.0]");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(
          R"pb(directives {
                 light_source {
                   spot {
                     I { blackbody_spectrum { temperature: 1.0 scale: 2.0 } }
                     from { x: 3.0 y: 4.0 z: 5.0 }
                     to { x: 6.0 y: 7.0 z: 8.0 }
                     coneangle: 9.0
                     conedeltaangle: 10.0
                     scale {
                       blackbody_spectrum { temperature: 11.0 scale: 12.0 }
                     }
                   }
                 }
               })pb")));
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

TEST(Convert, MakeNamedMaterialEmpty) {
  std::stringstream stream("MakeNamedMaterial \"a\" \"string type\" \"\"");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives {
                                              make_named_material {
                                                name: "a"
                                                material {}
                                              }
                                            })pb")));
}

TEST(Convert, MakeNamedMaterialNone) {
  std::stringstream stream("MakeNamedMaterial \"a\" \"string type\" \"none\"");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives {
                                              make_named_material {
                                                name: "a"
                                                material {}
                                              }
                                            })pb")));
}

TEST(Convert, MakeNamedMediumNone) {
  std::stringstream stream("MakeNamedMedium \"a\" ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives {
                                              make_named_medium { name: "a" }
                                            })pb")));
}

TEST(Convert, MakeNamedMediumHomogeneous) {
  std::stringstream stream(
      "MakeNamedMedium \"a\""
      "\"string type\" \"homogeneous\" "
      "\"string preset\" \"Apple\" "
      "\"spectrum sigma_a\" \"a\" "
      "\"spectrum sigma_s\" \"b\" "
      "\"float g\" 1.0 "
      "\"float scale\" 2.0 ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  R"pb(directives {
                         make_named_medium {
                           name: "a"
                           homogeneous {
                             preset: APPLE
                             sigma_a { sampled_spectrum_filename: "a" }
                             sigma_s { sampled_spectrum_filename: "b" }
                             g: 1.0
                             scale: 2.0
                           }
                         }
                       })pb")));
}

TEST(Convert, MakeNamedMediumHeterogeneous) {
  std::stringstream stream(
      "MakeNamedMedium \"a\""
      "\"string type\" \"heterogeneous\" "
      "\"string preset\" \"Apple\" "
      "\"spectrum sigma_a\" \"a\" "
      "\"spectrum sigma_s\" \"b\" "
      "\"float g\" 1.0 "
      "\"float scale\" 2.0 "
      "\"point p0\" [3 4 5] "
      "\"point p1\" [6 7 8] "
      "\"integer nx\" 1 "
      "\"integer ny\" 2 "
      "\"integer nz\" 3 "
      "\"float density\" [1 2 3 4 5 6] ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  R"pb(directives {
                         make_named_medium {
                           name: "a"
                           heterogeneous {
                             preset: APPLE
                             sigma_a { sampled_spectrum_filename: "a" }
                             sigma_s { sampled_spectrum_filename: "b" }
                             g: 1.0
                             scale: 2.0
                             p0 { x: 3.0 y: 4.0 z: 5.0 }
                             p1 { x: 6.0 y: 7.0 z: 8.0 }
                             nx: 1
                             ny: 2
                             nz: 3
                             density: 1
                             density: 2
                             density: 3
                             density: 4
                             density: 5
                             density: 6
                           }
                         }
                       })pb")));
}

TEST(Convert, MakeNamedMediumHeterogeneousBadNx) {
  std::stringstream stream(
      "MakeNamedMedium \"a\""
      "\"integer nx\" -1 "
      "\"integer ny\" 2 "
      "\"integer nz\" 3 "
      "\"float density\" [1 2 3 4 5 6] ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives {
                                              make_named_medium { name: "a" }
                                            })pb")));
}

TEST(Convert, MakeNamedMediumHeterogeneousBadNy) {
  std::stringstream stream(
      "MakeNamedMedium \"a\""
      "\"integer nx\" 1 "
      "\"integer ny\" -2 "
      "\"integer nz\" 3 "
      "\"float density\" [1 2 3 4 5 6] ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives {
                                              make_named_medium { name: "a" }
                                            })pb")));
}

TEST(Convert, MakeNamedMediumHeterogeneousBadNz) {
  std::stringstream stream(
      "MakeNamedMedium \"a\""
      "\"integer nx\" 1 "
      "\"integer ny\" 2 "
      "\"integer nz\" -3 "
      "\"float density\" [1 2 3 4 5 6] ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives {
                                              make_named_medium { name: "a" }
                                            })pb")));
}

TEST(Convert, MakeNamedMediumHeterogeneousNoDensity) {
  std::stringstream stream(
      "MakeNamedMedium \"a\""
      "\"string type\" \"heterogeneous\" ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives {
                                              make_named_medium { name: "a" }
                                            })pb")));
}

TEST(Convert, MakeNamedMediumHeterogeneousDensityWrongSize) {
  std::stringstream stream(
      "MakeNamedMedium \"a\""
      "\"integer nx\" 1 "
      "\"integer ny\" 2 "
      "\"integer nz\" 3 "
      "\"float density\" [1 2 3 4 5] ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives {
                                              make_named_medium { name: "a" }
                                            })pb")));
}

TEST(Convert, MaterialEmpty) {
  std::stringstream stream("Material \"\"");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives { material {} })pb")));
}

TEST(Convert, MaterialEmptyNone) {
  std::stringstream stream("Material \"none\"");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives { material {} })pb")));
}

TEST(Convert, MaterialInvalid) {
  std::stringstream stream("Material \"abcdefg\"");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(R"pb(directives { material { matte {} } })pb")));
}

TEST(Convert, MaterialDisney) {
  std::stringstream stream(
      "Material \"disney\" "
      "\"texture color\" \"a\" "
      "\"float anisotropic\" 1.0 "
      "\"float clearcoat\" 2.0 "
      "\"float clearcoatgloss\" 3.0 "
      "\"float eta\" 4.0 "
      "\"float metallic\" 5.0 "
      "\"float roughness\" 6.0 "
      "\"texture scatterdistance\" \"b\" "
      "\"float sheen\" 7.0 "
      "\"float sheentint\" 8.0 "
      "\"float spectrans\" 9.0 "
      "\"float speculartint\" 10.0 "
      "\"bool thin\" \"true\" "
      "\"float difftrans\" 11.0 "
      "\"float flatness\" 12.0 "
      "\"float bumpmap\" 13.0 ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  R"pb(directives {
                         material {
                           disney {
                             color { spectrum_texture_name: "a" }
                             scatterdistance { spectrum_texture_name: "b" }
                             anisotropic { float_value: 1.0 }
                             clearcoat { float_value: 2.0 }
                             clearcoatgloss { float_value: 3.0 }
                             eta { float_value: 4.0 }
                             metallic { float_value: 5.0 }
                             roughness { float_value: 6.0 }
                             sheen { float_value: 7.0 }
                             sheentint { float_value: 8.0 }
                             spectrans { float_value: 9.0 }
                             speculartint { float_value: 10.0 }
                             difftrans { float_value: 11.0 }
                             flatness { float_value: 12.0 }
                             bumpmap { float_value: 13.0 }
                             thin: true
                           }
                         }
                       })pb")));
}

TEST(Convert, MaterialFourier) {
  std::stringstream stream(
      "Material \"fourier\" "
      "\"string bsdffile\" \"a\" "
      "\"float bumpmap\" 1.0 ");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          material {
                                            fourier {
                                              bsdffile: "a"
                                              bumpmap { float_value: 1.0 }
                                            }
                                          }
                                        })pb")));
}

TEST(Convert, MaterialGlass) {
  std::stringstream stream(
      "Material \"glass\" "
      "\"texture Kr\" \"a\" "
      "\"texture Kt\" \"b\" "
      "\"float eta\" 1.0 "
      "\"float uroughness\" 2.0 "
      "\"float vroughness\" 3.0 "
      "\"bool remaproughness\" \"false\" "
      "\"float bumpmap\" 4.0 ");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          material {
                                            glass {
                                              Kr { spectrum_texture_name: "a" }
                                              Kt { spectrum_texture_name: "b" }
                                              eta { float_value: 1.0 }
                                              uroughness { float_value: 2.0 }
                                              vroughness { float_value: 3.0 }
                                              remaproughness: false
                                              bumpmap { float_value: 4.0 }
                                            }
                                          }
                                        })pb")));
}

TEST(Convert, MaterialGlassIndex) {
  std::stringstream stream(
      "Material \"glass\" "
      "\"texture Kr\" \"a\" "
      "\"texture Kt\" \"b\" "
      "\"float index\" 1.0 "
      "\"float uroughness\" 2.0 "
      "\"float vroughness\" 3.0 "
      "\"float bumpmap\" 4.0 ");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          material {
                                            glass {
                                              Kr { spectrum_texture_name: "a" }
                                              Kt { spectrum_texture_name: "b" }
                                              eta { float_value: 1.0 }
                                              uroughness { float_value: 2.0 }
                                              vroughness { float_value: 3.0 }
                                              bumpmap { float_value: 4.0 }
                                            }
                                          }
                                        })pb")));
}

TEST(Convert, MaterialHair) {
  std::stringstream stream(
      "Material \"hair\" "
      "\"texture sigma_a\" \"a\" "
      "\"texture color\" \"b\" "
      "\"float eumelanin\" 1.0 "
      "\"float pheomelanin\" 2.0 "
      "\"float eta\" 3.0 "
      "\"float beta_m\" 4.0 "
      "\"float beta_n\" 5.0 "
      "\"float alpha\" 6.0 "
      "\"float bumpmap\" 7.0 ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  R"pb(directives {
                         material {
                           hair {
                             sigma_a { spectrum_texture_name: "a" }
                             color { spectrum_texture_name: "b" }
                             eumelanin { float_value: 1.0 }
                             pheomelanin { float_value: 2.0 }
                             eta { float_value: 3.0 }
                             beta_m { float_value: 4.0 }
                             beta_n { float_value: 5.0 }
                             alpha { float_value: 6.0 }
                             bumpmap { float_value: 7.0 }
                           }
                         }
                       })pb")));
}

TEST(Convert, MaterialKdSubsurface) {
  std::stringstream stream(
      "Material \"kdsubsurface\" "
      "\"texture Kd\" \"a\" "
      "\"texture Kr\" \"b\" "
      "\"texture Kt\" \"c\" "
      "\"texture mfp\" \"d\" "
      "\"float uroughness\" 2.0 "
      "\"float vroughness\" 3.0 "
      "\"float eta\" 4.0 "
      "\"float scale\" 5.0 "
      "\"float g\" 6.0 "
      "\"bool remaproughness\" \"false\" "
      "\"float bumpmap\" 7.0 ");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          material {
                                            kdsubsurface {
                                              Kd { spectrum_texture_name: "a" }
                                              Kr { spectrum_texture_name: "b" }
                                              Kt { spectrum_texture_name: "c" }
                                              mfp { spectrum_texture_name: "d" }
                                              uroughness { float_value: 2.0 }
                                              vroughness { float_value: 3.0 }
                                              eta: 4.0
                                              scale: 5.0
                                              g: 6.0
                                              remaproughness: false
                                              bumpmap { float_value: 7.0 }
                                            }
                                          }
                                        })pb")));
}

TEST(Convert, MaterialMatte) {
  std::stringstream stream(
      "Material \"matte\" \"texture Kd\" \"a\" \"float sigma\" 1.0 \"float "
      "bumpmap\" 2.0");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(R"pb(directives {
                                      material {
                                        matte {
                                          Kd { spectrum_texture_name: "a" }
                                          sigma { float_value: 1.0 }
                                          bumpmap { float_value: 2.0 }
                                        }
                                      }
                                    })pb")));
}

TEST(Convert, MaterialMetal) {
  std::stringstream stream(
      "Material \"metal\" "
      "\"texture eta\" \"a\" "
      "\"texture k\" \"b\" "
      "\"float roughness\" 1.0 "
      "\"float uroughness\" 2.0 "
      "\"float vroughness\" 3.0 "
      "\"bool remaproughness\" \"false\" "
      "\"float bumpmap\" 4.0 ");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          material {
                                            metal {
                                              eta { spectrum_texture_name: "a" }
                                              k { spectrum_texture_name: "b" }
                                              roughness { float_value: 1.0 }
                                              uroughness { float_value: 2.0 }
                                              vroughness { float_value: 3.0 }
                                              remaproughness: false
                                              bumpmap { float_value: 4.0 }
                                            }
                                          }
                                        })pb")));
}

TEST(Convert, MaterialMirror) {
  std::stringstream stream(
      "Material \"mirror\" \"texture Kr\" \"a\" \"float bumpmap\" 1.0");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(R"pb(directives {
                                      material {
                                        mirror {
                                          Kr { spectrum_texture_name: "a" }
                                          bumpmap { float_value: 1.0 }
                                        }
                                      }
                                    })pb")));
}

TEST(Convert, MaterialMix) {
  std::stringstream stream(
      "Material \"mix\" "
      "\"texture amount\" \"a\" "
      "\"texture Kd\" \"b\" "
      "\"float bumpmap\" 1.0 "
      "\"float sigma\" 2.0 "
      "\"string namedmaterial1\" \"a\" "
      "\"string namedmaterial2\" \"b\" ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  R"pb(directives {
                         material {
                           mix {
                             bumpmap { float_value: 1.0 }
                             sigma { float_value: 2.0 }
                             amount { spectrum_texture_name: "a" }
                             Kd { spectrum_texture_name: "b" }
                             namedmaterial1: "a"
                             namedmaterial2: "b"
                           }
                         }
                       })pb")));
}

TEST(Convert, MaterialPlastic) {
  std::stringstream stream(
      "Material \"plastic\" "
      "\"texture Kd\" \"a\" "
      "\"texture Ks\" \"b\" "
      "\"float roughness\" 1.0 "
      "\"bool remaproughness\" \"false\" "
      "\"float bumpmap\" 2.0 ");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          material {
                                            plastic {
                                              Kd { spectrum_texture_name: "a" }
                                              Ks { spectrum_texture_name: "b" }
                                              roughness { float_value: 1.0 }
                                              remaproughness: false
                                              bumpmap { float_value: 2.0 }
                                            }
                                          }
                                        })pb")));
}

TEST(Convert, MaterialSubstrate) {
  std::stringstream stream(
      "Material \"substrate\" "
      "\"texture Kd\" \"a\" "
      "\"texture Ks\" \"b\" "
      "\"float uroughness\" 1.0 "
      "\"float vroughness\" 2.0 "
      "\"bool remaproughness\" \"false\" "
      "\"float bumpmap\" 3.0 ");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          material {
                                            substrate {
                                              Kd { spectrum_texture_name: "a" }
                                              Ks { spectrum_texture_name: "b" }
                                              uroughness { float_value: 1.0 }
                                              vroughness { float_value: 2.0 }
                                              remaproughness: false
                                              bumpmap { float_value: 3.0 }
                                            }
                                          }
                                        })pb")));
}

TEST(Convert, MaterialSubsurface) {
  std::stringstream stream(
      "Material \"subsurface\" "
      "\"string name\" \"Coke\" "
      "\"texture sigma_a\" \"a\" "
      "\"texture sigma_s\" \"b\" "
      "\"float g\" 1.0 "
      "\"float scale\" 2.0 "
      "\"float eta\" 3.0 "
      "\"texture Kr\" \"c\" "
      "\"texture Kt\" \"d\" "
      "\"float uroughness\" 4.0 "
      "\"float vroughness\" 5.0 "
      "\"bool remaproughness\" \"false\" "
      "\"float bumpmap\" 6.0 ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  R"pb(directives {
                         material {
                           subsurface {
                             name: COKE
                             sigma_a { spectrum_texture_name: "a" }
                             sigma_s { spectrum_texture_name: "b" }
                             g: 1.0
                             scale: 2.0
                             eta: 3.0
                             Kr { spectrum_texture_name: "c" }
                             Kt { spectrum_texture_name: "d" }
                             uroughness { float_value: 4.0 }
                             vroughness { float_value: 5.0 }
                             remaproughness: false
                             bumpmap { float_value: 6.0 }
                           }
                         }
                       })pb")));
}

TEST(Convert, MaterialTranslucent) {
  std::stringstream stream(
      "Material \"translucent\" "
      "\"texture Kd\" \"a\" "
      "\"texture Ks\" \"b\" "
      "\"texture reflect\" \"c\" "
      "\"texture transmit\" \"d\" "
      "\"float roughness\" 1.0 "
      "\"bool remaproughness\" \"false\" "
      "\"float bumpmap\" 2.0 ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  R"pb(directives {
                         material {
                           translucent {
                             Kd { spectrum_texture_name: "a" }
                             Ks { spectrum_texture_name: "b" }
                             reflect { spectrum_texture_name: "c" }
                             transmit { spectrum_texture_name: "d" }
                             roughness { float_value: 1.0 }
                             remaproughness: false
                             bumpmap { float_value: 2.0 }
                           }
                         }
                       })pb")));
}

TEST(Convert, MaterialUber) {
  std::stringstream stream(
      "Material \"uber\" "
      "\"texture Kd\" \"a\" "
      "\"texture Ks\" \"b\" "
      "\"texture Kr\" \"c\" "
      "\"texture Kt\" \"d\" "
      "\"texture opacity\" \"e\" "
      "\"float roughness\" 1.0 "
      "\"float uroughness\" 2.0 "
      "\"float vroughness\" 3.0 "
      "\"float eta\" 4.0 "
      "\"bool remaproughness\" \"false\" "
      "\"float bumpmap\" 6.0 ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  R"pb(directives {
                         material {
                           uber {
                             Kd { spectrum_texture_name: "a" }
                             Ks { spectrum_texture_name: "b" }
                             Kr { spectrum_texture_name: "c" }
                             Kt { spectrum_texture_name: "d" }
                             opacity { spectrum_texture_name: "e" }
                             roughness { float_value: 1.0 }
                             uroughness { float_value: 2.0 }
                             vroughness { float_value: 3.0 }
                             eta { float_value: 4.0 }
                             remaproughness: false
                             bumpmap { float_value: 6.0 }
                           }
                         }
                       })pb")));
}

TEST(Convert, MaterialUberIndex) {
  std::stringstream stream(
      "Material \"uber\" "
      "\"texture Kd\" \"a\" "
      "\"texture Ks\" \"b\" "
      "\"texture Kr\" \"c\" "
      "\"texture Kt\" \"d\" "
      "\"texture opacity\" \"e\" "
      "\"float roughness\" 1.0 "
      "\"float uroughness\" 2.0 "
      "\"float vroughness\" 3.0 "
      "\"float index\" 4.0 "
      "\"bool remaproughness\" \"false\" "
      "\"float bumpmap\" 6.0 ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  R"pb(directives {
                         material {
                           uber {
                             Kd { spectrum_texture_name: "a" }
                             Ks { spectrum_texture_name: "b" }
                             Kr { spectrum_texture_name: "c" }
                             Kt { spectrum_texture_name: "d" }
                             opacity { spectrum_texture_name: "e" }
                             roughness { float_value: 1.0 }
                             uroughness { float_value: 2.0 }
                             vroughness { float_value: 3.0 }
                             eta { float_value: 4.0 }
                             remaproughness: false
                             bumpmap { float_value: 6.0 }
                           }
                         }
                       })pb")));
}

TEST(Convert, MediumInterface) {
  std::stringstream stream("MediumInterface \"a\" \"b\"");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  R"pb(directives {
                         medium_interface { inside: "a" outside: "b" }
                       })pb")));
}

TEST(Convert, NamedMaterial) {
  std::stringstream stream("NamedMaterial \"a\"");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives {
                                              named_material { name: "a" }
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

TEST(Convert, PixelFilterUnknown) {
  std::stringstream stream("PixelFilter \"unknown\"");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(R"pb(directives {
                                                               pixel_filter {}
                                                             })pb")));
}

TEST(Convert, PixelFilterBox) {
  std::stringstream stream(
      "PixelFilter \"box\" \"float xwidth\" 1.0 \"float ywidth\" 2.0");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  R"pb(directives {
                         pixel_filter { box { xwidth: 1.0 ywidth: 2.0 } }
                       })pb")));
}

TEST(Convert, PixelFilterGaussian) {
  std::stringstream stream(
      "PixelFilter \"gaussian\" \"float xwidth\" 1.0 \"float ywidth\" 2.0 "
      "\"float "
      "alpha\" 3.0");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  R"pb(directives {
                         pixel_filter {
                           gaussian { xwidth: 1.0 ywidth: 2.0 alpha: 3.0 }
                         }
                       })pb")));
}

TEST(Convert, PixelFilterMitchell) {
  std::stringstream stream(
      "PixelFilter \"mitchell\" \"float xwidth\" 1.0 \"float ywidth\" 2.0 "
      "\"float "
      "B\" 3.0 \"float C\" 4.0");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  R"pb(directives {
                         pixel_filter {
                           mitchell { xwidth: 1.0 ywidth: 2.0 B: 3.0 C: 4.0 }
                         }
                       })pb")));
}

TEST(Convert, PixelFilterSinc) {
  std::stringstream stream(
      "PixelFilter \"sinc\" \"float xwidth\" 1.0 \"float ywidth\" 2.0 \"float "
      "tau\" "
      "3.0");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(
          R"pb(directives {
                 pixel_filter { sinc { xwidth: 1.0 ywidth: 2.0 tau: 3.0 } }
               })pb")));
}

TEST(Convert, PixelFilterTriangle) {
  std::stringstream stream(
      "PixelFilter \"triangle\" \"float xwidth\" 1.0 \"float ywidth\" 3.0");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  R"pb(directives {
                         pixel_filter { triangle { xwidth: 1.0 ywidth: 3.0 } }
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

TEST(Convert, Scale) {
  std::stringstream stream("Scale 1 2 3");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives {
                                              scale { x: 1 y: 2 z: 3 }
                                            })pb")));
}

TEST(Convert, ShapeUnknown) {
  std::stringstream stream("Shape \"unknown\"");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives { shape {} })pb")));
}

TEST(Convert, ShapeCone) {
  std::stringstream stream(
      "Shape \"cone\" "
      "\"float radius\" 2.0 "
      "\"float height\" 3.0 "
      "\"float phimax\" 4.0");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  R"pb(directives {
                         shape { cone { radius: 2.0 height: 3.0 phimax: 4.0 } }
                       })pb")));
}

TEST(Convert, ShapeCurve) {
  std::stringstream stream(
      "Shape \"curve\" "
      "\"point P\" [1 2 3] "
      "\"string basis\" \"bspline\" "
      "\"integer degree\" 3 "
      "\"string type\" \"ribbon\" "
      "\"normal N\" [5 6 7] "
      "\"float width\" 8.0 "
      "\"float width0\" 9.0 "
      "\"float width1\" 10.0 "
      "\"integer splitdepth\" 11 ");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          shape {
                                            curve {
                                              P { x: 1.0 y: 2.0 z: 3.0 }
                                              basis: BSPLINE
                                              degree: THREE
                                              type: RIBBON
                                              N { x: 5.0 y: 6.0 z: 7.0 }
                                              width: 8.0
                                              width0: 9.0
                                              width1: 10.0
                                              splitdepth: 11
                                            }
                                          }
                                        })pb")));
}

TEST(Convert, ShapeCurveBadBasis) {
  std::stringstream stream(
      "Shape \"curve\" "
      "\"string basis\" \"bad\" ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives { shape {} })pb")));
}

TEST(Convert, ShapeCurveBezier) {
  std::stringstream stream(
      "Shape \"curve\" "
      "\"string basis\" \"bezier\" ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives {
                                              shape { curve { basis: BEZIER } }
                                            })pb")));
}

TEST(Convert, ShapeCurveCylinder) {
  std::stringstream stream(
      "Shape \"curve\" "
      "\"string type\" \"cylinder\" ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives {
                                              shape { curve { type: CYLINDER } }
                                            })pb")));
}

TEST(Convert, ShapeCurveFlat) {
  std::stringstream stream(
      "Shape \"curve\" "
      "\"string type\" \"flat\" ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives {
                                              shape { curve { type: FLAT } }
                                            })pb")));
}

TEST(Convert, ShapeCurveRibbon) {
  std::stringstream stream(
      "Shape \"curve\" "
      "\"string type\" \"ribbon\" ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives {
                                              shape { curve { type: RIBBON } }
                                            })pb")));
}

TEST(Convert, ShapeCurveUnknownType) {
  std::stringstream stream(
      "Shape \"curve\" "
      "\"string type\" \"abc\" ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives {
                                              shape { curve { type: CYLINDER } }
                                            })pb")));
}

TEST(Convert, ShapeCurveDegreeThree) {
  std::stringstream stream(
      "Shape \"curve\" "
      "\"integer degree\" 3 ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives {
                                              shape { curve { degree: THREE } }
                                            })pb")));
}

TEST(Convert, ShapeCurveDegreeFour) {
  std::stringstream stream(
      "Shape \"curve\" "
      "\"integer degree\" 4 ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives {
                                              shape { curve { degree: FOUR } }
                                            })pb")));
}

TEST(Convert, ShapeCurveDegreeBad) {
  std::stringstream stream(
      "Shape \"curve\" "
      "\"integer degree\" 2 ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives { shape {} })pb")));
}

TEST(Convert, ShapeCylinder) {
  std::stringstream stream(
      "Shape \"cylinder\" "
      "\"float radius\" 2.0 "
      "\"float zmin\" 3.0 "
      "\"float zmax\" 4.0 "
      "\"float phimax\" 5.0");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(
          R"pb(directives {
                 shape {
                   cylinder { radius: 2.0 zmin: 3.0 zmax: 4.0 phimax: 5.0 }
                 }
               })pb")));
}

TEST(Convert, ShapeDisk) {
  std::stringstream stream(
      "Shape \"disk\" "
      "\"float height\" 1.0 "
      "\"float radius\" 2.0 "
      "\"float innerradius\" 3.0 "
      "\"float phimax\" 4.0");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(
          R"pb(directives {
                 shape {
                   disk { height: 1.0 radius: 2.0 innerradius: 3.0 phimax: 4.0 }
                 }
               })pb")));
}

TEST(Convert, ShapeHeightfield) {
  std::stringstream stream(
      "Shape \"heightfield\" "
      "\"integer nu\" 1 "
      "\"integer nv\" 2 "
      "\"float Pz\" [3.0 4.0] ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  R"pb(directives {
                         shape { heightfield { nu: 1 nv: 2 Pz: 3.0 Pz: 4.0 } }
                       })pb")));
}

TEST(Convert, ShapeHeightfieldNoNu) {
  std::stringstream stream("Shape \"heightfield\" ");
  EXPECT_THAT(Convert(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Missing required heightfield Shape parameter: 'nu'"));
}

TEST(Convert, ShapeHeightfieldBadNu) {
  std::stringstream stream(
      "Shape \"heightfield\" "
      "\"integer nu\" -1 "
      "\"integer nv\" 2 ");
  EXPECT_THAT(Convert(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Missing required heightfield Shape parameter: 'nu'"));
}

TEST(Convert, ShapeHeightfieldNoNv) {
  std::stringstream stream(
      "Shape \"heightfield\" "
      "\"integer nu\" 1 ");
  EXPECT_THAT(Convert(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Missing required heightfield Shape parameter: 'nv'"));
}

TEST(Convert, ShapeHeightfieldBadNv) {
  std::stringstream stream(
      "Shape \"heightfield\" "
      "\"integer nu\" 1 "
      "\"integer nv\" -2 ");
  EXPECT_THAT(Convert(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Missing required heightfield Shape parameter: 'nv'"));
}

TEST(Convert, ShapeHeightfieldTooLarge) {
  std::stringstream stream(
      "Shape \"heightfield\" "
      "\"integer nu\" 65535 "
      "\"integer nv\" 65535 ");
  EXPECT_THAT(
      Convert(stream),
      StatusIs(
          absl::StatusCode::kInvalidArgument,
          "Heighfield shape is too large to be stored in a 1D proto array"));
}

TEST(Convert, ShapeHeightfieldNoPz) {
  std::stringstream stream(
      "Shape \"heightfield\" "
      "\"integer nu\" 1 "
      "\"integer nv\" 2 ");
  EXPECT_THAT(Convert(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Missing or invalid heightfield Shape parameter: 'Pz'"));
}

TEST(Convert, ShapeHeightfieldNWrongSizePz) {
  std::stringstream stream(
      "Shape \"heightfield\" "
      "\"integer nu\" 1 "
      "\"integer nv\" 2 "
      "\"float Pz\" [3.0] ");
  EXPECT_THAT(Convert(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Missing or invalid heightfield Shape parameter: 'Pz'"));
}

TEST(Convert, ShapeHyperboloid) {
  std::stringstream stream(
      "Shape \"hyperboloid\" "
      "\"point p1\" [1.0 2.0 3.0] "
      "\"point p2\" [4.0 5.0 6.0] "
      "\"float phimax\" 7.0");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          shape {
                                            hyperboloid {
                                              p1 { x: 1.0 y: 2.0 z: 3.0 }
                                              p2 { x: 4.0 y: 5.0 z: 6.0 }
                                              phimax: 7.0
                                            }
                                          }
                                        })pb")));
}

TEST(Convert, ShapeLoopsubdiv) {
  std::stringstream stream(
      "Shape \"loopsubdiv\" "
      "\"integer indices\" [0 1 2 2 1 0] "
      "\"point P\" [4.0 5.0 6.0 7.0 8.0 9.0 10.0 11.0 12.0] "
      "\"integer levels\" 13 ");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          shape {
                                            loopsubdiv {
                                              indices { v0: 0 v1: 1 v2: 2 }
                                              indices { v0: 2 v1: 1 v2: 0 }
                                              P { x: 4.0 y: 5.0 z: 6.0 }
                                              P { x: 7.0 y: 8.0 z: 9.0 }
                                              P { x: 10.0 y: 11.0 z: 12.0 }
                                              levels: 13
                                            }
                                          }
                                        })pb")));
}

TEST(Convert, ShapeLoopsubdivNLevels) {
  std::stringstream stream(
      "Shape \"loopsubdiv\" "
      "\"integer indices\" [0 1 2 2 1 0] "
      "\"point P\" [4.0 5.0 6.0 7.0 8.0 9.0 10.0 11.0 12.0] "
      "\"integer nlevels\" 13 ");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          shape {
                                            loopsubdiv {
                                              indices { v0: 0 v1: 1 v2: 2 }
                                              indices { v0: 2 v1: 1 v2: 0 }
                                              P { x: 4.0 y: 5.0 z: 6.0 }
                                              P { x: 7.0 y: 8.0 z: 9.0 }
                                              P { x: 10.0 y: 11.0 z: 12.0 }
                                              levels: 13
                                            }
                                          }
                                        })pb")));
}

TEST(Convert, ShapeLoopsubdivBoth) {
  std::stringstream stream(
      "Shape \"loopsubdiv\" "
      "\"integer indices\" [0 1 2 2 1 0] "
      "\"point P\" [4.0 5.0 6.0 7.0 8.0 9.0 10.0 11.0 12.0] "
      "\"integer levels\" 13 "
      "\"integer nlevels\" 12 ");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          shape {
                                            loopsubdiv {
                                              indices { v0: 0 v1: 1 v2: 2 }
                                              indices { v0: 2 v1: 1 v2: 0 }
                                              P { x: 4.0 y: 5.0 z: 6.0 }
                                              P { x: 7.0 y: 8.0 z: 9.0 }
                                              P { x: 10.0 y: 11.0 z: 12.0 }
                                              levels: 13
                                            }
                                          }
                                        })pb")));
}

TEST(Convert, ShapeInvalidIndices0) {
  std::stringstream stream(
      "Shape \"loopsubdiv\" "
      "\"integer indices\" [-1 1 2] "
      "\"point P\" [4.0 5.0 6.0 7.0 8.0 9.0 10.0 11.0 12.0] ");
  EXPECT_THAT(Convert(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Out of bounds loopsubdiv Shape parameter: 'indices'"));
}

TEST(Convert, ShapeInvalidIndices1) {
  std::stringstream stream(
      "Shape \"loopsubdiv\" "
      "\"integer indices\" [0 -1 2] "
      "\"point P\" [4.0 5.0 6.0 7.0 8.0 9.0 10.0 11.0 12.0] ");
  EXPECT_THAT(Convert(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Out of bounds loopsubdiv Shape parameter: 'indices'"));
}

TEST(Convert, ShapeInvalidIndices2) {
  std::stringstream stream(
      "Shape \"loopsubdiv\" "
      "\"integer indices\" [0 1 -2] "
      "\"point P\" [4.0 5.0 6.0 7.0 8.0 9.0 10.0 11.0 12.0] ");
  EXPECT_THAT(Convert(stream),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Out of bounds loopsubdiv Shape parameter: 'indices'"));
}

TEST(Convert, ShapeLoopsubdivNoIndices) {
  std::stringstream stream(
      "Shape \"loopsubdiv\" "
      "\"point P\" [4.0 5.0 6.0 7.0 8.0 9.0 10.0 11.0 12.0] "
      "\"integer levels\" 13 ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives { shape {} })pb")));
}

TEST(Convert, ShapeLoopsubdivNoP) {
  std::stringstream stream(
      "Shape \"loopsubdiv\" "
      "\"integer indices\" [0 1 2 2 1 0] "
      "\"integer levels\" 13 ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives { shape {} })pb")));
}

TEST(Convert, ShapeNurbs) {
  std::stringstream stream(
      "Shape \"nurbs\" "
      "\"integer nu\" 1 "
      "\"integer nv\" 2 "
      "\"integer uorder\" 3 "
      "\"integer vorder\" 4 "
      "\"float uknots\" [5.0 6.0 7.0 8.0] "
      "\"float vknots\" [9.0 10.0 11.0 12.0 13.0 14.0] "
      "\"float v0\" 15.0 "
      "\"float v1\" 16.0 "
      "\"float u0\" 17.0 "
      "\"float u1\" 18.0 "
      "\"point P\" [19.0 20.0 21.0 22.0 23.0 24.0] ");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          shape {
                                            nurbs {
                                              nu: 1
                                              nv: 2
                                              uorder: 3
                                              vorder: 4
                                              uknots: 5.0
                                              uknots: 6.0
                                              uknots: 7.0
                                              uknots: 8.0
                                              vknots: 9.0
                                              vknots: 10.0
                                              vknots: 11.0
                                              vknots: 12.0
                                              vknots: 13.0
                                              vknots: 14.0
                                              v0: 15.0
                                              v1: 16.0
                                              u0: 17.0
                                              u1: 18.0
                                              P { x: 19.0 y: 20.0 z: 21.0 }
                                              P { x: 22.0 y: 23.0 z: 24.0 }
                                            }
                                          }
                                        })pb")));
}

TEST(Convert, ShapeNurbsPw) {
  std::stringstream stream(
      "Shape \"nurbs\" "
      "\"integer nu\" 1 "
      "\"integer nv\" 2 "
      "\"integer uorder\" 3 "
      "\"integer vorder\" 4 "
      "\"float uknots\" [5.0 6.0 7.0 8.0] "
      "\"float vknots\" [9.0 10.0 11.0 12.0 13.0 14.0] "
      "\"float v0\" 15.0 "
      "\"float v1\" 16.0 "
      "\"float u0\" 17.0 "
      "\"float u1\" 18.0 "
      "\"float Pw\" [19.0 20.0 21.0 22.0 23.0 24.0 25.0 26.0] ");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          shape {
                                            nurbs {
                                              nu: 1
                                              nv: 2
                                              uorder: 3
                                              vorder: 4
                                              uknots: 5.0
                                              uknots: 6.0
                                              uknots: 7.0
                                              uknots: 8.0
                                              vknots: 9.0
                                              vknots: 10.0
                                              vknots: 11.0
                                              vknots: 12.0
                                              vknots: 13.0
                                              vknots: 14.0
                                              v0: 15.0
                                              v1: 16.0
                                              u0: 17.0
                                              u1: 18.0
                                              Pw {
                                                p { x: 19.0 y: 20.0 z: 21.0 }
                                                weight: 22.0
                                              }
                                              Pw {
                                                p { x: 23.0 y: 24.0 z: 25.0 }
                                                weight: 26.0
                                              }
                                            }
                                          }
                                        })pb")));
}

TEST(Convert, ShapeNurbsBadNu) {
  std::stringstream stream(
      "Shape \"nurbs\" "
      "\"integer nu\" -1 "
      "\"integer nv\" 2 "
      "\"integer uorder\" 3 "
      "\"integer vorder\" 4 "
      "\"float uknots\" [5.0 6.0 7.0 8.0] "
      "\"float vknots\" [9.0 10.0 11.0 12.0 13.0 14.0] "
      "\"float v0\" 15.0 "
      "\"float v1\" 16.0 "
      "\"float u0\" 17.0 "
      "\"float u1\" 18.0 "
      "\"point P\" [19.0 20.0 21.0 22.0 23.0 24.0] ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives { shape {} })pb")));
}

TEST(Convert, ShapeNurbsNoNu) {
  std::stringstream stream(
      "Shape \"nurbs\" "
      "\"integer nv\" 2 "
      "\"integer uorder\" 3 "
      "\"integer vorder\" 4 "
      "\"float uknots\" [5.0 6.0 7.0 8.0] "
      "\"float vknots\" [9.0 10.0 11.0 12.0 13.0 14.0] "
      "\"float v0\" 15.0 "
      "\"float v1\" 16.0 "
      "\"float u0\" 17.0 "
      "\"float u1\" 18.0 "
      "\"point P\" [19.0 20.0 21.0 22.0 23.0 24.0] ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives { shape {} })pb")));
}

TEST(Convert, ShapeNurbsBadNv) {
  std::stringstream stream(
      "Shape \"nurbs\" "
      "\"integer nu\" 1 "
      "\"integer nv\" -2 "
      "\"integer uorder\" 3 "
      "\"integer vorder\" 4 "
      "\"float uknots\" [5.0 6.0 7.0 8.0] "
      "\"float vknots\" [9.0 10.0 11.0 12.0 13.0 14.0] "
      "\"float v0\" 15.0 "
      "\"float v1\" 16.0 "
      "\"float u0\" 17.0 "
      "\"float u1\" 18.0 "
      "\"point P\" [19.0 20.0 21.0 22.0 23.0 24.0] ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives { shape {} })pb")));
}

TEST(Convert, ShapeNurbsNoNv) {
  std::stringstream stream(
      "Shape \"nurbs\" "
      "\"integer nu\" 1 "
      "\"integer uorder\" 3 "
      "\"integer vorder\" 4 "
      "\"float uknots\" [5.0 6.0 7.0 8.0] "
      "\"float vknots\" [9.0 10.0 11.0 12.0 13.0 14.0] "
      "\"float v0\" 15.0 "
      "\"float v1\" 16.0 "
      "\"float u0\" 17.0 "
      "\"float u1\" 18.0 "
      "\"point P\" [19.0 20.0 21.0 22.0 23.0 24.0] ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives { shape {} })pb")));
}

TEST(Convert, ShapeNurbsBadUorder) {
  std::stringstream stream(
      "Shape \"nurbs\" "
      "\"integer nu\" -1 "
      "\"integer nv\" 2 "
      "\"integer uorder\" -3 "
      "\"integer vorder\" 4 "
      "\"float uknots\" [5.0 6.0 7.0 8.0] "
      "\"float vknots\" [9.0 10.0 11.0 12.0 13.0 14.0] "
      "\"float v0\" 15.0 "
      "\"float v1\" 16.0 "
      "\"float u0\" 17.0 "
      "\"float u1\" 18.0 "
      "\"point P\" [19.0 20.0 21.0 22.0 23.0 24.0] ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives { shape {} })pb")));
}

TEST(Convert, ShapeNurbsNoUorder) {
  std::stringstream stream(
      "Shape \"nurbs\" "
      "\"integer nu\" 1 "
      "\"integer nv\" 2 "
      "\"integer vorder\" 4 "
      "\"float uknots\" [5.0 6.0 7.0 8.0] "
      "\"float vknots\" [9.0 10.0 11.0 12.0 13.0 14.0] "
      "\"float v0\" 15.0 "
      "\"float v1\" 16.0 "
      "\"float u0\" 17.0 "
      "\"float u1\" 18.0 "
      "\"point P\" [19.0 20.0 21.0 22.0 23.0 24.0] ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives { shape {} })pb")));
}

TEST(Convert, ShapeNurbsBadVorder) {
  std::stringstream stream(
      "Shape \"nurbs\" "
      "\"integer nu\" 1 "
      "\"integer nv\" 2 "
      "\"integer uorder\" 3 "
      "\"integer vorder\" -4 "
      "\"float uknots\" [5.0 6.0 7.0 8.0] "
      "\"float vknots\" [9.0 10.0 11.0 12.0 13.0 14.0] "
      "\"float v0\" 15.0 "
      "\"float v1\" 16.0 "
      "\"float u0\" 17.0 "
      "\"float u1\" 18.0 "
      "\"point P\" [19.0 20.0 21.0 22.0 23.0 24.0] ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives { shape {} })pb")));
}

TEST(Convert, ShapeNurbsNoVorder) {
  std::stringstream stream(
      "Shape \"nurbs\" "
      "\"integer nu\" 1 "
      "\"integer nv\" 2 "
      "\"integer uorder\" 3 "
      "\"float uknots\" [5.0 6.0 7.0 8.0] "
      "\"float vknots\" [9.0 10.0 11.0 12.0 13.0 14.0] "
      "\"float v0\" 15.0 "
      "\"float v1\" 16.0 "
      "\"float u0\" 17.0 "
      "\"float u1\" 18.0 "
      "\"point P\" [19.0 20.0 21.0 22.0 23.0 24.0] ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives { shape {} })pb")));
}

TEST(Convert, ShapeNurbsNoUKnots) {
  std::stringstream stream(
      "Shape \"nurbs\" "
      "\"integer nu\" 1 "
      "\"integer nv\" 2 "
      "\"integer uorder\" 3 "
      "\"integer vorder\" 4 "
      "\"float vknots\" [9.0 10.0 11.0 12.0 13.0 14.0] "
      "\"float v0\" 15.0 "
      "\"float v1\" 16.0 "
      "\"float u0\" 17.0 "
      "\"float u1\" 18.0 "
      "\"point P\" [19.0 20.0 21.0 22.0 23.0 24.0] ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives { shape {} })pb")));
}

TEST(Convert, ShapeNurbsBadUKnots) {
  std::stringstream stream(
      "Shape \"nurbs\" "
      "\"integer nu\" 1 "
      "\"integer nv\" 2 "
      "\"integer uorder\" 3 "
      "\"integer vorder\" 4 "
      "\"float uknots\" [5.0 6.0 7.0] "
      "\"float vknots\" [9.0 10.0 11.0 12.0 13.0 14.0] "
      "\"float v0\" 15.0 "
      "\"float v1\" 16.0 "
      "\"float u0\" 17.0 "
      "\"float u1\" 18.0 "
      "\"point P\" [19.0 20.0 21.0 22.0 23.0 24.0] ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives { shape {} })pb")));
}

TEST(Convert, ShapeNurbsNoVKnots) {
  std::stringstream stream(
      "Shape \"nurbs\" "
      "\"integer nu\" 1 "
      "\"integer nv\" 2 "
      "\"integer uorder\" 3 "
      "\"integer vorder\" 4 "
      "\"float uknots\" [5.0 6.0 7.0 8.0] "
      "\"float v0\" 15.0 "
      "\"float v1\" 16.0 "
      "\"float u0\" 17.0 "
      "\"float u1\" 18.0 "
      "\"point P\" [19.0 20.0 21.0 22.0 23.0 24.0] ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives { shape {} })pb")));
}

TEST(Convert, ShapeNurbsNoBadVKnots) {
  std::stringstream stream(
      "Shape \"nurbs\" "
      "\"integer nu\" 1 "
      "\"integer nv\" 2 "
      "\"integer uorder\" 3 "
      "\"integer vorder\" 4 "
      "\"float uknots\" [5.0 6.0 7.0 8.0] "
      "\"float vknots\" [9.0 10.0 11.0 12.0 13.0] "
      "\"float v0\" 15.0 "
      "\"float v1\" 16.0 "
      "\"float u0\" 17.0 "
      "\"float u1\" 18.0 "
      "\"point P\" [19.0 20.0 21.0 22.0 23.0 24.0] ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives { shape {} })pb")));
}

TEST(Convert, ShapeNurbsNoP) {
  std::stringstream stream(
      "Shape \"nurbs\" "
      "\"integer nu\" 1 "
      "\"integer nv\" 2 "
      "\"integer uorder\" 3 "
      "\"integer vorder\" 4 "
      "\"float uknots\" [5.0 6.0 7.0 8.0] "
      "\"float vknots\" [9.0 10.0 11.0 12.0 13.0 14.0] "
      "\"float v0\" 15.0 "
      "\"float v1\" 16.0 "
      "\"float u0\" 17.0 "
      "\"float u1\" 18.0 ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives { shape {} })pb")));
}

TEST(Convert, ShapeNurbsBadP) {
  std::stringstream stream(
      "Shape \"nurbs\" "
      "\"integer nu\" 1 "
      "\"integer nv\" 2 "
      "\"integer uorder\" 3 "
      "\"integer vorder\" 4 "
      "\"float uknots\" [5.0 6.0 7.0 8.0] "
      "\"float vknots\" [9.0 10.0 11.0 12.0 13.0 14.0] "
      "\"float v0\" 15.0 "
      "\"float v1\" 16.0 "
      "\"float u0\" 17.0 "
      "\"float u1\" 18.0 "
      "\"point P\" [19.0 20.0 21.0] ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives { shape {} })pb")));
}

TEST(Convert, ShapeNurbsBadPw) {
  std::stringstream stream(
      "Shape \"nurbs\" "
      "\"integer nu\" 1 "
      "\"integer nv\" 2 "
      "\"integer uorder\" 3 "
      "\"integer vorder\" 4 "
      "\"float uknots\" [5.0 6.0 7.0 8.0] "
      "\"float vknots\" [9.0 10.0 11.0 12.0 13.0 14.0] "
      "\"float v0\" 15.0 "
      "\"float v1\" 16.0 "
      "\"float u0\" 17.0 "
      "\"float u1\" 18.0 "
      "\"float Pw\" [19.0 20.0 21.0] ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives { shape {} })pb")));
}

TEST(Convert, ShapeParaboloid) {
  std::stringstream stream(
      "Shape \"paraboloid\" "
      "\"float radius\" 1.0 "
      "\"float zmin\" 2.0 "
      "\"float zmax\" 3.0 "
      "\"float phimax\" 4.0");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(
          R"pb(directives {
                 shape {
                   paraboloid { radius: 1.0 zmin: 2.0 zmax: 3.0 phimax: 4.0 }
                 }
               })pb")));
}

TEST(Convert, ShapePlymesh) {
  std::stringstream stream(
      "Shape \"plymesh\" "
      "\"string filename\" \"1.0\" "
      "\"float alpha\" 2.0 "
      "\"float shadowalpha\" 3.0 ");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          shape {
                                            plymesh {
                                              filename: "1.0"
                                              alpha { float_value: 2.0 }
                                              shadowalpha { float_value: 3.0 }
                                            }
                                          }
                                        })pb")));
}

TEST(Convert, ShapeSphere) {
  std::stringstream stream(
      "Shape \"sphere\" "
      "\"float radius\" 1.0 "
      "\"float zmin\" 2.0 "
      "\"float zmax\" 3.0 "
      "\"float phimax\" 4.0");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(
          R"pb(directives {
                 shape {
                   sphere { radius: 1.0 zmin: 2.0 zmax: 3.0 phimax: 4.0 }
                 }
               })pb")));
}

TEST(Convert, ShapeTriangleMesh) {
  std::stringstream stream(
      "Shape \"trianglemesh\" "
      "\"integer indices\" [0 1 2] "
      "\"point3 P\" [3 4 5 6 7 8 9 10 11] "
      "\"normal N\" [12 13 14 15 16 17 18 19 20] "
      "\"vector S\" [21 22 23 24 25 26 27 28 29] "
      "\"point2 uv\" [30 31 32 33 34 35] "
      "\"integer faceIndices\" [36] "
      "\"float alpha\" 37.0 "
      "\"float shadowalpha\" 38.0 ");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          shape {
                                            trianglemesh {
                                              indices { v0: 0 v1: 1 v2: 2 }
                                              P { x: 3 y: 4 z: 5 }
                                              P { x: 6 y: 7 z: 8 }
                                              P { x: 9 y: 10 z: 11 }
                                              N { x: 12 y: 13 z: 14 }
                                              N { x: 15 y: 16 z: 17 }
                                              N { x: 18 y: 19 z: 20 }
                                              S { x: 21 y: 22 z: 23 }
                                              S { x: 24 y: 25 z: 26 }
                                              S { x: 27 y: 28 z: 29 }
                                              uv { u: 30 v: 31 }
                                              uv { u: 32 v: 33 }
                                              uv { u: 34 v: 35 }
                                              faceIndices: 36
                                              alpha { float_value: 37 }
                                              shadowalpha { float_value: 38 }
                                            }
                                          }
                                        })pb")));
}

TEST(Convert, ShapeOverrides) {
  std::stringstream stream(
      "Shape \"sphere\" "
      "\"bool remaproughness\" \"true\" "
      "\"bool thin\" \"true\" "
      "\"float g\" 1.0 "
      "\"float scale\" 2.0 "
      "\"float alpha\" 3.0 "
      "\"float anisotropic\" 5.0 "
      "\"float beta_m\" 6.0 "
      "\"float beta_n\" 7.0 "
      "\"float bumpmap\" 8.0 "
      "\"float clearcoat\" 9.0 "
      "\"float clearcoatgloss\" 10.0 "
      "\"float difftrans\" 10.0 "
      "\"float eumelanin\" 11.0 "
      "\"float flatness\" 12.0 "
      "\"float metallic\" 13.0 "
      "\"float pheomelanin\" 16.0 "
      "\"float roughness\" 17.0 "
      "\"float sheen\" 18.0 "
      "\"float sheentint\" 19.0 "
      "\"float sigma\" 20.0 "
      "\"float spectrans\" 21.0 "
      "\"float speculartint\" 22.0 "
      "\"float uroughness\" 23.0 "
      "\"float vroughness\" 24.0 "
      "\"string name\" \"Apple\" "
      "\"texture amount\" \"a\" "
      "\"texture color\" \"b\" "
      "\"texture k\" \"c\" "
      "\"texture Kd\" \"d\" "
      "\"texture Kr\" \"e\" "
      "\"texture Ks\" \"f\" "
      "\"texture Kt\" \"g\" "
      "\"texture mfp\" \"h\" "
      "\"texture opacity\" \"i\" "
      "\"texture reflect\" \"j\" "
      "\"texture scatterdistance\" \"k\" "
      "\"texture sigma_a\" \"l\" "
      "\"texture sigma_s\" \"m\" "
      "\"texture transmit\" \"n\" "
      "\"string bsdffile\" \"o\" "
      "\"string namedmaterial1\" \"p\" "
      "\"string namedmaterial2\" \"q\" "
      "\"float eta\" 25.0 ");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  R"pb(directives {
                         shape {
                           sphere {}
                           overrides {
                             remaproughness: true
                             thin: true
                             g: 1.0
                             scale: 2.0
                             alpha { float_value: 3.0 }
                             anisotropic { float_value: 5.0 }
                             beta_m { float_value: 6.0 }
                             beta_n { float_value: 7.0 }
                             bumpmap { float_value: 8.0 }
                             clearcoat { float_value: 9.0 }
                             clearcoatgloss { float_value: 10.0 }
                             difftrans { float_value: 10.0 }
                             eumelanin { float_value: 11.0 }
                             flatness { float_value: 12.0 }
                             metallic { float_value: 13.0 }
                             pheomelanin { float_value: 16.0 }
                             roughness { float_value: 17.0 }
                             sheen { float_value: 18.0 }
                             sheentint { float_value: 19.0 }
                             sigma { float_value: 20.0 }
                             spectrans { float_value: 21.0 }
                             speculartint { float_value: 22.0 }
                             uroughness { float_value: 23.0 }
                             vroughness { float_value: 24.0 }
                             name: APPLE
                             amount { spectrum_texture_name: "a" }
                             color { spectrum_texture_name: "b" }
                             k { spectrum_texture_name: "c" }
                             Kd { spectrum_texture_name: "d" }
                             Kr { spectrum_texture_name: "e" }
                             Ks { spectrum_texture_name: "f" }
                             Kt { spectrum_texture_name: "g" }
                             mfp { spectrum_texture_name: "h" }
                             opacity { spectrum_texture_name: "i" }
                             reflect { spectrum_texture_name: "j" }
                             scatterdistance { spectrum_texture_name: "k" }
                             sigma_a { spectrum_texture_name: "l" }
                             sigma_s { spectrum_texture_name: "m" }
                             transmit { spectrum_texture_name: "n" }
                             bsdffile: "o"
                             namedmaterial1: "p"
                             namedmaterial2: "q"
                             eta_as_value: 25.0
                             eta_as_float_texture { float_value: 25.0 }
                           }
                         }
                       })pb")));
}

TEST(Convert, ShapeOverridesEtaTexture) {
  std::stringstream stream(
      "Shape \"sphere\" "
      "\"texture eta\" \"a\" ");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(
          R"pb(directives {
                 shape {
                   sphere {}
                   overrides {
                     eta_as_float_texture { float_texture_name: "a" }
                     eta_as_spectrum_texture { spectrum_texture_name: "a" }
                   }
                 }
               })pb")));
}

TEST(Convert, ShapeOverridesEtaSampled) {
  std::stringstream stream(
      "Shape \"sphere\" "
      "\"spectrum eta\" \"a\" ");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(
          R"pb(directives {
                 shape {
                   sphere {}
                   overrides {
                     eta_as_spectrum_texture { sampled_spectrum_filename: "a" }
                   }
                 }
               })pb")));
}

TEST(Convert, SpectrumTextureUnknown) {
  std::stringstream stream("Texture \"name\" \"spectrum\" \"unknown\"");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(R"pb(directives {
                                              spectrum_texture { name: "name" }
                                            })pb")));
}

TEST(Convert, SpectrumTextureBilerp) {
  std::stringstream stream(
      "Texture \"name\" \"spectrum\" \"bilerp\" \"blackbody v00\" [1.0 2.0] "
      "\"blackbody v01\" [3.0 4.0] \"blackbody v10\" [5.0 6.0] \"blackbody "
      "v11\" [7.0 8.0] \"string mapping\" \"uv\" \"float uscale\" 5.0 \"float "
      "vscale\" 6.0 \"float udelta\" 7.0 \"float vdelta\" 8.0 \"vector v1\" "
      "[1.0 2 3.0] \"vector v2\" [4.0 5.0 6.0]");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(
          R"pb(directives {
                 spectrum_texture {
                   name: "name"
                   bilerp {
                     v00 { blackbody_spectrum { temperature: 1.0 scale: 2.0 } }
                     v01 { blackbody_spectrum { temperature: 3.0 scale: 4.0 } }
                     v10 { blackbody_spectrum { temperature: 5.0 scale: 6.0 } }
                     v11 { blackbody_spectrum { temperature: 7.0 scale: 8.0 } }
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

TEST(Convert, SpectrumTextureCheckerboard2DNone) {
  std::stringstream stream(
      "Texture \"name\" \"spectrum\" \"checkerboard\" \"integer dimensions\" 2 "
      "\"blackbody tex1\" [1.0 2.0] \"blackbody tex2\" [3.0 4.0] \"string "
      "aamode\" \"none\" \"string mapping\" \"uv\" \"float uscale\" 5.0 "
      "\"float vscale\" 6.0 \"float udelta\" 7.0 \"float vdelta\" 8.0 \"vector "
      "v1\" [1.0 2 3.0] \"vector v2\" [4.0 5.0 6.0]");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(
          R"pb(directives {
                 spectrum_texture {
                   name: "name"
                   checkerboard2d {
                     tex1 { blackbody_spectrum { temperature: 1.0 scale: 2.0 } }
                     tex2 { blackbody_spectrum { temperature: 3.0 scale: 4.0 } }
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

TEST(Convert, SpectrumTextureCheckerboard2DClosedForm) {
  std::stringstream stream(
      "Texture \"name\" \"spectrum\" \"checkerboard\" \"integer dimensions\" 2 "
      "\"blackbody tex1\" [1.0 2.0] \"blackbody tex2\" [3.0 4.0] \"string "
      "aamode\" \"closedform\" \"string mapping\" \"uv\" \"float uscale\" 5.0 "
      "\"float vscale\" 6.0 \"float udelta\" 7.0 \"float vdelta\" 8.0 \"vector "
      "v1\" [1.0 2 3.0] \"vector v2\" [4.0 5.0 6.0]");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(
          R"pb(directives {
                 spectrum_texture {
                   name: "name"
                   checkerboard2d {
                     tex1 { blackbody_spectrum { temperature: 1.0 scale: 2.0 } }
                     tex2 { blackbody_spectrum { temperature: 3.0 scale: 4.0 } }
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

TEST(Convert, SpectrumTextureCheckerboard3D) {
  std::stringstream stream(
      "Texture \"name\" \"spectrum\" \"checkerboard\" \"integer dimensions\" 3 "
      "\"blackbody tex1\" [1.0 2.0] \"blackbody tex2\" [3.0 4.0]");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(
          R"pb(directives {
                 spectrum_texture {
                   name: "name"
                   checkerboard3d {
                     tex1 { blackbody_spectrum { temperature: 1.0 scale: 2.0 } }
                     tex2 { blackbody_spectrum { temperature: 3.0 scale: 4.0 } }
                   }
                 }
               })pb")));
}

TEST(Convert, SpectrumTextureCheckerboardDots) {
  std::stringstream stream(
      "Texture \"name\" \"spectrum\" \"dots\" \"blackbody inside\" [1.0 2.0] "
      "\"blackbody outside\" [3.0 4.0] \"string mapping\" \"uv\" \"float "
      "uscale\" 5.0 \"float vscale\" 6.0 \"float udelta\" 7.0 \"float vdelta\" "
      "8.0 \"vector v1\" [1.0 2 3.0] \"vector v2\" [4.0 5.0 6.0]");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(
          R"pb(directives {
                 spectrum_texture {
                   name: "name"
                   dots {
                     inside {
                       blackbody_spectrum { temperature: 1.0 scale: 2.0 }
                     }
                     outside {
                       blackbody_spectrum { temperature: 3.0 scale: 4.0 }
                     }
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

TEST(Convert, SpectrumTextureFbm) {
  std::stringstream stream(
      "Texture \"name\" \"spectrum\" \"fbm\" \"integer octaves\" 1 \"float "
      "roughness\" 2.0");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          spectrum_texture {
                                            name: "name"
                                            fbm { octaves: 1 roughness: 2.0 }
                                          }
                                        })pb")));
}

TEST(Convert, SpectrumTextureConstant) {
  std::stringstream stream(
      "Texture \"name\" \"spectrum\" \"constant\" \"blackbody value\" [1.0 "
      "2.0]");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(
          R"pb(directives {
                 spectrum_texture {
                   name: "name"
                   constant {
                     value {
                       blackbody_spectrum { temperature: 1.0 scale: 2.0 }
                     }
                   }
                 }
               })pb")));
}

TEST(Convert, SpectrumTextureMarble) {
  std::stringstream stream(
      "Texture \"name\" \"spectrum\" \"marble\" \"integer octaves\" 1 \"float "
      "roughness\" 2.0 \"float scale\" 3.0 \"float variation\" 4.0");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          spectrum_texture {
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

TEST(Convert, SpectrumTextureImageMapBlack) {
  std::stringstream stream(
      "Texture \"name\" \"spectrum\" \"imagemap\" \"string filename\" \"a\" "
      "\"string wrap\" \"black\" \"float maxanisotropy\" 1.0 \"bool "
      "trilinear\" \"true\" \"float scale\" 2.0 \"bool gamma\" \"true\" "
      "\"string mapping\" \"uv\" \"float uscale\" 5.0 \"float vscale\" 6.0 "
      "\"float udelta\" 7.0 \"float vdelta\" 8.0 \"vector v1\" [1.0 2 3.0] "
      "\"vector v2\" [4.0 5.0 6.0]");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          spectrum_texture {
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

TEST(Convert, SpectrumTextureImageMapClamp) {
  std::stringstream stream(
      "Texture \"name\" \"spectrum\" \"imagemap\" \"string filename\" \"a\" "
      "\"string wrap\" \"clamp\" \"float maxanisotropy\" 1.0 \"bool "
      "trilinear\" \"true\" \"float scale\" 2.0 \"bool gamma\" \"true\" "
      "\"string mapping\" \"uv\" \"float uscale\" 5.0 \"float vscale\" 6.0 "
      "\"float udelta\" 7.0 \"float vdelta\" 8.0 \"vector v1\" [1.0 2 3.0] "
      "\"vector v2\" [4.0 5.0 6.0]");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          spectrum_texture {
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

TEST(Convert, SpectrumTextureImageMapRepeat) {
  std::stringstream stream(
      "Texture \"name\" \"spectrum\" \"imagemap\" \"string filename\" \"a\" "
      "\"string wrap\" \"repeat\" \"float maxanisotropy\" 1.0 \"bool "
      "trilinear\" \"true\" \"float scale\" 2.0 \"bool gamma\" \"true\" "
      "\"string mapping\" \"uv\" \"float uscale\" 5.0 \"float vscale\" 6.0 "
      "\"float udelta\" 7.0 \"float vdelta\" 8.0 \"vector v1\" [1.0 2 3.0] "
      "\"vector v2\" [4.0 5.0 6.0]");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          spectrum_texture {
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

TEST(Convert, SpectrumTextureMix) {
  std::stringstream stream(
      "Texture \"name\" \"spectrum\" \"mix\" \"blackbody tex1\" [1.0 2.0] "
      "\"blackbody tex2\" [3.0 4.0] \"float amount\" 5.0");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(
          R"pb(directives {
                 spectrum_texture {
                   name: "name"
                   mix {
                     tex1 { blackbody_spectrum { temperature: 1.0 scale: 2.0 } }
                     tex2 { blackbody_spectrum { temperature: 3.0 scale: 4.0 } }
                     amount { float_value: 5.0 }
                   }
                 }
               })pb")));
}

TEST(Convert, SpectrumTexturePtex) {
  std::stringstream stream(
      "Texture \"name\" \"spectrum\" \"ptex\" \"string filename\" \"a\" "
      "\"float gamma\" 2.0");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          spectrum_texture {
                                            name: "name"
                                            ptex { filename: "a" gamma: 2.0 }
                                          }
                                        })pb")));
}

TEST(Convert, SpectrumTextureScale) {
  std::stringstream stream(
      "Texture \"name\" \"spectrum\" \"scale\" \"blackbody tex1\" [1.0 2.0] "
      "\"blackbody tex2\" [3.0 4.0]");
  EXPECT_THAT(
      Convert(stream),
      IsOkAndHolds(EqualsProto(
          R"pb(directives {
                 spectrum_texture {
                   name: "name"
                   scale {
                     tex1 { blackbody_spectrum { temperature: 1.0 scale: 2.0 } }
                     tex2 { blackbody_spectrum { temperature: 3.0 scale: 4.0 } }
                   }
                 }
               })pb")));
}

TEST(Convert, SpectrumTextureWindy) {
  std::stringstream stream("Texture \"name\" \"spectrum\" \"windy\"");
  EXPECT_THAT(Convert(stream), IsOkAndHolds(EqualsProto(
                                   R"pb(directives {
                                          spectrum_texture {
                                            name: "name"
                                            windy {}
                                          }
                                        })pb")));
}

TEST(Convert, SpectrumTextureWrinkled) {
  std::stringstream stream(
      "Texture \"name\" \"spectrum\" \"wrinkled\" \"integer octaves\" 1 "
      "\"float roughness\" 2.0");
  EXPECT_THAT(Convert(stream),
              IsOkAndHolds(EqualsProto(
                  R"pb(directives {
                         spectrum_texture {
                           name: "name"
                           wrinkled { octaves: 1 roughness: 2.0 }
                         }
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
