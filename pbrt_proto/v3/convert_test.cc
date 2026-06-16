#include "pbrt_proto/v3/convert.h"

#include <sstream>
#include <string>

#include "absl/status/status.h"
#include "absl/status/status_matchers.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "pbrt_proto/testing/proto_matchers.h"
#include "pbrt_proto/v3/v3.pb.h"

namespace pbrt_proto::v3 {
namespace {

using ::absl_testing::StatusIs;
using ::google::protobuf::EqualsProto;

absl::Status Convert(absl::string_view input, PbrtProto& output) {
  std::string as_string(input);
  std::istringstream as_stream(as_string);
  return Convert(as_stream, output);
}

TEST(Accelerator, Bvh) {
  absl::string_view directive = R"pbrt(Accelerator "bvh")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { accelerator { bvh {} } })pb"));
}

TEST(Accelerator, KdTree) {
  absl::string_view directive = R"pbrt(Accelerator "kdtree")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { accelerator { kdtree {} } })pb"));
}

TEST(AreaLightSource, Area) {
  absl::string_view directive = R"pbrt(AreaLightSource "area")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         area_light_source { diffuse {} }
                                       })pb"));
}

TEST(AreaLightSource, Diffuse) {
  absl::string_view directive = R"pbrt(AreaLightSource "diffuse")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         area_light_source { diffuse {} }
                                       })pb"));
}

TEST(Camera, Environment) {
  absl::string_view directive = R"pbrt(Camera "environment")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { camera { environment {} } })pb"));
}

TEST(Camera, Orthographic) {
  absl::string_view directive = R"pbrt(Camera "orthographic")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { camera { orthographic {} } })pb"));
}

TEST(Camera, Perspective) {
  absl::string_view directive = R"pbrt(Camera "perspective")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { camera { perspective {} } })pb"));
}

TEST(Camera, Realistic) {
  absl::string_view directive = R"pbrt(Camera "realistic")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { camera { realistic {} } })pb"));
}

TEST(Film, Image) {
  absl::string_view directive = R"pbrt(Film "image")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives { film { image {} } })pb"));
}

TEST(FloatTexture, Bilerp) {
  absl::string_view directive = R"pbrt(Texture "name" "float" "bilerp")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         float_texture {
                                           name: "name"
                                           bilerp {}
                                         }
                                       })pb"));
}

TEST(FloatTexture, CheckerboardDefault) {
  absl::string_view directive =
      R"pbrt(Texture "name" "float" "checkerboard")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         float_texture {
                                           name: "name"
                                           checkerboard2d {}
                                         }
                                       })pb"));
}

TEST(FloatTexture, Checkerboard2D) {
  absl::string_view directive =
      R"pbrt(Texture "name" "float" "checkerboard" "integer dimension" 2)pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         float_texture {
                                           name: "name"
                                           checkerboard2d {}
                                         }
                                       })pb"));
}

TEST(FloatTexture, Checkerboard3D) {
  absl::string_view directive =
      R"pbrt(Texture "name" "float" "checkerboard" "integer dimension" 3)pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         float_texture {
                                           name: "name"
                                           checkerboard3d {}
                                         }
                                       })pb"));
}

TEST(FloatTexture, Checkerboard4D) {
  absl::string_view directive =
      R"pbrt(Texture "name" "float" "checkerboard" "integer dimension" 4)pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         float_texture {
                                           name: "name"
                                           checkerboard3d {}
                                         }
                                       })pb"));
}

TEST(FloatTexture, Constant) {
  absl::string_view directive = R"pbrt(Texture "name" "float" "constant")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         float_texture {
                                           name: "name"
                                           constant {}
                                         }
                                       })pb"));
}

TEST(FloatTexture, Dots) {
  absl::string_view directive = R"pbrt(Texture "name" "float" "dots")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         float_texture {
                                           name: "name"
                                           dots {}
                                         }
                                       })pb"));
}

TEST(FloatTexture, FBm) {
  absl::string_view directive = R"pbrt(Texture "name" "float" "fbm")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         float_texture {
                                           name: "name"
                                           fbm {}
                                         }
                                       })pb"));
}

TEST(FloatTexture, ImageMap) {
  absl::string_view directive = R"pbrt(Texture "name" "float" "imagemap")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         float_texture {
                                           name: "name"
                                           imagemap {}
                                         }
                                       })pb"));
}

TEST(FloatTexture, Marble) {
  absl::string_view directive = R"pbrt(Texture "name" "float" "marble")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         float_texture { name: "name" }
                                       })pb"));
}

