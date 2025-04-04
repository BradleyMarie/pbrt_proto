#include "pbrt_proto/v3/defaults.h"

#include "pbrt_proto/v3/pbrt.pb.h"

namespace pbrt_proto::v3 {
namespace {

PbrtProto BuildDefaults() {
  PbrtProto defaults;

  // Accelerator
  defaults.add_directives()->mutable_accelerator()->mutable_bvh();

  // Camera
  defaults.add_directives()->mutable_camera()->mutable_perspective();

  // Film
  defaults.add_directives()->mutable_film()->mutable_image();

  // Filter
  defaults.add_directives()->mutable_filter()->mutable_box();

  // Integrator
  defaults.add_directives()->mutable_integrator()->mutable_path();

  // Sampler
  defaults.add_directives()->mutable_sampler()->mutable_halton();

  return defaults;
}

void SetFloatTextureParameterDefault(FloatTextureParameter& parameter,
                                     double default_value) {
  if (parameter.float_texture_parameter_type_case() ==
      FloatTextureParameter::FLOAT_TEXTURE_PARAMETER_TYPE_NOT_SET) {
    parameter.set_float_value(default_value);
  }
}

void SetSpectrumTextureParameterDefault(SpectrumTextureParameter& parameter,
                                        double default_value) {
  if (parameter.spectrum_texture_parameter_type_case() ==
      SpectrumTextureParameter::SPECTRUM_TEXTURE_PARAMETER_TYPE_NOT_SET) {
    parameter.set_uniform_spectrum(default_value);
  }
}

template <typename T>
void SetDefaultV1(T& message) {
  if (!message.has_v1()) {
    auto& v1 = *message.mutable_v1();
    v1.set_x(1.0);
    v1.set_y(0.0);
    v1.set_z(0.0);
  }
}

template <typename T>
void SetDefaultV2(T& message) {
  if (!message.has_v2()) {
    auto& v2 = *message.mutable_v2();
    v2.set_x(0.0);
    v2.set_y(1.0);
    v2.set_z(0.0);
  }
}

}  // namespace

const PbrtProto& GetDefaults() {
  static const PbrtProto defaults = BuildDefaults();
  return defaults;
}

void Canonicalize(PbrtProto& proto) {
  for (Directive& directive : *proto.mutable_directives()) {
    switch (directive.directive_type_case()) {
      case Directive::kAccelerator:
        if (directive.accelerator().accelerator_type_case() ==
            Accelerator::ACCELERATOR_TYPE_NOT_SET) {
          directive.mutable_accelerator()->mutable_bvh();
        }
        break;
      case Directive::kFloatTexture:
        switch (directive.float_texture().float_texture_type_case()) {
          case FloatTexture::kBilerp:
            SetFloatTextureParameterDefault(*directive.mutable_float_texture()
                                                 ->mutable_bilerp()
                                                 ->mutable_v00(),
                                            0.0);
            SetFloatTextureParameterDefault(*directive.mutable_float_texture()
                                                 ->mutable_bilerp()
                                                 ->mutable_v01(),
                                            1.0);
            SetFloatTextureParameterDefault(*directive.mutable_float_texture()
                                                 ->mutable_bilerp()
                                                 ->mutable_v10(),
                                            0.0);
            SetFloatTextureParameterDefault(*directive.mutable_float_texture()
                                                 ->mutable_bilerp()
                                                 ->mutable_v11(),
                                            1.0);
            SetDefaultV1(*directive.mutable_float_texture()->mutable_bilerp());
            SetDefaultV2(*directive.mutable_float_texture()->mutable_bilerp());
            break;
          case FloatTexture::kConstant:
            SetFloatTextureParameterDefault(*directive.mutable_float_texture()
                                                 ->mutable_constant()
                                                 ->mutable_value(),
                                            1.0);
            break;
          case FloatTexture::kCheckerboard2D:
            SetFloatTextureParameterDefault(*directive.mutable_float_texture()
                                                 ->mutable_checkerboard2d()
                                                 ->mutable_tex1(),
                                            1.0);
            SetFloatTextureParameterDefault(*directive.mutable_float_texture()
                                                 ->mutable_checkerboard2d()
                                                 ->mutable_tex2(),
                                            0.0);
            SetDefaultV1(
                *directive.mutable_float_texture()->mutable_checkerboard2d());
            SetDefaultV2(
                *directive.mutable_float_texture()->mutable_checkerboard2d());
            break;
          case FloatTexture::kCheckerboard3D:
            SetFloatTextureParameterDefault(*directive.mutable_float_texture()
                                                 ->mutable_checkerboard3d()
                                                 ->mutable_tex1(),
                                            1.0);
            SetFloatTextureParameterDefault(*directive.mutable_float_texture()
                                                 ->mutable_checkerboard3d()
                                                 ->mutable_tex2(),
                                            0.0);
            break;
          case FloatTexture::kDots:
            SetFloatTextureParameterDefault(*directive.mutable_float_texture()
                                                 ->mutable_dots()
                                                 ->mutable_inside(),
                                            1.0);
            SetFloatTextureParameterDefault(*directive.mutable_float_texture()
                                                 ->mutable_dots()
                                                 ->mutable_outside(),
                                            0.0);
            SetDefaultV1(*directive.mutable_float_texture()->mutable_dots());
            SetDefaultV2(*directive.mutable_float_texture()->mutable_dots());
            break;
          case FloatTexture::kFbm:
            break;
          case FloatTexture::kImagemap:
            SetDefaultV1(
                *directive.mutable_float_texture()->mutable_imagemap());
            SetDefaultV2(
                *directive.mutable_float_texture()->mutable_imagemap());
            break;
          case FloatTexture::kMarble:
            break;
          case FloatTexture::kMix:
            SetFloatTextureParameterDefault(*directive.mutable_float_texture()
                                                 ->mutable_mix()
                                                 ->mutable_amount(),
                                            0.5);
            SetFloatTextureParameterDefault(*directive.mutable_float_texture()
                                                 ->mutable_mix()
                                                 ->mutable_tex1(),
                                            0.0);
            SetFloatTextureParameterDefault(*directive.mutable_float_texture()
                                                 ->mutable_mix()
                                                 ->mutable_tex2(),
                                            1.0);
            break;
          case FloatTexture::kPtex:
            break;
          case FloatTexture::kScale:
            SetFloatTextureParameterDefault(*directive.mutable_float_texture()
                                                 ->mutable_scale()
                                                 ->mutable_tex1(),
                                            1.0);
            SetFloatTextureParameterDefault(*directive.mutable_float_texture()
                                                 ->mutable_scale()
                                                 ->mutable_tex2(),
                                            1.0);
            break;
          case FloatTexture::kWindy:
            break;
          case FloatTexture::kWrinkled:
            break;
          case FloatTexture::FLOAT_TEXTURE_TYPE_NOT_SET:
            break;
        }
        break;
      case Directive::kSpectrumTexture:
        switch (directive.spectrum_texture().spectrum_texture_type_case()) {
          case SpectrumTexture::kBilerp:
            SetSpectrumTextureParameterDefault(
                *directive.mutable_spectrum_texture()
                     ->mutable_bilerp()
                     ->mutable_v00(),
                0.0);
            SetSpectrumTextureParameterDefault(
                *directive.mutable_spectrum_texture()
                     ->mutable_bilerp()
                     ->mutable_v01(),
                1.0);
            SetSpectrumTextureParameterDefault(
                *directive.mutable_spectrum_texture()
                     ->mutable_bilerp()
                     ->mutable_v10(),
                0.0);
            SetSpectrumTextureParameterDefault(
                *directive.mutable_spectrum_texture()
                     ->mutable_bilerp()
                     ->mutable_v11(),
                1.0);
            SetDefaultV1(
                *directive.mutable_spectrum_texture()->mutable_bilerp());
            SetDefaultV2(
                *directive.mutable_spectrum_texture()->mutable_bilerp());
            break;
          case SpectrumTexture::kConstant:
            SetSpectrumTextureParameterDefault(
                *directive.mutable_spectrum_texture()
                     ->mutable_constant()
                     ->mutable_value(),
                1.0);
            break;
          case SpectrumTexture::kCheckerboard2D:
            SetSpectrumTextureParameterDefault(
                *directive.mutable_spectrum_texture()
                     ->mutable_checkerboard2d()
                     ->mutable_tex1(),
                1.0);
            SetSpectrumTextureParameterDefault(
                *directive.mutable_spectrum_texture()
                     ->mutable_checkerboard2d()
                     ->mutable_tex2(),
                0.0);
            SetDefaultV1(*directive.mutable_spectrum_texture()
                              ->mutable_checkerboard2d());
            SetDefaultV2(*directive.mutable_spectrum_texture()
                              ->mutable_checkerboard2d());
            break;
          case SpectrumTexture::kCheckerboard3D:
            SetSpectrumTextureParameterDefault(
                *directive.mutable_spectrum_texture()
                     ->mutable_checkerboard3d()
                     ->mutable_tex1(),
                1.0);
            SetSpectrumTextureParameterDefault(
                *directive.mutable_spectrum_texture()
                     ->mutable_checkerboard3d()
                     ->mutable_tex2(),
                0.0);
            break;
          case SpectrumTexture::kDots:
            SetSpectrumTextureParameterDefault(
                *directive.mutable_spectrum_texture()
                     ->mutable_dots()
                     ->mutable_inside(),
                1.0);
            SetSpectrumTextureParameterDefault(
                *directive.mutable_spectrum_texture()
                     ->mutable_dots()
                     ->mutable_outside(),
                0.0);
            SetDefaultV1(*directive.mutable_spectrum_texture()->mutable_dots());
            SetDefaultV2(*directive.mutable_spectrum_texture()->mutable_dots());
            break;
          case SpectrumTexture::kFbm:
            break;
          case SpectrumTexture::kImagemap:
            SetDefaultV1(
                *directive.mutable_spectrum_texture()->mutable_imagemap());
            SetDefaultV2(
                *directive.mutable_spectrum_texture()->mutable_imagemap());
            break;
          case SpectrumTexture::kMarble:
            break;
          case SpectrumTexture::kMix:
            SetFloatTextureParameterDefault(
                *directive.mutable_spectrum_texture()
                     ->mutable_mix()
                     ->mutable_amount(),
                0.5);
            SetSpectrumTextureParameterDefault(
                *directive.mutable_spectrum_texture()
                     ->mutable_mix()
                     ->mutable_tex1(),
                0.0);
            SetSpectrumTextureParameterDefault(
                *directive.mutable_spectrum_texture()
                     ->mutable_mix()
                     ->mutable_tex2(),
                1.0);
            break;
          case SpectrumTexture::kPtex:
            break;
          case SpectrumTexture::kScale:
            SetSpectrumTextureParameterDefault(
                *directive.mutable_spectrum_texture()
                     ->mutable_scale()
                     ->mutable_tex1(),
                1.0);
            SetSpectrumTextureParameterDefault(
                *directive.mutable_spectrum_texture()
                     ->mutable_scale()
                     ->mutable_tex2(),
                1.0);
            break;
          case SpectrumTexture::kUv:
            break;
          case SpectrumTexture::kWindy:
            break;
          case SpectrumTexture::kWrinkled:
            break;
          case SpectrumTexture::SPECTRUM_TEXTURE_TYPE_NOT_SET:
            break;
        }
        break;
      default:
        break;
    }
  }
}

}  // namespace pbrt_proto::v3