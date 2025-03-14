#include "pbrt_proto/v3/convert.h"

#include <variant>

#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto::v3 {
namespace {

static const absl::flat_hash_map<absl::string_view, ParameterType>
    parameter_type_names = {
        {
            "blackbody",
            ParameterType::BLACKBODY_V1,
        },
        {
            "bool",
            ParameterType::BOOL,
        },
        {
            "float",
            ParameterType::FLOAT,
        },
        {
            "integer",
            ParameterType::INTEGER,
        },
        {
            "normal",
            ParameterType::NORMAL3,
        },
        {
            "normal3",
            ParameterType::NORMAL3,
        },
        {
            "point",
            ParameterType::POINT3,
        },
        {
            "point2",
            ParameterType::POINT2,
        },
        {
            "point3",
            ParameterType::POINT3,
        },
        {
            "rgb",
            ParameterType::RGB,
        },
        {
            "spectrum",
            ParameterType::SPECTRUM,
        },
        {"string", ParameterType::STRING},
        {"texture", ParameterType::STRING},
        {
            "vector",
            ParameterType::VECTOR3,
        },
        {
            "vector2",
            ParameterType::VECTOR2,
        },
        {
            "vector3",
            ParameterType::VECTOR3,
        },
        {
            "xyz",
            ParameterType::XYZ,
        },
};

class ParserV3 final : public Parser {
 public:
  ParserV3(PbrtProto& output) noexcept
      : Parser(parameter_type_names), output_(output) {}

