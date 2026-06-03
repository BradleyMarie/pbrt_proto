#include "pbrt_proto/shared/materials.h"

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

TEST(RemoveBuiltInMaterialV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  BuiltInMaterial actual;
  RemoveBuiltInMaterialV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveBuiltInMaterialV1, WithData) {
  std::vector<std::string_view> bumpmap = {"bump"};
  Parameter bumpmap_parameter{.directive = "",
                              .type = ParameterType::TEXTURE,
                              .type_name = "",
                              .values = absl::MakeSpan(bumpmap)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"bumpmap", bumpmap_parameter},
  };

  BuiltInMaterial actual;
  RemoveBuiltInMaterialV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                bumpmap { float_texture_name: "bump" }
              )pb"));
}

TEST(RemoveDisneyMaterialV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  DisneyMaterial actual;
  RemoveDisneyMaterialV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveDisneyMaterialV3, WithData) {
  std::vector<std::string_view> roughness = {"roughness"};
  Parameter roughness_parameter{.directive = "",
                                .type = ParameterType::TEXTURE,
                                .type_name = "",
                                .values = absl::MakeSpan(roughness)};

  std::vector<std::string_view> eta = {"eta"};
  Parameter eta_parameter{.directive = "",
                          .type = ParameterType::TEXTURE,
                          .type_name = "",
                          .values = absl::MakeSpan(eta)};

  std::vector<std::string_view> bumpmap = {"bump"};
  Parameter bumpmap_parameter{.directive = "",
                              .type = ParameterType::TEXTURE,
                              .type_name = "",
                              .values = absl::MakeSpan(bumpmap)};

  std::vector<std::string_view> color = {"color"};
  Parameter color_parameter{.directive = "",
                            .type = ParameterType::TEXTURE,
                            .type_name = "",
                            .values = absl::MakeSpan(color)};

  std::vector<std::string_view> scatterdistance = {"scatterdistance"};
  Parameter scatterdistance_parameter{
      .directive = "",
      .type = ParameterType::TEXTURE,
      .type_name = "",
      .values = absl::MakeSpan(scatterdistance)};

  std::vector<std::string_view> anisotropic = {"anisotropic"};
  Parameter anisotropic_parameter{.directive = "",
                                  .type = ParameterType::TEXTURE,
                                  .type_name = "",
                                  .values = absl::MakeSpan(anisotropic)};

  std::vector<std::string_view> clearcoat = {"clearcoat"};
  Parameter clearcoat_parameter{.directive = "",
                                .type = ParameterType::TEXTURE,
                                .type_name = "",
                                .values = absl::MakeSpan(clearcoat)};

  std::vector<std::string_view> clearcoatgloss = {"clearcoatgloss"};
  Parameter clearcoatgloss_parameter{.directive = "",
                                     .type = ParameterType::TEXTURE,
                                     .type_name = "",
                                     .values = absl::MakeSpan(clearcoatgloss)};

  std::vector<std::string_view> difftrans = {"difftrans"};
  Parameter difftrans_parameter{.directive = "",
                                .type = ParameterType::TEXTURE,
                                .type_name = "",
                                .values = absl::MakeSpan(difftrans)};

  std::vector<std::string_view> flatness = {"flatness"};
  Parameter flatness_parameter{.directive = "",
                               .type = ParameterType::TEXTURE,
                               .type_name = "",
                               .values = absl::MakeSpan(flatness)};

  std::vector<std::string_view> metallic = {"metallic"};
  Parameter metallic_parameter{.directive = "",
                               .type = ParameterType::TEXTURE,
                               .type_name = "",
                               .values = absl::MakeSpan(metallic)};

  std::vector<std::string_view> spectrans = {"spectrans"};
  Parameter spectrans_parameter{.directive = "",
                                .type = ParameterType::TEXTURE,
                                .type_name = "",
                                .values = absl::MakeSpan(spectrans)};

  std::vector<std::string_view> speculartint = {"speculartint"};
  Parameter speculartint_parameter{.directive = "",
                                   .type = ParameterType::TEXTURE,
                                   .type_name = "",
                                   .values = absl::MakeSpan(speculartint)};

  std::vector<std::string_view> sheen = {"sheen"};
  Parameter sheen_parameter{.directive = "",
                            .type = ParameterType::TEXTURE,
                            .type_name = "",
                            .values = absl::MakeSpan(sheen)};

  std::vector<std::string_view> sheentint = {"sheentint"};
  Parameter sheentint_parameter{.directive = "",
                                .type = ParameterType::TEXTURE,
                                .type_name = "",
                                .values = absl::MakeSpan(sheentint)};

  bool thin[] = {true};
  Parameter thin_parameter{.directive = "",
                           .type = ParameterType::BOOL,
                           .type_name = "",
                           .values = absl::MakeSpan(thin)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"roughness", roughness_parameter},
      {"eta", eta_parameter},
      {"bumpmap", bumpmap_parameter},
      {"color", color_parameter},
      {"scatterdistance", scatterdistance_parameter},
      {"anisotropic", anisotropic_parameter},
      {"clearcoat", clearcoat_parameter},
      {"clearcoatgloss", clearcoatgloss_parameter},
      {"difftrans", difftrans_parameter},
      {"flatness", flatness_parameter},
      {"metallic", metallic_parameter},
      {"spectrans", spectrans_parameter},
      {"speculartint", speculartint_parameter},
      {"sheen", sheen_parameter},
      {"sheentint", sheentint_parameter},
      {"thin", thin_parameter},
  };

  DisneyMaterial actual;
  RemoveDisneyMaterialV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                roughness { float_texture_name: "roughness" }
                eta { float_texture_name: "eta" }
                bumpmap { float_texture_name: "bump" }
                color { spectrum_texture_name: "color" }
                scatterdistance { spectrum_texture_name: "scatterdistance" }
                anisotropic { float_texture_name: "anisotropic" }
                clearcoat { float_texture_name: "clearcoat" }
                clearcoatgloss { float_texture_name: "clearcoatgloss" }
                difftrans { float_texture_name: "difftrans" }
                flatness { float_texture_name: "flatness" }
                metallic { float_texture_name: "metallic" }
                spectrans { float_texture_name: "spectrans" }
                speculartint { float_texture_name: "speculartint" }
                sheen { float_texture_name: "sheen" }
                sheentint { float_texture_name: "sheentint" }
                thin: true
              )pb"));
}

TEST(RemoveGlassMaterialV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  GlassMaterial actual;
  RemoveGlassMaterialV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveGlassMaterialV1, WithData) {
  std::vector<std::string_view> kr = {"kr"};
  Parameter kr_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(kr)};

  std::vector<std::string_view> kt = {"kt"};
  Parameter kt_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(kt)};

  std::vector<std::string_view> index = {"index"};
  Parameter index_parameter{.directive = "",
                            .type = ParameterType::TEXTURE,
                            .type_name = "",
                            .values = absl::MakeSpan(index)};

  std::vector<std::string_view> bumpmap = {"bump"};
  Parameter bumpmap_parameter{.directive = "",
                              .type = ParameterType::TEXTURE,
                              .type_name = "",
                              .values = absl::MakeSpan(bumpmap)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"Kr", kr_parameter},
      {"Kt", kt_parameter},
      {"index", index_parameter},
      {"bumpmap", bumpmap_parameter},
  };

  GlassMaterial actual;
  RemoveGlassMaterialV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                Kr { spectrum_texture_name: "kr" }
                Kt { spectrum_texture_name: "kt" }
                eta { float_texture_name: "index" }
                bumpmap { float_texture_name: "bump" }
              )pb"));
}

