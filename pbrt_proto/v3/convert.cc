#include "pbrt_proto/v3/convert.h"

#include <array>
#include <functional>
#include <variant>

#include "absl/functional/function_ref.h"
#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "absl/types/span.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto::v3 {
namespace {

Integrator::LightSampleStrategy ParseLightSampleStrategy(
    absl::string_view lightsamplestrategy) {
  if (lightsamplestrategy == "power") {
    return Integrator::POWER;
  } else if (lightsamplestrategy == "uniform") {
    return Integrator::UNIFORM;
  }
  return Integrator::SPATIAL;
}

std::optional<ImageWrapping> TryRemoveImageWrapping(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  if (std::optional<absl::string_view> wrap =
          TryRemoveString(parameters, "wrap");
      wrap) {
    if (*wrap == "black") {
      return ImageWrapping::BLACK;
    } else if (*wrap == "clamp") {
      return ImageWrapping::CLAMP;
    } else if (*wrap == "repeat") {
      return ImageWrapping::REPEAT;
    }
  }

  return std::nullopt;
}

std::optional<AntialiasingMode> TryRemoveAntialiasingMode(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  if (std::optional<absl::string_view> aamode =
          TryRemoveString(parameters, "aamode");
      aamode) {
    if (*aamode == "closedform") {
      return AntialiasingMode::CLOSEDFORM;
    } else if (*aamode == "none") {
      return AntialiasingMode::NONE;
    }
  }

  return std::nullopt;
}

void TryRemoveSpectrum(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name,
    absl::FunctionRef<Spectrum*()> get_output) {
  if (std::optional<std::array<double, 2>> value =
          TryRemoveBlackbodyV1(parameters, parameter_name);
      value) {
    get_output()->mutable_blackbody_spectrum()->set_temperature((*value)[0]);
    get_output()->mutable_blackbody_spectrum()->set_scale((*value)[1]);
  } else if (std::optional<std::array<double, 3>> value =
                 TryRemoveRgb(parameters, parameter_name);
             value) {
    get_output()->mutable_rgb_spectrum()->set_r((*value)[0]);
    get_output()->mutable_rgb_spectrum()->set_g((*value)[1]);
    get_output()->mutable_rgb_spectrum()->set_b((*value)[2]);
  } else if (std::optional<std::array<double, 3>> value =
                 TryRemoveXyz(parameters, parameter_name);
             value) {
    get_output()->mutable_xyz_spectrum()->set_x((*value)[0]);
    get_output()->mutable_xyz_spectrum()->set_y((*value)[1]);
    get_output()->mutable_xyz_spectrum()->set_z((*value)[2]);
  } else if (std::optional<absl::Span<std::array<double, 2>>> values =
                 TryRemoveSpectralSamples(parameters, parameter_name);
             values) {
    for (const auto [wavelength, intensity] : *values) {
      auto& sample = *get_output()->mutable_sampled_spectrum()->add_samples();
      sample.set_wavelength(wavelength);
      sample.set_intensity(intensity);
    }
  } else if (std::optional<absl::string_view> value =
                 TryRemoveSpectrumFilename(parameters, parameter_name);
             value) {
    get_output()->set_sampled_spectrum_filename(*value);
  }
}

void TryRemoveFloatTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name,
    absl::FunctionRef<FloatTextureParameter*()> get_output) {
  if (std::optional<double> value = TryRemoveFloat(parameters, parameter_name);
      value) {
    get_output()->set_float_value(*value);
  } else if (std::optional<absl::string_view> texture_name =
                 TryRemoveTexture(parameters, parameter_name);
             texture_name) {
    get_output()->set_float_texture_name(*texture_name);
  }
}

void TryRemoveSpectrumTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name,
    absl::FunctionRef<SpectrumTextureParameter*()> get_output) {
  if (std::optional<std::array<double, 2>> value =
          TryRemoveBlackbodyV1(parameters, parameter_name);
      value) {
    get_output()->mutable_blackbody_spectrum()->set_temperature((*value)[0]);
    get_output()->mutable_blackbody_spectrum()->set_scale((*value)[1]);
  } else if (std::optional<std::array<double, 3>> value =
                 TryRemoveRgb(parameters, parameter_name);
             value) {
    get_output()->mutable_rgb_spectrum()->set_r((*value)[0]);
    get_output()->mutable_rgb_spectrum()->set_g((*value)[1]);
    get_output()->mutable_rgb_spectrum()->set_b((*value)[2]);
  } else if (std::optional<std::array<double, 3>> value =
                 TryRemoveXyz(parameters, parameter_name);
             value) {
    get_output()->mutable_xyz_spectrum()->set_x((*value)[0]);
    get_output()->mutable_xyz_spectrum()->set_y((*value)[1]);
    get_output()->mutable_xyz_spectrum()->set_z((*value)[2]);
  } else if (std::optional<absl::Span<std::array<double, 2>>> values =
                 TryRemoveSpectralSamples(parameters, parameter_name);
             values) {
    for (const auto [wavelength, intensity] : *values) {
      auto& sample = *get_output()->mutable_sampled_spectrum()->add_samples();
      sample.set_wavelength(wavelength);
      sample.set_intensity(intensity);
    }
  } else if (std::optional<absl::string_view> value =
                 TryRemoveSpectrumFilename(parameters, parameter_name);
             value) {
    get_output()->set_sampled_spectrum_filename(*value);
  } else if (std::optional<absl::string_view> value =
                 TryRemoveTexture(parameters, parameter_name);
             value) {
    get_output()->set_spectrum_texture_name(*value);
  }
}

