#include "pbrt_proto/v1/convert.h"

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

class ParserV1 final : public ProtoParser<PbrtProto, 1> {
 public:
  ParserV1(PbrtProto& output) noexcept
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

  absl::Status LightSource(
      absl::string_view light_source_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status Material(
      absl::string_view material_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters,
      v1::Material& material) override;

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

absl::Status ParserV1::Accelerator(
    absl::string_view accelerator_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v1::Accelerator> kSupportedTypes = {
      {"grid", CB<RemoveGridAccelerator, &Accelerator::mutable_grid>()},
      {"kdtree", CB<RemoveKdTreeAccelerator, &Accelerator::mutable_kdtree>()},
  };

  return Parse<&Directive::mutable_accelerator>(kSupportedTypes,
                                                accelerator_type, parameters);
}

absl::Status ParserV1::AreaLightSource(
    absl::string_view area_light_source_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v1::AreaLightSource> kSupportedTypes = {
      {"area",
       CB<RemoveDiffuseAreaLightSource, &AreaLightSource::mutable_area>()},
  };

  return Parse<&Directive::mutable_area_light_source>(
      kSupportedTypes, area_light_source_type, parameters);
}

absl::Status ParserV1::Camera(
    absl::string_view camera_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v1::Camera> kSupportedTypes = {
      {"environment",
       CB<RemoveSphericalCamera, &Camera::mutable_environment>()},
      {"orthographic",
       CB<RemoveOrthographicCamera, &Camera::mutable_orthographic>()},
      {"perspective",
       CB<RemovePerspectiveCamera, &Camera::mutable_perspective>()},
  };

  return Parse<&Directive::mutable_camera>(kSupportedTypes, camera_type,
                                           parameters);
}

absl::Status ParserV1::Film(
    absl::string_view film_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v1::Film> kSupportedTypes = {
      {"image", CB<RemoveRgbFilm, &Film::mutable_image>()},
  };

  return Parse<&Directive::mutable_film>(kSupportedTypes, film_type,
                                         parameters);
}

absl::Status ParserV1::FloatTexture(
    absl::string_view float_texture_name, absl::string_view float_texture_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v1::FloatTexture> kSupportedTypes = {
      {"bilerp",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::FloatTexture& float_texture) {
         RemoveBilerpFloatTextureV1(parameters,
                                    *float_texture.mutable_bilerp());
         return absl::OkStatus();
       }},
      {"checkerboard",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::FloatTexture& float_texture) {
         int32_t dimension =
             TryRemoveInteger(parameters, "dimension").value_or(2);

         if (dimension == 2) {
           RemoveCheckerboard2DFloatTextureV1(
               parameters, *float_texture.mutable_checkerboard2d());
         } else {
           RemoveCheckerboard3DFloatTextureV1(
               parameters, *float_texture.mutable_checkerboard3d());
         }

         return absl::OkStatus();
       }},
      {"constant",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::FloatTexture& float_texture) {
         RemoveConstantFloatTextureV1(parameters,
                                      *float_texture.mutable_constant());
         return absl::OkStatus();
       }},
      {"dots",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::FloatTexture& float_texture) {
         RemoveDotsFloatTextureV1(parameters, *float_texture.mutable_dots());
         return absl::OkStatus();
       }},
      {"fbm",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::FloatTexture& float_texture) {
         RemoveFBmFloatTextureV1(parameters, *float_texture.mutable_fbm());
         return absl::OkStatus();
       }},
      {"imagemap",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::FloatTexture& float_texture) {
         RemoveImageMapFloatTextureV1(parameters,
                                      *float_texture.mutable_imagemap());
         return absl::OkStatus();
       }},
      {"marble",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::FloatTexture& float_texture) { return absl::OkStatus(); }},
      {"mix",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::FloatTexture& float_texture) {
         RemoveMixFloatTextureV1(parameters, *float_texture.mutable_mix());
         return absl::OkStatus();
       }},
      {"scale",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::FloatTexture& float_texture) {
         RemoveScaleFloatTextureV1(parameters, *float_texture.mutable_scale());
         return absl::OkStatus();
       }},
      {"windy",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::FloatTexture& float_texture) {
         float_texture.mutable_windy();
         return absl::OkStatus();
       }},
      {"wrinkled",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::FloatTexture& float_texture) {
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

absl::Status ParserV1::LightSource(
    absl::string_view light_source_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v1::LightSource> kSupportedTypes = {
      {"distant",
       CB<RemoveDistantLightSource, &LightSource::mutable_distant>()},
      {"goniometric",
       CB<RemoveGoniometricLightSource, &LightSource::mutable_goniometric>()},
      {"infinite",
       CB<RemoveInfiniteLightSource, &LightSource::mutable_infinite>()},
      {"infinitesample",
       CB<RemoveInfiniteLightSource, &LightSource::mutable_infinitesample>()},
      {"point", CB<RemovePointLightSource, &LightSource::mutable_point>()},
      {"projection",
       CB<RemoveProjectionLightSource, &LightSource::mutable_projection>()},
      {"spot", CB<RemoveSpotLightSource, &LightSource::mutable_spot>()},
  };

  return Parse<&Directive::mutable_light_source>(kSupportedTypes,
                                                 light_source_type, parameters);
}

absl::Status ParserV1::Material(
    absl::string_view material_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    v1::Material& material) {
  static const TypeMap<v1::Material> kSupportedTypes = {
      {"bluepaint",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::Material& material) {
         material.mutable_bluepaint();
         return absl::OkStatus();
       }},
      {"brushedmetal",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::Material& material) {
         material.mutable_brushedmetal();
         return absl::OkStatus();
       }},
      {"clay",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::Material& material) {
         material.mutable_clay();
         return absl::OkStatus();
       }},
      {"felt",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::Material& material) {
         material.mutable_felt();
         return absl::OkStatus();
       }},
      {"glass",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::Material& material) {
         RemoveGlassMaterialV1(parameters, *material.mutable_glass());
         return absl::OkStatus();
       }},
      {"matte",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::Material& material) {
         RemoveMatteMaterialV1(parameters, *material.mutable_matte());
         return absl::OkStatus();
       }},
      {"mirror",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::Material& material) {
         RemoveMirrorMaterialV1(parameters, *material.mutable_mirror());
         return absl::OkStatus();
       }},
      {"plastic",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::Material& material) {
         RemovePlasticMaterialV1(parameters, *material.mutable_plastic());
         return absl::OkStatus();
       }},
      {"primer",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::Material& material) {
         material.mutable_primer();
         return absl::OkStatus();
       }},
      {"shinymetal",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::Material& material) {
         RemoveShinyMetalMaterialV1(parameters, *material.mutable_shinymetal());
         return absl::OkStatus();
       }},
      {"skin",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::Material& material) {
         material.mutable_skin();
         return absl::OkStatus();
       }},
      {"substrate",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::Material& material) {
         RemoveSubstrateMaterialV1(parameters, *material.mutable_substrate());
         return absl::OkStatus();
       }},
      {"translucent",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::Material& material) {
         RemoveTranslucentMaterialV1(parameters,
                                     *material.mutable_translucent());
         return absl::OkStatus();
       }},
      {"uber", [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
                  v1::Material& material) {
         RemoveUberMaterialV1(parameters, *material.mutable_uber());
         return absl::OkStatus();
       }}};

  auto iter = kSupportedTypes.find(material_type);
  if (iter == kSupportedTypes.end()) {
    RemoveMatteMaterialV1(parameters, *material.mutable_matte());
    return UnrecognizedTypeError("Material", material_type);
  }

  return iter->second(parameters, material);
}

