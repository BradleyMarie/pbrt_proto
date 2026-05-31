#include "pbrt_proto/shared/shapes.h"

#include <array>
#include <cstdint>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "absl/status/status_matchers.h"
#include "absl/strings/string_view.h"
#include "googlemock/include/gmock/gmock.h"
#include "googletest/include/gtest/gtest.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"
#include "pbrt_proto/testing/proto_matchers.h"

namespace pbrt_proto {
namespace {

using ::absl_testing::StatusIs;
using ::google::protobuf::EqualsProto;

TEST(RemoveConeShapeV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  ConeShape actual;
  RemoveConeShapeV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveConeShapeV1, WithData) {
  std::vector<double> radius = {0.1};
  Parameter radius_parameter{.directive = "",
                             .type = ParameterType::FLOAT,
                             .type_name = "",
                             .values = absl::MakeSpan(radius)};

  std::vector<double> height = {0.2};
  Parameter height_parameter{.directive = "",
                             .type = ParameterType::FLOAT,
                             .type_name = "",
                             .values = absl::MakeSpan(height)};

  std::vector<double> phimax = {0.3};
  Parameter phimax_parameter{.directive = "",
                             .type = ParameterType::FLOAT,
                             .type_name = "",
                             .values = absl::MakeSpan(phimax)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"radius", radius_parameter},
      {"height", height_parameter},
      {"phimax", phimax_parameter}};

  ConeShape actual;
  RemoveConeShapeV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                radius: 0.1 height: 0.2 phimax: 0.3
              )pb"));
}

TEST(RemoveCylinderShapeV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  CylinderShape actual;
  RemoveCylinderShapeV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveCylinderShapeV1, WithData) {
  std::vector<double> radius = {0.1};
  Parameter radius_parameter{.directive = "",
                             .type = ParameterType::FLOAT,
                             .type_name = "",
                             .values = absl::MakeSpan(radius)};

  std::vector<double> zmin = {0.2};
  Parameter zmin_parameter{.directive = "",
                           .type = ParameterType::FLOAT,
                           .type_name = "",
                           .values = absl::MakeSpan(zmin)};

  std::vector<double> zmax = {0.3};
  Parameter zmax_parameter{.directive = "",
                           .type = ParameterType::FLOAT,
                           .type_name = "",
                           .values = absl::MakeSpan(zmax)};

  std::vector<double> phimax = {0.4};
  Parameter phimax_parameter{.directive = "",
                             .type = ParameterType::FLOAT,
                             .type_name = "",
                             .values = absl::MakeSpan(phimax)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"radius", radius_parameter},
      {"zmin", zmin_parameter},
      {"zmax", zmax_parameter},
      {"phimax", phimax_parameter}};

  CylinderShape actual;
  RemoveCylinderShapeV1(parameters, actual);
  EXPECT_THAT(actual,
              EqualsProto(R"pb(
                radius: 0.1 zmin: 0.2 zmax: 0.3 phimax: 0.4
              )pb"));
}

TEST(RemoveDiskShapeV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  DiskShape actual;
  RemoveDiskShapeV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveDiskShapeV1, WithData) {
  std::vector<double> innerradius = {0.1};
  Parameter innerradius_parameter{.directive = "",
                                  .type = ParameterType::FLOAT,
                                  .type_name = "",
                                  .values = absl::MakeSpan(innerradius)};

  std::vector<double> radius = {0.2};
  Parameter radius_parameter{.directive = "",
                             .type = ParameterType::FLOAT,
                             .type_name = "",
                             .values = absl::MakeSpan(radius)};

  std::vector<double> height = {0.3};
  Parameter height_parameter{.directive = "",
                             .type = ParameterType::FLOAT,
                             .type_name = "",
                             .values = absl::MakeSpan(height)};

  std::vector<double> phimax = {0.4};
  Parameter phimax_parameter{.directive = "",
                             .type = ParameterType::FLOAT,
                             .type_name = "",
                             .values = absl::MakeSpan(phimax)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"innerradius", innerradius_parameter},
      {"radius", radius_parameter},
      {"height", height_parameter},
      {"phimax", phimax_parameter}};

  DiskShape actual;
  RemoveDiskShapeV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                innerradius: 0.1
                radius: 0.2
                height: 0.3
                phimax: 0.4
              )pb"));
}

