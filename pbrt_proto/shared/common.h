#ifndef _PBRT_PROTO_SHARED_COMMON_
#define _PBRT_PROTO_SHARED_COMMON_

#include <optional>

#include "absl/container/flat_hash_map.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {

//
// Common Parameters
//

std::optional<CheckerboardAntialiasing> TryRemoveAaMode(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters);

std::optional<TextureMapping> TryRemoveMapping(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters);

std::optional<ImageWrap> TryRemoveWrap(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters);

template <typename T>
void TryRemoveUVParameters(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters, T& output) {
  if (std::optional<TextureMapping> mapping = TryRemoveMapping(parameters);
      mapping) {
    output.set_mapping(*mapping);
  }

  if (std::optional<double> uscale = TryRemoveFloat(parameters, "uscale");
      uscale) {
    output.set_uscale(*uscale);
  }

  if (std::optional<double> vscale = TryRemoveFloat(parameters, "vscale");
      vscale) {
    output.set_vscale(*vscale);
  }

  if (std::optional<double> udelta = TryRemoveFloat(parameters, "udelta");
      udelta) {
    output.set_udelta(*udelta);
  }

  if (std::optional<double> vdelta = TryRemoveFloat(parameters, "vdelta");
      vdelta) {
    output.set_vdelta(*vdelta);
  }

  if (std::optional<std::array<double, 3>> v1 =
          TryRemoveVector3(parameters, "v1");
      v1) {
    output.mutable_v1()->set_x((*v1)[0]);
    output.mutable_v1()->set_y((*v1)[1]);
    output.mutable_v1()->set_z((*v1)[2]);
  }

  if (std::optional<std::array<double, 3>> v2 =
          TryRemoveVector3(parameters, "v2");
      v2) {
    output.mutable_v2()->set_x((*v2)[0]);
    output.mutable_v2()->set_y((*v2)[1]);
    output.mutable_v2()->set_z((*v2)[2]);
  }
}

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_COMMON_
