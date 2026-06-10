#include "pbrt_proto/v1/convert.h"

#include <algorithm>
#include <array>
#include <functional>
#include <istream>
#include <variant>

#include "absl/container/flat_hash_map.h"
#include "absl/functional/function_ref.h"
#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"
#include "absl/types/span.h"
#include "pbrt_proto/shared/accelerators.h"
#include "pbrt_proto/shared/area_light_sources.h"
#include "pbrt_proto/shared/cameras.h"
#include "pbrt_proto/shared/common.h"
#include "pbrt_proto/shared/films.h"
#include "pbrt_proto/shared/integrators.h"
#include "pbrt_proto/shared/light_sources.h"
#include "pbrt_proto/shared/materials.h"
#include "pbrt_proto/shared/media.h"
#include "pbrt_proto/shared/parser.h"
#include "pbrt_proto/shared/pixel_filters.h"
#include "pbrt_proto/shared/samplers.h"
#include "pbrt_proto/shared/shapes.h"
#include "pbrt_proto/shared/textures.h"
#include "pbrt_proto/v1/v1.pb.h"

namespace pbrt_proto::v1 {
namespace {

static const absl::flat_hash_map<absl::string_view, ParameterType>
    parameter_type_names = {
        {
            "bool",
            ParameterType::BOOL,
        },
        {
            "color",
            ParameterType::RGB,
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
            "point",
            ParameterType::POINT3,
        },
        {"string", ParameterType::STRING},
        {"texture", ParameterType::TEXTURE},
        {
            "vector",
            ParameterType::VECTOR3,
        },
};

class ParserV1 final : public Parser {
 public:
  ParserV1(PbrtProto& output) noexcept
      : Parser(parameter_type_names), output_(output) {}

