#include "pbrt_proto/v3/defaults.h"

#include "absl/status/status_matchers.h"
#include "absl/strings/string_view.h"
#include "googlemock/include/gmock/gmock.h"
#include "googletest/include/gtest/gtest.h"
#include "pbrt_proto/testing/proto_matchers.h"
#include "pbrt_proto/v3/pbrt.pb.h"

namespace pbrt_proto::v3 {
namespace {

using ::absl_testing::IsOkAndHolds;
using ::absl_testing::StatusIs;
using ::google::protobuf::EqualsProto;
using ::google::protobuf::ParseTextOrDie;

PbrtProto MakeCanonical(absl::string_view textproto) {
  PbrtProto proto = ParseTextOrDie(textproto);
  Canonicalize(proto);
  return proto;
}

// Unset Accelerators are default to using the BVH accelerator
// https://github.com/mmp/pbrt-v3/blob/13d871faae88233b327d04cda24022b8bb0093ee/src/core/api.cpp#L1571
// https://github.com/mmp/pbrt-v3/blob/13d871faae88233b327d04cda24022b8bb0093ee/src/core/api.cpp#L1654
TEST(Canonicalize, Accelerator) {
  EXPECT_THAT(MakeCanonical(R"pb(directives { accelerator {} })pb"),
              EqualsProto(R"pb(directives { accelerator { bvh {} } })pb"));
}

// Unset AreaLightSource are left unset
TEST(Canonicalize, AreaLightSource) {
  EXPECT_THAT(MakeCanonical(R"pb(directives { area_light_source {} })pb"),
              EqualsProto(R"pb(directives { area_light_source {} })pb"));
}

TEST(Canonicalize, AreaLightSourceDiffuse) {
  EXPECT_THAT(
      MakeCanonical(R"pb(directives { area_light_source { diffuse {} } })pb"),
      EqualsProto(
          R"pb(directives {
                 area_light_source {
                   diffuse {
                     L { rgb_spectrum { r: 1.0 g: 1.0 b: 1.0 } }
                     scale { rgb_spectrum { r: 1.0 g: 1.0 b: 1.0 } }
                   }
                 }
               })pb"));
}

// Unset Film are left unset and should eventually cause rendering to fail
// https://github.com/mmp/pbrt-v3/blob/13d871faae88233b327d04cda24022b8bb0093ee/src/core/api.cpp#L1664
TEST(Canonicalize, Camera) {
  EXPECT_THAT(MakeCanonical(R"pb(directives { camera {} })pb"),
              EqualsProto(R"pb(directives { camera {} })pb"));
}

// Unset Film are left unset and should eventually cause rendering to fail
// https://github.com/mmp/pbrt-v3/blob/13d871faae88233b327d04cda24022b8bb0093ee/src/core/api.cpp#L1720
TEST(Canonicalize, Film) {
  EXPECT_THAT(MakeCanonical(R"pb(directives { film {} })pb"),
              EqualsProto(R"pb(directives { film {} })pb"));
}

// Unset Filter are left unset and should eventually cause rendering to fail
// https://github.com/mmp/pbrt-v3/blob/13d871faae88233b327d04cda24022b8bb0093ee/src/core/api.cpp#L852
TEST(Canonicalize, Filter) {
  EXPECT_THAT(MakeCanonical(R"pb(directives { filter {} })pb"),
              EqualsProto(R"pb(directives { filter {} })pb"));
}

// Unset FloatTexture are left unset
TEST(Canonicalize, FloatTexture) {
  EXPECT_THAT(MakeCanonical(R"pb(directives { float_texture { name: "" } })pb"),
              EqualsProto(R"pb(directives { float_texture { name: "" } })pb"));
}

TEST(Canonicalize, FloatTextureBilerp) {
  EXPECT_THAT(MakeCanonical(R"pb(directives {
                                   float_texture {
                                     name: ""
                                     bilerp {}
                                   }
                                 })pb"),
              EqualsProto(R"pb(directives {
                                 float_texture {
                                   name: ""
                                   bilerp {
                                     v00 { float_value: 0.0 }
                                     v01 { float_value: 1.0 }
                                     v10 { float_value: 0.0 }
                                     v11 { float_value: 1.0 }
                                     v1 { x: 1.0 y: 0.0 z: 0.0 }
                                     v2 { x: 0.0 y: 1.0 z: 0.0 }
                                   }
                                 }
                               })pb"));
}

TEST(Canonicalize, FloatTextureCheckerboard2D) {
  EXPECT_THAT(MakeCanonical(R"pb(directives {
                                   float_texture {
                                     name: ""
                                     checkerboard2d {}
                                   }
                                 })pb"),
              EqualsProto(R"pb(directives {
                                 float_texture {
                                   name: ""
                                   checkerboard2d {
                                     tex1 { float_value: 1.0 }
                                     tex2 { float_value: 0.0 }
                                     v1 { x: 1.0 y: 0.0 z: 0.0 }
                                     v2 { x: 0.0 y: 1.0 z: 0.0 }
                                   }
                                 }
                               })pb"));
}

