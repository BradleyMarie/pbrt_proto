#include "pbrt_proto/v3/convert.h"

#include <functional>
#include <istream>

#include "absl/container/flat_hash_map.h"
#include "absl/functional/function_ref.h"
#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"
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

  absl::Status MakeNamedMedium(
      absl::string_view medium_name,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status Material(
      absl::string_view material_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters,
      v3::Material& material) override;

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
};

absl::Status ParserV3::Accelerator(
    absl::string_view accelerator_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v3::Accelerator> kSupportedTypes = {
      {"bvh", CB<RemoveBvhAccelerator, &Accelerator::mutable_bvh>()},
      {"kdtree", CB<RemoveKdTreeAccelerator, &Accelerator::mutable_kdtree>()},
  };

  return Parse<&Directive::mutable_accelerator>(kSupportedTypes,
                                                accelerator_type, parameters);
}

absl::Status ParserV3::AreaLightSource(
    absl::string_view area_light_source_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v3::AreaLightSource> kSupportedTypes = {
      {"area",
       CB<RemoveDiffuseAreaLightSource, &AreaLightSource::mutable_diffuse>()},
      {"diffuse",
       CB<RemoveDiffuseAreaLightSource, &AreaLightSource::mutable_diffuse>()},
  };

  return Parse<&Directive::mutable_area_light_source>(
      kSupportedTypes, area_light_source_type, parameters);
}

absl::Status ParserV3::Camera(
    absl::string_view camera_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v3::Camera> kSupportedTypes = {
      {"environment",
       CB<RemoveSphericalCamera, &Camera::mutable_environment>()},
      {"orthographic",
       CB<RemoveOrthographicCamera, &Camera::mutable_orthographic>()},
      {"perspective",
       CB<RemovePerspectiveCamera, &Camera::mutable_perspective>()},
      {"realistic", CB<RemoveRealisticCamera, &Camera::mutable_realistic>()},
  };

  return Parse<&Directive::mutable_camera>(kSupportedTypes, camera_type,
                                           parameters);
}

absl::Status ParserV3::Film(
    absl::string_view film_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v3::Film> kSupportedTypes = {
      {"image", CB<RemoveRgbFilm, &Film::mutable_image>()},
  };

  return Parse<&Directive::mutable_film>(kSupportedTypes, film_type,
                                         parameters);
}

absl::Status ParserV3::FloatTexture(
    absl::string_view float_texture_name, absl::string_view float_texture_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v3::FloatTexture> kSupportedTypes = {
      {"bilerp",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::FloatTexture& float_texture) {
         RemoveBilerpFloatTextureV1(parameters,
                                    *float_texture.mutable_bilerp());
         return absl::OkStatus();
       }},
      {"checkerboard",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::FloatTexture& float_texture) {
         int32_t dimension =
             TryRemoveInteger(parameters, "dimension").value_or(2);

         if (dimension == 2) {
           RemoveCheckerboard2DFloatTextureV2(
               parameters, *float_texture.mutable_checkerboard2d());
         } else {
           RemoveCheckerboard3DFloatTextureV1(
               parameters, *float_texture.mutable_checkerboard3d());
         }

         return absl::OkStatus();
       }},
      {"constant",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::FloatTexture& float_texture) {
         RemoveConstantFloatTextureV1(parameters,
                                      *float_texture.mutable_constant());
         return absl::OkStatus();
       }},
      {"dots",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::FloatTexture& float_texture) {
         RemoveDotsFloatTextureV1(parameters, *float_texture.mutable_dots());
         return absl::OkStatus();
       }},
      {"fbm",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::FloatTexture& float_texture) {
         RemoveFBmFloatTextureV1(parameters, *float_texture.mutable_fbm());
         return absl::OkStatus();
       }},
      {"imagemap",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::FloatTexture& float_texture) {
         RemoveImageMapFloatTextureV3(parameters,
                                      *float_texture.mutable_imagemap());
         return absl::OkStatus();
       }},
      {"marble",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::FloatTexture& float_texture) { return absl::OkStatus(); }},
      {"mix",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::FloatTexture& float_texture) {
         RemoveMixFloatTextureV1(parameters, *float_texture.mutable_mix());
         return absl::OkStatus();
       }},
      {"ptex",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::FloatTexture& float_texture) {
         RemovePtexFloatTextureV3(parameters, *float_texture.mutable_ptex());
         return absl::OkStatus();
       }},
      {"scale",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::FloatTexture& float_texture) {
         RemoveScaleFloatTextureV1(parameters, *float_texture.mutable_scale());
         return absl::OkStatus();
       }},
      {"windy",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::FloatTexture& float_texture) {
         float_texture.mutable_windy();
         return absl::OkStatus();
       }},
      {"wrinkled",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::FloatTexture& float_texture) {
         RemoveWrinkledFloatTextureV1(parameters,
                                      *float_texture.mutable_wrinkled());
         return absl::OkStatus();
       }}};

  auto& float_texture = *output_.add_directives()->mutable_float_texture();
  float_texture.set_name(float_texture_name);

  auto iter = kSupportedTypes.find(float_texture_type);
  if (iter == kSupportedTypes.end()) {
    return UnrecognizedTypeError("Texture", float_texture_type);
  }

  return iter->second(parameters, float_texture);
}