absl::Status ParserV1::PixelFilter(
    absl::string_view pixel_filter_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v1::PixelFilter> kSupportedTypes = {
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

absl::Status ParserV1::Sampler(
    absl::string_view sampler_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v1::Sampler> kSupportedTypes = {
      {"bestcandidate",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::Sampler& sampler) {
         RemoveBestCandidateSamplerV1(parameters,
                                      *sampler.mutable_bestcandidate());
         return absl::OkStatus();
       }},
      {"lowdiscrepancy",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::Sampler& sampler) {
         RemoveZeroTwoSequenceSamplerV1(parameters,
                                        *sampler.mutable_lowdiscrepancy());
         return absl::OkStatus();
       }},
      {"random",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::Sampler& sampler) {
         RemoveRandomSamplerV1(parameters, *sampler.mutable_random());
         return absl::OkStatus();
       }},
      {"stratified",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::Sampler& sampler) {
         RemoveStratifiedSamplerV1(parameters, *sampler.mutable_stratified());
         return absl::OkStatus();
       }}};

  auto& sampler = *output_.add_directives()->mutable_sampler();

  auto iter = kSupportedTypes.find(sampler_type);
  if (iter == kSupportedTypes.end()) {
    return UnrecognizedTypeError("Sampler", sampler_type);
  }

  return iter->second(parameters, sampler);
}

