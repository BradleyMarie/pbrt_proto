#include "pbrt_proto/shared/pixel_filters.h"

#include <cmath>

#include "absl/container/flat_hash_map.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {
namespace {

template <typename T>
void RemoveWidth(absl::flat_hash_map<absl::string_view, Parameter>& parameters,
                 T& output) {
  if (std::optional<double> xwidth = TryRemoveFloat(parameters, "xwidth");
      xwidth.has_value()) {
    output.set_xradius(*xwidth);
  }

  if (std::optional<double> ywidth = TryRemoveFloat(parameters, "ywidth");
      ywidth.has_value()) {
    output.set_yradius(*ywidth);
  }
}

template <typename T>
void RemoveRadius(absl::flat_hash_map<absl::string_view, Parameter>& parameters,
                  T& output) {
  if (std::optional<double> xwidth = TryRemoveFloat(parameters, "xradius");
      xwidth.has_value()) {
    output.set_xradius(*xwidth);
  }

  if (std::optional<double> ywidth = TryRemoveFloat(parameters, "yradius");
      ywidth.has_value()) {
    output.set_yradius(*ywidth);
  }
}

template <typename T>
void RemoveBC(absl::flat_hash_map<absl::string_view, Parameter>& parameters,
              T& output) {
  if (std::optional<double> b = TryRemoveFloat(parameters, "B");
      b.has_value()) {
    output.set_b(*b);
  }

  if (std::optional<double> c = TryRemoveFloat(parameters, "C");
      c.has_value()) {
    output.set_c(*c);
  }
}

template <typename T>
void RemoveTau(absl::flat_hash_map<absl::string_view, Parameter>& parameters,
               T& output) {
  if (std::optional<double> tau = TryRemoveFloat(parameters, "tau");
      tau.has_value()) {
    output.set_tau(*tau);
  }
}

}  // namespace

absl::Status RemoveBoxPixelFilter(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, BoxPixelFilter& output) {
  if (pbrt_version <= 3) {
    RemoveWidth(parameters, output);
  } else {
    RemoveRadius(parameters, output);
  }

  return absl::OkStatus();
}

absl::Status RemoveGaussianPixelFilter(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, GaussianPixelFilter& output) {
  if (pbrt_version <= 3) {
    RemoveWidth(parameters, output);

    if (std::optional<double> alpha = TryRemoveFloat(parameters, "alpha");
        alpha.has_value()) {
      output.set_sigma(1.0 / std::sqrt(2.0 * std::abs(*alpha)));
    }
  } else {
    RemoveRadius(parameters, output);

    if (std::optional<double> sigma = TryRemoveFloat(parameters, "sigma");
        sigma.has_value()) {
      output.set_sigma(*sigma);
    }
  }

  return absl::OkStatus();
}

absl::Status RemoveLanczosPixelFilter(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, LanczosPixelFilter& output) {
  if (pbrt_version <= 3) {
    RemoveWidth(parameters, output);
    RemoveTau(parameters, output);
  } else {
    RemoveRadius(parameters, output);
    RemoveTau(parameters, output);
  }

  return absl::OkStatus();
}

absl::Status RemoveMitchellPixelFilter(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, MitchellPixelFilter& output) {
  if (pbrt_version <= 3) {
    RemoveWidth(parameters, output);
    RemoveBC(parameters, output);
  } else {
    RemoveRadius(parameters, output);
    RemoveBC(parameters, output);
  }

  return absl::OkStatus();
}

absl::Status RemoveTrianglePixelFilter(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, TrianglePixelFilter& output) {
  if (pbrt_version <= 3) {
    RemoveWidth(parameters, output);
  } else {
    RemoveRadius(parameters, output);
  }

  return absl::OkStatus();
}

}  // namespace pbrt_proto