TEST(RemoveHeightFieldShapeV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  HeightFieldShape actual;
  EXPECT_TRUE(RemoveHeightFieldShapeV1(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveHeightFieldShapeV1, TooBig) {
  std::vector<int32_t> nu = {2147483647};
  Parameter nu_parameter{.directive = "",
                         .type = ParameterType::INTEGER,
                         .type_name = "",
                         .values = absl::MakeSpan(nu)};

  std::vector<int32_t> nv = {2147483647};
  Parameter nv_parameter{.directive = "",
                         .type = ParameterType::INTEGER,
                         .type_name = "",
                         .values = absl::MakeSpan(nv)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"nu", nu_parameter}, {"nv", nv_parameter}};

  HeightFieldShape actual;
  EXPECT_THAT(
      RemoveHeightFieldShapeV1(parameters, actual),
      StatusIs(
          absl::StatusCode::kInvalidArgument,
          "Heighfield shape is too large to be stored in a 1D proto array"));
}

TEST(RemoveHeightFieldShapeV1, WithData) {
  std::vector<int32_t> nu = {1};
  Parameter nu_parameter{.directive = "",
                         .type = ParameterType::INTEGER,
                         .type_name = "",
                         .values = absl::MakeSpan(nu)};

  std::vector<int32_t> nv = {1};
  Parameter nv_parameter{.directive = "",
                         .type = ParameterType::INTEGER,
                         .type_name = "",
                         .values = absl::MakeSpan(nv)};

  std::vector<double> pz = {1.0};
  Parameter pz_parameter{.directive = "",
                         .type = ParameterType::FLOAT,
                         .type_name = "",
                         .values = absl::MakeSpan(pz)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"nu", nu_parameter}, {"nv", nv_parameter}, {"Pz", pz_parameter}};

  HeightFieldShape actual;
  EXPECT_TRUE(RemoveHeightFieldShapeV1(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                nu: 1 nv: 1 Pz: 1.0
              )pb"));
}

TEST(RemoveHyperboloidShapeV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  HyperboloidShape actual;
  RemoveHyperboloidShapeV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveHyperboloidShapeV1, WithData) {
  std::vector<std::array<double, 3>> p1 = {{1.0, 2.0, 3.0}};
  Parameter p1_parameter{.directive = "",
                         .type = ParameterType::POINT3,
                         .type_name = "",
                         .values = absl::MakeSpan(p1)};

  std::vector<std::array<double, 3>> p2 = {{4.0, 5.0, 6.0}};
  Parameter p2_parameter{.directive = "",
                         .type = ParameterType::POINT3,
                         .type_name = "",
                         .values = absl::MakeSpan(p2)};

  std::vector<double> phimax = {0.7};
  Parameter phimax_parameter{.directive = "",
                             .type = ParameterType::FLOAT,
                             .type_name = "",
                             .values = absl::MakeSpan(phimax)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"p1", p1_parameter}, {"p2", p2_parameter}, {"phimax", phimax_parameter}};

  HyperboloidShape actual;
  RemoveHyperboloidShapeV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                p1 { x: 1.0 y: 2.0 z: 3.0 }
                p2 { x: 4.0 y: 5.0 z: 6.0 }
                phimax: 0.7
              )pb"));
}