absl::Status ParserV3::Integrator(
    absl::string_view integrator_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v3::Integrator> kSupportedTypes = {
      {"ambientocclusion",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Integrator& integrator) {
         RemoveAmbientOcclusionIntegratorV3(
             parameters, *integrator.mutable_ambientocclusion());
         return absl::OkStatus();
       }},
      {"bdpt",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Integrator& integrator) {
         RemoveBdptIntegratorV3(parameters, *integrator.mutable_bdpt());
         return absl::OkStatus();
       }},
      {"directlighting",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Integrator& integrator) {
         RemoveDirectLightingIntegratorV3(parameters,
                                          *integrator.mutable_directlighting());
         return absl::OkStatus();
       }},
      {"mlt",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Integrator& integrator) {
         RemoveMltIntegratorV3(parameters, *integrator.mutable_mlt());
         return absl::OkStatus();
       }},
      {"path",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Integrator& integrator) {
         RemovePathIntegratorV3(parameters, *integrator.mutable_path());
         return absl::OkStatus();
       }},
      {"sppm",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Integrator& integrator) {
         RemoveSppmIntegratorV3(parameters, *integrator.mutable_sppm());
         return absl::OkStatus();
       }},
      {"volpath",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Integrator& integrator) {
         RemoveVolPathIntegratorV3(parameters, *integrator.mutable_volpath());
         return absl::OkStatus();
       }},
      {"whitted",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Integrator& integrator) {
         RemoveWhittedIntegratorV3(parameters, *integrator.mutable_whitted());
         return absl::OkStatus();
       }},
  };

  auto& integrator = *output_.add_directives()->mutable_integrator();

  auto iter = kSupportedTypes.find(integrator_type);
  if (iter == kSupportedTypes.end()) {
    return UnrecognizedTypeError("Integrator", integrator_type);
  }

  return iter->second(parameters, integrator);
}

absl::Status ParserV3::LightSource(
    absl::string_view light_source_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v3::LightSource> kSupportedTypes = {
      {"distant",
       CB<RemoveDistantLightSource, &LightSource::mutable_distant>()},
      {"goniometric",
       CB<RemoveGoniometricLightSource, &LightSource::mutable_goniometric>()},
      {"infinite",
       CB<RemoveInfiniteLightSource, &LightSource::mutable_infinite>()},
      {"point", CB<RemovePointLightSource, &LightSource::mutable_point>()},
      {"projection",
       CB<RemoveProjectionLightSource, &LightSource::mutable_projection>()},
      {"spot", CB<RemoveSpotLightSource, &LightSource::mutable_spot>()},
  };

  return Parse<&Directive::mutable_light_source>(kSupportedTypes,
                                                 light_source_type, parameters);
}

