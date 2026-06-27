#include "pbrt_proto/shared/accelerators.h"

#include <cassert>
#include <functional>
#include <iostream>

#include "absl/container/flat_hash_map.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/enums.h"
#include "pbrt_proto/shared/parser.h"
#include "pbrt_proto/shared/version.h"

namespace pbrt_proto {

absl::Status RemoveGridAccelerator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, GridAccelerator& output) {
  assert(IsSupported(pbrt_version, output));

  if (std::optional<bool> refineimmediately =
          TryRemoveBool(parameters, "refineimmediately");
      refineimmediately.has_value()) {
    output.set_refineimmediately(*refineimmediately);
  }

  return absl::OkStatus();
}

absl::Status RemoveKdTreeAccelerator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, KdTreeAccelerator& output) {
  assert(IsSupported(pbrt_version, output));

  if (std::optional<double> emptybonus =
          TryRemoveFloat(parameters, "emptybonus");
      emptybonus.has_value()) {
    output.set_emptybonus(*emptybonus);
  }

  if (std::optional<int32_t> intersectcost =
          TryRemoveInteger(parameters, "intersectcost");
      intersectcost.has_value()) {
    output.set_intersectcost(*intersectcost);
  }

  if (std::optional<int32_t> maxdepth =
          TryRemoveInteger(parameters, "maxdepth");
      maxdepth.has_value()) {
    output.set_maxdepth(std::max(0, std::max(0, *maxdepth)));
  }

  if (std::optional<int32_t> maxprims =
          TryRemoveInteger(parameters, "maxprims");
      maxprims.has_value()) {
    output.set_maxprims(std::max(0, *maxprims));
  }

  if (std::optional<int32_t> traversalcost =
          TryRemoveInteger(parameters, "traversalcost");
      traversalcost.has_value()) {
    output.set_traversalcost(*traversalcost);
  }

  return absl::OkStatus();
}

absl::Status RemoveBvhAccelerator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, BvhAccelerator& output) {
  assert(IsSupported(pbrt_version, output));

  if (std::optional<int32_t> maxnodeprims =
          TryRemoveInteger(parameters, "maxnodeprims");
      maxnodeprims.has_value()) {
    output.set_maxnodeprims(std::max(0, *maxnodeprims));
  }

  if (absl::Status status =
          RemoveEnum(parameters, pbrt_version, "bvh", "splitmethod",
                     std::bind(&BvhAccelerator::set_splitmethod, &output,
                               std::placeholders::_1),
                     BvhAccelerator::SAH);
      !status.ok()) {
    return status;
  }

  return absl::OkStatus();
}

}  // namespace pbrt_proto