template <typename T>
void TryRemoveUVParameters(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters, T& output) {
  if (std::optional<absl::string_view> mapping =
          TryRemoveString(parameters, "mapping");
      mapping) {
    if (*mapping == "uv") {
      output.set_mapping(Mapping2D::UV);
    } else if (*mapping == "spherical") {
      output.set_mapping(Mapping2D::SPHERICAL);
    } else if (*mapping == "cylindrical") {
      output.set_mapping(Mapping2D::CYLINDRICAL);
    } else if (*mapping == "planar") {
      output.set_mapping(Mapping2D::PLANAR);
    }
  }

  if (std::optional<double> uscale = TryRemoveFloat(parameters, "uscale");
      uscale) {
    output.set_uscale(*uscale);
  }

  if (std::optional<double> vscale = TryRemoveFloat(parameters, "vscale");
      vscale) {
    output.set_vscale(*vscale);
  }

  if (std::optional<double> udelta = TryRemoveFloat(parameters, "udelta");
      udelta) {
    output.set_udelta(*udelta);
  }

  if (std::optional<double> vdelta = TryRemoveFloat(parameters, "vdelta");
      vdelta) {
    output.set_vdelta(*vdelta);
  }

  if (std::optional<std::array<double, 3>> v1 =
          TryRemoveVector3(parameters, "v1");
      v1) {
    output.mutable_v1()->set_x((*v1)[0]);
    output.mutable_v1()->set_y((*v1)[1]);
    output.mutable_v1()->set_z((*v1)[2]);
  }

  if (std::optional<std::array<double, 3>> v2 =
          TryRemoveVector3(parameters, "v2");
      v2) {
    output.mutable_v2()->set_x((*v2)[0]);
    output.mutable_v2()->set_y((*v2)[1]);
    output.mutable_v2()->set_z((*v2)[2]);
  }
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

  absl::Status Filter(
      absl::string_view filter_type,
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

  absl::Status LookAt(double eye_x, double eye_y, double eye_z, double look_x,
                      double look_y, double look_z, double up_x, double up_y,
                      double up_z) override;

  absl::Status ObjectBegin(absl::string_view path) override;

  absl::Status ObjectEnd() override;

  absl::Status ObjectInstance(absl::string_view path) override;

  absl::Status ReverseOrientation() override;

  absl::Status Rotate(double angle, double x, double y, double z) override;

  absl::Status Sampler(
      absl::string_view sampler_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) override;

  absl::Status Scale(double x, double y, double z) override;

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
    auto& bvh = *accelerator.mutable_bvh();

    if (std::optional<int32_t> maxnodeprims =
            TryRemoveInteger(parameters, "maxnodeprims");
        maxnodeprims.has_value()) {
      bvh.set_maxnodeprims(*maxnodeprims);
    }

    if (std::optional<absl::string_view> splitmethod =
            TryRemoveString(parameters, "splitmethod");
        splitmethod.has_value()) {
      if (*splitmethod == "sah") {
        bvh.set_splitmethod(Accelerator::Bvh::SAH);
      } else if (*splitmethod == "middle") {
        bvh.set_splitmethod(Accelerator::Bvh::MIDDLE);
      } else if (*splitmethod == "equal") {
        bvh.set_splitmethod(Accelerator::Bvh::EQUAL);
      } else if (*splitmethod == "hlbvh") {
        bvh.set_splitmethod(Accelerator::Bvh::HLBVH);
      }
    }
  } else if (accelerator_type == "kdtree") {
    auto& kdtree = *accelerator.mutable_kdtree();

    if (std::optional<double> emptybonus =
            TryRemoveFloat(parameters, "emptybonus");
        emptybonus.has_value()) {
      kdtree.set_emptybonus(*emptybonus);
    }

    if (std::optional<int32_t> intersectcost =
            TryRemoveInteger(parameters, "intersectcost");
        intersectcost.has_value()) {
      kdtree.set_intersectcost(*intersectcost);
    }

    if (std::optional<int32_t> maxdepth =
            TryRemoveInteger(parameters, "maxdepth");
        maxdepth.has_value()) {
      kdtree.set_maxdepth(*maxdepth);
    }

    if (std::optional<int32_t> maxprims =
            TryRemoveInteger(parameters, "maxprims");
        maxprims.has_value()) {
      kdtree.set_maxprims(*maxprims);
    }

    if (std::optional<int32_t> traversalcost =
            TryRemoveInteger(parameters, "traversalcost");
        traversalcost.has_value()) {
      kdtree.set_traversalcost(*traversalcost);
    }
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

  if (area_light_source_type == "diffuse") {
    auto& diffuse = *area_light_source.mutable_diffuse();

    TryRemoveSpectrum(
        parameters, "L",
        std::bind(&AreaLightSource::Diffuse::mutable_l, &diffuse));

    if (std::optional<int32_t> samples =
            TryRemoveInteger(parameters, "samples");
        samples) {
      diffuse.set_samples(*samples);
    }

    if (std::optional<bool> twosided = TryRemoveBool(parameters, "twosided");
        twosided) {
      diffuse.set_twosided(*twosided);
    }
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
    auto& environment = *camera.mutable_environment();

    if (std::optional<double> shutteropen =
            TryRemoveFloat(parameters, "shutteropen");
        shutteropen.has_value()) {
      environment.set_shutteropen(*shutteropen);
    }

    if (std::optional<double> shutterclose =
            TryRemoveFloat(parameters, "shutterclose");
        shutterclose.has_value()) {
      environment.set_shutterclose(*shutterclose);
    }
  } else if (camera_type == "orthographic") {
    auto& orthographic = *camera.mutable_orthographic();

    if (std::optional<double> lensradius =
            TryRemoveFloat(parameters, "lensradius");
        lensradius.has_value()) {
      orthographic.set_lensradius(*lensradius);
    }

    if (std::optional<double> focaldistance =
            TryRemoveFloat(parameters, "focaldistance");
        focaldistance.has_value()) {
      orthographic.set_focaldistance(*focaldistance);
    }

    if (std::optional<double> frameaspectratio =
            TryRemoveFloat(parameters, "frameaspectratio");
        frameaspectratio.has_value()) {
      orthographic.set_frameaspectratio(*frameaspectratio);
    }

    std::optional<absl::Span<double>> screenwindow;
    if (absl::Status status =
            TryRemoveFloats(parameters, "screenwindow", 4, screenwindow);
        status.ok() && screenwindow.has_value()) {
      orthographic.mutable_screenwindow()->set_x_min((*screenwindow)[0]);
      orthographic.mutable_screenwindow()->set_x_max((*screenwindow)[1]);
      orthographic.mutable_screenwindow()->set_y_min((*screenwindow)[2]);
      orthographic.mutable_screenwindow()->set_y_max((*screenwindow)[3]);
    }

    if (std::optional<double> shutteropen =
            TryRemoveFloat(parameters, "shutteropen");
        shutteropen.has_value()) {
      orthographic.set_shutteropen(*shutteropen);
    }

    if (std::optional<double> shutterclose =
            TryRemoveFloat(parameters, "shutterclose");
        shutterclose.has_value()) {
      orthographic.set_shutterclose(*shutterclose);
    }
  } else if (camera_type == "perspective") {
    auto& perspective = *camera.mutable_perspective();

    if (std::optional<double> lensradius =
            TryRemoveFloat(parameters, "lensradius");
        lensradius.has_value()) {
      perspective.set_lensradius(*lensradius);
    }

    if (std::optional<double> focaldistance =
            TryRemoveFloat(parameters, "focaldistance");
        focaldistance.has_value()) {
      perspective.set_focaldistance(*focaldistance);
    }

    if (std::optional<double> frameaspectratio =
            TryRemoveFloat(parameters, "frameaspectratio");
        frameaspectratio.has_value()) {
      perspective.set_frameaspectratio(*frameaspectratio);
    }

    std::optional<absl::Span<double>> screenwindow;
    if (absl::Status status =
            TryRemoveFloats(parameters, "screenwindow", 4, screenwindow);
        status.ok() && screenwindow.has_value()) {
      perspective.mutable_screenwindow()->set_x_min((*screenwindow)[0]);
      perspective.mutable_screenwindow()->set_x_max((*screenwindow)[1]);
      perspective.mutable_screenwindow()->set_y_min((*screenwindow)[2]);
      perspective.mutable_screenwindow()->set_y_max((*screenwindow)[3]);
    }

    if (std::optional<double> fov = TryRemoveFloat(parameters, "fov");
        fov.has_value()) {
      perspective.set_fov(*fov);
    }

    if (std::optional<double> halffov = TryRemoveFloat(parameters, "halffov");
        halffov.has_value()) {
      perspective.set_halffov(*halffov);
    }

    if (std::optional<double> shutteropen =
            TryRemoveFloat(parameters, "shutteropen");
        shutteropen.has_value()) {
      perspective.set_shutteropen(*shutteropen);
    }

    if (std::optional<double> shutterclose =
            TryRemoveFloat(parameters, "shutterclose");
        shutterclose.has_value()) {
      perspective.set_shutterclose(*shutterclose);
    }
  } else if (camera_type == "realistic") {
    auto& realistic = *camera.mutable_realistic();

    if (std::optional<absl::string_view> lensfile =
            TryRemoveString(parameters, "lensfile");
        lensfile.has_value()) {
      realistic.set_lensfile(*lensfile);
    }

    if (std::optional<double> aperturediameter =
            TryRemoveFloat(parameters, "aperturediameter");
        aperturediameter.has_value()) {
      realistic.set_aperturediameter(*aperturediameter);
    }

    if (std::optional<double> focaldistance =
            TryRemoveFloat(parameters, "focaldistance");
        focaldistance.has_value()) {
      realistic.set_focaldistance(*focaldistance);
    }

    if (std::optional<bool> simpleweighting =
            TryRemoveBool(parameters, "simpleweighting");
        simpleweighting.has_value()) {
      realistic.set_simpleweighting(*simpleweighting);
    }

    if (std::optional<double> shutteropen =
            TryRemoveFloat(parameters, "shutteropen");
        shutteropen.has_value()) {
      realistic.set_shutteropen(*shutteropen);
    }

    if (std::optional<double> shutterclose =
            TryRemoveFloat(parameters, "shutterclose");
        shutterclose.has_value()) {
      realistic.set_shutterclose(*shutterclose);
    }
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
    auto& image = *film.mutable_image();

    if (std::optional<int32_t> xresolution =
            TryRemoveInteger(parameters, "xresolution");
        xresolution.has_value()) {
      image.set_xresolution(*xresolution);
    }

    if (std::optional<int32_t> yresolution =
            TryRemoveInteger(parameters, "yresolution");
        yresolution.has_value()) {
      image.set_yresolution(*yresolution);
    }

    std::optional<absl::Span<double>> cropwindow;
    if (absl::Status status =
            TryRemoveFloats(parameters, "cropwindow", 4, cropwindow);
        !status.ok()) {
      return status;
    } else if (cropwindow.has_value()) {
      image.mutable_cropwindow()->set_x_min((*cropwindow)[0]);
      image.mutable_cropwindow()->set_x_max((*cropwindow)[1]);
      image.mutable_cropwindow()->set_y_min((*cropwindow)[2]);
      image.mutable_cropwindow()->set_y_max((*cropwindow)[3]);
    }

    if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
        scale.has_value()) {
      image.set_scale(*scale);
    }

    if (std::optional<double> maxsampleluminance =
            TryRemoveFloat(parameters, "maxsampleluminance");
        maxsampleluminance.has_value()) {
      image.set_maxsampleluminance(*maxsampleluminance);
    }

    if (std::optional<double> diagonal = TryRemoveFloat(parameters, "diagonal");
        diagonal.has_value()) {
      image.set_diagonal(*diagonal);
    }

    if (std::optional<absl::string_view> filename =
            TryRemoveString(parameters, "filename");
        filename.has_value()) {
      image.set_filename(*filename);
    }
  } else {
    std::cerr << "Unrecognized Film type: \"" << film_type << "\"" << std::endl;
  }

  return absl::OkStatus();
}

