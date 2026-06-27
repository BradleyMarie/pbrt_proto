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

void AddAllFieldsByNumber(const Descriptor& descriptor,
                          absl::flat_hash_map<int, const FieldDescriptor*>&
                              field_descriptors_by_number) {
  for (int f = 0; f < descriptor.field_count(); f++) {
    const FieldDescriptor* field_descriptor = descriptor.field(f);
    if (!field_descriptor) {
      continue;
    }

    if (absl::StartsWith(field_descriptor->name(), "eta")) {
      continue;
    }

    if (auto iter =
            field_descriptors_by_number.find(field_descriptor->number());
        iter != field_descriptors_by_number.end()) {
      EXPECT_EQ(iter->second->type(), field_descriptor->type());
      EXPECT_EQ(iter->second->number(), field_descriptor->number());
      if (iter->second->type() == FieldDescriptor::TYPE_MESSAGE) {
        if (iter->second->message_type()->name() == "SurfaceIntegrator" &&
            field_descriptor->message_type()->name() == "Integrator") {
          continue;
        }

        EXPECT_EQ(iter->second->message_type()->name(),
                  field_descriptor->message_type()->name());
      }
      continue;
    }

    field_descriptors_by_number[field_descriptor->number()] = field_descriptor;
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

std::vector<std::pair<int, int>> GenerateVersionPairs(int max) {
  std::vector<std::pair<int, int>> result;
  for (int i = 1; i < max; i++) {
    for (int j = i + 1; j <= max; j++) {
      result.emplace_back(i, j);
    }
  }
  return result;
}

class Directives : public testing::TestWithParam<std::pair<int, int>> {};

TEST_P(Directives, ForwardCompatible) {
  auto [base, next] = GetParam();
  std::vector<std::vector<const Descriptor*>> all_descriptors =
      AllPbrtVersions();
  std::vector<const Descriptor*> base_descriptors = all_descriptors.at(base);

  absl::flat_hash_map<absl::string_view, const Descriptor*> next_descriptors;
  for (const Descriptor* descriptor : all_descriptors.at(next)) {
    next_descriptors[descriptor->name()] = descriptor;
  }

  for (const Descriptor* base_descriptor : base_descriptors) {
    absl::string_view next_name = base_descriptor->name();
    if (next_name == "SurfaceIntegrator" && next >= 3) {
      next_name = "Integrator";
    }

    auto iter = next_descriptors.find(next_name);
    if (iter == next_descriptors.end()) {
      continue;
    }

    const Descriptor* next_descriptor = iter->second;
    ASSERT_TRUE(base_descriptor);
    ASSERT_TRUE(next_descriptor);

    absl::flat_hash_map<int, const FieldDescriptor*>
        field_descriptors_by_number;
    AddAllFieldsByNumber(*base_descriptor, field_descriptors_by_number);
    AddAllFieldsByNumber(*next_descriptor, field_descriptors_by_number);
  }
}

INSTANTIATE_TEST_SUITE_P(AllDirectives, Directives,
                         testing::ValuesIn(GenerateVersionPairs(4)));

TEST(MaterialOverrides, AreBinaryCompatible) {
  absl::flat_hash_map<absl::string_view, const FieldDescriptor*>
      field_descriptors_by_name;
  absl::flat_hash_set<int> claimed_field_numbers;
  AddAllFieldsByNameAndNumber(MaterialOverridesV1(), field_descriptors_by_name,
                              claimed_field_numbers);
  AddAllFieldsByNameAndNumber(MaterialOverridesV2(), field_descriptors_by_name,
                              claimed_field_numbers);
  AddAllFieldsByNameAndNumber(MaterialOverridesV3(), field_descriptors_by_name,
                              claimed_field_numbers);

  for (const Descriptor* descriptor : Materials()) {
    AddAllFieldsByNameAndNumber(*descriptor, field_descriptors_by_name,
                                claimed_field_numbers);
  }
}

}  // namespace
}  // namespace pbrt_proto
