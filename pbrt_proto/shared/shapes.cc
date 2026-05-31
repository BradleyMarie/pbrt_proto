#include "pbrt_proto/shared/shapes.h"

#include <algorithm>

#include "absl/container/flat_hash_map.h"
#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/common.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {

void RemoveConeShapeV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ConeShape& output) {
  if (std::optional<double> radius = TryRemoveFloat(parameters, "radius");
      radius.has_value()) {
    output.set_radius(*radius);
  }

  if (std::optional<double> height = TryRemoveFloat(parameters, "height");
      height.has_value()) {
    output.set_height(*height);
  }

  if (std::optional<double> phimax = TryRemoveFloat(parameters, "phimax");
      phimax.has_value()) {
    output.set_phimax(*phimax);
  }
}

void RemoveCylinderShapeV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    CylinderShape& output) {
  if (std::optional<double> radius = TryRemoveFloat(parameters, "radius");
      radius.has_value()) {
    output.set_radius(*radius);
  }

  if (std::optional<double> zmin = TryRemoveFloat(parameters, "zmin");
      zmin.has_value()) {
    output.set_zmin(*zmin);
  }

  if (std::optional<double> zmax = TryRemoveFloat(parameters, "zmax");
      zmax.has_value()) {
    output.set_zmax(*zmax);
  }

  if (std::optional<double> phimax = TryRemoveFloat(parameters, "phimax");
      phimax.has_value()) {
    output.set_phimax(*phimax);
  }
}

void RemoveDiskShapeV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DiskShape& output) {
  if (std::optional<double> height = TryRemoveFloat(parameters, "height");
      height.has_value()) {
    output.set_height(*height);
  }

  if (std::optional<double> radius = TryRemoveFloat(parameters, "radius");
      radius.has_value()) {
    output.set_radius(*radius);
  }

  if (std::optional<double> innerradius =
          TryRemoveFloat(parameters, "innerradius");
      innerradius.has_value()) {
    output.set_innerradius(*innerradius);
  }

  if (std::optional<double> phimax = TryRemoveFloat(parameters, "phimax");
      phimax.has_value()) {
    output.set_phimax(*phimax);
  }
}

absl::Status RemoveHeightFieldShapeV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    HeightFieldShape& output) {
  if (std::optional<int32_t> nu = TryRemoveInteger(parameters, "nu");
      nu.has_value()) {
    output.set_nu(std::max(0, *nu));
  }

  if (std::optional<int32_t> nv = TryRemoveInteger(parameters, "nv");
      nv.has_value()) {
    output.set_nv(std::max(0, *nv));
  }

  uint64_t required_size =
      static_cast<uint64_t>(output.nu()) * static_cast<uint64_t>(output.nv());
  if (required_size > std::numeric_limits<int32_t>::max()) {
    return absl::InvalidArgumentError(
        "Heighfield shape is too large to be stored in a 1D proto array");
  }

  std::optional<absl::Span<double>> pz;
  if (absl::Status status = TryRemoveFloats(
          parameters, "Pz", static_cast<size_t>(required_size), pz);
      status.ok() && pz.has_value()) {
    output.mutable_pz()->Add(pz->begin(), pz->end());
  }

  return absl::OkStatus();
}

void RemoveHyperboloidShapeV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    HyperboloidShape& output) {
  if (std::optional<std::array<double, 3>> p1 =
          TryRemovePoint3(parameters, "p1");
      p1.has_value()) {
    output.mutable_p1()->set_x((*p1)[0]);
    output.mutable_p1()->set_y((*p1)[1]);
    output.mutable_p1()->set_z((*p1)[2]);
  }

  if (std::optional<std::array<double, 3>> p2 =
          TryRemovePoint3(parameters, "p2");
      p2.has_value()) {
    output.mutable_p2()->set_x((*p2)[0]);
    output.mutable_p2()->set_y((*p2)[1]);
    output.mutable_p2()->set_z((*p2)[2]);
  }

  if (std::optional<double> phimax = TryRemoveFloat(parameters, "phimax");
      phimax.has_value()) {
    output.set_phimax(*phimax);
  }
}

absl::Status RemoveLoopSubdivShapeV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    LoopSubdivShape& output) {
  if (std::optional<int32_t> nlevels = TryRemoveInteger(parameters, "nlevels");
      nlevels.has_value()) {
    output.set_levels(std::max(0, *nlevels));
  }

  if (std::optional<absl::Span<std::array<double, 3>>> p =
          TryRemovePoint3s(parameters, "P");
      p.has_value()) {
    p->remove_suffix(p->size() % 3);
    if (p->size() > std::numeric_limits<int32_t>::max()) {
      return absl::InvalidArgumentError(
          "LoopSubdiv shape has too many indices to be stored in a 1D proto "
          "array");
    }

    for (const std::array<double, 3>& src : *p) {
      Point& dest = *output.add_p();
      dest.set_x(src[0]);
      dest.set_y(src[1]);
      dest.set_z(src[2]);
    }
  }

  if (std::optional<absl::Span<int32_t>> indices =
          TryRemoveIntegers(parameters, "indices");
      indices.has_value()) {
    indices->remove_suffix(indices->size() % 3);
    if (indices->size() / 3 > std::numeric_limits<int32_t>::max()) {
      return absl::InvalidArgumentError(
          "LoopSubdiv shape has too many indices to be stored in a 1D proto "
          "array");
    }

    for (size_t i = 0; i < indices->size() / 3u; i++) {
      VertexIndices& face = *output.add_indices();
      face.set_v0(std::max(0, (*indices)[3u * i + 0]));
      face.set_v1(std::max(0, (*indices)[3u * i + 1]));
      face.set_v2(std::max(0, (*indices)[3u * i + 2]));
    }
  }

  return absl::OkStatus();
}

