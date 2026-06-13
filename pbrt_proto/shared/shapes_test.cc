#include "pbrt_proto/shared/shapes.h"

#include <array>
#include <cstdint>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "absl/status/status_matchers.h"
#include "absl/strings/string_view.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"
#include "pbrt_proto/testing/proto_matchers.h"

namespace pbrt_proto {
namespace {

using ::absl_testing::StatusIs;
using ::google::protobuf::EqualsProto;

TEST(RemoveConeShape, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  ConeShape actual;
  EXPECT_TRUE(RemoveConeShape(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveConeShape, WithData) {
  std::vector<double> radius = {0.1};
  Parameter radius_parameter{/*directive=*/"",
                             /*type=*/ParameterType::FLOAT,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(radius)};

  std::vector<double> height = {0.2};
  Parameter height_parameter{/*directive=*/"",
                             /*type=*/ParameterType::FLOAT,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(height)};

  std::vector<double> phimax = {0.3};
  Parameter phimax_parameter{/*directive=*/"",
                             /*type=*/ParameterType::FLOAT,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(phimax)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"radius", radius_parameter},
      {"height", height_parameter},
      {"phimax", phimax_parameter}};

  ConeShape actual;
  EXPECT_TRUE(RemoveConeShape(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                radius: 0.1 height: 0.2 phimax: 0.3
              )pb"));
}

TEST(TryRemoveCurveShape, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  CurveShape actual;
  bool called = false;
  EXPECT_TRUE(TryRemoveCurveShape(parameters, /*pbrt_version=*/3, [&]() {
                called = true;
                return &actual;
              }).ok());
  EXPECT_TRUE(called);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(TryRemoveCurveShape, InvalidDegree) {
  std::vector<int32_t> degree = {1};
  Parameter degrees_parameter{/*directive=*/"",
                              /*type=*/ParameterType::INTEGER,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(degree)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"degree", degrees_parameter}};

  CurveShape actual;
  EXPECT_TRUE(TryRemoveCurveShape(parameters, /*pbrt_version=*/3, [&]() {
                EXPECT_TRUE(false);
                return &actual;
              }).ok());
}

TEST(TryRemoveCurveShape, DegreeTwo) {
  std::vector<int32_t> degree = {2};
  Parameter degrees_parameter{/*directive=*/"",
                              /*type=*/ParameterType::INTEGER,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(degree)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"degree", degrees_parameter}};

  CurveShape actual;
  EXPECT_TRUE(TryRemoveCurveShape(parameters, /*pbrt_version=*/3, [&]() {
                return &actual;
              }).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                degree: TWO
              )pb"));
}

TEST(TryRemoveCurveShape, DegreeThree) {
  std::vector<int32_t> degree = {3};
  Parameter degrees_parameter{/*directive=*/"",
                              /*type=*/ParameterType::INTEGER,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(degree)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"degree", degrees_parameter}};

  CurveShape actual;
  EXPECT_TRUE(TryRemoveCurveShape(parameters, /*pbrt_version=*/3, [&]() {
                return &actual;
              }).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                degree: THREE
              )pb"));
}

TEST(TryRemoveCurveShape, InvalidBasis) {
  std::vector<absl::string_view> basis = {"invalid"};
  Parameter basis_parameter{/*directive=*/"",
                            /*type=*/ParameterType::STRING,
                            /*type_name=*/"",
                            /*values=*/absl::MakeSpan(basis)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"basis", basis_parameter}};

  CurveShape actual;
  EXPECT_TRUE(TryRemoveCurveShape(parameters, /*pbrt_version=*/3, [&]() {
                EXPECT_TRUE(false);
                return &actual;
              }).ok());
}

TEST(TryRemoveCurveShape, BasisBezier) {
  std::vector<absl::string_view> basis = {"bezier"};
  Parameter basis_parameter{/*directive=*/"",
                            /*type=*/ParameterType::STRING,
                            /*type_name=*/"",
                            /*values=*/absl::MakeSpan(basis)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"basis", basis_parameter}};

  CurveShape actual;
  EXPECT_TRUE(TryRemoveCurveShape(parameters, /*pbrt_version=*/3, [&]() {
                return &actual;
              }).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                basis: BEZIER
              )pb"));
}

TEST(TryRemoveCurveShape, BasisBspline) {
  std::vector<absl::string_view> basis = {"bspline"};
  Parameter basis_parameter{/*directive=*/"",
                            /*type=*/ParameterType::STRING,
                            /*type_name=*/"",
                            /*values=*/absl::MakeSpan(basis)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"basis", basis_parameter}};

  CurveShape actual;
  EXPECT_TRUE(TryRemoveCurveShape(parameters, /*pbrt_version=*/3, [&]() {
                return &actual;
              }).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                basis: BSPLINE
              )pb"));
}