TEST(FloatTexture, Mix) {
  absl::string_view directive = R"pbrt(Texture "name" "float" "mix")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         float_texture {
                                           name: "name"
                                           mix {}
                                         }
                                       })pb"));
}

TEST(FloatTexture, Ptex) {
  absl::string_view directive = R"pbrt(Texture "name" "float" "ptex")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         float_texture {
                                           name: "name"
                                           ptex {}
                                         }
                                       })pb"));
}

TEST(FloatTexture, Scale) {
  absl::string_view directive = R"pbrt(Texture "name" "float" "scale")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         float_texture {
                                           name: "name"
                                           scale {}
                                         }
                                       })pb"));
}

TEST(FloatTexture, Windy) {
  absl::string_view directive = R"pbrt(Texture "name" "float" "windy")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         float_texture {
                                           name: "name"
                                           windy {}
                                         }
                                       })pb"));
}

TEST(FloatTexture, Wrinkled) {
  absl::string_view directive = R"pbrt(Texture "name" "float" "wrinkled")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         float_texture {
                                           name: "name"
                                           wrinkled {}
                                         }
                                       })pb"));
}

TEST(Integrator, AmbientOcclusion) {
  absl::string_view directive = R"pbrt(Integrator "ambientocclusion")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         integrator { ambientocclusion {} }
                                       })pb"));
}

TEST(Integrator, Bdpt) {
  absl::string_view directive = R"pbrt(Integrator "bdpt")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { integrator { bdpt {} } })pb"));
}

TEST(Integrator, DirectLighting) {
  absl::string_view directive = R"pbrt(Integrator "directlighting")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         integrator { directlighting {} }
                                       })pb"));
}

TEST(Integrator, Mlt) {
  absl::string_view directive = R"pbrt(Integrator "mlt")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { integrator { mlt {} } })pb"));
}

TEST(Integrator, Path) {
  absl::string_view directive = R"pbrt(Integrator "path")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { integrator { path {} } })pb"));
}

TEST(Integrator, Sppm) {
  absl::string_view directive = R"pbrt(Integrator "sppm")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { integrator { sppm {} } })pb"));
}

TEST(Integrator, VolPath) {
  absl::string_view directive = R"pbrt(Integrator "volpath")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { integrator { volpath {} } })pb"));
}

TEST(Integrator, Whitted) {
  absl::string_view directive = R"pbrt(Integrator "whitted")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { integrator { whitted {} } })pb"));
}

TEST(LightSource, Wrinkled) {
  absl::string_view directive = R"pbrt(LightSource "distant")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { light_source { distant {} } })pb"));
}

TEST(LightSource, Goniometric) {
  absl::string_view directive = R"pbrt(LightSource "goniometric")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         light_source { goniometric {} }
                                       })pb"));
}

TEST(LightSource, Infinite) {
  absl::string_view directive = R"pbrt(LightSource "infinite")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         light_source { infinite {} }
                                       })pb"));
}

TEST(LightSource, Point) {
  absl::string_view directive = R"pbrt(LightSource "point")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { light_source { point {} } })pb"));
}

TEST(LightSource, Projection) {
  absl::string_view directive = R"pbrt(LightSource "projection")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         light_source { projection {} }
                                       })pb"));
}

TEST(LightSource, Spot) {
  absl::string_view directive = R"pbrt(LightSource "spot")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { light_source { spot {} } })pb"));
}

TEST(MakeNamedMedium, Heterogeneous) {
  absl::string_view directive =
      R"pbrt(MakeNamedMedium "name" "string type" "heterogeneous")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         make_named_medium {
                                           name: "name"
                                           heterogeneous {}
                                         }
                                       })pb"));
}

TEST(MakeNamedMedium, Homogeneous) {
  absl::string_view directive =
      R"pbrt(MakeNamedMedium "name" "string type" "homogeneous")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         make_named_medium {
                                           name: "name"
                                           homogeneous {}
                                         }
                                       })pb"));
}

TEST(Material, Disney) {
  absl::string_view directive = R"pbrt(Material "disney")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { material { disney {} } })pb"));
}

TEST(Material, Fourier) {
  absl::string_view directive = R"pbrt(Material "fourier")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { material { fourier {} } })pb"));
}

TEST(Material, Glass) {
  absl::string_view directive = R"pbrt(Material "glass")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { material { glass {} } })pb"));
}

TEST(Material, KdSubsurface) {
  absl::string_view directive = R"pbrt(Material "kdsubsurface")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         material { kdsubsurface {} }
                                       })pb"));
}

