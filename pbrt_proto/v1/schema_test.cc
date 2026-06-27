#include <algorithm>

#include "absl/strings/match.h"
#include "gmock/gmock.h"
#include "google/protobuf/descriptor.pb.h"
#include "gtest/gtest.h"
#include "pbrt_proto/shared/version.h"
#include "pbrt_proto/shared/version_set.h"
#include "pbrt_proto/testing/descriptors.h"

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
  auto check_recursive = [](const auto& self,
                            const Descriptor* descriptor) -> void {
    SCOPED_TRACE(descriptor->full_name());

    int max_field_count = 0;
    for (int f = 0; f < descriptor->field_count(); f++) {
      const FieldDescriptor* field_descriptor = descriptor->field(f);
      ASSERT_TRUE(field_descriptor);

      const Descriptor* message_type = field_descriptor->message_type();
      if (!message_type) {
        continue;
      }

      if (absl::StartsWith(message_type->full_name(), "pbrt_proto.v1")) {
        self(self, field_descriptor->message_type());
      }

      max_field_count = std::max(max_field_count, field_descriptor->number());
    }

    EXPECT_EQ(max_field_count, descriptor->field_count());
  };

  check_recursive(check_recursive, &TopLevelPbrtV1());
}

}  // namespace
}  // namespace pbrt_proto::v1
