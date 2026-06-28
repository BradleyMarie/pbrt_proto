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
#include "pbrt_proto/shared/enums.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {
namespace {

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
absl::Status TryRemoveUVParameters(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, absl::string_view type, T& output) {
  if (absl::Status status =
          RemoveEnum(parameters, pbrt_version, type, "mapping",
                     std::bind(&T::set_mapping, &output, std::placeholders::_1),
                     TextureMapping::UV);
      !status.ok()) {
    return status;
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

  return absl::OkStatus();
}

}  // namespace

absl::Status RemoveBilerpFloatTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, BilerpFloatTexture& output) {
  if (absl::Status status =
          TryRemoveUVParameters(parameters, pbrt_version, "bilerp", output);
      !status.ok()) {
    return status;
  }

  TryRemoveFloatTexture(parameters, "v00",
                        std::bind(&BilerpFloatTexture::mutable_v00, &output));
  TryRemoveFloatTexture(parameters, "v01",
                        std::bind(&BilerpFloatTexture::mutable_v01, &output));
  TryRemoveFloatTexture(parameters, "v10",
                        std::bind(&BilerpFloatTexture::mutable_v10, &output));
  TryRemoveFloatTexture(parameters, "v11",
                        std::bind(&BilerpFloatTexture::mutable_v11, &output));
  return absl::OkStatus();
}

absl::Status RemoveBilerpSpectrumTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, BilerpSpectrumTexture& output) {
  if (absl::Status status =
          TryRemoveUVParameters(parameters, pbrt_version, "bilerp", output);
      !status.ok()) {
    return status;
  }

  if (absl::Status status = TryRemoveSpectrumTexture(
          parameters, "v00",
          std::bind(&BilerpSpectrumTexture::mutable_v00, &output));
      !status.ok()) {
    return status;
  }

  if (absl::Status status = TryRemoveSpectrumTexture(
          parameters, "v01",
          std::bind(&BilerpSpectrumTexture::mutable_v01, &output));
      !status.ok()) {
    return status;
  }

  if (absl::Status status = TryRemoveSpectrumTexture(
          parameters, "v10",
          std::bind(&BilerpSpectrumTexture::mutable_v10, &output));
      !status.ok()) {
    return status;
  }

  if (absl::Status status = TryRemoveSpectrumTexture(
          parameters, "v11",
          std::bind(&BilerpSpectrumTexture::mutable_v11, &output));
      !status.ok()) {
    return status;
  }

  return absl::OkStatus();
}

absl::Status RemoveCheckerboard2DFloatTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, Checkerboard2DFloatTexture& output) {
  if (absl::Status status = TryRemoveUVParameters(parameters, pbrt_version,
                                                  "checkerboard", output);
      !status.ok()) {
    return status;
  }

  TryRemoveFloatTexture(
      parameters, "tex1",
      std::bind(&Checkerboard2DFloatTexture::mutable_tex1, &output));
  TryRemoveFloatTexture(
      parameters, "tex2",
      std::bind(&Checkerboard2DFloatTexture::mutable_tex2, &output));

  if (pbrt_version <= 3) {
    if (absl::Status status =
            RemoveEnum(parameters, pbrt_version, "checkerboard", "aamode",
                       std::bind(&Checkerboard2DFloatTexture::set_aamode,
                                 &output, std::placeholders::_1),
                       CheckerboardAaMode::CLOSEDFORM);
        !status.ok()) {
      return status;
    }
  }

  return absl::OkStatus();
}

absl::Status RemoveCheckerboard2DSpectrumTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, Checkerboard2DSpectrumTexture& output) {
  if (absl::Status status = TryRemoveUVParameters(parameters, pbrt_version,
                                                  "checkerboard", output);
      !status.ok()) {
    return status;
  }

  if (absl::Status status = TryRemoveSpectrumTexture(
          parameters, "tex1",
          std::bind(&Checkerboard2DSpectrumTexture::mutable_tex1, &output));
      !status.ok()) {
    return status;
  }

  if (absl::Status status = TryRemoveSpectrumTexture(
          parameters, "tex2",
          std::bind(&Checkerboard2DSpectrumTexture::mutable_tex2, &output));
      !status.ok()) {
    return status;
  }

  if (pbrt_version <= 3) {
    if (absl::Status status =
            RemoveEnum(parameters, pbrt_version, "checkerboard", "aamode",
                       std::bind(&Checkerboard2DSpectrumTexture::set_aamode,
                                 &output, std::placeholders::_1),
                       CheckerboardAaMode::CLOSEDFORM);
        !status.ok()) {
      return status;
    }
  }

  return absl::OkStatus();
}

