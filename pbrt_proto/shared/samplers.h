#ifndef _PBRT_PROTO_SHARED_SAMPLERS_
#define _PBRT_PROTO_SHARED_SAMPLERS_

#include "absl/container/flat_hash_map.h"
#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {

void RemoveAdaptiveSamplerV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    AdaptiveSampler& output);

void RemoveBestCandidateSamplerV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    BestCandidateSampler& output);

void RemoveHaltonSamplerV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    HaltonSampler& output);

void RemoveHaltonSamplerV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    HaltonSampler& output);

absl::Status RemoveHaltonSamplerV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    HaltonSampler& output);

void RemoveIndependentSamplerV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    IndependentSampler& output);

void RemoveIndependentSamplerV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    IndependentSampler& output);

void RemoveMaxMinDistSamplerV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    MaxMinDistSampler& output);

absl::Status RemovePaddedSobolSamplerV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PaddedSobolSampler& output);

void RemovePMJ02BNSamplerV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PMJ02BNSampler& output);

void RemoveRandomSamplerV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    RandomSampler& output);

void RemoveSobolSamplerV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SobolSampler& output);

absl::Status RemoveSobolSamplerV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SobolSampler& output);

void RemoveStratifiedSamplerV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    StratifiedSampler& output);

void RemoveStratifiedSamplerV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    StratifiedSampler& output);

void RemoveStratifiedSamplerV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    StratifiedSampler& output);

void RemoveZeroTwoSequenceSamplerV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ZeroTwoSequenceSampler& output);

void RemoveZeroTwoSequenceSamplerV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ZeroTwoSequenceSampler& output);

absl::Status RemoveZSobolSamplerV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ZSobolSampler& output);

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_SAMPLERS_
