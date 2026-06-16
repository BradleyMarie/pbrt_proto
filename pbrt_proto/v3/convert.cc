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
    kParameterTypeNames = {
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
      {"bilerp", CB<RemoveBilerpFloatTexture, &FloatTexture::mutable_bilerp>()},
      {"checkerboard",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::FloatTexture& float_texture) {
         int32_t dimension =
             TryRemoveInteger(parameters, "dimension").value_or(2);

         if (dimension == 2) {
           return RemoveCheckerboard2DFloatTexture(
               parameters, kPbrtVersion,
               *float_texture.mutable_checkerboard2d());
         }

         if (dimension != 3) {
           std::cerr << "WARNING: Unsupported value for 'checkerboard' Texture "
                        "parameter 'dimension': "
                     << dimension << std::endl;
         }

         return RemoveCheckerboard3DFloatTexture(
             parameters, kPbrtVersion, *float_texture.mutable_checkerboard3d());
       }},
      {"constant",
       CB<RemoveConstantFloatTexture, &FloatTexture::mutable_constant>()},
      {"dots", CB<RemoveDotsFloatTexture, &FloatTexture::mutable_dots>()},
      {"fbm", CB<RemoveFBmFloatTexture, &FloatTexture::mutable_fbm>()},
      {"imagemap",
       CB<RemoveImageMapFloatTexture, &FloatTexture::mutable_imagemap>()},
      {"marble", &Ignored<v3::FloatTexture>},
      {"mix", CB<RemoveMixFloatTexture, &FloatTexture::mutable_mix>()},
      {"ptex", CB<RemovePtexFloatTexture, &FloatTexture::mutable_ptex>()},
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

absl::Status ParserV3::Integrator(
    absl::string_view integrator_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v3::Integrator> kSupportedTypes = {
      {"ambientocclusion", CB<RemoveAmbientOcclusionIntegrator,
                              &Integrator::mutable_ambientocclusion>()},
      {"bdpt", CB<RemoveBdptIntegrator, &Integrator::mutable_bdpt>()},
      {"directlighting", CB<RemoveDirectLightingIntegrator,
                            &Integrator::mutable_directlighting>()},
      {"mlt", CB<RemoveMltIntegrator, &Integrator::mutable_mlt>()},
      {"path", CB<RemovePathIntegrator, &Integrator::mutable_path>()},
      {"sppm", CB<RemoveSppmIntegrator, &Integrator::mutable_sppm>()},
      {"volpath", CB<RemoveVolPathIntegrator, &Integrator::mutable_volpath>()},
      {"whitted", CB<RemoveWhittedIntegrator, &Integrator::mutable_whitted>()},
  };

  return Parse<&Directive::mutable_integrator>(kSupportedTypes, integrator_type,
                                               parameters);
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
      {"heterogeneous",
       CB<RemoveUniformGridMedium, &MakeNamedMedium::mutable_heterogeneous>()},
      {"homogeneous",
       CB<RemoveHomogeneousMedium, &MakeNamedMedium::mutable_homogeneous>()},
  };

  absl::Status status = Parse<&Directive::mutable_make_named_medium>(
      kSupportedTypes, TryRemoveString(parameters, "type").value_or(""),
      parameters);

  // No need to check status. The directive is always added by Parse.
  output_.mutable_directives()->rbegin()->mutable_make_named_medium()->set_name(
      medium_name);

  return absl::OkStatus();
}

