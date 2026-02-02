#include "pbrt_proto/shared/accelerators.h"

#include "absl/container/flat_hash_map.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {
namespace {

void RemoveBvhAccelerator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    bool allow_hlbvh, BvhAccelerator& output) {
  if (std::optional<int32_t> maxnodeprims =
          TryRemoveInteger(parameters, "maxnodeprims");
      maxnodeprims.has_value()) {
    output.set_maxnodeprims(std::max(0, *maxnodeprims));
  }

  if (std::optional<absl::string_view> splitmethod =
          TryRemoveString(parameters, "splitmethod");
      splitmethod.has_value()) {
    if (*splitmethod == "sah") {
      output.set_splitmethod(BvhAccelerator::SAH);
    } else if (*splitmethod == "middle") {
      output.set_splitmethod(BvhAccelerator::MIDDLE);
    } else if (*splitmethod == "equal") {
      output.set_splitmethod(BvhAccelerator::EQUAL);
    } else if (allow_hlbvh && *splitmethod == "hlbvh") {
      output.set_splitmethod(BvhAccelerator::HLBVH);
    }
  }
}

}  // namespace

void RemoveGridAccelerator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    GridAccelerator& output) {
  if (std::optional<bool> refineimmediately =
          TryRemoveBool(parameters, "refineimmediately");
      refineimmediately) {
    output.set_refineimmediately(*refineimmediately);
  }
}

void RemoveKdTreeAccelerator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    KdTreeAccelerator& output) {
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
}

void RemoveBvhAcceleratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    BvhAccelerator& output) {
  RemoveBvhAccelerator(parameters, /*allow_hlbvh=*/false, output);
}

void RemoveBvhAcceleratorV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    BvhAccelerator& output) {
  RemoveBvhAccelerator(parameters, /*allow_hlbvh=*/true, output);
}

}  // namespace pbrt_proto