TEST(Canonicalize, FloatTextureCheckerboard3D) {
  EXPECT_THAT(MakeCanonical(R"pb(directives {
                                   float_texture {
                                     name: ""
                                     checkerboard3d {}
                                   }
                                 })pb"),
              EqualsProto(R"pb(directives {
                                 float_texture {
                                   name: ""
                                   checkerboard3d {
                                     tex1 { float_value: 1.0 }
                                     tex2 { float_value: 0.0 }
                                   }
                                 }
                               })pb"));
}

TEST(Canonicalize, FloatTextureConstant) {
  EXPECT_THAT(MakeCanonical(R"pb(directives {
                                   float_texture {
                                     name: ""
                                     constant {}
                                   }
                                 })pb"),
              EqualsProto(R"pb(directives {
                                 float_texture {
                                   name: ""
                                   constant { value { float_value: 1.0 } }
                                 }
                               })pb"));
}

TEST(Canonicalize, FloatTextureDots) {
  EXPECT_THAT(MakeCanonical(R"pb(directives {
                                   float_texture {
                                     name: ""
                                     dots {}
                                   }
                                 })pb"),
              EqualsProto(R"pb(directives {
                                 float_texture {
                                   name: ""
                                   dots {
                                     inside { float_value: 1.0 }
                                     outside { float_value: 0.0 }
                                     v1 { x: 1.0 y: 0.0 z: 0.0 }
                                     v2 { x: 0.0 y: 1.0 z: 0.0 }
                                   }
                                 }
                               })pb"));
}

TEST(Canonicalize, FloatTextureImagemap) {
  EXPECT_THAT(MakeCanonical(R"pb(directives {
                                   float_texture {
                                     name: ""
                                     imagemap {}
                                   }
                                 })pb"),
              EqualsProto(R"pb(directives {
                                 float_texture {
                                   name: ""
                                   imagemap {
                                     v1 { x: 1.0 y: 0.0 z: 0.0 }
                                     v2 { x: 0.0 y: 1.0 z: 0.0 }
                                   }
                                 }
                               })pb"));
}

TEST(Canonicalize, FloatTextureMix) {
  EXPECT_THAT(MakeCanonical(R"pb(directives {
                                   float_texture {
                                     name: ""
                                     mix {}
                                   }
                                 })pb"),
              EqualsProto(R"pb(directives {
                                 float_texture {
                                   name: ""
                                   mix {
                                     tex1 { float_value: 0.0 }
                                     tex2 { float_value: 1.0 }
                                     amount { float_value: 0.5 }
                                   }
                                 }
                               })pb"));
}

TEST(Canonicalize, FloatTextureScale) {
  EXPECT_THAT(MakeCanonical(R"pb(directives {
                                   float_texture {
                                     name: ""
                                     scale {}
                                   }
                                 })pb"),
              EqualsProto(R"pb(directives {
                                 float_texture {
                                   name: ""
                                   scale {
                                     tex1 { float_value: 1.0 }
                                     tex2 { float_value: 1.0 }
                                   }
                                 }
                               })pb"));
}

// Unset Integrator are left unset and should eventually cause a black image to
// be rendered
// https://github.com/mmp/pbrt-v3/blob/13d871faae88233b327d04cda24022b8bb0093ee/src/core/api.cpp#L1619
TEST(Canonicalize, Integrator) {
  EXPECT_THAT(MakeCanonical(R"pb(directives { integrator {} })pb"),
              EqualsProto(R"pb(directives { integrator {} })pb"));
}

// Unset LightSource are left unset
TEST(Canonicalize, LightSource) {
  EXPECT_THAT(MakeCanonical(R"pb(directives { light_source {} })pb"),
              EqualsProto(R"pb(directives { light_source {} })pb"));
}

