#include "pbrt_proto/shared/films.h"

#include "absl/container/flat_hash_map.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/enums.h"
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
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view type_name, T& output) {
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

  if (absl::Status status =
          RemoveEnum(parameters, /*pbrt_version=*/4, type_name, "sensor",
                     std::bind(&T::set_sensor, &output, std::placeholders::_1));
      !status.ok()) {
    return status;
  }

  if (std::optional<double> maxcomponentvalue =
          TryRemoveFloat(parameters, "maxcomponentvalue");
      maxcomponentvalue.has_value()) {
    output.set_maxcomponentvalue(*maxcomponentvalue);
  }

  return absl::OkStatus();
}

}  // namespace

absl::Status RemoveRgbFilm(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, RgbFilm& output) {
  if (pbrt_version == 1) {
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
  } else if (pbrt_version == 2) {
    if (std::optional<bool> display = TryRemoveBool(parameters, "display");
        display.has_value()) {
      output.set_display(*display);
    }

  } else if (pbrt_version == 3) {
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
  } else if (pbrt_version >= 4) {
    if (absl::Status status = RemoveCommonV4(parameters, "rgb", output);
        !status.ok()) {
      return status;
    }
  }

  // Must be done after RemoveCommonV4
  RemoveUniversal(parameters, output);

  return absl::OkStatus();
}

absl::Status RemoveGBufferFilm(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, GBufferFilm& output) {
  if (absl::Status status = RemoveCommonV4(parameters, "gbuffer", output);
      !status.ok()) {
    return status;
  }

  if (absl::Status status =
          RemoveEnum(parameters, pbrt_version, "gbuffer", "coordinatesystem",
                     std::bind(&GBufferFilm::set_coordinatesystem, &output,
                               std::placeholders::_1));
      !status.ok()) {
    return status;
  }

  // Must be done after RemoveCommonV4
  RemoveUniversal(parameters, output);

  return absl::OkStatus();
}

absl::Status RemoveSpectralFilm(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, SpectralFilm& output) {
  if (absl::Status status = RemoveCommonV4(parameters, "spectral", output);
      !status.ok()) {
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

  // Must be done after RemoveCommonV4
  RemoveUniversal(parameters, output);

  return absl::OkStatus();
}

}  // namespace pbrt_proto
