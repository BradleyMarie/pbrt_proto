#include "pbrt_proto/v1/convert.h"

#include <functional>
#include <iostream>
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
    kParameterTypeNames = {
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
      : ProtoParser(kParameterTypeNames, output) {}

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
      {"bilerp", CB<RemoveBilerpFloatTexture, &FloatTexture::mutable_bilerp>()},
      {"checkerboard",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::FloatTexture& float_texture) {
         int32_t dimension =
             TryRemoveInteger(parameters, "dimension").value_or(2);

         if (dimension == 2) {
           return RemoveCheckerboard2DFloatTexture(
               parameters, kPbrtVersion,
               *float_texture.mutable_checkerboard2d());
         }

         if (dimension == 3) {
           return RemoveCheckerboard3DFloatTexture(
               parameters, kPbrtVersion,
               *float_texture.mutable_checkerboard3d());
         }

         std::cerr << "WARNING: Unsupported value for 'checkerboard' Texture "
                      "parameter 'dimension': "
                   << dimension << std::endl;

         return absl::OkStatus();
       }},
      {"constant",
       CB<RemoveConstantFloatTexture, &FloatTexture::mutable_constant>()},
      {"dots", CB<RemoveDotsFloatTexture, &FloatTexture::mutable_dots>()},
      {"fbm", CB<RemoveFBmFloatTexture, &FloatTexture::mutable_fbm>()},
      {"imagemap",
       CB<RemoveImageMapFloatTexture, &FloatTexture::mutable_imagemap>()},
      {"marble", &Ignored<v1::FloatTexture>},
      {"mix", CB<RemoveMixFloatTexture, &FloatTexture::mutable_mix>()},
      {"scale", CB<RemoveScaleFloatTexture, &FloatTexture::mutable_scale>()},
      {"windy", EmptyCallback<&FloatTexture::mutable_windy>()},
      {"wrinkled",
       CB<RemoveWrinkledFloatTexture, &FloatTexture::mutable_wrinkled>()},
  };

  absl::Status status = Parse<&Directive::mutable_float_texture>(
      kSupportedTypes, float_texture_type, parameters);

  // No need to check status. The directive is always added by Parse.
  output_.mutable_directives()->rbegin()->mutable_float_texture()->set_name(
      float_texture_name);

  return absl::OkStatus();
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
      {"bluepaint", CB<RemoveBuiltInMaterial, &Material::mutable_bluepaint>()},
      {"brushedmetal",
       CB<RemoveBuiltInMaterial, &Material::mutable_brushedmetal>()},
      {"clay", CB<RemoveBuiltInMaterial, &Material::mutable_clay>()},
      {"felt", CB<RemoveBuiltInMaterial, &Material::mutable_felt>()},
      {"glass", CB<RemoveGlassMaterial, &Material::mutable_glass>()},
      {"matte", CB<RemoveMatteMaterial, &Material::mutable_matte>()},
      {"mirror", CB<RemoveMirrorMaterial, &Material::mutable_mirror>()},
      {"plastic", CB<RemovePlasticMaterial, &Material::mutable_plastic>()},
      {"primer", CB<RemoveBuiltInMaterial, &Material::mutable_primer>()},
      {"shinymetal",
       CB<RemoveShinyMetalMaterial, &Material::mutable_shinymetal>()},
      {"skin", CB<RemoveBuiltInMaterial, &Material::mutable_skin>()},
      {"substrate",
       CB<RemoveSubstrateMaterial, &Material::mutable_substrate>()},
      {"translucent",
       CB<RemoveTranslucentMaterial, &Material::mutable_translucent>()},
      {"uber", CB<RemoveUberMaterial, &Material::mutable_uber>()},
  };

  absl::Status status =
      Parse(kSupportedTypes, material_type, parameters, material);

  if (material.material_type_case() == Material::MATERIAL_TYPE_NOT_SET &&
      !kSupportedTypes.contains(material_type)) {
    if (absl::Status fallback_status = RemoveMatteMaterial(
            parameters, kPbrtVersion, *material.mutable_matte());
        !fallback_status.ok()) {
      return fallback_status;
    }
  }

  return status;
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
       CB<RemoveBestCandidateSampler, &Sampler::mutable_bestcandidate>()},
      {"lowdiscrepancy",
       CB<RemoveZeroTwoSequenceSampler, &Sampler::mutable_lowdiscrepancy>()},
      {"random", CB<RemoveRandomSampler, &Sampler::mutable_random>()},
      {"stratified",
       CB<RemoveStratifiedSampler, &Sampler::mutable_stratified>()},
  };

  return Parse<&Directive::mutable_sampler>(kSupportedTypes, sampler_type,
                                            parameters);
}

