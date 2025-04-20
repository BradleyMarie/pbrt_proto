#include "pbrt_proto/v3/defaults.h"

#include <array>

#include "pbrt_proto/v3/pbrt.pb.h"

namespace pbrt_proto::v3 {
namespace {

const size_t kCopperSamples = 56;
const double kCopperWavelengths[kCopperSamples] = {
    298.7570554, 302.4004341, 306.1337728, 309.960445,  313.8839949,
    317.9081487, 322.036826,  326.2741526, 330.6244747, 335.092373,
    339.6826795, 344.4004944, 349.2512056, 354.2405086, 359.374429,
    364.6593471, 370.1020239, 375.7096303, 381.4897785, 387.4505563,
    393.6005651, 399.9489613, 406.5055016, 413.2805933, 420.2853492,
    427.5316483, 435.0322035, 442.8006357, 450.8515564, 459.2006593,
    467.8648226, 476.8622231, 486.2124627, 495.936712,  506.0578694,
    516.6007417, 527.5922468, 539.0616435, 551.0407911, 563.5644455,
    576.6705953, 590.4008476, 604.8008683, 619.92089,   635.8162974,
    652.5483053, 670.1847459, 688.8009889, 708.4810171, 729.3186941,
    751.4192606, 774.9011125, 799.8979226, 826.5611867, 855.0632966,
    885.6012714};
const double kCopperN[kCopperSamples] = {
    1.400313, 1.38,  1.358438, 1.34,  1.329063, 1.325, 1.3325,   1.34,
    1.334375, 1.325, 1.317812, 1.31,  1.300313, 1.29,  1.281563, 1.27,
    1.249062, 1.225, 1.2,      1.18,  1.174375, 1.175, 1.1775,   1.18,
    1.178125, 1.175, 1.172812, 1.17,  1.165312, 1.16,  1.155312, 1.15,
    1.142812, 1.135, 1.131562, 1.12,  1.092437, 1.04,  0.950375, 0.826,
    0.645875, 0.468, 0.35125,  0.272, 0.230813, 0.214, 0.20925,  0.213,
    0.21625,  0.223, 0.2365,   0.25,  0.254188, 0.26,  0.28,     0.3};
const double kCopperK[kCopperSamples] = {
    1.662125, 1.687, 1.703313, 1.72,  1.744563, 1.77,  1.791625, 1.81,
    1.822125, 1.834, 1.85175,  1.872, 1.89425,  1.916, 1.931688, 1.95,
    1.972438, 2.015, 2.121562, 2.21,  2.177188, 2.13,  2.160063, 2.21,
    2.249938, 2.289, 2.326,    2.362, 2.397625, 2.433, 2.469187, 2.504,
    2.535875, 2.564, 2.589625, 2.605, 2.595562, 2.583, 2.5765,   2.599,
    2.678062, 2.809, 3.01075,  3.24,  3.458187, 3.67,  3.863125, 4.05,
    4.239563, 4.43,  4.619563, 4.817, 5.034125, 5.26,  5.485625, 5.717};

const std::array<double, 3> kSigmaA[47] = {
    {2.29, 2.39, 1.97},                    // APPLE
    {0.15, 0.21, 0.38},                    // CHICKEN1
    {0.19, 0.25, 0.32},                    // CHICKEN2
    {7.38, 5.47, 3.15},                    // CREAM
    {0.18, 0.07, 0.03},                    // KETCHUP
    {2.19, 2.62, 3.00},                    // MARBLE
    {0.68, 0.70, 0.55},                    // POTATO
    {0.70, 1.22, 1.90},                    // SKIMMILK
    {0.74, 0.88, 1.01},                    // SKIN1
    {1.09, 1.59, 1.79},                    // SKIN2
    {11.6, 20.4, 14.9},                    // SPECTRALON
    {2.55, 3.21, 3.77},                    // WHOLEMILK
    {0.89187, 1.5136, 2.532},              // LOWFAT_MILK
    {2.4858, 3.1669, 4.5214},              // REDUCED_MILK
    {4.5513, 5.8294, 7.136},               // REGULAR_MILK
    {0.72378, 0.84557, 1.0247},            // ESPRESSO
    {0.31602, 0.38538, 0.48131},           // MINT_MOCHA_COFFEE
    {0.30576, 0.34233, 0.61664},           // LOWFAT_SOY_MILK
    {0.59223, 0.73866, 1.4693},            // REGULAR_SOY_MILK
    {0.64925, 0.83916, 1.1057},            // LOWFAT_CHOCOLATE_MILK
    {1.4585, 2.1289, 2.9527},              // REGULAR_CHOCOLATE_MILK
    {8.9053e-05, 8.372e-05, 0},            // COKE
    {6.1697e-05, 4.2564e-05, 0},           // PEPSI
    {6.0306e-06, 6.4139e-06, 6.5504e-06},  // SPRITE
    {0.0024574, 0.003007, 0.0037325},      // GATORADE
    {1.7982e-05, 1.3758e-05, 1.2023e-05},  // CHARDONNAY
    {1.7501e-05, 1.9069e-05, 1.288e-05},   // WHITE_ZINFANDEL
    {2.1129e-05, 0.0, 0.0},                // MERLOT
    {2.4356e-05, 2.4079e-05, 1.0564e-05},  // BUDWEISER_BEER
    {5.0922e-05, 4.301e-05, 0},            // COORS_LIGHT_BEER
    {0.0024035, 0.0031373, 0.003991},      // CLOROX
    {0.00013612, 0.00015836, 0.000227},    // APPLE_JUICE
    {0.00010402, 0.00011646, 7.8139e-05},  // CRANBERRY_JUICE
    {5.382e-05, 0, 0},                     // GRAPE_JUICE
    {0.011002, 0.010927, 0.011036},        // RUBY_GRAPEFRUIT_JUICE
    {0.22826, 0.23998, 0.32748},           // WHITE_GRAPEFRUIT_JUICE
    {0.0007176, 0.0008303, 0.0009016},     // SHAMPOO
    {0.00015671, 0.00015947, 1.518e-05},   // STRAWBERRY_SHAMPOO
    {0.023805, 0.028804, 0.034306},        // HEAD_AND_SHOULDERS_SHAMPOO
    {0.040224, 0.045264, 0.051081},        // LEMON_TEA_POWDER
    {0.00015617, 0.00017482, 0.0001762},   // ORANGE_POWDER
    {0.00012103, 0.00013073, 0.00012528},  // PINK_LEMONADE
    {1.8436, 2.5851, 2.1662},              // CAPPUCCINO_POWDER
    {0.027333, 0.032451, 0.031979},        // SALT_POWDER
    {0.00022272, 0.00025513, 0.000271},    // SUGAR_POWDER
    {2.7979, 3.5452, 4.3365},              // SUISSE_MOCHA_POWDER
    {0.0001764, 0.00032095, 0.00019617},   // PACIFIC_OCEAN_SURFACE_WATER
};

const std::array<double, 3> kSigmaS[47] = {
    {0.0030, 0.0034, 0.046},           // APPLE
    {0.015, 0.077, 0.19},              // CHICKEN1
    {0.018, 0.088, 0.20},              // CHICKEN2
    {0.0002, 0.0028, 0.0163},          // CREAM
    {0.061, 0.97, 1.45},               // KETCHUP
    {0.0021, 0.0041, 0.0071},          // MARBLE
    {0.0024, 0.0090, 0.12},            // POTATO
    {0.0014, 0.0025, 0.0142},          // SKIMMILK
    {0.032, 0.17, 0.48},               // SKIN1
    {0.013, 0.070, 0.145},             // SKIN2
    {0.00, 0.00, 0.00},                // SPECTRALON
    {0.0011, 0.0024, 0.014},           // WHOLEMILK
    {0.002875, 0.00575, 0.0115},       // LOWFAT_MILK
    {0.0025556, 0.0051111, 0.012778},  // REDUCED_MILK
    {0.0015333, 0.0046, 0.019933},     // REGULAR_MILK
    {4.7984, 6.5751, 8.8493},          // ESPRESSO
    {3.772, 5.8228, 7.82},             // MINT_MOCHA_COFFEE
    {0.0014375, 0.0071875, 0.035937},  // LOWFAT_SOY_MILK
    {0.0019167, 0.0095833, 0.065167},  // REGULAR_SOY_MILK
    {0.0115, 0.0368, 0.1564},          // LOWFAT_CHOCOLATE_MILK
    {0.010063, 0.043125, 0.14375},     // REGULAR_CHOCOLATE_MILK
    {0.10014, 0.16503, 0.2468},        // COKE
    {0.091641, 0.14158, 0.20729},      // PEPSI
    {0.001886, 0.0018308, 0.0020025},  // SPRITE
    {0.024794, 0.019289, 0.008878},    // GATORADE
    {0.010782, 0.011855, 0.023997},    // CHARDONNAY
    {0.012072, 0.016184, 0.019843},    // WHITE_ZINFANDEL
    {0.11632, 0.25191, 0.29434},       // MERLOT
    {0.011492, 0.024911, 0.057786},    // BUDWEISER_BEER
    {0.006164, 0.013984, 0.034983},    // COORS_LIGHT_BEER
    {0.0033542, 0.014892, 0.026297},   // CLOROX
    {0.012957, 0.023741, 0.052184},    // APPLE_JUICE
    {0.039437, 0.094223, 0.12426},     // CRANBERRY_JUICE
    {0.10404, 0.23958, 0.29325},       // GRAPE_JUICE
    {0.085867, 0.18314, 0.25262},      // RUBY_GRAPEFRUIT_JUICE
    {0.0138, 0.018831, 0.056781},      // WHITE_GRAPEFRUIT_JUICE
    {0.014107, 0.045693, 0.061717},    // SHAMPOO
    {0.01449, 0.05796, 0.075823},      // STRAWBERRY_SHAMPOO
    {0.084621, 0.15688, 0.20365},      // HEAD_AND_SHOULDERS_SHAMPOO
    {2.4288, 4.5757, 7.2127},          // LEMON_TEA_POWDER
    {0.001449, 0.003441, 0.007863},    // ORANGE_POWDER
    {0.001165, 0.002366, 0.003195},    // PINK_LEMONADE
    {35.844, 49.547, 61.084},          // CAPPUCCINO_POWDER
    {0.28415, 0.3257, 0.34148},        // SALT_POWDER
    {0.012638, 0.031051, 0.050124},    // SUGAR_POWDER
    {17.502, 27.004, 35.433},          // SUISSE_MOCHA_POWDER
    {0.031845, 0.031324, 0.030147},    // PACIFIC_OCEAN_SURFACE_WATER
};

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

template <typename T>
void SetDefaultFrom(T& message) {
  if (!message.has_from()) {
    message.mutable_from()->set_x(0.0);
    message.mutable_from()->set_y(0.0);
    message.mutable_from()->set_z(0.0);
  }
}

template <typename T>
void SetDefaultTo(T& message) {
  if (!message.has_to()) {
    message.mutable_to()->set_x(0.0);
    message.mutable_to()->set_y(0.0);
    message.mutable_to()->set_z(1.0);
  }
}

template <typename T>
void SetDefaultI(T& message) {
  if (!message.has_i()) {
    message.mutable_i()->mutable_rgb_spectrum()->set_r(1.0);
    message.mutable_i()->mutable_rgb_spectrum()->set_g(1.0);
    message.mutable_i()->mutable_rgb_spectrum()->set_b(1.0);
  }
}

template <typename T>
void SetDefaultL(T& message) {
  if (!message.has_l()) {
    message.mutable_l()->mutable_rgb_spectrum()->set_r(1.0);
    message.mutable_l()->mutable_rgb_spectrum()->set_g(1.0);
    message.mutable_l()->mutable_rgb_spectrum()->set_b(1.0);
  }
}

template <typename T>
void SetDefaultScale(T& message) {
  if (!message.has_scale()) {
    message.mutable_scale()->mutable_rgb_spectrum()->set_r(1.0);
    message.mutable_scale()->mutable_rgb_spectrum()->set_g(1.0);
    message.mutable_scale()->mutable_rgb_spectrum()->set_b(1.0);
  }
}

void CanonicalizeMaterial(Material& material) {
  switch (material.material_type_case()) {
    case Material::kDisney:
      SetSpectrumTextureParameterDefault(
          *material.mutable_disney()->mutable_color(), 0.5);
      SetFloatTextureParameterDefault(
          *material.mutable_disney()->mutable_anisotropic(), 0.0);
      SetFloatTextureParameterDefault(
          *material.mutable_disney()->mutable_clearcoat(), 0.0);
      SetFloatTextureParameterDefault(
          *material.mutable_disney()->mutable_clearcoatgloss(), 1.0);
      SetFloatTextureParameterDefault(*material.mutable_disney()->mutable_eta(),
                                      1.5);
      SetFloatTextureParameterDefault(
          *material.mutable_disney()->mutable_metallic(), 0.0);
      SetFloatTextureParameterDefault(
          *material.mutable_disney()->mutable_roughness(), 0.5);
      SetSpectrumTextureParameterDefault(
          *material.mutable_disney()->mutable_scatterdistance(), 0.0);
      SetFloatTextureParameterDefault(
          *material.mutable_disney()->mutable_sheen(), 0.0);
      SetFloatTextureParameterDefault(
          *material.mutable_disney()->mutable_sheentint(), 0.5);
      SetFloatTextureParameterDefault(
          *material.mutable_disney()->mutable_spectrans(), 0.0);
      SetFloatTextureParameterDefault(
          *material.mutable_disney()->mutable_speculartint(), 0.0);
      SetFloatTextureParameterDefault(
          *material.mutable_disney()->mutable_difftrans(), 1.0);
      SetFloatTextureParameterDefault(
          *material.mutable_disney()->mutable_flatness(), 0.0);
      break;
    case Material::kFourier:
      break;
    case Material::kGlass:
      SetSpectrumTextureParameterDefault(
          *material.mutable_glass()->mutable_kr(), 1.0);
      SetSpectrumTextureParameterDefault(
          *material.mutable_glass()->mutable_kt(), 1.0);
      SetFloatTextureParameterDefault(*material.mutable_glass()->mutable_eta(),
                                      1.5);
      SetFloatTextureParameterDefault(
          *material.mutable_glass()->mutable_uroughness(), 0.0);
      SetFloatTextureParameterDefault(
          *material.mutable_glass()->mutable_vroughness(), 0.0);
      break;
    case Material::kHair:
      if (!material.hair().has_sigma_a() && !material.hair().has_color() &&
          !material.hair().has_eumelanin() &&
          !material.hair().has_pheomelanin()) {
        auto& sigma_a =
            *material.mutable_hair()->mutable_sigma_a()->mutable_rgb_spectrum();
        sigma_a.set_r(1.3 * 0.419);
        sigma_a.set_g(1.3 * 0.697);
        sigma_a.set_b(1.3 * 1.37);
      }
      SetFloatTextureParameterDefault(
          *material.mutable_hair()->mutable_eumelanin(), 0.0);
      SetFloatTextureParameterDefault(
          *material.mutable_hair()->mutable_pheomelanin(), 0.0);
      SetFloatTextureParameterDefault(*material.mutable_hair()->mutable_eta(),
                                      1.55);
      SetFloatTextureParameterDefault(
          *material.mutable_hair()->mutable_beta_m(), 0.3);
      SetFloatTextureParameterDefault(
          *material.mutable_hair()->mutable_beta_n(), 0.3);
      SetFloatTextureParameterDefault(*material.mutable_hair()->mutable_alpha(),
                                      2.0);
      break;
    case Material::kKdsubsurface:
      if (!material.kdsubsurface().has_kd()) {
        auto& kd = *material.mutable_kdsubsurface()
                        ->mutable_kd()
                        ->mutable_rgb_spectrum();
        kd.set_r(0.5);
        kd.set_g(0.5);
        kd.set_b(0.5);
      }
      SetFloatTextureParameterDefault(
          *material.mutable_kdsubsurface()->mutable_mfp(), 1.0);
      SetSpectrumTextureParameterDefault(
          *material.mutable_kdsubsurface()->mutable_kr(), 1.0);
      SetSpectrumTextureParameterDefault(
          *material.mutable_kdsubsurface()->mutable_kt(), 1.0);
      SetFloatTextureParameterDefault(
          *material.mutable_kdsubsurface()->mutable_uroughness(), 0.0);
      SetFloatTextureParameterDefault(
          *material.mutable_kdsubsurface()->mutable_vroughness(), 0.0);
      break;
    case Material::kMatte:
      SetSpectrumTextureParameterDefault(
          *material.mutable_matte()->mutable_kd(), 0.5);
      SetFloatTextureParameterDefault(
          *material.mutable_matte()->mutable_sigma(), 0.0);
      break;
    case Material::kMetal:
      if (!material.metal().has_eta()) {
        auto& eta = *material.mutable_metal()
                         ->mutable_eta()
                         ->mutable_sampled_spectrum();
        for (size_t i = 0; i < kCopperSamples; i++) {
          auto& sample = *eta.add_samples();
          sample.set_wavelength(kCopperWavelengths[i]);
          sample.set_intensity(kCopperN[i]);
        }
      }
      if (!material.metal().has_k()) {
        auto& k =
            *material.mutable_metal()->mutable_k()->mutable_sampled_spectrum();
        for (size_t i = 0; i < kCopperSamples; i++) {
          auto& sample = *k.add_samples();
          sample.set_wavelength(kCopperWavelengths[i]);
          sample.set_intensity(kCopperK[i]);
        }
      }
      SetFloatTextureParameterDefault(
          *material.mutable_metal()->mutable_roughness(), 0.01);
      if (!material.metal().has_uroughness()) {
        *material.mutable_metal()->mutable_uroughness() =
            material.metal().roughness();
      }
      if (!material.metal().has_vroughness()) {
        *material.mutable_metal()->mutable_vroughness() =
            material.metal().roughness();
      }
      break;
    case Material::kMirror:
      break;
    case Material::kMix:
      break;
    case Material::kPlastic:
      break;
    case Material::kSubstrate:
      break;
    case Material::kSubsurface:
      break;
    case Material::kTranslucent:
      break;
    case Material::kUber:
      break;
    case Material::MATERIAL_TYPE_NOT_SET:
      break;
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
      case Directive::kAreaLightSource:
        if (directive.area_light_source().area_light_source_type_case() ==
            AreaLightSource::kDiffuse) {
          SetDefaultL(
              *directive.mutable_area_light_source()->mutable_diffuse());
          SetDefaultScale(
              *directive.mutable_area_light_source()->mutable_diffuse());
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
      case Directive::kLightSource:
        switch (directive.light_source().light_source_type_case()) {
          case LightSource::kDistant:
            SetDefaultL(*directive.mutable_light_source()->mutable_distant());
            SetDefaultFrom(
                *directive.mutable_light_source()->mutable_distant());
            SetDefaultTo(*directive.mutable_light_source()->mutable_distant());
            SetDefaultScale(
                *directive.mutable_light_source()->mutable_distant());
            break;
          case LightSource::kGoniometric:
            SetDefaultI(
                *directive.mutable_light_source()->mutable_goniometric());
            SetDefaultScale(
                *directive.mutable_light_source()->mutable_goniometric());
            break;
          case LightSource::kInfinite:
            SetDefaultL(*directive.mutable_light_source()->mutable_infinite());
            SetDefaultScale(
                *directive.mutable_light_source()->mutable_infinite());
            break;
          case LightSource::kPoint:
            SetDefaultL(*directive.mutable_light_source()->mutable_point());
            SetDefaultFrom(*directive.mutable_light_source()->mutable_point());
            SetDefaultScale(*directive.mutable_light_source()->mutable_point());
            break;
          case LightSource::kProjection:
            SetDefaultI(
                *directive.mutable_light_source()->mutable_projection());
            SetDefaultScale(
                *directive.mutable_light_source()->mutable_projection());
            break;
          case LightSource::kSpot:
            SetDefaultI(*directive.mutable_light_source()->mutable_spot());
            SetDefaultFrom(*directive.mutable_light_source()->mutable_spot());
            SetDefaultTo(*directive.mutable_light_source()->mutable_spot());
            SetDefaultScale(*directive.mutable_light_source()->mutable_spot());
            break;
          case LightSource::LIGHT_SOURCE_TYPE_NOT_SET:
            break;
        }
        break;
      case Directive::kMakeNamedMaterial:
        CanonicalizeMaterial(
            *directive.mutable_make_named_material()->mutable_material());
        break;
      case Directive::kMaterial:
        CanonicalizeMaterial(*directive.mutable_material());
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