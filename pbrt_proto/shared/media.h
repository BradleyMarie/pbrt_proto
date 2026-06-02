#ifndef _PBRT_PROTO_SHARED_MEDIA_
#define _PBRT_PROTO_SHARED_MEDIA_

#include "absl/container/flat_hash_map.h"
#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {

void RemoveCloudMediumV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    CloudMedium& output);

void RemoveHomogeneousMediumV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    HomogeneousMedium& output);

void RemoveHomogeneousMediumV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    HomogeneousMedium& output);

void RemoveNanoVdbMediumV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    NanoVdbMedium& output);

absl::Status RemoveRgbGridMediumV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    RgbGridMedium& output);

absl::Status RemoveUniformGridMediumV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    UniformGridMedium& output);

absl::Status RemoveUniformGridMediumV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    UniformGridMedium& output);

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_MEDIA_
