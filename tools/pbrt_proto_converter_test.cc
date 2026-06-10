#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <thread>
#include <tuple>
#include <vector>

#include "gtest/gtest.h"
#include "tools/cpp/runfiles/runfiles.h"

namespace {

using ::bazel::tools::cpp::runfiles::Runfiles;
using ::testing::TestWithParam;
using ::testing::ValuesIn;

const static std::filesystem::path kBinaryName = "pbrt_proto_converter";
const static std::filesystem::path kTestData = "test_data";

std::string GetRunfilePath(const std::filesystem::path& file_path) {
  std::filesystem::path path =
      std::filesystem::path("_main/tools/") / file_path;

  std::unique_ptr<Runfiles> runfiles(Runfiles::CreateForTest());
  return std::filesystem::path(runfiles->Rlocation(path.generic_string()))
      .string();
}

std::pair<int, std::string> Convert(int version,
                                    const std::filesystem::path& file_name,
                                    bool allow_warnings = false) {
  std::filesystem::path binary_name = kBinaryName;

#ifdef _WIN32
  binary_name.replace_extension(".exe");
#endif

  std::string binary = GetRunfilePath(binary_name);
  std::string input_file = GetRunfilePath(kTestData / file_name);
  std::string output_file = input_file + ".out";

#ifdef _WIN32
  std::string command = "\"\"" + binary +
                        "\" --pbrt_version=" + std::to_string(version) +
                        " --validate_only --recursive \"" + input_file +
                        "\" 2> \"" + output_file + "\"\"";
#else
  std::string command = "\"" + binary +
                        "\" --pbrt_version=" + std::to_string(version) +
                        " --validate_only --recursive \"" + input_file +
                        "\" 2> \"" + output_file + "\"";
#endif

  int result = std::system(command.c_str());
  if (allow_warnings) {
    return std::make_pair(result, "");
  }

  std::ostringstream output_stream;
  output_stream << std::ifstream(output_file.c_str()).rdbuf();

  return std::make_pair(result, output_stream.str());
}

struct TestInput {
  std::filesystem::path path;
  bool allow_warnings = false;
};

const std::vector<TestInput> pbrt_v1_inputs = {
    {"pbrt-v1-scenes/arealight-sampling.pbrt"},
    {"pbrt-v1-scenes/areapot.pbrt"},
    {"pbrt-v1-scenes/balls.pbrt"},
    {"pbrt-v1-scenes/buddha.pbrt"},
    {"pbrt-v1-scenes/bump-sphere.pbrt"},
    {"pbrt-v1-scenes/bunny.pbrt"},
    {"pbrt-v1-scenes/caustic-proj.pbrt"},
    {"pbrt-v1-scenes/caustic-simple.pbrt"},
    {"pbrt-v1-scenes/checkerboard.pbrt"},
    {"pbrt-v1-scenes/cylinders.pbrt"},
    {"pbrt-v1-scenes/disks.pbrt", /*allow_warnings=*/true},
    {"pbrt-v1-scenes/dof-dragons.pbrt"},
    {"pbrt-v1-scenes/dof-simple.pbrt"},
    {"pbrt-v1-scenes/dragon.pbrt"},
    {"pbrt-v1-scenes/fbm-sphere.pbrt"},
    {"pbrt-v1-scenes/killeroo-glossy.pbrt"},
    {"pbrt-v1-scenes/killeroo-materials.pbrt"},
    {"pbrt-v1-scenes/killeroo-simple.pbrt"},
    {"pbrt-v1-scenes/light-examples.pbrt"},
    {"pbrt-v1-scenes/miscquads.pbrt"},
    {"pbrt-v1-scenes/mis.pbrt"},
    {"pbrt-v1-scenes/mount.pbrt"},
    {"pbrt-v1-scenes/plants-directsun.pbrt", /*allow_warnings=*/true},
    {"pbrt-v1-scenes/plants-dof-1.pbrt", /*allow_warnings=*/true},
    {"pbrt-v1-scenes/plants-dof-2.pbrt", /*allow_warnings=*/true},
    {"pbrt-v1-scenes/plants-dusk.pbrt", /*allow_warnings=*/true},
    {"pbrt-v1-scenes/plants-godrays.pbrt", /*allow_warnings=*/true},
    {"pbrt-v1-scenes/plants.pbrt", /*allow_warnings=*/true},
    {"pbrt-v1-scenes/sibenik.pbrt"},
    {"pbrt-v1-scenes/simple.pbrt"},
    {"pbrt-v1-scenes/smoke-1.pbrt"},
    {"pbrt-v1-scenes/smoke-2.pbrt"},
    {"pbrt-v1-scenes/smoke-3.pbrt"},
    {"pbrt-v1-scenes/sphere-ewa-vs-trilerp.pbrt"},
    {"pbrt-v1-scenes/spheres-aniso.pbrt"},
    {"pbrt-v1-scenes/spheres-over-plane.pbrt"},
    {"pbrt-v1-scenes/sponza-corner.pbrt"},
    {"pbrt-v1-scenes/sponza-fog-corridor.pbrt", /*allow_warnings=*/true},
    {"pbrt-v1-scenes/sponza-fog.pbrt"},
    {"pbrt-v1-scenes/sponza-irrad.pbrt"},
    {"pbrt-v1-scenes/sponza-phomap.pbrt"},
    {"pbrt-v1-scenes/spotfog.pbrt"},
    {"pbrt-v1-scenes/subdiv-example.pbrt", /*allow_warnings=*/true},
    {"pbrt-v1-scenes/subdiv-tetra.pbrt"},
    // {"pbrt-v1-scenes/tt.pbrt"},
    // {"pbrt-v1-scenes/tt-tire.pbrt"},
};

const std::vector<TestInput> pbrt_v3_inputs = {
    {"pbrt-v3-scenes/barcelona-pavilion/pavilion-day.pbrt"},
    {"pbrt-v3-scenes/barcelona-pavilion/pavilion-night.pbrt"},
    {"pbrt-v3-scenes/bathroom/bathroom.pbrt"},
    {"pbrt-v3-scenes/bmw-m6/bmw-m6.pbrt"},
    {"pbrt-v3-scenes/breakfast/breakfast-lamps.pbrt"},
    {"pbrt-v3-scenes/breakfast/breakfast.pbrt"},
    {"pbrt-v3-scenes/breakfast/f16-8a.pbrt"},
    {"pbrt-v3-scenes/breakfast/f16-8b.pbrt"},
    {"pbrt-v3-scenes/buddha-fractal/buddha-fractal.pbrt"},
    {"pbrt-v3-scenes/bunny-fur/f3-15.pbrt"},
    {"pbrt-v3-scenes/caustic-glass/f16-11a.pbrt"},
    {"pbrt-v3-scenes/caustic-glass/f16-11b.pbrt"},
    {"pbrt-v3-scenes/caustic-glass/f16-9a.pbrt"},
    {"pbrt-v3-scenes/caustic-glass/f16-9b.pbrt"},
    {"pbrt-v3-scenes/caustic-glass/f16-9c.pbrt"},
    {"pbrt-v3-scenes/caustic-glass/glass.pbrt"},
    {"pbrt-v3-scenes/chopper-titan/chopper-titan.pbrt"},
    {"pbrt-v3-scenes/cloud/cloud.pbrt"},
    {"pbrt-v3-scenes/cloud/f15-4a.pbrt"},
    {"pbrt-v3-scenes/cloud/f15-4b.pbrt"},
    {"pbrt-v3-scenes/cloud/f15-4c.pbrt"},
    {"pbrt-v3-scenes/cloud/smoke.pbrt"},
    {"pbrt-v3-scenes/coffee-splash/f15-5.pbrt"},
    {"pbrt-v3-scenes/coffee-splash/splash.pbrt"},
    {"pbrt-v3-scenes/contemporary-bathroom/contemporary-bathroom.pbrt"},
    {"pbrt-v3-scenes/crown/crown.pbrt"},
    {"pbrt-v3-scenes/dambreak/dambreak0.pbrt"},
    {"pbrt-v3-scenes/dambreak/dambreak1.pbrt"},
    {"pbrt-v3-scenes/dragon/f11-13.pbrt"},
    {"pbrt-v3-scenes/dragon/f11-14.pbrt"},
    {"pbrt-v3-scenes/dragon/f14-3.pbrt"},
    {"pbrt-v3-scenes/dragon/f14-5.pbrt"},
    {"pbrt-v3-scenes/dragon/f15-13.pbrt"},
    {"pbrt-v3-scenes/dragon/f8-10.pbrt"},
    {"pbrt-v3-scenes/dragon/f8-14a.pbrt"},
    {"pbrt-v3-scenes/dragon/f8-14b.pbrt"},
    {"pbrt-v3-scenes/dragon/f8-21a.pbrt"},
    {"pbrt-v3-scenes/dragon/f8-21b.pbrt"},
    {"pbrt-v3-scenes/dragon/f8-24.pbrt"},
    {"pbrt-v3-scenes/dragon/f8-4a.pbrt"},
    {"pbrt-v3-scenes/dragon/f8-4b.pbrt"},
    {"pbrt-v3-scenes/dragon/f9-3.pbrt"},
    {"pbrt-v3-scenes/dragon/f9-4.pbrt"},
    {"pbrt-v3-scenes/ecosys/ecosys.pbrt", /*allow_warnings=*/true},
    {"pbrt-v3-scenes/figures/f10-1ac.pbrt"},
    {"pbrt-v3-scenes/figures/f10-1b.pbrt"},
    {"pbrt-v3-scenes/figures/f11-15.pbrt"},
    {"pbrt-v3-scenes/figures/f3-18.pbrt"},
    {"pbrt-v3-scenes/figures/f7-19a.pbrt"},
    {"pbrt-v3-scenes/figures/f7-19b.pbrt"},
    {"pbrt-v3-scenes/figures/f7-19c.pbrt"},
    {"pbrt-v3-scenes/figures/f7-30a.pbrt"},
    {"pbrt-v3-scenes/figures/f7-30b.pbrt"},
    {"pbrt-v3-scenes/figures/f7-30c.pbrt"},
    {"pbrt-v3-scenes/figures/f7-34a.pbrt"},
    {"pbrt-v3-scenes/figures/f7-34b.pbrt"},
    {"pbrt-v3-scenes/figures/f7-34c.pbrt"},
    {"pbrt-v3-scenes/figures/f8-22.pbrt"},
    {"pbrt-v3-scenes/ganesha/f3-11.pbrt"},
    {"pbrt-v3-scenes/ganesha/ganesha.pbrt"},
    {"pbrt-v3-scenes/hair/curly-hair.pbrt"},
    {"pbrt-v3-scenes/hair/models/block.pbrt"},
    {"pbrt-v3-scenes/hair/models/curly-hair.pbrt"},
    {"pbrt-v3-scenes/hair/models/straight-hair.pbrt"},
    {"pbrt-v3-scenes/hair/sphere-hairblock.pbrt"},
    {"pbrt-v3-scenes/hair/straight-hair.pbrt"},
    {"pbrt-v3-scenes/head/f9-5.pbrt"},
    {"pbrt-v3-scenes/head/head.pbrt"},
    {"pbrt-v3-scenes/killeroos/killeroo-gold.pbrt"},
    {"pbrt-v3-scenes/killeroos/killeroo-moving.pbrt"},
    {"pbrt-v3-scenes/killeroos/killeroo-simple.pbrt"},
    {"pbrt-v3-scenes/landscape/f4-1.pbrt"},
    {"pbrt-v3-scenes/landscape/f6-13.pbrt"},
    {"pbrt-v3-scenes/landscape/f6-14.pbrt"},
    {"pbrt-v3-scenes/landscape/view-0.pbrt"},
    {"pbrt-v3-scenes/landscape/view-1.pbrt"},
    {"pbrt-v3-scenes/landscape/view-2.pbrt"},
    {"pbrt-v3-scenes/landscape/view-3.pbrt"},
    {"pbrt-v3-scenes/landscape/view-4.pbrt"},
    {"pbrt-v3-scenes/lte-orb/lte-orb-roughglass.pbrt"},
    {"pbrt-v3-scenes/lte-orb/lte-orb-silver.pbrt"},
    {"pbrt-v3-scenes/measure-one/frame120.pbrt"},
    {"pbrt-v3-scenes/measure-one/frame180.pbrt"},
    {"pbrt-v3-scenes/measure-one/frame210.pbrt"},
    {"pbrt-v3-scenes/measure-one/frame25.pbrt"},
    {"pbrt-v3-scenes/measure-one/frame300.pbrt"},
    {"pbrt-v3-scenes/measure-one/frame35.pbrt"},
    {"pbrt-v3-scenes/measure-one/frame380.pbrt"},
    {"pbrt-v3-scenes/measure-one/frame52.pbrt"},
    {"pbrt-v3-scenes/measure-one/frame85.pbrt"},
    {"pbrt-v3-scenes/measure-one/main.pbrt"},
    {"pbrt-v3-scenes/measure-one/textures.pbrt"},
    {"pbrt-v3-scenes/pbrt-book/book.pbrt"},
    {"pbrt-v3-scenes/sanmiguel/f10-8.pbrt"},
    {"pbrt-v3-scenes/sanmiguel/f16-21a.pbrt"},
    {"pbrt-v3-scenes/sanmiguel/f16-21b.pbrt"},
    {"pbrt-v3-scenes/sanmiguel/f16-21c.pbrt"},
    {"pbrt-v3-scenes/sanmiguel/f6-17.pbrt"},
    {"pbrt-v3-scenes/sanmiguel/f6-25.pbrt"},
    {"pbrt-v3-scenes/sanmiguel/sanmiguel_cam1.pbrt"},
    {"pbrt-v3-scenes/sanmiguel/sanmiguel_cam14.pbrt"},
    {"pbrt-v3-scenes/sanmiguel/sanmiguel_cam15.pbrt"},
    {"pbrt-v3-scenes/sanmiguel/sanmiguel_cam18.pbrt"},
    {"pbrt-v3-scenes/sanmiguel/sanmiguel_cam20.pbrt"},
    {"pbrt-v3-scenes/sanmiguel/sanmiguel_cam25.pbrt"},
    {"pbrt-v3-scenes/sanmiguel/sanmiguel_cam3.pbrt"},
    {"pbrt-v3-scenes/sanmiguel/sanmiguel_cam4.pbrt"},
    {"pbrt-v3-scenes/sanmiguel/sanmiguel.pbrt"},
    {"pbrt-v3-scenes/simple/anim-bluespheres.pbrt"},
    {"pbrt-v3-scenes/simple/buddha.pbrt"},
    {"pbrt-v3-scenes/simple/bump-sphere.pbrt"},
    {"pbrt-v3-scenes/simple/caustic-proj.pbrt"},
    {"pbrt-v3-scenes/simple/dof-dragons.pbrt"},
    {"pbrt-v3-scenes/simple/miscquads.pbrt"},
    {"pbrt-v3-scenes/simple/room-mlt.pbrt"},
    {"pbrt-v3-scenes/simple/room-path.pbrt"},
    {"pbrt-v3-scenes/simple/room-sppm.pbrt"},
    {"pbrt-v3-scenes/simple/spheres-differentials-texfilt.pbrt"},
    {"pbrt-v3-scenes/simple/spotfog.pbrt"},
    {"pbrt-v3-scenes/simple/teapot-area-light.pbrt"},
    {"pbrt-v3-scenes/simple/teapot-metal.pbrt"},
    {"pbrt-v3-scenes/smoke-plume/plume-084.pbrt"},
    {"pbrt-v3-scenes/smoke-plume/plume-184.pbrt"},
    {"pbrt-v3-scenes/smoke-plume/plume-284.pbrt"},
    {"pbrt-v3-scenes/sportscar/f12-19a.pbrt"},
    {"pbrt-v3-scenes/sportscar/f12-19b.pbrt"},
    {"pbrt-v3-scenes/sportscar/f12-20a.pbrt"},
    {"pbrt-v3-scenes/sportscar/f12-20b.pbrt"},
    {"pbrt-v3-scenes/sportscar/f7-37a.pbrt"},
    {"pbrt-v3-scenes/sportscar/f7-37b.pbrt"},
    {"pbrt-v3-scenes/sportscar/sportscar.pbrt"},
    {"pbrt-v3-scenes/sssdragon/dragon_10.pbrt"},
    {"pbrt-v3-scenes/sssdragon/dragon_250.pbrt"},
    {"pbrt-v3-scenes/sssdragon/dragon_50.pbrt"},
    {"pbrt-v3-scenes/sssdragon/f15-7.pbrt"},
    {"pbrt-v3-scenes/structuresynth/arcsphere.pbrt"},
    {"pbrt-v3-scenes/structuresynth/ballpile.pbrt"},
    {"pbrt-v3-scenes/structuresynth/metal.pbrt"},
    {"pbrt-v3-scenes/structuresynth/microcity.pbrt"},
    {"pbrt-v3-scenes/transparent-machines/frame1266.pbrt"},
    {"pbrt-v3-scenes/transparent-machines/frame542.pbrt"},
    {"pbrt-v3-scenes/transparent-machines/frame675.pbrt"},
    {"pbrt-v3-scenes/transparent-machines/frame812.pbrt"},
    {"pbrt-v3-scenes/transparent-machines/frame888.pbrt"},
    {"pbrt-v3-scenes/tt/tt.pbrt"},
    {"pbrt-v3-scenes/veach-bidir/bidir.pbrt"},
    {"pbrt-v3-scenes/veach-mis/mis.pbrt"},
    {"pbrt-v3-scenes/villa/f16-20a.pbrt"},
    {"pbrt-v3-scenes/villa/f16-20b.pbrt"},
    {"pbrt-v3-scenes/villa/f16-20c.pbrt"},
    {"pbrt-v3-scenes/villa/villa-daylight.pbrt"},
    {"pbrt-v3-scenes/villa/villa-lights-on.pbrt"},
    {"pbrt-v3-scenes/villa/villa-photons.pbrt"},
    {"pbrt-v3-scenes/volume-caustic/caustic.pbrt"},
    {"pbrt-v3-scenes/volume-caustic/f16-22a.pbrt"},
    {"pbrt-v3-scenes/volume-caustic/f16-22b.pbrt"},
    {"pbrt-v3-scenes/vw-van/vw-van.pbrt"},
    {"pbrt-v3-scenes/white-room/lights-daytime.pbrt"},
    {"pbrt-v3-scenes/white-room/lights-night.pbrt"},
    {"pbrt-v3-scenes/white-room/whiteroom-daytime.pbrt"},
    {"pbrt-v3-scenes/white-room/whiteroom-night.pbrt"},
    {"pbrt-v3-scenes/wip/glass/glass.pbrt"},
    {"pbrt-v3-scenes/yeahright/yeahright.pbrt"},
};

void AddAllCallbacks(
    std::vector<
        std::tuple<std::string, std::function<std::pair<int, std::string>()>,
                   std::pair<int, std::string>>>& output,
    int version, const std::vector<TestInput>& all_input) {
  for (const TestInput& input : all_input) {
    auto& back = output.emplace_back();
    std::get<0>(back) = input.path;
    std::get<1>(back) = [version, input]() {
      return Convert(version, input.path, input.allow_warnings);
    };
  }
}

TEST(Convert, All) {
  std::vector<
      std::tuple<std::string, std::function<std::pair<int, std::string>()>,
                 std::pair<int, std::string>>>
      state;
  AddAllCallbacks(state, /*version=*/1, pbrt_v1_inputs);
  AddAllCallbacks(state, /*version=*/3, pbrt_v3_inputs);

  std::atomic<size_t> index = 0;
  std::vector<std::thread> threads;
  for (size_t i = 0; i < std::thread::hardware_concurrency(); i++) {
    threads.emplace_back([&]() {
      for (size_t work_index = index.fetch_add(1); work_index < state.size();
           work_index = index.fetch_add(1)) {
        std::get<2>(state[work_index]) = std::get<1>(state[work_index])();
      }
    });
  }

  for (std::thread& t : threads) {
    t.join();
  }

  for (const auto& [input_path, _, result] : state) {
    SCOPED_TRACE(input_path);
    EXPECT_EQ(0, result.first);
    EXPECT_EQ("", result.second);
  }
}

}  // namespace