absl::Status RemoveNurbsShapeV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    NurbsShape& output) {
  if (std::optional<int32_t> nu = TryRemoveInteger(parameters, "nu");
      nu.has_value()) {
    output.set_nu(std::max(0, *nu));
  }

  if (std::optional<int32_t> nv = TryRemoveInteger(parameters, "nv");
      nv.has_value()) {
    output.set_nv(std::max(0, *nv));
  }

  if (std::optional<int32_t> uorder = TryRemoveInteger(parameters, "uorder");
      uorder.has_value()) {
    output.set_uorder(std::max(0, *uorder));
  }

  if (std::optional<int32_t> vorder = TryRemoveInteger(parameters, "vorder");
      vorder.has_value()) {
    output.set_vorder(std::max(0, *vorder));
  }

  if (std::optional<absl::Span<double>> uknots =
          TryRemoveFloats(parameters, "uknots");
      uknots.has_value()) {
    output.mutable_uknots()->Add(uknots->begin(), uknots->end());
  }

  if (std::optional<absl::Span<double>> vknots =
          TryRemoveFloats(parameters, "vknots");
      vknots.has_value()) {
    output.mutable_vknots()->Add(vknots->begin(), vknots->end());
  }

  if (std::optional<double> v0 = TryRemoveFloat(parameters, "v0");
      v0.has_value()) {
    output.set_v0(*v0);
  }

  if (std::optional<double> v1 = TryRemoveFloat(parameters, "v1");
      v1.has_value()) {
    output.set_v1(*v1);
  }

  if (std::optional<double> u0 = TryRemoveFloat(parameters, "u0");
      u0.has_value()) {
    output.set_u0(*u0);
  }

  if (std::optional<double> u1 = TryRemoveFloat(parameters, "u1");
      u1.has_value()) {
    output.set_u1(*u1);
  }

  int64_t required_points =
      static_cast<int64_t>(output.nu()) * static_cast<int64_t>(output.nv());
  if (required_points > std::numeric_limits<int32_t>::max()) {
    return absl::InvalidArgumentError(
        "Nurbs shape has too many points to be stored in a 1D proto array");
  }

  if (std::optional<absl::Span<std::array<double, 3>>> p =
          TryRemovePoint3s(parameters, "P");
      p.has_value()) {
    if (p->size() > std::numeric_limits<int32_t>::max()) {
      return absl::InvalidArgumentError(
          "Nurbs shape has too many points to be stored in a 1D proto "
          "array");
    }

    for (const std::array<double, 3>& src : *p) {
      Point& dest = *output.add_p();
      dest.set_x(src[0]);
      dest.set_y(src[1]);
      dest.set_z(src[2]);
    }
  }

  if (std::optional<absl::Span<double>> pw = TryRemoveFloats(parameters, "Pw");
      pw.has_value()) {
    pw->remove_suffix(pw->size() % 4);
    if (pw->size() / 4 > std::numeric_limits<int32_t>::max()) {
      return absl::InvalidArgumentError(
          "Nurbs shape has too many points to be stored in a 1D proto "
          "array");
    }

    for (size_t i = 0; i < pw->size() / 4; i++) {
      NurbsShape::PointWithWeight& dest = *output.add_pw();
      dest.mutable_p()->set_x((*pw)[4 * i + 0]);
      dest.mutable_p()->set_y((*pw)[4 * i + 1]);
      dest.mutable_p()->set_z((*pw)[4 * i + 2]);
      dest.set_weight((*pw)[4 * i + 3]);
    }
  }

  return absl::OkStatus();
}

void RemoveParaboloidShapeV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ParaboloidShape& output) {
  if (std::optional<double> radius = TryRemoveFloat(parameters, "radius");
      radius.has_value()) {
    output.set_radius(*radius);
  }

  if (std::optional<double> zmin = TryRemoveFloat(parameters, "zmin");
      zmin.has_value()) {
    output.set_zmin(*zmin);
  }

  if (std::optional<double> zmax = TryRemoveFloat(parameters, "zmax");
      zmax.has_value()) {
    output.set_zmax(*zmax);
  }

  if (std::optional<double> phimax = TryRemoveFloat(parameters, "phimax");
      phimax.has_value()) {
    output.set_phimax(*phimax);
  }
}

