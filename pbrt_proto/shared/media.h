#ifndef _PBRT_PROTO_SHARED_MEDIA_
#define _PBRT_PROTO_SHARED_MEDIA_

#include "absl/container/flat_hash_map.h"
#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {

absl::Status RemoveCloudMediumV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    CloudMedium& output);

absl::Status RemoveExponentialMediumV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ExponentialMedium& output);

absl::Status RemoveHomogeneousMediumV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    HomogeneousMedium& output);

absl::Status RemoveHomogeneousMediumV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    HomogeneousMedium& output);

absl::Status RemoveHomogeneousMediumV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    HomogeneousMedium& output);

absl::Status RemoveNanoVdbMediumV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    NanoVdbMedium& output);

absl::Status RemoveRgbGridMediumV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    RgbGridMedium& output);

absl::Status RemoveUniformGridMediumV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    UniformGridMedium& output);

absl::Status RemoveUniformGridMediumV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    UniformGridMedium& output);

absl::Status RemoveUniformGridMediumV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    UniformGridMedium& output);

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_MEDIA_