absl::Status ParserV3::MakeNamedMedium(
    absl::string_view medium_name,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v3::MakeNamedMedium> kSupportedTypes = {
      {"homogeneous",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::MakeNamedMedium& make_named_medium) {
         return RemoveHomogeneousMediumV3(
             parameters, *make_named_medium.mutable_homogeneous());
       }},
      {"heterogeneous",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::MakeNamedMedium& make_named_medium) {
         return RemoveUniformGridMediumV3(
             parameters, *make_named_medium.mutable_heterogeneous());
       }},
  };

  absl::string_view medium_type =
      TryRemoveString(parameters, "type").value_or("");

  auto& make_named_medium =
      *output_.add_directives()->mutable_make_named_medium();
  make_named_medium.set_name(medium_name);

  auto iter = kSupportedTypes.find(medium_type);
  if (iter == kSupportedTypes.end()) {
    return UnrecognizedTypeError("MakeNamedMedium", medium_type);
  }

  return iter->second(parameters, make_named_medium);
}

absl::Status ParserV3::Material(
    absl::string_view material_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    v3::Material& material) {
  static const TypeMap<v3::Material> kSupportedTypes = {
      {"", [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
              v3::Material& material) { return absl::OkStatus(); }},
      {"disney",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Material& material) {
         RemoveDisneyMaterialV3(parameters, *material.mutable_disney());
         return absl::OkStatus();
       }},
      {"fourier",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Material& material) {
         RemoveMeasuredFourierMaterialV3(parameters,
                                         *material.mutable_fourier());
         return absl::OkStatus();
       }},
      {"glass",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Material& material) {
         RemoveGlassMaterialV3(parameters, *material.mutable_glass());
         return absl::OkStatus();
       }},
      {"hair",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Material& material) {
         RemoveHairMaterialV3(parameters, *material.mutable_hair());
         return absl::OkStatus();
       }},
      {"kdsubsurface",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Material& material) {
         RemoveKdSubsurfaceMaterialV3(parameters,
                                      *material.mutable_kdsubsurface());
         return absl::OkStatus();
       }},
      {"matte",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Material& material) {
         RemoveMatteMaterialV1(parameters, *material.mutable_matte());
         return absl::OkStatus();
       }},
      {"metal",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Material& material) {
         RemoveMetalMaterialV3(parameters, *material.mutable_metal());
         return absl::OkStatus();
       }},
      {"mirror",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Material& material) {
         RemoveMirrorMaterialV1(parameters, *material.mutable_mirror());
         return absl::OkStatus();
       }},
      {"mix",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Material& material) {
         RemoveMixMaterialV2(parameters, *material.mutable_mix());
         return absl::OkStatus();
       }},
      {"none", [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
                  v3::Material& material) { return absl::OkStatus(); }},
      {"plastic",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Material& material) {
         RemovePlasticMaterialV3(parameters, *material.mutable_plastic());
         return absl::OkStatus();
       }},
      {"substrate",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Material& material) {
         RemoveSubstrateMaterialV3(parameters, *material.mutable_substrate());
         return absl::OkStatus();
       }},
      {"subsurface",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Material& material) {
         RemoveSubsurfaceMaterialV3(parameters, *material.mutable_subsurface());
         return absl::OkStatus();
       }},
      {"translucent",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Material& material) {
         RemoveTranslucentMaterialV3(parameters,
                                     *material.mutable_translucent());
         return absl::OkStatus();
       }},
      {"uber",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Material& material) {
         RemoveUberMaterialV3(parameters, *material.mutable_uber());
         return absl::OkStatus();
       }},
  };

  auto iter = kSupportedTypes.find(material_type);
  if (iter == kSupportedTypes.end()) {
    RemoveMatteMaterialV1(parameters, *material.mutable_matte());
    return UnrecognizedTypeError("Material", material_type);
  }

  return iter->second(parameters, material);
}

