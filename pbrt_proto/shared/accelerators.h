#ifndef _PBRT_PROTO_SHARED_ACCELERATORS_
#define _PBRT_PROTO_SHARED_ACCELERATORS_

#include "absl/container/flat_hash_map.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {

void RemoveGridAcceleratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    GridAccelerator& output);

void RemoveKdTreeAcceleratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    KdTreeAccelerator& output);

void RemoveBvhAcceleratorV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    BvhAccelerator& output);

void RemoveBvhAcceleratorV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    BvhAccelerator& output);

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_ACCELERATORS_