TEST(TryRemoveCurveShape, InvalidType) {
  std::vector<absl::string_view> type = {"invalid"};
  Parameter type_parameter{/*directive=*/"",
                           /*type=*/ParameterType::STRING,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(type)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"type", type_parameter}};

  CurveShape actual;
  EXPECT_TRUE(TryRemoveCurveShape(parameters, /*pbrt_version=*/3, [&]() {
                return &actual;
              }).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                type: CYLINDER
              )pb"));
}

TEST(TryRemoveCurveShape, TypeCylinder) {
  std::vector<absl::string_view> type = {"cylinder"};
  Parameter type_parameter{/*directive=*/"",
                           /*type=*/ParameterType::STRING,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(type)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"type", type_parameter}};

  CurveShape actual;
  EXPECT_TRUE(TryRemoveCurveShape(parameters, /*pbrt_version=*/3, [&]() {
                return &actual;
              }).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                type: CYLINDER
              )pb"));
}

TEST(TryRemoveCurveShape, TypeFlat) {
  std::vector<absl::string_view> type = {"flat"};
  Parameter type_parameter{/*directive=*/"",
                           /*type=*/ParameterType::STRING,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(type)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"type", type_parameter}};

  CurveShape actual;
  EXPECT_TRUE(TryRemoveCurveShape(parameters, /*pbrt_version=*/3, [&]() {
                return &actual;
              }).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                type: FLAT
              )pb"));
}

TEST(TryRemoveCurveShape, TypeRibbon) {
  std::vector<absl::string_view> type = {"ribbon"};
  Parameter type_parameter{/*directive=*/"",
                           /*type=*/ParameterType::STRING,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(type)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"type", type_parameter}};

  CurveShape actual;
  EXPECT_TRUE(TryRemoveCurveShape(parameters, /*pbrt_version=*/3, [&]() {
                return &actual;
              }).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                type: RIBBON
              )pb"));
}

TEST(TryRemoveCurveShape, WithData) {
  std::vector<std::array<double, 3>> p = {{1.0, 2.0, 3.0}};
  Parameter p_parameter{/*directive=*/"",
                        /*type=*/ParameterType::POINT3,
                        /*type_name=*/"",
                        /*values=*/absl::MakeSpan(p)};

  std::vector<absl::string_view> basis = {"bezier"};
  Parameter basis_parameter{/*directive=*/"",
                            /*type=*/ParameterType::STRING,
                            /*type_name=*/"",
                            /*values=*/absl::MakeSpan(basis)};

  std::vector<absl::string_view> type = {"flat"};
  Parameter type_parameter{/*directive=*/"",
                           /*type=*/ParameterType::STRING,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(type)};

  std::vector<int32_t> degree = {2};
  Parameter degrees_parameter{/*directive=*/"",
                              /*type=*/ParameterType::INTEGER,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(degree)};

  std::vector<std::array<double, 3>> n = {{4.0, 5.0, 6.0}};
  Parameter n_parameter{/*directive=*/"",
                        /*type=*/ParameterType::NORMAL3,
                        /*type_name=*/"",
                        /*values=*/absl::MakeSpan(n)};

  std::vector<double> width = {0.1};
  Parameter width_parameter{/*directive=*/"",
                            /*type=*/ParameterType::FLOAT,
                            /*type_name=*/"",
                            /*values=*/absl::MakeSpan(width)};

  std::vector<double> width0 = {0.2};
  Parameter width0_parameter{/*directive=*/"",
                             /*type=*/ParameterType::FLOAT,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(width0)};

  std::vector<double> width1 = {0.3};
  Parameter width1_parameter{/*directive=*/"",
                             /*type=*/ParameterType::FLOAT,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(width1)};

  std::vector<int32_t> splitdepth = {10};
  Parameter splitdepth_parameter{/*directive=*/"",
                                 /*type=*/ParameterType::INTEGER,
                                 /*type_name=*/"",
                                 /*values=*/absl::MakeSpan(splitdepth)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"P", p_parameter},
      {"basis", basis_parameter},
      {"type", type_parameter},
      {"degree", degrees_parameter},
      {"N", n_parameter},
      {"width", width_parameter},
      {"width0", width0_parameter},
      {"width1", width1_parameter},
      {"splitdepth", splitdepth_parameter},
  };

  CurveShape actual;
  EXPECT_TRUE(TryRemoveCurveShape(parameters, /*pbrt_version=*/3, [&]() {
                return &actual;
              }).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                P { x: 1.0 y: 2.0 z: 3.0 }
                basis: BEZIER
                type: FLAT
                degree: TWO
                N { x: 4.0 y: 5.0 z: 6.0 }
                width: 0.1
                width0: 0.2
                width1: 0.3
                splitdepth: 10
              )pb"));
}