TEST(Material, Matte) {
  absl::string_view directive = R"pbrt(Material "matte")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { material { matte {} } })pb"));
}

TEST(Material, None) {
  absl::string_view directive = R"pbrt(Material "none")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives { material {} })pb"));
}

TEST(Material, Mirror) {
  absl::string_view directive = R"pbrt(Material "mirror")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { material { mirror {} } })pb"));
}

TEST(Material, Mix) {
  absl::string_view directive = R"pbrt(Material "mix")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives { material { mix {} } })pb"));
}

TEST(Material, Plastic) {
  absl::string_view directive = R"pbrt(Material "plastic")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { material { plastic {} } })pb"));
}

TEST(Material, Substrate) {
  absl::string_view directive = R"pbrt(Material "substrate")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { material { substrate {} } })pb"));
}

TEST(Material, Subsurface) {
  absl::string_view directive = R"pbrt(Material "subsurface")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { material { subsurface {} } })pb"));
}

TEST(Material, Translucent) {
  absl::string_view directive = R"pbrt(Material "translucent")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { material { translucent {} } })pb"));
}

TEST(Material, Uber) {
  absl::string_view directive = R"pbrt(Material "uber")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { material { uber {} } })pb"));
}

TEST(Material, Empty) {
  absl::string_view directive = R"pbrt(Material "")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives { material {} })pb"));
}

TEST(PixelFilter, Box) {
  absl::string_view directive = R"pbrt(PixelFilter "box")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { pixel_filter { box {} } })pb"));
}

TEST(PixelFilter, Gaussian) {
  absl::string_view directive = R"pbrt(PixelFilter "gaussian")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         pixel_filter { gaussian {} }
                                       })pb"));
}

TEST(PixelFilter, Mitchell) {
  absl::string_view directive = R"pbrt(PixelFilter "mitchell")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         pixel_filter { mitchell {} }
                                       })pb"));
}

TEST(PixelFilter, Sinc) {
  absl::string_view directive = R"pbrt(PixelFilter "sinc")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { pixel_filter { sinc {} } })pb"));
}

TEST(PixelFilter, Triangle) {
  absl::string_view directive = R"pbrt(PixelFilter "triangle")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         pixel_filter { triangle {} }
                                       })pb"));
}

TEST(Sampler, Halton) {
  absl::string_view directive = R"pbrt(Sampler "halton")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { sampler { halton {} } })pb"));
}

TEST(Sampler, LowDiscrepancy) {
  absl::string_view directive = R"pbrt(Sampler "lowdiscrepancy")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         sampler { zerotwosequence {} }
                                       })pb"));
}

TEST(Sampler, MaxMinDist) {
  absl::string_view directive = R"pbrt(Sampler "maxmindist")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { sampler { maxmindist {} } })pb"));
}

TEST(Sampler, Random) {
  absl::string_view directive = R"pbrt(Sampler "random")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { sampler { random {} } })pb"));
}

TEST(Sampler, Stratified) {
  absl::string_view directive = R"pbrt(Sampler "stratified")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { sampler { stratified {} } })pb"));
}

TEST(Sampler, ZeroTwoSequence) {
  absl::string_view directive = R"pbrt(Sampler "02sequence")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         sampler { zerotwosequence {} }
                                       })pb"));
}

TEST(Shape, Cone) {
  absl::string_view directive = R"pbrt(Shape "cone")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives { shape { cone {} } })pb"));
}

TEST(Shape, Curve) {
  absl::string_view directive = R"pbrt(Shape "curve")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives { shape { curve {} } })pb"));
}

TEST(Shape, Cylinder) {
  absl::string_view directive = R"pbrt(Shape "cylinder")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { shape { cylinder {} } })pb"));
}

TEST(Shape, Disk) {
  absl::string_view directive = R"pbrt(Shape "disk")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives { shape { disk {} } })pb"));
}

TEST(Shape, HeightField) {
  absl::string_view directive = R"pbrt(Shape "heightfield")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { shape { heightfield {} } })pb"));
}

TEST(Shape, Hyperboloid) {
  absl::string_view directive = R"pbrt(Shape "hyperboloid")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { shape { hyperboloid {} } })pb"));
}

TEST(Shape, LoopSubdiv) {
  absl::string_view directive = R"pbrt(Shape "loopsubdiv")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { shape { loopsubdiv {} } })pb"));
}

TEST(Shape, Nurbs) {
  absl::string_view directive = R"pbrt(Shape "nurbs")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives { shape { nurbs {} } })pb"));
}

