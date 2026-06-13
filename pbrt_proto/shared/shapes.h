#ifndef _PBRT_PROTO_SHARED_SHAPES_
#define _PBRT_PROTO_SHARED_SHAPES_

#include "absl/container/flat_hash_map.h"
#include "absl/functional/function_ref.h"
#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {

absl::Status RemoveConeShape(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, ConeShape& output);

absl::Status TryRemoveCurveShape(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, absl::FunctionRef<CurveShape*()> get_output);

absl::Status RemoveCylinderShape(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, CylinderShape& output);

absl::Status RemoveDiskShape(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, DiskShape& output);

absl::Status RemoveHeightFieldShape(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, HeightFieldShape& output);

absl::Status RemoveHyperboloidShape(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, HyperboloidShape& output);

absl::Status RemoveLoopSubdivShape(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, LoopSubdivShape& output);

absl::Status RemoveNurbsShape(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, NurbsShape& output);

absl::Status RemoveParaboloidShape(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, ParaboloidShape& output);

absl::Status RemovePlyMeshShape(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, PlyMeshShape& output);

absl::Status RemoveSphereShape(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, SphereShape& output);

absl::Status RemoveTriangleMeshShape(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, TriangleMeshShape& output);

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_SHAPES_
