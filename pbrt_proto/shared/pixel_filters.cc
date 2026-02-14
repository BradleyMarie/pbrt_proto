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

void RemoveBoxPixelFilterV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    BoxPixelFilter& output) {
  RemoveWidth(parameters, output);
}

void RemoveBoxPixelFilterV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    BoxPixelFilter& output) {
  RemoveRadius(parameters, output);
}

void RemoveGaussianPixelFilterV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    GaussianPixelFilter& output) {
  RemoveWidth(parameters, output);

  if (std::optional<double> alpha = TryRemoveFloat(parameters, "alpha");
      alpha.has_value()) {
    output.set_sigma(1.0 / std::sqrt(2.0 * std::abs(*alpha)));
  }
}

void RemoveGaussianPixelFilterV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    GaussianPixelFilter& output) {
  RemoveRadius(parameters, output);

  if (std::optional<double> sigma = TryRemoveFloat(parameters, "sigma");
      sigma.has_value()) {
    output.set_sigma(*sigma);
  }
}

void RemoveLanczosPixelFilterV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    LanczosPixelFilter& output) {
  RemoveWidth(parameters, output);
  RemoveTau(parameters, output);
}

void RemoveLanczosPixelFilterV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    LanczosPixelFilter& output) {
  RemoveRadius(parameters, output);
  RemoveTau(parameters, output);
}

void RemoveMitchellPixelFilterV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    MitchellPixelFilter& output) {
  RemoveWidth(parameters, output);
  RemoveBC(parameters, output);
}

void RemoveMitchellPixelFilterV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    MitchellPixelFilter& output) {
  RemoveRadius(parameters, output);
  RemoveBC(parameters, output);
}

void RemoveTrianglePixelFilterV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    TrianglePixelFilter& output) {
  RemoveWidth(parameters, output);
}

void RemoveTrianglePixelFilterV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    TrianglePixelFilter& output) {
  RemoveRadius(parameters, output);
}

}  // namespace pbrt_proto
