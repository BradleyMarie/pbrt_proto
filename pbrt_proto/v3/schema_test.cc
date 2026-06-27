#include "gmock/gmock.h"
#include "google/protobuf/descriptor.pb.h"
#include "gtest/gtest.h"
#include "pbrt_proto/shared/version.h"
#include "pbrt_proto/shared/version_set.h"
#include "pbrt_proto/testing/descriptors.h"

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

}  // namespace
}  // namespace pbrt_proto::v3
