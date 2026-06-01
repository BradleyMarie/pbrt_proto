#include "pbrt_proto/shared/samplers.h"

#include "absl/container/flat_hash_map.h"
#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {
namespace {

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

template <typename T>
void RemoveJitter(absl::flat_hash_map<absl::string_view, Parameter>& parameters,
                  T& output) {
  if (std::optional<bool> jitter = TryRemoveBool(parameters, "jitter");
      jitter.has_value()) {
    output.set_jitter(*jitter);
  }
}

template <typename T>
absl::Status RemoveRandomization(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters, T& output) {
  if (std::optional<absl::string_view> randomization =
          TryRemoveString(parameters, "randomization");
      randomization.has_value()) {
    if (*randomization == "none") {
      output.set_randomization(NONE);
    } else if (*randomization == "permutedigits") {
      output.set_randomization(PERMUTEDIGITS);
    } else if (*randomization == "owen") {
      output.set_randomization(OWEN);
    } else if (*randomization == "fastowen") {
      output.set_randomization(FASTOWEN);
    } else {
      return absl::InvalidArgumentError(
          "A Sampler specified an invalid 'randomization'");
    }
  }

  return absl::OkStatus();
}

template <typename T>
void RemoveSeed(absl::flat_hash_map<absl::string_view, Parameter>& parameters,
                T& output) {
  if (std::optional<int32_t> seed = TryRemoveInteger(parameters, "seed");
      seed.has_value()) {
    output.set_seed(*seed);
  }
}

}  // namespace

void RemoveAdaptiveSamplerV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    AdaptiveSampler& output) {
  if (std::optional<int32_t> minsamples =
          TryRemoveInteger(parameters, "minsamples");
      minsamples.has_value()) {
    output.set_minsamples(std::max(0, *minsamples));
  }

  if (std::optional<int32_t> maxsamples =
          TryRemoveInteger(parameters, "maxsamples");
      maxsamples.has_value()) {
    output.set_maxsamples(std::max(0, *maxsamples));
  }

  if (std::optional<absl::string_view> method =
          TryRemoveString(parameters, "method");
      method.has_value()) {
    if (*method == "contrast") {
      output.set_method(AdaptiveSampler::CONTRAST);
    } else if (*method == "shapeid") {
      output.set_method(AdaptiveSampler::SHAPEID);
    } else {
      std::cerr
          << "Unsupported value for 'adaptive' Sampler parameter 'method': \""
          << *method << "\"" << std::endl;
      output.set_method(AdaptiveSampler::CONTRAST);
    }
  }
}

void RemoveBestCandidateSampler(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    BestCandidateSampler& output) {
  RemovePixelSamples(parameters, output);
}

void RemoveHaltonSamplerV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    HaltonSampler& output) {
  RemovePixelSamples(parameters, output);
}

void RemoveHaltonSamplerV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    HaltonSampler& output) {
  RemovePixelSamples(parameters, output);

  if (std::optional<bool> samplepixelcenter =
          TryRemoveBool(parameters, "samplepixelcenter");
      samplepixelcenter.has_value()) {
    output.set_samplepixelcenter(*samplepixelcenter);
  }
}

absl::Status RemoveHaltonSamplerV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    HaltonSampler& output) {
  RemovePixelSamples(parameters, output);
  RemoveSeed(parameters, output);
  return RemoveRandomization(parameters, output);
}

void RemoveIndependentSamplerV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    IndependentSampler& output) {
  RemovePixelSamples(parameters, output);
}

void RemoveIndependentSamplerV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    IndependentSampler& output) {
  RemovePixelSamples(parameters, output);
  RemoveSeed(parameters, output);
}

void RemoveMaxMinDistSamplerV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    MaxMinDistSampler& output) {
  RemovePixelSamples(parameters, output);
  RemoveDimension(parameters, output);
}

absl::Status RemovePaddedSobolSampler(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PaddedSobolSampler& output) {
  RemovePixelSamples(parameters, output);
  RemoveSeed(parameters, output);
  return RemoveRandomization(parameters, output);
}

void RemovePMJ02BNSampler(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PMJ02BNSampler& output) {
  RemovePixelSamples(parameters, output);
  RemoveSeed(parameters, output);
}

void RemoveRandomSamplerV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    RandomSampler& output) {
  RemoveXSamples(parameters, output);
  RemoveYSamples(parameters, output);
}

void RemoveSobolSamplerV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SobolSampler& output) {
  RemovePixelSamples(parameters, output);
}

absl::Status RemoveSobolSamplerV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SobolSampler& output) {
  RemovePixelSamples(parameters, output);
  RemoveSeed(parameters, output);
  return RemoveRandomization(parameters, output);
}

void RemoveStratifiedSamplerV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    StratifiedSampler& output) {
  RemoveJitter(parameters, output);
  RemoveXSamples(parameters, output);
  RemoveYSamples(parameters, output);
}

void RemoveStratifiedSamplerV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    StratifiedSampler& output) {
  RemoveJitter(parameters, output);
  RemoveXSamples(parameters, output);
  RemoveYSamples(parameters, output);
  RemoveDimension(parameters, output);
}

void RemoveStratifiedSamplerV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    StratifiedSampler& output) {
  RemoveJitter(parameters, output);
  RemoveXSamples(parameters, output);
  RemoveYSamples(parameters, output);
  RemoveSeed(parameters, output);
}

void RemoveZeroTwoSequenceSamplerV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ZeroTwoSequenceSampler& output) {
  RemovePixelSamples(parameters, output);
}

void RemoveZeroTwoSequenceSamplerV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ZeroTwoSequenceSampler& output) {
  RemovePixelSamples(parameters, output);
  RemoveDimension(parameters, output);
}

absl::Status RemoveZSobolSampler(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ZSobolSampler& output) {
  RemovePixelSamples(parameters, output);
  RemoveSeed(parameters, output);
  return RemoveRandomization(parameters, output);
}

}  // namespace pbrt_proto