absl::Status ParserV3::PixelFilter(
    absl::string_view pixel_filter_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v3::PixelFilter> kSupportedTypes = {
      {"box", CB<RemoveBoxPixelFilter, &PixelFilter::mutable_box>()},
      {"gaussian",
       CB<RemoveGaussianPixelFilter, &PixelFilter::mutable_gaussian>()},
      {"mitchell",
       CB<RemoveMitchellPixelFilter, &PixelFilter::mutable_mitchell>()},
      {"sinc", CB<RemoveLanczosPixelFilter, &PixelFilter::mutable_sinc>()},
      {"triangle",
       CB<RemoveTrianglePixelFilter, &PixelFilter::mutable_triangle>()},
  };

  return Parse<&Directive::mutable_pixel_filter>(kSupportedTypes,
                                                 pixel_filter_type, parameters);
}

absl::Status ParserV3::Sampler(
    absl::string_view sampler_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v3::Sampler> kSupportedTypes = {
      {"halton",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Sampler& sampler) {
         RemoveHaltonSamplerV3(parameters, *sampler.mutable_halton());
         return absl::OkStatus();
       }},
      {"lowdiscrepancy",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Sampler& sampler) {
         RemoveZeroTwoSequenceSamplerV3(parameters,
                                        *sampler.mutable_zerotwosequence());
         return absl::OkStatus();
       }},
      {"maxmindist",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Sampler& sampler) {
         RemoveMaxMinDistSamplerV3(parameters, *sampler.mutable_maxmindist());
         return absl::OkStatus();
       }},
      {"random",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Sampler& sampler) {
         RemoveIndependentSamplerV2(parameters, *sampler.mutable_random());
         return absl::OkStatus();
       }},
      {"sobol",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Sampler& sampler) {
         RemoveSobolSamplerV3(parameters, *sampler.mutable_sobol());
         return absl::OkStatus();
       }},
      {"stratified",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Sampler& sampler) {
         RemoveStratifiedSamplerV3(parameters, *sampler.mutable_stratified());
         return absl::OkStatus();
       }},
      {"02sequence",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Sampler& sampler) {
         RemoveZeroTwoSequenceSamplerV3(parameters,
                                        *sampler.mutable_zerotwosequence());
         return absl::OkStatus();
       }},
  };

  auto& sampler = *output_.add_directives()->mutable_sampler();

  auto iter = kSupportedTypes.find(sampler_type);
  if (iter == kSupportedTypes.end()) {
    return UnrecognizedTypeError("Sampler", sampler_type);
  }

  return iter->second(parameters, sampler);
}

