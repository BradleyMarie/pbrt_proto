#include "pbrt_proto/shared/textures.h"

#include <cmath>
#include <functional>
#include <optional>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "absl/strings/numbers.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/common.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {
namespace {

std::optional<CheckerboardAntialiasing> TryRemoveAaMode(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    bool pbrt_v1) {
  if (std::optional<absl::string_view> aamode =
          TryRemoveString(parameters, "aamode");
      aamode) {
    if (*aamode == "closedform") {
      return CheckerboardAntialiasing::CLOSEDFORM;
    } else if (*aamode == "none") {
      return CheckerboardAntialiasing::DISABLED;
    } else if (pbrt_v1 && *aamode == "supersample") {
      return CheckerboardAntialiasing::SUPERSAMPLE;
    } else {
      std::cerr << "Unsupported value for 'checkerboard' Texture parameter "
                   "'aamode': \""
                << *aamode << "\"" << std::endl;
      return CheckerboardAntialiasing::CLOSEDFORM;
    }
  }

  return std::nullopt;
}

template <typename T>
void RemoveDir(absl::flat_hash_map<absl::string_view, Parameter>& parameters,
               T& output) {
  if (std::optional<std::array<double, 3>> dir =
          TryRemoveVector3(parameters, "dir");
      dir) {
    output.mutable_dir()->set_x((*dir)[0]);
    output.mutable_dir()->set_y((*dir)[1]);
    output.mutable_dir()->set_z((*dir)[2]);
  }
}

template <typename T>
absl::Status RemoveEncoding(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view type_name, T& output) {
  if (std::optional<absl::string_view> encoding =
          TryRemoveString(parameters, "encoding");
      encoding) {
    if (*encoding == "sRGB") {
      output.set_encoding(ImageEncoding::SRGB);
    } else if (*encoding == "linear") {
      output.set_encoding(ImageEncoding::LINEAR);
    } else {
      std::vector<std::string_view> tokens =
          absl::StrSplit(*encoding, ' ', absl::SkipEmpty());

      double gamma;
      if (tokens.size() != 2 || tokens[0] != "gamma" ||
          !absl::SimpleAtod(tokens[1], &gamma) || !std::isfinite(gamma)) {
        return absl::InvalidArgumentError(
            absl::StrCat("Invalid ", type_name, " parameter : 'encoding'"));
      }

      output.set_encoding(ImageEncoding::GAMMA);
      output.set_gamma(gamma);
    }
  }

  return absl::OkStatus();
}

template <typename T>
void RemoveFilter(absl::flat_hash_map<absl::string_view, Parameter>& parameters,
                  T& output) {
  if (std::optional<absl::string_view> filter =
          TryRemoveString(parameters, "filter");
      filter) {
    if (*filter == "bilinear") {
      output.set_filter(TextureFilter::BILINEAR);
    } else if (*filter == "trilinear") {
      output.set_filter(TextureFilter::TRILINEAR);
    } else if (*filter == "ewa" || *filter == "EWA") {
      output.set_filter(TextureFilter::EWA);
    } else if (*filter == "point") {
      output.set_filter(TextureFilter::POINT);
    } else {
      std::cerr << "Unsupported value for 'imagemap' Texture parameter "
                   "'filter': \""
                << *filter << "\"" << std::endl;
      output.set_filter(TextureFilter::EWA);
    }
  }
}

std::optional<TextureMapping> TryRemoveMapping(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view type) {
  if (std::optional<absl::string_view> mapping =
          TryRemoveString(parameters, "mapping");
      mapping) {
    if (*mapping == "uv") {
      return TextureMapping::UV;
    } else if (*mapping == "spherical") {
      return TextureMapping::SPHERICAL;
    } else if (*mapping == "cylindrical") {
      return TextureMapping::CYLINDRICAL;
    } else if (*mapping == "planar") {
      return TextureMapping::PLANAR;
    } else {
      std::cerr << "Unsupported value for '" << type
                << "' Texture parameter 'mapping': \"" << *mapping << "\""
                << std::endl;
      return TextureMapping::UV;
    }
  }

  return std::nullopt;
}

std::optional<ImageWrap> TryRemoveWrap(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters) {
  if (std::optional<absl::string_view> wrap =
          TryRemoveString(parameters, "wrap");
      wrap) {
    if (*wrap == "black") {
      return ImageWrap::BLACK;
    } else if (*wrap == "clamp") {
      return ImageWrap::CLAMP;
    } else if (*wrap == "repeat") {
      return ImageWrap::REPEAT;
    } else {
      std::cerr
          << "Unsupported value for 'imagemap' Texture parameter 'wrap': \""
          << *wrap << "\"" << std::endl;
      return ImageWrap::REPEAT;
    }
  }

  return std::nullopt;
}

template <typename T>
void TryRemoveUVParameters(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view type, T& output) {
  if (std::optional<TextureMapping> mapping =
          TryRemoveMapping(parameters, type);
      mapping) {
    output.set_mapping(*mapping);
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

template <typename T>
void RemoveImageMapBase(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters, T& output) {
  if (std::optional<absl::string_view> filename =
          TryRemoveString(parameters, "filename");
      filename) {
    output.set_filename(*filename);
  }

  if (std::optional<double> maxanisotropy =
          TryRemoveFloat(parameters, "maxanisotropy");
      maxanisotropy) {
    output.set_maxanisotropy(*maxanisotropy);
  }

  if (std::optional<ImageWrap> wrap = TryRemoveWrap(parameters); wrap) {
    output.set_wrap(*wrap);
  }

  TryRemoveUVParameters(parameters, "imagemap", output);
}

}  // namespace

void RemoveBilerpFloatTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    BilerpFloatTexture& output) {
  TryRemoveFloatTexture(parameters, "v00",
                        std::bind(&BilerpFloatTexture::mutable_v00, &output));
  TryRemoveFloatTexture(parameters, "v01",
                        std::bind(&BilerpFloatTexture::mutable_v01, &output));
  TryRemoveFloatTexture(parameters, "v10",
                        std::bind(&BilerpFloatTexture::mutable_v10, &output));
  TryRemoveFloatTexture(parameters, "v11",
                        std::bind(&BilerpFloatTexture::mutable_v11, &output));
  TryRemoveUVParameters(parameters, "bilerp", output);
}

void RemoveBilerpSpectrumTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    BilerpSpectrumTexture& output) {
  TryRemoveSpectrumTextureV1(
      parameters, "v00",
      std::bind(&BilerpSpectrumTexture::mutable_v00, &output));
  TryRemoveSpectrumTextureV1(
      parameters, "v01",
      std::bind(&BilerpSpectrumTexture::mutable_v01, &output));
  TryRemoveSpectrumTextureV1(
      parameters, "v10",
      std::bind(&BilerpSpectrumTexture::mutable_v10, &output));
  TryRemoveSpectrumTextureV1(
      parameters, "v11",
      std::bind(&BilerpSpectrumTexture::mutable_v11, &output));
  TryRemoveUVParameters(parameters, "bilerp", output);
}

