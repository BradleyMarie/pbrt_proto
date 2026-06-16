#include "pbrt_proto/v1/convert.h"

#include <sstream>
#include <string>

#include "absl/status/status.h"
#include "absl/status/status_matchers.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "pbrt_proto/testing/proto_matchers.h"
#include "pbrt_proto/v1/v1.pb.h"

namespace pbrt_proto::v1 {
namespace {

using ::absl_testing::StatusIs;
using ::google::protobuf::EqualsProto;

absl::Status Convert(absl::string_view input, PbrtProto& output) {
  std::string as_string(input);
  std::istringstream as_stream(as_string);
  return Convert(as_stream, output);
}

TEST(Accelerator, Grid) {
  absl::string_view directive = R"pbrt(Accelerator "grid")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { accelerator { grid {} } })pb"));
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
                                         area_light_source { area {} }
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
                                         float_texture { name: "name" }
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

TEST(LightSource, InfiniteSample) {
  absl::string_view directive = R"pbrt(LightSource "infinitesample")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         light_source { infinitesample {} }
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

TEST(Material, BluePaint) {
  absl::string_view directive = R"pbrt(Material "bluepaint")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { material { bluepaint {} } })pb"));
}

TEST(Material, BrushedMetal) {
  absl::string_view directive = R"pbrt(Material "brushedmetal")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         material { brushedmetal {} }
                                       })pb"));
}

TEST(Material, Clay) {
  absl::string_view directive = R"pbrt(Material "clay")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { material { clay {} } })pb"));
}

TEST(Material, Felt) {
  absl::string_view directive = R"pbrt(Material "felt")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { material { felt {} } })pb"));
}

TEST(Material, Glass) {
  absl::string_view directive = R"pbrt(Material "glass")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { material { glass {} } })pb"));
}

TEST(Material, Matte) {
  absl::string_view directive = R"pbrt(Material "matte")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { material { matte {} } })pb"));
}

TEST(Material, Mirror) {
  absl::string_view directive = R"pbrt(Material "mirror")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { material { mirror {} } })pb"));
}

TEST(Material, Plastic) {
  absl::string_view directive = R"pbrt(Material "plastic")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { material { plastic {} } })pb"));
}

TEST(Material, Primer) {
  absl::string_view directive = R"pbrt(Material "primer")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { material { primer {} } })pb"));
}

TEST(Material, ShinyMetal) {
  absl::string_view directive = R"pbrt(Material "shinymetal")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { material { shinymetal {} } })pb"));
}

TEST(Material, Skin) {
  absl::string_view directive = R"pbrt(Material "skin")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { material { skin {} } })pb"));
}

TEST(Material, Substrate) {
  absl::string_view directive = R"pbrt(Material "substrate")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { material { substrate {} } })pb"));
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

TEST(Material, None) {
  absl::string_view directive = R"pbrt(Material "")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { material { matte {} } })pb"));
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

TEST(Sampler, BestCandidate) {
  absl::string_view directive = R"pbrt(Sampler "bestcandidate")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         sampler { bestcandidate {} }
                                       })pb"));
}

TEST(Sampler, LowDiscrepancy) {
  absl::string_view directive = R"pbrt(Sampler "lowdiscrepancy")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         sampler { lowdiscrepancy {} }
                                       })pb"));
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

TEST(Shape, Cone) {
  absl::string_view directive = R"pbrt(Shape "cone")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives { shape { cone {} } })pb"));
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
        "texture eta" "b"
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
    )pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives {
                                 shape {
                                   sphere {}
                                   overrides {
                                     bumpmap { float_texture_name: "a" }
                                     eta { float_texture_name: "b" }
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
                                         spectrum_texture { name: "name" }
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

TEST(SurfaceIntegrator, Bidirectional) {
  absl::string_view directive = R"pbrt(SurfaceIntegrator "bidirectional")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         surface_integrator { bidirectional {} }
                                       })pb"));
}

TEST(SurfaceIntegrator, Debug) {
  absl::string_view directive = R"pbrt(SurfaceIntegrator "debug")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         surface_integrator { debug {} }
                                       })pb"));
}

TEST(SurfaceIntegrator, DirectLighting) {
  absl::string_view directive = R"pbrt(SurfaceIntegrator "directlighting")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives {
                                 surface_integrator { directlighting {} }
                               })pb"));
}

TEST(SurfaceIntegrator, ExPhotonMap) {
  absl::string_view directive = R"pbrt(SurfaceIntegrator "exphotonmap")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         surface_integrator { exphotonmap {} }
                                       })pb"));
}

TEST(SurfaceIntegrator, Igi) {
  absl::string_view directive = R"pbrt(SurfaceIntegrator "igi")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         surface_integrator { igi {} }
                                       })pb"));
}

TEST(SurfaceIntegrator, IrradianceCache) {
  absl::string_view directive =
      R"pbrt(SurfaceIntegrator "irradiancecache")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives {
                                 surface_integrator { irradiancecache {} }
                               })pb"));
}

TEST(SurfaceIntegrator, Path) {
  absl::string_view directive = R"pbrt(SurfaceIntegrator "path")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         surface_integrator { path {} }
                                       })pb"));
}

TEST(SurfaceIntegrator, PhotonMap) {
  absl::string_view directive = R"pbrt(SurfaceIntegrator "photonmap")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         surface_integrator { photonmap {} }
                                       })pb"));
}

TEST(SurfaceIntegrator, Whitted) {
  absl::string_view directive = R"pbrt(SurfaceIntegrator "whitted")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         surface_integrator { whitted {} }
                                       })pb"));
}

TEST(Volume, Exponential) {
  absl::string_view directive = R"pbrt(Volume "exponential")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { volume { exponential {} } })pb"));
}

TEST(Volume, Homogeneous) {
  absl::string_view directive = R"pbrt(Volume "homogeneous")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { volume { homogeneous {} } })pb"));
}

TEST(Volume, VolumeGrid) {
  absl::string_view directive = R"pbrt(Volume "volumegrid")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(directives { volume { volumegrid {} } })pb"));
}

TEST(VolumeIntegrator, Emission) {
  absl::string_view directive = R"pbrt(VolumeIntegrator "emission")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         volume_integrator { emission {} }
                                       })pb"));
}

TEST(VolumeIntegrator, Single) {
  absl::string_view directive = R"pbrt(VolumeIntegrator "single")pbrt";

  PbrtProto actual;
  EXPECT_TRUE(Convert(directive, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(directives {
                                         volume_integrator { single {} }
                                       })pb"));
}

}  // namespace
}  // namespace pbrt_proto::v1
