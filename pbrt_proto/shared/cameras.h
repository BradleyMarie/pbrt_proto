#ifndef _PBRT_PROTO_SHARED_CAMERAS_
#define _PBRT_PROTO_SHARED_CAMERAS_

#include "absl/container/flat_hash_map.h"
#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {

absl::Status RemoveOrthographicCamera(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, OrthographicCamera& output);

absl::Status RemovePerspectiveCamera(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, PerspectiveCamera& output);

absl::Status RemoveSphericalCamera(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, SphericalCamera& output);

absl::Status RemoveRealisticCamera(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, RealisticCamera& output);

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_CAMERAS_