TEST(RemoveGlassMaterialV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  GlassMaterial actual;
  RemoveGlassMaterialV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveGlassMaterialV3, WithDataIndex) {
  std::vector<std::string_view> kr = {"kr"};
  Parameter kr_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(kr)};

  std::vector<std::string_view> kt = {"kt"};
  Parameter kt_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(kt)};

  std::vector<std::string_view> index = {"index"};
  Parameter index_parameter{.directive = "",
                            .type = ParameterType::TEXTURE,
                            .type_name = "",
                            .values = absl::MakeSpan(index)};

  std::vector<std::string_view> bumpmap = {"bump"};
  Parameter bumpmap_parameter{.directive = "",
                              .type = ParameterType::TEXTURE,
                              .type_name = "",
                              .values = absl::MakeSpan(bumpmap)};

  std::vector<std::string_view> uroughness = {"uroughness"};
  Parameter uroughness_parameter{.directive = "",
                                 .type = ParameterType::TEXTURE,
                                 .type_name = "",
                                 .values = absl::MakeSpan(uroughness)};

  std::vector<std::string_view> vroughness = {"vroughness"};
  Parameter vroughness_parameter{.directive = "",
                                 .type = ParameterType::TEXTURE,
                                 .type_name = "",
                                 .values = absl::MakeSpan(vroughness)};

  bool remaproughness[] = {false};
  Parameter remaproughness_parameter{.directive = "",
                                     .type = ParameterType::BOOL,
                                     .type_name = "",
                                     .values = absl::MakeSpan(remaproughness)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"Kr", kr_parameter},
      {"Kt", kt_parameter},
      {"index", index_parameter},
      {"bumpmap", bumpmap_parameter},
      {"uroughness", uroughness_parameter},
      {"vroughness", vroughness_parameter},
      {"remaproughness", remaproughness_parameter},
  };

  GlassMaterial actual;
  RemoveGlassMaterialV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                Kr { spectrum_texture_name: "kr" }
                Kt { spectrum_texture_name: "kt" }
                eta { float_texture_name: "index" }
                bumpmap { float_texture_name: "bump" }
                uroughness { float_texture_name: "uroughness" }
                vroughness { float_texture_name: "vroughness" }
                remaproughness: false
              )pb"));
}

TEST(RemoveGlassMaterialV3, WithDataEta) {
  std::vector<std::string_view> kr = {"kr"};
  Parameter kr_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(kr)};

  std::vector<std::string_view> kt = {"kt"};
  Parameter kt_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(kt)};

  std::vector<std::string_view> eta = {"eta"};
  Parameter eta_parameter{.directive = "",
                          .type = ParameterType::TEXTURE,
                          .type_name = "",
                          .values = absl::MakeSpan(eta)};

  std::vector<std::string_view> index = {"index"};
  Parameter index_parameter{.directive = "",
                            .type = ParameterType::TEXTURE,
                            .type_name = "",
                            .values = absl::MakeSpan(index)};

  std::vector<std::string_view> bumpmap = {"bump"};
  Parameter bumpmap_parameter{.directive = "",
                              .type = ParameterType::TEXTURE,
                              .type_name = "",
                              .values = absl::MakeSpan(bumpmap)};

  std::vector<std::string_view> uroughness = {"uroughness"};
  Parameter uroughness_parameter{.directive = "",
                                 .type = ParameterType::TEXTURE,
                                 .type_name = "",
                                 .values = absl::MakeSpan(uroughness)};

  std::vector<std::string_view> vroughness = {"vroughness"};
  Parameter vroughness_parameter{.directive = "",
                                 .type = ParameterType::TEXTURE,
                                 .type_name = "",
                                 .values = absl::MakeSpan(vroughness)};

  bool remaproughness[] = {false};
  Parameter remaproughness_parameter{.directive = "",
                                     .type = ParameterType::BOOL,
                                     .type_name = "",
                                     .values = absl::MakeSpan(remaproughness)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"Kr", kr_parameter},
      {"Kt", kt_parameter},
      {"eta", eta_parameter},
      {"index", index_parameter},
      {"bumpmap", bumpmap_parameter},
      {"uroughness", uroughness_parameter},
      {"vroughness", vroughness_parameter},
      {"remaproughness", remaproughness_parameter},
  };

  GlassMaterial actual;
  RemoveGlassMaterialV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                Kr { spectrum_texture_name: "kr" }
                Kt { spectrum_texture_name: "kt" }
                eta { float_texture_name: "eta" }
                bumpmap { float_texture_name: "bump" }
                uroughness { float_texture_name: "uroughness" }
                vroughness { float_texture_name: "vroughness" }
                remaproughness: false
              )pb"));
}

TEST(RemoveHairMaterialV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  HairMaterial actual;
  RemoveHairMaterialV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveHairMaterialV3, WithData) {
  std::vector<std::string_view> eta = {"eta"};
  Parameter eta_parameter{.directive = "",
                          .type = ParameterType::TEXTURE,
                          .type_name = "",
                          .values = absl::MakeSpan(eta)};

  std::vector<std::string_view> sigma_a = {"sigma_a"};
  Parameter sigma_a_parameter{.directive = "",
                              .type = ParameterType::TEXTURE,
                              .type_name = "",
                              .values = absl::MakeSpan(sigma_a)};

  std::vector<std::string_view> color = {"color"};
  Parameter color_parameter{.directive = "",
                            .type = ParameterType::TEXTURE,
                            .type_name = "",
                            .values = absl::MakeSpan(color)};

  std::vector<std::string_view> eumelanin = {"eumelanin"};
  Parameter eumelanin_parameter{.directive = "",
                                .type = ParameterType::TEXTURE,
                                .type_name = "",
                                .values = absl::MakeSpan(eumelanin)};

  std::vector<std::string_view> pheomelanin = {"pheomelanin"};
  Parameter pheomelanin_parameter{.directive = "",
                                  .type = ParameterType::TEXTURE,
                                  .type_name = "",
                                  .values = absl::MakeSpan(pheomelanin)};

  std::vector<std::string_view> beta_m = {"beta_m"};
  Parameter beta_m_parameter{.directive = "",
                             .type = ParameterType::TEXTURE,
                             .type_name = "",
                             .values = absl::MakeSpan(beta_m)};

  std::vector<std::string_view> beta_n = {"beta_n"};
  Parameter beta_n_parameter{.directive = "",
                             .type = ParameterType::TEXTURE,
                             .type_name = "",
                             .values = absl::MakeSpan(beta_n)};

  std::vector<std::string_view> alpha = {"alpha"};
  Parameter alpha_parameter{.directive = "",
                            .type = ParameterType::TEXTURE,
                            .type_name = "",
                            .values = absl::MakeSpan(alpha)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"eta", eta_parameter},
      {"sigma_a", sigma_a_parameter},
      {"color", color_parameter},
      {"eumelanin", eumelanin_parameter},
      {"pheomelanin", pheomelanin_parameter},
      {"beta_m", beta_m_parameter},
      {"beta_n", beta_n_parameter},
      {"alpha", alpha_parameter},
  };

  HairMaterial actual;
  RemoveHairMaterialV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                eta { float_texture_name: "eta" }
                sigma_a { spectrum_texture_name: "sigma_a" }
                color { spectrum_texture_name: "color" }
                eumelanin { float_texture_name: "eumelanin" }
                pheomelanin { float_texture_name: "pheomelanin" }
                beta_m { float_texture_name: "beta_m" }
                beta_n { float_texture_name: "beta_n" }
                alpha { float_texture_name: "alpha" }
              )pb"));
}

