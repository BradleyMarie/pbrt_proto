#include "pbrt_proto/v2/convert.h"

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
#include "pbrt_proto/shared/enums.h"
#include "pbrt_proto/shared/films.h"
#include "pbrt_proto/shared/integrators.h"
#include "pbrt_proto/shared/light_sources.h"
#include "pbrt_proto/shared/materials.h"
#include "pbrt_proto/shared/media.h"
#include "pbrt_proto/shared/parser.h"
#include "pbrt_proto/shared/pixel_filters.h"
#include "pbrt_proto/shared/proto_parser.h"
#include "pbrt_proto/shared/renderers.h"
#include "pbrt_proto/shared/samplers.h"
#include "pbrt_proto/shared/shapes.h"
#include "pbrt_proto/shared/textures.h"
#include "pbrt_proto/v2/v2.pb.h"

namespace pbrt_proto::v2 {
namespace {

static const absl::flat_hash_map<absl::string_view, ParameterType>
    kParameterTypeNames = {
        {"blackbody", ParameterType::BLACKBODY_V1},
        {"bool", ParameterType::BOOL},
        {"color", ParameterType::RGB},
        {"float", ParameterType::FLOAT},
        {"integer", ParameterType::INTEGER},
        {"normal", ParameterType::NORMAL3},
        {"point", ParameterType::POINT3},
        {"rgb", ParameterType::RGB},
        {"spectrum", ParameterType::SPECTRUM},
        {"string", ParameterType::STRING},
        {"texture", ParameterType::TEXTURE},
        {"vector", ParameterType::VECTOR3},
        {"xyz", ParameterType::XYZ},
};

class ParserV2 final : public ProtoParser<PbrtProto, 2> {
 public:
  ParserV2(PbrtProto& output) noexcept
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
      v2::Material& material) override;