absl::Status ParserV3::Shape(
    absl::string_view shape_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v3::Shape> kSupportedTypes = {
      {"cone",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Shape& shape) {
         RemoveConeShapeV1(parameters, *shape.mutable_cone());
         return absl::OkStatus();
       }},
      {"curve",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Shape& shape) {
         return TryRemoveCurveShapeV3(parameters,
                                      std::bind(&Shape::mutable_curve, &shape));
       }},
      {"cylinder",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Shape& shape) {
         RemoveCylinderShapeV1(parameters, *shape.mutable_cylinder());
         return absl::OkStatus();
       }},
      {"disk",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Shape& shape) {
         RemoveDiskShapeV1(parameters, *shape.mutable_disk());
         return absl::OkStatus();
       }},
      {"heightfield",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Shape& shape) {
         return RemoveHeightFieldShapeV1(parameters,
                                         *shape.mutable_heightfield());
       }},
      {"hyperboloid",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Shape& shape) {
         RemoveHyperboloidShapeV1(parameters, *shape.mutable_hyperboloid());
         return absl::OkStatus();
       }},
      {"loopsubdiv",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Shape& shape) {
         return RemoveLoopSubdivShapeV3(parameters,
                                        *shape.mutable_loopsubdiv());
       }},
      {"nurbs",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Shape& shape) {
         return RemoveNurbsShapeV1(parameters, *shape.mutable_nurbs());
       }},
      {"paraboloid",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Shape& shape) {
         RemoveParaboloidShapeV1(parameters, *shape.mutable_paraboloid());
         return absl::OkStatus();
       }},
      {"plymesh",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Shape& shape) {
         RemovePlyMeshShapeV3(parameters, *shape.mutable_plymesh());
         return absl::OkStatus();
       }},
      {"sphere",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Shape& shape) {
         RemoveSphereShapeV1(parameters, *shape.mutable_sphere());
         return absl::OkStatus();
       }},
      {"trianglemesh",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Shape& shape) {
         return RemoveTriangleMeshShapeV3(parameters,
                                          *shape.mutable_trianglemesh());
       }}};

  auto& shape = *output_.add_directives()->mutable_shape();

  auto iter = kSupportedTypes.find(shape_type);
  if (iter == kSupportedTypes.end()) {
    return UnrecognizedTypeError("Shape", shape_type);
  }

  if (absl::Status status = iter->second(parameters, shape); !status.ok()) {
    return status;
  }

  auto overrides = std::bind(&Shape::mutable_overrides, &shape);
  TryRemoveFloatTexture(
      parameters, "alpha",
      std::bind(&Shape::MaterialOverrides::mutable_alpha, overrides));
  TryRemoveFloatTexture(
      parameters, "anisotropic",
      std::bind(&Shape::MaterialOverrides::mutable_anisotropic, overrides));
  TryRemoveFloatTexture(
      parameters, "beta_m",
      std::bind(&Shape::MaterialOverrides::mutable_beta_m, overrides));
  TryRemoveFloatTexture(
      parameters, "beta_n",
      std::bind(&Shape::MaterialOverrides::mutable_beta_n, overrides));
  TryRemoveFloatTexture(
      parameters, "bumpmap",
      std::bind(&Shape::MaterialOverrides::mutable_bumpmap, overrides));
  TryRemoveFloatTexture(
      parameters, "clearcoat",
      std::bind(&Shape::MaterialOverrides::mutable_clearcoat, overrides));
  TryRemoveFloatTexture(
      parameters, "clearcoatgloss",
      std::bind(&Shape::MaterialOverrides::mutable_clearcoatgloss, overrides));
  TryRemoveFloatTexture(
      parameters, "difftrans",
      std::bind(&Shape::MaterialOverrides::mutable_difftrans, overrides));
  TryRemoveFloatTexture(
      parameters, "eumelanin",
      std::bind(&Shape::MaterialOverrides::mutable_eumelanin, overrides));
  TryRemoveFloatTexture(
      parameters, "flatness",
      std::bind(&Shape::MaterialOverrides::mutable_flatness, overrides));
  TryRemoveFloatTexture(
      parameters, "metallic",
      std::bind(&Shape::MaterialOverrides::mutable_metallic, overrides));
  TryRemoveFloatTexture(
      parameters, "pheomelanin",
      std::bind(&Shape::MaterialOverrides::mutable_pheomelanin, overrides));
  TryRemoveFloatTexture(
      parameters, "roughness",
      std::bind(&Shape::MaterialOverrides::mutable_roughness, overrides));
  TryRemoveFloatTexture(
      parameters, "sheen",
      std::bind(&Shape::MaterialOverrides::mutable_sheen, overrides));
  TryRemoveFloatTexture(
      parameters, "sheentint",
      std::bind(&Shape::MaterialOverrides::mutable_sheentint, overrides));
  TryRemoveFloatTexture(
      parameters, "sigma",
      std::bind(&Shape::MaterialOverrides::mutable_sigma, overrides));
  TryRemoveFloatTexture(
      parameters, "spectrans",
      std::bind(&Shape::MaterialOverrides::mutable_spectrans, overrides));
  TryRemoveFloatTexture(
      parameters, "speculartint",
      std::bind(&Shape::MaterialOverrides::mutable_speculartint, overrides));
  TryRemoveFloatTexture(
      parameters, "uroughness",
      std::bind(&Shape::MaterialOverrides::mutable_uroughness, overrides));
  TryRemoveFloatTexture(
      parameters, "vroughness",
      std::bind(&Shape::MaterialOverrides::mutable_vroughness, overrides));
  TryRemoveSpectrumTextureV1(
      parameters, "amount",
      std::bind(&Shape::MaterialOverrides::mutable_amount, overrides));
  TryRemoveSpectrumTextureV1(
      parameters, "color",
      std::bind(&Shape::MaterialOverrides::mutable_color, overrides));
  TryRemoveSpectrumTextureV1(
      parameters, "k",
      std::bind(&Shape::MaterialOverrides::mutable_k, overrides));
  TryRemoveSpectrumTextureV1(
      parameters, "Kd",
      std::bind(&Shape::MaterialOverrides::mutable_kd, overrides));
  TryRemoveSpectrumTextureV1(
      parameters, "Kr",
      std::bind(&Shape::MaterialOverrides::mutable_kr, overrides));
  TryRemoveSpectrumTextureV1(
      parameters, "Ks",
      std::bind(&Shape::MaterialOverrides::mutable_ks, overrides));
  TryRemoveSpectrumTextureV1(
      parameters, "Kt",
      std::bind(&Shape::MaterialOverrides::mutable_kt, overrides));
  TryRemoveSpectrumTextureV1(
      parameters, "mfp",
      std::bind(&Shape::MaterialOverrides::mutable_mfp, overrides));
  TryRemoveSpectrumTextureV1(
      parameters, "opacity",
      std::bind(&Shape::MaterialOverrides::mutable_opacity, overrides));
  TryRemoveSpectrumTextureV1(
      parameters, "reflect",
      std::bind(&Shape::MaterialOverrides::mutable_reflect, overrides));
  TryRemoveSpectrumTextureV1(
      parameters, "scatterdistance",
      std::bind(&Shape::MaterialOverrides::mutable_scatterdistance, overrides));
  TryRemoveSpectrumTextureV1(
      parameters, "sigma_a",
      std::bind(&Shape::MaterialOverrides::mutable_sigma_a, overrides));
  TryRemoveSpectrumTextureV1(
      parameters, "sigma_s",
      std::bind(&Shape::MaterialOverrides::mutable_sigma_s, overrides));
  TryRemoveSpectrumTextureV1(
      parameters, "transmit",
      std::bind(&Shape::MaterialOverrides::mutable_transmit, overrides));

  if (std::optional<absl::string_view> name =
          TryRemoveString(parameters, "name");
      name.has_value()) {
    auto iter = kNamedMeasuredScatteringPresets.find(*name);
    if (iter != kNamedMeasuredScatteringPresets.end()) {
      shape.mutable_overrides()->set_name(iter->second);
    }
  }

  if (std::optional<bool> remaproughness =
          TryRemoveBool(parameters, "remaproughness");
      remaproughness.has_value()) {
    shape.mutable_overrides()->set_remaproughness(*remaproughness);
  }

  if (std::optional<bool> thin = TryRemoveBool(parameters, "thin");
      thin.has_value()) {
    shape.mutable_overrides()->set_thin(*thin);
  }

  if (std::optional<double> g = TryRemoveFloat(parameters, "g");
      g.has_value()) {
    shape.mutable_overrides()->set_g(*g);
  }

  if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
      scale.has_value()) {
    shape.mutable_overrides()->set_scale(*scale);
  }

  if (std::optional<absl::string_view> bsdffile =
          TryRemoveString(parameters, "bsdffile");
      bsdffile.has_value()) {
    shape.mutable_overrides()->set_filename(*bsdffile);
  }

  if (std::optional<absl::string_view> namedmaterial1 =
          TryRemoveString(parameters, "namedmaterial1");
      namedmaterial1.has_value()) {
    shape.mutable_overrides()->set_namedmaterial1(*namedmaterial1);
  }

  if (std::optional<absl::string_view> namedmaterial2 =
          TryRemoveString(parameters, "namedmaterial2");
      namedmaterial2.has_value()) {
    shape.mutable_overrides()->set_namedmaterial2(*namedmaterial2);
  }

  if (std::optional<double> eta = TryRemoveFloat(parameters, "eta");
      eta.has_value()) {
    shape.mutable_overrides()->set_eta_as_value(*eta);
    shape.mutable_overrides()->mutable_eta()->set_float_value(*eta);
  } else if (std::optional<absl::string_view> eta =
                 TryRemoveTexture(parameters, "eta");
             eta.has_value()) {
    shape.mutable_overrides()->mutable_eta()->set_float_texture_name(*eta);
    shape.mutable_overrides()
        ->mutable_eta_as_spectrum_texture()
        ->set_spectrum_texture_name(*eta);
  } else {
    TryRemoveSpectrumTextureV1(
        parameters, "eta",
        std::bind(&Shape::MaterialOverrides::mutable_eta_as_spectrum_texture,
                  overrides));
  }

  return absl::OkStatus();
}

