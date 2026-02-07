#include "pbrt_proto/shared/cameras.h"

#include <algorithm>
#include <functional>

#include "absl/container/flat_hash_map.h"
#include "absl/functional/function_ref.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/common.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {
namespace {

void MaybeRemoveCameraScreenWindow(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::FunctionRef<CameraScreenWindow*()> get_output) {
  std::optional<absl::Span<double>> screenwindow;
  if (absl::Status status =
          TryRemoveFloats(parameters, "screenwindow", 4, screenwindow);
      status.ok() && screenwindow.has_value()) {
    CameraScreenWindow* output = get_output();
    output->set_x_min((*screenwindow)[0]);
    output->set_x_max((*screenwindow)[1]);
    output->set_y_min((*screenwindow)[2]);
    output->set_y_max((*screenwindow)[3]);
  }
}

void RemoveRealisticCamera(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    bool parse_simpleweighting, bool parse_aperture, RealisticCamera& output) {
  if (std::optional<double> shutteropen =
          TryRemoveFloat(parameters, "shutteropen");
      shutteropen.has_value()) {
    output.set_shutteropen(*shutteropen);
  }

  if (std::optional<double> shutterclose =
          TryRemoveFloat(parameters, "shutterclose");
      shutterclose.has_value()) {
    output.set_shutterclose(*shutterclose);
  }

  if (std::optional<absl::string_view> lensfile =
          TryRemoveString(parameters, "lensfile");
      lensfile.has_value()) {
    output.set_lensfile(*lensfile);
  }

  if (std::optional<double> aperturediameter =
          TryRemoveFloat(parameters, "aperturediameter");
      aperturediameter.has_value()) {
    output.set_aperturediameter(*aperturediameter);
  }

  if (parse_aperture) {
    if (std::optional<absl::string_view> aperture =
            TryRemoveString(parameters, "aperture");
        aperture.has_value()) {
      output.set_aperture(*aperture);
    }
  }

  if (std::optional<double> focusdistance =
          TryRemoveFloat(parameters, "focusdistance");
      focusdistance.has_value()) {
    output.set_focusdistance(*focusdistance);
  }

  if (parse_simpleweighting) {
    if (std::optional<bool> simpleweighting =
            TryRemoveBool(parameters, "simpleweighting");
        simpleweighting.has_value()) {
      output.set_simpleweighting(*simpleweighting);
    }
  }
}

}  // namespace

void RemoveOrthographicCameraV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    OrthographicCamera& output) {
  RemoveOrthographicCameraV2(parameters, output);

  if (std::optional<double> hither = TryRemoveFloat(parameters, "hither");
      hither.has_value()) {
    output.set_hither(*hither);
  }

  if (std::optional<double> yon = TryRemoveFloat(parameters, "yon");
      yon.has_value()) {
    output.set_yon(*yon);
  }
}

void RemoveOrthographicCameraV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    OrthographicCamera& output) {
  if (std::optional<double> shutteropen =
          TryRemoveFloat(parameters, "shutteropen");
      shutteropen.has_value()) {
    output.set_shutteropen(*shutteropen);
  }

  if (std::optional<double> shutterclose =
          TryRemoveFloat(parameters, "shutterclose");
      shutterclose.has_value()) {
    output.set_shutterclose(*shutterclose);
  }

  if (std::optional<double> lensradius =
          TryRemoveFloat(parameters, "lensradius");
      lensradius.has_value()) {
    output.set_lensradius(*lensradius);
  }

  if (std::optional<double> focaldistance =
          TryRemoveFloat(parameters, "focaldistance");
      focaldistance.has_value()) {
    output.set_focaldistance(*focaldistance);
  }

  if (std::optional<double> frameaspectratio =
          TryRemoveFloat(parameters, "frameaspectratio");
      frameaspectratio.has_value()) {
    output.set_frameaspectratio(*frameaspectratio);
  }

  MaybeRemoveCameraScreenWindow(
      parameters,
      std::bind(&OrthographicCamera::mutable_screenwindow, &output));
}

void RemovePerspectiveCameraV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PerspectiveCamera& output) {
  RemovePerspectiveCameraV3(parameters, output);

  if (std::optional<double> hither = TryRemoveFloat(parameters, "hither");
      hither.has_value()) {
    output.set_hither(*hither);
  }

  if (std::optional<double> yon = TryRemoveFloat(parameters, "yon");
      yon.has_value()) {
    output.set_yon(*yon);
  }
}