absl::Status RemoveCheckerboard3DFloatTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, Checkerboard3DFloatTexture& output) {
  TryRemoveFloatTexture(
      parameters, "tex1",
      std::bind(&Checkerboard3DFloatTexture::mutable_tex1, &output));
  TryRemoveFloatTexture(
      parameters, "tex2",
      std::bind(&Checkerboard3DFloatTexture::mutable_tex2, &output));
  return absl::OkStatus();
}

absl::Status RemoveCheckerboard3DSpectrumTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, Checkerboard3DSpectrumTexture& output) {
  if (absl::Status status = TryRemoveSpectrumTexture(
          parameters, "tex1",
          std::bind(&Checkerboard3DSpectrumTexture::mutable_tex1, &output));
      !status.ok()) {
    return status;
  }

  if (absl::Status status = TryRemoveSpectrumTexture(
          parameters, "tex2",
          std::bind(&Checkerboard3DSpectrumTexture::mutable_tex2, &output));
      !status.ok()) {
    return status;
  }

  return absl::OkStatus();
}

absl::Status RemoveConstantFloatTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, ConstantFloatTexture& output) {
  if (std::optional<double> value = TryRemoveFloat(parameters, "value");
      value) {
    output.set_value(*value);
  }

  return absl::OkStatus();
}

absl::Status RemoveConstantSpectrumTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, ConstantSpectrumTexture& output) {
  return TryRemoveSpectrum(
      parameters, "value",
      std::bind(&ConstantSpectrumTexture::mutable_value, &output));
}

absl::Status RemoveDirectionMixFloatTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, DirectionMixFloatTexture& output) {
  RemoveDir(parameters, output);
  TryRemoveFloatTexture(
      parameters, "tex1",
      std::bind(&DirectionMixFloatTexture::mutable_tex1, &output));
  TryRemoveFloatTexture(
      parameters, "tex2",
      std::bind(&DirectionMixFloatTexture::mutable_tex2, &output));
  return absl::OkStatus();
}

absl::Status RemoveDirectionMixSpectrumTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, DirectionMixSpectrumTexture& output) {
  RemoveDir(parameters, output);

  if (absl::Status status = TryRemoveSpectrumTexture(
          parameters, "tex1",
          std::bind(&DirectionMixSpectrumTexture::mutable_tex1, &output));
      !status.ok()) {
    return status;
  }

  if (absl::Status status = TryRemoveSpectrumTexture(
          parameters, "tex2",
          std::bind(&DirectionMixSpectrumTexture::mutable_tex2, &output));
      !status.ok()) {
    return status;
  }

  return absl::OkStatus();
}

absl::Status RemoveDotsFloatTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, DotsFloatTexture& output) {
  if (absl::Status status =
          TryRemoveUVParameters(parameters, pbrt_version, "dots", output);
      !status.ok()) {
    return status;
  }

  TryRemoveFloatTexture(parameters, "inside",
                        std::bind(&DotsFloatTexture::mutable_inside, &output));
  TryRemoveFloatTexture(parameters, "outside",
                        std::bind(&DotsFloatTexture::mutable_outside, &output));
  return absl::OkStatus();
}

absl::Status RemoveDotsSpectrumTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, DotsSpectrumTexture& output) {
  if (absl::Status status =
          TryRemoveUVParameters(parameters, pbrt_version, "dots", output);
      !status.ok()) {
    return status;
  }

  if (absl::Status status = TryRemoveSpectrumTexture(
          parameters, "inside",
          std::bind(&DotsSpectrumTexture::mutable_inside, &output));
      !status.ok()) {
    return status;
  }

  if (absl::Status status = TryRemoveSpectrumTexture(
          parameters, "outside",
          std::bind(&DotsSpectrumTexture::mutable_outside, &output));
      !status.ok()) {
    return status;
  }

  return absl::OkStatus();
}

absl::Status RemoveFBmFloatTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, FBmFloatTexture& output) {
  if (std::optional<int32_t> octaves = TryRemoveInteger(parameters, "octaves");
      octaves) {
    output.set_octaves(std::max(0, *octaves));
  }

  if (std::optional<double> roughness = TryRemoveFloat(parameters, "roughness");
      roughness) {
    output.set_roughness(*roughness);
  }

  return absl::OkStatus();
}

absl::Status RemoveFBmSpectrumTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, FBmSpectrumTexture& output) {
  if (std::optional<int32_t> octaves = TryRemoveInteger(parameters, "octaves");
      octaves) {
    output.set_octaves(std::max(0, *octaves));
  }

  if (std::optional<double> roughness = TryRemoveFloat(parameters, "roughness");
      roughness) {
    output.set_roughness(*roughness);
  }

  return absl::OkStatus();
}