TEST(Canonicalize, LightSourceDistant) {
  EXPECT_THAT(
      MakeCanonical(R"pb(directives { light_source { distant {} } })pb"),
      EqualsProto(
          R"pb(directives {
                 light_source {
                   distant {
                     L { rgb_spectrum { r: 1.0 g: 1.0 b: 1.0 } }
                     from { x: 0.0 y: 0.0 z: 0.0 }
                     to { x: 0.0 y: 0.0 z: 1.0 }
                     scale { rgb_spectrum { r: 1.0 g: 1.0 b: 1.0 } }
                   }
                 }
               })pb"));
}

TEST(Canonicalize, LightSourceGoniometric) {
  EXPECT_THAT(
      MakeCanonical(R"pb(directives { light_source { goniometric {} } })pb"),
      EqualsProto(
          R"pb(directives {
                 light_source {
                   goniometric {
                     I { rgb_spectrum { r: 1.0 g: 1.0 b: 1.0 } }
                     scale { rgb_spectrum { r: 1.0 g: 1.0 b: 1.0 } }
                   }
                 }
               })pb"));
}

TEST(Canonicalize, LightSourceInfinite) {
  EXPECT_THAT(
      MakeCanonical(R"pb(directives { light_source { infinite {} } })pb"),
      EqualsProto(
          R"pb(directives {
                 light_source {
                   infinite {
                     L { rgb_spectrum { r: 1.0 g: 1.0 b: 1.0 } }
                     scale { rgb_spectrum { r: 1.0 g: 1.0 b: 1.0 } }
                   }
                 }
               })pb"));
}

TEST(Canonicalize, LightSourcePoint) {
  EXPECT_THAT(MakeCanonical(R"pb(directives { light_source { point {} } })pb"),
              EqualsProto(
                  R"pb(directives {
                         light_source {
                           point {
                             L { rgb_spectrum { r: 1.0 g: 1.0 b: 1.0 } }
                             from { x: 0.0 y: 0.0 z: 0.0 }
                             scale { rgb_spectrum { r: 1.0 g: 1.0 b: 1.0 } }
                           }
                         }
                       })pb"));
}

TEST(Canonicalize, LightSourceProjection) {
  EXPECT_THAT(
      MakeCanonical(R"pb(directives { light_source { projection {} } })pb"),
      EqualsProto(
          R"pb(directives {
                 light_source {
                   projection {
                     I { rgb_spectrum { r: 1.0 g: 1.0 b: 1.0 } }
                     scale { rgb_spectrum { r: 1.0 g: 1.0 b: 1.0 } }
                   }
                 }
               })pb"));
}

TEST(Canonicalize, LightSourceSpot) {
  EXPECT_THAT(MakeCanonical(R"pb(directives { light_source { spot {} } })pb"),
              EqualsProto(
                  R"pb(directives {
                         light_source {
                           spot {
                             I { rgb_spectrum { r: 1.0 g: 1.0 b: 1.0 } }
                             from { x: 0.0 y: 0.0 z: 0.0 }
                             to { x: 0.0 y: 0.0 z: 1.0 }
                             scale { rgb_spectrum { r: 1.0 g: 1.0 b: 1.0 } }
                           }
                         }
                       })pb"));
}

// Unset Sampler are left unset and should eventually cause a black image to be
// rendered
// https://github.com/mmp/pbrt-v3/blob/13d871faae88233b327d04cda24022b8bb0093ee/src/core/api.cpp#L1671
TEST(Canonicalize, Sampler) {
  EXPECT_THAT(MakeCanonical(R"pb(directives { sampler {} })pb"),
              EqualsProto(R"pb(directives { sampler {} })pb"));
}

// Unset SpectrumTexture are left unset
TEST(Canonicalize, SpectrumTexture) {
  EXPECT_THAT(
      MakeCanonical(R"pb(directives { spectrum_texture { name: "" } })pb"),
      EqualsProto(R"pb(directives { spectrum_texture { name: "" } })pb"));
}

TEST(Canonicalize, SpectrumTextureBilerp) {
  EXPECT_THAT(MakeCanonical(R"pb(directives {
                                   spectrum_texture {
                                     name: ""
                                     bilerp {}
                                   }
                                 })pb"),
              EqualsProto(R"pb(directives {
                                 spectrum_texture {
                                   name: ""
                                   bilerp {
                                     v00 { uniform_spectrum: 0.0 }
                                     v01 { uniform_spectrum: 1.0 }
                                     v10 { uniform_spectrum: 0.0 }
                                     v11 { uniform_spectrum: 1.0 }
                                     v1 { x: 1.0 y: 0.0 z: 0.0 }
                                     v2 { x: 0.0 y: 1.0 z: 0.0 }
                                   }
                                 }
                               })pb"));
}

