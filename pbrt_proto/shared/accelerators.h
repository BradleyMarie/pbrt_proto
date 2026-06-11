#ifndef _PBRT_PROTO_SHARED_ACCELERATORS_
#define _PBRT_PROTO_SHARED_ACCELERATORS_

#include "absl/container/flat_hash_map.h"
#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {

absl::Status RemoveGridAccelerator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, GridAccelerator& output);

absl::Status RemoveKdTreeAccelerator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, KdTreeAccelerator& output);

absl::Status RemoveBvhAccelerator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, BvhAccelerator& output);

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_ACCELERATORS_