TEST(Shape, Paraboloid) {
  absl::string_view directive = R"pbrt(Shape "paraboloid")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { shape { paraboloid {} } })pb"));
}

TEST(Shape, PlyMesh) {
  absl::string_view directive = R"pbrt(Shape "plymesh")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { shape { plymesh {} } })pb"));
}

TEST(Shape, Sphere) {
  absl::string_view directive = R"pbrt(Shape "sphere")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives { shape { sphere {} } })pb"));
}

TEST(Shape, TriangleMesh) {
  absl::string_view directive = R"pbrt(Shape "trianglemesh")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { shape { trianglemesh {} } })pb"));
}

TEST(Shape, Overrides) {
  absl::string_view directive = R"pbrt(
    Shape "sphere"
        "texture bumpmap" "a"
        "texture roughness" "c"
        "texture sigma" "d"
        "texture uroughness" "e"
        "texture vroughness" "f"
        "texture Kd" "g"
        "texture Kr" "h"
        "texture Ks" "i"
        "texture Kt" "j"
        "texture opacity" "k"
        "texture reflect" "l"
        "texture transmit" "m"
        "string name" "Apple"
        "texture amount" "n"
        "texture k" "o"
        "texture sigma_a" "p"
        "texture sigma_s" "q"
        "string bsdffile" "file"
        "string namedmaterial1" "mat1"
        "string namedmaterial2" "mat2"
        "texture color" "r"
        "texture mfp" "s"
        "texture scatterdistance" "t"
        "texture alpha" "u"
        "texture anisotropic" "v"
        "texture beta_m" "w"
        "texture beta_n" "x"
        "texture clearcoat" "y"
        "texture clearcoatgloss" "z"
        "texture difftrans" "aa"
        "texture eumelanin" "ab"
        "texture flatness" "ac"
        "texture metallic" "ad"
        "texture pheomelanin" "ae"
        "texture spectrans" "af"
        "texture speculartint" "ag"
        "texture sheen" "ah"
        "texture sheentint" "ai"
        "float g" 1.0
        "float scale" 2.0
        "bool remaproughness" "true"
        "bool thin" "false"
    )pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(
      actual,
      EqualsProto(R"pb(directives {
                         shape {
                           sphere {}
                           overrides {
                             bumpmap { float_texture_name: "a" }
                             roughness { float_texture_name: "c" }
                             sigma { float_texture_name: "d" }
                             uroughness { float_texture_name: "e" }
                             vroughness { float_texture_name: "f" }
                             Kd { spectrum_texture_name: "g" }
                             Kr { spectrum_texture_name: "h" }
                             Ks { spectrum_texture_name: "i" }
                             Kt { spectrum_texture_name: "j" }
                             opacity { spectrum_texture_name: "k" }
                             reflect { spectrum_texture_name: "l" }
                             transmit { spectrum_texture_name: "m" }
                             name: APPLE
                             amount { spectrum_texture_name: "n" }
                             k { spectrum_texture_name: "o" }
                             sigma_a { spectrum_texture_name: "p" }
                             sigma_s { spectrum_texture_name: "q" }
                             filename: "file"
                             namedmaterial1: "mat1"
                             namedmaterial2: "mat2"
                             color { spectrum_texture_name: "r" }
                             mfp { spectrum_texture_name: "s" }
                             scatterdistance { spectrum_texture_name: "t" }
                             alpha { float_texture_name: "u" }
                             anisotropic { float_texture_name: "v" }
                             beta_m { float_texture_name: "w" }
                             beta_n { float_texture_name: "x" }
                             clearcoat { float_texture_name: "y" }
                             clearcoatgloss { float_texture_name: "z" }
                             difftrans { float_texture_name: "aa" }
                             eumelanin { float_texture_name: "ab" }
                             flatness { float_texture_name: "ac" }
                             metallic { float_texture_name: "ad" }
                             pheomelanin { float_texture_name: "ae" }
                             spectrans { float_texture_name: "af" }
                             speculartint { float_texture_name: "ag" }
                             sheen { float_texture_name: "ah" }
                             sheentint { float_texture_name: "ai" }
                             g: 1.0
                             scale: 2.0
                             remaproughness: true
                             thin: false
                           }
                         }
                       })pb"));
}

TEST(Shape, OverridesEtaFloat) {
  absl::string_view directive = R"pbrt(Shape "sphere" "float eta" 1.0)pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         shape {
                                           sphere {}
                                           overrides {
                                             eta { float_value: 1.0 }
                                             eta_as_value: 1.0
                                           }
                                         }
                                       })pb"));
}

