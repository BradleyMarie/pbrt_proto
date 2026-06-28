#include <algorithm>

#include "absl/strings/match.h"
#include "gmock/gmock.h"
#include "google/protobuf/descriptor.pb.h"
#include "gtest/gtest.h"
#include "pbrt_proto/shared/version.h"
#include "pbrt_proto/shared/version_set.h"
#include "pbrt_proto/testing/descriptors.h"
#include "pbrt_proto/testing/field_numbering.h"

namespace pbrt_proto::v1 {
namespace {

using ::google::protobuf::Descriptor;
using ::google::protobuf::FieldDescriptor;

TEST(PBRTv1, AllSupportV1) {
  for (const Descriptor* top_level_descriptors : AllPbrtV1()) {
    for (int f = 0; f < top_level_descriptors->field_count(); f++) {
      const Descriptor* descriptor =
          top_level_descriptors->field(f)->message_type();
      if (!descriptor) {
        continue;
      }

      VersionSet versions =
          GetMessageSupportedVersions(descriptor->full_name());
      if (versions.Empty()) {
        continue;
      }

      EXPECT_TRUE(versions.Supported(1));
    }
  }
}

TEST(PBRTv1, FieldNumbersAreContiguous) {
  EXPECT_TRUE(FieldsContinueContiguously(nullptr, &TopLevelPbrtV1()));
}

}  // namespace
}  // namespace pbrt_proto::v1
