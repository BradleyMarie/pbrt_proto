#ifndef _PBRT_PROTO_SHARED_COMMON_
#define _PBRT_PROTO_SHARED_COMMON_

#include <optional>

#include "absl/container/flat_hash_map.h"
#include "absl/functional/function_ref.h"
#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {

//
// Common Data
//

extern const absl::flat_hash_map<absl::string_view, ScatteringPreset::Type>
    kNamedScatteringPresets;

//
// Common Variant Data Types
//

absl::Status TryRemoveSpectrum(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name,
    absl::FunctionRef<Spectrum*()> get_output);

void TryRemoveFloatTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name,
    absl::FunctionRef<FloatTextureParameter*()> get_output);

absl::Status TryRemoveSpectrumTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name,
    absl::FunctionRef<SpectrumTextureParameter*()> get_output);

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_COMMON_
