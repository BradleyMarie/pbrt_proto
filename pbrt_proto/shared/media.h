#ifndef _PBRT_PROTO_SHARED_MEDIA_
#define _PBRT_PROTO_SHARED_MEDIA_

#include "absl/container/flat_hash_map.h"
#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {

absl::Status RemoveCloudMedium(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, CloudMedium& output);

absl::Status RemoveExponentialMedium(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, ExponentialMedium& output);

absl::Status RemoveHomogeneousMedium(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, HomogeneousMedium& output);

absl::Status RemoveNanoVdbMedium(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, NanoVdbMedium& output);

absl::Status RemoveRgbGridMedium(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, RgbGridMedium& output);

absl::Status RemoveUniformGridMedium(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, UniformGridMedium& output);

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_MEDIA_