 private:
  absl::Status Accelerator(
      absl::string_view accelerator_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status ActiveTransform(ActiveTransformation active) override;

  absl::Status ConcatTransform(double m00, double m01, double m02, double m03,
                               double m10, double m11, double m12, double m13,
                               double m20, double m21, double m22, double m23,
                               double m30, double m31, double m32,
                               double m33) override;

  absl::Status CoordinateSystem(absl::string_view name) override;

  absl::Status CoordSysTransform(absl::string_view name) override;

  absl::Status Identity() override;

  absl::Status Include(absl::string_view path) override;

  absl::Status Import(absl::string_view path) override;

  absl::Status LookAt(double eye_x, double eye_y, double eye_z, double look_x,
                      double look_y, double look_z, double up_x, double up_y,
                      double up_z) override;

  absl::Status ReverseOrientation() override;

  absl::Status Rotate(double angle, double x, double y, double z) override;

  absl::Status Scale(double x, double y, double z) override;

  absl::Status Transform(double m00, double m01, double m02, double m03,
                         double m10, double m11, double m12, double m13,
                         double m20, double m21, double m22, double m23,
                         double m30, double m31, double m32,
                         double m33) override;

  absl::Status TransformTimes(double start_time, double end_time) override;

  absl::Status Translate(double x, double y, double z) override;

  absl::Status WorldBegin() override;

  absl::Status WorldEnd() override;

  PbrtProto& output_;
};

absl::Status ParserV3::Accelerator(
    absl::string_view accelerator_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  if (accelerator_type == "bvh") {
    auto& bvh = *output_.add_directives()->mutable_accelerator()->mutable_bvh();

    if (std::optional<int32_t> maxnodeprims =
            TryRemoveInteger(parameters, "maxnodeprims");
        maxnodeprims.has_value()) {
      bvh.set_maxnodeprims(*maxnodeprims);
    }

    if (std::optional<absl::string_view> splitmethod =
            TryRemoveString(parameters, "splitmethod");
        splitmethod.has_value()) {
      if (*splitmethod == "sah") {
        bvh.set_splitmethod(Accelerator::Bvh::SAH);
      } else if (*splitmethod == "middle") {
        bvh.set_splitmethod(Accelerator::Bvh::MIDDLE);
      } else if (*splitmethod == "equal") {
        bvh.set_splitmethod(Accelerator::Bvh::EQUAL);
      } else if (*splitmethod == "hlbvh") {
        bvh.set_splitmethod(Accelerator::Bvh::HLBVH);
      }
    }
  } else if (accelerator_type == "kdtree") {
    auto& kdtree =
        *output_.add_directives()->mutable_accelerator()->mutable_kdtree();

    if (std::optional<double> emptybonus =
            TryRemoveFloat(parameters, "emptybonus");
        emptybonus.has_value()) {
      kdtree.set_emptybonus(*emptybonus);
    }

    if (std::optional<int32_t> intersectcost =
            TryRemoveInteger(parameters, "intersectcost");
        intersectcost.has_value()) {
      kdtree.set_intersectcost(*intersectcost);
    }

    if (std::optional<int32_t> maxdepth =
            TryRemoveInteger(parameters, "maxdepth");
        maxdepth.has_value()) {
      kdtree.set_maxdepth(*maxdepth);
    }

    if (std::optional<int32_t> maxprims =
            TryRemoveInteger(parameters, "maxprims");
        maxprims.has_value()) {
      kdtree.set_maxprims(*maxprims);
    }

    if (std::optional<int32_t> traversalcost =
            TryRemoveInteger(parameters, "traversalcost");
        traversalcost.has_value()) {
      kdtree.set_traversalcost(*traversalcost);
    }
  }

  return absl::OkStatus();
}

absl::Status ParserV3::ActiveTransform(ActiveTransformation active) {
  auto* active_transform = output_.add_directives()->mutable_active_transform();
  switch (active) {
    case ActiveTransformation::ALL:
      active_transform->set_active(ActiveTransform::ALL);
      break;
    case ActiveTransformation::START_TIME:
      active_transform->set_active(ActiveTransform::START_TIME);
      break;
    case ActiveTransformation::END_TIME:
      active_transform->set_active(ActiveTransform::END_TIME);
      break;
  }
  return absl::OkStatus();
}

absl::Status ParserV3::ConcatTransform(double m00, double m01, double m02,
                                       double m03, double m10, double m11,
                                       double m12, double m13, double m20,
                                       double m21, double m22, double m23,
                                       double m30, double m31, double m32,
                                       double m33) {
  auto* concat_transform = output_.add_directives()->mutable_concat_transform();
  concat_transform->set_m00(m00);
  concat_transform->set_m01(m01);
  concat_transform->set_m02(m02);
  concat_transform->set_m03(m03);
  concat_transform->set_m10(m10);
  concat_transform->set_m11(m11);
  concat_transform->set_m12(m12);
  concat_transform->set_m13(m13);
  concat_transform->set_m20(m20);
  concat_transform->set_m21(m21);
  concat_transform->set_m22(m22);
  concat_transform->set_m23(m23);
  concat_transform->set_m30(m30);
  concat_transform->set_m31(m31);
  concat_transform->set_m32(m32);
  concat_transform->set_m33(m33);
  return absl::OkStatus();
}

absl::Status ParserV3::CoordinateSystem(absl::string_view name) {
  output_.add_directives()->mutable_coordinate_system()->set_name(name);
  return absl::OkStatus();
}

absl::Status ParserV3::CoordSysTransform(absl::string_view name) {
  output_.add_directives()->mutable_coord_sys_transform()->set_name(name);
  return absl::OkStatus();
}

absl::Status ParserV3::Identity() {
  output_.add_directives()->mutable_identity();
  return absl::OkStatus();
}

absl::Status ParserV3::Include(absl::string_view path) {
  output_.add_directives()->mutable_include()->set_path(path);
  return absl::OkStatus();
}

absl::Status ParserV3::Import(absl::string_view path) {
  return absl::UnimplementedError(
      "Directive 'Import' is not supported in pbrt-v3");
}

absl::Status ParserV3::LookAt(double eye_x, double eye_y, double eye_z,
                              double look_x, double look_y, double look_z,
                              double up_x, double up_y, double up_z) {
  auto* look_at = output_.add_directives()->mutable_look_at();
  look_at->set_eye_x(eye_x);
  look_at->set_eye_y(eye_y);
  look_at->set_eye_z(eye_z);
  look_at->set_look_x(look_x);
  look_at->set_look_y(look_y);
  look_at->set_look_z(look_z);
  look_at->set_up_x(up_x);
  look_at->set_up_y(up_y);
  look_at->set_up_z(up_z);
  return absl::OkStatus();
}

absl::Status ParserV3::ReverseOrientation() {
  output_.add_directives()->mutable_reverse_orientation();
  return absl::OkStatus();
}

absl::Status ParserV3::Rotate(double angle, double x, double y, double z) {
  auto* rotate = output_.add_directives()->mutable_rotate();
  rotate->set_angle(angle);
  rotate->set_x(x);
  rotate->set_y(y);
  rotate->set_z(z);
  return absl::OkStatus();
}

absl::Status ParserV3::Scale(double x, double y, double z) {
  auto* scale = output_.add_directives()->mutable_scale();
  scale->set_x(x);
  scale->set_y(y);
  scale->set_z(z);
  return absl::OkStatus();
}

absl::Status ParserV3::Transform(double m00, double m01, double m02, double m03,
                                 double m10, double m11, double m12, double m13,
                                 double m20, double m21, double m22, double m23,
                                 double m30, double m31, double m32,
                                 double m33) {
  auto* transform = output_.add_directives()->mutable_transform();
  transform->set_m00(m00);
  transform->set_m01(m01);
  transform->set_m02(m02);
  transform->set_m03(m03);
  transform->set_m10(m10);
  transform->set_m11(m11);
  transform->set_m12(m12);
  transform->set_m13(m13);
  transform->set_m20(m20);
  transform->set_m21(m21);
  transform->set_m22(m22);
  transform->set_m23(m23);
  transform->set_m30(m30);
  transform->set_m31(m31);
  transform->set_m32(m32);
  transform->set_m33(m33);
  return absl::OkStatus();
}

absl::Status ParserV3::TransformTimes(double start_time, double end_time) {
  auto* transform_times = output_.add_directives()->mutable_transform_times();
  transform_times->set_start_time(start_time);
  transform_times->set_end_time(end_time);
  return absl::OkStatus();
}

absl::Status ParserV3::Translate(double x, double y, double z) {
  auto* translate = output_.add_directives()->mutable_translate();
  translate->set_x(x);
  translate->set_y(y);
  translate->set_z(z);
  return absl::OkStatus();
}

absl::Status ParserV3::WorldBegin() {
  output_.add_directives()->mutable_world_begin();
  return absl::OkStatus();
}

absl::Status ParserV3::WorldEnd() {
  output_.add_directives()->mutable_world_end();
  return absl::OkStatus();
}

}  // namespace

absl::StatusOr<PbrtProto> Convert(std::istream& input) {
  PbrtProto output;
  if (absl::Status error = ParserV3(output).ReadFrom(input); !error.ok()) {
    return error;
  }
  return output;
}

}  // namespace pbrt_proto::v3