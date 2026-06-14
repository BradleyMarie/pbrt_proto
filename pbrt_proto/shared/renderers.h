#ifndef _PBRT_PROTO_SHARED_RENDERERS_
#define _PBRT_PROTO_SHARED_RENDERERS_

#include "absl/container/flat_hash_map.h"
#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {

absl::Status RemoveAggregateTestRenderer(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, AggregateTestRenderer& output);

absl::Status RemoveCreateProbesRenderer(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, CreateProbesRenderer& output);

absl::Status RemoveMetropolisRenderer(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, MetropolisRenderer& output);

absl::Status RemoveSamplerRenderer(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, SamplerRenderer& output);

absl::Status RemoveSurfacePointsRenderer(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, SurfacePointsRenderer& output);

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_RENDERERS_
