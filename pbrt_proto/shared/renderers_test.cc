#include "pbrt_proto/shared/renderers.h"

#include <cstdint>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "absl/strings/string_view.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"
#include "pbrt_proto/testing/proto_matchers.h"

namespace pbrt_proto {
namespace {

using ::google::protobuf::EqualsProto;

TEST(RemoveAggregateTestRenderer, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  AggregateTestRenderer actual;
  EXPECT_TRUE(
      RemoveAggregateTestRenderer(parameters, /*pbrt_version=*/2, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveAggregateTestRenderer, WithData) {
  std::vector<int32_t> niters = {1};
  Parameter niters_parameter{/*directive=*/"",
                             /*type=*/ParameterType::INTEGER,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(niters)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"niters", niters_parameter}};

  AggregateTestRenderer actual;
  EXPECT_TRUE(
      RemoveAggregateTestRenderer(parameters, /*pbrt_version=*/2, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                niters: 1
              )pb"));
}

TEST(RemoveCreateProbesRenderer, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  CreateProbesRenderer actual;
  EXPECT_TRUE(
      RemoveCreateProbesRenderer(parameters, /*pbrt_version=*/2, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveCreateProbesRenderer, TooFewBounds) {
  std::vector<double> bounds = {{1.0}};
  Parameter bounds_parameter{/*directive=*/"",
                             /*type=*/ParameterType::FLOAT,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(bounds)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"bounds", bounds_parameter}};

  CreateProbesRenderer actual;
  EXPECT_TRUE(
      RemoveCreateProbesRenderer(parameters, /*pbrt_version=*/2, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
              )pb"));
}

TEST(RemoveCreateProbesRenderer, WithData) {
  std::vector<double> bounds = {{1.0, 2.0, 3.0, 4.0, 5.0, 6.0}};
  Parameter bounds_parameter{/*directive=*/"",
                             /*type=*/ParameterType::FLOAT,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(bounds)};

  bool directlighting[] = {true};
  Parameter directlighting_parameter{/*directive=*/"",
                                     /*type=*/ParameterType::BOOL,
                                     /*type_name=*/"",
                                     /*values=*/absl::MakeSpan(directlighting)};

  std::vector<absl::string_view> filename = {"file"};
  Parameter filename_parameter{/*directive=*/"",
                               /*type=*/ParameterType::STRING,
                               /*type_name=*/"",
                               /*value=*/absl::MakeSpan(filename)};

  bool indirectlighting[] = {true};
  Parameter indirectlighting_parameter{
      /*directive=*/"",
      /*type=*/ParameterType::BOOL,
      /*type_name=*/"",
      /*values=*/absl::MakeSpan(indirectlighting)};

  std::vector<int32_t> lmax = {7};
  Parameter lmax_parameter{/*directive=*/"",
                           /*type=*/ParameterType::INTEGER,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(lmax)};

  std::vector<int32_t> indirectsamples = {8};
  Parameter indirectsamples_parameter{
      /*directive=*/"",
      /*type=*/ParameterType::INTEGER,
      /*type_name=*/"",
      /*values=*/absl::MakeSpan(indirectsamples)};

  std::vector<double> samplespacing = {9.0};
  Parameter samplespacing_parameter{/*directive=*/"",
                                    /*type=*/ParameterType::FLOAT,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(samplespacing)};

  std::vector<double> time = {10.0};
  Parameter time_parameter{/*directive=*/"",
                           /*type=*/ParameterType::FLOAT,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(time)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"bounds", bounds_parameter},
      {"directlighting", directlighting_parameter},
      {"filename", filename_parameter},
      {"indirectlighting", indirectlighting_parameter},
      {"lmax", lmax_parameter},
      {"indirectsamples", indirectsamples_parameter},
      {"samplespacing", samplespacing_parameter},
      {"time", time_parameter},
  };

  CreateProbesRenderer actual;
  EXPECT_TRUE(
      RemoveCreateProbesRenderer(parameters, /*pbrt_version=*/2, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                bounds {
                  p0 { x: 1.0 y: 2.0 z: 3.0 }
                  p1 { x: 4.0 y: 5.0 z: 6.0 }
                }
                directlighting: true
                filename: "file"
                indirectlighting: true
                lmax: 7
                indirectsamples: 8
                samplespacing: 9.0
                time: 10.0
              )pb"));
}

TEST(RemoveMetropolisRenderer, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  MetropolisRenderer actual;
  EXPECT_TRUE(
      RemoveMetropolisRenderer(parameters, /*pbrt_version=*/2, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveMetropolisRenderer, WithData) {
  std::vector<double> largestepprobability = {1.0};
  Parameter largestepprobability_parameter{
      /*directive=*/"",
      /*type=*/ParameterType::FLOAT,
      /*type_name=*/"",
      /*values=*/absl::MakeSpan(largestepprobability)};

  std::vector<int32_t> samplesperpixel = {2};
  Parameter samplesperpixel_parameter{
      /*directive=*/"",
      /*type=*/ParameterType::INTEGER,
      /*type_name=*/"",
      /*values=*/absl::MakeSpan(samplesperpixel)};

  std::vector<int32_t> bootstrapsamples = {3};
  Parameter bootstrapsamples_parameter{
      /*directive=*/"",
      /*type=*/ParameterType::INTEGER,
      /*type_name=*/"",
      /*values=*/absl::MakeSpan(bootstrapsamples)};

  std::vector<int32_t> directsamples = {4};
  Parameter directsamples_parameter{/*directive=*/"",
                                    /*type=*/ParameterType::INTEGER,
                                    /*type_name=*/"",
                                    /*values=*/absl::MakeSpan(directsamples)};

  bool dodirectseparately[] = {true};
  Parameter dodirectseparately_parameter{
      /*directive=*/"",
      /*type=*/ParameterType::BOOL,
      /*type_name=*/"",
      /*value=*/absl::MakeSpan(dodirectseparately)};

  std::vector<int32_t> maxconsecutiverejects = {5};
  Parameter maxconsecutiverejects_parameter{
      /*directive=*/"",
      /*type=*/ParameterType::INTEGER,
      /*type_name=*/"",
      /*values=*/absl::MakeSpan(maxconsecutiverejects)};

  std::vector<int32_t> maxdepth = {6};
  Parameter maxdepth_parameter{/*directive=*/"",
                               /*type=*/ParameterType::INTEGER,
                               /*type_name=*/"",
                               /*values=*/absl::MakeSpan(maxdepth)};

  bool bidirectional[] = {true};
  Parameter bidirectional_parameter{/*directive=*/"",
                                    /*type=*/ParameterType::BOOL,
                                    /*type_name=*/"",
                                    /*value=*/absl::MakeSpan(bidirectional)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"largestepprobability", largestepprobability_parameter},
      {"samplesperpixel", samplesperpixel_parameter},
      {"bootstrapsamples", bootstrapsamples_parameter},
      {"directsamples", directsamples_parameter},
      {"dodirectseparately", dodirectseparately_parameter},
      {"maxconsecutiverejects", maxconsecutiverejects_parameter},
      {"maxdepth", maxdepth_parameter},
      {"bidirectional", bidirectional_parameter},
  };

  MetropolisRenderer actual;
  EXPECT_TRUE(
      RemoveMetropolisRenderer(parameters, /*pbrt_version=*/2, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                largestepprobability: 1.0
                samplesperpixel: 2
                bootstrapsamples: 3
                directsamples: 4
                dodirectseparately: true
                maxconsecutiverejects: 5
                maxdepth: 6
                bidirectional: true
              )pb"));
}

TEST(RemoveSamplerRenderer, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  SamplerRenderer actual;
  EXPECT_TRUE(
      RemoveSamplerRenderer(parameters, /*pbrt_version=*/2, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveSamplerRenderer, WithData) {
  bool visualizeobjectids[] = {true};
  Parameter visualizeobjectids_parameter{
      /*directive=*/"",
      /*type=*/ParameterType::BOOL,
      /*type_name=*/"",
      /*value=*/absl::MakeSpan(visualizeobjectids)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"visualizeobjectids", visualizeobjectids_parameter}};

  SamplerRenderer actual;
  EXPECT_TRUE(
      RemoveSamplerRenderer(parameters, /*pbrt_version=*/2, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                visualizeobjectids: true
              )pb"));
}

TEST(RemoveSurfacePointsRenderer, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  SurfacePointsRenderer actual;
  EXPECT_TRUE(
      RemoveSurfacePointsRenderer(parameters, /*pbrt_version=*/2, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveSurfacePointsRenderer, WithData) {
  std::vector<double> minsampledistance = {1.0};
  Parameter minsampledistance_parameter{
      /*directive=*/"",
      /*type=*/ParameterType::FLOAT,
      /*type_name=*/"",
      /*values=*/absl::MakeSpan(minsampledistance)};

  std::vector<absl::string_view> filename = {"file"};
  Parameter filename_parameter{/*directive=*/"",
                               /*type=*/ParameterType::STRING,
                               /*type_name=*/"",
                               /*value=*/absl::MakeSpan(filename)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"minsampledistance", minsampledistance_parameter},
      {"filename", filename_parameter}};

  SurfacePointsRenderer actual;
  EXPECT_TRUE(
      RemoveSurfacePointsRenderer(parameters, /*pbrt_version=*/2, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                minsampledistance: 1.0 filename: "file"
              )pb"));
}

}  // namespace
}  // namespace pbrt_proto
