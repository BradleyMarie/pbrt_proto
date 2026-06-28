#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "absl/strings/match.h"
#include "absl/strings/string_view.h"
#include "gmock/gmock.h"
#include "google/protobuf/descriptor.pb.h"
#include "gtest/gtest.h"
#include "pbrt_proto/testing/descriptors.h"

namespace pbrt_proto {
namespace {

using ::google::protobuf::Descriptor;
using ::google::protobuf::FieldDescriptor;
using ::google::protobuf::FileDescriptor;
using ::testing::Contains;
using ::testing::Not;

void AddAllFieldsByNameAndNumber(
    const Descriptor& descriptor,
    absl::flat_hash_map<absl::string_view, const FieldDescriptor*>&
        field_descriptors_by_name,
    absl::flat_hash_set<int>& claimed_field_numbers) {
  for (int f = 0; f < descriptor.field_count(); f++) {
    const FieldDescriptor* field_descriptor = descriptor.field(f);
    if (!field_descriptor) {
      continue;
    }

    if (absl::EndsWith(descriptor.name(), "Material") &&
        field_descriptor->name() == "eta") {
      continue;
    }

    if (absl::EndsWith(descriptor.name(), "Medium") &&
        (field_descriptor->name() == "sigma_a" ||
         field_descriptor->name() == "sigma_s" ||
         field_descriptor->name() == "Le")) {
      continue;
    }

    if (absl::EndsWith(descriptor.name(), "Integrator") &&
        (field_descriptor->name() == "maxdist" ||
         field_descriptor->name() == "maxdistance" ||
         field_descriptor->name() == "lightsampler" ||
         field_descriptor->name() == "lightsamplestrategy")) {
      continue;
    }

    if (descriptor.name() == "AdaptiveSampler" &&
        (field_descriptor->name() == "maxsamples")) {
      continue;
    }

    if (auto iter = field_descriptors_by_name.find(field_descriptor->name());
        iter != field_descriptors_by_name.end()) {
      EXPECT_EQ(iter->second->type(), field_descriptor->type());
      EXPECT_EQ(iter->second->type_name(), field_descriptor->type_name());
      EXPECT_EQ(iter->second->number(), field_descriptor->number());
      continue;
    }

    EXPECT_THAT(claimed_field_numbers,
                Not(Contains(field_descriptor->number())));

    field_descriptors_by_name[field_descriptor->name()] = field_descriptor;
    claimed_field_numbers.insert(field_descriptor->number());
  }
}

TEST(CommonTypes, AreBinaryCompatible) {
  for (auto& [directive, descriptors] : AllMessageGroups()) {
    absl::flat_hash_map<absl::string_view, const FieldDescriptor*>
        field_descriptors_by_name;
    absl::flat_hash_set<int> claimed_field_numbers;
    for (const Descriptor* descriptor : descriptors) {
      AddAllFieldsByNameAndNumber(*descriptor, field_descriptors_by_name,
                                  claimed_field_numbers);
    }
  }
}

}  // namespace
}  // namespace pbrt_proto
