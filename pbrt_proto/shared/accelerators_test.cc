#include "pbrt_proto/shared/accelerators.h"

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

TEST(RemoveGridAccelerator, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  GridAccelerator actual;
  EXPECT_TRUE(
      RemoveGridAccelerator(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveGridAccelerator, RefineImmediately) {
  bool values[] = {true};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::BOOL,
                      /*type_name=*/"",
                      /*value=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"refineimmediately", parameter}};

  GridAccelerator actual;
  EXPECT_TRUE(
      RemoveGridAccelerator(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                refineimmediately: true
              )pb"));
}

TEST(RemoveKdTreeAccelerator, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  KdTreeAccelerator actual;
  EXPECT_TRUE(
      RemoveKdTreeAccelerator(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveKdTreeAccelerator, EmptyBonus) {
  std::vector<double> values = {1.0};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::FLOAT,
                      /*type_name=*/"",
                      /*value=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"emptybonus", parameter}};

  KdTreeAccelerator actual;
  EXPECT_TRUE(
      RemoveKdTreeAccelerator(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                emptybonus: 1.0
              )pb"));
}

TEST(RemoveKdTreeAccelerator, IntersectCost) {
  std::vector<int32_t> values = {1};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::INTEGER,
                      /*type_name=*/"",
                      /*value=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"intersectcost", parameter}};

  KdTreeAccelerator actual;
  EXPECT_TRUE(
      RemoveKdTreeAccelerator(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                intersectcost: 1
              )pb"));
}

TEST(RemoveKdTreeAccelerator, MaxDepth) {
  std::vector<int32_t> values = {1};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::INTEGER,
                      /*type_name=*/"",
                      /*value=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"maxdepth", parameter}};

  KdTreeAccelerator actual;
  EXPECT_TRUE(
      RemoveKdTreeAccelerator(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                maxdepth: 1
              )pb"));
}

TEST(RemoveKdTreeAccelerator, MaxPrims) {
  std::vector<int32_t> values = {1};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::INTEGER,
                      /*type_name=*/"",
                      /*value=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"maxprims", parameter}};

  KdTreeAccelerator actual;
  EXPECT_TRUE(
      RemoveKdTreeAccelerator(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                maxprims: 1
              )pb"));
}

TEST(RemoveKdTreeAccelerator, TraversalCost) {
  std::vector<int32_t> values = {1};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::INTEGER,
                      /*type_name=*/"",
                      /*value=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"traversalcost", parameter}};

  KdTreeAccelerator actual;
  EXPECT_TRUE(
      RemoveKdTreeAccelerator(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                traversalcost: 1
              )pb"));
}

TEST(RemoveBvhAcceleratorV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  BvhAccelerator actual;
  EXPECT_TRUE(
      RemoveBvhAccelerator(parameters, /*pbrt_version=*/2, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveBvhAcceleratorV2, MaxNodePrims) {
  std::vector<int32_t> values = {1};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::INTEGER,
                      /*type_name=*/"",
                      /*value=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"maxnodeprims", parameter}};

  BvhAccelerator actual;
  EXPECT_TRUE(
      RemoveBvhAccelerator(parameters, /*pbrt_version=*/2, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                maxnodeprims: 1
              )pb"));
}

TEST(RemoveBvhAcceleratorV2, Middle) {
  std::vector<absl::string_view> values = {"middle"};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::STRING,
                      /*type_name=*/"",
                      /*value=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"splitmethod", parameter}};

  BvhAccelerator actual;
  EXPECT_TRUE(
      RemoveBvhAccelerator(parameters, /*pbrt_version=*/2, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                splitmethod: MIDDLE
              )pb"));
}

TEST(RemoveBvhAcceleratorV2, Equal) {
  std::vector<absl::string_view> values = {"equal"};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::STRING,
                      /*type_name=*/"",
                      /*value=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"splitmethod", parameter}};

  BvhAccelerator actual;
  EXPECT_TRUE(
      RemoveBvhAccelerator(parameters, /*pbrt_version=*/2, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                splitmethod: EQUAL
              )pb"));
}

TEST(RemoveBvhAcceleratorV2, Hlbvh) {
  std::vector<absl::string_view> values = {"hlbvh"};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::STRING,
                      /*type_name=*/"",
                      /*value=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"splitmethod", parameter}};

  BvhAccelerator actual;
  EXPECT_TRUE(
      RemoveBvhAccelerator(parameters, /*pbrt_version=*/2, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                splitmethod: SAH
              )pb"));
}

TEST(RemoveBvhAcceleratorV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  BvhAccelerator actual;
  EXPECT_TRUE(
      RemoveBvhAccelerator(parameters, /*pbrt_version=*/3, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveBvhAcceleratorV3, MaxNodePrims) {
  std::vector<int32_t> values = {1};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::INTEGER,
                      /*type_name=*/"",
                      /*value=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"maxnodeprims", parameter}};

  BvhAccelerator actual;
  EXPECT_TRUE(
      RemoveBvhAccelerator(parameters, /*pbrt_version=*/3, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                maxnodeprims: 1
              )pb"));
}

TEST(RemoveBvhAcceleratorV3, Middle) {
  std::vector<absl::string_view> values = {"middle"};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::STRING,
                      /*type_name=*/"",
                      /*value=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"splitmethod", parameter}};

  BvhAccelerator actual;
  EXPECT_TRUE(
      RemoveBvhAccelerator(parameters, /*pbrt_version=*/3, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                splitmethod: MIDDLE
              )pb"));
}

TEST(RemoveBvhAcceleratorV3, Equal) {
  std::vector<absl::string_view> values = {"equal"};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::STRING,
                      /*type_name=*/"",
                      /*value=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"splitmethod", parameter}};

  BvhAccelerator actual;
  EXPECT_TRUE(
      RemoveBvhAccelerator(parameters, /*pbrt_version=*/3, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                splitmethod: EQUAL
              )pb"));
}

TEST(RemoveBvhAcceleratorV3, Hlbvh) {
  std::vector<absl::string_view> values = {"hlbvh"};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::STRING,
                      /*type_name=*/"",
                      /*value=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"splitmethod", parameter}};

  BvhAccelerator actual;
  EXPECT_TRUE(
      RemoveBvhAccelerator(parameters, /*pbrt_version=*/3, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                splitmethod: HLBVH
              )pb"));
}

TEST(RemoveBvhAcceleratorV3, Unknown) {
  std::vector<absl::string_view> values = {"unknown"};
  Parameter parameter{/*directive=*/"",
                      /*type=*/ParameterType::STRING,
                      /*type_name=*/"",
                      /*value=*/absl::MakeSpan(values)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"splitmethod", parameter}};

  BvhAccelerator actual;
  EXPECT_TRUE(
      RemoveBvhAccelerator(parameters, /*pbrt_version=*/3, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                splitmethod: SAH
              )pb"));
}

}  // namespace
}  // namespace pbrt_proto