TEST(Canonicalize, SpectrumTextureCheckerboard2D) {
  EXPECT_THAT(MakeCanonical(R"pb(directives {
                                   spectrum_texture {
                                     name: ""
                                     checkerboard2d {}
                                   }
                                 })pb"),
              EqualsProto(R"pb(directives {
                                 spectrum_texture {
                                   name: ""
                                   checkerboard2d {
                                     tex1 { uniform_spectrum: 1.0 }
                                     tex2 { uniform_spectrum: 0.0 }
                                     v1 { x: 1.0 y: 0.0 z: 0.0 }
                                     v2 { x: 0.0 y: 1.0 z: 0.0 }
                                   }
                                 }
                               })pb"));
}

TEST(Canonicalize, SpectrumTextureCheckerboard3D) {
  EXPECT_THAT(MakeCanonical(R"pb(directives {
                                   spectrum_texture {
                                     name: ""
                                     checkerboard3d {}
                                   }
                                 })pb"),
              EqualsProto(R"pb(directives {
                                 spectrum_texture {
                                   name: ""
                                   checkerboard3d {
                                     tex1 { uniform_spectrum: 1.0 }
                                     tex2 { uniform_spectrum: 0.0 }
                                   }
                                 }
                               })pb"));
}

TEST(Canonicalize, SpectrumTextureConstant) {
  EXPECT_THAT(MakeCanonical(R"pb(directives {
                                   spectrum_texture {
                                     name: ""
                                     constant {}
                                   }
                                 })pb"),
              EqualsProto(R"pb(directives {
                                 spectrum_texture {
                                   name: ""
                                   constant { value { uniform_spectrum: 1.0 } }
                                 }
                               })pb"));
}

TEST(Canonicalize, SpectrumTextureDots) {
  EXPECT_THAT(MakeCanonical(R"pb(directives {
                                   spectrum_texture {
                                     name: ""
                                     dots {}
                                   }
                                 })pb"),
              EqualsProto(R"pb(directives {
                                 spectrum_texture {
                                   name: ""
                                   dots {
                                     inside { uniform_spectrum: 1.0 }
                                     outside { uniform_spectrum: 0.0 }
                                     v1 { x: 1.0 y: 0.0 z: 0.0 }
                                     v2 { x: 0.0 y: 1.0 z: 0.0 }
                                   }
                                 }
                               })pb"));
}

TEST(Canonicalize, SpectrumTextureImagemap) {
  EXPECT_THAT(MakeCanonical(R"pb(directives {
                                   spectrum_texture {
                                     name: ""
                                     imagemap {}
                                   }
                                 })pb"),
              EqualsProto(R"pb(directives {
                                 spectrum_texture {
                                   name: ""
                                   imagemap {
                                     v1 { x: 1.0 y: 0.0 z: 0.0 }
                                     v2 { x: 0.0 y: 1.0 z: 0.0 }
                                   }
                                 }
                               })pb"));
}

TEST(Canonicalize, SpectrumTextureMix) {
  EXPECT_THAT(MakeCanonical(R"pb(directives {
                                   spectrum_texture {
                                     name: ""
                                     mix {}
                                   }
                                 })pb"),
              EqualsProto(R"pb(directives {
                                 spectrum_texture {
                                   name: ""
                                   mix {
                                     tex1 { uniform_spectrum: 0.0 }
                                     tex2 { uniform_spectrum: 1.0 }
                                     amount { float_value: 0.5 }
                                   }
                                 }
                               })pb"));
}

TEST(Canonicalize, SpectrumTextureScale) {
  EXPECT_THAT(MakeCanonical(R"pb(directives {
                                   spectrum_texture {
                                     name: ""
                                     scale {}
                                   }
                                 })pb"),
              EqualsProto(R"pb(directives {
                                 spectrum_texture {
                                   name: ""
                                   scale {
                                     tex1 { uniform_spectrum: 1.0 }
                                     tex2 { uniform_spectrum: 1.0 }
                                   }
                                 }
                               })pb"));
}

}  // namespace
}  // namespace pbrt_proto::v3