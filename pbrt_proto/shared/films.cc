#include "pbrt_proto/shared/films.h"

#include "absl/container/flat_hash_map.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {
namespace {

template <typename T>
void RemoveUniversal(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters, T& output) {
  if (std::optional<absl::string_view> filename =
          TryRemoveString(parameters, "filename");
      filename.has_value()) {
    output.set_filename(*filename);
  }

  if (std::optional<int32_t> xresolution =
          TryRemoveInteger(parameters, "xresolution");
      xresolution.has_value()) {
    output.set_xresolution(std::max(0, *xresolution));
  }

  if (std::optional<int32_t> yresolution =
          TryRemoveInteger(parameters, "yresolution");
      yresolution.has_value()) {
    output.set_yresolution(std::max(0, *yresolution));
  }

  std::optional<absl::Span<double>> cropwindow;
  if (absl::Status status =
          TryRemoveFloats(parameters, "cropwindow", 4, cropwindow);
      status.ok() && cropwindow.has_value()) {
    output.mutable_cropwindow()->set_x_min((*cropwindow)[0]);
    output.mutable_cropwindow()->set_x_max((*cropwindow)[1]);
    output.mutable_cropwindow()->set_y_min((*cropwindow)[2]);
    output.mutable_cropwindow()->set_y_max((*cropwindow)[3]);
  }
}

template <typename T>
absl::Status RemoveCommonV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters, T& output) {
  // Parse `pixelbounds` first since `cropwindow` should override it.
  std::optional<absl::Span<int32_t>> pixelbounds;
  if (absl::Status status =
          TryRemoveIntegers(parameters, "pixelbounds", 4, pixelbounds);
      status.ok() && pixelbounds.has_value()) {
    output.mutable_pixelbounds()->set_x_min((std::max(0, (*pixelbounds)[0])));
    output.mutable_pixelbounds()->set_x_max((std::max(0, (*pixelbounds)[1])));
    output.mutable_pixelbounds()->set_y_min((std::max(0, (*pixelbounds)[2])));
    output.mutable_pixelbounds()->set_y_max((std::max(0, (*pixelbounds)[3])));
  }

  RemoveUniversal(parameters, output);

  if (std::optional<double> diagonal = TryRemoveFloat(parameters, "diagonal");
      diagonal.has_value()) {
    output.set_diagonal(*diagonal);
  }

  if (std::optional<bool> savefp16 = TryRemoveBool(parameters, "savefp16");
      savefp16) {
    output.set_savefp16(*savefp16);
  }

  if (std::optional<double> iso = TryRemoveFloat(parameters, "iso");
      iso.has_value()) {
    output.set_iso(*iso);
  }

  if (std::optional<double> whitebalance =
          TryRemoveFloat(parameters, "whitebalance");
      whitebalance.has_value()) {
    output.set_whitebalance(*whitebalance);
  }

  if (std::optional<absl::string_view> sensor =
          TryRemoveString(parameters, "sensor");
      sensor.has_value()) {
    if (*sensor == "cie_1931") {
      output.set_sensor(FilmSensor::CIE_1931);
    } else if (*sensor == "canon_eos_100d") {
      output.set_sensor(FilmSensor::CANON_EOS_100D);
    } else if (*sensor == "canon_eos_1dx_mkii") {
      output.set_sensor(FilmSensor::CANON_EOS_1DX_MKII);
    } else if (*sensor == "canon_eos_200d_mkii") {
      output.set_sensor(FilmSensor::CANON_EOS_200D_MKII);
    } else if (*sensor == "canon_eos_200d") {
      output.set_sensor(FilmSensor::CANON_EOS_200D);
    } else if (*sensor == "canon_eos_5d_mkii") {
      output.set_sensor(FilmSensor::CANON_EOS_5D_MKII);
    } else if (*sensor == "canon_eos_5d_mkiii") {
      output.set_sensor(FilmSensor::CANON_EOS_5D_MKIII);
    } else if (*sensor == "canon_eos_5d_mkiv") {
      output.set_sensor(FilmSensor::CANON_EOS_5D_MKIV);
    } else if (*sensor == "canon_eos_5d") {
      output.set_sensor(FilmSensor::CANON_EOS_5D);
    } else if (*sensor == "canon_eos_5ds") {
      output.set_sensor(FilmSensor::CANON_EOS_5DS);
    } else if (*sensor == "canon_eos_m") {
      output.set_sensor(FilmSensor::CANON_EOS_M);
    } else if (*sensor == "hasselblad_l1d_20c") {
      output.set_sensor(FilmSensor::HASSELBLAD_L1D_20C);
    } else if (*sensor == "nikon_d810") {
      output.set_sensor(FilmSensor::NIKON_D810);
    } else if (*sensor == "nikon_d850") {
      output.set_sensor(FilmSensor::NIKON_D850);
    } else if (*sensor == "sony_ilce_6400") {
      output.set_sensor(FilmSensor::SONY_ILCE_6400);
    } else if (*sensor == "sony_ilce_7m3") {
      output.set_sensor(FilmSensor::SONY_ILCE_7M3);
    } else if (*sensor == "sony_ilce_7rm3") {
      output.set_sensor(FilmSensor::SONY_ILCE_7RM3);
    } else if (*sensor == "sony_ilce_9") {
      output.set_sensor(FilmSensor::SONY_ILCE_9);
    } else {
      return absl::InvalidArgumentError("A Film specified an invalid 'sensor'");
    }
  }

  if (std::optional<double> maxcomponentvalue =
          TryRemoveFloat(parameters, "maxcomponentvalue");
      maxcomponentvalue.has_value()) {
    output.set_maxcomponentvalue(*maxcomponentvalue);
  }

  return absl::OkStatus();
}

}  // namespace

