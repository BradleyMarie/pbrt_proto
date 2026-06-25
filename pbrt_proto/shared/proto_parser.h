#ifndef _PBRT_PROTO_SHARED_PROTO_PARSER_

#include <cassert>
#include <string>
#include <utility>

#include "absl/container/flat_hash_map.h"
#include "absl/functional/function_ref.h"
#include "absl/status/status.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"
#include "pbrt_proto/shared/version.h"

namespace pbrt_proto {

template <typename T, int PbrtVersion>
class ProtoParser : public Parser {
 protected:
  ProtoParser(const absl::flat_hash_map<absl::string_view, ParameterType>&
                  parameter_type_names,
              T& output)
      : Parser(parameter_type_names), output_(output) {}

  template <typename U>
  using TypeMap = absl::flat_hash_map<
      absl::string_view,
      absl::FunctionRef<absl::Status(
          absl::flat_hash_map<absl::string_view, Parameter>&, U&)>>;

  template <auto Func, auto Getter>
  auto CB() {
    return [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
              auto& output) {
      auto& field = *(output.*Getter)();
      assert(IsSupported(PbrtVersion, field));

      return Func(parameters, PbrtVersion, field);
    };
  }

  template <auto Getter>
  auto EmptyCallback() {
    return [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
              auto& output) {
      auto& field = *(output.*Getter)();
      assert(IsSupported(PbrtVersion, field));

      return absl::OkStatus();
    };
  }

  template <typename U>
  static absl::Status Ignored(
      absl::flat_hash_map<absl::string_view, Parameter>& parameters,
      U& output) {
    return absl::OkStatus();
  }

  template <typename U>
  absl::Status Parse(
      const TypeMap<U>& type_map, absl::string_view type_name,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters, U& output);

  template <auto Func, typename U>
  absl::Status Parse(
      const TypeMap<U>& type_map, absl::string_view type_name,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters);

  virtual absl::Status Material(
      absl::string_view material_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters,
      decltype(*std::declval<T>().add_directives()->mutable_material())&
          material) = 0;

  static constexpr int kPbrtVersion = PbrtVersion;

  T& output_;

 private:
  absl::Status ActiveTransform(ActiveTransformation active) final;

  absl::Status AttributeBegin() final;

  absl::Status AttributeEnd() final;

  absl::Status ConcatTransform(double m00, double m01, double m02, double m03,
                               double m10, double m11, double m12, double m13,
                               double m20, double m21, double m22, double m23,
                               double m30, double m31, double m32,
                               double m33) final;

  absl::Status CoordinateSystem(absl::string_view name) final;

  absl::Status CoordSysTransform(absl::string_view name) final;

  absl::Status Identity() final;

  absl::Status Include(absl::string_view path) final;

