#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "googletest/include/gtest/gtest.h"
#include "tools/cpp/runfiles/runfiles.h"

namespace {

using ::bazel::tools::cpp::runfiles::Runfiles;

std::string GetRunfilePath(const std::string& path) {
  std::unique_ptr<Runfiles> runfiles(Runfiles::CreateForTest());
  return runfiles->Rlocation("_main/tools/" + path);
}

bool Convert(int version, const std::string& file_name,
             bool allow_warnings = false) {
  std::string binary = GetRunfilePath("pbrt_proto_converter");
  std::string input_file = GetRunfilePath("test_data/" + file_name);
  std::string output_file = GetRunfilePath("test_data/" + file_name + ".out");
  std::string command = binary + " --pbrt_version=" + std::to_string(version) +
                        " --recursive " + input_file + " 2> " + output_file;

  int result = std::system(command.c_str());

  std::ostringstream output_stream;
  output_stream << std::ifstream(output_file.c_str()).rdbuf();
  std::string output = output_stream.str();

  if (result != 0 || (!output.empty() && !allow_warnings)) {
    std::cout << output << std::endl;
    return false;
  }

  return true;
}

TEST(ProtoConverter, PBRTv3) {
  EXPECT_TRUE(
      Convert(3, "pbrt-v3-scenes/barcelona-pavilion/pavilion-day.pbrt"));
  EXPECT_TRUE(
      Convert(3, "pbrt-v3-scenes/barcelona-pavilion/pavilion-night.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/bathroom/bathroom.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/bmw-m6/bmw-m6.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/breakfast/breakfast-lamps.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/breakfast/breakfast.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/breakfast/f16-8a.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/breakfast/f16-8b.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/buddha-fractal/buddha-fractal.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/bunny-fur/f3-15.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/caustic-glass/f16-11a.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/caustic-glass/f16-11b.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/caustic-glass/f16-9a.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/caustic-glass/f16-9b.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/caustic-glass/f16-9c.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/caustic-glass/glass.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/chopper-titan/chopper-titan.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/cloud/cloud.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/cloud/f15-4a.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/cloud/f15-4b.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/cloud/f15-4c.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/cloud/smoke.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/coffee-splash/f15-5.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/coffee-splash/splash.pbrt"));
  EXPECT_TRUE(Convert(
      3, "pbrt-v3-scenes/contemporary-bathroom/contemporary-bathroom.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/crown/crown.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/dambreak/dambreak0.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/dambreak/dambreak1.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/dragon/f11-13.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/dragon/f11-14.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/dragon/f14-3.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/dragon/f14-5.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/dragon/f15-13.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/dragon/f8-10.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/dragon/f8-14a.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/dragon/f8-14b.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/dragon/f8-21a.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/dragon/f8-21b.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/dragon/f8-24.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/dragon/f8-4a.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/dragon/f8-4b.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/dragon/f9-3.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/dragon/f9-4.pbrt"));
  EXPECT_TRUE(
      Convert(3, "pbrt-v3-scenes/ecosys/ecosys.pbrt", /*allow_warnings=*/true));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/figures/f10-1ac.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/figures/f10-1b.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/figures/f11-15.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/figures/f3-18.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/figures/f7-19a.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/figures/f7-19b.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/figures/f7-19c.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/figures/f7-30a.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/figures/f7-30b.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/figures/f7-30c.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/figures/f7-34a.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/figures/f7-34b.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/figures/f7-34c.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/figures/f8-22.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/ganesha/f3-11.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/ganesha/ganesha.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/hair/curly-hair.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/hair/models/block.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/hair/models/curly-hair.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/hair/models/straight-hair.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/hair/sphere-hairblock.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/hair/straight-hair.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/head/f9-5.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/head/head.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/killeroos/killeroo-gold.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/killeroos/killeroo-moving.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/killeroos/killeroo-simple.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/landscape/f4-1.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/landscape/f6-13.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/landscape/f6-14.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/landscape/view-0.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/landscape/view-1.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/landscape/view-2.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/landscape/view-3.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/landscape/view-4.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/lte-orb/lte-orb-roughglass.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/lte-orb/lte-orb-silver.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/measure-one/frame120.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/measure-one/frame180.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/measure-one/frame210.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/measure-one/frame25.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/measure-one/frame300.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/measure-one/frame35.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/measure-one/frame380.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/measure-one/frame52.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/measure-one/frame85.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/measure-one/main.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/measure-one/textures.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/pbrt-book/book.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/sanmiguel/f10-8.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/sanmiguel/f16-21a.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/sanmiguel/f16-21b.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/sanmiguel/f16-21c.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/sanmiguel/f6-17.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/sanmiguel/f6-25.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/sanmiguel/sanmiguel_cam1.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/sanmiguel/sanmiguel_cam14.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/sanmiguel/sanmiguel_cam15.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/sanmiguel/sanmiguel_cam18.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/sanmiguel/sanmiguel_cam20.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/sanmiguel/sanmiguel_cam25.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/sanmiguel/sanmiguel_cam3.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/sanmiguel/sanmiguel_cam4.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/sanmiguel/sanmiguel.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/simple/anim-bluespheres.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/simple/buddha.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/simple/bump-sphere.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/simple/caustic-proj.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/simple/dof-dragons.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/simple/miscquads.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/simple/room-mlt.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/simple/room-path.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/simple/room-sppm.pbrt"));
  EXPECT_TRUE(
      Convert(3, "pbrt-v3-scenes/simple/spheres-differentials-texfilt.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/simple/spotfog.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/simple/teapot-area-light.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/simple/teapot-metal.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/smoke-plume/plume-084.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/smoke-plume/plume-184.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/smoke-plume/plume-284.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/sportscar/f12-19a.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/sportscar/f12-19b.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/sportscar/f12-20a.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/sportscar/f12-20b.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/sportscar/f7-37a.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/sportscar/f7-37b.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/sportscar/sportscar.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/sssdragon/dragon_10.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/sssdragon/dragon_250.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/sssdragon/dragon_50.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/sssdragon/f15-7.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/structuresynth/arcsphere.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/structuresynth/ballpile.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/structuresynth/metal.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/structuresynth/microcity.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/transparent-machines/frame1266.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/transparent-machines/frame542.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/transparent-machines/frame675.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/transparent-machines/frame812.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/transparent-machines/frame888.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/tt/tt.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/veach-bidir/bidir.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/veach-mis/mis.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/villa/f16-20a.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/villa/f16-20b.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/villa/f16-20c.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/villa/villa-daylight.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/villa/villa-lights-on.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/villa/villa-photons.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/volume-caustic/caustic.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/volume-caustic/f16-22a.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/volume-caustic/f16-22b.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/vw-van/vw-van.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/white-room/lights-daytime.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/white-room/lights-night.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/white-room/whiteroom-daytime.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/white-room/whiteroom-night.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/wip/glass/glass.pbrt"));
  EXPECT_TRUE(Convert(3, "pbrt-v3-scenes/yeahright/yeahright.pbrt"));
}

}  // namespace