void RemoveCheckerboard2DFloatTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    Checkerboard2DFloatTexture& output) {
  if (std::optional<CheckerboardAntialiasing> aamode =
          TryRemoveAaMode(parameters, /*pbrt_v1=*/true);
      aamode) {
    output.set_aamode(*aamode);
  }

  RemoveCheckerboard2DFloatTextureV4(parameters, output);
}

void RemoveCheckerboard2DFloatTextureV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    Checkerboard2DFloatTexture& output) {
  if (std::optional<CheckerboardAntialiasing> aamode =
          TryRemoveAaMode(parameters, /*pbrt_v1=*/false);
      aamode) {
    output.set_aamode(*aamode);
  }

  RemoveCheckerboard2DFloatTextureV4(parameters, output);
}

void RemoveCheckerboard2DFloatTextureV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    Checkerboard2DFloatTexture& output) {
  TryRemoveFloatTexture(
      parameters, "tex1",
      std::bind(&Checkerboard2DFloatTexture::mutable_tex1, &output));
  TryRemoveFloatTexture(
      parameters, "tex2",
      std::bind(&Checkerboard2DFloatTexture::mutable_tex2, &output));
  TryRemoveUVParameters(parameters, "checkerboard", output);
}

void RemoveCheckerboard2DSpectrumTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    Checkerboard2DSpectrumTexture& output) {
  if (std::optional<CheckerboardAntialiasing> aamode =
          TryRemoveAaMode(parameters, /*pbrt_v1=*/true);
      aamode) {
    output.set_aamode(*aamode);
  }

  RemoveCheckerboard2DSpectrumTextureV4(parameters, output);
}

