#ifndef _PBRT_PROTO_SHARED_PROTO_PARSER_

#include "absl/container/flat_hash_map.h"
#include "absl/status/status.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {

template <typename T, int PbrtVersion>
class ProtoParser : public Parser {
 protected:
  ProtoParser(const absl::flat_hash_map<absl::string_view, ParameterType>&
                  parameter_type_names,
              T& output)
      : Parser(parameter_type_names), output_(output) {}

  T& output_;

 private:
  virtual absl::Status ActiveTransform(ActiveTransformation active) final;

  virtual absl::Status AttributeBegin() final;

  virtual absl::Status AttributeEnd() final;

  virtual absl::Status ConcatTransform(double m00, double m01, double m02,
                                       double m03, double m10, double m11,
                                       double m12, double m13, double m20,
                                       double m21, double m22, double m23,
                                       double m30, double m31, double m32,
                                       double m33) final;

  virtual absl::Status CoordinateSystem(absl::string_view name) final;

  virtual absl::Status CoordSysTransform(absl::string_view name) final;

  virtual absl::Status Identity() final;

  virtual absl::Status Include(absl::string_view path) final;

  virtual absl::Status Integrator(
      absl::string_view integrator_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  virtual absl::Status Import(absl::string_view path) final;

  virtual absl::Status LookAt(double eye_x, double eye_y, double eye_z,
                              double look_x, double look_y, double look_z,
                              double up_x, double up_y, double up_z) final;

  virtual absl::Status MakeNamedMaterial(
      absl::string_view material_name,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  virtual absl::Status MakeNamedMedium(
      absl::string_view medium_name,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  virtual absl::Status MediumInterface(absl::string_view inside,
                                       absl::string_view outside) override;

  virtual absl::Status NamedMaterial(absl::string_view material) override;

  virtual absl::Status ObjectBegin(absl::string_view name) final;

  virtual absl::Status ObjectEnd() final;

  virtual absl::Status ObjectInstance(absl::string_view name) final;

  virtual absl::Status ReverseOrientation() final;

  virtual absl::Status Rotate(double angle, double x, double y, double z) final;

  virtual absl::Status Scale(double x, double y, double z) final;

  virtual absl::Status SearchPath(absl::string_view path) final;

  virtual absl::Status SurfaceIntegrator(
      absl::string_view integrator_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  virtual absl::Status Transform(double m00, double m01, double m02, double m03,
                                 double m10, double m11, double m12, double m13,
                                 double m20, double m21, double m22, double m23,
                                 double m30, double m31, double m32,
                                 double m33) final;

  virtual absl::Status TransformBegin() final;

  virtual absl::Status TransformEnd() final;

  virtual absl::Status TransformTimes(double start_time, double end_time) final;

  virtual absl::Status Translate(double x, double y, double z) final;

  virtual absl::Status Volume(
      absl::string_view volume_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  virtual absl::Status VolumeIntegrator(
      absl::string_view integrator_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  virtual absl::Status WorldBegin() final;

  virtual absl::Status WorldEnd() final;

  absl::Status UnsupportedError(absl::string_view directive) const;
};

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::UnsupportedError(
    absl::string_view directive) const {
  return absl::UnimplementedError(absl::StrCat(
      "Directive '", directive, "' is not supported in pbrt-v", PbrtVersion));
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::ActiveTransform(
    ActiveTransformation active) {
  if constexpr (PbrtVersion >= 2) {
    auto& active_transform =
        *output_.add_directives()->mutable_active_transform();
    switch (active) {
      case ActiveTransformation::ALL:
        active_transform.set_active(ActiveTransform::ALL);
        break;
      case ActiveTransformation::START_TIME:
        active_transform.set_active(ActiveTransform::START_TIME);
        break;
      case ActiveTransformation::END_TIME:
        active_transform.set_active(ActiveTransform::END_TIME);
        break;
    }

    return absl::OkStatus();
  }

  return UnsupportedError("ActiveTransformation");
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::AttributeBegin() {
  output_.add_directives()->mutable_attribute_begin();
  return absl::OkStatus();
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::AttributeEnd() {
  output_.add_directives()->mutable_attribute_end();
  return absl::OkStatus();
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::ConcatTransform(
    double m00, double m01, double m02, double m03, double m10, double m11,
    double m12, double m13, double m20, double m21, double m22, double m23,
    double m30, double m31, double m32, double m33) {
  auto& concat_transform =
      *output_.add_directives()->mutable_concat_transform();
  concat_transform.set_m00(m00);
  concat_transform.set_m01(m01);
  concat_transform.set_m02(m02);
  concat_transform.set_m03(m03);
  concat_transform.set_m10(m10);
  concat_transform.set_m11(m11);
  concat_transform.set_m12(m12);
  concat_transform.set_m13(m13);
  concat_transform.set_m20(m20);
  concat_transform.set_m21(m21);
  concat_transform.set_m22(m22);
  concat_transform.set_m23(m23);
  concat_transform.set_m30(m30);
  concat_transform.set_m31(m31);
  concat_transform.set_m32(m32);
  concat_transform.set_m33(m33);
  return absl::OkStatus();
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::CoordinateSystem(
    absl::string_view name) {
  output_.add_directives()->mutable_coordinate_system()->set_name(name);
  return absl::OkStatus();
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::CoordSysTransform(
    absl::string_view name) {
  output_.add_directives()->mutable_coord_sys_transform()->set_name(name);
  return absl::OkStatus();
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::Identity() {
  output_.add_directives()->mutable_identity();
  return absl::OkStatus();
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::Include(absl::string_view path) {
  output_.add_directives()->mutable_include()->set_path(path);
  return absl::OkStatus();
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::Integrator(
    absl::string_view integrator_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  return UnsupportedError("Integrator");
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::Import(absl::string_view path) {
  if constexpr (PbrtVersion >= 4) {
    output_.add_directives()->mutable_import()->set_path(path);
  }

  return UnsupportedError("Import");
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::LookAt(double eye_x, double eye_y,
                                                 double eye_z, double look_x,
                                                 double look_y, double look_z,
                                                 double up_x, double up_y,
                                                 double up_z) {
  auto& look_at = *output_.add_directives()->mutable_look_at();
  look_at.set_eye_x(eye_x);
  look_at.set_eye_y(eye_y);
  look_at.set_eye_z(eye_z);
  look_at.set_look_x(look_x);
  look_at.set_look_y(look_y);
  look_at.set_look_z(look_z);
  look_at.set_up_x(up_x);
  look_at.set_up_y(up_y);
  look_at.set_up_z(up_z);
  return absl::OkStatus();
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::MakeNamedMaterial(
    absl::string_view material_name,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  return UnsupportedError("MakeNamedMaterial");
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::MakeNamedMedium(
    absl::string_view medium_name,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  return UnsupportedError("MakeNamedMedium");
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::MediumInterface(
    absl::string_view inside, absl::string_view outside) {
  return UnsupportedError("MediumInterface");
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::NamedMaterial(
    absl::string_view material) {
  return UnsupportedError("NamedMaterial");
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::ObjectBegin(absl::string_view name) {
  output_.add_directives()->mutable_object_begin()->set_name(name);
  return absl::OkStatus();
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::ObjectEnd() {
  output_.add_directives()->mutable_object_end();
  return absl::OkStatus();
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::ObjectInstance(
    absl::string_view name) {
  output_.add_directives()->mutable_object_instance()->set_name(name);
  return absl::OkStatus();
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::ReverseOrientation() {
  output_.add_directives()->mutable_reverse_orientation();
  return absl::OkStatus();
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::Rotate(double angle, double x,
                                                 double y, double z) {
  auto& rotate = *output_.add_directives()->mutable_rotate();
  rotate.set_angle(angle);
  rotate.set_x(x);
  rotate.set_y(y);
  rotate.set_z(z);
  return absl::OkStatus();
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::Scale(double x, double y, double z) {
  auto& scale = *output_.add_directives()->mutable_scale();
  scale.set_x(x);
  scale.set_y(y);
  scale.set_z(z);
  return absl::OkStatus();
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::SearchPath(absl::string_view path) {
  if constexpr (PbrtVersion == 1) {
    output_.add_directives()->mutable_search_path()->set_path(path);
    return absl::OkStatus();
  }

  return UnsupportedError("SearchPath");
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::SurfaceIntegrator(
    absl::string_view integrator_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  return UnsupportedError("SurfaceIntegrator");
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::Transform(
    double m00, double m01, double m02, double m03, double m10, double m11,
    double m12, double m13, double m20, double m21, double m22, double m23,
    double m30, double m31, double m32, double m33) {
  auto& transform = *output_.add_directives()->mutable_transform();
  transform.set_m00(m00);
  transform.set_m01(m01);
  transform.set_m02(m02);
  transform.set_m03(m03);
  transform.set_m10(m10);
  transform.set_m11(m11);
  transform.set_m12(m12);
  transform.set_m13(m13);
  transform.set_m20(m20);
  transform.set_m21(m21);
  transform.set_m22(m22);
  transform.set_m23(m23);
  transform.set_m30(m30);
  transform.set_m31(m31);
  transform.set_m32(m32);
  transform.set_m33(m33);
  return absl::OkStatus();
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::TransformBegin() {
  output_.add_directives()->mutable_transform_begin();
  return absl::OkStatus();
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::TransformEnd() {
  output_.add_directives()->mutable_transform_end();
  return absl::OkStatus();
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::TransformTimes(double start_time,
                                                         double end_time) {
  if constexpr (PbrtVersion >= 2) {
    auto& transform_times =
        *output_.add_directives()->mutable_transform_times();
    transform_times.set_start_time(start_time);
    transform_times.set_end_time(end_time);
    return absl::OkStatus();
  }

  return UnsupportedError("TransformTimes");
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::Translate(double x, double y,
                                                    double z) {
  auto& translate = *output_.add_directives()->mutable_translate();
  translate.set_x(x);
  translate.set_y(y);
  translate.set_z(z);
  return absl::OkStatus();
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::Volume(
    absl::string_view volume_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  return UnsupportedError("Volume");
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::VolumeIntegrator(
    absl::string_view integrator_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  return UnsupportedError("VolumeIntegrator");
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::WorldBegin() {
  output_.add_directives()->mutable_world_begin();
  return absl::OkStatus();
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::WorldEnd() {
  if constexpr (PbrtVersion <= 3) {
    output_.add_directives()->mutable_world_end();
    return absl::OkStatus();
  }

  return UnsupportedError("WorldEnd");
}

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_PROTO_PARSER_
