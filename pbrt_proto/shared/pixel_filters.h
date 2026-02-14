#ifndef _PBRT_PROTO_SHARED_PIXEL_FILTERS_
#define _PBRT_PROTO_SHARED_PIXEL_FILTERS_

#include "absl/container/flat_hash_map.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {

void RemoveBoxPixelFilterV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    BoxPixelFilter& output);

void RemoveBoxPixelFilterV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    BoxPixelFilter& output);

void RemoveGaussianPixelFilterV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    GaussianPixelFilter& output);

void RemoveGaussianPixelFilterV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    GaussianPixelFilter& output);

void RemoveLanczosPixelFilterV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    LanczosPixelFilter& output);

void RemoveLanczosPixelFilterV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    LanczosPixelFilter& output);

void RemoveMitchellPixelFilterV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    MitchellPixelFilter& output);

void RemoveMitchellPixelFilterV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    MitchellPixelFilter& output);

void RemoveTrianglePixelFilterV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    TrianglePixelFilter& output);

void RemoveTrianglePixelFilterV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    TrianglePixelFilter& output);

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_PIXEL_FILTERS_