void RemoveSphereShapeV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SphereShape& output) {
  if (std::optional<double> radius = TryRemoveFloat(parameters, "radius");
      radius.has_value()) {
    output.set_radius(*radius);
  }

  if (std::optional<double> zmin = TryRemoveFloat(parameters, "zmin");
      zmin.has_value()) {
    output.set_zmin(*zmin);
  }

  if (std::optional<double> zmax = TryRemoveFloat(parameters, "zmax");
      zmax.has_value()) {
    output.set_zmax(*zmax);
  }

  if (std::optional<double> phimax = TryRemoveFloat(parameters, "phimax");
      phimax.has_value()) {
    output.set_phimax(*phimax);
  }
}

absl::Status RemoveTriangleMeshShapeV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    TriangleMeshShape& output) {
  if (std::optional<absl::Span<std::array<double, 3>>> p =
          TryRemovePoint3s(parameters, "P");
      p.has_value()) {
    if (p->size() > std::numeric_limits<int32_t>::max()) {
      return absl::InvalidArgumentError(
          "Trianglemesh shape has too many points to be stored in a 1D proto "
          "array");
    }

    for (const auto& src : *p) {
      Point& dest = *output.add_p();
      dest.set_x(src[0]);
      dest.set_y(src[1]);
      dest.set_z(src[2]);
    }
  }

  if (std::optional<absl::Span<int32_t>> indices =
          TryRemoveIntegers(parameters, "indices");
      indices.has_value()) {
    indices->remove_suffix(indices->size() % 3);
    if (indices->size() / 3 > std::numeric_limits<int32_t>::max()) {
      return absl::InvalidArgumentError(
          "Trianglemesh shape has too many indices to be stored in a 1D proto "
          "array");
    }

    for (size_t i = 0; i < indices->size() / 3; i++) {
      VertexIndices& dest = *output.add_indices();
      dest.set_v0(std::max(0, (*indices)[3u * i + 0]));
      dest.set_v1(std::max(0, (*indices)[3u * i + 1]));
      dest.set_v2(std::max(0, (*indices)[3u * i + 2]));
    }
  }

  if (std::optional<absl::Span<std::array<double, 3>>> n =
          TryRemoveNormals(parameters, "N");
      n.has_value()) {
    if (n->size() > std::numeric_limits<int32_t>::max()) {
      return absl::InvalidArgumentError(
          "Trianglemesh shape has too many normals to be stored in a 1D proto "
          "array");
    }

    for (const auto& src : *n) {
      Vector& dest = *output.add_n();
      dest.set_x(src[0]);
      dest.set_y(src[1]);
      dest.set_z(src[2]);
    }
  }

  if (std::optional<absl::Span<std::array<double, 3>>> s =
          TryRemoveVector3s(parameters, "S");
      s.has_value()) {
    if (s->size() > std::numeric_limits<int32_t>::max()) {
      return absl::InvalidArgumentError(
          "Trianglemesh shape has too many tangents to be stored in a 1D proto "
          "array");
    }

    for (const auto& src : *s) {
      Vector& dest = *output.add_s();
      dest.set_x(src[0]);
      dest.set_y(src[1]);
      dest.set_z(src[2]);
    }
  }

  if (std::optional<absl::Span<double>> uv = TryRemoveFloats(parameters, "uv");
      uv.has_value()) {
    uv->remove_suffix(uv->size() % 2);
    if (uv->size() / 2 > std::numeric_limits<int32_t>::max()) {
      return absl::InvalidArgumentError(
          "Trianglemesh shape has too many texture coordinates to be stored in "
          "a 1D proto array");
    }

    for (size_t i = 0; i < uv->size() / 2; i++) {
      TriangleMeshShape::UVCoordinate& dest = *output.add_uv();
      dest.set_u((*uv)[2 * i + 0]);
      dest.set_v((*uv)[2 * i + 1]);
    }
  } else if (std::optional<absl::Span<double>> st =
                 TryRemoveFloats(parameters, "st");
             st.has_value()) {
    st->remove_suffix(st->size() % 2);
    if (st->size() / 2 > std::numeric_limits<int32_t>::max()) {
      return absl::InvalidArgumentError(
          "Trianglemesh shape has too many texture coordinates to be stored in "
          "a 1D proto array");
    }

    for (size_t i = 0; i < st->size() / 2; i++) {
      TriangleMeshShape::UVCoordinate& dest = *output.add_uv();
      dest.set_u((*st)[2 * i + 0]);
      dest.set_v((*st)[2 * i + 1]);
    }
  }

  return absl::OkStatus();
}

absl::Status RemoveTriangleMeshShapeV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    TriangleMeshShape& output) {
  RemoveTriangleMeshShapeV1(parameters, output);

  if (std::optional<bool> discarddegenerateuvs =
          TryRemoveBool(parameters, "discarddegenerateUVs");
      discarddegenerateuvs.has_value()) {
    output.set_discarddegenerateuvs(*discarddegenerateuvs);
  }

  TryRemoveFloatTexture(parameters, "alpha",
                        std::bind(&TriangleMeshShape::mutable_alpha, &output));

  return absl::OkStatus();
}

}  // namespace pbrt_proto
