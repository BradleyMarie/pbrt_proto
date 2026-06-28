#include "gmock/gmock.h"
#include "google/protobuf/descriptor.pb.h"
#include "gtest/gtest.h"
#include "pbrt_proto/shared/version.h"
#include "pbrt_proto/shared/version_set.h"
#include "pbrt_proto/testing/descriptors.h"
#include "pbrt_proto/testing/field_numbering.h"
#include "pbrt_proto/testing/message_compatibility.h"

namespace pbrt_proto::v3 {
namespace {

using ::google::protobuf::Descriptor;
using ::google::protobuf::FieldDescriptor;

TEST(PBRTv3, AllSupportV3) {
  for (const Descriptor* top_level_descriptors : AllPbrtV3()) {
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

      EXPECT_TRUE(versions.Supported(3));
    }
  }
}

TEST(PBRTv3, FieldNumbersAreContiguous) {
  EXPECT_TRUE(FieldsContinueContiguously(&TopLevelPbrtV3(), &TopLevelPbrtV3()));
}

TEST(PBRTv3, MessagesAreCompatible) {
  EXPECT_TRUE(MessagesAreCompatible(TopLevelPbrtV1(), TopLevelPbrtV3()));
  EXPECT_TRUE(MessagesAreCompatible(TopLevelPbrtV3(), TopLevelPbrtV3()));
}

TEST(PBRTv3, MaterialOverridesAreCompatible) {
  for (int f = 0; f < MaterialV3().field_count(); f++) {
    const FieldDescriptor* field_descriptor = MaterialV3().field(f);
    ASSERT_TRUE(field_descriptor);

    const Descriptor* descriptor = field_descriptor->message_type();
    if (!descriptor) {
      continue;
    }

    EXPECT_TRUE(MessagesAreCompatible(*descriptor, MaterialOverridesV3()));
  }
}

}  // namespace
}  // namespace pbrt_proto::v3
