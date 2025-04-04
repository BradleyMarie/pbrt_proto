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
            break;
          case FloatTexture::kConstant:
            SetFloatTextureParameterDefault(*directive.mutable_float_texture()
                                                 ->mutable_constant()
                                                 ->mutable_value(),
                                            1.0);
            break;
          case FloatTexture::kCheckerboard2D:
            break;
          case FloatTexture::kCheckerboard3D:
            break;
          case FloatTexture::kDots:
            break;
          case FloatTexture::kFbm:
            break;
          case FloatTexture::kImagemap:
            break;
          case FloatTexture::kMarble:
            break;
          case FloatTexture::kMix:
            SetFloatTextureParameterDefault(*directive.mutable_float_texture()
                                                 ->mutable_mix()
                                                 ->mutable_tex1(),
                                            0.0);
            SetFloatTextureParameterDefault(*directive.mutable_float_texture()
                                                 ->mutable_mix()
                                                 ->mutable_tex2(),
                                            1.0);
            SetFloatTextureParameterDefault(*directive.mutable_float_texture()
                                                 ->mutable_mix()
                                                 ->mutable_amount(),
                                            0.5);
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
        break;
      default:
        break;
    }
  }
}

}  // namespace pbrt_proto::v3