absl::Status ParserV1::Shape(
    absl::string_view shape_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v1::Shape> kSupportedTypes = {
      {"cone", CB<RemoveConeShape, &Shape::mutable_cone>()},
      {"cylinder", CB<RemoveCylinderShape, &Shape::mutable_cylinder>()},
      {"disk", CB<RemoveDiskShape, &Shape::mutable_disk>()},
      {"heightfield",
       CB<RemoveHeightFieldShape, &Shape::mutable_heightfield>()},
      {"hyperboloid",
       CB<RemoveHyperboloidShape, &Shape::mutable_hyperboloid>()},
      {"loopsubdiv", CB<RemoveLoopSubdivShape, &Shape::mutable_loopsubdiv>()},
      {"nurbs", CB<RemoveNurbsShape, &Shape::mutable_nurbs>()},
      {"paraboloid", CB<RemoveParaboloidShape, &Shape::mutable_paraboloid>()},
      {"sphere", CB<RemoveSphereShape, &Shape::mutable_sphere>()},
      {"trianglemesh",
       CB<RemoveTriangleMeshShape, &Shape::mutable_trianglemesh>()},
  };

  if (absl::Status status = Parse<&Directive::mutable_shape>(
          kSupportedTypes, shape_type, parameters);
      !status.ok()) {
    return status;
  }

  // No need to check status. The directive is always added by Parse.
  auto& shape = *output_.mutable_directives()->rbegin()->mutable_shape();
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

  if (absl::Status status = TryRemoveSpectrumTexture(
          parameters, "Kd",
          std::bind(&Shape::MaterialOverrides::mutable_kd, overrides));
      !status.ok()) {
    return status;
  }

  if (absl::Status status = TryRemoveSpectrumTexture(
          parameters, "Kr",
          std::bind(&Shape::MaterialOverrides::mutable_kr, overrides));
      !status.ok()) {
    return status;
  }

  if (absl::Status status = TryRemoveSpectrumTexture(
          parameters, "Ks",
          std::bind(&Shape::MaterialOverrides::mutable_ks, overrides));
      !status.ok()) {
    return status;
  }

  if (absl::Status status = TryRemoveSpectrumTexture(
          parameters, "Kt",
          std::bind(&Shape::MaterialOverrides::mutable_kt, overrides));
      !status.ok()) {
    return status;
  }

  if (absl::Status status = TryRemoveSpectrumTexture(
          parameters, "opacity",
          std::bind(&Shape::MaterialOverrides::mutable_opacity, overrides));
      !status.ok()) {
    return status;
  }

  if (absl::Status status = TryRemoveSpectrumTexture(
          parameters, "reflect",
          std::bind(&Shape::MaterialOverrides::mutable_reflect, overrides));
      !status.ok()) {
    return status;
  }

  if (absl::Status status = TryRemoveSpectrumTexture(
          parameters, "transmit",
          std::bind(&Shape::MaterialOverrides::mutable_transmit, overrides));
      !status.ok()) {
    return status;
  }

  return absl::OkStatus();
}

