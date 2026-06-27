#include "pbrt_proto/shared/samplers.h"

#include <cassert>

#include "absl/container/flat_hash_map.h"
#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/enums.h"
#include "pbrt_proto/shared/parser.h"
#include "pbrt_proto/shared/version.h"

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
void RemoveSeed(absl::flat_hash_map<absl::string_view, Parameter>& parameters,
                T& output) {
  if (std::optional<int32_t> seed = TryRemoveInteger(parameters, "seed");
      seed.has_value()) {
    output.set_seed(*seed);
  }
}

}  // namespace

absl::Status RemoveAdaptiveSampler(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, AdaptiveSampler& output) {
  assert(IsSupported(pbrt_version, output));

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
      std::cerr << "WARNING: Unsupported value for 'adaptive' Sampler "
                   "parameter 'method': \""
                << *method << "\"" << std::endl;
      output.set_method(AdaptiveSampler::CONTRAST);
    }
  }

  return absl::OkStatus();
}

absl::Status RemoveBestCandidateSampler(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, BestCandidateSampler& output) {
  assert(IsSupported(pbrt_version, output));

  RemovePixelSamples(parameters, output);
  return absl::OkStatus();
}

absl::Status RemoveHaltonSampler(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, HaltonSampler& output) {
  assert(IsSupported(pbrt_version, output));

  RemovePixelSamples(parameters, output);

  if (pbrt_version == 3) {
    if (std::optional<bool> samplepixelcenter =
            TryRemoveBool(parameters, "samplepixelcenter");
        samplepixelcenter.has_value()) {
      output.set_samplepixelcenter(*samplepixelcenter);
    }
  }

  if (pbrt_version >= 4) {
    RemoveSeed(parameters, output);

    if (absl::Status status =
            RemoveEnum(parameters, pbrt_version, "halton", "randomization",
                       std::bind(&HaltonSampler::set_randomization, &output,
                                 std::placeholders::_1));
        !status.ok()) {
      return status;
    }
  }

  return absl::OkStatus();
}

absl::Status RemoveIndependentSampler(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, IndependentSampler& output) {
  assert(IsSupported(pbrt_version, output));

  RemovePixelSamples(parameters, output);

  if (pbrt_version >= 4) {
    RemoveSeed(parameters, output);
  }

  return absl::OkStatus();
}

absl::Status RemoveMaxMinDistSampler(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, MaxMinDistSampler& output) {
  assert(IsSupported(pbrt_version, output));

  RemovePixelSamples(parameters, output);
  RemoveDimension(parameters, output);
  return absl::OkStatus();
}

absl::Status RemovePaddedSobolSampler(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, PaddedSobolSampler& output) {
  assert(IsSupported(pbrt_version, output));

  RemovePixelSamples(parameters, output);
  RemoveSeed(parameters, output);
  return RemoveEnum(parameters, pbrt_version, "paddedsobol", "randomization",
                    std::bind(&PaddedSobolSampler::set_randomization, &output,
                              std::placeholders::_1));
}

absl::Status RemovePMJ02BNSampler(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, PMJ02BNSampler& output) {
  assert(IsSupported(pbrt_version, output));

  RemovePixelSamples(parameters, output);
  RemoveSeed(parameters, output);
  return absl::OkStatus();
}

absl::Status RemoveRandomSampler(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, RandomSampler& output) {
  assert(IsSupported(pbrt_version, output));

  RemoveXSamples(parameters, output);
  RemoveYSamples(parameters, output);
  return absl::OkStatus();
}

absl::Status RemoveSobolSampler(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, SobolSampler& output) {
  assert(IsSupported(pbrt_version, output));

  RemovePixelSamples(parameters, output);

  if (pbrt_version >= 4) {
    RemoveSeed(parameters, output);

    if (absl::Status status =
            RemoveEnum(parameters, pbrt_version, "sobol", "randomization",
                       std::bind(&SobolSampler::set_randomization, &output,
                                 std::placeholders::_1));
        !status.ok()) {
      return status;
    }
  }

  return absl::OkStatus();
}

absl::Status RemoveStratifiedSampler(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, StratifiedSampler& output) {
  assert(IsSupported(pbrt_version, output));

  RemoveJitter(parameters, output);
  RemoveXSamples(parameters, output);
  RemoveYSamples(parameters, output);

  if (pbrt_version == 3) {
    RemoveDimension(parameters, output);
  }

  if (pbrt_version >= 4) {
    RemoveSeed(parameters, output);
  }

  return absl::OkStatus();
}

absl::Status RemoveZeroTwoSequenceSampler(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, ZeroTwoSequenceSampler& output) {
  assert(IsSupported(pbrt_version, output));

  RemovePixelSamples(parameters, output);

  if (pbrt_version >= 3) {
    RemoveDimension(parameters, output);
  }

  return absl::OkStatus();
}

absl::Status RemoveZSobolSampler(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, ZSobolSampler& output) {
  assert(IsSupported(pbrt_version, output));

  RemovePixelSamples(parameters, output);
  RemoveSeed(parameters, output);
  return RemoveEnum(parameters, pbrt_version, "zsobol", "randomization",
                    std::bind(&ZSobolSampler::set_randomization, &output,
                              std::placeholders::_1));
}

}  // namespace pbrt_proto