 private:
  absl::Status Accelerator(
      absl::string_view accelerator_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status ActiveTransform(ActiveTransformation active) override;

  absl::Status AreaLightSource(
      absl::string_view area_light_source_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status AttributeBegin() override;

  absl::Status AttributeEnd() override;

  absl::Status Camera(
      absl::string_view camera_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status ConcatTransform(double m00, double m01, double m02, double m03,
                               double m10, double m11, double m12, double m13,
                               double m20, double m21, double m22, double m23,
                               double m30, double m31, double m32,
                               double m33) override;

  absl::Status CoordinateSystem(absl::string_view name) override;

  absl::Status CoordSysTransform(absl::string_view name) override;

  absl::Status Film(
      absl::string_view film_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status FloatTexture(
      absl::string_view float_texture_name,
      absl::string_view float_texture_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status Identity() override;

  absl::Status Include(absl::string_view path) override;

  absl::Status Integrator(
      absl::string_view integrator_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status Import(absl::string_view path) override;

  absl::Status LightSource(
      absl::string_view light_source_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status LookAt(double eye_x, double eye_y, double eye_z, double look_x,
                      double look_y, double look_z, double up_x, double up_y,
                      double up_z) override;

  absl::Status MakeNamedMaterial(
      absl::string_view material_name,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status MakeNamedMedium(
      absl::string_view medium_name,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status Material(
      absl::string_view material_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status MediumInterface(absl::string_view inside,
                               absl::string_view outside) override;

  absl::Status NamedMaterial(absl::string_view material) override;

  absl::Status ObjectBegin(absl::string_view path) override;

  absl::Status ObjectEnd() override;

  absl::Status ObjectInstance(absl::string_view path) override;

  absl::Status PixelFilter(
      absl::string_view filter_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status ReverseOrientation() override;

  absl::Status Rotate(double angle, double x, double y, double z) override;

  absl::Status Sampler(
      absl::string_view sampler_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status Scale(double x, double y, double z) override;

  absl::Status SearchPath(absl::string_view path) override;

  absl::Status Shape(
      absl::string_view shape_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status SpectrumTexture(
      absl::string_view spectrum_texture_name,
      absl::string_view spectrum_texture_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status SurfaceIntegrator(
      absl::string_view integrator_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status Transform(double m00, double m01, double m02, double m03,
                         double m10, double m11, double m12, double m13,
                         double m20, double m21, double m22, double m23,
                         double m30, double m31, double m32,
                         double m33) override;

  absl::Status TransformTimes(double start_time, double end_time) override;

  absl::Status TransformBegin() override;

  absl::Status TransformEnd() override;

  absl::Status Translate(double x, double y, double z) override;

  absl::Status Volume(
      absl::string_view volume_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status VolumeIntegrator(
      absl::string_view integrator_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status WorldBegin() override;

  absl::Status WorldEnd() override;

  PbrtProto& output_;
};

absl::Status ParserV1::Accelerator(
    absl::string_view accelerator_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& accelerator = *output_.add_directives()->mutable_accelerator();

  if (accelerator_type == "grid") {
    RemoveGridAcceleratorV1(parameters, *accelerator.mutable_grid());
  } else if (accelerator_type == "kdtree") {
    RemoveKdTreeAcceleratorV1(parameters, *accelerator.mutable_kdtree());
  } else {
    std::cerr << "Unrecognized Accelerator type: \"" << accelerator_type << "\""
              << std::endl;
  }

  return absl::OkStatus();
}

absl::Status ParserV1::ActiveTransform(ActiveTransformation active) {
  return absl::UnimplementedError(
      "Directive 'ActiveTransform' is not supported in pbrt-v1");
}

absl::Status ParserV1::AreaLightSource(
    absl::string_view area_light_source_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& area_light_source =
      *output_.add_directives()->mutable_area_light_source();

  if (area_light_source_type == "area") {
    return RemoveDiffuseAreaLightSourceV1(parameters,
                                          *area_light_source.mutable_area());
  }

  std::cerr << "Unrecognized AreaLightSource type: \"" << area_light_source_type
            << "\"" << std::endl;

  return absl::OkStatus();
}

absl::Status ParserV1::AttributeBegin() {
  output_.add_directives()->mutable_attribute_begin();
  return absl::OkStatus();
}

absl::Status ParserV1::AttributeEnd() {
  output_.add_directives()->mutable_attribute_end();
  return absl::OkStatus();
}

absl::Status ParserV1::Camera(
    absl::string_view camera_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& camera = *output_.add_directives()->mutable_camera();

  if (camera_type == "environment") {
    RemoveSphericalCameraV1(parameters, *camera.mutable_environment());
  } else if (camera_type == "orthographic") {
    RemoveOrthographicCameraV1(parameters, *camera.mutable_orthographic());
  } else if (camera_type == "perspective") {
    RemovePerspectiveCameraV1(parameters, *camera.mutable_perspective());
  } else {
    std::cerr << "Unrecognized Camera type: \"" << camera_type << "\""
              << std::endl;
  }

  return absl::OkStatus();
}

absl::Status ParserV1::ConcatTransform(double m00, double m01, double m02,
                                       double m03, double m10, double m11,
                                       double m12, double m13, double m20,
                                       double m21, double m22, double m23,
                                       double m30, double m31, double m32,
                                       double m33) {
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

absl::Status ParserV1::CoordinateSystem(absl::string_view name) {
  output_.add_directives()->mutable_coordinate_system()->set_name(name);
  return absl::OkStatus();
}

absl::Status ParserV1::CoordSysTransform(absl::string_view name) {
  output_.add_directives()->mutable_coord_sys_transform()->set_name(name);
  return absl::OkStatus();
}

absl::Status ParserV1::Film(
    absl::string_view film_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& film = *output_.add_directives()->mutable_film();

  if (film_type == "image") {
    RemoveRgbFilmV1(parameters, *film.mutable_image());
  } else {
    std::cerr << "Unrecognized Film type: \"" << film_type << "\"" << std::endl;
  }

  return absl::OkStatus();
}

absl::Status ParserV1::FloatTexture(
    absl::string_view float_texture_name, absl::string_view float_texture_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& float_texture = *output_.add_directives()->mutable_float_texture();

  float_texture.set_name(float_texture_name);

  if (float_texture_type == "bilerp") {
    RemoveBilerpFloatTextureV1(parameters, *float_texture.mutable_bilerp());
  } else if (float_texture_type == "checkerboard") {
    int32_t dimension = TryRemoveInteger(parameters, "dimension").value_or(2);
    if (dimension == 2) {
      RemoveCheckerboard2DFloatTextureV1(
          parameters, *float_texture.mutable_checkerboard2d());
    } else {
      RemoveCheckerboard3DFloatTextureV1(
          parameters, *float_texture.mutable_checkerboard3d());
    }
  } else if (float_texture_type == "constant") {
    RemoveConstantFloatTextureV1(parameters, *float_texture.mutable_constant());
  } else if (float_texture_type == "dots") {
    RemoveDotsFloatTextureV1(parameters, *float_texture.mutable_dots());
  } else if (float_texture_type == "fbm") {
    RemoveFBmFloatTextureV1(parameters, *float_texture.mutable_fbm());
  } else if (float_texture_type == "imagemap") {
    RemoveImageMapFloatTextureV1(parameters, *float_texture.mutable_imagemap());
  } else if (float_texture_type == "marble") {
    // Ignored
  } else if (float_texture_type == "mix") {
    RemoveMixFloatTextureV1(parameters, *float_texture.mutable_mix());
  } else if (float_texture_type == "scale") {
    RemoveScaleFloatTextureV1(parameters, *float_texture.mutable_scale());
  } else if (float_texture_type == "windy") {
    float_texture.mutable_windy();
  } else if (float_texture_type == "wrinkled") {
    RemoveWrinkledFloatTextureV1(parameters, *float_texture.mutable_wrinkled());
  } else {
    std::cerr << "Unrecognized Texture type: \"" << float_texture_type << "\""
              << std::endl;
  }

  return absl::OkStatus();
}

absl::Status ParserV1::Identity() {
  output_.add_directives()->mutable_identity();
  return absl::OkStatus();
}

absl::Status ParserV1::Include(absl::string_view path) {
  output_.add_directives()->mutable_include()->set_path(path);
  return absl::OkStatus();
}

absl::Status ParserV1::Integrator(
    absl::string_view integrator_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  return absl::UnimplementedError(
      "Directive 'Integrator' is not supported in pbrt-v1");
}

absl::Status ParserV1::Import(absl::string_view path) {
  return absl::UnimplementedError(
      "Directive 'Import' is not supported in pbrt-v1");
}

absl::Status ParserV1::LightSource(
    absl::string_view light_source_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& light_source = *output_.add_directives()->mutable_light_source();

  if (light_source_type == "distant") {
    return RemoveDistantLightSourceV1(parameters,
                                      *light_source.mutable_distant());
  }

  if (light_source_type == "goniometric") {
    return RemoveGoniometricLightSourceV1(parameters,
                                          *light_source.mutable_goniometric());
  }

  if (light_source_type == "infinite") {
    return RemoveInfiniteLightSourceV1(parameters,
                                       *light_source.mutable_infinite());
  }

  if (light_source_type == "infinitesample") {
    return RemoveInfiniteLightSourceV1(parameters,
                                       *light_source.mutable_infinitesample());
  }

  if (light_source_type == "point") {
    return RemovePointLightSourceV1(parameters, *light_source.mutable_point());
  }

  if (light_source_type == "projection") {
    return RemoveProjectionLightSourceV1(parameters,
                                         *light_source.mutable_projection());
  }

  if (light_source_type == "spot") {
    return RemoveSpotLightSourceV1(parameters, *light_source.mutable_spot());
  }

  std::cerr << "Unrecognized LightSource type: \"" << light_source_type << "\""
            << std::endl;

  return absl::OkStatus();
}

absl::Status ParserV1::LookAt(double eye_x, double eye_y, double eye_z,
                              double look_x, double look_y, double look_z,
                              double up_x, double up_y, double up_z) {
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

absl::Status ParserV1::MakeNamedMaterial(
    absl::string_view material_name,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  return absl::UnimplementedError(
      "Directive 'MakeNamedMaterial' is not supported in pbrt-v1");
}

absl::Status ParserV1::MakeNamedMedium(
    absl::string_view medium_name,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  return absl::UnimplementedError(
      "Directive 'MakeNamedMedium' is not supported in pbrt-v1");
}

absl::Status ParserV1::Material(
    absl::string_view material_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& material = *output_.add_directives()->mutable_material();

  if (material_type == "bluepaint") {
    material.mutable_bluepaint();
  } else if (material_type == "brushedmetal") {
    material.mutable_brushedmetal();
  } else if (material_type == "clay") {
    material.mutable_clay();
  } else if (material_type == "felt") {
    material.mutable_felt();
  } else if (material_type == "glass") {
    RemoveGlassMaterialV1(parameters, *material.mutable_glass());
  } else if (material_type == "matte") {
    RemoveMatteMaterialV1(parameters, *material.mutable_matte());
  } else if (material_type == "mirror") {
    RemoveMirrorMaterialV1(parameters, *material.mutable_mirror());
  } else if (material_type == "plastic") {
    RemovePlasticMaterialV1(parameters, *material.mutable_plastic());
  } else if (material_type == "primer") {
    material.mutable_primer();
  } else if (material_type == "shinymetal") {
    RemoveShinyMetalMaterialV1(parameters, *material.mutable_shinymetal());
  } else if (material_type == "skin") {
    material.mutable_skin();
  } else if (material_type == "substrate") {
    RemoveSubstrateMaterialV1(parameters, *material.mutable_substrate());
  } else if (material_type == "translucent") {
    RemoveTranslucentMaterialV1(parameters, *material.mutable_translucent());
  } else if (material_type == "uber") {
    RemoveUberMaterialV1(parameters, *material.mutable_uber());
  } else {
    if (material_type != "matte") {
      std::cerr << "Unrecognized Material type: \"" << material_type << "\""
                << std::endl;
    }

    RemoveMatteMaterialV1(parameters, *material.mutable_matte());
  }

  return absl::OkStatus();
}

absl::Status ParserV1::MediumInterface(absl::string_view inside,
                                       absl::string_view outside) {
  return absl::UnimplementedError(
      "Directive 'MaterialInterface' is not supported in pbrt-v1");
}

absl::Status ParserV1::NamedMaterial(absl::string_view name) {
  return absl::UnimplementedError(
      "Directive 'NamedMaterial' is not supported in pbrt-v1");
}

absl::Status ParserV1::ObjectBegin(absl::string_view name) {
  output_.add_directives()->mutable_object_begin()->set_name(name);
  return absl::OkStatus();
}

absl::Status ParserV1::ObjectEnd() {
  output_.add_directives()->mutable_object_end();
  return absl::OkStatus();
}

absl::Status ParserV1::ObjectInstance(absl::string_view name) {
  output_.add_directives()->mutable_object_instance()->set_name(name);
  return absl::OkStatus();
}

absl::Status ParserV1::PixelFilter(
    absl::string_view pixel_filter_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& pixel_filter = *output_.add_directives()->mutable_pixel_filter();

  if (pixel_filter_type == "box") {
    RemoveBoxPixelFilterV1(parameters, *pixel_filter.mutable_box());
  } else if (pixel_filter_type == "gaussian") {
    RemoveGaussianPixelFilterV1(parameters, *pixel_filter.mutable_gaussian());
  } else if (pixel_filter_type == "mitchell") {
    RemoveMitchellPixelFilterV1(parameters, *pixel_filter.mutable_mitchell());
  } else if (pixel_filter_type == "sinc") {
    RemoveLanczosPixelFilterV1(parameters, *pixel_filter.mutable_sinc());
  } else if (pixel_filter_type == "triangle") {
    RemoveTrianglePixelFilterV1(parameters, *pixel_filter.mutable_triangle());
  } else {
    std::cerr << "Unrecognized PixelFilter type: \"" << pixel_filter_type
              << "\"" << std::endl;
  }

  return absl::OkStatus();
}

absl::Status ParserV1::ReverseOrientation() {
  output_.add_directives()->mutable_reverse_orientation();
  return absl::OkStatus();
}

absl::Status ParserV1::Rotate(double angle, double x, double y, double z) {
  auto& rotate = *output_.add_directives()->mutable_rotate();
  rotate.set_angle(angle);
  rotate.set_x(x);
  rotate.set_y(y);
  rotate.set_z(z);
  return absl::OkStatus();
}

absl::Status ParserV1::Sampler(
    absl::string_view sampler_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& sampler = *output_.add_directives()->mutable_sampler();

  if (sampler_type == "bestcandidate") {
    RemoveBestCandidateSamplerV1(parameters, *sampler.mutable_bestcandidate());
  } else if (sampler_type == "lowdiscrepancy") {
    RemoveZeroTwoSequenceSamplerV1(parameters,
                                   *sampler.mutable_lowdiscrepancy());
  } else if (sampler_type == "random") {
    RemoveRandomSamplerV1(parameters, *sampler.mutable_random());
  } else if (sampler_type == "stratified") {
    RemoveStratifiedSamplerV1(parameters, *sampler.mutable_stratified());
  } else {
    std::cerr << "Unrecognized Sampler type: \"" << sampler_type << "\""
              << std::endl;
  }

  return absl::OkStatus();
}

absl::Status ParserV1::Scale(double x, double y, double z) {
  auto& scale = *output_.add_directives()->mutable_scale();
  scale.set_x(x);
  scale.set_y(y);
  scale.set_z(z);
  return absl::OkStatus();
}

absl::Status ParserV1::SearchPath(absl::string_view path) {
  output_.add_directives()->mutable_search_path()->set_path(path);
  return absl::OkStatus();
}

absl::Status ParserV1::Shape(
    absl::string_view shape_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& shape = *output_.add_directives()->mutable_shape();

  if (shape_type == "cone") {
    RemoveConeShapeV1(parameters, *shape.mutable_cone());
  } else if (shape_type == "cylinder") {
    RemoveCylinderShapeV1(parameters, *shape.mutable_cylinder());
  } else if (shape_type == "disk") {
    RemoveDiskShapeV1(parameters, *shape.mutable_disk());
  } else if (shape_type == "heightfield") {
    if (absl::Status status =
            RemoveHeightFieldShapeV1(parameters, *shape.mutable_heightfield());
        !status.ok()) {
      return status;
    }
  } else if (shape_type == "hyperboloid") {
    RemoveHyperboloidShapeV1(parameters, *shape.mutable_hyperboloid());
  } else if (shape_type == "loopsubdiv") {
    if (absl::Status status =
            RemoveLoopSubdivShapeV1(parameters, *shape.mutable_loopsubdiv());
        !status.ok()) {
      return status;
    }
  } else if (shape_type == "nurbs") {
    if (absl::Status status =
            RemoveNurbsShapeV1(parameters, *shape.mutable_nurbs());
        !status.ok()) {
      return status;
    }
  } else if (shape_type == "paraboloid") {
    RemoveParaboloidShapeV1(parameters, *shape.mutable_paraboloid());
  } else if (shape_type == "sphere") {
    RemoveSphereShapeV1(parameters, *shape.mutable_sphere());
  } else if (shape_type == "trianglemesh") {
    if (absl::Status status = RemoveTriangleMeshShapeV1(
            parameters, *shape.mutable_trianglemesh());
        !status.ok()) {
      return status;
    }
  } else {
    std::cerr << "Unrecognized Shape type: \"" << shape_type << "\""
              << std::endl;
    return absl::OkStatus();
  }

  auto& overrides = *shape.mutable_overrides();

  bool overrides_populated = false;
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "bumpmap",
      std::bind(&Shape::MaterialOverrides::mutable_bumpmap, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "eta",
      std::bind(&Shape::MaterialOverrides::mutable_eta, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "roughness",
      std::bind(&Shape::MaterialOverrides::mutable_roughness, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "sigma",
      std::bind(&Shape::MaterialOverrides::mutable_sigma, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "uroughness",
      std::bind(&Shape::MaterialOverrides::mutable_uroughness, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "vroughness",
      std::bind(&Shape::MaterialOverrides::mutable_vroughness, &overrides));

  overrides_populated |= TryRemoveSpectrumTextureV1(
      parameters, "Kd",
      std::bind(&Shape::MaterialOverrides::mutable_kd, &overrides));
  overrides_populated |= TryRemoveSpectrumTextureV1(
      parameters, "Kr",
      std::bind(&Shape::MaterialOverrides::mutable_kr, &overrides));
  overrides_populated |= TryRemoveSpectrumTextureV1(
      parameters, "Ks",
      std::bind(&Shape::MaterialOverrides::mutable_ks, &overrides));
  overrides_populated |= TryRemoveSpectrumTextureV1(
      parameters, "Kt",
      std::bind(&Shape::MaterialOverrides::mutable_kt, &overrides));
  overrides_populated |= TryRemoveSpectrumTextureV1(
      parameters, "opacity",
      std::bind(&Shape::MaterialOverrides::mutable_opacity, &overrides));
  overrides_populated |= TryRemoveSpectrumTextureV1(
      parameters, "reflect",
      std::bind(&Shape::MaterialOverrides::mutable_reflect, &overrides));
  overrides_populated |= TryRemoveSpectrumTextureV1(
      parameters, "transmit",
      std::bind(&Shape::MaterialOverrides::mutable_transmit, &overrides));

  if (!overrides_populated) {
    shape.clear_overrides();
  }

  return absl::OkStatus();
}

absl::Status ParserV1::SpectrumTexture(
    absl::string_view spectrum_texture_name,
    absl::string_view spectrum_texture_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& spectrum_texture =
      *output_.add_directives()->mutable_spectrum_texture();

  spectrum_texture.set_name(spectrum_texture_name);

  if (spectrum_texture_type == "bilerp") {
    RemoveBilerpSpectrumTextureV1(parameters,
                                  *spectrum_texture.mutable_bilerp());
  } else if (spectrum_texture_type == "checkerboard") {
    int32_t dimension = TryRemoveInteger(parameters, "dimension").value_or(2);
    if (dimension == 2) {
      RemoveCheckerboard2DSpectrumTextureV1(
          parameters, *spectrum_texture.mutable_checkerboard2d());
    } else {
      RemoveCheckerboard3DSpectrumTextureV1(
          parameters, *spectrum_texture.mutable_checkerboard3d());
    }
  } else if (spectrum_texture_type == "constant") {
    return RemoveConstantSpectrumTextureV1(
        parameters, *spectrum_texture.mutable_constant());
  } else if (spectrum_texture_type == "dots") {
    RemoveDotsSpectrumTextureV1(parameters, *spectrum_texture.mutable_dots());
  } else if (spectrum_texture_type == "fbm") {
    RemoveFBmSpectrumTextureV1(parameters, *spectrum_texture.mutable_fbm());
  } else if (spectrum_texture_type == "imagemap") {
    RemoveImageMapSpectrumTextureV1(parameters,
                                    *spectrum_texture.mutable_imagemap());
  } else if (spectrum_texture_type == "marble") {
    RemoveMarbleSpectrumTextureV1(parameters,
                                  *spectrum_texture.mutable_marble());
  } else if (spectrum_texture_type == "mix") {
    RemoveMixSpectrumTextureV1(parameters, *spectrum_texture.mutable_mix());
  } else if (spectrum_texture_type == "scale") {
    RemoveScaleSpectrumTextureV1(parameters, *spectrum_texture.mutable_scale());
  } else if (spectrum_texture_type == "uv") {
    RemoveUvSpectrumTextureV1(parameters, *spectrum_texture.mutable_uv());
  } else if (spectrum_texture_type == "windy") {
    spectrum_texture.mutable_windy();
  } else if (spectrum_texture_type == "wrinkled") {
    RemoveWrinkledSpectrumTextureV1(parameters,
                                    *spectrum_texture.mutable_wrinkled());
  } else {
    std::cerr << "Unrecognized Texture type: \"" << spectrum_texture_type
              << "\"" << std::endl;
  }

  return absl::OkStatus();
}

absl::Status ParserV1::SurfaceIntegrator(
    absl::string_view integrator_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& integrator = *output_.add_directives()->mutable_surface_integrator();

  if (integrator_type == "bidirectional") {
    RemoveBdptIntegratorV1(parameters, *integrator.mutable_bidirectional());
  } else if (integrator_type == "debug") {
    RemoveDebugIntegratorV1(parameters, *integrator.mutable_debug());
  }
  if (integrator_type == "directlighting") {
    RemoveDirectLightingIntegratorV1(parameters,
                                     *integrator.mutable_directlighting());
  } else if (integrator_type == "exphotonmap") {
    RemoveExPhotonMapIntegratorV1(parameters,
                                  *integrator.mutable_exphotonmap());
  } else if (integrator_type == "igi") {
    RemoveIgiIntegratorV1(parameters, *integrator.mutable_igi());
  } else if (integrator_type == "irradiancecache") {
    RemoveIrradianceCacheIntegratorV1(parameters,
                                      *integrator.mutable_irradiancecache());
  } else if (integrator_type == "path") {
    RemovePathIntegratorV1(parameters, *integrator.mutable_path());
  } else if (integrator_type == "photonmap") {
    RemovePhotonMapIntegratorV1(parameters, *integrator.mutable_photonmap());
  } else if (integrator_type == "whitted") {
    RemoveWhittedIntegratorV1(parameters, *integrator.mutable_whitted());
  } else {
    std::cerr << "Unrecognized SurfaceIntegrator type: \"" << integrator_type
              << "\"" << std::endl;
  }

  return absl::OkStatus();
}

absl::Status ParserV1::Transform(double m00, double m01, double m02, double m03,
                                 double m10, double m11, double m12, double m13,
                                 double m20, double m21, double m22, double m23,
                                 double m30, double m31, double m32,
                                 double m33) {
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

absl::Status ParserV1::TransformTimes(double start_time, double end_time) {
  return absl::UnimplementedError(
      "Directive 'TransformTimes' is not supported in pbrt-v1");
}

absl::Status ParserV1::TransformBegin() {
  output_.add_directives()->mutable_transform_begin();
  return absl::OkStatus();
}

absl::Status ParserV1::TransformEnd() {
  output_.add_directives()->mutable_transform_end();
  return absl::OkStatus();
}

absl::Status ParserV1::Translate(double x, double y, double z) {
  auto& translate = *output_.add_directives()->mutable_translate();
  translate.set_x(x);
  translate.set_y(y);
  translate.set_z(z);
  return absl::OkStatus();
}

absl::Status ParserV1::Volume(
    absl::string_view volume_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& volume = *output_.add_directives()->mutable_volume();

  if (volume_type == "exponential") {
    return RemoveExponentialMediumV1(parameters, *volume.mutable_exponential());
  } else if (volume_type == "homogeneous") {
    return RemoveHomogeneousMediumV1(parameters, *volume.mutable_homogeneous());
  } else if (volume_type == "volumegrid") {
    return RemoveUniformGridMediumV1(parameters, *volume.mutable_volumegrid());
  } else {
    std::cerr << "Unrecognized Volume type: \"" << volume_type << "\""
              << std::endl;
  }

  return absl::OkStatus();
}

absl::Status ParserV1::VolumeIntegrator(
    absl::string_view integrator_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& integrator = *output_.add_directives()->mutable_volume_integrator();

  if (integrator_type == "emission") {
    RemoveEmissionVolumeIntegratorV1(parameters,
                                     *integrator.mutable_emission());
  } else if (integrator_type == "single") {
    RemoveSingleVolumeIntegratorV1(parameters, *integrator.mutable_single());
  } else {
    std::cerr << "Unrecognized VolumeIntegrator type: \"" << integrator_type
              << "\"" << std::endl;
  }

  return absl::OkStatus();
}

absl::Status ParserV1::WorldBegin() {
  output_.add_directives()->mutable_world_begin();
  return absl::OkStatus();
}

absl::Status ParserV1::WorldEnd() {
  output_.add_directives()->mutable_world_end();
  return absl::OkStatus();
}

}  // namespace

absl::Status Convert(std::istream& input, PbrtProto& output) {
  return ParserV1(output).ReadFrom(input);
}

absl::StatusOr<PbrtProto> Convert(std::istream& input) {
  PbrtProto output;
  if (absl::Status error = Convert(input, output); !error.ok()) {
    return error;
  }
  return output;
}

}  // namespace pbrt_proto::v1
