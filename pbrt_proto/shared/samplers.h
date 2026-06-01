#ifndef _PBRT_PROTO_SHARED_SAMPLERS_
#define _PBRT_PROTO_SHARED_SAMPLERS_

#include "absl/container/flat_hash_map.h"
#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {

void RemoveAdaptiveSamplerV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    AdaptiveSampler& output);

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

void RemoveMaxMinDistSamplerV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    MaxMinDistSampler& output);

absl::Status RemovePaddedSobolSampler(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PaddedSobolSampler& output);

void RemovePMJ02BNSampler(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PMJ02BNSampler& output);

void RemoveRandomSamplerV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    RandomSampler& output);

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

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_SAMPLERS_
