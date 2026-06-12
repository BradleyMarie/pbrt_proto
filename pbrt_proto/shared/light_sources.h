#ifndef _PBRT_PROTO_SHARED_LIGHT_SOURCES_
#define _PBRT_PROTO_SHARED_LIGHT_SOURCES_

#include "absl/container/flat_hash_map.h"
#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {

absl::Status RemoveDistantLightSource(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, DistantLightSource& output);

absl::Status RemoveGoniometricLightSource(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, GoniometricLightSource& output);

absl::Status RemoveInfiniteLightSource(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, InfiniteLightSource& output);

absl::Status RemovePointLightSource(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, PointLightSource& output);

absl::Status RemoveProjectionLightSource(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, ProjectionLightSource& output);

absl::Status RemoveSpotLightSource(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, SpotLightSource& output);

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_LIGHT_SOURCES_