absl::Status ParserV1::Shape(
    absl::string_view shape_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v1::Shape> kSupportedTypes = {
      {"cone",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::Shape& shape) {
         RemoveConeShapeV1(parameters, *shape.mutable_cone());
         return absl::OkStatus();
       }},
      {"cylinder",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::Shape& shape) {
         RemoveCylinderShapeV1(parameters, *shape.mutable_cylinder());
         return absl::OkStatus();
       }},
      {"disk",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::Shape& shape) {
         RemoveDiskShapeV1(parameters, *shape.mutable_disk());
         return absl::OkStatus();
       }},
      {"heightfield",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::Shape& shape) {
         return RemoveHeightFieldShapeV1(parameters,
                                         *shape.mutable_heightfield());
       }},
      {"hyperboloid",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::Shape& shape) {
         RemoveHyperboloidShapeV1(parameters, *shape.mutable_hyperboloid());
         return absl::OkStatus();
       }},
      {"loopsubdiv",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::Shape& shape) {
         return RemoveLoopSubdivShapeV1(parameters,
                                        *shape.mutable_loopsubdiv());
       }},
      {"nurbs",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::Shape& shape) {
         return RemoveNurbsShapeV1(parameters, *shape.mutable_nurbs());
       }},
      {"paraboloid",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::Shape& shape) {
         RemoveParaboloidShapeV1(parameters, *shape.mutable_paraboloid());
         return absl::OkStatus();
       }},
      {"sphere",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::Shape& shape) {
         RemoveSphereShapeV1(parameters, *shape.mutable_sphere());
         return absl::OkStatus();
       }},
      {"trianglemesh",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::Shape& shape) {
         return RemoveTriangleMeshShapeV1(parameters,
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
      parameters, "bumpmap",
      std::bind(&Shape::MaterialOverrides::mutable_bumpmap, overrides));
  TryRemoveFloatTexture(
      parameters, "eta",
      std::bind(&Shape::MaterialOverrides::mutable_eta, overrides));
  TryRemoveFloatTexture(
      parameters, "roughness",
      std::bind(&Shape::MaterialOverrides::mutable_roughness, overrides));
  TryRemoveFloatTexture(
      parameters, "sigma",
      std::bind(&Shape::MaterialOverrides::mutable_sigma, overrides));
  TryRemoveFloatTexture(
      parameters, "uroughness",
      std::bind(&Shape::MaterialOverrides::mutable_uroughness, overrides));
  TryRemoveFloatTexture(
      parameters, "vroughness",
      std::bind(&Shape::MaterialOverrides::mutable_vroughness, overrides));
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
      parameters, "opacity",
      std::bind(&Shape::MaterialOverrides::mutable_opacity, overrides));
  TryRemoveSpectrumTextureV1(
      parameters, "reflect",
      std::bind(&Shape::MaterialOverrides::mutable_reflect, overrides));
  TryRemoveSpectrumTextureV1(
      parameters, "transmit",
      std::bind(&Shape::MaterialOverrides::mutable_transmit, overrides));

  return absl::OkStatus();
}