TEST(RemoveKdSubsurfaceMaterialV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  KdSubsurfaceMaterial actual;
  RemoveKdSubsurfaceMaterialV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveKdSubsurfaceMaterialV2, WithData) {
  std::vector<std::string_view> kd = {"kd"};
  Parameter kd_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(kd)};

  std::vector<std::string_view> kr = {"kr"};
  Parameter kr_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(kr)};

  std::vector<std::string_view> eta = {"eta"};
  Parameter eta_parameter{.directive = "",
                          .type = ParameterType::TEXTURE,
                          .type_name = "",
                          .values = absl::MakeSpan(eta)};

  std::vector<std::string_view> mfp = {"mfp"};
  Parameter mfp_parameter{.directive = "",
                          .type = ParameterType::TEXTURE,
                          .type_name = "",
                          .values = absl::MakeSpan(mfp)};

  std::vector<std::string_view> bumpmap = {"bump"};
  Parameter bumpmap_parameter{.directive = "",
                              .type = ParameterType::TEXTURE,
                              .type_name = "",
                              .values = absl::MakeSpan(bumpmap)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"Kd", kd_parameter},           {"Kr", kr_parameter},
      {"index", eta_parameter},       {"meanfreepath", mfp_parameter},
      {"bumpmap", bumpmap_parameter},
  };

  KdSubsurfaceMaterial actual;
  RemoveKdSubsurfaceMaterialV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                Kd { spectrum_texture_name: "kd" }
                Kr { spectrum_texture_name: "kr" }
                eta { float_texture_name: "eta" }
                meanfreepath { float_texture_name: "mfp" }
                bumpmap { float_texture_name: "bump" }
              )pb"));
}

TEST(RemoveKdSubsurfaceMaterialV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  KdSubsurfaceMaterial actual;
  RemoveKdSubsurfaceMaterialV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveKdSubsurfaceMaterialV3, WithData) {
  std::vector<std::string_view> kd = {"kd"};
  Parameter kd_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(kd)};

  std::vector<std::string_view> kr = {"kr"};
  Parameter kr_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(kr)};

  std::vector<std::string_view> kt = {"kt"};
  Parameter kt_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(kt)};

  std::vector<double> eta = {2.0};
  Parameter eta_parameter{.directive = "",
                          .type = ParameterType::FLOAT,
                          .type_name = "",
                          .values = absl::MakeSpan(eta)};

  std::vector<std::string_view> uroughness = {"uroughness"};
  Parameter uroughness_parameter{.directive = "",
                                 .type = ParameterType::TEXTURE,
                                 .type_name = "",
                                 .values = absl::MakeSpan(uroughness)};

  std::vector<std::string_view> vroughness = {"vroughness"};
  Parameter vroughness_parameter{.directive = "",
                                 .type = ParameterType::TEXTURE,
                                 .type_name = "",
                                 .values = absl::MakeSpan(vroughness)};

  std::vector<std::string_view> mfp = {"mfp"};
  Parameter mfp_parameter{.directive = "",
                          .type = ParameterType::TEXTURE,
                          .type_name = "",
                          .values = absl::MakeSpan(mfp)};

  std::vector<std::string_view> bumpmap = {"bump"};
  Parameter bumpmap_parameter{.directive = "",
                              .type = ParameterType::TEXTURE,
                              .type_name = "",
                              .values = absl::MakeSpan(bumpmap)};

  std::vector<double> g = {1.0};
  Parameter g_parameter{.directive = "",
                        .type = ParameterType::FLOAT,
                        .type_name = "",
                        .values = absl::MakeSpan(g)};

  std::vector<double> scale = {0.0};
  Parameter scale_parameter{.directive = "",
                            .type = ParameterType::FLOAT,
                            .type_name = "",
                            .values = absl::MakeSpan(scale)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"Kd", kd_parameter},
      {"Kr", kr_parameter},
      {"Kt", kt_parameter},
      {"eta", eta_parameter},
      {"uroughness", uroughness_parameter},
      {"vroughness", vroughness_parameter},
      {"mfp", mfp_parameter},
      {"bumpmap", bumpmap_parameter},
      {"g", g_parameter},
      {"scale", scale_parameter},
  };

  KdSubsurfaceMaterial actual;
  RemoveKdSubsurfaceMaterialV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                Kd { spectrum_texture_name: "kd" }
                Kr { spectrum_texture_name: "kr" }
                Kt { spectrum_texture_name: "kt" }
                eta { float_value: 2.0 }
                uroughness { float_texture_name: "uroughness" }
                vroughness { float_texture_name: "vroughness" }
                mfp { spectrum_texture_name: "mfp" }
                bumpmap { float_texture_name: "bump" }
                g: 1.0
                scale: 0.0
              )pb"));
}

TEST(RemoveMatteMaterialV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  MatteMaterial actual;
  RemoveMatteMaterialV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveMatteMaterialV1, WithData) {
  std::vector<std::string_view> kd = {"kd"};
  Parameter kd_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(kd)};

  std::vector<std::string_view> sigma = {"sigma"};
  Parameter sigma_parameter{.directive = "",
                            .type = ParameterType::TEXTURE,
                            .type_name = "",
                            .values = absl::MakeSpan(sigma)};

  std::vector<std::string_view> bumpmap = {"bump"};
  Parameter bumpmap_parameter{.directive = "",
                              .type = ParameterType::TEXTURE,
                              .type_name = "",
                              .values = absl::MakeSpan(bumpmap)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"Kd", kd_parameter},
      {"sigma", sigma_parameter},
      {"bumpmap", bumpmap_parameter},
  };

  MatteMaterial actual;
  RemoveMatteMaterialV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                Kd { spectrum_texture_name: "kd" }
                sigma { float_texture_name: "sigma" }
                bumpmap { float_texture_name: "bump" }
              )pb"));
}

