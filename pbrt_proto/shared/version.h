#ifndef _PBRT_PROTO_SHARED_VERSION_
#define _PBRT_PROTO_SHARED_VERSION_

#include <cassert>

#include "absl/container/flat_hash_map.h"
#include "absl/strings/string_view.h"

namespace pbrt_proto {

bool IsMessageSupported(absl::string_view full_path, int pbrt_version);
bool IsFieldSupported(absl::string_view full_path, int pbrt_version);
bool IsEnumValueSupported(absl::string_view full_path, int pbrt_version);

#ifndef NDEBUG

template <typename T>
bool IsSupported(int pbrt_version, const T& output) {
  return IsMessageSupported(output.GetDescriptor()->full_name(), pbrt_version);
}

#endif  // NDEBUG

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_VERSION_