absl::Status RemoveImageMapFloatTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, ImageMapFloatTexture& output) {
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

  if (absl::Status status =
          RemoveEnum(parameters, pbrt_version, "imagemap", "wrap",
                     std::bind(&ImageMapFloatTexture::set_wrap, &output,
                               std::placeholders::_1),
                     ImageWrap::REPEAT);
      !status.ok()) {
    return status;
  }

  if (absl::Status status =
          TryRemoveUVParameters(parameters, pbrt_version, "imagemap", output);
      !status.ok()) {
    return status;
  }

  if (pbrt_version <= 3) {
    if (std::optional<bool> trilinear = TryRemoveBool(parameters, "trilinear");
        trilinear) {
      output.set_filter(TextureFilter::TRILINEAR);
    }
  }

  if (pbrt_version >= 2) {
    if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
        scale) {
      output.set_scale(*scale);
    }
  }

  if (pbrt_version == 2) {
    if (std::optional<double> gamma = TryRemoveFloat(parameters, "gamma");
        gamma) {
      output.set_gamma(*gamma);
    }
  }

  if (pbrt_version == 3) {
    if (std::optional<bool> gamma = TryRemoveBool(parameters, "gamma"); gamma) {
      output.set_encoding(*gamma ? ImageEncoding::GAMMA : ImageEncoding::SRGB);
    }
  }

  if (pbrt_version >= 4) {
    if (absl::Status status =
            RemoveEnum(parameters, pbrt_version, "imagemap", "filter",
                       std::bind(&ImageMapFloatTexture::set_filter, &output,
                                 std::placeholders::_1),
                       TextureFilter::EWA);
        !status.ok()) {
      return status;
    }

    if (std::optional<bool> invert = TryRemoveBool(parameters, "invert");
        invert) {
      output.set_invert(*invert);
    }

    if (absl::Status status =
            RemoveEncoding(parameters, "imagemap Texture", output);
        !status.ok()) {
      return status;
    }
  }

  return absl::OkStatus();
}

absl::Status RemoveImageMapSpectrumTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, ImageMapSpectrumTexture& output) {
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

  if (absl::Status status =
          RemoveEnum(parameters, pbrt_version, "imagemap", "wrap",
                     std::bind(&ImageMapSpectrumTexture::set_wrap, &output,
                               std::placeholders::_1),
                     ImageWrap::REPEAT);
      !status.ok()) {
    return status;
  }

  if (absl::Status status =
          TryRemoveUVParameters(parameters, pbrt_version, "imagemap", output);
      !status.ok()) {
    return status;
  }

  if (pbrt_version <= 3) {
    if (std::optional<bool> trilinear = TryRemoveBool(parameters, "trilinear");
        trilinear) {
      output.set_filter(TextureFilter::TRILINEAR);
    }
  }

  if (pbrt_version >= 2) {
    if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
        scale) {
      output.set_scale(*scale);
    }
  }

  if (pbrt_version == 2) {
    if (std::optional<double> gamma = TryRemoveFloat(parameters, "gamma");
        gamma) {
      output.set_gamma(*gamma);
    }
  }

  if (pbrt_version == 3) {
    if (std::optional<bool> gamma = TryRemoveBool(parameters, "gamma"); gamma) {
      output.set_encoding(*gamma ? ImageEncoding::GAMMA : ImageEncoding::SRGB);
    }
  }

  if (pbrt_version >= 4) {
    if (absl::Status status =
            RemoveEnum(parameters, pbrt_version, "imagemap", "filter",
                       std::bind(&ImageMapSpectrumTexture::set_filter, &output,
                                 std::placeholders::_1),
                       TextureFilter::EWA);
        !status.ok()) {
      return status;
    }

    if (std::optional<bool> invert = TryRemoveBool(parameters, "invert");
        invert) {
      output.set_invert(*invert);
    }

    if (absl::Status status =
            RemoveEncoding(parameters, "imagemap Texture", output);
        !status.ok()) {
      return status;
    }
  }

  return absl::OkStatus();
}

absl::Status RemoveMarbleSpectrumTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, MarbleSpectrumTexture& output) {
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

  return absl::OkStatus();
}

