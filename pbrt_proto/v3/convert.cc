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
#include "pbrt_proto/shared/proto_parser.h"
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
    RemoveDisneyMaterialV3(parameters, *material.mutable_disney());
  } else if (material_type == "fourier") {
    RemoveMeasuredFourierMaterialV3(parameters, *material.mutable_fourier());
  } else if (material_type == "glass") {
    RemoveGlassMaterialV3(parameters, *material.mutable_glass());
  } else if (material_type == "hair") {
    RemoveHairMaterialV3(parameters, *material.mutable_hair());
  } else if (material_type == "kdsubsurface") {
    RemoveKdSubsurfaceMaterialV3(parameters, *material.mutable_kdsubsurface());
  } else if (material_type == "metal") {
    RemoveMetalMaterialV3(parameters, *material.mutable_metal());
  } else if (material_type == "mirror") {
    RemoveMirrorMaterialV1(parameters, *material.mutable_mirror());
  } else if (material_type == "mix") {
    RemoveMixMaterialV2(parameters, *material.mutable_mix());
  } else if (material_type == "plastic") {
    RemovePlasticMaterialV3(parameters, *material.mutable_plastic());
  } else if (material_type == "substrate") {
    RemoveSubstrateMaterialV3(parameters, *material.mutable_substrate());
  } else if (material_type == "subsurface") {
    RemoveSubsurfaceMaterialV3(parameters, *material.mutable_subsurface());
  } else if (material_type == "translucent") {
    RemoveTranslucentMaterialV3(parameters, *material.mutable_translucent());
  } else if (material_type == "uber") {
    RemoveUberMaterialV3(parameters, *material.mutable_uber());
  } else {
    if (material_type != "matte") {
      std::cerr << "Unrecognized Material type: \"" << material_type << "\""
                << std::endl;
    }

    RemoveMatteMaterialV1(parameters, *material.mutable_matte());
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

class ParserV3 final : public ProtoParser<PbrtProto, 3> {
 public:
  ParserV3(PbrtProto& output) noexcept
      : ProtoParser(parameter_type_names, output) {}

 private:
  absl::Status Accelerator(
      absl::string_view accelerator_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status AreaLightSource(
      absl::string_view area_light_source_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status Camera(
      absl::string_view camera_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status Film(
      absl::string_view film_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status FloatTexture(
      absl::string_view float_texture_name,
      absl::string_view float_texture_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status Integrator(
      absl::string_view integrator_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status LightSource(
      absl::string_view light_source_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

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

  absl::Status PixelFilter(
      absl::string_view filter_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status Sampler(
      absl::string_view sampler_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

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

  absl::Status Volume(
      absl::string_view volume_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status VolumeIntegrator(
      absl::string_view integrator_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;
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

absl::Status ParserV3::AreaLightSource(
    absl::string_view area_light_source_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& area_light_source =
      *output_.add_directives()->mutable_area_light_source();

  if (area_light_source_type == "area" || area_light_source_type == "diffuse") {
    return RemoveDiffuseAreaLightSourceV3(parameters,
                                          *area_light_source.mutable_diffuse());
  }

  std::cerr << "Unrecognized AreaLightSource type: \"" << area_light_source_type
            << "\"" << std::endl;

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
    RemoveRealisticCameraV3(parameters, *camera.mutable_realistic());
  } else {
    std::cerr << "Unrecognized Camera type: \"" << camera_type << "\""
              << std::endl;
  }

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
    int32_t dimension = TryRemoveInteger(parameters, "dimension").value_or(2);
    if (dimension == 2) {
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
    RemovePtexFloatTextureV3(parameters, *float_texture.mutable_ptex());
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

absl::Status ParserV3::Integrator(
    absl::string_view integrator_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& integrator = *output_.add_directives()->mutable_integrator();

  if (integrator_type == "ambientocclusion") {
    RemoveAmbientOcclusionIntegratorV3(parameters,
                                       *integrator.mutable_ambientocclusion());
  } else if (integrator_type == "bdpt") {
    RemoveBdptIntegratorV3(parameters, *integrator.mutable_bdpt());
  } else if (integrator_type == "directlighting") {
    RemoveDirectLightingIntegratorV3(parameters,
                                     *integrator.mutable_directlighting());
  } else if (integrator_type == "mlt") {
    RemoveMltIntegratorV3(parameters, *integrator.mutable_mlt());
  } else if (integrator_type == "path") {
    RemovePathIntegratorV3(parameters, *integrator.mutable_path());
  } else if (integrator_type == "sppm") {
    RemoveSppmIntegratorV3(parameters, *integrator.mutable_sppm());
  } else if (integrator_type == "volpath") {
    RemoveVolPathIntegratorV3(parameters, *integrator.mutable_volpath());
  } else if (integrator_type == "whitted") {
    RemoveWhittedIntegratorV3(parameters, *integrator.mutable_whitted());
  } else {
    std::cerr << "Unrecognized Integrator type: \"" << integrator_type << "\""
              << std::endl;
  }

  return absl::OkStatus();
}

absl::Status ParserV3::LightSource(
    absl::string_view light_source_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& light_source = *output_.add_directives()->mutable_light_source();

  if (light_source_type == "distant") {
    return RemoveDistantLightSourceV2(parameters,
                                      *light_source.mutable_distant());
  }

  if (light_source_type == "goniometric") {
    return RemoveGoniometricLightSourceV2(parameters,
                                          *light_source.mutable_goniometric());
  }

  if (light_source_type == "infinite") {
    return RemoveInfiniteLightSourceV3(parameters,
                                       *light_source.mutable_infinite());
  }

  if (light_source_type == "point") {
    return RemovePointLightSourceV2(parameters, *light_source.mutable_point());
  }

  if (light_source_type == "projection") {
    return RemoveProjectionLightSourceV2(parameters,
                                         *light_source.mutable_projection());
  }

  if (light_source_type == "spot") {
    return RemoveSpotLightSourceV2(parameters, *light_source.mutable_spot());
  }

  std::cerr << "Unrecognized LightSource type: \"" << light_source_type << "\""
            << std::endl;

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
    return RemoveHomogeneousMediumV3(parameters,
                                     *make_named_medium.mutable_homogeneous());
  }

  if (medium_type == "heterogeneous") {
    return RemoveUniformGridMediumV3(
        parameters, *make_named_medium.mutable_heterogeneous());
  }

  std::cerr << "Unrecognized MakeNamedMedium type: \"" << medium_type << "\""
            << std::endl;

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
    int32_t dimension = TryRemoveInteger(parameters, "dimension").value_or(2);
    if (dimension == 2) {
      RemoveCheckerboard2DSpectrumTextureV2(
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
    RemoveImageMapSpectrumTextureV3(parameters,
                                    *spectrum_texture.mutable_imagemap());
  } else if (spectrum_texture_type == "marble") {
    RemoveMarbleSpectrumTextureV1(parameters,
                                  *spectrum_texture.mutable_marble());
  } else if (spectrum_texture_type == "mix") {
    RemoveMixSpectrumTextureV1(parameters, *spectrum_texture.mutable_mix());
  } else if (spectrum_texture_type == "ptex") {
    RemovePtexSpectrumTextureV3(parameters, *spectrum_texture.mutable_ptex());
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

absl::Status ParserV3::SurfaceIntegrator(
    absl::string_view integrator_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  return absl::UnimplementedError(
      "Directive 'SurfaceIntegrator' is not supported in pbrt-v3");
}

absl::Status ParserV3::Volume(
    absl::string_view volume_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  return absl::UnimplementedError(
      "Directive 'Volume' is not supported in pbrt-v3");
}

absl::Status ParserV3::VolumeIntegrator(
    absl::string_view integrator_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  return absl::UnimplementedError(
      "Directive 'VolumeIntegrator' is not supported in pbrt-v3");
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
