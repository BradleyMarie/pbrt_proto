#include "pbrt_proto/v3/convert.h"

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
#include "pbrt_proto/v3/v3.pb.h"

namespace pbrt_proto::v3 {
namespace {

Material ParseMaterial(
    absl::string_view material_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  Material material;

  if (material_type == "" || material_type == "none") {
    // Do Nothing
  } else if (material_type == "disney") {
    RemoveDisneyMaterial(parameters, *material.mutable_disney());
  } else if (material_type == "fourier") {
    RemoveMeasuredFourierMaterial(parameters, *material.mutable_fourier());
  } else if (material_type == "glass") {
    RemoveGlassMaterialV2(parameters, *material.mutable_glass());
  } else if (material_type == "hair") {
    RemoveHairMaterial(parameters, *material.mutable_hair());
  } else if (material_type == "kdsubsurface") {
    RemoveKdSubsurfaceMaterialV2(parameters, *material.mutable_kdsubsurface());
  } else if (material_type == "metal") {
    RemoveMetalMaterialV2(parameters, *material.mutable_metal());
  } else if (material_type == "mirror") {
    RemoveMirrorMaterial(parameters, *material.mutable_mirror());
  } else if (material_type == "mix") {
    RemoveMixMaterial(parameters, *material.mutable_mix());
  } else if (material_type == "plastic") {
    RemovePlasticMaterialV2(parameters, *material.mutable_plastic());
  } else if (material_type == "substrate") {
    RemoveSubstrateMaterialV2(parameters, *material.mutable_substrate());
  } else if (material_type == "subsurface") {
    RemoveSubsurfaceMaterial(parameters, *material.mutable_subsurface());
  } else if (material_type == "translucent") {
    RemoveTranslucentMaterialV2(parameters, *material.mutable_translucent());
  } else if (material_type == "uber") {
    RemoveUberMaterialV3(parameters, *material.mutable_uber());
  } else {
    if (material_type != "matte") {
      std::cerr << "Unrecognized Material type: \"" << material_type << "\""
                << std::endl;
    }

    RemoveMatteMaterial(parameters, *material.mutable_matte());
  }

  return material;
}

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
        {"texture", ParameterType::TEXTURE},
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

