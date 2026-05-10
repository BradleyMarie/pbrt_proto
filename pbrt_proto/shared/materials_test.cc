#include "pbrt_proto/shared/materials.h"

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

TEST(BuiltInMaterial, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  BuiltInMaterial actual;
  RemoveBuiltInMaterial(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(BuiltInMaterial, WithData) {
  std::vector<std::string_view> bumpmap = {"bump"};
  Parameter bumpmap_parameter{.directive = "",
                              .type = ParameterType::TEXTURE,
                              .type_name = "",
                              .values = absl::MakeSpan(bumpmap)};

  absl::flat_hash_map<absl::string_view, Parameter> parameters = {
      {"bumpmap", bumpmap_parameter},
  };

  BuiltInMaterial actual;
  RemoveBuiltInMaterial(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                bumpmap { float_texture_name: "bump" }
              )pb"));
}

TEST(GlassMaterialV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  GlassMaterial actual;
  RemoveGlassMaterialV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(GlassMaterialV1, WithData) {
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

TEST(GlassMaterialV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  GlassMaterial actual;
  RemoveGlassMaterialV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(GlassMaterialV2, WithDataIndex) {
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
  RemoveGlassMaterialV2(parameters, actual);
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

TEST(GlassMaterialV2, WithDataEta) {
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
  RemoveGlassMaterialV2(parameters, actual);
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

TEST(MirrorMaterial, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  MirrorMaterial actual;
  RemoveMirrorMaterial(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(MirrorMaterial, WithData) {
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
  RemoveMirrorMaterial(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                Kr { spectrum_texture_name: "kr" }
                bumpmap { float_texture_name: "bump" }
              )pb"));
}

TEST(MatteMaterial, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  MatteMaterial actual;
  RemoveMatteMaterial(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(MatteMaterial, WithData) {
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
  RemoveMatteMaterial(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                Kd { spectrum_texture_name: "kd" }
                sigma { float_texture_name: "sigma" }
                bumpmap { float_texture_name: "bump" }
              )pb"));
}

TEST(PlasticMaterialV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  PlasticMaterial actual;
  RemovePlasticMaterialV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(PlasticMaterialV1, WithData) {
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
  RemovePlasticMaterialV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                Kd { spectrum_texture_name: "kd" }
                Ks { spectrum_texture_name: "ks" }
                roughness { float_texture_name: "roughness" }
                bumpmap { float_texture_name: "bump" }
              )pb"));
}

TEST(PlasticMaterialV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  PlasticMaterial actual;
  RemovePlasticMaterialV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(PlasticMaterialV2, WithData) {
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
  RemovePlasticMaterialV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb(
                Kd { spectrum_texture_name: "kd" }
                Ks { spectrum_texture_name: "ks" }
                roughness { float_texture_name: "roughness" }
                bumpmap { float_texture_name: "bump" }
                remaproughness: false
              )pb"));
}

TEST(UberMaterialV1, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  UberMaterial actual;
  RemoveUberMaterialV1(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(UberMaterialV1, WithData) {
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

TEST(UberMaterialV2, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  UberMaterial actual;
  RemoveUberMaterialV2(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(UberMaterialV2, WithData) {
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

TEST(UberMaterialV3, Empty) {
  absl::flat_hash_map<absl::string_view, Parameter> parameters;

  UberMaterial actual;
  RemoveUberMaterialV3(parameters, actual);
  EXPECT_THAT(actual, EqualsProto(R"pb()pb"));
}

TEST(UberMaterialV3, WithDataIndex) {
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

TEST(UberMaterialV3, WithDataEta) {
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
