#include <cstdint>
#include <map>
#include <set>
#include <string>

#include "absl/strings/match.h"
#include "absl/strings/string_view.h"
#include "google/protobuf/descriptor.pb.h"
#include "googlemock/include/gmock/gmock.h"
#include "googletest/include/gtest/gtest.h"
#include "pbrt_proto/pbrt.pb.h"

namespace pbrt_proto {
namespace {

using ::google::protobuf::Descriptor;
using ::google::protobuf::FieldDescriptor;
using ::testing::Contains;
using ::testing::Not;

class CommonTypes : public testing::TestWithParam<std::string> {};

TEST_P(CommonTypes, AreBinaryCompatible) {
  const google::protobuf::FileDescriptor* file_descriptor =
      BlackbodySpectrum::descriptor()->file();
  ASSERT_TRUE(file_descriptor);

  std::map<absl::string_view, const FieldDescriptor*> field_descriptors_by_name;
  std::set<int> claimed_field_numbers;
  for (int m = 0; m < file_descriptor->message_type_count(); m++) {
    const Descriptor* message_descriptor = file_descriptor->message_type(m);
    if (!message_descriptor ||
        !absl::EndsWith(message_descriptor->full_name(), GetParam())) {
      continue;
    }

    if (GetParam() == "LightSource" &&
        absl::EndsWith(message_descriptor->full_name(), "AreaLightSource")) {
      continue;
    }

    for (int f = 0; f < message_descriptor->field_count(); f++) {
      const FieldDescriptor* field_descriptor = message_descriptor->field(f);
      if (!field_descriptor) {
        continue;
      }

      if (auto iter = field_descriptors_by_name.find(field_descriptor->name());
          iter != field_descriptors_by_name.end()) {
        EXPECT_EQ(iter->second->type(), field_descriptor->type());
        EXPECT_EQ(iter->second->type_name(), field_descriptor->type_name());
        continue;
      }

      EXPECT_THAT(claimed_field_numbers,
                  Not(Contains(field_descriptor->number())));

      field_descriptors_by_name[field_descriptor->name()] = field_descriptor;
      claimed_field_numbers.insert(field_descriptor->number());
    }
  }
}

INSTANTIATE_TEST_CASE_P(AllTypes, CommonTypes,
                        testing::Values("Accelerator", "AreaLightSource",
                                        "Camera", "Film", "LightSource",
                                        "Medium", "PixelFilter", "Sampler",
                                        "FloatTexture", "SpectrumTexture"));

}  // namespace
}  // namespace pbrt_proto