void RemoveCheckerboard2DSpectrumTextureV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    Checkerboard2DSpectrumTexture& output) {
  if (std::optional<CheckerboardAntialiasing> aamode =
          TryRemoveAaMode(parameters, /*pbrt_v1=*/false);
      aamode) {
    output.set_aamode(*aamode);
  }

  RemoveCheckerboard2DSpectrumTextureV4(parameters, output);
}

void RemoveCheckerboard2DSpectrumTextureV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    Checkerboard2DSpectrumTexture& output) {
  TryRemoveSpectrumTextureV1(
      parameters, "tex1",
      std::bind(&Checkerboard2DSpectrumTexture::mutable_tex1, &output));
  TryRemoveSpectrumTextureV1(
      parameters, "tex2",
      std::bind(&Checkerboard2DSpectrumTexture::mutable_tex2, &output));
  TryRemoveUVParameters(parameters, "checkerboard", output);
}

void RemoveCheckerboard3DFloatTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    Checkerboard3DFloatTexture& output) {
  TryRemoveFloatTexture(
      parameters, "tex1",
      std::bind(&Checkerboard3DFloatTexture::mutable_tex1, &output));
  TryRemoveFloatTexture(
      parameters, "tex2",
      std::bind(&Checkerboard3DFloatTexture::mutable_tex2, &output));
}

void RemoveCheckerboard3DSpectrumTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    Checkerboard3DSpectrumTexture& output) {
  TryRemoveSpectrumTextureV1(
      parameters, "tex1",
      std::bind(&Checkerboard3DSpectrumTexture::mutable_tex1, &output));
  TryRemoveSpectrumTextureV1(
      parameters, "tex2",
      std::bind(&Checkerboard3DSpectrumTexture::mutable_tex2, &output));
}

void RemoveConstantFloatTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ConstantFloatTexture& output) {
  if (std::optional<double> value = TryRemoveFloat(parameters, "value");
      value) {
    output.set_value(*value);
  }
}

absl::Status RemoveConstantSpectrumTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ConstantSpectrumTexture& output) {
  return TryRemoveSpectrumV1(
      parameters, "value",
      std::bind(&ConstantSpectrumTexture::mutable_value, &output));
}

void RemoveDirectionMixFloatTextureV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DirectionMixFloatTexture& output) {
  TryRemoveFloatTexture(
      parameters, "tex1",
      std::bind(&DirectionMixFloatTexture::mutable_tex1, &output));
  TryRemoveFloatTexture(
      parameters, "tex2",
      std::bind(&DirectionMixFloatTexture::mutable_tex2, &output));
  RemoveDir(parameters, output);
}

void RemoveDirectionMixSpectrumTextureV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DirectionMixSpectrumTexture& output) {
  TryRemoveSpectrumTextureV1(
      parameters, "tex1",
      std::bind(&DirectionMixSpectrumTexture::mutable_tex1, &output));
  TryRemoveSpectrumTextureV1(
      parameters, "tex2",
      std::bind(&DirectionMixSpectrumTexture::mutable_tex2, &output));
  RemoveDir(parameters, output);
}

void RemoveDotsFloatTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DotsFloatTexture& output) {
  TryRemoveFloatTexture(parameters, "inside",
                        std::bind(&DotsFloatTexture::mutable_inside, &output));
  TryRemoveFloatTexture(parameters, "outside",
                        std::bind(&DotsFloatTexture::mutable_outside, &output));
  TryRemoveUVParameters(parameters, "dots", output);
}

void RemoveDotsSpectrumTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DotsSpectrumTexture& output) {
  TryRemoveSpectrumTextureV1(
      parameters, "inside",
      std::bind(&DotsSpectrumTexture::mutable_inside, &output));
  TryRemoveSpectrumTextureV1(
      parameters, "outside",
      std::bind(&DotsSpectrumTexture::mutable_outside, &output));
  TryRemoveUVParameters(parameters, "dots", output);
}

void RemoveFBmFloatTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    FBmFloatTexture& output) {
  if (std::optional<int32_t> octaves = TryRemoveInteger(parameters, "octaves");
      octaves) {
    output.set_octaves(std::max(0, *octaves));
  }

  if (std::optional<double> roughness = TryRemoveFloat(parameters, "roughness");
      roughness) {
    output.set_roughness(*roughness);
  }
}

void RemoveFBmSpectrumTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    FBmSpectrumTexture& output) {
  if (std::optional<int32_t> octaves = TryRemoveInteger(parameters, "octaves");
      octaves) {
    output.set_octaves(std::max(0, *octaves));
  }

  if (std::optional<double> roughness = TryRemoveFloat(parameters, "roughness");
      roughness) {
    output.set_roughness(*roughness);
  }
}

void RemoveImageMapFloatTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ImageMapFloatTexture& output) {
  RemoveImageMapBase(parameters, output);

  if (std::optional<bool> trilinear = TryRemoveBool(parameters, "trilinear");
      trilinear) {
    output.set_filter(TextureFilter::TRILINEAR);
  }
}

void RemoveImageMapFloatTextureV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ImageMapFloatTexture& output) {
  RemoveImageMapFloatTextureV1(parameters, output);

  if (std::optional<double> gamma = TryRemoveFloat(parameters, "gamma");
      gamma) {
    output.set_gamma(*gamma);
  }

  if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
      scale) {
    output.set_scale(*scale);
  }
}

void RemoveImageMapFloatTextureV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ImageMapFloatTexture& output) {
  RemoveImageMapFloatTextureV1(parameters, output);

  if (std::optional<bool> gamma = TryRemoveBool(parameters, "gamma"); gamma) {
    output.set_encoding(*gamma ? ImageEncoding::GAMMA : ImageEncoding::SRGB);
  }

  if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
      scale) {
    output.set_scale(*scale);
  }
}

absl::Status RemoveImageMapFloatTextureV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ImageMapFloatTexture& output) {
  RemoveImageMapBase(parameters, output);

  RemoveFilter(parameters, output);

  if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
      scale) {
    output.set_scale(*scale);
  }

  if (std::optional<bool> invert = TryRemoveBool(parameters, "invert");
      invert) {
    output.set_invert(*invert);
  }

  return RemoveEncoding(parameters, "imagemap FloatTexture", output);
}

void RemoveImageMapSpectrumTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ImageMapSpectrumTexture& output) {
  RemoveImageMapBase(parameters, output);

  if (std::optional<bool> trilinear = TryRemoveBool(parameters, "trilinear");
      trilinear) {
    output.set_filter(TextureFilter::TRILINEAR);
  }
}

void RemoveImageMapSpectrumTextureV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ImageMapSpectrumTexture& output) {
  RemoveImageMapSpectrumTextureV1(parameters, output);

  if (std::optional<double> gamma = TryRemoveFloat(parameters, "gamma");
      gamma) {
    output.set_gamma(*gamma);
  }

  if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
      scale) {
    output.set_scale(*scale);
  }
}

void RemoveImageMapSpectrumTextureV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ImageMapSpectrumTexture& output) {
  RemoveImageMapSpectrumTextureV1(parameters, output);

  if (std::optional<bool> gamma = TryRemoveBool(parameters, "gamma"); gamma) {
    output.set_encoding(*gamma ? ImageEncoding::GAMMA : ImageEncoding::SRGB);
  }

  if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
      scale) {
    output.set_scale(*scale);
  }
}

absl::Status RemoveImageMapSpectrumTextureV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ImageMapSpectrumTexture& output) {
  RemoveImageMapBase(parameters, output);

  RemoveFilter(parameters, output);

  if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
      scale) {
    output.set_scale(*scale);
  }

  if (std::optional<bool> invert = TryRemoveBool(parameters, "invert");
      invert) {
    output.set_invert(*invert);
  }

  return RemoveEncoding(parameters, "imagemap SpectrumTexture", output);
}

void RemoveMarbleSpectrumTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    MarbleSpectrumTexture& output) {
  if (std::optional<int32_t> octaves = TryRemoveInteger(parameters, "octaves");
      octaves) {
    output.set_octaves(std::max(0, *octaves));
  }

  if (std::optional<double> roughness = TryRemoveFloat(parameters, "roughness");
      roughness) {
    output.set_roughness(*roughness);
  }

  if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
      scale) {
    output.set_scale(*scale);
  }

  if (std::optional<double> variation = TryRemoveFloat(parameters, "variation");
      variation) {
    output.set_variation(*variation);
  }
}

void RemoveMixFloatTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    MixFloatTexture& output) {
  TryRemoveFloatTexture(parameters, "amount",
                        std::bind(&MixFloatTexture::mutable_amount, &output));
  TryRemoveFloatTexture(parameters, "tex1",
                        std::bind(&MixFloatTexture::mutable_tex1, &output));
  TryRemoveFloatTexture(parameters, "tex2",
                        std::bind(&MixFloatTexture::mutable_tex2, &output));
}

void RemoveMixSpectrumTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    MixSpectrumTexture& output) {
  TryRemoveFloatTexture(
      parameters, "amount",
      std::bind(&MixSpectrumTexture::mutable_amount, &output));
  TryRemoveSpectrumTextureV1(
      parameters, "tex1",
      std::bind(&MixSpectrumTexture::mutable_tex1, &output));
  TryRemoveSpectrumTextureV1(
      parameters, "tex2",
      std::bind(&MixSpectrumTexture::mutable_tex2, &output));
}

void RemovePtexFloatTextureV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PtexFloatTexture& output) {
  if (std::optional<absl::string_view> filename =
          TryRemoveString(parameters, "filename");
      filename) {
    output.set_filename(*filename);
  }

  if (std::optional<double> gamma = TryRemoveFloat(parameters, "gamma");
      gamma) {
    output.set_gamma(*gamma);
  }
}

absl::Status RemovePtexFloatTextureV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PtexFloatTexture& output) {
  if (std::optional<absl::string_view> filename =
          TryRemoveString(parameters, "filename");
      filename) {
    output.set_filename(*filename);
  }

  if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
      scale) {
    output.set_scale(*scale);
  }

  return RemoveEncoding(parameters, "ptex SpectrumTexture", output);
}

void RemovePtexSpectrumTextureV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PtexSpectrumTexture& output) {
  if (std::optional<absl::string_view> filename =
          TryRemoveString(parameters, "filename");
      filename) {
    output.set_filename(*filename);
  }

  if (std::optional<double> gamma = TryRemoveFloat(parameters, "gamma");
      gamma) {
    output.set_gamma(*gamma);
  }
}

absl::Status RemovePtexSpectrumTextureV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PtexSpectrumTexture& output) {
  if (std::optional<absl::string_view> filename =
          TryRemoveString(parameters, "filename");
      filename) {
    output.set_filename(*filename);
  }

  if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
      scale) {
    output.set_scale(*scale);
  }

  return RemoveEncoding(parameters, "ptex SpectrumTexture", output);
}

void RemoveScaleFloatTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ScaleFloatTexture& output) {
  TryRemoveFloatTexture(parameters, "tex1",
                        std::bind(&ScaleFloatTexture::mutable_tex1, &output));
  TryRemoveFloatTexture(parameters, "tex2",
                        std::bind(&ScaleFloatTexture::mutable_tex2, &output));
}

void RemoveScaleSpectrumTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    ScaleSpectrumTexture& output) {
  TryRemoveSpectrumTextureV1(
      parameters, "tex1",
      std::bind(&ScaleSpectrumTexture::mutable_tex1, &output));
  TryRemoveSpectrumTextureV1(
      parameters, "tex2",
      std::bind(&ScaleSpectrumTexture::mutable_tex2, &output));
}

void RemoveWrinkledFloatTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    WrinkledFloatTexture& output) {
  if (std::optional<int32_t> octaves = TryRemoveInteger(parameters, "octaves");
      octaves) {
    output.set_octaves(std::max(0, *octaves));
  }

  if (std::optional<double> roughness = TryRemoveFloat(parameters, "roughness");
      roughness) {
    output.set_roughness(*roughness);
  }
}

void RemoveWrinkledSpectrumTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    WrinkledSpectrumTexture& output) {
  if (std::optional<int32_t> octaves = TryRemoveInteger(parameters, "octaves");
      octaves) {
    output.set_octaves(std::max(0, *octaves));
  }

  if (std::optional<double> roughness = TryRemoveFloat(parameters, "roughness");
      roughness) {
    output.set_roughness(*roughness);
  }
}

void RemoveUvSpectrumTextureV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    UvSpectrumTexture& output) {
  TryRemoveUVParameters(parameters, "uv", output);
}

}  // namespace pbrt_proto