  absl::Status Shape(
      absl::string_view shape_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status SpectrumTexture(
      absl::string_view spectrum_texture_name,
      absl::string_view spectrum_texture_type,
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

  absl::Status WorldBegin() override;

  absl::Status WorldEnd() override;

  PbrtProto& output_;
};

absl::Status ParserV3::Accelerator(
    absl::string_view accelerator_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& accelerator = *output_.add_directives()->mutable_accelerator();

  if (accelerator_type == "bvh") {
    RemoveBvhAcceleratorV3(parameters, *accelerator.mutable_bvh());
  } else if (accelerator_type == "kdtree") {
    RemoveKdTreeAcceleratorV1(parameters, *accelerator.mutable_kdtree());
  } else {
    std::cerr << "Unrecognized Accelerator type: \"" << accelerator_type << "\""
              << std::endl;
  }

  return absl::OkStatus();
}

absl::Status ParserV3::ActiveTransform(ActiveTransformation active) {
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

absl::Status ParserV3::AreaLightSource(
    absl::string_view area_light_source_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& area_light_source =
      *output_.add_directives()->mutable_area_light_source();

  if (area_light_source_type == "area" || area_light_source_type == "diffuse") {
    RemoveDiffuseAreaLightSourceV3(parameters,
                                   *area_light_source.mutable_diffuse());
  } else {
    std::cerr << "Unrecognized AreaLightSource type: \""
              << area_light_source_type << "\"" << std::endl;
  }

  return absl::OkStatus();
}

absl::Status ParserV3::AttributeBegin() {
  output_.add_directives()->mutable_attribute_begin();
  return absl::OkStatus();
}

absl::Status ParserV3::AttributeEnd() {
  output_.add_directives()->mutable_attribute_end();
  return absl::OkStatus();
}

absl::Status ParserV3::Camera(
    absl::string_view camera_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& camera = *output_.add_directives()->mutable_camera();

  if (camera_type == "environment") {
    RemoveSphericalCameraV2(parameters, *camera.mutable_environment());
  } else if (camera_type == "orthographic") {
    RemoveOrthographicCameraV2(parameters, *camera.mutable_orthographic());
  } else if (camera_type == "perspective") {
    RemovePerspectiveCameraV2(parameters, *camera.mutable_perspective());
  } else if (camera_type == "realistic") {
    RemoveRealisticCameraV1(parameters, *camera.mutable_realistic());
  } else {
    std::cerr << "Unrecognized Camera type: \"" << camera_type << "\""
              << std::endl;
  }

  return absl::OkStatus();
}

absl::Status ParserV3::ConcatTransform(double m00, double m01, double m02,
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

absl::Status ParserV3::CoordinateSystem(absl::string_view name) {
  output_.add_directives()->mutable_coordinate_system()->set_name(name);
  return absl::OkStatus();
}

absl::Status ParserV3::CoordSysTransform(absl::string_view name) {
  output_.add_directives()->mutable_coord_sys_transform()->set_name(name);
  return absl::OkStatus();
}

absl::Status ParserV3::Film(
    absl::string_view film_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& film = *output_.add_directives()->mutable_film();

  if (film_type == "image") {
    RemoveRgbFilmV3(parameters, *film.mutable_image());
  } else {
    std::cerr << "Unrecognized Film type: \"" << film_type << "\"" << std::endl;
  }

  return absl::OkStatus();
}

absl::Status ParserV3::FloatTexture(
    absl::string_view float_texture_name, absl::string_view float_texture_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& float_texture = *output_.add_directives()->mutable_float_texture();

  float_texture.set_name(float_texture_name);

  if (float_texture_type == "bilerp") {
    RemoveBilerpFloatTextureV1(parameters, *float_texture.mutable_bilerp());
  } else if (float_texture_type == "checkerboard") {
    int32_t dimensions = TryRemoveInteger(parameters, "dimensions").value_or(2);
    if (dimensions == 2) {
      RemoveCheckerboard2DFloatTextureV2(
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
    RemoveImageMapFloatTextureV3(parameters, *float_texture.mutable_imagemap());
  } else if (float_texture_type == "marble") {
    // Ignored
  } else if (float_texture_type == "mix") {
    RemoveMixFloatTextureV1(parameters, *float_texture.mutable_mix());
  } else if (float_texture_type == "ptex") {
    RemovePtexFloatTextureV1(parameters, *float_texture.mutable_ptex());
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

absl::Status ParserV3::Identity() {
  output_.add_directives()->mutable_identity();
  return absl::OkStatus();
}

absl::Status ParserV3::Include(absl::string_view path) {
  output_.add_directives()->mutable_include()->set_path(path);
  return absl::OkStatus();
}

absl::Status ParserV3::Integrator(
    absl::string_view integrator_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& integrator = *output_.add_directives()->mutable_integrator();

  if (integrator_type == "ambientocclusion") {
    RemoveAmbientOcclusionIntegratorV2(parameters,
                                       *integrator.mutable_ambientocclusion());
  } else if (integrator_type == "bdpt") {
    RemoveBdptIntegratorV2(parameters, *integrator.mutable_bdpt());
  } else if (integrator_type == "directlighting") {
    RemoveDirectLightingIntegratorV3(parameters,
                                     *integrator.mutable_directlighting());
  } else if (integrator_type == "mlt") {
    RemoveMltIntegratorV1(parameters, *integrator.mutable_mlt());
  } else if (integrator_type == "path") {
    RemovePathIntegratorV2(parameters, *integrator.mutable_path());
  } else if (integrator_type == "sppm") {
    RemoveSppmIntegratorV1(parameters, *integrator.mutable_sppm());
  } else if (integrator_type == "volpath") {
    RemoveVolPathIntegratorV1(parameters, *integrator.mutable_volpath());
  } else if (integrator_type == "whitted") {
    RemoveWhittedIntegratorV2(parameters, *integrator.mutable_whitted());
  } else {
    std::cerr << "Unrecognized Integrator type: \"" << integrator_type << "\""
              << std::endl;
  }

  return absl::OkStatus();
}

absl::Status ParserV3::Import(absl::string_view path) {
  return absl::UnimplementedError(
      "Directive 'Import' is not supported in pbrt-v3");
}

absl::Status ParserV3::LightSource(
    absl::string_view light_source_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& light_source = *output_.add_directives()->mutable_light_source();

  if (light_source_type == "distant") {
    RemoveDistantLightSourceV2(parameters, *light_source.mutable_distant());
  } else if (light_source_type == "goniometric") {
    RemoveGoniometricLightSourceV2(parameters,
                                   *light_source.mutable_goniometric());
  } else if (light_source_type == "infinite") {
    RemoveInfiniteLightSourceV3(parameters, *light_source.mutable_infinite());
  } else if (light_source_type == "point") {
    RemovePointLightSourceV2(parameters, *light_source.mutable_point());
  } else if (light_source_type == "projection") {
    RemoveProjectionLightSourceV2(parameters,
                                  *light_source.mutable_projection());
  } else if (light_source_type == "spot") {
    RemoveSpotLightSourceV2(parameters, *light_source.mutable_spot());
  } else {
    std::cerr << "Unrecognized LightSource type: \"" << light_source_type
              << "\"" << std::endl;
  }

  return absl::OkStatus();
}

absl::Status ParserV3::LookAt(double eye_x, double eye_y, double eye_z,
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

absl::Status ParserV3::MakeNamedMaterial(
    absl::string_view material_name,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& make_named_material =
      *output_.add_directives()->mutable_make_named_material();

  make_named_material.set_name(material_name);
  *make_named_material.mutable_material() = ParseMaterial(
      TryRemoveString(parameters, "type").value_or(""), parameters);

  return absl::OkStatus();
}

absl::Status ParserV3::MakeNamedMedium(
    absl::string_view medium_name,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& make_named_medium =
      *output_.add_directives()->mutable_make_named_medium();

  make_named_medium.set_name(medium_name);

  absl::string_view medium_type =
      TryRemoveString(parameters, "type").value_or("");
  if (medium_type == "homogeneous") {
    RemoveHomogeneousMediumV3(parameters,
                              *make_named_medium.mutable_homogeneous());
  } else if (medium_type == "heterogeneous") {
    if (absl::Status status = RemoveUniformGridMediumV3(
            parameters, *make_named_medium.mutable_heterogeneous());
        !status.ok()) {
      return status;
    }
  } else {
    std::cerr << "Unrecognized MakeNamedMedium type: \"" << medium_type << "\""
              << std::endl;
  }

  return absl::OkStatus();
}

absl::Status ParserV3::Material(
    absl::string_view material_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  *output_.add_directives()->mutable_material() =
      ParseMaterial(material_type, parameters);
  return absl::OkStatus();
}

absl::Status ParserV3::MediumInterface(absl::string_view inside,
                                       absl::string_view outside) {
  auto& material_interface =
      *output_.add_directives()->mutable_medium_interface();
  material_interface.set_inside(inside);
  material_interface.set_outside(outside);
  return absl::OkStatus();
}

absl::Status ParserV3::NamedMaterial(absl::string_view name) {
  output_.add_directives()->mutable_named_material()->set_name(name);
  return absl::OkStatus();
}

absl::Status ParserV3::ObjectBegin(absl::string_view name) {
  output_.add_directives()->mutable_object_begin()->set_name(name);
  return absl::OkStatus();
}

absl::Status ParserV3::ObjectEnd() {
  output_.add_directives()->mutable_object_end();
  return absl::OkStatus();
}

absl::Status ParserV3::ObjectInstance(absl::string_view name) {
  output_.add_directives()->mutable_object_instance()->set_name(name);
  return absl::OkStatus();
}

absl::Status ParserV3::PixelFilter(
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

absl::Status ParserV3::ReverseOrientation() {
  output_.add_directives()->mutable_reverse_orientation();
  return absl::OkStatus();
}

absl::Status ParserV3::Rotate(double angle, double x, double y, double z) {
  auto& rotate = *output_.add_directives()->mutable_rotate();
  rotate.set_angle(angle);
  rotate.set_x(x);
  rotate.set_y(y);
  rotate.set_z(z);
  return absl::OkStatus();
}

absl::Status ParserV3::Scale(double x, double y, double z) {
  auto& scale = *output_.add_directives()->mutable_scale();
  scale.set_x(x);
  scale.set_y(y);
  scale.set_z(z);
  return absl::OkStatus();
}

absl::Status ParserV3::Sampler(
    absl::string_view sampler_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& sampler = *output_.add_directives()->mutable_sampler();

  if (sampler_type == "halton") {
    RemoveHaltonSamplerV3(parameters, *sampler.mutable_halton());
  } else if (sampler_type == "maxmindist") {
    RemoveMaxMinDistSamplerV3(parameters, *sampler.mutable_maxmindist());
  } else if (sampler_type == "random") {
    RemoveIndependentSamplerV2(parameters, *sampler.mutable_random());
  } else if (sampler_type == "sobol") {
    RemoveSobolSamplerV3(parameters, *sampler.mutable_sobol());
  } else if (sampler_type == "stratified") {
    RemoveStratifiedSamplerV3(parameters, *sampler.mutable_stratified());
  } else if (sampler_type == "02sequence" || sampler_type == "lowdiscrepancy") {
    RemoveZeroTwoSequenceSamplerV3(parameters,
                                   *sampler.mutable_zerotwosequence());
  } else {
    std::cerr << "Unrecognized Sampler type: \"" << sampler_type << "\""
              << std::endl;
  }

  return absl::OkStatus();
}

absl::Status ParserV3::Shape(
    absl::string_view shape_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& shape = *output_.add_directives()->mutable_shape();

  if (shape_type == "cone") {
    RemoveConeShapeV1(parameters, *shape.mutable_cone());
  } else if (shape_type == "curve") {
    if (absl::Status status = TryRemoveCurveShapeV3(
            parameters, std::bind(&Shape::mutable_curve, &shape));
        !status.ok()) {
      return status;
    }
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
            RemoveLoopSubdivShapeV3(parameters, *shape.mutable_loopsubdiv());
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
  } else if (shape_type == "plymesh") {
    RemovePlyMeshShapeV3(parameters, *shape.mutable_plymesh());
  } else if (shape_type == "sphere") {
    RemoveSphereShapeV1(parameters, *shape.mutable_sphere());
  } else if (shape_type == "trianglemesh") {
    if (absl::Status status = RemoveTriangleMeshShapeV3(
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
  if (std::optional<bool> remaproughness =
          TryRemoveBool(parameters, "remaproughness");
      remaproughness.has_value()) {
    overrides.set_remaproughness(*remaproughness);
    overrides_populated = true;
  }

  if (std::optional<bool> thin = TryRemoveBool(parameters, "thin");
      thin.has_value()) {
    overrides.set_thin(*thin);
    overrides_populated = true;
  }

  if (std::optional<double> g = TryRemoveFloat(parameters, "g");
      g.has_value()) {
    overrides.set_g(*g);
    overrides_populated = true;
  }

  if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
      scale.has_value()) {
    overrides.set_scale(*scale);
    overrides_populated = true;
  }

  overrides_populated |= TryRemoveFloatTexture(
      parameters, "alpha",
      std::bind(&Shape::MaterialOverrides::mutable_alpha, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "anisotropic",
      std::bind(&Shape::MaterialOverrides::mutable_anisotropic, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "beta_m",
      std::bind(&Shape::MaterialOverrides::mutable_beta_m, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "beta_n",
      std::bind(&Shape::MaterialOverrides::mutable_beta_n, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "bumpmap",
      std::bind(&Shape::MaterialOverrides::mutable_bumpmap, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "clearcoat",
      std::bind(&Shape::MaterialOverrides::mutable_clearcoat, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "clearcoatgloss",
      std::bind(&Shape::MaterialOverrides::mutable_clearcoatgloss, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "difftrans",
      std::bind(&Shape::MaterialOverrides::mutable_difftrans, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "eumelanin",
      std::bind(&Shape::MaterialOverrides::mutable_eumelanin, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "flatness",
      std::bind(&Shape::MaterialOverrides::mutable_flatness, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "metallic",
      std::bind(&Shape::MaterialOverrides::mutable_metallic, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "pheomelanin",
      std::bind(&Shape::MaterialOverrides::mutable_pheomelanin, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "roughness",
      std::bind(&Shape::MaterialOverrides::mutable_roughness, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "sheen",
      std::bind(&Shape::MaterialOverrides::mutable_sheen, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "sheentint",
      std::bind(&Shape::MaterialOverrides::mutable_sheentint, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "sigma",
      std::bind(&Shape::MaterialOverrides::mutable_sigma, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "spectrans",
      std::bind(&Shape::MaterialOverrides::mutable_spectrans, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "speculartint",
      std::bind(&Shape::MaterialOverrides::mutable_speculartint, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "uroughness",
      std::bind(&Shape::MaterialOverrides::mutable_uroughness, &overrides));
  overrides_populated |= TryRemoveFloatTexture(
      parameters, "vroughness",
      std::bind(&Shape::MaterialOverrides::mutable_vroughness, &overrides));

  if (std::optional<absl::string_view> name =
          TryRemoveString(parameters, "name");
      name.has_value()) {
    auto iter = kNamedMeasuredScatteringPresets.find(*name);
    if (iter != kNamedMeasuredScatteringPresets.end()) {
      overrides.set_name(iter->second);
      overrides_populated = true;
    }
  }

  overrides_populated |= TryRemoveSpectrumTextureV1(
      parameters, "amount",
      std::bind(&Shape::MaterialOverrides::mutable_amount, &overrides));
  overrides_populated |= TryRemoveSpectrumTextureV1(
      parameters, "color",
      std::bind(&Shape::MaterialOverrides::mutable_color, &overrides));
  overrides_populated |= TryRemoveSpectrumTextureV1(
      parameters, "k",
      std::bind(&Shape::MaterialOverrides::mutable_k, &overrides));
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
      parameters, "mfp",
      std::bind(&Shape::MaterialOverrides::mutable_mfp, &overrides));
  overrides_populated |= TryRemoveSpectrumTextureV1(
      parameters, "opacity",
      std::bind(&Shape::MaterialOverrides::mutable_opacity, &overrides));
  overrides_populated |= TryRemoveSpectrumTextureV1(
      parameters, "reflect",
      std::bind(&Shape::MaterialOverrides::mutable_reflect, &overrides));
  overrides_populated |= TryRemoveSpectrumTextureV1(
      parameters, "scatterdistance",
      std::bind(&Shape::MaterialOverrides::mutable_scatterdistance,
                &overrides));
  overrides_populated |= TryRemoveSpectrumTextureV1(
      parameters, "sigma_a",
      std::bind(&Shape::MaterialOverrides::mutable_sigma_a, &overrides));
  overrides_populated |= TryRemoveSpectrumTextureV1(
      parameters, "sigma_s",
      std::bind(&Shape::MaterialOverrides::mutable_sigma_s, &overrides));
  overrides_populated |= TryRemoveSpectrumTextureV1(
      parameters, "transmit",
      std::bind(&Shape::MaterialOverrides::mutable_transmit, &overrides));

  if (std::optional<absl::string_view> bsdffile =
          TryRemoveString(parameters, "bsdffile");
      bsdffile.has_value()) {
    overrides.set_filename(*bsdffile);
    overrides_populated = true;
  }

  if (std::optional<absl::string_view> namedmaterial1 =
          TryRemoveString(parameters, "namedmaterial1");
      namedmaterial1.has_value()) {
    overrides.set_namedmaterial1(*namedmaterial1);
    overrides_populated = true;
  }

  if (std::optional<absl::string_view> namedmaterial2 =
          TryRemoveString(parameters, "namedmaterial2");
      namedmaterial2.has_value()) {
    overrides.set_namedmaterial2(*namedmaterial2);
    overrides_populated = true;
  }

  if (std::optional<double> eta = TryRemoveFloat(parameters, "eta");
      eta.has_value()) {
    overrides.set_eta_as_value(*eta);
    overrides.mutable_eta()->set_float_value(*eta);
    overrides_populated = true;
  } else if (std::optional<absl::string_view> eta =
                 TryRemoveTexture(parameters, "eta");
             eta.has_value()) {
    overrides.mutable_eta()->set_float_texture_name(*eta);
    overrides.mutable_eta_as_spectrum_texture()->set_spectrum_texture_name(
        *eta);
    overrides_populated = true;
  } else {
    overrides_populated |= TryRemoveSpectrumTextureV1(
        parameters, "eta",
        std::bind(&Shape::MaterialOverrides::mutable_eta_as_spectrum_texture,
                  &overrides));
  }

  if (!overrides_populated) {
    shape.clear_overrides();
  }

  return absl::OkStatus();
}

absl::Status ParserV3::SpectrumTexture(
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
    int32_t dimensions = TryRemoveInteger(parameters, "dimensions").value_or(2);
    if (dimensions == 2) {
      RemoveCheckerboard2DSpectrumTextureV2(
          parameters, *spectrum_texture.mutable_checkerboard2d());
    } else {
      RemoveCheckerboard3DSpectrumTextureV1(
          parameters, *spectrum_texture.mutable_checkerboard3d());
    }
  } else if (spectrum_texture_type == "constant") {
    RemoveConstantSpectrumTextureV1(parameters,
                                    *spectrum_texture.mutable_constant());
  } else if (spectrum_texture_type == "dots") {
    RemoveDotsSpectrumTextureV1(parameters, *spectrum_texture.mutable_dots());
  } else if (spectrum_texture_type == "fbm") {
    RemoveFBmSpectrumTextureV1(parameters, *spectrum_texture.mutable_fbm());
  } else if (spectrum_texture_type == "imagemap") {
    RemoveImageMapSpectrumTextureV3(parameters,
                                    *spectrum_texture.mutable_imagemap());
  } else if (spectrum_texture_type == "marble") {
    RemoveMarbleSpectrumTextureV1(parameters,
                                  *spectrum_texture.mutable_marble());
  } else if (spectrum_texture_type == "mix") {
    RemoveMixSpectrumTextureV1(parameters, *spectrum_texture.mutable_mix());
  } else if (spectrum_texture_type == "ptex") {
    RemovePtexSpectrumTextureV1(parameters, *spectrum_texture.mutable_ptex());
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

absl::Status ParserV3::Transform(double m00, double m01, double m02, double m03,
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

absl::Status ParserV3::TransformTimes(double start_time, double end_time) {
  auto& transform_times = *output_.add_directives()->mutable_transform_times();
  transform_times.set_start_time(start_time);
  transform_times.set_end_time(end_time);
  return absl::OkStatus();
}

absl::Status ParserV3::TransformBegin() {
  output_.add_directives()->mutable_transform_begin();
  return absl::OkStatus();
}

absl::Status ParserV3::TransformEnd() {
  output_.add_directives()->mutable_transform_end();
  return absl::OkStatus();
}

absl::Status ParserV3::Translate(double x, double y, double z) {
  auto& translate = *output_.add_directives()->mutable_translate();
  translate.set_x(x);
  translate.set_y(y);
  translate.set_z(z);
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

absl::Status Convert(std::istream& input, PbrtProto& output) {
  return ParserV3(output).ReadFrom(input);
}

absl::StatusOr<PbrtProto> Convert(std::istream& input) {
  PbrtProto output;
  if (absl::Status error = Convert(input, output); !error.ok()) {
    return error;
  }
  return output;
}

}  // namespace pbrt_proto::v3