TEST(RemoveMeasuredFourierMaterialV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  MeasuredFourierMaterial actual;
  RemoveMeasuredFourierMaterialV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveMeasuredFourierMaterialV3, WithData) {
  std::vector<std::string_view> filename = {"filename"};
  Parameter filename_parameter{.directive = "",
                               .type = ParameterType::STRING,
                               .type_name = "",
                               .values = absl::MakeSpan(filename)};

  std::vector<std::string_view> bumpmap = {"bump"};
  Parameter bumpmap_parameter{.directive = "",
                              .type = ParameterType::TEXTURE,
                              .type_name = "",
                              .values = absl::MakeSpan(bumpmap)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"bsdffile", filename_parameter},
      {"bumpmap", bumpmap_parameter},
  };

  MeasuredFourierMaterial actual;
  RemoveMeasuredFourierMaterialV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                filename: "filename"
                bumpmap { float_texture_name: "bump" }
              )pb"));
}

TEST(RemoveMeasuredMerlMaterialV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  MeasuredMerlMaterial actual;
  RemoveMeasuredMerlMaterialV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveMeasuredMerlMaterialV2, WithData) {
  std::vector<std::string_view> filename = {"filename"};
  Parameter filename_parameter{.directive = "",
                               .type = ParameterType::STRING,
                               .type_name = "",
                               .values = absl::MakeSpan(filename)};

  std::vector<std::string_view> bumpmap = {"bump"};
  Parameter bumpmap_parameter{.directive = "",
                              .type = ParameterType::TEXTURE,
                              .type_name = "",
                              .values = absl::MakeSpan(bumpmap)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"filename", filename_parameter},
      {"bumpmap", bumpmap_parameter},
  };

  MeasuredMerlMaterial actual;
  RemoveMeasuredMerlMaterialV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                filename: "filename"
                bumpmap { float_texture_name: "bump" }
              )pb"));
}

TEST(RemoveMetalMaterialV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  MetalMaterial actual;
  RemoveMetalMaterialV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveMetalMaterialV2, WithData) {
  std::vector<std::string_view> k = {"k"};
  Parameter k_parameter{.directive = "",
                        .type = ParameterType::TEXTURE,
                        .type_name = "",
                        .values = absl::MakeSpan(k)};

  std::vector<std::string_view> eta = {"eta"};
  Parameter eta_parameter{.directive = "",
                          .type = ParameterType::TEXTURE,
                          .type_name = "",
                          .values = absl::MakeSpan(eta)};

  std::vector<std::string_view> roughness = {"roughness"};
  Parameter roughness_parameter{.directive = "",
                                .type = ParameterType::TEXTURE,
                                .type_name = "",
                                .values = absl::MakeSpan(roughness)};

  std::vector<std::string_view> bumpmap = {"bump"};
  Parameter bumpmap_parameter{.directive = "",
                              .type = ParameterType::TEXTURE,
                              .type_name = "",
                              .values = absl::MakeSpan(bumpmap)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"k", k_parameter},
      {"eta", eta_parameter},
      {"roughness", roughness_parameter},
      {"bumpmap", bumpmap_parameter},
  };

  MetalMaterial actual;
  RemoveMetalMaterialV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                k { spectrum_texture_name: "k" }
                eta { spectrum_texture_name: "eta" }
                roughness { float_texture_name: "roughness" }
                bumpmap { float_texture_name: "bump" }
              )pb"));
}

TEST(RemoveMetalMaterialV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  MetalMaterial actual;
  RemoveMetalMaterialV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveMetalMaterialV3, WithData) {
  std::vector<std::string_view> k = {"k"};
  Parameter k_parameter{.directive = "",
                        .type = ParameterType::TEXTURE,
                        .type_name = "",
                        .values = absl::MakeSpan(k)};

  std::vector<std::string_view> eta = {"eta"};
  Parameter eta_parameter{.directive = "",
                          .type = ParameterType::TEXTURE,
                          .type_name = "",
                          .values = absl::MakeSpan(eta)};

  std::vector<std::string_view> roughness = {"roughness"};
  Parameter roughness_parameter{.directive = "",
                                .type = ParameterType::TEXTURE,
                                .type_name = "",
                                .values = absl::MakeSpan(roughness)};

  std::vector<std::string_view> bumpmap = {"bump"};
  Parameter bumpmap_parameter{.directive = "",
                              .type = ParameterType::TEXTURE,
                              .type_name = "",
                              .values = absl::MakeSpan(bumpmap)};

  std::vector<std::string_view> uroughness = {"uroughness"};
  Parameter uroughness_parameter{.directive = "",
                                 .type = ParameterType::TEXTURE,
                                 .type_name = "",
                                 .values = absl::MakeSpan(uroughness)};

  std::vector<std::string_view> vroughness = {"vroughness"};
  Parameter vroughness_parameter{.directive = "",
                                 .type = ParameterType::TEXTURE,
                                 .type_name = "",
                                 .values = absl::MakeSpan(vroughness)};

  bool remaproughness[] = {false};
  Parameter remaproughness_parameter{.directive = "",
                                     .type = ParameterType::BOOL,
                                     .type_name = "",
                                     .values = absl::MakeSpan(remaproughness)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"k", k_parameter},
      {"eta", eta_parameter},
      {"roughness", roughness_parameter},
      {"bumpmap", bumpmap_parameter},
      {"uroughness", uroughness_parameter},
      {"vroughness", vroughness_parameter},
      {"remaproughness", remaproughness_parameter},
  };

  MetalMaterial actual;
  RemoveMetalMaterialV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                k { spectrum_texture_name: "k" }
                eta { spectrum_texture_name: "eta" }
                roughness { float_texture_name: "roughness" }
                bumpmap { float_texture_name: "bump" }
                uroughness { float_texture_name: "uroughness" }
                vroughness { float_texture_name: "vroughness" }
                remaproughness: false
              )pb"));
}

TEST(RemoveMirrorMaterialV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  MirrorMaterial actual;
  RemoveMirrorMaterialV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveMirrorMaterialV1, WithData) {
  std::vector<std::string_view> kr = {"kr"};
  Parameter kr_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(kr)};

  std::vector<std::string_view> bumpmap = {"bump"};
  Parameter bumpmap_parameter{.directive = "",
                              .type = ParameterType::TEXTURE,
                              .type_name = "",
                              .values = absl::MakeSpan(bumpmap)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"Kr", kr_parameter},
      {"bumpmap", bumpmap_parameter},
  };

  MirrorMaterial actual;
  RemoveMirrorMaterialV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                Kr { spectrum_texture_name: "kr" }
                bumpmap { float_texture_name: "bump" }
              )pb"));
}

