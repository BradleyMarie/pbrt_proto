#include "gmock/gmock.h"
#include "google/protobuf/descriptor.pb.h"
#include "gtest/gtest.h"
#include "pbrt_proto/shared/version.h"
#include "pbrt_proto/shared/version_set.h"
#include "pbrt_proto/testing/descriptors.h"
#include "pbrt_proto/testing/message_compatibility.h"

namespace pbrt_proto::v4 {
namespace {

using ::google::protobuf::Descriptor;
using ::google::protobuf::FieldDescriptor;

TEST(PBRTv4, AllSupportV4) {
  for (const Descriptor* top_level_descriptors : AllPbrtV4()) {
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

      EXPECT_TRUE(versions.Supported(4));
    }
  }
}

TEST(PBRTv4, MessagesAreCompatible) {
  EXPECT_TRUE(MessagesAreCompatible(TopLevelPbrtV4(), TopLevelPbrtV1()));
  EXPECT_TRUE(MessagesAreCompatible(TopLevelPbrtV4(), TopLevelPbrtV2()));
  EXPECT_TRUE(MessagesAreCompatible(TopLevelPbrtV4(), TopLevelPbrtV3()));
}

}  // namespace
}  // namespace pbrt_proto::v4