void RemoveRgbFilmV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    RgbFilm& output) {
  RemoveUniversal(parameters, output);

  if (std::optional<bool> premultiplyalpha =
          TryRemoveBool(parameters, "premultiplyalpha");
      premultiplyalpha) {
    output.set_premultiplyalpha(*premultiplyalpha);
  }

  if (std::optional<int32_t> writefrequency =
          TryRemoveInteger(parameters, "writefrequency");
      writefrequency.has_value() && *writefrequency >= 0) {
    output.set_writefrequency(*writefrequency);
  }
}

void RemoveRgbFilmV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    RgbFilm& output) {
  RemoveUniversal(parameters, output);

  if (std::optional<bool> display = TryRemoveBool(parameters, "display");
      display.has_value()) {
    output.set_display(*display);
  }
}

void RemoveRgbFilmV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    RgbFilm& output) {
  RemoveUniversal(parameters, output);

  if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
      scale.has_value()) {
    output.set_scale(*scale);
  }

  if (std::optional<double> maxsampleluminance =
          TryRemoveFloat(parameters, "maxsampleluminance");
      maxsampleluminance.has_value()) {
    output.set_maxsampleluminance(*maxsampleluminance);
  }

  if (std::optional<double> diagonal = TryRemoveFloat(parameters, "diagonal");
      diagonal.has_value()) {
    output.set_diagonal(*diagonal);
  }
}

absl::Status RemoveRgbFilmV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    RgbFilm& output) {
  return RemoveCommonV4(parameters, output);
}

absl::Status RemoveGBufferFilmV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    GBufferFilm& output) {
  if (absl::Status status = RemoveCommonV4(parameters, output); !status.ok()) {
    return status;
  }

  if (std::optional<absl::string_view> coordinatesystem =
          TryRemoveString(parameters, "coordinatesystem");
      coordinatesystem.has_value()) {
    if (*coordinatesystem == "camera") {
      output.set_coordinatesystem(GBufferFilm::CAMERA);
    } else if (*coordinatesystem == "world") {
      output.set_coordinatesystem(GBufferFilm::WORLD);
    } else {
      return absl::InvalidArgumentError(
          "A gbuffer Film specified an invalid 'coordinatesystem'");
    }
  }

  return absl::OkStatus();
}

absl::Status RemoveSpectralFilmV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SpectralFilm& output) {
  if (absl::Status status = RemoveCommonV4(parameters, output); !status.ok()) {
    return status;
  }

  if (std::optional<int32_t> nbuckets =
          TryRemoveInteger(parameters, "nbuckets");
      nbuckets.has_value()) {
    output.set_nbuckets(std::max(0, *nbuckets));
  }

  if (std::optional<double> lambdamin = TryRemoveFloat(parameters, "lambdamin");
      lambdamin.has_value()) {
    output.set_lambdamin(*lambdamin);
  }

  if (std::optional<double> lambdamax = TryRemoveFloat(parameters, "lambdamax");
      lambdamax.has_value()) {
    output.set_lambdamax(*lambdamax);
  }

  return absl::OkStatus();
}

}  // namespace pbrt_proto
