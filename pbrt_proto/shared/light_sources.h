#ifndef _PBRT_PROTO_SHARED_LIGHT_SOURCES_
#define _PBRT_PROTO_SHARED_LIGHT_SOURCES_

#include "absl/container/flat_hash_map.h"
#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {

void RemoveDistantLightSourceV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DistantLightSource& output);

void RemoveDistantLightSourceV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DistantLightSource& output);

void RemoveDistantLightSourceV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DistantLightSource& output);

void RemoveGoniometricLightSourceV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    GoniometricLightSource& output);

void RemoveGoniometricLightSourceV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    GoniometricLightSource& output);

void RemoveGoniometricLightSourceV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    GoniometricLightSource& output);

void RemoveInfiniteLightSourceV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    InfiniteLightSource& output);

void RemoveInfiniteLightSourceV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    InfiniteLightSource& output);

void RemoveInfiniteLightSourceV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    InfiniteLightSource& output);

void RemoveInfiniteLightSourceV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    InfiniteLightSource& output);

void RemovePointLightSourceV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PointLightSource& output);

void RemovePointLightSourceV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PointLightSource& output);

void RemovePointLightSourceV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PointLightSource& output);

void RemoveProjectionLightSourceV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ProjectionLightSource& output);

void RemoveProjectionLightSourceV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ProjectionLightSource& output);

void RemoveProjectionLightSourceV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ProjectionLightSource& output);

void RemoveSpotLightSourceV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SpotLightSource& output);

void RemoveSpotLightSourceV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SpotLightSource& output);

void RemoveSpotLightSourceV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SpotLightSource& output);

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_LIGHT_SOURCES_