absl::Status ParserV3::Filter(
    absl::string_view filter_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& filter = *output_.add_directives()->mutable_filter();

  if (filter_type == "box") {
    auto& box = *filter.mutable_box();

    if (std::optional<double> xwidth = TryRemoveFloat(parameters, "xwidth");
        xwidth.has_value()) {
      box.set_xwidth(*xwidth);
    }

    if (std::optional<double> ywidth = TryRemoveFloat(parameters, "ywidth");
        ywidth.has_value()) {
      box.set_ywidth(*ywidth);
    }
  } else if (filter_type == "gaussian") {
    auto& gaussian = *filter.mutable_gaussian();

    if (std::optional<double> xwidth = TryRemoveFloat(parameters, "xwidth");
        xwidth.has_value()) {
      gaussian.set_xwidth(*xwidth);
    }

    if (std::optional<double> ywidth = TryRemoveFloat(parameters, "ywidth");
        ywidth.has_value()) {
      gaussian.set_ywidth(*ywidth);
    }

    if (std::optional<double> alpha = TryRemoveFloat(parameters, "alpha");
        alpha.has_value()) {
      gaussian.set_alpha(*alpha);
    }
  } else if (filter_type == "mitchell") {
    auto& mitchell = *filter.mutable_mitchell();

    if (std::optional<double> xwidth = TryRemoveFloat(parameters, "xwidth");
        xwidth.has_value()) {
      mitchell.set_xwidth(*xwidth);
    }

    if (std::optional<double> ywidth = TryRemoveFloat(parameters, "ywidth");
        ywidth.has_value()) {
      mitchell.set_ywidth(*ywidth);
    }

    if (std::optional<double> B = TryRemoveFloat(parameters, "B");
        B.has_value()) {
      mitchell.set_b(*B);
    }

    if (std::optional<double> C = TryRemoveFloat(parameters, "C");
        C.has_value()) {
      mitchell.set_c(*C);
    }
  } else if (filter_type == "sinc") {
    auto& sinc = *filter.mutable_sinc();

    if (std::optional<double> xwidth = TryRemoveFloat(parameters, "xwidth");
        xwidth.has_value()) {
      sinc.set_xwidth(*xwidth);
    }

    if (std::optional<double> ywidth = TryRemoveFloat(parameters, "ywidth");
        ywidth.has_value()) {
      sinc.set_ywidth(*ywidth);
    }

    if (std::optional<double> tau = TryRemoveFloat(parameters, "tau");
        tau.has_value()) {
      sinc.set_tau(*tau);
    }
  } else if (filter_type == "triangle") {
    auto& triangle = *filter.mutable_triangle();

    if (std::optional<double> xwidth = TryRemoveFloat(parameters, "xwidth");
        xwidth.has_value()) {
      triangle.set_xwidth(*xwidth);
    }

    if (std::optional<double> ywidth = TryRemoveFloat(parameters, "ywidth");
        ywidth.has_value()) {
      triangle.set_ywidth(*ywidth);
    }
  } else {
    std::cerr << "Unrecognized Filter type: \"" << filter_type << "\""
              << std::endl;
  }

  return absl::OkStatus();
}

