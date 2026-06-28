#include <algorithm>
#include <set>
#include <string>

#include "gmock/gmock.h"
#include "google/protobuf/descriptor.pb.h"
#include "gtest/gtest.h"
#include "pbrt_proto/shared/version.h"
#include "pbrt_proto/testing/descriptors.h"
#include "pbrt_proto/testing/message_compatibility.h"

namespace pbrt_proto {
namespace {

using ::google::protobuf::Descriptor;
using ::google::protobuf::FieldDescriptor;

TEST(Shared, MutuallyCompatible) {
  for (const auto& [name, directives] : AllMessageGroups()) {
    for (const Descriptor* d0 : directives) {
      for (const Descriptor* d1 : directives) {
        EXPECT_TRUE(MessagesAreCompatible(*d0, *d1));
      }
    }
  }
}

TEST(Shared, FieldNumbersAreContiguous) {
  for (const auto& [name, directives] : AllMessageGroups()) {
    std::set<int> claimed_values;
    int max_field_number = 0;

    // Marble only exists as a SpectrumTexture, but its numbers are unused in
    // order to allow Float and Spectum textures to have the same field numbers.
    if (name == "FloatTexture") {
      claimed_values.insert(17);
      claimed_values.insert(18);
      max_field_number = 18;
    }

    for (int pbrt_version = 1; pbrt_version <= 4; pbrt_version++) {
      SCOPED_TRACE(name + "@PBRTv" + std::to_string(pbrt_version));

      for (const Descriptor* d0 : directives) {
        if (!GetMessageSupportedVersions(d0->full_name())
                 .Supported(pbrt_version)) {
          continue;
        }

        for (int f = 0; f < d0->field_count(); f++) {
          const FieldDescriptor* field_descriptor = d0->field(f);
          ASSERT_TRUE(field_descriptor);

          if (GetFieldSupportedVersions(field_descriptor->full_name())
                  .Supported(pbrt_version)) {
            max_field_number =
                std::max(max_field_number, field_descriptor->number());
            claimed_values.insert(field_descriptor->number());
          }
        }
      }

      EXPECT_EQ(max_field_number, static_cast<int>(claimed_values.size()));
    }
  }
}

}  // namespace
}  // namespace pbrt_proto
