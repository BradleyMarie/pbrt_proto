#ifndef _PBRT_PROTO_SHARED_AREA_LIGHT_SOURCES_
#define _PBRT_PROTO_SHARED_AREA_LIGHT_SOURCES_

#include "absl/container/flat_hash_map.h"
#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {

void RemoveDiffuseAreaLightSourceV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DiffuseAreaLightSourceV1& output);

void RemoveDiffuseAreaLightSourceV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DiffuseAreaLightSourceV1& output);

void RemoveDiffuseAreaLightSourceV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DiffuseAreaLightSourceV1& output);

absl::Status RemoveDiffuseAreaLightSourceV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DiffuseAreaLightSourceV2& output);

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_AREA_LIGHT_SOURCES_
