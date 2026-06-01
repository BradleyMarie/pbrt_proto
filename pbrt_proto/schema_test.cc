#include <cstdint>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "absl/strings/match.h"
#include "absl/strings/string_view.h"
#include "google/protobuf/descriptor.pb.h"
#include "googlemock/include/gmock/gmock.h"
#include "googletest/include/gtest/gtest.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/v1/v1.pb.h"
#include "pbrt_proto/v2/v2.pb.h"
#include "pbrt_proto/v3/v3.pb.h"
#include "pbrt_proto/v4/v4.pb.h"

namespace pbrt_proto {
namespace {

using ::google::protobuf::Descriptor;
using ::google::protobuf::FieldDescriptor;
using ::google::protobuf::FileDescriptor;
using ::testing::Contains;
using ::testing::Not;

void AddAllFieldsByNameAndNumber(
    const Descriptor& descriptor,
    std::map<absl::string_view, const FieldDescriptor*>&
        field_descriptors_by_name,
    std::set<int>& claimed_field_numbers) {
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
         field_descriptor->name() == "maxdistance")) {
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

void AddAllFieldsByNumber(
    const Descriptor& descriptor,
    std::map<int, const FieldDescriptor*>& field_descriptors_by_number) {
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

class CommonTypes : public testing::TestWithParam<std::string> {};

TEST_P(CommonTypes, AreBinaryCompatible) {
  const FileDescriptor* file_descriptor =
      BlackbodySpectrum::descriptor()->file();
  ASSERT_TRUE(file_descriptor);

  std::map<absl::string_view, const FieldDescriptor*> field_descriptors_by_name;
  std::set<int> claimed_field_numbers;
  for (int m = 0; m < file_descriptor->message_type_count(); m++) {
    const Descriptor* message_descriptor = file_descriptor->message_type(m);
    ASSERT_NE(message_descriptor, nullptr);

    if (!absl::EndsWith(message_descriptor->full_name(), GetParam())) {
      continue;
    }

    if (GetParam() == "LightSource" &&
        absl::EndsWith(message_descriptor->full_name(), "AreaLightSource")) {
      continue;
    }

    if (GetParam() == "Material" &&
        absl::EndsWith(message_descriptor->full_name(), "NamedMaterial")) {
      continue;
    }

    AddAllFieldsByNameAndNumber(*message_descriptor, field_descriptors_by_name,
                                claimed_field_numbers);
  }
}

INSTANTIATE_TEST_CASE_P(AllTypes, CommonTypes,
                        testing::Values("Accelerator", "AreaLightSource",
                                        "Camera", "Film", "FloatTexture",
                                        "Integrator", "LightSource", "Material",
                                        "Medium", "PixelFilter", "Sampler",
                                        "Shape", "SpectrumTexture"));

std::vector<std::pair<int, int>> GenerateVersionPairs(int max) {
  std::vector<std::pair<int, int>> result;
  for (int i = 1; i < max; i++) {
    for (int j = i + 1; j <= max; j++) {
      result.emplace_back(i, j);
    }
  }
  return result;
}

std::map<std::string, const Descriptor*> GetMessageDescriptors(int version) {
  static std::vector<const FileDescriptor*> file_descriptors = {
      v1::Accelerator::GetDescriptor()->file(),
      v2::Accelerator::GetDescriptor()->file(),
      v3::Accelerator::GetDescriptor()->file(),
      v4::Accelerator::GetDescriptor()->file(),
  };

  const FileDescriptor* file_descriptor = file_descriptors.at(version - 1);

  std::map<std::string, const Descriptor*> result;
  for (int m = 0; m < file_descriptor->message_type_count(); m++) {
    const Descriptor* message_descriptor = file_descriptor->message_type(m);
    result.emplace(message_descriptor->name(), message_descriptor);
  }

  return result;
}

class Directives : public testing::TestWithParam<std::pair<int, int>> {};

TEST_P(Directives, ForwardCompatible) {
  auto [base, next] = GetParam();
  std::map<std::string, const Descriptor*> base_descriptors =
      GetMessageDescriptors(base);
  std::map<std::string, const Descriptor*> next_descriptors =
      GetMessageDescriptors(next);

  for (const auto& [name, base_descriptor] : base_descriptors) {
    std::string next_name = name;
    if (name == "SurfaceIntegrator" && next >= 3) {
      next_name = "Integrator";
    }

    auto iter = next_descriptors.find(next_name);
    if (iter == next_descriptors.end()) {
      continue;
    }

    const Descriptor* next_descriptor = iter->second;
    ASSERT_TRUE(base_descriptor);
    ASSERT_TRUE(next_descriptor);

    std::map<int, const FieldDescriptor*> field_descriptors_by_number;
    AddAllFieldsByNumber(*base_descriptor, field_descriptors_by_number);
    AddAllFieldsByNumber(*next_descriptor, field_descriptors_by_number);
  }
}

INSTANTIATE_TEST_CASE_P(AllDirectives, Directives,
                        testing::ValuesIn(GenerateVersionPairs(4)));

TEST(MaterialOverrides, AreBinaryCompatible) {
  const FileDescriptor* file_descriptor =
      BlackbodySpectrum::descriptor()->file();
  ASSERT_TRUE(file_descriptor);

  std::map<absl::string_view, const FieldDescriptor*> field_descriptors_by_name;
  std::set<int> claimed_field_numbers;
  AddAllFieldsByNameAndNumber(*v1::Shape::MaterialOverrides::GetDescriptor(),
                              field_descriptors_by_name, claimed_field_numbers);
  AddAllFieldsByNameAndNumber(*v2::Shape::MaterialOverrides::GetDescriptor(),
                              field_descriptors_by_name, claimed_field_numbers);
  AddAllFieldsByNameAndNumber(*v3::Shape::MaterialOverrides::GetDescriptor(),
                              field_descriptors_by_name, claimed_field_numbers);

  for (int m = 0; m < file_descriptor->message_type_count(); m++) {
    const Descriptor* message_descriptor = file_descriptor->message_type(m);
    ASSERT_NE(message_descriptor, nullptr);

    if (!absl::EndsWith(message_descriptor->full_name(), "Material") ||
        absl::EndsWith(message_descriptor->full_name(), "NamedMaterial")) {
      continue;
    }

    AddAllFieldsByNameAndNumber(*message_descriptor, field_descriptors_by_name,
                                claimed_field_numbers);
  }
}

}  // namespace
}  // namespace pbrt_proto