TEST(RemoveMixMaterialV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  MixMaterial actual;
  RemoveMixMaterialV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveMixMaterialV2, WithData) {
  std::vector<std::string_view> kd = {"kd"};
  Parameter kd_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(kd)};

  std::vector<std::string_view> sigma = {"sigma"};
  Parameter sigma_parameter{.directive = "",
                            .type = ParameterType::TEXTURE,
                            .type_name = "",
                            .values = absl::MakeSpan(sigma)};

  std::vector<std::string_view> bumpmap = {"bump"};
  Parameter bumpmap_parameter{.directive = "",
                              .type = ParameterType::TEXTURE,
                              .type_name = "",
                              .values = absl::MakeSpan(bumpmap)};

  std::vector<std::string_view> namedmaterial1 = {"namedmaterial1"};
  Parameter namedmaterial1_parameter{.directive = "",
                                     .type = ParameterType::TEXTURE,
                                     .type_name = "",
                                     .values = absl::MakeSpan(namedmaterial1)};

  std::vector<std::string_view> namedmaterial2 = {"namedmaterial2"};
  Parameter namedmaterial2_parameter{.directive = "",
                                     .type = ParameterType::TEXTURE,
                                     .type_name = "",
                                     .values = absl::MakeSpan(namedmaterial2)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"Kd", kd_parameter},
      {"sigma", sigma_parameter},
      {"bumpmap", bumpmap_parameter},
      {"namedmaterial1", namedmaterial1_parameter},
      {"namedmaterial2", namedmaterial2_parameter},
  };

  MixMaterial actual;
  RemoveMixMaterialV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                Kd { spectrum_texture_name: "kd" }
                sigma { float_texture_name: "sigma" }
                bumpmap { float_texture_name: "bump" }
              )pb"));
}

TEST(RemovePlasticMaterialV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  PlasticMaterial actual;
  RemovePlasticMaterialV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemovePlasticMaterialV1, WithData) {
  std::vector<std::string_view> kd = {"kd"};
  Parameter kd_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(kd)};

  std::vector<std::string_view> ks = {"ks"};
  Parameter ks_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(ks)};

  std::vector<std::string_view> roughness = {"roughness"};
  Parameter roughness_parameter{.directive = "",
                                .type = ParameterType::TEXTURE,
                                .type_name = "",
                                .values = absl::MakeSpan(roughness)};

  std::vector<std::string_view> bumpmap = {"bump"};
  Parameter bumpmap_parameter{.directive = "",
                              .type = ParameterType::TEXTURE,
                              .type_name = "",
                              .values = absl::MakeSpan(bumpmap)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"Kd", kd_parameter},
      {"Ks", ks_parameter},
      {"roughness", roughness_parameter},
      {"bumpmap", bumpmap_parameter},
  };

  PlasticMaterial actual;
  RemovePlasticMaterialV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                Kd { spectrum_texture_name: "kd" }
                Ks { spectrum_texture_name: "ks" }
                roughness { float_texture_name: "roughness" }
                bumpmap { float_texture_name: "bump" }
              )pb"));
}

TEST(RemovePlasticMaterialV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  PlasticMaterial actual;
  RemovePlasticMaterialV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemovePlasticMaterialV3, WithData) {
  std::vector<std::string_view> kd = {"kd"};
  Parameter kd_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(kd)};

  std::vector<std::string_view> ks = {"ks"};
  Parameter ks_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(ks)};

  std::vector<std::string_view> roughness = {"roughness"};
  Parameter roughness_parameter{.directive = "",
                                .type = ParameterType::TEXTURE,
                                .type_name = "",
                                .values = absl::MakeSpan(roughness)};

  std::vector<std::string_view> bumpmap = {"bump"};
  Parameter bumpmap_parameter{.directive = "",
                              .type = ParameterType::TEXTURE,
                              .type_name = "",
                              .values = absl::MakeSpan(bumpmap)};

  bool remaproughness[] = {false};
  Parameter remaproughness_parameter{.directive = "",
                                     .type = ParameterType::BOOL,
                                     .type_name = "",
                                     .values = absl::MakeSpan(remaproughness)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"Kd", kd_parameter},
      {"Ks", ks_parameter},
      {"roughness", roughness_parameter},
      {"bumpmap", bumpmap_parameter},
      {"remaproughness", remaproughness_parameter},
  };

  PlasticMaterial actual;
  RemovePlasticMaterialV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                Kd { spectrum_texture_name: "kd" }
                Ks { spectrum_texture_name: "ks" }
                roughness { float_texture_name: "roughness" }
                bumpmap { float_texture_name: "bump" }
                remaproughness: false
              )pb"));
}

TEST(RemoveShinyMetalMaterialV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  ShinyMetalMaterial actual;
  RemoveShinyMetalMaterialV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveShinyMetalMaterialV1, WithData) {
  std::vector<std::string_view> ks = {"ks"};
  Parameter ks_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(ks)};

  std::vector<std::string_view> kr = {"kr"};
  Parameter kr_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(kr)};

  std::vector<std::string_view> roughness = {"roughness"};
  Parameter roughness_parameter{.directive = "",
                                .type = ParameterType::TEXTURE,
                                .type_name = "",
                                .values = absl::MakeSpan(roughness)};

  std::vector<std::string_view> bumpmap = {"bump"};
  Parameter bumpmap_parameter{.directive = "",
                              .type = ParameterType::TEXTURE,
                              .type_name = "",
                              .values = absl::MakeSpan(bumpmap)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"Ks", ks_parameter},
      {"Kr", kr_parameter},
      {"roughness", roughness_parameter},
      {"bumpmap", bumpmap_parameter},
  };

  ShinyMetalMaterial actual;
  RemoveShinyMetalMaterialV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                Ks { spectrum_texture_name: "ks" }
                Kr { spectrum_texture_name: "kr" }
                roughness { float_texture_name: "roughness" }
                bumpmap { float_texture_name: "bump" }
              )pb"));
}

TEST(RemoveSubstrateMaterialV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  SubstrateMaterial actual;
  RemoveSubstrateMaterialV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveSubstrateMaterialV1, WithDataIndex) {
  std::vector<std::string_view> kd = {"kd"};
  Parameter kd_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(kd)};

  std::vector<std::string_view> ks = {"ks"};
  Parameter ks_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(ks)};

  std::vector<std::string_view> bumpmap = {"bump"};
  Parameter bumpmap_parameter{.directive = "",
                              .type = ParameterType::TEXTURE,
                              .type_name = "",
                              .values = absl::MakeSpan(bumpmap)};

  std::vector<std::string_view> uroughness = {"uroughness"};
  Parameter uroughness_parameter{.directive = "",
                                 .type = ParameterType::TEXTURE,
                                 .type_name = "",
                                 .values = absl::MakeSpan(uroughness)};

  std::vector<std::string_view> vroughness = {"vroughness"};
  Parameter vroughness_parameter{.directive = "",
                                 .type = ParameterType::TEXTURE,
                                 .type_name = "",
                                 .values = absl::MakeSpan(vroughness)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"Kd", kd_parameter},
      {"Ks", ks_parameter},
      {"bumpmap", bumpmap_parameter},
      {"uroughness", uroughness_parameter},
      {"vroughness", vroughness_parameter},
  };

  SubstrateMaterial actual;
  RemoveSubstrateMaterialV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                Kd { spectrum_texture_name: "kd" }
                Ks { spectrum_texture_name: "ks" }
                bumpmap { float_texture_name: "bump" }
                uroughness { float_texture_name: "uroughness" }
                vroughness { float_texture_name: "vroughness" }
              )pb"));
}