void RemovePerspectiveCameraV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PerspectiveCamera& output) {
  RemovePerspectiveCameraV3(parameters, output);

  if (std::optional<double> halffov = TryRemoveFloat(parameters, "halffov");
      halffov.has_value() && *halffov > 0.0) {
    output.set_fov(*halffov * 2.0);
  }
}

void RemovePerspectiveCameraV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PerspectiveCamera& output) {
  if (std::optional<double> shutteropen =
          TryRemoveFloat(parameters, "shutteropen");
      shutteropen.has_value()) {
    output.set_shutteropen(*shutteropen);
  }

  if (std::optional<double> shutterclose =
          TryRemoveFloat(parameters, "shutterclose");
      shutterclose.has_value()) {
    output.set_shutterclose(*shutterclose);
  }

  if (std::optional<double> fov = TryRemoveFloat(parameters, "fov");
      fov.has_value()) {
    output.set_fov(*fov);
  }

  if (std::optional<double> lensradius =
          TryRemoveFloat(parameters, "lensradius");
      lensradius.has_value()) {
    output.set_lensradius(*lensradius);
  }

  if (std::optional<double> focaldistance =
          TryRemoveFloat(parameters, "focaldistance");
      focaldistance.has_value()) {
    output.set_focaldistance(*focaldistance);
  }

  if (std::optional<double> frameaspectratio =
          TryRemoveFloat(parameters, "frameaspectratio");
      frameaspectratio.has_value()) {
    output.set_frameaspectratio(*frameaspectratio);
  }

  MaybeRemoveCameraScreenWindow(
      parameters, std::bind(&PerspectiveCamera::mutable_screenwindow, &output));
}

void RemoveSphericalCameraV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SphericalCamera& output) {
  RemoveSphericalCameraV2(parameters, output);

  if (std::optional<double> hither = TryRemoveFloat(parameters, "hither");
      hither.has_value()) {
    output.set_hither(*hither);
  }

  if (std::optional<double> yon = TryRemoveFloat(parameters, "yon");
      yon.has_value()) {
    output.set_yon(*yon);
  }
}

void RemoveSphericalCameraV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SphericalCamera& output) {
  if (std::optional<double> shutteropen =
          TryRemoveFloat(parameters, "shutteropen");
      shutteropen.has_value()) {
    output.set_shutteropen(*shutteropen);
  }

  if (std::optional<double> shutterclose =
          TryRemoveFloat(parameters, "shutterclose");
      shutterclose.has_value()) {
    output.set_shutterclose(*shutterclose);
  }

  if (std::optional<double> lensradius =
          TryRemoveFloat(parameters, "lensradius");
      lensradius.has_value()) {
    output.set_lensradius(*lensradius);
  }

  if (std::optional<double> focaldistance =
          TryRemoveFloat(parameters, "focaldistance");
      focaldistance.has_value()) {
    output.set_focaldistance(*focaldistance);
  }

  if (std::optional<double> frameaspectratio =
          TryRemoveFloat(parameters, "frameaspectratio");
      frameaspectratio.has_value()) {
    output.set_frameaspectratio(*frameaspectratio);
  }

  MaybeRemoveCameraScreenWindow(
      parameters, std::bind(&SphericalCamera::mutable_screenwindow, &output));
}

absl::Status RemoveSphericalCameraV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SphericalCamera& output) {
  RemoveSphericalCameraV2(parameters, output);

  if (std::optional<absl::string_view> mapping =
          TryRemoveString(parameters, "mapping");
      mapping.has_value()) {
    if (*mapping == "equalarea") {
      output.set_mapping(SphericalCamera::EQUALAREA);
    } else if (*mapping == "equirectangular") {
      output.set_mapping(SphericalCamera::EQUIRECTANGULAR);
    } else {
      return absl::InvalidArgumentError(
          "A spherical Camera specified an invalid 'mapping'");
    }
  }

  return absl::OkStatus();
}

void RemoveRealisticCameraV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    RealisticCamera& output) {
  RemoveRealisticCamera(parameters, /*parse_simpleweighting=*/true,
                        /*parse_aperture=*/false, output);
}

void RemoveRealisticCameraV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    RealisticCamera& output) {
  RemoveRealisticCamera(parameters, /*parse_simpleweighting=*/false,
                        /*parse_aperture=*/true, output);
}

}  // namespace pbrt_proto