absl::Status RemoveMixFloatTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, MixFloatTexture& output) {
  TryRemoveFloatTexture(parameters, "amount",
                        std::bind(&MixFloatTexture::mutable_amount, &output));
  TryRemoveFloatTexture(parameters, "tex1",
                        std::bind(&MixFloatTexture::mutable_tex1, &output));
  TryRemoveFloatTexture(parameters, "tex2",
                        std::bind(&MixFloatTexture::mutable_tex2, &output));
  return absl::OkStatus();
}

absl::Status RemoveMixSpectrumTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, MixSpectrumTexture& output) {
  TryRemoveFloatTexture(
      parameters, "amount",
      std::bind(&MixSpectrumTexture::mutable_amount, &output));

  if (absl::Status status = TryRemoveSpectrumTexture(
          parameters, "tex1",
          std::bind(&MixSpectrumTexture::mutable_tex1, &output));
      !status.ok()) {
    return status;
  }

  if (absl::Status status = TryRemoveSpectrumTexture(
          parameters, "tex2",
          std::bind(&MixSpectrumTexture::mutable_tex2, &output));
      !status.ok()) {
    return status;
  }

  return absl::OkStatus();
}

absl::Status RemovePtexFloatTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, PtexFloatTexture& output) {
  if (std::optional<absl::string_view> filename =
          TryRemoveString(parameters, "filename");
      filename) {
    output.set_filename(*filename);
  }

  if (pbrt_version == 3) {
    if (std::optional<double> gamma = TryRemoveFloat(parameters, "gamma");
        gamma) {
      output.set_gamma(*gamma);
    }
  }

  if (pbrt_version >= 4) {
    if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
        scale) {
      output.set_scale(*scale);
    }

    if (absl::Status status =
            RemoveEncoding(parameters, "ptex Texture", output);
        !status.ok()) {
      return status;
    }
  }

  return absl::OkStatus();
}

absl::Status RemovePtexSpectrumTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, PtexSpectrumTexture& output) {
  if (std::optional<absl::string_view> filename =
          TryRemoveString(parameters, "filename");
      filename) {
    output.set_filename(*filename);
  }

  if (pbrt_version == 3) {
    if (std::optional<double> gamma = TryRemoveFloat(parameters, "gamma");
        gamma) {
      output.set_gamma(*gamma);
    }
  }

  if (pbrt_version >= 4) {
    if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
        scale) {
      output.set_scale(*scale);
    }

    if (absl::Status status =
            RemoveEncoding(parameters, "ptex Texture", output);
        !status.ok()) {
      return status;
    }
  }

  return absl::OkStatus();
}

absl::Status RemoveScaleFloatTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, ScaleFloatTexture& output) {
  TryRemoveFloatTexture(parameters, "tex1",
                        std::bind(&ScaleFloatTexture::mutable_tex1, &output));
  TryRemoveFloatTexture(parameters, "tex2",
                        std::bind(&ScaleFloatTexture::mutable_tex2, &output));
  return absl::OkStatus();
}

absl::Status RemoveScaleSpectrumTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, ScaleSpectrumTexture& output) {
  if (absl::Status status = TryRemoveSpectrumTexture(
          parameters, "tex1",
          std::bind(&ScaleSpectrumTexture::mutable_tex1, &output));
      !status.ok()) {
    return status;
  }

  if (absl::Status status = TryRemoveSpectrumTexture(
          parameters, "tex2",
          std::bind(&ScaleSpectrumTexture::mutable_tex2, &output));
      !status.ok()) {
    return status;
  }

  return absl::OkStatus();
}

absl::Status RemoveWrinkledFloatTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, WrinkledFloatTexture& output) {
  if (std::optional<int32_t> octaves = TryRemoveInteger(parameters, "octaves");
      octaves) {
    output.set_octaves(std::max(0, *octaves));
  }

  if (std::optional<double> roughness = TryRemoveFloat(parameters, "roughness");
      roughness) {
    output.set_roughness(*roughness);
  }

  return absl::OkStatus();
}

absl::Status RemoveWrinkledSpectrumTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, WrinkledSpectrumTexture& output) {
  if (std::optional<int32_t> octaves = TryRemoveInteger(parameters, "octaves");
      octaves) {
    output.set_octaves(std::max(0, *octaves));
  }

  if (std::optional<double> roughness = TryRemoveFloat(parameters, "roughness");
      roughness) {
    output.set_roughness(*roughness);
  }

  return absl::OkStatus();
}

absl::Status RemoveUvSpectrumTexture(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, UvSpectrumTexture& output) {
  if (absl::Status status =
          TryRemoveUVParameters(parameters, pbrt_version, "uv", output);
      !status.ok()) {
    return status;
  }

  return absl::OkStatus();
}

}  // namespace pbrt_proto
