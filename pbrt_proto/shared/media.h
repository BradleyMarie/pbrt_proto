#ifndef _PBRT_PROTO_SHARED_MEDIA_
#define _PBRT_PROTO_SHARED_MEDIA_

#include "absl/container/flat_hash_map.h"
#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {

extern const absl::flat_hash_map<absl::string_view, MeasuredScatteringPreset>
    kNamedMeasuredScatteringPresets;

void RemoveCloudMediumV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    CloudMedium& output);

void RemoveHomogeneousMediumV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    HomogeneousMedium& output);

void RemoveHomogeneousMediumV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    HomogeneousMedium& output);

void RemoveNanoVdbMediumV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    NanoVdbMedium& output);

absl::Status RemoveRgbGridMediumV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    RgbGridMedium& output);

absl::Status RemoveUniformGridMediumV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    UniformGridMedium& output);

absl::Status RemoveUniformGridMediumV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    UniformGridMedium& output);

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_MEDIA_