absl::Status ParserV3::FloatTexture(
    absl::string_view float_texture_name, absl::string_view float_texture_type,
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  auto& float_texture = *output_.add_directives()->mutable_float_texture();

  float_texture.set_name(float_texture_name);

  if (float_texture_type == "bilerp") {
    auto& bilerp = *float_texture.mutable_bilerp();

    TryRemoveFloatTexture(
        parameters, "v00",
        std::bind(&FloatTexture::Bilerp::mutable_v00, &bilerp));
    TryRemoveFloatTexture(
        parameters, "v01",
        std::bind(&FloatTexture::Bilerp::mutable_v01, &bilerp));
    TryRemoveFloatTexture(
        parameters, "v10",
        std::bind(&FloatTexture::Bilerp::mutable_v10, &bilerp));
    TryRemoveFloatTexture(
        parameters, "v11",
        std::bind(&FloatTexture::Bilerp::mutable_v11, &bilerp));
    TryRemoveUVParameters(parameters, bilerp);
  } else if (float_texture_type == "checkerboard") {
    int32_t dimensions = TryRemoveInteger(parameters, "dimensions").value_or(2);
    if (dimensions == 2) {
      auto& checkerboard = *float_texture.mutable_checkerboard2d();

      if (std::optional<AntialiasingMode> aamode =
              TryRemoveAntialiasingMode(parameters);
          aamode) {
        checkerboard.set_aamode(*aamode);
      }

      TryRemoveFloatTexture(
          parameters, "tex1",
          std::bind(&FloatTexture::Checkerboard2D::mutable_tex1,
                    &checkerboard));
      TryRemoveFloatTexture(
          parameters, "tex2",
          std::bind(&FloatTexture::Checkerboard2D::mutable_tex2,
                    &checkerboard));
      TryRemoveUVParameters(parameters, checkerboard);
    } else {
      auto& checkerboard = *float_texture.mutable_checkerboard3d();

      TryRemoveFloatTexture(
          parameters, "tex1",
          std::bind(&FloatTexture::Checkerboard3D::mutable_tex1,
                    &checkerboard));
      TryRemoveFloatTexture(
          parameters, "tex2",
          std::bind(&FloatTexture::Checkerboard3D::mutable_tex2,
                    &checkerboard));
    }
  } else if (float_texture_type == "constant") {
    auto& constant = *float_texture.mutable_constant();

    TryRemoveFloatTexture(
        parameters, "value",
        std::bind(&FloatTexture::Constant::mutable_value, &constant));
  } else if (float_texture_type == "dots") {
    auto& dots = *float_texture.mutable_dots();

    TryRemoveFloatTexture(
        parameters, "inside",
        std::bind(&FloatTexture::Dots::mutable_inside, &dots));
    TryRemoveFloatTexture(
        parameters, "outside",
        std::bind(&FloatTexture::Dots::mutable_outside, &dots));
    TryRemoveUVParameters(parameters, dots);
  } else if (float_texture_type == "fbm") {
    auto& fbm = *float_texture.mutable_fbm();

    if (std::optional<int32_t> octaves =
            TryRemoveInteger(parameters, "octaves");
        octaves) {
      fbm.set_octaves(*octaves);
    }

    if (std::optional<double> roughness =
            TryRemoveFloat(parameters, "roughness");
        roughness) {
      fbm.set_roughness(*roughness);
    }
  } else if (float_texture_type == "imagemap") {
    auto& imagemap = *float_texture.mutable_imagemap();

    if (std::optional<bool> gamma = TryRemoveBool(parameters, "gamma"); gamma) {
      imagemap.set_gamma(*gamma);
    }

    if (std::optional<absl::string_view> filename =
            TryRemoveString(parameters, "filename");
        filename) {
      imagemap.set_filename(*filename);
    }

    if (std::optional<double> maxanisotropy =
            TryRemoveFloat(parameters, "maxanisotropy");
        maxanisotropy) {
      imagemap.set_maxanisotropy(*maxanisotropy);
    }

    if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
        scale) {
      imagemap.set_scale(*scale);
    }

    if (std::optional<bool> trilinear = TryRemoveBool(parameters, "trilinear");
        trilinear) {
      imagemap.set_trilinear(*trilinear);
    }

    if (std::optional<ImageWrapping> wrap = TryRemoveImageWrapping(parameters);
        wrap) {
      imagemap.set_wrap(*wrap);
    }

    TryRemoveUVParameters(parameters, imagemap);
  } else if (float_texture_type == "marble") {
    auto& marble = *float_texture.mutable_marble();

    if (std::optional<int32_t> octaves =
            TryRemoveInteger(parameters, "octaves");
        octaves) {
      marble.set_octaves(*octaves);
    }

    if (std::optional<double> roughness =
            TryRemoveFloat(parameters, "roughness");
        roughness) {
      marble.set_roughness(*roughness);
    }

    if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
        scale) {
      marble.set_scale(*scale);
    }

    if (std::optional<double> variation =
            TryRemoveFloat(parameters, "variation");
        variation) {
      marble.set_variation(*variation);
    }
  } else if (float_texture_type == "mix") {
    auto& mix = *float_texture.mutable_mix();

    TryRemoveFloatTexture(parameters, "amount",
                          std::bind(&FloatTexture::Mix::mutable_amount, &mix));
    TryRemoveFloatTexture(parameters, "tex1",
                          std::bind(&FloatTexture::Mix::mutable_tex1, &mix));
    TryRemoveFloatTexture(parameters, "tex2",
                          std::bind(&FloatTexture::Mix::mutable_tex2, &mix));
  } else if (float_texture_type == "ptex") {
    auto& ptex = *float_texture.mutable_ptex();

    if (std::optional<absl::string_view> filename =
            TryRemoveString(parameters, "filename");
        filename) {
      ptex.set_filename(*filename);
    }

    if (std::optional<double> gamma = TryRemoveFloat(parameters, "gamma");
        gamma) {
      ptex.set_gamma(*gamma);
    }
  } else if (float_texture_type == "scale") {
    auto& scale = *float_texture.mutable_scale();

    TryRemoveFloatTexture(
        parameters, "tex1",
        std::bind(&FloatTexture::Scale::mutable_tex1, &scale));
    TryRemoveFloatTexture(
        parameters, "tex2",
        std::bind(&FloatTexture::Scale::mutable_tex2, &scale));
  } else if (float_texture_type == "windy") {
    float_texture.mutable_windy();
  } else if (float_texture_type == "wrinkled") {
    auto& wrinkled = *float_texture.mutable_wrinkled();

    if (std::optional<int32_t> octaves =
            TryRemoveInteger(parameters, "octaves");
        octaves) {
      wrinkled.set_octaves(*octaves);
    }

    if (std::optional<double> roughness =
            TryRemoveFloat(parameters, "roughness");
        roughness) {
      wrinkled.set_roughness(*roughness);
    }
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
    auto& ambientocclusion = *integrator.mutable_ambientocclusion();

    if (std::optional<bool> cossample = TryRemoveBool(parameters, "cossample");
        cossample.has_value()) {
      ambientocclusion.set_cossample(*cossample);
    }

    if (std::optional<int32_t> nsamples =
            TryRemoveInteger(parameters, "nsamples");
        nsamples.has_value()) {
      ambientocclusion.set_nsamples(*nsamples);
    }

    std::optional<absl::Span<int32_t>> pixelbounds;
    if (absl::Status status =
            TryRemoveIntegers(parameters, "pixelbounds", 4, pixelbounds);
        status.ok() && pixelbounds.has_value()) {
      ambientocclusion.mutable_pixelbounds()->set_x_min((*pixelbounds)[0]);
      ambientocclusion.mutable_pixelbounds()->set_x_max((*pixelbounds)[1]);
      ambientocclusion.mutable_pixelbounds()->set_y_min((*pixelbounds)[2]);
      ambientocclusion.mutable_pixelbounds()->set_y_max((*pixelbounds)[3]);
    }
  } else if (integrator_type == "bdpt") {
    auto& bdpt = *integrator.mutable_bdpt();

    if (std::optional<int32_t> maxdepth =
            TryRemoveInteger(parameters, "maxdepth");
        maxdepth.has_value()) {
      bdpt.set_maxdepth(*maxdepth);
    }

    if (std::optional<bool> visualizestrategies =
            TryRemoveBool(parameters, "visualizestrategies");
        visualizestrategies.has_value()) {
      bdpt.set_visualizestrategies(*visualizestrategies);
    }

    if (std::optional<bool> visualizeweights =
            TryRemoveBool(parameters, "visualizeweights");
        visualizeweights.has_value()) {
      bdpt.set_visualizeweights(*visualizeweights);
    }

    if (std::optional<absl::string_view> lightsamplestrategy =
            TryRemoveString(parameters, "lightsamplestrategy");
        lightsamplestrategy.has_value()) {
      bdpt.set_lightsamplestrategy(
          ParseLightSampleStrategy(*lightsamplestrategy));
    }

    std::optional<absl::Span<int32_t>> pixelbounds;
    if (absl::Status status =
            TryRemoveIntegers(parameters, "pixelbounds", 4, pixelbounds);
        status.ok() && pixelbounds.has_value()) {
      bdpt.mutable_pixelbounds()->set_x_min((*pixelbounds)[0]);
      bdpt.mutable_pixelbounds()->set_x_max((*pixelbounds)[1]);
      bdpt.mutable_pixelbounds()->set_y_min((*pixelbounds)[2]);
      bdpt.mutable_pixelbounds()->set_y_max((*pixelbounds)[3]);
    }
  } else if (integrator_type == "directlighting") {
    auto& directlighting = *integrator.mutable_directlighting();

    if (std::optional<int32_t> maxdepth =
            TryRemoveInteger(parameters, "maxdepth");
        maxdepth.has_value()) {
      directlighting.set_maxdepth(*maxdepth);
    }

    if (std::optional<absl::string_view> strategy =
            TryRemoveString(parameters, "strategy");
        strategy.has_value()) {
      if (*strategy == "one") {
        directlighting.set_strategy(Integrator::DirectLighting::ONE);
      } else {
        directlighting.set_strategy(Integrator::DirectLighting::ALL);
      }
    }

    std::optional<absl::Span<int32_t>> pixelbounds;
    if (absl::Status status =
            TryRemoveIntegers(parameters, "pixelbounds", 4, pixelbounds);
        status.ok() && pixelbounds.has_value()) {
      directlighting.mutable_pixelbounds()->set_x_min((*pixelbounds)[0]);
      directlighting.mutable_pixelbounds()->set_x_max((*pixelbounds)[1]);
      directlighting.mutable_pixelbounds()->set_y_min((*pixelbounds)[2]);
      directlighting.mutable_pixelbounds()->set_y_max((*pixelbounds)[3]);
    }
  } else if (integrator_type == "mlt") {
    auto& mlt = *integrator.mutable_mlt();

    if (std::optional<int32_t> maxdepth =
            TryRemoveInteger(parameters, "maxdepth");
        maxdepth.has_value()) {
      mlt.set_maxdepth(*maxdepth);
    }

    if (std::optional<int32_t> bootstrapsamples =
            TryRemoveInteger(parameters, "bootstrapsamples");
        bootstrapsamples.has_value()) {
      mlt.set_bootstrapsamples(*bootstrapsamples);
    }

    if (std::optional<int32_t> chains = TryRemoveInteger(parameters, "chains");
        chains.has_value()) {
      mlt.set_chains(*chains);
    }

    if (std::optional<int32_t> mutationsperpixel =
            TryRemoveInteger(parameters, "mutationsperpixel");
        mutationsperpixel.has_value()) {
      mlt.set_mutationsperpixel(*mutationsperpixel);
    }

    if (std::optional<double> largestepprobability =
            TryRemoveFloat(parameters, "largestepprobability");
        largestepprobability.has_value()) {
      mlt.set_largestepprobability(*largestepprobability);
    }

    if (std::optional<double> sigma = TryRemoveFloat(parameters, "sigma");
        sigma.has_value()) {
      mlt.set_sigma(*sigma);
    }
  } else if (integrator_type == "path") {
    auto& path = *integrator.mutable_path();

    if (std::optional<int32_t> maxdepth =
            TryRemoveInteger(parameters, "maxdepth");
        maxdepth.has_value()) {
      path.set_maxdepth(*maxdepth);
    }

    if (std::optional<double> rrthreshold =
            TryRemoveFloat(parameters, "rrthreshold");
        rrthreshold.has_value()) {
      path.set_rrthreshold(*rrthreshold);
    }

    if (std::optional<absl::string_view> lightsamplestrategy =
            TryRemoveString(parameters, "lightsamplestrategy");
        lightsamplestrategy.has_value()) {
      path.set_lightsamplestrategy(
          ParseLightSampleStrategy(*lightsamplestrategy));
    }

    std::optional<absl::Span<int32_t>> pixelbounds;
    if (absl::Status status =
            TryRemoveIntegers(parameters, "pixelbounds", 4, pixelbounds);
        status.ok() && pixelbounds.has_value()) {
      path.mutable_pixelbounds()->set_x_min((*pixelbounds)[0]);
      path.mutable_pixelbounds()->set_x_max((*pixelbounds)[1]);
      path.mutable_pixelbounds()->set_y_min((*pixelbounds)[2]);
      path.mutable_pixelbounds()->set_y_max((*pixelbounds)[3]);
    }
  } else if (integrator_type == "sppm") {
    auto& sppm = *integrator.mutable_sppm();

    if (std::optional<int32_t> maxdepth =
            TryRemoveInteger(parameters, "maxdepth");
        maxdepth.has_value()) {
      sppm.set_maxdepth(*maxdepth);
    }

    if (std::optional<int32_t> numiterations =
            TryRemoveInteger(parameters, "numiterations");
        numiterations.has_value()) {
      sppm.set_numiterations(*numiterations);
    }

    if (std::optional<int32_t> photonsperiteration =
            TryRemoveInteger(parameters, "photonsperiteration");
        photonsperiteration.has_value()) {
      sppm.set_photonsperiteration(*photonsperiteration);
    }

    if (std::optional<int32_t> imagewritefrequency =
            TryRemoveInteger(parameters, "imagewritefrequency");
        imagewritefrequency.has_value()) {
      sppm.set_imagewritefrequency(*imagewritefrequency);
    }

    if (std::optional<double> radius = TryRemoveFloat(parameters, "radius");
        radius.has_value()) {
      sppm.set_radius(*radius);
    }
  } else if (integrator_type == "volpath") {
    auto& volpath = *integrator.mutable_volpath();

    if (std::optional<int32_t> maxdepth =
            TryRemoveInteger(parameters, "maxdepth");
        maxdepth.has_value()) {
      volpath.set_maxdepth(*maxdepth);
    }

    if (std::optional<double> rrthreshold =
            TryRemoveFloat(parameters, "rrthreshold");
        rrthreshold.has_value()) {
      volpath.set_rrthreshold(*rrthreshold);
    }

    if (std::optional<absl::string_view> lightsamplestrategy =
            TryRemoveString(parameters, "lightsamplestrategy");
        lightsamplestrategy.has_value()) {
      volpath.set_lightsamplestrategy(
          ParseLightSampleStrategy(*lightsamplestrategy));
    }

    std::optional<absl::Span<int32_t>> pixelbounds;
    if (absl::Status status =
            TryRemoveIntegers(parameters, "pixelbounds", 4, pixelbounds);
        status.ok() && pixelbounds.has_value()) {
      volpath.mutable_pixelbounds()->set_x_min((*pixelbounds)[0]);
      volpath.mutable_pixelbounds()->set_x_max((*pixelbounds)[1]);
      volpath.mutable_pixelbounds()->set_y_min((*pixelbounds)[2]);
      volpath.mutable_pixelbounds()->set_y_max((*pixelbounds)[3]);
    }
  } else if (integrator_type == "whitted") {
    auto& whitted = *integrator.mutable_whitted();

    if (std::optional<int32_t> maxdepth =
            TryRemoveInteger(parameters, "maxdepth");
        maxdepth.has_value()) {
      whitted.set_maxdepth(*maxdepth);
    }

    std::optional<absl::Span<int32_t>> pixelbounds;
    if (absl::Status status =
            TryRemoveIntegers(parameters, "pixelbounds", 4, pixelbounds);
        status.ok() && pixelbounds.has_value()) {
      whitted.mutable_pixelbounds()->set_x_min((*pixelbounds)[0]);
      whitted.mutable_pixelbounds()->set_x_max((*pixelbounds)[1]);
      whitted.mutable_pixelbounds()->set_y_min((*pixelbounds)[2]);
      whitted.mutable_pixelbounds()->set_y_max((*pixelbounds)[3]);
    }
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
    auto& halton = *sampler.mutable_halton();

    if (std::optional<int32_t> pixelsamples =
            TryRemoveInteger(parameters, "pixelsamples");
        pixelsamples.has_value()) {
      halton.set_pixelsamples(*pixelsamples);
    }

    if (std::optional<bool> samplepixelcenter =
            TryRemoveBool(parameters, "samplepixelcenter");
        samplepixelcenter.has_value()) {
      halton.set_samplepixelcenter(*samplepixelcenter);
    }
  } else if (sampler_type == "maxmindist") {
    auto& maxmindist = *sampler.mutable_maxmindist();

    if (std::optional<int32_t> pixelsamples =
            TryRemoveInteger(parameters, "pixelsamples");
        pixelsamples.has_value()) {
      maxmindist.set_pixelsamples(*pixelsamples);
    }

    if (std::optional<int32_t> dimensions =
            TryRemoveInteger(parameters, "dimensions");
        dimensions.has_value()) {
      maxmindist.set_dimensions(*dimensions);
    }
  } else if (sampler_type == "random") {
    auto& random = *sampler.mutable_random();

    if (std::optional<int32_t> pixelsamples =
            TryRemoveInteger(parameters, "pixelsamples");
        pixelsamples.has_value()) {
      random.set_pixelsamples(*pixelsamples);
    }
  } else if (sampler_type == "sobol") {
    auto& sobol = *sampler.mutable_sobol();

    if (std::optional<int32_t> pixelsamples =
            TryRemoveInteger(parameters, "pixelsamples");
        pixelsamples.has_value()) {
      sobol.set_pixelsamples(*pixelsamples);
    }
  } else if (sampler_type == "stratified") {
    auto& stratified = *sampler.mutable_stratified();

    if (std::optional<bool> jitter = TryRemoveBool(parameters, "jitter");
        jitter.has_value()) {
      stratified.set_jitter(*jitter);
    }

    if (std::optional<int32_t> xsamples =
            TryRemoveInteger(parameters, "xsamples");
        xsamples.has_value()) {
      stratified.set_xsamples(*xsamples);
    }

    if (std::optional<int32_t> ysamples =
            TryRemoveInteger(parameters, "ysamples");
        ysamples.has_value()) {
      stratified.set_ysamples(*ysamples);
    }

    if (std::optional<int32_t> dimensions =
            TryRemoveInteger(parameters, "dimensions");
        dimensions.has_value()) {
      stratified.set_dimensions(*dimensions);
    }
  } else if (sampler_type == "02sequence" || sampler_type == "lowdiscrepancy") {
    auto& zerotwosequence = *sampler.mutable_zerotwosequence();

    if (std::optional<int32_t> pixelsamples =
            TryRemoveInteger(parameters, "pixelsamples");
        pixelsamples.has_value()) {
      zerotwosequence.set_pixelsamples(*pixelsamples);
    }

    if (std::optional<int32_t> dimensions =
            TryRemoveInteger(parameters, "dimensions");
        dimensions.has_value()) {
      zerotwosequence.set_dimensions(*dimensions);
    }
  } else {
    std::cerr << "Unrecognized Sampler type: \"" << sampler_type << "\""
              << std::endl;
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
    auto& bilerp = *spectrum_texture.mutable_bilerp();

    TryRemoveSpectrumTexture(
        parameters, "v00",
        std::bind(&SpectrumTexture::Bilerp::mutable_v00, &bilerp));
    TryRemoveSpectrumTexture(
        parameters, "v01",
        std::bind(&SpectrumTexture::Bilerp::mutable_v01, &bilerp));
    TryRemoveSpectrumTexture(
        parameters, "v10",
        std::bind(&SpectrumTexture::Bilerp::mutable_v10, &bilerp));
    TryRemoveSpectrumTexture(
        parameters, "v11",
        std::bind(&SpectrumTexture::Bilerp::mutable_v11, &bilerp));
    TryRemoveUVParameters(parameters, bilerp);
  } else if (spectrum_texture_type == "checkerboard") {
    int32_t dimensions = TryRemoveInteger(parameters, "dimensions").value_or(2);
    if (dimensions == 2) {
      auto& checkerboard = *spectrum_texture.mutable_checkerboard2d();

      if (std::optional<AntialiasingMode> aamode =
              TryRemoveAntialiasingMode(parameters);
          aamode) {
        checkerboard.set_aamode(*aamode);
      }

      TryRemoveSpectrumTexture(
          parameters, "tex1",
          std::bind(&SpectrumTexture::Checkerboard2D::mutable_tex1,
                    &checkerboard));
      TryRemoveSpectrumTexture(
          parameters, "tex2",
          std::bind(&SpectrumTexture::Checkerboard2D::mutable_tex2,
                    &checkerboard));
      TryRemoveUVParameters(parameters, checkerboard);
    } else {
      auto& checkerboard = *spectrum_texture.mutable_checkerboard3d();

      TryRemoveSpectrumTexture(
          parameters, "tex1",
          std::bind(&SpectrumTexture::Checkerboard3D::mutable_tex1,
                    &checkerboard));
      TryRemoveSpectrumTexture(
          parameters, "tex2",
          std::bind(&SpectrumTexture::Checkerboard3D::mutable_tex2,
                    &checkerboard));
    }
  } else if (spectrum_texture_type == "constant") {
    auto& constant = *spectrum_texture.mutable_constant();

    TryRemoveSpectrumTexture(
        parameters, "value",
        std::bind(&SpectrumTexture::Constant::mutable_value, &constant));
  } else if (spectrum_texture_type == "dots") {
    auto& dots = *spectrum_texture.mutable_dots();

    TryRemoveSpectrumTexture(
        parameters, "inside",
        std::bind(&SpectrumTexture::Dots::mutable_inside, &dots));
    TryRemoveSpectrumTexture(
        parameters, "outside",
        std::bind(&SpectrumTexture::Dots::mutable_outside, &dots));
    TryRemoveUVParameters(parameters, dots);
  } else if (spectrum_texture_type == "fbm") {
    auto& fbm = *spectrum_texture.mutable_fbm();

    if (std::optional<int32_t> octaves =
            TryRemoveInteger(parameters, "octaves");
        octaves) {
      fbm.set_octaves(*octaves);
    }

    if (std::optional<double> roughness =
            TryRemoveFloat(parameters, "roughness");
        roughness) {
      fbm.set_roughness(*roughness);
    }
  } else if (spectrum_texture_type == "imagemap") {
    auto& imagemap = *spectrum_texture.mutable_imagemap();

    if (std::optional<bool> gamma = TryRemoveBool(parameters, "gamma"); gamma) {
      imagemap.set_gamma(*gamma);
    }

    if (std::optional<absl::string_view> filename =
            TryRemoveString(parameters, "filename");
        filename) {
      imagemap.set_filename(*filename);
    }

    if (std::optional<double> maxanisotropy =
            TryRemoveFloat(parameters, "maxanisotropy");
        maxanisotropy) {
      imagemap.set_maxanisotropy(*maxanisotropy);
    }

    if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
        scale) {
      imagemap.set_scale(*scale);
    }

    if (std::optional<bool> trilinear = TryRemoveBool(parameters, "trilinear");
        trilinear) {
      imagemap.set_trilinear(*trilinear);
    }

    if (std::optional<ImageWrapping> wrap = TryRemoveImageWrapping(parameters);
        wrap) {
      imagemap.set_wrap(*wrap);
    }

    TryRemoveUVParameters(parameters, imagemap);
  } else if (spectrum_texture_type == "marble") {
    auto& marble = *spectrum_texture.mutable_marble();

    if (std::optional<int32_t> octaves =
            TryRemoveInteger(parameters, "octaves");
        octaves) {
      marble.set_octaves(*octaves);
    }

    if (std::optional<double> roughness =
            TryRemoveFloat(parameters, "roughness");
        roughness) {
      marble.set_roughness(*roughness);
    }

    if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
        scale) {
      marble.set_scale(*scale);
    }

    if (std::optional<double> variation =
            TryRemoveFloat(parameters, "variation");
        variation) {
      marble.set_variation(*variation);
    }
  } else if (spectrum_texture_type == "mix") {
    auto& mix = *spectrum_texture.mutable_mix();

    TryRemoveFloatTexture(
        parameters, "amount",
        std::bind(&SpectrumTexture::Mix::mutable_amount, &mix));
    TryRemoveSpectrumTexture(
        parameters, "tex1",
        std::bind(&SpectrumTexture::Mix::mutable_tex1, &mix));
    TryRemoveSpectrumTexture(
        parameters, "tex2",
        std::bind(&SpectrumTexture::Mix::mutable_tex2, &mix));
  } else if (spectrum_texture_type == "ptex") {
    auto& ptex = *spectrum_texture.mutable_ptex();

    if (std::optional<absl::string_view> filename =
            TryRemoveString(parameters, "filename");
        filename) {
      ptex.set_filename(*filename);
    }

    if (std::optional<double> gamma = TryRemoveFloat(parameters, "gamma");
        gamma) {
      ptex.set_gamma(*gamma);
    }
  } else if (spectrum_texture_type == "scale") {
    auto& scale = *spectrum_texture.mutable_scale();

    TryRemoveSpectrumTexture(
        parameters, "tex1",
        std::bind(&SpectrumTexture::Scale::mutable_tex1, &scale));
    TryRemoveSpectrumTexture(
        parameters, "tex2",
        std::bind(&SpectrumTexture::Scale::mutable_tex2, &scale));
  } else if (spectrum_texture_type == "uv") {
    auto& uv = *spectrum_texture.mutable_uv();
    TryRemoveUVParameters(parameters, uv);
  } else if (spectrum_texture_type == "windy") {
    spectrum_texture.mutable_windy();
  } else if (spectrum_texture_type == "wrinkled") {
    auto& wrinkled = *spectrum_texture.mutable_wrinkled();

    if (std::optional<int32_t> octaves =
            TryRemoveInteger(parameters, "octaves");
        octaves) {
      wrinkled.set_octaves(*octaves);
    }

    if (std::optional<double> roughness =
            TryRemoveFloat(parameters, "roughness");
        roughness) {
      wrinkled.set_roughness(*roughness);
    }
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

absl::StatusOr<PbrtProto> Convert(std::istream& input) {
  PbrtProto output;
  if (absl::Status error = ParserV3(output).ReadFrom(input); !error.ok()) {
    return error;
  }
  return output;
}

}  // namespace pbrt_proto::v3