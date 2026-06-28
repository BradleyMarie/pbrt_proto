#ifndef _PBRT_PROTO_SHARED_VERSION_
#define _PBRT_PROTO_SHARED_VERSION_

#include "absl/container/flat_hash_map.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/shared/version_set.h"

namespace pbrt_proto {

VersionSet GetMessageSupportedVersions(absl::string_view full_path);
VersionSet GetFieldSupportedVersions(absl::string_view full_path);
VersionSet GetEnumSupportedVersions(absl::string_view full_path);

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_VERSION_
