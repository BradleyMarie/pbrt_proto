#ifndef _PBRT_PROTO_SHARED_SHAPES_
#define _PBRT_PROTO_SHARED_SHAPES_

#include "absl/container/flat_hash_map.h"
#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {

void RemoveConeShapeV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ConeShape& output);

void RemoveCylinderShapeV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    CylinderShape& output);

void RemoveDiskShapeV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DiskShape& output);

absl::Status RemoveHeightFieldShapeV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    HeightFieldShape& output);

void RemoveHyperboloidShapeV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    HyperboloidShape& output);

absl::Status RemoveLoopSubdivShapeV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    LoopSubdivShape& output);

absl::Status RemoveNurbsShapeV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    NurbsShape& output);

void RemoveParaboloidShapeV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ParaboloidShape& output);

void RemoveSphereShapeV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SphereShape& output);

absl::Status RemoveTriangleMeshShapeV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    TriangleMeshShape& output);

absl::Status RemoveTriangleMeshShapeV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    TriangleMeshShape& output);

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_SHAPES_
