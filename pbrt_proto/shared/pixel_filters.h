#ifndef _PBRT_PROTO_SHARED_PIXEL_FILTERS_
#define _PBRT_PROTO_SHARED_PIXEL_FILTERS_

#include "absl/container/flat_hash_map.h"
#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {

absl::Status RemoveBoxPixelFilter(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, BoxPixelFilter& output);

absl::Status RemoveGaussianPixelFilter(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, GaussianPixelFilter& output);

absl::Status RemoveLanczosPixelFilter(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, LanczosPixelFilter& output);

absl::Status RemoveMitchellPixelFilter(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, MitchellPixelFilter& output);

absl::Status RemoveTrianglePixelFilter(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, TrianglePixelFilter& output);

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_PIXEL_FILTERS_
