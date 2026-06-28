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
// Material and Material Overrides
//

const google::protobuf::Descriptor& MaterialV1();
const google::protobuf::Descriptor& MaterialOverridesV1();
const google::protobuf::Descriptor& MaterialV2();
const google::protobuf::Descriptor& MaterialOverridesV2();
const google::protobuf::Descriptor& MaterialV3();
const google::protobuf::Descriptor& MaterialOverridesV3();

//
// Unshared Descriptors
//

std::vector<const google::protobuf::Descriptor* absl_nonnull> AllPbrtV1();
const google::protobuf::Descriptor& TopLevelPbrtV1();

std::vector<const google::protobuf::Descriptor* absl_nonnull> AllPbrtV2();
const google::protobuf::Descriptor& TopLevelPbrtV2();

std::vector<const google::protobuf::Descriptor* absl_nonnull> AllPbrtV3();
const google::protobuf::Descriptor& TopLevelPbrtV3();

std::vector<const google::protobuf::Descriptor* absl_nonnull> AllPbrtV4();
const google::protobuf::Descriptor& TopLevelPbrtV4();

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_TESTING_DESCRIPTORS_
