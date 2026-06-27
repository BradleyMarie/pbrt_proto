#ifndef _PBRT_PROTO_TESTING_DESCRIPTORS_
#define _PBRT_PROTO_TESTING_DESCRIPTORS_

#include <string>
#include <vector>

#include "absl/base/nullability.h"
#include "google/protobuf/descriptor.pb.h"

namespace pbrt_proto {

//
// Message Groups
//

std::vector<const google::protobuf::Descriptor* absl_nonnull> Accelerators();
std::vector<const google::protobuf::Descriptor* absl_nonnull>
AreaLightSources();
std::vector<const google::protobuf::Descriptor* absl_nonnull> Cameras();
std::vector<const google::protobuf::Descriptor* absl_nonnull> Films();
std::vector<const google::protobuf::Descriptor* absl_nonnull> FloatTextures();
std::vector<const google::protobuf::Descriptor* absl_nonnull> Integrators();
std::vector<const google::protobuf::Descriptor* absl_nonnull> LightSources();
std::vector<const google::protobuf::Descriptor* absl_nonnull> Materials();
std::vector<const google::protobuf::Descriptor* absl_nonnull> Media();
std::vector<const google::protobuf::Descriptor* absl_nonnull> PixelFilters();
std::vector<const google::protobuf::Descriptor* absl_nonnull> Renderers();
std::vector<const google::protobuf::Descriptor* absl_nonnull> Samplers();
std::vector<const google::protobuf::Descriptor* absl_nonnull> Shapes();
std::vector<const google::protobuf::Descriptor* absl_nonnull>
SpectrumTextures();
std::vector<const google::protobuf::Descriptor* absl_nonnull>
VolumeIntegrators();

//
// All Message Groups
//

absl::flat_hash_map<
    std::string, std::vector<const google::protobuf::Descriptor* absl_nonnull>>
AllMessageGroups();

//
// Material Overrides Overrides
//

const google::protobuf::Descriptor& MaterialOverridesV1();
const google::protobuf::Descriptor& MaterialOverridesV2();
const google::protobuf::Descriptor& MaterialOverridesV3();
std::vector<const google::protobuf::Descriptor*> AllMaterialOverrides();

//
// Unshared Descriptors
//

std::vector<const google::protobuf::Descriptor* absl_nonnull> AllPbrtV1();
std::vector<const google::protobuf::Descriptor* absl_nonnull> AllPbrtV2();
std::vector<const google::protobuf::Descriptor* absl_nonnull> AllPbrtV3();
std::vector<const google::protobuf::Descriptor* absl_nonnull> AllPbrtV4();
std::vector<std::vector<const google::protobuf::Descriptor* absl_nonnull>>
AllPbrtVersions();

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_TESTING_DESCRIPTORS_