absl::Status ParserV3::Material(
    absl::string_view material_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    v3::Material& material) {
  static const TypeMap<v3::Material> kSupportedTypes = {
      {"", &Ignored<v3::Material>},
      {"disney", CB<RemoveDisneyMaterial, &Material::mutable_disney>()},
      {"fourier",
       CB<RemoveMeasuredFourierMaterial, &Material::mutable_fourier>()},
      {"glass", CB<RemoveGlassMaterial, &Material::mutable_glass>()},
      {"hair", CB<RemoveHairMaterial, &Material::mutable_hair>()},
      {"kdsubsurface",
       CB<RemoveKdSubsurfaceMaterial, &Material::mutable_kdsubsurface>()},
      {"matte", CB<RemoveMatteMaterial, &Material::mutable_matte>()},
      {"metal", CB<RemoveMetalMaterial, &Material::mutable_metal>()},
      {"mirror", CB<RemoveMirrorMaterial, &Material::mutable_mirror>()},
      {"mix", CB<RemoveMixMaterial, &Material::mutable_mix>()},
      {"none", &Ignored<v3::Material>},
      {"plastic", CB<RemovePlasticMaterial, &Material::mutable_plastic>()},
      {"substrate",
       CB<RemoveSubstrateMaterial, &Material::mutable_substrate>()},
      {"subsurface",
       CB<RemoveSubsurfaceMaterial, &Material::mutable_subsurface>()},
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
      {"halton", CB<RemoveHaltonSampler, &Sampler::mutable_halton>()},
      {"lowdiscrepancy",
       CB<RemoveZeroTwoSequenceSampler, &Sampler::mutable_zerotwosequence>()},
      {"maxmindist",
       CB<RemoveMaxMinDistSampler, &Sampler::mutable_maxmindist>()},
      {"random", CB<RemoveIndependentSampler, &Sampler::mutable_random>()},
      {"sobol", CB<RemoveSobolSampler, &Sampler::mutable_sobol>()},
      {"stratified",
       CB<RemoveStratifiedSampler, &Sampler::mutable_stratified>()},
      {"02sequence",
       CB<RemoveZeroTwoSequenceSampler, &Sampler::mutable_zerotwosequence>()},
  };

  return Parse<&Directive::mutable_sampler>(kSupportedTypes, sampler_type,
                                            parameters);
}

absl::Status ParserV3::Shape(
    absl::string_view shape_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v3::Shape> kSupportedTypes = {
      {"cone", CB<RemoveConeShape, &Shape::mutable_cone>()},
      {"curve",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::Shape& shape) {
         return TryRemoveCurveShape(parameters, kPbrtVersion,
                                    std::bind(&Shape::mutable_curve, &shape));
       }},
      {"cylinder", CB<RemoveCylinderShape, &Shape::mutable_cylinder>()},
      {"disk", CB<RemoveDiskShape, &Shape::mutable_disk>()},
      {"heightfield",
       CB<RemoveHeightFieldShape, &Shape::mutable_heightfield>()},
      {"hyperboloid",
       CB<RemoveHyperboloidShape, &Shape::mutable_hyperboloid>()},
      {"loopsubdiv", CB<RemoveLoopSubdivShape, &Shape::mutable_loopsubdiv>()},
      {"nurbs", CB<RemoveNurbsShape, &Shape::mutable_nurbs>()},
      {"paraboloid", CB<RemoveParaboloidShape, &Shape::mutable_paraboloid>()},
      {"plymesh", CB<RemovePlyMeshShape, &Shape::mutable_plymesh>()},
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

  if (absl::Status status = TryRemoveSpectrumTexture(
          parameters, "amount",
          std::bind(&Shape::MaterialOverrides::mutable_amount, overrides));
      !status.ok()) {
    return status;
  }

  if (absl::Status status = TryRemoveSpectrumTexture(
          parameters, "color",
          std::bind(&Shape::MaterialOverrides::mutable_color, overrides));
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
          parameters, "mfp",
          std::bind(&Shape::MaterialOverrides::mutable_mfp, overrides));
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
          parameters, "scatterdistance",
          std::bind(&Shape::MaterialOverrides::mutable_scatterdistance,
                    overrides));
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
          parameters, "sigma_s",
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

absl::Status ParserV3::SpectrumTexture(
    absl::string_view spectrum_texture_name,
    absl::string_view spectrum_texture_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  static const TypeMap<v3::SpectrumTexture> kSupportedTypes = {
      {"bilerp",
       CB<RemoveBilerpSpectrumTexture, &SpectrumTexture::mutable_bilerp>()},
      {"checkerboard",
       [](absl::flat_hash_map<absl::string_view, Parameter>& parameters,
          v3::SpectrumTexture& spectrum_texture) {
         int32_t dimension =
             TryRemoveInteger(parameters, "dimension").value_or(2);

         if (dimension == 2) {
           return RemoveCheckerboard2DSpectrumTexture(
               parameters, kPbrtVersion,
               *spectrum_texture.mutable_checkerboard2d());
         }

         if (dimension != 3) {
           std::cerr << "WARNING: Unsupported value for 'checkerboard' Texture "
                        "parameter 'dimension': "
                     << dimension << std::endl;
         }

         return RemoveCheckerboard3DSpectrumTexture(
             parameters, kPbrtVersion,
             *spectrum_texture.mutable_checkerboard3d());
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
      {"ptex", CB<RemovePtexSpectrumTexture, &SpectrumTexture::mutable_ptex>()},
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