TEST(RemoveSubstrateMaterialV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  SubstrateMaterial actual;
  RemoveSubstrateMaterialV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveSubstrateMaterialV3, WithDataIndex) {
  std::vector<std::string_view> kd = {"kd"};
  Parameter kd_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(kd)};

  std::vector<std::string_view> ks = {"ks"};
  Parameter ks_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(ks)};

  std::vector<std::string_view> bumpmap = {"bump"};
  Parameter bumpmap_parameter{.directive = "",
                              .type = ParameterType::TEXTURE,
                              .type_name = "",
                              .values = absl::MakeSpan(bumpmap)};

  std::vector<std::string_view> uroughness = {"uroughness"};
  Parameter uroughness_parameter{.directive = "",
                                 .type = ParameterType::TEXTURE,
                                 .type_name = "",
                                 .values = absl::MakeSpan(uroughness)};

  std::vector<std::string_view> vroughness = {"vroughness"};
  Parameter vroughness_parameter{.directive = "",
                                 .type = ParameterType::TEXTURE,
                                 .type_name = "",
                                 .values = absl::MakeSpan(vroughness)};

  bool remaproughness[] = {false};
  Parameter remaproughness_parameter{.directive = "",
                                     .type = ParameterType::BOOL,
                                     .type_name = "",
                                     .values = absl::MakeSpan(remaproughness)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"Kd", kd_parameter},
      {"Ks", ks_parameter},
      {"bumpmap", bumpmap_parameter},
      {"uroughness", uroughness_parameter},
      {"vroughness", vroughness_parameter},
      {"remaproughness", remaproughness_parameter},
  };

  SubstrateMaterial actual;
  RemoveSubstrateMaterialV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                Kd { spectrum_texture_name: "kd" }
                Ks { spectrum_texture_name: "ks" }
                bumpmap { float_texture_name: "bump" }
                uroughness { float_texture_name: "uroughness" }
                vroughness { float_texture_name: "vroughness" }
                remaproughness: false
              )pb"));
}

TEST(RemoveSubsurfaceMaterialV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  SubsurfaceMaterial actual;
  RemoveSubsurfaceMaterialV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveSubsurfaceMaterialV3, WithData) {
  std::vector<std::string_view> kr = {"kr"};
  Parameter kr_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(kr)};

  std::vector<std::string_view> kt = {"kt"};
  Parameter kt_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(kt)};

  std::vector<std::string_view> uroughness = {"uroughness"};
  Parameter uroughness_parameter{.directive = "",
                                 .type = ParameterType::TEXTURE,
                                 .type_name = "",
                                 .values = absl::MakeSpan(uroughness)};

  std::vector<std::string_view> vroughness = {"vroughness"};
  Parameter vroughness_parameter{.directive = "",
                                 .type = ParameterType::TEXTURE,
                                 .type_name = "",
                                 .values = absl::MakeSpan(vroughness)};

  std::vector<std::string_view> bumpmap = {"bump"};
  Parameter bumpmap_parameter{.directive = "",
                              .type = ParameterType::TEXTURE,
                              .type_name = "",
                              .values = absl::MakeSpan(bumpmap)};

  bool remaproughness[] = {false};
  Parameter remaproughness_parameter{.directive = "",
                                     .type = ParameterType::BOOL,
                                     .type_name = "",
                                     .values = absl::MakeSpan(remaproughness)};

  std::vector<double> g = {1.0};
  Parameter g_parameter{.directive = "",
                        .type = ParameterType::FLOAT,
                        .type_name = "",
                        .values = absl::MakeSpan(g)};

  std::vector<double> scale = {0.0};
  Parameter scale_parameter{.directive = "",
                            .type = ParameterType::FLOAT,
                            .type_name = "",
                            .values = absl::MakeSpan(scale)};

  std::vector<double> eta = {2.0};
  Parameter eta_parameter{.directive = "",
                          .type = ParameterType::FLOAT,
                          .type_name = "",
                          .values = absl::MakeSpan(eta)};

  std::vector<std::string_view> sigma_a = {"sigma_a"};
  Parameter sigma_a_parameter{.directive = "",
                              .type = ParameterType::TEXTURE,
                              .type_name = "",
                              .values = absl::MakeSpan(sigma_a)};

  std::vector<std::string_view> sigma_s = {"sigma_s"};
  Parameter sigma_s_parameter{.directive = "",
                              .type = ParameterType::TEXTURE,
                              .type_name = "",
                              .values = absl::MakeSpan(sigma_s)};

  std::vector<std::string_view> name = {"Apple"};
  Parameter name_parameter{.directive = "",
                           .type = ParameterType::STRING,
                           .type_name = "",
                           .values = absl::MakeSpan(name)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"Kr", kr_parameter},
      {"Kt", kt_parameter},
      {"uroughness", uroughness_parameter},
      {"vroughness", vroughness_parameter},
      {"remaproughness", remaproughness_parameter},
      {"bumpmap", bumpmap_parameter},
      {"g", g_parameter},
      {"scale", scale_parameter},
      {"eta", eta_parameter},
      {"sigma_a", sigma_a_parameter},
      {"sigma_s", sigma_s_parameter},
      {"name", name_parameter},
  };

  SubsurfaceMaterial actual;
  RemoveSubsurfaceMaterialV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                Kr { spectrum_texture_name: "kr" }
                Kt { spectrum_texture_name: "kt" }
                uroughness { float_texture_name: "uroughness" }
                vroughness { float_texture_name: "vroughness" }
                remaproughness: false
                bumpmap { float_texture_name: "bump" }
                g: 1.0
                scale: 0.0
                eta: 2.0
                sigma_a { spectrum_texture_name: "sigma_a" }
                sigma_s { spectrum_texture_name: "sigma_s" }
                name: APPLE
              )pb"));
}

TEST(RemoveTranslucentMaterialV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  TranslucentMaterial actual;
  RemoveTranslucentMaterialV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveTranslucentMaterialV1, WithData) {
  std::vector<std::string_view> kd = {"kd"};
  Parameter kd_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(kd)};

  std::vector<std::string_view> ks = {"ks"};
  Parameter ks_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(ks)};

  std::vector<std::string_view> reflect = {"reflect"};
  Parameter reflect_parameter{.directive = "",
                              .type = ParameterType::TEXTURE,
                              .type_name = "",
                              .values = absl::MakeSpan(reflect)};

  std::vector<std::string_view> transmit = {"transmit"};
  Parameter transmit_parameter{.directive = "",
                               .type = ParameterType::TEXTURE,
                               .type_name = "",
                               .values = absl::MakeSpan(transmit)};

  std::vector<std::string_view> roughness = {"roughness"};
  Parameter roughness_parameter{.directive = "",
                                .type = ParameterType::TEXTURE,
                                .type_name = "",
                                .values = absl::MakeSpan(roughness)};

  std::vector<std::string_view> bumpmap = {"bump"};
  Parameter bumpmap_parameter{.directive = "",
                              .type = ParameterType::TEXTURE,
                              .type_name = "",
                              .values = absl::MakeSpan(bumpmap)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"Kd", kd_parameter},
      {"Ks", ks_parameter},
      {"reflect", reflect_parameter},
      {"transmit", transmit_parameter},
      {"roughness", roughness_parameter},
      {"bumpmap", bumpmap_parameter},
  };

  TranslucentMaterial actual;
  RemoveTranslucentMaterialV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                Kd { spectrum_texture_name: "kd" }
                Ks { spectrum_texture_name: "ks" }
                reflect { spectrum_texture_name: "reflect" }
                transmit { spectrum_texture_name: "transmit" }
                roughness { float_texture_name: "roughness" }
                bumpmap { float_texture_name: "bump" }
              )pb"));
}

