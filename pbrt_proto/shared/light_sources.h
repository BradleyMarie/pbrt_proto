#ifndef _PBRT_PROTO_SHARED_LIGHT_SOURCES_
#define _PBRT_PROTO_SHARED_LIGHT_SOURCES_

#include "absl/container/flat_hash_map.h"
#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {

absl::Status RemoveDistantLightSourceV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DistantLightSource& output);

absl::Status RemoveDistantLightSourceV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DistantLightSource& output);

absl::Status RemoveDistantLightSourceV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DistantLightSource& output);

absl::Status RemoveGoniometricLightSourceV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    GoniometricLightSource& output);

absl::Status RemoveGoniometricLightSourceV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    GoniometricLightSource& output);

absl::Status RemoveGoniometricLightSourceV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    GoniometricLightSource& output);

absl::Status RemoveInfiniteLightSourceV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    InfiniteLightSource& output);

absl::Status RemoveInfiniteLightSourceV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    InfiniteLightSource& output);

absl::Status RemoveInfiniteLightSourceV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    InfiniteLightSource& output);

absl::Status RemoveInfiniteLightSourceV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    InfiniteLightSource& output);

absl::Status RemovePointLightSourceV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PointLightSource& output);

absl::Status RemovePointLightSourceV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PointLightSource& output);

absl::Status RemovePointLightSourceV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PointLightSource& output);

absl::Status RemoveProjectionLightSourceV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ProjectionLightSource& output);

absl::Status RemoveProjectionLightSourceV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ProjectionLightSource& output);

absl::Status RemoveProjectionLightSourceV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ProjectionLightSource& output);

absl::Status RemoveSpotLightSourceV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SpotLightSource& output);

absl::Status RemoveSpotLightSourceV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SpotLightSource& output);

absl::Status RemoveSpotLightSourceV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SpotLightSource& output);

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_LIGHT_SOURCES_