absl::Status ParserV1::SpectrumTexture(
    absl::string_view spectrum_texture_name,
    absl::string_view spectrum_texture_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v1::SpectrumTexture> kSupportedTypes = {
      {"bilerp",
       CB<RemoveBilerpSpectrumTexture, &SpectrumTexture::mutable_bilerp>()},
      {"checkerboard",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v1::SpectrumTexture& spectrum_texture) {
         int32_t dimension =
             TryRemoveInteger(parameters, "dimension").value_or(2);

         if (dimension == 2) {
           return RemoveCheckerboard2DSpectrumTexture(
               parameters, kPbrtVersion,
               *spectrum_texture.mutable_checkerboard2d());
         }

         if (dimension == 3) {
           return RemoveCheckerboard3DSpectrumTexture(
               parameters, kPbrtVersion,
               *spectrum_texture.mutable_checkerboard3d());
         }

         std::cerr << "WARNING: Unsupported value for 'checkerboard' Texture "
                      "parameter 'dimension': "
                   << dimension << std::endl;

         return absl::OkStatus();
       }},
      {"constant",
       CB<RemoveConstantSpectrumTexture, &SpectrumTexture::mutable_constant>()},
      {"dots", CB<RemoveDotsSpectrumTexture, &SpectrumTexture::mutable_dots>()},
      {"fbm", CB<RemoveFBmSpectrumTexture, &SpectrumTexture::mutable_fbm>()},
      {"imagemap",
       CB<RemoveImageMapSpectrumTexture, &SpectrumTexture::mutable_imagemap>()},
      {"marble",
       CB<RemoveMarbleSpectrumTexture, &SpectrumTexture::mutable_marble>()},
      {"mix", CB<RemoveMixSpectrumTexture, &SpectrumTexture::mutable_mix>()},
      {"scale",
       CB<RemoveScaleSpectrumTexture, &SpectrumTexture::mutable_scale>()},
      {"uv", CB<RemoveUvSpectrumTexture, &SpectrumTexture::mutable_uv>()},
      {"windy", EmptyCallback<&SpectrumTexture::mutable_windy>()},
      {"wrinkled",
       CB<RemoveWrinkledSpectrumTexture, &SpectrumTexture::mutable_wrinkled>()},
  };

  absl::Status status = Parse<&Directive::mutable_spectrum_texture>(
      kSupportedTypes, spectrum_texture_type, parameters);

  // No need to check status. The directive is always added by Parse.
  output_.mutable_directives()->rbegin()->mutable_spectrum_texture()->set_name(
      spectrum_texture_name);

  return absl::OkStatus();
}

absl::Status ParserV1::SurfaceIntegrator(
    absl::string_view integrator_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v1::SurfaceIntegrator> kSupportedTypes = {
      {"bidirectional",
       CB<RemoveBdptIntegrator, &SurfaceIntegrator::mutable_bidirectional>()},
      {"debug", CB<RemoveDebugIntegrator, &SurfaceIntegrator::mutable_debug>()},
      {"directlighting", CB<RemoveDirectLightingIntegrator,
                            &SurfaceIntegrator::mutable_directlighting>()},
      {"exphotonmap", CB<RemoveExPhotonMapIntegrator,
                         &SurfaceIntegrator::mutable_exphotonmap>()},
      {"igi", CB<RemoveIgiIntegrator, &SurfaceIntegrator::mutable_igi>()},
      {"irradiancecache", CB<RemoveIrradianceCacheIntegrator,
                             &SurfaceIntegrator::mutable_irradiancecache>()},
      {"path", CB<RemovePathIntegrator, &SurfaceIntegrator::mutable_path>()},
      {"photonmap",
       CB<RemovePhotonMapIntegrator, &SurfaceIntegrator::mutable_photonmap>()},
      {"whitted",
       CB<RemoveWhittedIntegrator, &SurfaceIntegrator::mutable_whitted>()},
  };

  return Parse<&Directive::mutable_surface_integrator>(
      kSupportedTypes, integrator_type, parameters);
}

absl::Status ParserV1::Volume(
    absl::string_view volume_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v1::Volume> kSupportedTypes = {
      {"exponential",
       CB<RemoveExponentialMedium, &Volume::mutable_exponential>()},
      {"homogeneous",
       CB<RemoveHomogeneousMedium, &Volume::mutable_homogeneous>()},
      {"volumegrid",
       CB<RemoveUniformGridMedium, &Volume::mutable_volumegrid>()},
  };

  return Parse<&Directive::mutable_volume>(kSupportedTypes, volume_type,
                                           parameters);
}

absl::Status ParserV1::VolumeIntegrator(
    absl::string_view integrator_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v1::VolumeIntegrator> kSupportedTypes = {
      {"emission", CB<RemoveEmissionVolumeIntegrator,
                      &VolumeIntegrator::mutable_emission>()},
      {"single",
       CB<RemoveSingleVolumeIntegrator, &VolumeIntegrator::mutable_single>()},
  };

  return Parse<&Directive::mutable_volume_integrator>(
      kSupportedTypes, integrator_type, parameters);
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
