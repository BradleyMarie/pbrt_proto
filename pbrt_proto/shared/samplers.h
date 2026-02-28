#ifndef _PBRT_PROTO_SHARED_SAMPLERS_
#define _PBRT_PROTO_SHARED_SAMPLERS_

#include "absl/container/flat_hash_map.h"
#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {

void RemoveBestCandidateSampler(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    BestCandidateSampler& output);

void RemoveHaltonSamplerV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    HaltonSampler& output);

void RemoveHaltonSamplerV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    HaltonSampler& output);

absl::Status RemoveHaltonSamplerV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    HaltonSampler& output);

void RemoveIndependentSamplerV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    IndependentSampler& output);

void RemoveIndependentSamplerV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    IndependentSampler& output);

absl::Status RemovePaddedSobolSampler(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PaddedSobolSampler& output);

void RemovePMJ02BNSampler(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PMJ02BNSampler& output);

void RemoveSobolSamplerV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SobolSampler& output);

absl::Status RemoveSobolSamplerV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SobolSampler& output);

void RemoveStratifiedSamplerV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    StratifiedSampler& output);

void RemoveStratifiedSamplerV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    StratifiedSampler& output);

void RemoveStratifiedSamplerV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    StratifiedSampler& output);

void RemoveZeroTwoSequenceSamplerV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ZeroTwoSequenceSampler& output);

void RemoveZeroTwoSequenceSamplerV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ZeroTwoSequenceSampler& output);

absl::Status RemoveZSobolSampler(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ZSobolSampler& output);

//
// Parameters
//

template <typename T>
void RemovePixelSamples(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters, T& output) {
  if (std::optional<int32_t> pixelsamples =
          TryRemoveInteger(parameters, "pixelsamples");
      pixelsamples.has_value()) {
    output.set_pixelsamples(std::max(0, *pixelsamples));
  }
}

template <typename T>
void RemoveDimension(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters, T& output) {
  if (std::optional<int32_t> dimensions =
          TryRemoveInteger(parameters, "dimensions");
      dimensions.has_value()) {
    output.set_dimensions(std::max(0, *dimensions));
  }
}

template <typename T>
void RemoveXSamples(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters, T& output) {
  if (std::optional<int32_t> xsamples =
          TryRemoveInteger(parameters, "xsamples");
      xsamples.has_value()) {
    output.set_xsamples(std::max(0, *xsamples));
  }
}

template <typename T>
void RemoveYSamples(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters, T& output) {
  if (std::optional<int32_t> ysamples =
          TryRemoveInteger(parameters, "ysamples");
      ysamples.has_value()) {
    output.set_ysamples(std::max(0, *ysamples));
  }
}

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_SAMPLERS_
