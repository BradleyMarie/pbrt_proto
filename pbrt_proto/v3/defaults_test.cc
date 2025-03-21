#include "pbrt_proto/v3/defaults.h"

#include "absl/status/status_matchers.h"
#include "absl/strings/string_view.h"
#include "googlemock/include/gmock/gmock.h"
#include "googletest/include/gtest/gtest.h"
#include "pbrt_proto/testing/proto_matchers.h"
#include "pbrt_proto/v3/pbrt.pb.h"

namespace pbrt_proto::v3 {
namespace {

using ::absl_testing::IsOkAndHolds;
using ::absl_testing::StatusIs;
using ::google::protobuf::EqualsProto;
using ::google::protobuf::ParseTextOrDie;

PbrtProto MakeCanonical(absl::string_view textproto) {
  PbrtProto proto = ParseTextOrDie(textproto);
  Canonicalize(proto);
  return proto;
}

// Unset Accelerators are default to using the BVH accelerator
// https://github.com/mmp/pbrt-v3/blob/13d871faae88233b327d04cda24022b8bb0093ee/src/core/api.cpp#L1571
// https://github.com/mmp/pbrt-v3/blob/13d871faae88233b327d04cda24022b8bb0093ee/src/core/api.cpp#L1654
TEST(Canonicalize, Accelerator) {
  EXPECT_THAT(MakeCanonical(R"pb(directives { accelerator {} })pb"),
              EqualsProto(R"pb(directives { accelerator { bvh {} } })pb"));
}

// Unset Film are left unset and should eventually cause rendering to fail
// https://github.com/mmp/pbrt-v3/blob/13d871faae88233b327d04cda24022b8bb0093ee/src/core/api.cpp#L1664
TEST(Canonicalize, Camera) {
  EXPECT_THAT(MakeCanonical(R"pb(directives { camera {} })pb"),
              EqualsProto(R"pb(directives { camera {} })pb"));
}

// Unset Film are left unset and should eventually cause rendering to fail
// https://github.com/mmp/pbrt-v3/blob/13d871faae88233b327d04cda24022b8bb0093ee/src/core/api.cpp#L1720
TEST(Canonicalize, Film) {
  EXPECT_THAT(MakeCanonical(R"pb(directives { film {} })pb"),
              EqualsProto(R"pb(directives { film {} })pb"));
}

// Unset Filter are left unset and should eventually cause rendering to fail
// https://github.com/mmp/pbrt-v3/blob/13d871faae88233b327d04cda24022b8bb0093ee/src/core/api.cpp#L852
TEST(Canonicalize, Filter) {
  EXPECT_THAT(MakeCanonical(R"pb(directives { filter {} })pb"),
              EqualsProto(R"pb(directives { filter {} })pb"));
}

// Unset Integrator are left unset and should eventually cause a black image to
// be rendered
// https://github.com/mmp/pbrt-v3/blob/13d871faae88233b327d04cda24022b8bb0093ee/src/core/api.cpp#L1619
TEST(Canonicalize, Integrator) {
  EXPECT_THAT(MakeCanonical(R"pb(directives { integrator {} })pb"),
              EqualsProto(R"pb(directives { integrator {} })pb"));
}

// Unset Sampler are left unset and should eventually cause a black image to be
// rendered
// https://github.com/mmp/pbrt-v3/blob/13d871faae88233b327d04cda24022b8bb0093ee/src/core/api.cpp#L1671
TEST(Canonicalize, Sampler) {
  EXPECT_THAT(MakeCanonical(R"pb(directives { sampler {} })pb"),
              EqualsProto(R"pb(directives { sampler {} })pb"));
}

}  // namespace
}  // namespace pbrt_proto::v3