  absl::Status PixelFilter(
      absl::string_view filter_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status Renderer(
      absl::string_view renderer_type,
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

absl::Status ParserV2::Accelerator(
    absl::string_view accelerator_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v2::Accelerator> kSupportedTypes = {
      {"bvh", CB<RemoveBvhAccelerator, &Accelerator::mutable_bvh>()},
      {"grid", CB<RemoveGridAccelerator, &Accelerator::mutable_grid>()},
      {"kdtree", CB<RemoveKdTreeAccelerator, &Accelerator::mutable_kdtree>()},
  };

  return Parse<&Directive::mutable_accelerator>(kSupportedTypes,
                                                accelerator_type, parameters);
}

absl::Status ParserV2::AreaLightSource(
    absl::string_view area_light_source_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v2::AreaLightSource> kSupportedTypes = {
      {"area",
       CB<RemoveDiffuseAreaLightSource, &AreaLightSource::mutable_diffuse>()},
      {"diffuse",
       CB<RemoveDiffuseAreaLightSource, &AreaLightSource::mutable_diffuse>()},
  };

  return Parse<&Directive::mutable_area_light_source>(
      kSupportedTypes, area_light_source_type, parameters);
}

absl::Status ParserV2::Camera(
    absl::string_view camera_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v2::Camera> kSupportedTypes = {
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

absl::Status ParserV2::Film(
    absl::string_view film_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v2::Film> kSupportedTypes = {
      {"image", CB<RemoveRgbFilm, &Film::mutable_image>()},
  };

  return Parse<&Directive::mutable_film>(kSupportedTypes, film_type,
                                         parameters);
}

absl::Status ParserV2::FloatTexture(
    absl::string_view float_texture_name, absl::string_view float_texture_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v2::FloatTexture> kSupportedTypes = {
      {"bilerp", CB<RemoveBilerpFloatTexture, &FloatTexture::mutable_bilerp>()},
      {"checkerboard",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v2::FloatTexture& float_texture) {
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
      {"marble", &Ignored<v2::FloatTexture>},
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

absl::Status ParserV2::LightSource(
    absl::string_view light_source_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v2::LightSource> kSupportedTypes = {
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

absl::Status ParserV2::Material(
    absl::string_view material_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    v2::Material& material) {
  static const TypeMap<v2::Material> kSupportedTypes = {
      {"glass", CB<RemoveGlassMaterial, &Material::mutable_glass>()},
      {"kdsubsurface",
       CB<RemoveKdSubsurfaceMaterial, &Material::mutable_kdsubsurface>()},
      {"matte", CB<RemoveMatteMaterial, &Material::mutable_matte>()},
      {"measured",
       CB<RemoveMeasuredMerlMaterial, &Material::mutable_measured>()},
      {"metal", CB<RemoveMetalMaterial, &Material::mutable_metal>()},
      {"mirror", CB<RemoveMirrorMaterial, &Material::mutable_mirror>()},
      {"mix", CB<RemoveMixMaterial, &Material::mutable_mix>()},
      {"plastic", CB<RemovePlasticMaterial, &Material::mutable_plastic>()},
      {"shinymetal",
       CB<RemoveShinyMetalMaterial, &Material::mutable_shinymetal>()},
      {"substrate",
       CB<RemoveSubstrateMaterial, &Material::mutable_substrate>()},
      {"subsurface",
       CB<RemoveSubsurfaceMaterial, &Material::mutable_subsurface>()},
      {"translucent",
       CB<RemoveTranslucentMaterial, &Material::mutable_translucent>()},
      {"uber", CB<RemoveUberMaterial, &Material::mutable_uber>()},
  };

  return Parse(kSupportedTypes, material_type, parameters, material);
}

absl::Status ParserV2::PixelFilter(
    absl::string_view pixel_filter_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v2::PixelFilter> kSupportedTypes = {
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

absl::Status ParserV2::Renderer(
    absl::string_view renderer_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v2::Renderer> kSupportedTypes = {
      {"aggregatetest",
       CB<RemoveAggregateTestRenderer, &Renderer::mutable_aggregatetest>()},
      {"createprobes",
       CB<RemoveCreateProbesRenderer, &Renderer::mutable_createprobes>()},
      {"metropolis",
       CB<RemoveMetropolisRenderer, &Renderer::mutable_metropolis>()},
      {"sampler", CB<RemoveSamplerRenderer, &Renderer::mutable_sampler>()},
      {"surfacepoints",
       CB<RemoveSurfacePointsRenderer, &Renderer::mutable_surfacepoints>()},
  };

  return Parse<&Directive::mutable_renderer>(kSupportedTypes, renderer_type,
                                             parameters);
}

absl::Status ParserV2::Sampler(
    absl::string_view sampler_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v2::Sampler> kSupportedTypes = {
      {"adaptive", CB<RemoveAdaptiveSampler, &Sampler::mutable_adaptive>()},
      {"bestcandidate",
       CB<RemoveBestCandidateSampler, &Sampler::mutable_bestcandidate>()},
      {"halton", CB<RemoveHaltonSampler, &Sampler::mutable_halton>()},
      {"lowdiscrepancy",
       CB<RemoveZeroTwoSequenceSampler, &Sampler::mutable_lowdiscrepancy>()},
      {"random", CB<RemoveIndependentSampler, &Sampler::mutable_random>()},
      {"stratified",
       CB<RemoveStratifiedSampler, &Sampler::mutable_stratified>()},
  };

  return Parse<&Directive::mutable_sampler>(kSupportedTypes, sampler_type,
                                            parameters);
}

absl::Status ParserV2::Shape(
    absl::string_view shape_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v2::Shape> kSupportedTypes = {
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
      parameters, "vroughness",
      std::bind(&Shape::MaterialOverrides::mutable_vroughness, overrides));
  TryRemoveFloatTexture(
      parameters, "meanfreepath",
      std::bind(&Shape::MaterialOverrides::mutable_meanfreepath, overrides));
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
          parameters, "amount",
          std::bind(&Shape::MaterialOverrides::mutable_amount, overrides));
      !status.ok()) {
    return status;
  }

  if (absl::Status status = TryRemoveSpectrumTexture(
          parameters, "k",
          std::bind(&Shape::MaterialOverrides::mutable_k, overrides));
      !status.ok()) {
    return status;
  }

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
          parameters, "sigma_a",
          std::bind(&Shape::MaterialOverrides::mutable_sigma_a, overrides));
      !status.ok()) {
    return status;
  }

  if (absl::Status status = TryRemoveSpectrumTexture(
          parameters, "sigma_prime_s",
          std::bind(&Shape::MaterialOverrides::mutable_sigma_s, overrides));
      !status.ok()) {
    return status;
  }

  if (absl::Status status = TryRemoveSpectrumTexture(
          parameters, "transmit",
          std::bind(&Shape::MaterialOverrides::mutable_transmit, overrides));
      !status.ok()) {
    return status;
  }

  if (absl::Status status =
          RemoveEnum(parameters, /*pbrt_version=*/2, shape_type, "name",
                     std::bind(&Shape::MaterialOverrides::set_name, overrides,
                               std::placeholders::_1),
                     ScatteringPreset::DEFAULT);
      !status.ok()) {
    return status;
  }

  if (std::optional<absl::string_view> filename =
          TryRemoveString(parameters, "filename");
      filename.has_value()) {
    shape.mutable_overrides()->set_filename(*filename);
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
    shape.mutable_overrides()->mutable_eta()->set_float_value(*eta);
  } else if (std::optional<absl::string_view> eta =
                 TryRemoveTexture(parameters, "eta");
             eta.has_value()) {
    shape.mutable_overrides()->mutable_eta()->set_float_texture_name(*eta);
    shape.mutable_overrides()
        ->mutable_eta_as_spectrum_texture()
        ->set_spectrum_texture_name(*eta);
  } else {
    if (absl::Status status = TryRemoveSpectrumTexture(
            parameters, "eta",
            std::bind(
                &Shape::MaterialOverrides::mutable_eta_as_spectrum_texture,
                overrides));
        !status.ok()) {
      return status;
    }
  }

  return absl::OkStatus();
}

absl::Status ParserV2::SpectrumTexture(
    absl::string_view spectrum_texture_name,
    absl::string_view spectrum_texture_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v2::SpectrumTexture> kSupportedTypes = {
      {"bilerp",
       CB<RemoveBilerpSpectrumTexture, &SpectrumTexture::mutable_bilerp>()},
      {"checkerboard",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v2::SpectrumTexture& spectrum_texture) {
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

absl::Status ParserV2::SurfaceIntegrator(
    absl::string_view integrator_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v2::SurfaceIntegrator> kSupportedTypes = {
      {"ambientocclusion", CB<RemoveAmbientOcclusionIntegrator,
                              &SurfaceIntegrator::mutable_ambientocclusion>()},
      {"diffuseprt", CB<RemoveDiffusePrtIntegrator,
                        &SurfaceIntegrator::mutable_diffuseprt>()},
      {"dipolesubsurface", CB<RemoveDipoleSubsurfaceIntegrator,
                              &SurfaceIntegrator::mutable_dipolesubsurface>()},
      {"directlighting", CB<RemoveDirectLightingIntegrator,
                            &SurfaceIntegrator::mutable_directlighting>()},
      {"glossyprt",
       CB<RemoveGlossyPrtIntegrator, &SurfaceIntegrator::mutable_glossyprt>()},
      {"igi", CB<RemoveIgiIntegrator, &SurfaceIntegrator::mutable_igi>()},
      {"irradiancecache", CB<RemoveIrradianceCacheIntegrator,
                             &SurfaceIntegrator::mutable_irradiancecache>()},
      {"path", CB<RemovePathIntegrator, &SurfaceIntegrator::mutable_path>()},
      {"photonmap",
       CB<RemovePhotonMapIntegrator, &SurfaceIntegrator::mutable_photonmap>()},
      {"useprobes",
       CB<RemoveUseProbesIntegrator, &SurfaceIntegrator::mutable_useprobes>()},
      {"whitted",
       CB<RemoveWhittedIntegrator, &SurfaceIntegrator::mutable_whitted>()},
  };

  return Parse<&Directive::mutable_surface_integrator>(
      kSupportedTypes, integrator_type, parameters);
}

absl::Status ParserV2::Volume(
    absl::string_view volume_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v2::Volume> kSupportedTypes = {
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

absl::Status ParserV2::VolumeIntegrator(
    absl::string_view integrator_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v2::VolumeIntegrator> kSupportedTypes = {
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
  return ParserV2(output).ReadFrom(input);
}

absl::StatusOr<PbrtProto> Convert(std::istream& input) {
  PbrtProto output;
  if (absl::Status error = Convert(input, output); !error.ok()) {
    return error;
  }
  return output;
}

}  // namespace pbrt_proto::v2
