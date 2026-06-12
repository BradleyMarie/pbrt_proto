#ifndef _PBRT_PROTO_SHARED_SAMPLERS_
#define _PBRT_PROTO_SHARED_SAMPLERS_

#include "absl/container/flat_hash_map.h"
#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {

absl::Status RemoveAdaptiveSampler(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, AdaptiveSampler& output);

absl::Status RemoveBestCandidateSampler(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, BestCandidateSampler& output);

absl::Status RemoveHaltonSampler(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, HaltonSampler& output);

absl::Status RemoveIndependentSampler(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, IndependentSampler& output);

absl::Status RemoveMaxMinDistSampler(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, MaxMinDistSampler& output);

absl::Status RemovePaddedSobolSampler(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, PaddedSobolSampler& output);

absl::Status RemovePMJ02BNSampler(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, PMJ02BNSampler& output);

absl::Status RemoveRandomSampler(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, RandomSampler& output);

absl::Status RemoveSobolSampler(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, SobolSampler& output);

absl::Status RemoveStratifiedSampler(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, StratifiedSampler& output);

absl::Status RemoveZeroTwoSequenceSampler(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, ZeroTwoSequenceSampler& output);

absl::Status RemoveZSobolSampler(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, ZSobolSampler& output);

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_SAMPLERS_