TEST(RemoveLoopSubdivShapeV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  LoopSubdivShape actual;
  EXPECT_TRUE(RemoveLoopSubdivShapeV1(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveLoopSubdivShapeV1, WithData) {
  std::vector<std::array<double, 3>> p = {
      {1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0}};
  Parameter p_parameter{.directive = "",
                        .type = ParameterType::POINT3,
                        .type_name = "",
                        .values = absl::MakeSpan(p)};

  std::vector<int32_t> indices = {0, 1, 2};
  Parameter indices_parameter{.directive = "",
                              .type = ParameterType::INTEGER,
                              .type_name = "",
                              .values = absl::MakeSpan(indices)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"P", p_parameter}, {"indices", indices_parameter}};

  LoopSubdivShape actual;
  EXPECT_TRUE(RemoveLoopSubdivShapeV1(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                indices { v0: 0 v1: 1 v2: 2 }
                P { x: 1.0 y: 2.0 z: 3.0 }
                P { x: 4.0 y: 5.0 z: 6.0 }
                P { x: 7.0 y: 8.0 z: 9.0 }
              )pb"));
}

TEST(RemoveNurbsShapeV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  NurbsShape actual;
  EXPECT_TRUE(RemoveNurbsShapeV1(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveNurbsShapeV1, WithData) {
  std::vector<int32_t> nu = {1};
  Parameter nu_parameter{.directive = "",
                         .type = ParameterType::INTEGER,
                         .type_name = "",
                         .values = absl::MakeSpan(nu)};

  std::vector<int32_t> nv = {2};
  Parameter nv_parameter{.directive = "",
                         .type = ParameterType::INTEGER,
                         .type_name = "",
                         .values = absl::MakeSpan(nv)};

  std::vector<int32_t> uorder = {3};
  Parameter uorder_parameter{.directive = "",
                             .type = ParameterType::INTEGER,
                             .type_name = "",
                             .values = absl::MakeSpan(uorder)};

  std::vector<int32_t> vorder = {4};
  Parameter vorder_parameter{.directive = "",
                             .type = ParameterType::INTEGER,
                             .type_name = "",
                             .values = absl::MakeSpan(vorder)};

  std::vector<double> uknots = {5.0};
  Parameter uknots_parameter{.directive = "",
                             .type = ParameterType::FLOAT,
                             .type_name = "",
                             .values = absl::MakeSpan(uknots)};

  std::vector<double> vknots = {6.0};
  Parameter vknots_parameter{.directive = "",
                             .type = ParameterType::FLOAT,
                             .type_name = "",
                             .values = absl::MakeSpan(vknots)};

  std::vector<double> u0 = {7.0};
  Parameter u0_parameter{.directive = "",
                         .type = ParameterType::FLOAT,
                         .type_name = "",
                         .values = absl::MakeSpan(u0)};

  std::vector<double> v0 = {8.0};
  Parameter v0_parameter{.directive = "",
                         .type = ParameterType::FLOAT,
                         .type_name = "",
                         .values = absl::MakeSpan(v0)};

  std::vector<double> u1 = {9.0};
  Parameter u1_parameter{.directive = "",
                         .type = ParameterType::FLOAT,
                         .type_name = "",
                         .values = absl::MakeSpan(u1)};

  std::vector<double> v1 = {10.0};
  Parameter v1_parameter{.directive = "",
                         .type = ParameterType::FLOAT,
                         .type_name = "",
                         .values = absl::MakeSpan(v1)};

  std::vector<std::array<double, 3>> p = {{11.0, 12.0, 13.0}};
  Parameter p_parameter{.directive = "",
                        .type = ParameterType::POINT3,
                        .type_name = "",
                        .values = absl::MakeSpan(p)};

  std::vector<double> pw = {14.0, 15.0, 16.0, 17.0};
  Parameter pw_parameter{.directive = "",
                         .type = ParameterType::FLOAT,
                         .type_name = "",
                         .values = absl::MakeSpan(pw)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"nu", nu_parameter},         {"nv", nv_parameter},
      {"uorder", uorder_parameter}, {"vorder", vorder_parameter},
      {"uknots", uknots_parameter}, {"vknots", vknots_parameter},
      {"u0", u0_parameter},         {"v0", v0_parameter},
      {"u1", u1_parameter},         {"v1", v1_parameter},
      {"P", p_parameter},           {"Pw", pw_parameter}};

  NurbsShape actual;
  EXPECT_TRUE(RemoveNurbsShapeV1(parameters, actual).ok());
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

TEST(RemoveParaboloidShapeV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  ParaboloidShape actual;
  RemoveParaboloidShapeV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveParaboloidShapeV1, WithData) {
  std::vector<double> radius = {0.1};
  Parameter radius_parameter{.directive = "",
                             .type = ParameterType::FLOAT,
                             .type_name = "",
                             .values = absl::MakeSpan(radius)};

  std::vector<double> zmin = {0.2};
  Parameter zmin_parameter{.directive = "",
                           .type = ParameterType::FLOAT,
                           .type_name = "",
                           .values = absl::MakeSpan(zmin)};

  std::vector<double> zmax = {0.3};
  Parameter zmax_parameter{.directive = "",
                           .type = ParameterType::FLOAT,
                           .type_name = "",
                           .values = absl::MakeSpan(zmax)};

  std::vector<double> phimax = {0.4};
  Parameter phimax_parameter{.directive = "",
                             .type = ParameterType::FLOAT,
                             .type_name = "",
                             .values = absl::MakeSpan(phimax)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"radius", radius_parameter},
      {"zmin", zmin_parameter},
      {"zmax", zmax_parameter},
      {"phimax", phimax_parameter}};

  ParaboloidShape actual;
  RemoveParaboloidShapeV1(parameters, actual);
  EXPECT_THAT(actual,
              EqualsProto(R"pb(
                radius: 0.1 zmin: 0.2 zmax: 0.3 phimax: 0.4
              )pb"));
}

TEST(RemoveSphereShapeV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  SphereShape actual;
  RemoveSphereShapeV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveSphereShapeV1, WithData) {
  std::vector<double> radius = {0.1};
  Parameter radius_parameter{.directive = "",
                             .type = ParameterType::FLOAT,
                             .type_name = "",
                             .values = absl::MakeSpan(radius)};

  std::vector<double> zmin = {0.2};
  Parameter zmin_parameter{.directive = "",
                           .type = ParameterType::FLOAT,
                           .type_name = "",
                           .values = absl::MakeSpan(zmin)};

  std::vector<double> zmax = {0.3};
  Parameter zmax_parameter{.directive = "",
                           .type = ParameterType::FLOAT,
                           .type_name = "",
                           .values = absl::MakeSpan(zmax)};

  std::vector<double> phimax = {0.4};
  Parameter phimax_parameter{.directive = "",
                             .type = ParameterType::FLOAT,
                             .type_name = "",
                             .values = absl::MakeSpan(phimax)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"radius", radius_parameter},
      {"zmin", zmin_parameter},
      {"zmax", zmax_parameter},
      {"phimax", phimax_parameter}};

  SphereShape actual;
  RemoveSphereShapeV1(parameters, actual);
  EXPECT_THAT(actual,
              EqualsProto(R"pb(
                radius: 0.1 zmin: 0.2 zmax: 0.3 phimax: 0.4
              )pb"));
}

TEST(RemoveTriangleMeshShapeV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  TriangleMeshShape actual;
  EXPECT_TRUE(RemoveTriangleMeshShapeV1(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveTriangleMeshShapeV1, WithData) {
  std::vector<std::array<double, 3>> p = {{1.0, 2.0, 3.0}};
  Parameter p_parameter{.directive = "",
                        .type = ParameterType::POINT3,
                        .type_name = "",
                        .values = absl::MakeSpan(p)};

  std::vector<int32_t> indices = {4, 5, 6};
  Parameter indices_parameter{.directive = "",
                              .type = ParameterType::INTEGER,
                              .type_name = "",
                              .values = absl::MakeSpan(indices)};

  std::vector<std::array<double, 3>> n = {{7.0, 8.0, 9.0}};
  Parameter n_parameter{.directive = "",
                        .type = ParameterType::NORMAL3,
                        .type_name = "",
                        .values = absl::MakeSpan(n)};

  std::vector<std::array<double, 3>> s = {{10.0, 11.0, 12.0}};
  Parameter s_parameter{.directive = "",
                        .type = ParameterType::VECTOR3,
                        .type_name = "",
                        .values = absl::MakeSpan(s)};

  std::vector<double> uv = {13.0, 14.0};
  Parameter uv_parameter{.directive = "",
                         .type = ParameterType::FLOAT,
                         .type_name = "",
                         .values = absl::MakeSpan(uv)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"P", p_parameter},
      {"indices", indices_parameter},
      {"N", n_parameter},
      {"S", s_parameter},
      {"uv", uv_parameter}};

  TriangleMeshShape actual;
  EXPECT_TRUE(RemoveTriangleMeshShapeV1(parameters, actual).ok());
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
  Parameter st_parameter{.directive = "",
                         .type = ParameterType::FLOAT,
                         .type_name = "",
                         .values = absl::MakeSpan(st)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"st", st_parameter}};

  TriangleMeshShape actual;
  EXPECT_TRUE(RemoveTriangleMeshShapeV1(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb(
                uv { u: 13.0 v: 14.0 }
              )pb"));
}

TEST(RemoveTriangleMeshShapeV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  TriangleMeshShape actual;
  EXPECT_TRUE(RemoveTriangleMeshShapeV2(parameters, actual).ok());
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveTriangleMeshShapeV2, WithData) {
  std::vector<std::array<double, 3>> p = {{1.0, 2.0, 3.0}};
  Parameter p_parameter{.directive = "",
                        .type = ParameterType::POINT3,
                        .type_name = "",
                        .values = absl::MakeSpan(p)};

  std::vector<int32_t> indices = {4, 5, 6};
  Parameter indices_parameter{.directive = "",
                              .type = ParameterType::INTEGER,
                              .type_name = "",
                              .values = absl::MakeSpan(indices)};

  std::vector<std::array<double, 3>> n = {{7.0, 8.0, 9.0}};
  Parameter n_parameter{.directive = "",
                        .type = ParameterType::NORMAL3,
                        .type_name = "",
                        .values = absl::MakeSpan(n)};

  std::vector<std::array<double, 3>> s = {{10.0, 11.0, 12.0}};
  Parameter s_parameter{.directive = "",
                        .type = ParameterType::VECTOR3,
                        .type_name = "",
                        .values = absl::MakeSpan(s)};

  std::vector<double> uv = {13.0, 14.0};
  Parameter uv_parameter{.directive = "",
                         .type = ParameterType::FLOAT,
                         .type_name = "",
                         .values = absl::MakeSpan(uv)};

  bool discarddegenerateuvs[] = {true};
  Parameter discarddegenerateuvs_parameter{
      .directive = "",
      .type = ParameterType::BOOL,
      .type_name = "",
      .values = absl::MakeSpan(discarddegenerateuvs)};

  std::vector<double> alpha = {1.0};
  Parameter alpha_parameter{.directive = "",
                            .type = ParameterType::FLOAT,
                            .type_name = "",
                            .values = absl::MakeSpan(alpha)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"P", p_parameter},
      {"indices", indices_parameter},
      {"N", n_parameter},
      {"S", s_parameter},
      {"uv", uv_parameter},
      {"discarddegenerateUVs", discarddegenerateuvs_parameter},
      {"alpha", alpha_parameter}};

  TriangleMeshShape actual;
  EXPECT_TRUE(RemoveTriangleMeshShapeV2(parameters, actual).ok());
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

}  // namespace
}  // namespace pbrt_proto