TEST(RemoveTranslucentMaterialV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  TranslucentMaterial actual;
  RemoveTranslucentMaterialV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveTranslucentMaterialV3, WithData) {
  std::vector<std::string_view> kd = {"kd"};
  Parameter kd_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(kd)};

  std::vector<std::string_view> ks = {"ks"};
  Parameter ks_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(ks)};

  std::vector<std::string_view> reflect = {"reflect"};
  Parameter reflect_parameter{.directive = "",
                              .type = ParameterType::TEXTURE,
                              .type_name = "",
                              .values = absl::MakeSpan(reflect)};

  std::vector<std::string_view> transmit = {"transmit"};
  Parameter transmit_parameter{.directive = "",
                               .type = ParameterType::TEXTURE,
                               .type_name = "",
                               .values = absl::MakeSpan(transmit)};

  std::vector<std::string_view> roughness = {"roughness"};
  Parameter roughness_parameter{.directive = "",
                                .type = ParameterType::TEXTURE,
                                .type_name = "",
                                .values = absl::MakeSpan(roughness)};

  std::vector<std::string_view> bumpmap = {"bump"};
  Parameter bumpmap_parameter{.directive = "",
                              .type = ParameterType::TEXTURE,
                              .type_name = "",
                              .values = absl::MakeSpan(bumpmap)};

  bool remaproughness[] = {false};
  Parameter remaproughness_parameter{.directive = "",
                                     .type = ParameterType::BOOL,
                                     .type_name = "",
                                     .values = absl::MakeSpan(remaproughness)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"Kd", kd_parameter},
      {"Ks", ks_parameter},
      {"reflect", reflect_parameter},
      {"transmit", transmit_parameter},
      {"roughness", roughness_parameter},
      {"bumpmap", bumpmap_parameter},
      {"remaproughness", remaproughness_parameter},
  };

  TranslucentMaterial actual;
  RemoveTranslucentMaterialV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                Kd { spectrum_texture_name: "kd" }
                Ks { spectrum_texture_name: "ks" }
                reflect { spectrum_texture_name: "reflect" }
                transmit { spectrum_texture_name: "transmit" }
                roughness { float_texture_name: "roughness" }
                bumpmap { float_texture_name: "bump" }
                remaproughness: false
              )pb"));
}

TEST(RemoveUberMaterialV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  UberMaterial actual;
  RemoveUberMaterialV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveUberMaterialV1, WithData) {
  std::vector<std::string_view> kd = {"kd"};
  Parameter kd_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(kd)};

  std::vector<std::string_view> ks = {"ks"};
  Parameter ks_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(ks)};

  std::vector<std::string_view> kr = {"kr"};
  Parameter kr_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(kr)};

  std::vector<std::string_view> opacity = {"opacity"};
  Parameter opacity_parameter{.directive = "",
                              .type = ParameterType::TEXTURE,
                              .type_name = "",
                              .values = absl::MakeSpan(opacity)};

  std::vector<std::string_view> roughness = {"roughness"};
  Parameter roughness_parameter{.directive = "",
                                .type = ParameterType::TEXTURE,
                                .type_name = "",
                                .values = absl::MakeSpan(roughness)};

  std::vector<std::string_view> bumpmap = {"bump"};
  Parameter bumpmap_parameter{.directive = "",
                              .type = ParameterType::TEXTURE,
                              .type_name = "",
                              .values = absl::MakeSpan(bumpmap)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"Kd", kd_parameter},
      {"Ks", ks_parameter},
      {"Kr", kr_parameter},
      {"opacity", opacity_parameter},
      {"roughness", roughness_parameter},
      {"bumpmap", bumpmap_parameter},
  };

  UberMaterial actual;
  RemoveUberMaterialV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                Kd { spectrum_texture_name: "kd" }
                Ks { spectrum_texture_name: "ks" }
                Kr { spectrum_texture_name: "kr" }
                opacity { spectrum_texture_name: "opacity" }
                roughness { float_texture_name: "roughness" }
                bumpmap { float_texture_name: "bump" }
              )pb"));
}

TEST(RemoveUberMaterialV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  UberMaterial actual;
  RemoveUberMaterialV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveUberMaterialV2, WithData) {
  std::vector<std::string_view> kd = {"kd"};
  Parameter kd_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(kd)};

  std::vector<std::string_view> ks = {"ks"};
  Parameter ks_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(ks)};

  std::vector<std::string_view> kr = {"kr"};
  Parameter kr_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(kr)};

  std::vector<std::string_view> kt = {"kt"};
  Parameter kt_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(kt)};

  std::vector<std::string_view> opacity = {"opacity"};
  Parameter opacity_parameter{.directive = "",
                              .type = ParameterType::TEXTURE,
                              .type_name = "",
                              .values = absl::MakeSpan(opacity)};

  std::vector<std::string_view> index = {"index"};
  Parameter index_parameter{.directive = "",
                            .type = ParameterType::TEXTURE,
                            .type_name = "",
                            .values = absl::MakeSpan(index)};

  std::vector<std::string_view> roughness = {"roughness"};
  Parameter roughness_parameter{.directive = "",
                                .type = ParameterType::TEXTURE,
                                .type_name = "",
                                .values = absl::MakeSpan(roughness)};

  std::vector<std::string_view> bumpmap = {"bump"};
  Parameter bumpmap_parameter{.directive = "",
                              .type = ParameterType::TEXTURE,
                              .type_name = "",
                              .values = absl::MakeSpan(bumpmap)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"Kd", kd_parameter},
      {"Ks", ks_parameter},
      {"Kr", kr_parameter},
      {"Kt", kt_parameter},
      {"opacity", opacity_parameter},
      {"index", index_parameter},
      {"roughness", roughness_parameter},
      {"bumpmap", bumpmap_parameter},
  };

  UberMaterial actual;
  RemoveUberMaterialV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                Kd { spectrum_texture_name: "kd" }
                Ks { spectrum_texture_name: "ks" }
                Kr { spectrum_texture_name: "kr" }
                Kt { spectrum_texture_name: "kt" }
                opacity { spectrum_texture_name: "opacity" }
                eta { float_texture_name: "index" }
                roughness { float_texture_name: "roughness" }
                bumpmap { float_texture_name: "bump" }
              )pb"));
}