TEST(Shape, OverridesEtaTexture) {
  absl::string_view directive = R"pbrt(Shape "sphere" "texture eta" "a")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(
      actual,
      EqualsProto(
          R"pb(directives {
                 shape {
                   sphere {}
                   overrides {
                     eta { float_texture_name: "a" }
                     eta_as_spectrum_texture { spectrum_texture_name: "a" }
                   }
                 }
               })pb"));
}

TEST(Shape, OverridesEtaSpectrum) {
  absl::string_view directive = R"pbrt(Shape "sphere" "rgb eta" [1 1 1])pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives {
                                 shape {
                                   sphere {}
                                   overrides {
                                     eta_as_spectrum_texture {
                                       rgb_spectrum { r: 1.0 g: 1.0 b: 1.0 }
                                     }
                                   }
                                 }
                               })pb"));
}

TEST(SpectrumTexture, Bilerp) {
  absl::string_view directive = R"pbrt(Texture "name" "spectrum" "bilerp")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         spectrum_texture {
                                           name: "name"
                                           bilerp {}
                                         }
                                       })pb"));
}

TEST(SpectrumTexture, CheckerboardDefault) {
  absl::string_view directive =
      R"pbrt(Texture "name" "spectrum" "checkerboard")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         spectrum_texture {
                                           name: "name"
                                           checkerboard2d {}
                                         }
                                       })pb"));
}

TEST(SpectrumTexture, Checkerboard2D) {
  absl::string_view directive =
      R"pbrt(Texture "name" "spectrum" "checkerboard" "integer dimension" 2)pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         spectrum_texture {
                                           name: "name"
                                           checkerboard2d {}
                                         }
                                       })pb"));
}

TEST(SpectrumTexture, Checkerboard3D) {
  absl::string_view directive =
      R"pbrt(Texture "name" "spectrum" "checkerboard" "integer dimension" 3)pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         spectrum_texture {
                                           name: "name"
                                           checkerboard3d {}
                                         }
                                       })pb"));
}

TEST(SpectrumTexture, Checkerboard4D) {
  absl::string_view directive =
      R"pbrt(Texture "name" "spectrum" "checkerboard" "integer dimension" 4)pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         spectrum_texture {
                                           name: "name"
                                           checkerboard3d {}
                                         }
                                       })pb"));
}

TEST(SpectrumTexture, Constant) {
  absl::string_view directive =
      R"pbrt(Texture "name" "spectrum" "constant")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         spectrum_texture {
                                           name: "name"
                                           constant {}
                                         }
                                       })pb"));
}

TEST(SpectrumTexture, Dots) {
  absl::string_view directive = R"pbrt(Texture "name" "spectrum" "dots")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         spectrum_texture {
                                           name: "name"
                                           dots {}
                                         }
                                       })pb"));
}

TEST(SpectrumTexture, FBm) {
  absl::string_view directive = R"pbrt(Texture "name" "spectrum" "fbm")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         spectrum_texture {
                                           name: "name"
                                           fbm {}
                                         }
                                       })pb"));
}

TEST(SpectrumTexture, ImageMap) {
  absl::string_view directive =
      R"pbrt(Texture "name" "spectrum" "imagemap")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         spectrum_texture {
                                           name: "name"
                                           imagemap {}
                                         }
                                       })pb"));
}

TEST(SpectrumTexture, Marble) {
  absl::string_view directive = R"pbrt(Texture "name" "spectrum" "marble")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         spectrum_texture {
                                           name: "name"
                                           marble {}
                                         }
                                       })pb"));
}

TEST(SpectrumTexture, Mix) {
  absl::string_view directive = R"pbrt(Texture "name" "spectrum" "mix")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         spectrum_texture {
                                           name: "name"
                                           mix {}
                                         }
                                       })pb"));
}

TEST(SpectrumTexture, Scale) {
  absl::string_view directive = R"pbrt(Texture "name" "spectrum" "scale")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         spectrum_texture {
                                           name: "name"
                                           scale {}
                                         }
                                       })pb"));
}

TEST(SpectrumTexture, Windy) {
  absl::string_view directive = R"pbrt(Texture "name" "spectrum" "windy")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         spectrum_texture {
                                           name: "name"
                                           windy {}
                                         }
                                       })pb"));
}

TEST(SpectrumTexture, Wrinkled) {
  absl::string_view directive =
      R"pbrt(Texture "name" "spectrum" "wrinkled")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         spectrum_texture {
                                           name: "name"
                                           wrinkled {}
                                         }
                                       })pb"));
}

}  // namespace
}  // namespace pbrt_proto::v3