TEST(RemoveCylinderShape, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  CylinderShape actual;
  EXPECT_TRUE(RemoveCylinderShape(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveCylinderShape, WithData) {
  std::vector<double> radius = {0.1};
  Parameter radius_parameter{/*directive=*/"",
                             /*type=*/ParameterType::FLOAT,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(radius)};

  std::vector<double> zmin = {0.2};
  Parameter zmin_parameter{/*directive=*/"",
                           /*type=*/ParameterType::FLOAT,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(zmin)};

  std::vector<double> zmax = {0.3};
  Parameter zmax_parameter{/*directive=*/"",
                           /*type=*/ParameterType::FLOAT,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(zmax)};

  std::vector<double> phimax = {0.4};
  Parameter phimax_parameter{/*directive=*/"",
                             /*type=*/ParameterType::FLOAT,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(phimax)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"radius", radius_parameter},
      {"zmin", zmin_parameter},
      {"zmax", zmax_parameter},
      {"phimax", phimax_parameter}};

  CylinderShape actual;
  EXPECT_TRUE(RemoveCylinderShape(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(
                radius: 0.1 zmin: 0.2 zmax: 0.3 phimax: 0.4
              )pb"));
}

TEST(RemoveDiskShape, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  DiskShape actual;
  EXPECT_TRUE(RemoveDiskShape(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveDiskShape, WithData) {
  std::vector<double> innerradius = {0.1};
  Parameter innerradius_parameter{/*directive=*/"",
                                  /*type=*/ParameterType::FLOAT,
                                  /*type_name=*/"",
                                  /*values=*/absl::MakeSpan(innerradius)};

  std::vector<double> radius = {0.2};
  Parameter radius_parameter{/*directive=*/"",
                             /*type=*/ParameterType::FLOAT,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(radius)};

  std::vector<double> height = {0.3};
  Parameter height_parameter{/*directive=*/"",
                             /*type=*/ParameterType::FLOAT,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(height)};

  std::vector<double> phimax = {0.4};
  Parameter phimax_parameter{/*directive=*/"",
                             /*type=*/ParameterType::FLOAT,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(phimax)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"innerradius", innerradius_parameter},
      {"radius", radius_parameter},
      {"height", height_parameter},
      {"phimax", phimax_parameter}};

  DiskShape actual;
  EXPECT_TRUE(RemoveDiskShape(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                innerradius: 0.1
                radius: 0.2
                height: 0.3
                phimax: 0.4
              )pb"));
}

TEST(RemoveHeightFieldShape, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  HeightFieldShape actual;
  EXPECT_TRUE(
      RemoveHeightFieldShape(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveHeightFieldShape, TooBig) {
  std::vector<int32_t> nu = {2147483647};
  Parameter nu_parameter{/*directive=*/"",
                         /*type=*/ParameterType::INTEGER,
                         /*type_name=*/"",
                         /*values=*/absl::MakeSpan(nu)};

  std::vector<int32_t> nv = {2147483647};
  Parameter nv_parameter{/*directive=*/"",
                         /*type=*/ParameterType::INTEGER,
                         /*type_name=*/"",
                         /*values=*/absl::MakeSpan(nv)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"nu", nu_parameter}, {"nv", nv_parameter}};

  HeightFieldShape actual;
  EXPECT_THAT(
      RemoveHeightFieldShape(parameters, /*pbrt_version=*/1, actual),
      StatusIs(
          absl::StatusCode::kResourceExhausted,
          "Heighfield shape is too large to be stored in a 1D proto array"));
}

TEST(RemoveHeightFieldShape, WithData) {
  std::vector<int32_t> nu = {1};
  Parameter nu_parameter{/*directive=*/"",
                         /*type=*/ParameterType::INTEGER,
                         /*type_name=*/"",
                         /*values=*/absl::MakeSpan(nu)};

  std::vector<int32_t> nv = {1};
  Parameter nv_parameter{/*directive=*/"",
                         /*type=*/ParameterType::INTEGER,
                         /*type_name=*/"",
                         /*values=*/absl::MakeSpan(nv)};

  std::vector<double> pz = {1.0};
  Parameter pz_parameter{/*directive=*/"",
                         /*type=*/ParameterType::FLOAT,
                         /*type_name=*/"",
                         /*values=*/absl::MakeSpan(pz)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"nu", nu_parameter}, {"nv", nv_parameter}, {"Pz", pz_parameter}};

  HeightFieldShape actual;
  EXPECT_TRUE(
      RemoveHeightFieldShape(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                nu: 1 nv: 1 Pz: 1.0
              )pb"));
}

TEST(RemoveHyperboloidShape, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  HyperboloidShape actual;
  EXPECT_TRUE(
      RemoveHyperboloidShape(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveHyperboloidShape, WithData) {
  std::vector<std::array<double, 3>> p1 = {{1.0, 2.0, 3.0}};
  Parameter p1_parameter{/*directive=*/"",
                         /*type=*/ParameterType::POINT3,
                         /*type_name=*/"",
                         /*values=*/absl::MakeSpan(p1)};

  std::vector<std::array<double, 3>> p2 = {{4.0, 5.0, 6.0}};
  Parameter p2_parameter{/*directive=*/"",
                         /*type=*/ParameterType::POINT3,
                         /*type_name=*/"",
                         /*values=*/absl::MakeSpan(p2)};

  std::vector<double> phimax = {0.7};
  Parameter phimax_parameter{/*directive=*/"",
                             /*type=*/ParameterType::FLOAT,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(phimax)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"p1", p1_parameter}, {"p2", p2_parameter}, {"phimax", phimax_parameter}};

  HyperboloidShape actual;
  EXPECT_TRUE(
      RemoveHyperboloidShape(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                p1 { x: 1.0 y: 2.0 z: 3.0 }
                p2 { x: 4.0 y: 5.0 z: 6.0 }
                phimax: 0.7
              )pb"));
}

TEST(RemoveLoopSubdivShapeV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  LoopSubdivShape actual;
  EXPECT_TRUE(
      RemoveLoopSubdivShape(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveLoopSubdivShapeV1, WithData) {
  std::vector<std::array<double, 3>> p = {
      {1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0}};
  Parameter p_parameter{/*directive=*/"",
                        /*type=*/ParameterType::POINT3,
                        /*type_name=*/"",
                        /*values=*/absl::MakeSpan(p)};

  std::vector<int32_t> indices = {0, 1, 2};
  Parameter indices_parameter{/*directive=*/"",
                              /*type=*/ParameterType::INTEGER,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(indices)};

  std::vector<int32_t> levels = {10};
  Parameter levels_parameter{/*directive=*/"",
                             /*type=*/ParameterType::INTEGER,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(levels)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"P", p_parameter},
      {"indices", indices_parameter},
      {"nlevels", levels_parameter}};

  LoopSubdivShape actual;
  EXPECT_TRUE(
      RemoveLoopSubdivShape(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                indices { v0: 0 v1: 1 v2: 2 }
                P { x: 1.0 y: 2.0 z: 3.0 }
                P { x: 4.0 y: 5.0 z: 6.0 }
                P { x: 7.0 y: 8.0 z: 9.0 }
                levels: 10
              )pb"));
}

TEST(RemoveLoopSubdivShapeV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  LoopSubdivShape actual;
  EXPECT_TRUE(
      RemoveLoopSubdivShape(parameters, /*pbrt_version=*/2, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveLoopSubdivShapeV2, WithData) {
  std::vector<std::array<double, 3>> p = {
      {1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0}};
  Parameter p_parameter{/*directive=*/"",
                        /*type=*/ParameterType::POINT3,
                        /*type_name=*/"",
                        /*values=*/absl::MakeSpan(p)};

  std::vector<int32_t> indices = {0, 1, 2};
  Parameter indices_parameter{/*directive=*/"",
                              /*type=*/ParameterType::INTEGER,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(indices)};

  std::vector<int32_t> levels = {10};
  Parameter levels_parameter{/*directive=*/"",
                             /*type=*/ParameterType::INTEGER,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(levels)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"P", p_parameter},
      {"indices", indices_parameter},
      {"nlevels", levels_parameter}};

  LoopSubdivShape actual;
  EXPECT_TRUE(
      RemoveLoopSubdivShape(parameters, /*pbrt_version=*/2, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                indices { v0: 0 v1: 1 v2: 2 }
                P { x: 1.0 y: 2.0 z: 3.0 }
                P { x: 4.0 y: 5.0 z: 6.0 }
                P { x: 7.0 y: 8.0 z: 9.0 }
                levels: 10
              )pb"));
}

TEST(RemoveLoopSubdivShapeV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  LoopSubdivShape actual;
  EXPECT_TRUE(
      RemoveLoopSubdivShape(parameters, /*pbrt_version=*/3, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveLoopSubdivShapeV3, WithData) {
  std::vector<std::array<double, 3>> p = {
      {1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0}};
  Parameter p_parameter{/*directive=*/"",
                        /*type=*/ParameterType::POINT3,
                        /*type_name=*/"",
                        /*values=*/absl::MakeSpan(p)};

  std::vector<int32_t> indices = {0, 1, 2};
  Parameter indices_parameter{/*directive=*/"",
                              /*type=*/ParameterType::INTEGER,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(indices)};

  std::vector<int32_t> levels = {10};
  Parameter levels_parameter{/*directive=*/"",
                             /*type=*/ParameterType::INTEGER,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(levels)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"P", p_parameter},
      {"indices", indices_parameter},
      {"levels", levels_parameter}};

  LoopSubdivShape actual;
  EXPECT_TRUE(
      RemoveLoopSubdivShape(parameters, /*pbrt_version=*/3, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                indices { v0: 0 v1: 1 v2: 2 }
                P { x: 1.0 y: 2.0 z: 3.0 }
                P { x: 4.0 y: 5.0 z: 6.0 }
                P { x: 7.0 y: 8.0 z: 9.0 }
                levels: 10
              )pb"));
}

TEST(RemoveNurbsShape, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  NurbsShape actual;
  EXPECT_TRUE(RemoveNurbsShape(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveNurbsShape, WithData) {
  std::vector<int32_t> nu = {1};
  Parameter nu_parameter{/*directive=*/"",
                         /*type=*/ParameterType::INTEGER,
                         /*type_name=*/"",
                         /*values=*/absl::MakeSpan(nu)};

  std::vector<int32_t> nv = {2};
  Parameter nv_parameter{/*directive=*/"",
                         /*type=*/ParameterType::INTEGER,
                         /*type_name=*/"",
                         /*values=*/absl::MakeSpan(nv)};

  std::vector<int32_t> uorder = {3};
  Parameter uorder_parameter{/*directive=*/"",
                             /*type=*/ParameterType::INTEGER,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(uorder)};

  std::vector<int32_t> vorder = {4};
  Parameter vorder_parameter{/*directive=*/"",
                             /*type=*/ParameterType::INTEGER,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(vorder)};

  std::vector<double> uknots = {5.0};
  Parameter uknots_parameter{/*directive=*/"",
                             /*type=*/ParameterType::FLOAT,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(uknots)};

  std::vector<double> vknots = {6.0};
  Parameter vknots_parameter{/*directive=*/"",
                             /*type=*/ParameterType::FLOAT,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(vknots)};

  std::vector<double> u0 = {7.0};
  Parameter u0_parameter{/*directive=*/"",
                         /*type=*/ParameterType::FLOAT,
                         /*type_name=*/"",
                         /*values=*/absl::MakeSpan(u0)};

  std::vector<double> v0 = {8.0};
  Parameter v0_parameter{/*directive=*/"",
                         /*type=*/ParameterType::FLOAT,
                         /*type_name=*/"",
                         /*values=*/absl::MakeSpan(v0)};

  std::vector<double> u1 = {9.0};
  Parameter u1_parameter{/*directive=*/"",
                         /*type=*/ParameterType::FLOAT,
                         /*type_name=*/"",
                         /*values=*/absl::MakeSpan(u1)};

  std::vector<double> v1 = {10.0};
  Parameter v1_parameter{/*directive=*/"",
                         /*type=*/ParameterType::FLOAT,
                         /*type_name=*/"",
                         /*values=*/absl::MakeSpan(v1)};

  std::vector<std::array<double, 3>> p = {{11.0, 12.0, 13.0}};
  Parameter p_parameter{/*directive=*/"",
                        /*type=*/ParameterType::POINT3,
                        /*type_name=*/"",
                        /*values=*/absl::MakeSpan(p)};

  std::vector<double> pw = {14.0, 15.0, 16.0, 17.0};
  Parameter pw_parameter{/*directive=*/"",
                         /*type=*/ParameterType::FLOAT,
                         /*type_name=*/"",
                         /*values=*/absl::MakeSpan(pw)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"nu", nu_parameter},         {"nv", nv_parameter},
      {"uorder", uorder_parameter}, {"vorder", vorder_parameter},
      {"uknots", uknots_parameter}, {"vknots", vknots_parameter},
      {"u0", u0_parameter},         {"v0", v0_parameter},
      {"u1", u1_parameter},         {"v1", v1_parameter},
      {"P", p_parameter},           {"Pw", pw_parameter}};

  NurbsShape actual;
  EXPECT_TRUE(RemoveNurbsShape(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                nu: 1
                nv: 2
                uorder: 3
                vorder: 4
                uknots: 5.0
                vknots: 6.0
                u0: 7.0
                v0: 8.0
                u1: 9.0
                v1: 10.0
                P { x: 11.0 y: 12.0 z: 13.0 }
                Pw {
                  p { x: 14.0 y: 15.0 z: 16.0 }
                  weight: 17.0
                }
              )pb"));
}

TEST(RemoveParaboloidShape, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  ParaboloidShape actual;
  EXPECT_TRUE(
      RemoveParaboloidShape(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveParaboloidShape, WithData) {
  std::vector<double> radius = {0.1};
  Parameter radius_parameter{/*directive=*/"",
                             /*type=*/ParameterType::FLOAT,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(radius)};

  std::vector<double> zmin = {0.2};
  Parameter zmin_parameter{/*directive=*/"",
                           /*type=*/ParameterType::FLOAT,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(zmin)};

  std::vector<double> zmax = {0.3};
  Parameter zmax_parameter{/*directive=*/"",
                           /*type=*/ParameterType::FLOAT,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(zmax)};

  std::vector<double> phimax = {0.4};
  Parameter phimax_parameter{/*directive=*/"",
                             /*type=*/ParameterType::FLOAT,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(phimax)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"radius", radius_parameter},
      {"zmin", zmin_parameter},
      {"zmax", zmax_parameter},
      {"phimax", phimax_parameter}};

  ParaboloidShape actual;
  EXPECT_TRUE(
      RemoveParaboloidShape(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(
                radius: 0.1 zmin: 0.2 zmax: 0.3 phimax: 0.4
              )pb"));
}

TEST(RemovePlyMeshShape, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  PlyMeshShape actual;
  EXPECT_TRUE(RemovePlyMeshShape(parameters, /*pbrt_version=*/3, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemovePlyMeshShape, WithData) {
  std::vector<absl::string_view> filename = {"file"};
  Parameter filename_parameter{/*directive=*/"",
                               /*type=*/ParameterType::STRING,
                               /*type_name=*/"",
                               /*values=*/absl::MakeSpan(filename)};

  std::vector<double> alpha = {1.0};
  Parameter alpha_parameter{/*directive=*/"",
                            /*type=*/ParameterType::FLOAT,
                            /*type_name=*/"",
                            /*values=*/absl::MakeSpan(alpha)};

  std::vector<double> shadowalpha = {2.0};
  Parameter shadowalpha_parameter{/*directive=*/"",
                                  /*type=*/ParameterType::FLOAT,
                                  /*type_name=*/"",
                                  /*values=*/absl::MakeSpan(shadowalpha)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"filename", filename_parameter},
      {"alpha", alpha_parameter},
      {"shadowalpha", shadowalpha_parameter}};

  PlyMeshShape actual;
  EXPECT_TRUE(RemovePlyMeshShape(parameters, /*pbrt_version=*/3, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                filename: "file"
                alpha { float_value: 1.0 }
                shadowalpha { float_value: 2.0 }
              )pb"));
}

TEST(RemoveSphereShape, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  SphereShape actual;
  EXPECT_TRUE(RemoveSphereShape(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveSphereShape, WithData) {
  std::vector<double> radius = {0.1};
  Parameter radius_parameter{/*directive=*/"",
                             /*type=*/ParameterType::FLOAT,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(radius)};

  std::vector<double> zmin = {0.2};
  Parameter zmin_parameter{/*directive=*/"",
                           /*type=*/ParameterType::FLOAT,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(zmin)};

  std::vector<double> zmax = {0.3};
  Parameter zmax_parameter{/*directive=*/"",
                           /*type=*/ParameterType::FLOAT,
                           /*type_name=*/"",
                           /*values=*/absl::MakeSpan(zmax)};

  std::vector<double> phimax = {0.4};
  Parameter phimax_parameter{/*directive=*/"",
                             /*type=*/ParameterType::FLOAT,
                             /*type_name=*/"",
                             /*values=*/absl::MakeSpan(phimax)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"radius", radius_parameter},
      {"zmin", zmin_parameter},
      {"zmax", zmax_parameter},
      {"phimax", phimax_parameter}};

  SphereShape actual;
  EXPECT_TRUE(RemoveSphereShape(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual,
              EqualsProto(R"pb(
                radius: 0.1 zmin: 0.2 zmax: 0.3 phimax: 0.4
              )pb"));
}

TEST(RemoveTriangleMeshShapeV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  TriangleMeshShape actual;
  EXPECT_TRUE(
      RemoveTriangleMeshShape(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveTriangleMeshShapeV1, WithData) {
  std::vector<std::array<double, 3>> p = {{1.0, 2.0, 3.0}};
  Parameter p_parameter{/*directive=*/"",
                        /*type=*/ParameterType::POINT3,
                        /*type_name=*/"",
                        /*values=*/absl::MakeSpan(p)};

  std::vector<int32_t> indices = {4, 5, 6};
  Parameter indices_parameter{/*directive=*/"",
                              /*type=*/ParameterType::INTEGER,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(indices)};

  std::vector<std::array<double, 3>> n = {{7.0, 8.0, 9.0}};
  Parameter n_parameter{/*directive=*/"",
                        /*type=*/ParameterType::NORMAL3,
                        /*type_name=*/"",
                        /*values=*/absl::MakeSpan(n)};

  std::vector<std::array<double, 3>> s = {{10.0, 11.0, 12.0}};
  Parameter s_parameter{/*directive=*/"",
                        /*type=*/ParameterType::VECTOR3,
                        /*type_name=*/"",
                        /*values=*/absl::MakeSpan(s)};

  std::vector<double> uv = {13.0, 14.0};
  Parameter uv_parameter{/*directive=*/"",
                         /*type=*/ParameterType::FLOAT,
                         /*type_name=*/"",
                         /*values=*/absl::MakeSpan(uv)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"P", p_parameter},
      {"indices", indices_parameter},
      {"N", n_parameter},
      {"S", s_parameter},
      {"uv", uv_parameter}};

  TriangleMeshShape actual;
  EXPECT_TRUE(
      RemoveTriangleMeshShape(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                P { x: 1.0 y: 2.0 z: 3.0 }
                indices { v0: 4 v1: 5 v2: 6 }
                N { x: 7.0 y: 8.0 z: 9.0 }
                S { x: 10.0 y: 11.0 z: 12.0 }
                uv { u: 13.0 v: 14.0 }
              )pb"));
}

TEST(RemoveTriangleMeshShapeV1, St) {
  std::vector<double> st = {13.0, 14.0};
  Parameter st_parameter{/*directive=*/"",
                         /*type=*/ParameterType::FLOAT,
                         /*type_name=*/"",
                         /*values=*/absl::MakeSpan(st)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"st", st_parameter}};

  TriangleMeshShape actual;
  EXPECT_TRUE(
      RemoveTriangleMeshShape(parameters, /*pbrt_version=*/1, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                uv { u: 13.0 v: 14.0 }
              )pb"));
}

TEST(RemoveTriangleMeshShapeV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  TriangleMeshShape actual;
  EXPECT_TRUE(
      RemoveTriangleMeshShape(parameters, /*pbrt_version=*/2, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveTriangleMeshShapeV2, WithData) {
  std::vector<std::array<double, 3>> p = {{1.0, 2.0, 3.0}};
  Parameter p_parameter{/*directive=*/"",
                        /*type=*/ParameterType::POINT3,
                        /*type_name=*/"",
                        /*values=*/absl::MakeSpan(p)};

  std::vector<int32_t> indices = {4, 5, 6};
  Parameter indices_parameter{/*directive=*/"",
                              /*type=*/ParameterType::INTEGER,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(indices)};

  std::vector<std::array<double, 3>> n = {{7.0, 8.0, 9.0}};
  Parameter n_parameter{/*directive=*/"",
                        /*type=*/ParameterType::NORMAL3,
                        /*type_name=*/"",
                        /*values=*/absl::MakeSpan(n)};

  std::vector<std::array<double, 3>> s = {{10.0, 11.0, 12.0}};
  Parameter s_parameter{/*directive=*/"",
                        /*type=*/ParameterType::VECTOR3,
                        /*type_name=*/"",
                        /*values=*/absl::MakeSpan(s)};

  std::vector<double> uv = {13.0, 14.0};
  Parameter uv_parameter{/*directive=*/"",
                         /*type=*/ParameterType::FLOAT,
                         /*type_name=*/"",
                         /*values=*/absl::MakeSpan(uv)};

  bool discarddegenerateuvs[] = {true};
  Parameter discarddegenerateuvs_parameter{
      /*directive=*/"",
      /*type=*/ParameterType::BOOL,
      /*type_name=*/"",
      /*values=*/absl::MakeSpan(discarddegenerateuvs)};

  std::vector<double> alpha = {1.0};
  Parameter alpha_parameter{/*directive=*/"",
                            /*type=*/ParameterType::FLOAT,
                            /*type_name=*/"",
                            /*values=*/absl::MakeSpan(alpha)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"P", p_parameter},
      {"indices", indices_parameter},
      {"N", n_parameter},
      {"S", s_parameter},
      {"uv", uv_parameter},
      {"discarddegenerateUVs", discarddegenerateuvs_parameter},
      {"alpha", alpha_parameter}};

  TriangleMeshShape actual;
  EXPECT_TRUE(
      RemoveTriangleMeshShape(parameters, /*pbrt_version=*/2, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                P { x: 1.0 y: 2.0 z: 3.0 }
                indices { v0: 4 v1: 5 v2: 6 }
                N { x: 7.0 y: 8.0 z: 9.0 }
                S { x: 10.0 y: 11.0 z: 12.0 }
                uv { u: 13.0 v: 14.0 }
                discarddegenerateUVs: true
                alpha { float_value: 1.0 }
              )pb"));
}

TEST(RemoveTriangleMeshShapeV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  TriangleMeshShape actual;
  EXPECT_TRUE(
      RemoveTriangleMeshShape(parameters, /*pbrt_version=*/3, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveTriangleMeshShapeV3, WithData) {
  std::vector<std::array<double, 3>> p = {{1.0, 2.0, 3.0}};
  Parameter p_parameter{/*directive=*/"",
                        /*type=*/ParameterType::POINT3,
                        /*type_name=*/"",
                        /*values=*/absl::MakeSpan(p)};

  std::vector<int32_t> indices = {4, 5, 6};
  Parameter indices_parameter{/*directive=*/"",
                              /*type=*/ParameterType::INTEGER,
                              /*type_name=*/"",
                              /*values=*/absl::MakeSpan(indices)};

  std::vector<std::array<double, 3>> n = {{7.0, 8.0, 9.0}};
  Parameter n_parameter{/*directive=*/"",
                        /*type=*/ParameterType::NORMAL3,
                        /*type_name=*/"",
                        /*values=*/absl::MakeSpan(n)};

  std::vector<std::array<double, 3>> s = {{10.0, 11.0, 12.0}};
  Parameter s_parameter{/*directive=*/"",
                        /*type=*/ParameterType::VECTOR3,
                        /*type_name=*/"",
                        /*values=*/absl::MakeSpan(s)};

  std::vector<double> uv = {13.0, 14.0};
  Parameter uv_parameter{/*directive=*/"",
                         /*type=*/ParameterType::FLOAT,
                         /*type_name=*/"",
                         /*values=*/absl::MakeSpan(uv)};

  std::vector<double> alpha = {1.0};
  Parameter alpha_parameter{/*directive=*/"",
                            /*type=*/ParameterType::FLOAT,
                            /*type_name=*/"",
                            /*values=*/absl::MakeSpan(alpha)};

  std::vector<int32_t> faceindices = {-1};
  Parameter faceindices_parameter{/*directive=*/"",
                                  /*type=*/ParameterType::INTEGER,
                                  /*type_name=*/"",
                                  /*values=*/absl::MakeSpan(faceindices)};

  std::vector<double> shadowalpha = {2.0};
  Parameter shadowalpha_parameter{/*directive=*/"",
                                  /*type=*/ParameterType::FLOAT,
                                  /*type_name=*/"",
                                  /*values=*/absl::MakeSpan(shadowalpha)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"P", p_parameter},
      {"indices", indices_parameter},
      {"N", n_parameter},
      {"S", s_parameter},
      {"uv", uv_parameter},
      {"alpha", alpha_parameter},
      {"faceIndices", faceindices_parameter},
      {"shadowalpha", shadowalpha_parameter}};

  TriangleMeshShape actual;
  EXPECT_TRUE(
      RemoveTriangleMeshShape(parameters, /*pbrt_version=*/3, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                P { x: 1.0 y: 2.0 z: 3.0 }
                indices { v0: 4 v1: 5 v2: 6 }
                N { x: 7.0 y: 8.0 z: 9.0 }
                S { x: 10.0 y: 11.0 z: 12.0 }
                uv { u: 13.0 v: 14.0 }
                alpha { float_value: 1.0 }
                faceIndices: -1
                shadowalpha { float_value: 2.0 }
              )pb"));
}

}  // namespace
}  // namespace pbrt_proto
