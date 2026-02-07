#ifndef _PBRT_PROTO_SHARED_CAMERAS_
#define _PBRT_PROTO_SHARED_CAMERAS_

#include "absl/container/flat_hash_map.h"
#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {

void RemoveOrthographicCameraV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    OrthographicCamera& output);

void RemoveOrthographicCameraV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    OrthographicCamera& output);

void RemovePerspectiveCameraV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PerspectiveCamera& output);

void RemovePerspectiveCameraV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PerspectiveCamera& output);

void RemovePerspectiveCameraV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PerspectiveCamera& output);

void RemoveSphericalCameraV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SphericalCamera& output);

void RemoveSphericalCameraV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SphericalCamera& output);

absl::Status RemoveSphericalCameraV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SphericalCamera& output);

void RemoveRealisticCameraV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    RealisticCamera& output);

void RemoveRealisticCameraV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    RealisticCamera& output);

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_CAMERAS_
