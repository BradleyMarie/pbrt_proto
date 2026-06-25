#include "pbrt_proto/shared/shapes.h"

#include <algorithm>
#include <cassert>
#include <utility>

#include "absl/container/flat_hash_map.h"
#include "absl/status/status.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/common.h"
#include "pbrt_proto/shared/parser.h"
#include "pbrt_proto/shared/version.h"

namespace pbrt_proto {

absl::Status RemoveConeShape(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, ConeShape& output) {
  assert(IsSupported(pbrt_version, output));

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

  return absl::OkStatus();
}

absl::Status TryRemoveCurveShape(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, absl::FunctionRef<CurveShape*()> get_output) {
  CurveShape output;
  assert(IsSupported(pbrt_version, output));

  bool write_output = true;
  if (std::optional<int32_t> degree = TryRemoveInteger(parameters, "degree");
      degree.has_value()) {
    if (*degree == 3) {
      output.set_degree(CurveShape::THREE);
    } else if (*degree == 2) {
      output.set_degree(CurveShape::TWO);
    } else {
      std::cerr << "WARNING: Unsupported value for 'curve' Shape parameter "
                   "'degree': \""
                << *degree << "\"" << std::endl;
      write_output = false;
    }
  }

  if (std::optional<absl::string_view> basis =
          TryRemoveString(parameters, "basis");
      basis.has_value()) {
    if (*basis == "bezier") {
      output.set_basis(CurveShape::BEZIER);
    } else if (*basis == "bspline") {
      output.set_basis(CurveShape::BSPLINE);
    } else {
      std::cerr << "WARNING: Unrecognized value for 'curve' Shape parameter "
                   "'basis': \""
                << *basis << "\"" << std::endl;
      write_output = false;
    }
  }

  if (std::optional<absl::string_view> type =
          TryRemoveString(parameters, "type");
      type.has_value()) {
    if (*type == "cylinder") {
      output.set_type(CurveShape::CYLINDER);
    } else if (*type == "flat") {
      output.set_type(CurveShape::FLAT);
    } else if (*type == "ribbon") {
      output.set_type(CurveShape::RIBBON);
    } else {
      std::cerr << "Unrecognized value for 'curve' Shape parameter 'type': \""
                << *type << "\"" << std::endl;
      output.set_type(CurveShape::CYLINDER);
    }
  }

  if (std::optional<double> width = TryRemoveFloat(parameters, "width");
      width.has_value()) {
    output.set_width(*width);
  }

  if (std::optional<double> width0 = TryRemoveFloat(parameters, "width0");
      width0.has_value()) {
    output.set_width0(*width0);
  }

  if (std::optional<double> width1 = TryRemoveFloat(parameters, "width1");
      width1.has_value()) {
    output.set_width1(*width1);
  }

  if (std::optional<absl::Span<std::array<double, 3>>> p =
          TryRemovePoint3s(parameters, "P");
      p.has_value()) {
    if (p->size() > std::numeric_limits<int32_t>::max()) {
      return absl::ResourceExhaustedError(
          "Curve shape has too many points to be stored in a 1D proto array");
    }

    for (const std::array<double, 3>& src : *p) {
      Point& dest = *output.add_p();
      dest.set_x(src[0]);
      dest.set_y(src[1]);
      dest.set_z(src[2]);
    }
  }

  if (std::optional<absl::Span<std::array<double, 3>>> n =
          TryRemoveNormals(parameters, "N");
      n.has_value()) {
    if (n->size() > std::numeric_limits<int32_t>::max()) {
      return absl::ResourceExhaustedError(
          "Curve shape has too many normals to be stored in a 1D proto array");
    }

    for (const std::array<double, 3>& src : *n) {
      Vector& dest = *output.add_n();
      dest.set_x(src[0]);
      dest.set_y(src[1]);
      dest.set_z(src[2]);
    }
  }

  if (std::optional<int32_t> splitdepth =
          TryRemoveInteger(parameters, "splitdepth");
      splitdepth.has_value()) {
    output.set_splitdepth(std::max(0, *splitdepth));
  }

  if (write_output) {
    *get_output() = std::move(output);
  }

  return absl::OkStatus();
}

absl::Status RemoveCylinderShape(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, CylinderShape& output) {
  assert(IsSupported(pbrt_version, output));

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

  return absl::OkStatus();
}

absl::Status RemoveDiskShape(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, DiskShape& output) {
  assert(IsSupported(pbrt_version, output));

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

  return absl::OkStatus();
}

absl::Status RemoveHeightFieldShape(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, HeightFieldShape& output) {
  assert(IsSupported(pbrt_version, output));

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
    return absl::ResourceExhaustedError(
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

absl::Status RemoveHyperboloidShape(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, HyperboloidShape& output) {
  assert(IsSupported(pbrt_version, output));

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

  return absl::OkStatus();
}

absl::Status RemoveLoopSubdivShape(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, LoopSubdivShape& output) {
  assert(IsSupported(pbrt_version, output));

  if (std::optional<int32_t> nlevels = TryRemoveInteger(parameters, "nlevels");
      nlevels.has_value()) {
    output.set_levels(std::max(0, *nlevels));
  }

  if (std::optional<absl::Span<std::array<double, 3>>> p =
          TryRemovePoint3s(parameters, "P");
      p.has_value()) {
    p->remove_suffix(p->size() % 3);
    if (p->size() > std::numeric_limits<int32_t>::max()) {
      return absl::ResourceExhaustedError(
          "LoopSubdiv shape has too many points to be stored in a 1D proto "
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
      return absl::ResourceExhaustedError(
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

  if (pbrt_version >= 3) {
    if (std::optional<int32_t> levels = TryRemoveInteger(parameters, "levels");
        levels.has_value()) {
      output.set_levels(std::max(0, *levels));
    }
  }

  return absl::OkStatus();
}

absl::Status RemoveNurbsShape(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, NurbsShape& output) {
  assert(IsSupported(pbrt_version, output));

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
    if (uknots->size() > std::numeric_limits<int32_t>::max()) {
      return absl::ResourceExhaustedError(
          "Nurbs shape has too many uknot values to be stored in a 1D proto "
          "array");
    }

    output.mutable_uknots()->Add(uknots->begin(), uknots->end());
  }

  if (std::optional<absl::Span<double>> vknots =
          TryRemoveFloats(parameters, "vknots");
      vknots.has_value()) {
    if (vknots->size() > std::numeric_limits<int32_t>::max()) {
      return absl::ResourceExhaustedError(
          "Nurbs shape has too many vknot values to be stored in a 1D proto "
          "array");
    }

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
    return absl::ResourceExhaustedError(
        "Nurbs shape has too many points to be stored in a 1D proto array");
  }

  if (std::optional<absl::Span<std::array<double, 3>>> p =
          TryRemovePoint3s(parameters, "P");
      p.has_value()) {
    if (p->size() > std::numeric_limits<int32_t>::max()) {
      return absl::ResourceExhaustedError(
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
      return absl::ResourceExhaustedError(
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

absl::Status RemoveParaboloidShape(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, ParaboloidShape& output) {
  assert(IsSupported(pbrt_version, output));

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

  return absl::OkStatus();
}

absl::Status RemovePlyMeshShape(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, PlyMeshShape& output) {
  assert(IsSupported(pbrt_version, output));

  if (std::optional<absl::string_view> filename =
          TryRemoveString(parameters, "filename");
      filename) {
    output.set_filename(*filename);
  }

  TryRemoveFloatTexture(parameters, "alpha",
                        std::bind(&PlyMeshShape::mutable_alpha, &output));

  TryRemoveFloatTexture(parameters, "shadowalpha",
                        std::bind(&PlyMeshShape::mutable_shadowalpha, &output));

  return absl::OkStatus();
}

absl::Status RemoveSphereShape(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, SphereShape& output) {
  assert(IsSupported(pbrt_version, output));

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

  return absl::OkStatus();
}

absl::Status RemoveTriangleMeshShape(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, TriangleMeshShape& output) {
  assert(IsSupported(pbrt_version, output));

  if (std::optional<absl::Span<std::array<double, 3>>> p =
          TryRemovePoint3s(parameters, "P");
      p.has_value()) {
    if (p->size() > std::numeric_limits<int32_t>::max()) {
      return absl::ResourceExhaustedError(
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
      return absl::ResourceExhaustedError(
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
      return absl::ResourceExhaustedError(
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
      return absl::ResourceExhaustedError(
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
      return absl::ResourceExhaustedError(
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
      return absl::ResourceExhaustedError(
          "Trianglemesh shape has too many texture coordinates to be stored in "
          "a 1D proto array");
    }

    for (size_t i = 0; i < st->size() / 2; i++) {
      TriangleMeshShape::UVCoordinate& dest = *output.add_uv();
      dest.set_u((*st)[2 * i + 0]);
      dest.set_v((*st)[2 * i + 1]);
    }
  }

  if (pbrt_version == 2) {
    if (std::optional<bool> discarddegenerateuvs =
            TryRemoveBool(parameters, "discarddegenerateUVs");
        discarddegenerateuvs.has_value()) {
      output.set_discarddegenerateuvs(*discarddegenerateuvs);
    }
  }

  if (pbrt_version >= 2) {
    TryRemoveFloatTexture(
        parameters, "alpha",
        std::bind(&TriangleMeshShape::mutable_alpha, &output));
  }

  if (pbrt_version == 3) {
    TryRemoveFloatTexture(
        parameters, "shadowalpha",
        std::bind(&TriangleMeshShape::mutable_shadowalpha, &output));
  }

  if (pbrt_version >= 3) {
    if (std::optional<absl::Span<int32_t>> faceIndices =
            TryRemoveIntegers(parameters, "faceIndices");
        faceIndices.has_value()) {
      if (faceIndices->size() > std::numeric_limits<int32_t>::max()) {
        return absl::ResourceExhaustedError(
            "Trianglemesh shape has too many face indices to be stored in "
            "a 1D proto array");
      }

      output.mutable_faceindices()->Add(faceIndices->begin(),
                                        faceIndices->end());
    }

    if (std::optional<absl::Span<std::array<double, 2>>> uv =
            TryRemovePoint2s(parameters, "uv");
        uv.has_value()) {
      if (uv->size() > std::numeric_limits<int32_t>::max()) {
        return absl::ResourceExhaustedError(
            "Trianglemesh shape has too many texture coordinates to be stored "
            "in a 1D proto array");
      }

      for (const std::array<double, 2>& src : *uv) {
        TriangleMeshShape::UVCoordinate& dest = *output.add_uv();
        dest.set_u(src[0]);
        dest.set_v(src[1]);
      }
    } else if (std::optional<absl::Span<std::array<double, 2>>> st =
                   TryRemovePoint2s(parameters, "st");
               st.has_value()) {
      if (st->size() > std::numeric_limits<int32_t>::max()) {
        return absl::ResourceExhaustedError(
            "Trianglemesh shape has too many texture coordinates to be stored "
            "in a 1D proto array");
      }

      for (const std::array<double, 2>& src : *st) {
        TriangleMeshShape::UVCoordinate& dest = *output.add_uv();
        dest.set_u(src[0]);
        dest.set_v(src[1]);
      }
    }
  }

  return absl::OkStatus();
}

}  // namespace pbrt_proto