absl::Status ParserV3::SpectrumTexture(
    absl::string_view spectrum_texture_name,
    absl::string_view spectrum_texture_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v3::SpectrumTexture> kSupportedTypes = {
      {"bilerp",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::SpectrumTexture& spectrum_texture) {
         RemoveBilerpSpectrumTextureV1(parameters,
                                       *spectrum_texture.mutable_bilerp());
         return absl::OkStatus();
       }},
      {"checkerboard",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::SpectrumTexture& spectrum_texture) {
         int32_t dimension =
             TryRemoveInteger(parameters, "dimension").value_or(2);

         if (dimension == 2) {
           RemoveCheckerboard2DSpectrumTextureV2(
               parameters, *spectrum_texture.mutable_checkerboard2d());
         } else {
           RemoveCheckerboard3DSpectrumTextureV1(
               parameters, *spectrum_texture.mutable_checkerboard3d());
         }

         return absl::OkStatus();
       }},
      {"constant",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::SpectrumTexture& spectrum_texture) {
         return RemoveConstantSpectrumTextureV1(
             parameters, *spectrum_texture.mutable_constant());
       }},
      {"dots",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::SpectrumTexture& spectrum_texture) {
         RemoveDotsSpectrumTextureV1(parameters,
                                     *spectrum_texture.mutable_dots());
         return absl::OkStatus();
       }},
      {"fbm",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::SpectrumTexture& spectrum_texture) {
         RemoveFBmSpectrumTextureV1(parameters,
                                    *spectrum_texture.mutable_fbm());
         return absl::OkStatus();
       }},
      {"imagemap",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::SpectrumTexture& spectrum_texture) {
         RemoveImageMapSpectrumTextureV3(parameters,
                                         *spectrum_texture.mutable_imagemap());
         return absl::OkStatus();
       }},
      {"marble",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::SpectrumTexture& spectrum_texture) {
         RemoveMarbleSpectrumTextureV1(parameters,
                                       *spectrum_texture.mutable_marble());
         return absl::OkStatus();
       }},
      {"mix",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::SpectrumTexture& spectrum_texture) {
         RemoveMixSpectrumTextureV1(parameters,
                                    *spectrum_texture.mutable_mix());
         return absl::OkStatus();
       }},
      {"ptex",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::SpectrumTexture& spectrum_texture) {
         RemovePtexSpectrumTextureV3(parameters,
                                     *spectrum_texture.mutable_ptex());
         return absl::OkStatus();
       }},
      {"scale",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::SpectrumTexture& spectrum_texture) {
         RemoveScaleSpectrumTextureV1(parameters,
                                      *spectrum_texture.mutable_scale());
         return absl::OkStatus();
       }},
      {"uv",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::SpectrumTexture& spectrum_texture) {
         RemoveUvSpectrumTextureV1(parameters, *spectrum_texture.mutable_uv());
         return absl::OkStatus();
       }},
      {"windy",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::SpectrumTexture& spectrum_texture) {
         spectrum_texture.mutable_windy();
         return absl::OkStatus();
       }},
      {"wrinkled",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::SpectrumTexture& spectrum_texture) {
         RemoveWrinkledSpectrumTextureV1(parameters,
                                         *spectrum_texture.mutable_wrinkled());
         return absl::OkStatus();
       }}};

  auto& spectrum_texture =
      *output_.add_directives()->mutable_spectrum_texture();
  spectrum_texture.set_name(spectrum_texture_name);

  auto iter = kSupportedTypes.find(spectrum_texture_type);
  if (iter == kSupportedTypes.end()) {
    return UnrecognizedTypeError("Texture", spectrum_texture_type);
  }

  return iter->second(parameters, spectrum_texture);
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
