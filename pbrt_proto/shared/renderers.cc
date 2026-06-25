#include "pbrt_proto/shared/renderers.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <optional>

#include "absl/container/flat_hash_map.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"
#include "pbrt_proto/shared/version.h"

namespace pbrt_proto {

absl::Status RemoveAggregateTestRenderer(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, AggregateTestRenderer& output) {
  assert(IsSupported(pbrt_version, output));

  if (std::optional<int32_t> niters = TryRemoveInteger(parameters, "niters");
      niters.has_value()) {
    output.set_niters(std::max(0, *niters));
  }

  return absl::OkStatus();
}

absl::Status RemoveCreateProbesRenderer(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, CreateProbesRenderer& output) {
  assert(IsSupported(pbrt_version, output));

  std::optional<absl::Span<double>> bounds;
  if (absl::Status status = TryRemoveFloats(parameters, "bounds", 6, bounds);
      !status.ok()) {
    std::cerr << "WARNING: " << status.message() << std::endl;
  } else if (bounds.has_value()) {
    auto& p0 = *output.mutable_bounds()->mutable_p0();
    p0.set_x((*bounds)[0]);
    p0.set_y((*bounds)[1]);
    p0.set_z((*bounds)[2]);

    auto& p1 = *output.mutable_bounds()->mutable_p1();
    p1.set_x((*bounds)[3]);
    p1.set_y((*bounds)[4]);
    p1.set_z((*bounds)[5]);
  }

  if (std::optional<bool> directlighting =
          TryRemoveBool(parameters, "directlighting");
      directlighting.has_value()) {
    output.set_directlighting(*directlighting);
  }

  if (std::optional<absl::string_view> filename =
          TryRemoveString(parameters, "filename");
      filename.has_value()) {
    output.set_filename(*filename);
  }

  if (std::optional<bool> indirectlighting =
          TryRemoveBool(parameters, "indirectlighting");
      indirectlighting.has_value()) {
    output.set_indirectlighting(*indirectlighting);
  }

  if (std::optional<int32_t> lmax = TryRemoveInteger(parameters, "lmax");
      lmax.has_value()) {
    output.set_lmax(std::max(0, *lmax));
  }

  if (std::optional<int32_t> indirectsamples =
          TryRemoveInteger(parameters, "indirectsamples");
      indirectsamples.has_value()) {
    output.set_indirectsamples(std::max(0, *indirectsamples));
  }

  if (std::optional<double> samplespacing =
          TryRemoveFloat(parameters, "samplespacing");
      samplespacing.has_value()) {
    output.set_samplespacing(*samplespacing);
  }

  if (std::optional<double> time = TryRemoveFloat(parameters, "time");
      time.has_value()) {
    output.set_time(*time);
  }

  return absl::OkStatus();
}

absl::Status RemoveMetropolisRenderer(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, MetropolisRenderer& output) {
  assert(IsSupported(pbrt_version, output));

  if (std::optional<double> largestepprobability =
          TryRemoveFloat(parameters, "largestepprobability");
      largestepprobability.has_value()) {
    output.set_largestepprobability(*largestepprobability);
  }

  if (std::optional<int32_t> samplesperpixel =
          TryRemoveInteger(parameters, "samplesperpixel");
      samplesperpixel.has_value()) {
    output.set_samplesperpixel(std::max(0, *samplesperpixel));
  }

  if (std::optional<int32_t> bootstrapsamples =
          TryRemoveInteger(parameters, "bootstrapsamples");
      bootstrapsamples.has_value()) {
    output.set_bootstrapsamples(std::max(0, *bootstrapsamples));
  }

  if (std::optional<int32_t> directsamples =
          TryRemoveInteger(parameters, "directsamples");
      directsamples.has_value()) {
    output.set_directsamples(std::max(0, *directsamples));
  }

  if (std::optional<bool> dodirectseparately =
          TryRemoveBool(parameters, "dodirectseparately");
      dodirectseparately.has_value()) {
    output.set_dodirectseparately(*dodirectseparately);
  }

  if (std::optional<int32_t> maxconsecutiverejects =
          TryRemoveInteger(parameters, "maxconsecutiverejects");
      maxconsecutiverejects.has_value()) {
    output.set_maxconsecutiverejects(std::max(0, *maxconsecutiverejects));
  }

  if (std::optional<int32_t> maxdepth =
          TryRemoveInteger(parameters, "maxdepth");
      maxdepth.has_value()) {
    output.set_maxdepth(std::max(0, *maxdepth));
  }

  if (std::optional<bool> bidirectional =
          TryRemoveBool(parameters, "bidirectional");
      bidirectional.has_value()) {
    output.set_bidirectional(*bidirectional);
  }

  return absl::OkStatus();
}

absl::Status RemoveSamplerRenderer(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, SamplerRenderer& output) {
  assert(IsSupported(pbrt_version, output));

  if (std::optional<bool> visualizeobjectids =
          TryRemoveBool(parameters, "visualizeobjectids");
      visualizeobjectids.has_value()) {
    output.set_visualizeobjectids(*visualizeobjectids);
  }

  return absl::OkStatus();
}

absl::Status RemoveSurfacePointsRenderer(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, SurfacePointsRenderer& output) {
  assert(IsSupported(pbrt_version, output));

  if (std::optional<double> minsampledistance =
          TryRemoveFloat(parameters, "minsampledistance");
      minsampledistance.has_value()) {
    output.set_minsampledistance(*minsampledistance);
  }

  if (std::optional<absl::string_view> filename =
          TryRemoveString(parameters, "filename");
      filename.has_value()) {
    output.set_filename(*filename);
  }

  return absl::OkStatus();
}

}  // namespace pbrt_proto
