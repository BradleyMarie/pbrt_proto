#include "gmock/gmock.h"
#include "google/protobuf/descriptor.pb.h"
#include "gtest/gtest.h"
#include "pbrt_proto/testing/descriptors.h"
#include "pbrt_proto/testing/message_compatibility.h"

namespace pbrt_proto {
namespace {

using ::google::protobuf::Descriptor;

TEST(Shared, MutuallyCompatible) {
  for (const auto& [name, directives] : AllMessageGroups()) {
    for (const Descriptor* d0 : directives) {
      for (const Descriptor* d1 : directives) {
        EXPECT_TRUE(MessagesAreCompatible(*d0, *d1));
      }
    }
  }
}

}  // namespace
}  // namespace pbrt_proto