absl::Status ParserV1::SpectrumTexture(
    absl::string_view spectrum_texture_name,
    absl::string_view spectrum_texture_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v1::SpectrumTexture> kSupportedTypes = {
      {"bilerp",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::SpectrumTexture& spectrum_texture) {
         RemoveBilerpSpectrumTextureV1(parameters,
                                       *spectrum_texture.mutable_bilerp());
         return absl::OkStatus();
       }},
      {"checkerboard",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::SpectrumTexture& spectrum_texture) {
         int32_t dimension =
             TryRemoveInteger(parameters, "dimension").value_or(2);

         if (dimension == 2) {
           RemoveCheckerboard2DSpectrumTextureV1(
               parameters, *spectrum_texture.mutable_checkerboard2d());
         } else {
           RemoveCheckerboard3DSpectrumTextureV1(
               parameters, *spectrum_texture.mutable_checkerboard3d());
         }

         return absl::OkStatus();
       }},
      {"constant",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::SpectrumTexture& spectrum_texture) {
         return RemoveConstantSpectrumTextureV1(
             parameters, *spectrum_texture.mutable_constant());
       }},
      {"dots",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::SpectrumTexture& spectrum_texture) {
         RemoveDotsSpectrumTextureV1(parameters,
                                     *spectrum_texture.mutable_dots());
         return absl::OkStatus();
       }},
      {"fbm",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::SpectrumTexture& spectrum_texture) {
         RemoveDotsSpectrumTextureV1(parameters,
                                     *spectrum_texture.mutable_dots());
         return absl::OkStatus();
       }},
      {"imagemap",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::SpectrumTexture& spectrum_texture) {
         RemoveImageMapSpectrumTextureV1(parameters,
                                         *spectrum_texture.mutable_imagemap());
         return absl::OkStatus();
       }},
      {"marble",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::SpectrumTexture& spectrum_texture) {
         RemoveMarbleSpectrumTextureV1(parameters,
                                       *spectrum_texture.mutable_marble());
         return absl::OkStatus();
       }},
      {"mix",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::SpectrumTexture& spectrum_texture) {
         RemoveMarbleSpectrumTextureV1(parameters,
                                       *spectrum_texture.mutable_marble());
         return absl::OkStatus();
       }},
      {"scale",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::SpectrumTexture& spectrum_texture) {
         RemoveScaleSpectrumTextureV1(parameters,
                                      *spectrum_texture.mutable_scale());
         return absl::OkStatus();
       }},
      {"uv",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::SpectrumTexture& spectrum_texture) {
         RemoveUvSpectrumTextureV1(parameters, *spectrum_texture.mutable_uv());
         return absl::OkStatus();
       }},
      {"windy",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::SpectrumTexture& spectrum_texture) {
         spectrum_texture.mutable_windy();
         return absl::OkStatus();
       }},
      {"wrinkled",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::SpectrumTexture& spectrum_texture) {
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

absl::Status ParserV1::SurfaceIntegrator(
    absl::string_view integrator_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v1::SurfaceIntegrator> kSupportedTypes = {
      {"bidirectional",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::SurfaceIntegrator& integrator) {
         RemoveBdptIntegratorV1(parameters,
                                *integrator.mutable_bidirectional());
         return absl::OkStatus();
       }},
      {"debug",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::SurfaceIntegrator& integrator) {
         RemoveDebugIntegratorV1(parameters, *integrator.mutable_debug());
         return absl::OkStatus();
       }},
      {"directlighting",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::SurfaceIntegrator& integrator) {
         RemoveDirectLightingIntegratorV1(parameters,
                                          *integrator.mutable_directlighting());
         return absl::OkStatus();
       }},
      {"exphotonmap",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::SurfaceIntegrator& integrator) {
         RemoveExPhotonMapIntegratorV1(parameters,
                                       *integrator.mutable_exphotonmap());
         return absl::OkStatus();
       }},
      {"igi",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::SurfaceIntegrator& integrator) {
         RemoveIgiIntegratorV1(parameters, *integrator.mutable_igi());
         return absl::OkStatus();
       }},
      {"irradiancecache",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::SurfaceIntegrator& integrator) {
         RemoveIrradianceCacheIntegratorV1(
             parameters, *integrator.mutable_irradiancecache());
         return absl::OkStatus();
       }},
      {"path",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::SurfaceIntegrator& integrator) {
         RemovePathIntegratorV1(parameters, *integrator.mutable_path());
         return absl::OkStatus();
       }},
      {"photonmap",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::SurfaceIntegrator& integrator) {
         RemovePhotonMapIntegratorV1(parameters,
                                     *integrator.mutable_photonmap());
         return absl::OkStatus();
       }},
      {"whitted",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::SurfaceIntegrator& integrator) {
         RemoveWhittedIntegratorV1(parameters, *integrator.mutable_whitted());
         return absl::OkStatus();
       }}};

  auto& integrator = *output_.add_directives()->mutable_surface_integrator();

  auto iter = kSupportedTypes.find(integrator_type);
  if (iter == kSupportedTypes.end()) {
    return UnrecognizedTypeError("SurfaceIntegrator", integrator_type);
  }

  return iter->second(parameters, integrator);
}

absl::Status ParserV1::Volume(
    absl::string_view volume_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v1::Volume> kSupportedTypes = {
      {"exponential",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::Volume& volume) {
         return RemoveExponentialMediumV1(parameters,
                                          *volume.mutable_exponential());
       }},
      {"homogeneous",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::Volume& volume) {
         return RemoveHomogeneousMediumV1(parameters,
                                          *volume.mutable_homogeneous());
       }},
      {"volumegrid",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::Volume& volume) {
         return RemoveUniformGridMediumV1(parameters,
                                          *volume.mutable_volumegrid());
       }}};

  auto& volume = *output_.add_directives()->mutable_volume();

  auto iter = kSupportedTypes.find(volume_type);
  if (iter == kSupportedTypes.end()) {
    return UnrecognizedTypeError("Volume", volume_type);
  }

  return iter->second(parameters, volume);
}

absl::Status ParserV1::VolumeIntegrator(
    absl::string_view integrator_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v1::VolumeIntegrator> kSupportedTypes = {
      {"emission",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::VolumeIntegrator& integrator) {
         RemoveEmissionVolumeIntegratorV1(parameters,
                                          *integrator.mutable_emission());
         return absl::OkStatus();
       }},
      {"single",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::VolumeIntegrator& integrator) {
         RemoveSingleVolumeIntegratorV1(parameters,
                                        *integrator.mutable_single());
         return absl::OkStatus();
       }}};

  auto& integrator = *output_.add_directives()->mutable_volume_integrator();

  auto iter = kSupportedTypes.find(integrator_type);
  if (iter == kSupportedTypes.end()) {
    return UnrecognizedTypeError("VolumeIntegrator", integrator_type);
  }

  return iter->second(parameters, integrator);
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