TEST(RemoveUberMaterialV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  UberMaterial actual;
  RemoveUberMaterialV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(RemoveUberMaterialV3, WithDataIndex) {
  std::vector<std::string_view> kd = {"kd"};
  Parameter kd_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(kd)};

  std::vector<std::string_view> ks = {"ks"};
  Parameter ks_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(ks)};

  std::vector<std::string_view> kr = {"kr"};
  Parameter kr_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(kr)};

  std::vector<std::string_view> kt = {"kt"};
  Parameter kt_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(kt)};

  std::vector<std::string_view> opacity = {"opacity"};
  Parameter opacity_parameter{.directive = "",
                              .type = ParameterType::TEXTURE,
                              .type_name = "",
                              .values = absl::MakeSpan(opacity)};

  std::vector<std::string_view> index = {"index"};
  Parameter index_parameter{.directive = "",
                            .type = ParameterType::TEXTURE,
                            .type_name = "",
                            .values = absl::MakeSpan(index)};

  std::vector<std::string_view> roughness = {"roughness"};
  Parameter roughness_parameter{.directive = "",
                                .type = ParameterType::TEXTURE,
                                .type_name = "",
                                .values = absl::MakeSpan(roughness)};

  std::vector<std::string_view> bumpmap = {"bump"};
  Parameter bumpmap_parameter{.directive = "",
                              .type = ParameterType::TEXTURE,
                              .type_name = "",
                              .values = absl::MakeSpan(bumpmap)};

  std::vector<std::string_view> uroughness = {"uroughness"};
  Parameter uroughness_parameter{.directive = "",
                                 .type = ParameterType::TEXTURE,
                                 .type_name = "",
                                 .values = absl::MakeSpan(uroughness)};

  std::vector<std::string_view> vroughness = {"vroughness"};
  Parameter vroughness_parameter{.directive = "",
                                 .type = ParameterType::TEXTURE,
                                 .type_name = "",
                                 .values = absl::MakeSpan(vroughness)};

  bool remaproughness[] = {false};
  Parameter remaproughness_parameter{.directive = "",
                                     .type = ParameterType::BOOL,
                                     .type_name = "",
                                     .values = absl::MakeSpan(remaproughness)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"Kd", kd_parameter},
      {"Ks", ks_parameter},
      {"Kr", kr_parameter},
      {"Kt", kt_parameter},
      {"opacity", opacity_parameter},
      {"index", index_parameter},
      {"roughness", roughness_parameter},
      {"bumpmap", bumpmap_parameter},
      {"uroughness", uroughness_parameter},
      {"vroughness", vroughness_parameter},
      {"remaproughness", remaproughness_parameter},
  };

  UberMaterial actual;
  RemoveUberMaterialV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                Kd { spectrum_texture_name: "kd" }
                Ks { spectrum_texture_name: "ks" }
                Kr { spectrum_texture_name: "kr" }
                Kt { spectrum_texture_name: "kt" }
                opacity { spectrum_texture_name: "opacity" }
                eta { float_texture_name: "index" }
                roughness { float_texture_name: "roughness" }
                bumpmap { float_texture_name: "bump" }
                uroughness { float_texture_name: "uroughness" }
                vroughness { float_texture_name: "vroughness" }
                remaproughness: false
              )pb"));
}

TEST(RemoveUberMaterialV3, WithDataEta) {
  std::vector<std::string_view> kd = {"kd"};
  Parameter kd_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(kd)};

  std::vector<std::string_view> ks = {"ks"};
  Parameter ks_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(ks)};

  std::vector<std::string_view> kr = {"kr"};
  Parameter kr_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(kr)};

  std::vector<std::string_view> kt = {"kt"};
  Parameter kt_parameter{.directive = "",
                         .type = ParameterType::TEXTURE,
                         .type_name = "",
                         .values = absl::MakeSpan(kt)};

  std::vector<std::string_view> opacity = {"opacity"};
  Parameter opacity_parameter{.directive = "",
                              .type = ParameterType::TEXTURE,
                              .type_name = "",
                              .values = absl::MakeSpan(opacity)};

  std::vector<std::string_view> index = {"index"};
  Parameter index_parameter{.directive = "",
                            .type = ParameterType::TEXTURE,
                            .type_name = "",
                            .values = absl::MakeSpan(index)};

  std::vector<std::string_view> eta = {"eta"};
  Parameter eta_parameter{.directive = "",
                          .type = ParameterType::TEXTURE,
                          .type_name = "",
                          .values = absl::MakeSpan(eta)};

  std::vector<std::string_view> roughness = {"roughness"};
  Parameter roughness_parameter{.directive = "",
                                .type = ParameterType::TEXTURE,
                                .type_name = "",
                                .values = absl::MakeSpan(roughness)};

  std::vector<std::string_view> bumpmap = {"bump"};
  Parameter bumpmap_parameter{.directive = "",
                              .type = ParameterType::TEXTURE,
                              .type_name = "",
                              .values = absl::MakeSpan(bumpmap)};

  std::vector<std::string_view> uroughness = {"uroughness"};
  Parameter uroughness_parameter{.directive = "",
                                 .type = ParameterType::TEXTURE,
                                 .type_name = "",
                                 .values = absl::MakeSpan(uroughness)};

  std::vector<std::string_view> vroughness = {"vroughness"};
  Parameter vroughness_parameter{.directive = "",
                                 .type = ParameterType::TEXTURE,
                                 .type_name = "",
                                 .values = absl::MakeSpan(vroughness)};

  bool remaproughness[] = {false};
  Parameter remaproughness_parameter{.directive = "",
                                     .type = ParameterType::BOOL,
                                     .type_name = "",
                                     .values = absl::MakeSpan(remaproughness)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"Kd", kd_parameter},
      {"Ks", ks_parameter},
      {"Kr", kr_parameter},
      {"Kt", kt_parameter},
      {"opacity", opacity_parameter},
      {"index", index_parameter},
      {"eta", eta_parameter},
      {"roughness", roughness_parameter},
      {"bumpmap", bumpmap_parameter},
      {"uroughness", uroughness_parameter},
      {"vroughness", vroughness_parameter},
      {"remaproughness", remaproughness_parameter},
  };

  UberMaterial actual;
  RemoveUberMaterialV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                Kd { spectrum_texture_name: "kd" }
                Ks { spectrum_texture_name: "ks" }
                Kr { spectrum_texture_name: "kr" }
                Kt { spectrum_texture_name: "kt" }
                opacity { spectrum_texture_name: "opacity" }
                eta { float_texture_name: "eta" }
                roughness { float_texture_name: "roughness" }
                bumpmap { float_texture_name: "bump" }
                uroughness { float_texture_name: "uroughness" }
                vroughness { float_texture_name: "vroughness" }
                remaproughness: false
              )pb"));
}

}  // namespace
}  // namespace pbrt_proto
