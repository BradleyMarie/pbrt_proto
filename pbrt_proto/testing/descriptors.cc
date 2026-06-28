#include "pbrt_proto/testing/descriptors.h"

#include <vector>

#include "absl/base/nullability.h"
#include "absl/functional/function_ref.h"
#include "absl/log/absl_check.h"
#include "absl/strings/match.h"
#include "google/protobuf/descriptor.pb.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/v1/v1.pb.h"
#include "pbrt_proto/v2/v2.pb.h"
#include "pbrt_proto/v3/v3.pb.h"
#include "pbrt_proto/v4/v4.pb.h"

namespace pbrt_proto {
namespace {

using ::google::protobuf::Descriptor;
using ::google::protobuf::FileDescriptor;

std::vector<const Descriptor* absl_nonnull> GetAllMessages(
    const FileDescriptor* file_descriptor,
    absl::FunctionRef<bool(const Descriptor&)> predicate =
        [](const Descriptor&) { return true; }) {
  ABSL_CHECK(file_descriptor);

  std::vector<const Descriptor* absl_nonnull> result;
  for (int m = 0; m < file_descriptor->message_type_count(); m++) {
    const Descriptor* descriptor = file_descriptor->message_type(m);
    ABSL_CHECK(descriptor);

    if (predicate(*descriptor)) {
      result.push_back(descriptor);
    }
  }

  return result;
}

std::vector<const Descriptor* absl_nonnull> GetSharedMessages(
    absl::FunctionRef<bool(const Descriptor&)> predicate) {
  return GetAllMessages(BlackbodySpectrum::descriptor()->file(), predicate);
}

}  // namespace

std::vector<const Descriptor* absl_nonnull> Accelerators() {
  return GetSharedMessages([](const Descriptor& descriptor) {
    return absl::EndsWith(descriptor.name(), "Accelerator");
  });
}

std::vector<const Descriptor* absl_nonnull> AreaLightSources() {
  return GetSharedMessages([](const Descriptor& descriptor) {
    return absl::EndsWith(descriptor.name(), "AreaLightSource");
  });
}

std::vector<const Descriptor* absl_nonnull> Cameras() {
  return GetSharedMessages([](const Descriptor& descriptor) {
    return absl::EndsWith(descriptor.name(), "Camera");
  });
}

std::vector<const Descriptor* absl_nonnull> Films() {
  return GetSharedMessages([](const Descriptor& descriptor) {
    return absl::EndsWith(descriptor.name(), "Film");
  });
}

std::vector<const Descriptor* absl_nonnull> FloatTextures() {
  return GetSharedMessages([](const Descriptor& descriptor) {
    return absl::EndsWith(descriptor.name(), "FloatTexture");
  });
}

std::vector<const Descriptor* absl_nonnull> Integrators() {
  return GetSharedMessages([](const Descriptor& descriptor) {
    return absl::EndsWith(descriptor.name(), "Integrator") &&
           !absl::EndsWith(descriptor.name(), "VolumeIntegrator");
  });
}

std::vector<const Descriptor* absl_nonnull> LightSources() {
  return GetSharedMessages([](const Descriptor& descriptor) {
    return absl::EndsWith(descriptor.name(), "LightSource") &&
           !absl::EndsWith(descriptor.name(), "AreaLightSource");
  });
}

std::vector<const Descriptor* absl_nonnull> Materials() {
  return GetSharedMessages([](const Descriptor& descriptor) {
    return absl::EndsWith(descriptor.name(), "Material") &&
           !absl::EndsWith(descriptor.name(), "NamedMaterial");
  });
}

std::vector<const Descriptor* absl_nonnull> Media() {
  return GetSharedMessages([](const Descriptor& descriptor) {
    return absl::EndsWith(descriptor.name(), "Medium");
  });
}

std::vector<const Descriptor* absl_nonnull> PixelFilters() {
  return GetSharedMessages([](const Descriptor& descriptor) {
    return absl::EndsWith(descriptor.name(), "PixelFilter");
  });
}

std::vector<const Descriptor* absl_nonnull> Renderers() {
  return GetSharedMessages([](const Descriptor& descriptor) {
    return absl::EndsWith(descriptor.name(), "Renderer");
  });
}

std::vector<const Descriptor* absl_nonnull> Samplers() {
  return GetSharedMessages([](const Descriptor& descriptor) {
    return absl::EndsWith(descriptor.name(), "Sampler");
  });
}

std::vector<const Descriptor* absl_nonnull> Shapes() {
  return GetSharedMessages([](const Descriptor& descriptor) {
    return absl::EndsWith(descriptor.name(), "Shape");
  });
}

std::vector<const Descriptor* absl_nonnull> SpectrumTextures() {
  return GetSharedMessages([](const Descriptor& descriptor) {
    return absl::EndsWith(descriptor.name(), "SpectrumTexture");
  });
}

std::vector<const Descriptor* absl_nonnull> VolumeIntegrators() {
  return GetSharedMessages([](const Descriptor& descriptor) {
    return absl::EndsWith(descriptor.name(), "VolumeIntegrators");
  });
}

absl::flat_hash_map<
    std::string, std::vector<const google::protobuf::Descriptor* absl_nonnull>>
AllMessageGroups() {
  return {
      {"Accelerator", Accelerators()},
      {"AreaLightSource", AreaLightSources()},
      {"Camera", Cameras()},
      {"Film", Films()},
      {"FloatTexture", FloatTextures()},
      {"Integrator", Integrators()},
      {"LightSource", LightSources()},
      {"Material", Materials()},
      {"Medium", Media()},
      {"PixelFilter", PixelFilters()},
      {"Renderer", Renderers()},
      {"Sampler", Samplers()},
      {"Shape", Shapes()},
      {"SpectrumTexture", SpectrumTextures()},
      {"VolumeIntegrator", VolumeIntegrators()},
  };
}

const google::protobuf::Descriptor& MaterialV1() {
  const Descriptor* result = v1::Material::GetDescriptor();
  ABSL_CHECK(result);
  return *result;
}

const google::protobuf::Descriptor& MaterialOverridesV1() {
  const Descriptor* result = v1::Shape::MaterialOverrides::GetDescriptor();
  ABSL_CHECK(result);
  return *result;
}

const google::protobuf::Descriptor& MaterialV2() {
  const Descriptor* result = v2::Material::GetDescriptor();
  ABSL_CHECK(result);
  return *result;
}

const google::protobuf::Descriptor& MaterialOverridesV2() {
  const Descriptor* result = v2::Shape::MaterialOverrides::GetDescriptor();
  ABSL_CHECK(result);
  return *result;
}

const google::protobuf::Descriptor& MaterialV3() {
  const Descriptor* result = v3::Material::GetDescriptor();
  ABSL_CHECK(result);
  return *result;
}

const google::protobuf::Descriptor& MaterialOverridesV3() {
  const Descriptor* result = v3::Shape::MaterialOverrides::GetDescriptor();
  ABSL_CHECK(result);
  return *result;
}

std::vector<const google::protobuf::Descriptor* absl_nonnull> AllPbrtV1() {
  return GetAllMessages(v1::Accelerator::GetDescriptor()->file());
}

const google::protobuf::Descriptor& TopLevelPbrtV1() {
  const Descriptor* descriptor = v1::PbrtProto::GetDescriptor();
  ABSL_CHECK(descriptor);
  return *descriptor;
}

std::vector<const google::protobuf::Descriptor* absl_nonnull> AllPbrtV2() {
  return GetAllMessages(v2::Accelerator::GetDescriptor()->file());
}

const google::protobuf::Descriptor& TopLevelPbrtV2() {
  const Descriptor* descriptor = v2::PbrtProto::GetDescriptor();
  ABSL_CHECK(descriptor);
  return *descriptor;
}

std::vector<const google::protobuf::Descriptor* absl_nonnull> AllPbrtV3() {
  return GetAllMessages(v3::Accelerator::GetDescriptor()->file());
}

const google::protobuf::Descriptor& TopLevelPbrtV3() {
  const Descriptor* descriptor = v3::PbrtProto::GetDescriptor();
  ABSL_CHECK(descriptor);
  return *descriptor;
}

std::vector<const google::protobuf::Descriptor* absl_nonnull> AllPbrtV4() {
  return GetAllMessages(v4::Accelerator::GetDescriptor()->file());
}

const google::protobuf::Descriptor& TopLevelPbrtV4() {
  const Descriptor* descriptor = v4::PbrtProto::GetDescriptor();
  ABSL_CHECK(descriptor);
  return *descriptor;
}

}  // namespace pbrt_proto