  absl::Status Integrator(
      absl::string_view integrator_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status Import(absl::string_view path) final;

  absl::Status LookAt(double eye_x, double eye_y, double eye_z, double look_x,
                      double look_y, double look_z, double up_x, double up_y,
                      double up_z) final;

  absl::Status MakeNamedMaterial(
      absl::string_view material_name,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) final;

  absl::Status MakeNamedMedium(
      absl::string_view medium_name,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status Material(
      absl::string_view material_name,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) final;

  absl::Status MediumInterface(absl::string_view inside,
                               absl::string_view outside) final;

  absl::Status NamedMaterial(absl::string_view material) final;

  absl::Status ObjectBegin(absl::string_view name) final;

  absl::Status ObjectEnd() final;

  absl::Status ObjectInstance(absl::string_view name) final;

  absl::Status Renderer(
      absl::string_view renderer_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status ReverseOrientation() final;

  absl::Status Rotate(double angle, double x, double y, double z) final;

  absl::Status Scale(double x, double y, double z) final;

  absl::Status SearchPath(absl::string_view path) final;

  absl::Status SurfaceIntegrator(
      absl::string_view integrator_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status Transform(double m00, double m01, double m02, double m03,
                         double m10, double m11, double m12, double m13,
                         double m20, double m21, double m22, double m23,
                         double m30, double m31, double m32, double m33) final;

  absl::Status TransformBegin() final;

  absl::Status TransformEnd() final;

  absl::Status TransformTimes(double start_time, double end_time) final;

  absl::Status Translate(double x, double y, double z) final;

  absl::Status Volume(
      absl::string_view volume_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status VolumeIntegrator(
      absl::string_view integrator_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status WorldBegin() final;

  absl::Status WorldEnd() final;

  static absl::Status UnsupportedDirectiveError(absl::string_view directive);
  static absl::Status UnrecognizedTypeError(absl::string_view directive,
                                            absl::string_view type);
  static std::string GetShortName(absl::string_view full_name);
};

template <typename T, int PbrtVersion>
template <typename U>
absl::Status ProtoParser<T, PbrtVersion>::Parse(
    const TypeMap<U>& type_map, absl::string_view type_name,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters, U& output) {
  auto iter = type_map.find(type_name);
  if (iter == type_map.end()) {
    static const std::string directive_name =
        GetShortName(output.GetTypeName());
    return UnrecognizedTypeError(directive_name, type_name);
  }

  return iter->second(parameters, output);
}

template <typename T, int PbrtVersion>
template <auto Func, typename U>
absl::Status ProtoParser<T, PbrtVersion>::Parse(
    const TypeMap<U>& type_map, absl::string_view type_name,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  return Parse(type_map, type_name, parameters,
               *(*output_.add_directives().*Func)());
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::ActiveTransform(
    ActiveTransformation active) {
  if constexpr (PbrtVersion >= 2) {
    auto& active_transform =
        *output_.add_directives()->mutable_active_transform();
    assert(IsSupported(PbrtVersion, active_transform));

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

  return UnsupportedDirectiveError("ActiveTransform");
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::AttributeBegin() {
  auto& attribute_begin = *output_.add_directives()->mutable_attribute_begin();
  assert(IsSupported(PbrtVersion, attribute_begin));

  return absl::OkStatus();
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::AttributeEnd() {
  auto& attribute_end = *output_.add_directives()->mutable_attribute_end();
  assert(IsSupported(PbrtVersion, attribute_end));

  return absl::OkStatus();
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::ConcatTransform(
    double m00, double m01, double m02, double m03, double m10, double m11,
    double m12, double m13, double m20, double m21, double m22, double m23,
    double m30, double m31, double m32, double m33) {
  auto& concat_transform =
      *output_.add_directives()->mutable_concat_transform();
  assert(IsSupported(PbrtVersion, concat_transform));

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
  auto& coordinate_system =
      *output_.add_directives()->mutable_coordinate_system();
  assert(IsSupported(PbrtVersion, coordinate_system));

  coordinate_system.set_name(name);
  return absl::OkStatus();
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::CoordSysTransform(
    absl::string_view name) {
  auto& coord_sys_transform =
      *output_.add_directives()->mutable_coord_sys_transform();
  assert(IsSupported(PbrtVersion, coord_sys_transform));

  coord_sys_transform.set_name(name);
  return absl::OkStatus();
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::Identity() {
  auto& identity = *output_.add_directives()->mutable_identity();
  assert(IsSupported(PbrtVersion, identity));

  return absl::OkStatus();
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::Include(absl::string_view path) {
  auto& include = *output_.add_directives()->mutable_include();
  assert(IsSupported(PbrtVersion, include));

  include.set_path(path);
  return absl::OkStatus();
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::Integrator(
    absl::string_view integrator_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  return UnsupportedDirectiveError("Integrator");
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::Import(absl::string_view path) {
  if constexpr (PbrtVersion >= 4) {
    auto& import = *output_.add_directives()->mutable_import();
    assert(IsSupported(PbrtVersion, import));

    import.set_path(path);
    return absl::OkStatus();
  }

  return UnsupportedDirectiveError("Import");
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::LookAt(double eye_x, double eye_y,
                                                 double eye_z, double look_x,
                                                 double look_y, double look_z,
                                                 double up_x, double up_y,
                                                 double up_z) {
  auto& look_at = *output_.add_directives()->mutable_look_at();
  assert(IsSupported(PbrtVersion, look_at));

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
  if constexpr (PbrtVersion >= 2) {
    auto& make_named_material =
        *output_.add_directives()->mutable_make_named_material();
    make_named_material.set_name(material_name);
    return Material(TryRemoveString(parameters, "type").value_or(""),
                    parameters, *make_named_material.mutable_material());
  }

  return UnsupportedDirectiveError("MakeNamedMaterial");
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::MakeNamedMedium(
    absl::string_view medium_name,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  return UnsupportedDirectiveError("MakeNamedMedium");
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::Material(
    absl::string_view material_name,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  return Material(material_name, parameters,
                  *output_.add_directives()->mutable_material());
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::MediumInterface(
    absl::string_view inside, absl::string_view outside) {
  if constexpr (PbrtVersion == 3) {
    auto& material_interface =
        *output_.add_directives()->mutable_medium_interface();
    assert(IsSupported(PbrtVersion, material_interface));

    material_interface.set_inside(inside);
    material_interface.set_outside(outside);
    return absl::OkStatus();
  }

  return UnsupportedDirectiveError("MediumInterface");
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::NamedMaterial(
    absl::string_view material) {
  if constexpr (PbrtVersion >= 2) {
    auto& named_material = *output_.add_directives()->mutable_named_material();
    assert(IsSupported(PbrtVersion, named_material));

    named_material.set_name(material);
    return absl::OkStatus();
  }

  return UnsupportedDirectiveError("NamedMaterial");
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::ObjectBegin(absl::string_view name) {
  auto& object_begin = *output_.add_directives()->mutable_object_begin();
  assert(IsSupported(PbrtVersion, object_begin));

  object_begin.set_name(name);
  return absl::OkStatus();
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::ObjectEnd() {
  auto& object_end = *output_.add_directives()->mutable_object_end();
  assert(IsSupported(PbrtVersion, object_end));

  return absl::OkStatus();
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::ObjectInstance(
    absl::string_view name) {
  auto& object_instance = *output_.add_directives()->mutable_object_instance();
  assert(IsSupported(PbrtVersion, object_instance));

  object_instance.set_name(name);
  return absl::OkStatus();
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::Renderer(
    absl::string_view integrator_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  return UnsupportedDirectiveError("Renderer");
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::ReverseOrientation() {
  auto& reverse_orientation =
      *output_.add_directives()->mutable_reverse_orientation();
  assert(IsSupported(PbrtVersion, reverse_orientation));

  return absl::OkStatus();
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::Rotate(double angle, double x,
                                                 double y, double z) {
  auto& rotate = *output_.add_directives()->mutable_rotate();
  assert(IsSupported(PbrtVersion, rotate));

  rotate.set_angle(angle);
  rotate.set_x(x);
  rotate.set_y(y);
  rotate.set_z(z);
  return absl::OkStatus();
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::Scale(double x, double y, double z) {
  auto& scale = *output_.add_directives()->mutable_scale();
  assert(IsSupported(PbrtVersion, scale));

  scale.set_x(x);
  scale.set_y(y);
  scale.set_z(z);
  return absl::OkStatus();
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::SearchPath(absl::string_view path) {
  if constexpr (PbrtVersion == 1) {
    auto& search_path = *output_.add_directives()->mutable_search_path();
    assert(IsSupported(PbrtVersion, search_path));

    search_path.set_path(path);
    return absl::OkStatus();
  }

  return UnsupportedDirectiveError("SearchPath");
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::SurfaceIntegrator(
    absl::string_view integrator_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  return UnsupportedDirectiveError("SurfaceIntegrator");
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::Transform(
    double m00, double m01, double m02, double m03, double m10, double m11,
    double m12, double m13, double m20, double m21, double m22, double m23,
    double m30, double m31, double m32, double m33) {
  auto& transform = *output_.add_directives()->mutable_transform();
  assert(IsSupported(PbrtVersion, transform));

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
  if (PbrtVersion <= 3) {
    auto& transform_begin =
        *output_.add_directives()->mutable_transform_begin();
    assert(IsSupported(PbrtVersion, transform_begin));

    return absl::OkStatus();
  }

  return UnsupportedDirectiveError("TransformBegin");
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::TransformEnd() {
  if (PbrtVersion <= 3) {
    auto& transform_end = *output_.add_directives()->mutable_transform_end();
    assert(IsSupported(PbrtVersion, transform_end));

    return absl::OkStatus();
  }

  return UnsupportedDirectiveError("TransformEnd");
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::TransformTimes(double start_time,
                                                         double end_time) {
  if constexpr (PbrtVersion >= 2) {
    auto& transform_times =
        *output_.add_directives()->mutable_transform_times();
    assert(IsSupported(PbrtVersion, transform_times));

    transform_times.set_start_time(start_time);
    transform_times.set_end_time(end_time);
    return absl::OkStatus();
  }

  return UnsupportedDirectiveError("TransformTimes");
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::Translate(double x, double y,
                                                    double z) {
  auto& translate = *output_.add_directives()->mutable_translate();
  assert(IsSupported(PbrtVersion, translate));

  translate.set_x(x);
  translate.set_y(y);
  translate.set_z(z);
  return absl::OkStatus();
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::Volume(
    absl::string_view volume_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  return UnsupportedDirectiveError("Volume");
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::VolumeIntegrator(
    absl::string_view integrator_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  return UnsupportedDirectiveError("VolumeIntegrator");
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::WorldBegin() {
  auto& world_begin = *output_.add_directives()->mutable_world_begin();
  assert(IsSupported(PbrtVersion, world_begin));

  return absl::OkStatus();
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::WorldEnd() {
  if constexpr (PbrtVersion <= 3) {
    auto& world_end = *output_.add_directives()->mutable_world_end();
    assert(IsSupported(PbrtVersion, world_end));

    return absl::OkStatus();
  }

  return UnsupportedDirectiveError("WorldEnd");
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::UnsupportedDirectiveError(
    absl::string_view directive) {
  return absl::UnimplementedError(absl::StrCat(
      "Directive '", directive, "' is not supported in pbrt-v", PbrtVersion));
}

template <typename T, int PbrtVersion>
absl::Status ProtoParser<T, PbrtVersion>::UnrecognizedTypeError(
    absl::string_view directive, absl::string_view type) {
  std::string message =
      absl::StrCat(directive, " type \'", type, "\' is not supported in pbrt-v",
                   PbrtVersion);

  if constexpr (PbrtVersion < 4) {
    std::cerr << "WARNING: " << message << std::endl;
    return absl::OkStatus();
  }

  return absl::InvalidArgumentError(message);
}

template <typename T, int PbrtVersion>
std::string ProtoParser<T, PbrtVersion>::GetShortName(
    absl::string_view full_name) {
  size_t last_dot = full_name.find_last_of('.');
  if (last_dot == absl::string_view::npos) {
    return std::string(full_name);
  }

  absl::string_view result = full_name.substr(last_dot + 1);
  if (result == "FloatTexture" || result == "SpectrumTexture") {
    return "Texture";
  }

  return std::string(result);
}

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_PROTO_PARSER_
