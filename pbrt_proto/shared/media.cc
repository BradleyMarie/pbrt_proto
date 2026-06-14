#include "pbrt_proto/shared/media.h"

#include "absl/container/flat_hash_map.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/common.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {
namespace {

template <typename T>
absl::Status RemoveSigma(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, T& output) {
  if (absl::Status status = TryRemoveSpectrum(
          parameters, "sigma_a", std::bind(&T::mutable_sigma_a, &output));
      !status.ok()) {
    return status;
  }

  return TryRemoveSpectrum(parameters, "sigma_s",
                           std::bind(&T::mutable_sigma_s, &output));
}

template <typename T>
void RemoveBounds(absl::flat_hash_map<absl::string_view, Parameter>& parameters,
                  T& output) {
  if (std::optional<std::array<double, 3>> p0 =
          TryRemovePoint3(parameters, "p0");
      p0.has_value()) {
    output.mutable_p0()->set_x((*p0)[0]);
    output.mutable_p0()->set_y((*p0)[1]);
    output.mutable_p0()->set_z((*p0)[2]);
  }

  if (std::optional<std::array<double, 3>> p1 =
          TryRemovePoint3(parameters, "p1");
      p1.has_value()) {
    output.mutable_p1()->set_x((*p1)[0]);
    output.mutable_p1()->set_y((*p1)[1]);
    output.mutable_p1()->set_z((*p1)[2]);
  }
}

}  // namespace

absl::Status RemoveCloudMedium(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, CloudMedium& output) {
  if (std::optional<double> g = TryRemoveFloat(parameters, "g");
      g.has_value()) {
    output.set_g(*g);
  }

  if (std::optional<std::array<double, 3>> p0 =
          TryRemovePoint3(parameters, "p0");
      p0.has_value()) {
    output.mutable_p0()->set_x((*p0)[0]);
    output.mutable_p0()->set_y((*p0)[1]);
    output.mutable_p0()->set_z((*p0)[2]);
  }

  if (std::optional<std::array<double, 3>> p1 =
          TryRemovePoint3(parameters, "p1");
      p1.has_value()) {
    output.mutable_p1()->set_x((*p1)[0]);
    output.mutable_p1()->set_y((*p1)[1]);
    output.mutable_p1()->set_z((*p1)[2]);
  }

  if (std::optional<double> density = TryRemoveFloat(parameters, "density");
      density.has_value()) {
    output.set_density(*density);
  }

  if (std::optional<double> frequency = TryRemoveFloat(parameters, "frequency");
      frequency.has_value()) {
    output.set_frequency(*frequency);
  }

  if (std::optional<double> wispiness = TryRemoveFloat(parameters, "wispiness");
      wispiness.has_value()) {
    output.set_wispiness(*wispiness);
  }

  return RemoveSigma(parameters, pbrt_version, output);
}

absl::Status RemoveExponentialMedium(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, ExponentialMedium& output) {
  RemoveBounds(parameters, output);

  if (absl::Status status = RemoveSigma(parameters, pbrt_version, output);
      !status.ok()) {
    return status;
  }

  if (std::optional<double> g = TryRemoveFloat(parameters, "g");
      g.has_value()) {
    output.set_g(*g);
  }

  if (std::optional<double> a = TryRemoveFloat(parameters, "a");
      a.has_value()) {
    output.set_a(*a);
  }

  if (std::optional<double> b = TryRemoveFloat(parameters, "b");
      b.has_value()) {
    output.set_b(*b);
  }

  if (std::optional<std::array<double, 3>> updir =
          TryRemoveVector3(parameters, "updir");
      updir.has_value()) {
    output.mutable_updir()->set_x((*updir)[0]);
    output.mutable_updir()->set_y((*updir)[1]);
    output.mutable_updir()->set_z((*updir)[2]);
  }

  return TryRemoveSpectrum(parameters, "Le",
                           std::bind(&ExponentialMedium::mutable_le, &output));
}

absl::Status RemoveHomogeneousMedium(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, HomogeneousMedium& output) {
  if (absl::Status status = TryRemoveSpectrum(
          parameters, "Le", std::bind(&HomogeneousMedium::mutable_le, &output));
      !status.ok()) {
    return status;
  }

  if (std::optional<double> g = TryRemoveFloat(parameters, "g");
      g.has_value()) {
    output.set_g(*g);
  }

  if (pbrt_version <= 2) {
    RemoveBounds(parameters, output);
  }

  if (pbrt_version >= 3) {
    if (std::optional<absl::string_view> preset =
            TryRemoveString(parameters, "preset");
        preset.has_value()) {
      auto iter = kNamedMeasuredScatteringPresets.find(*preset);
      if (iter != kNamedMeasuredScatteringPresets.end()) {
        output.set_preset(iter->second);
      }
    }

    if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
        scale.has_value()) {
      output.set_scale(*scale);
    }
  }

  if (pbrt_version >= 4) {
    if (std::optional<double> lescale = TryRemoveFloat(parameters, "Lescale");
        lescale.has_value()) {
      output.set_lescale(*lescale);
    }
  }

  return RemoveSigma(parameters, pbrt_version, output);
}

absl::Status RemoveNanoVdbMedium(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, NanoVdbMedium& output) {
  if (std::optional<double> g = TryRemoveFloat(parameters, "g");
      g.has_value()) {
    output.set_g(*g);
  }

  if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
      scale.has_value()) {
    output.set_scale(*scale);
  }

  if (std::optional<double> lescale = TryRemoveFloat(parameters, "Lescale");
      lescale.has_value()) {
    output.set_lescale(*lescale);
  }

  if (std::optional<double> temperatureoffset =
          TryRemoveFloat(parameters, "temperatureoffset");
      temperatureoffset.has_value()) {
    output.set_temperatureoffset(*temperatureoffset);
  }

  if (std::optional<double> temperaturescale =
          TryRemoveFloat(parameters, "temperaturescale");
      temperaturescale.has_value()) {
    output.set_temperaturescale(*temperaturescale);
  }

  if (std::optional<absl::string_view> filename =
          TryRemoveString(parameters, "filename");
      filename) {
    output.set_filename(*filename);
  }

  return RemoveSigma(parameters, pbrt_version, output);
}

absl::Status RemoveRgbGridMedium(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, RgbGridMedium& output) {
  if (std::optional<std::array<double, 3>> sigma_a =
          TryRemoveRgb(parameters, "sigma_a");
      sigma_a.has_value()) {
    output.mutable_sigma_a()->set_r((*sigma_a)[0]);
    output.mutable_sigma_a()->set_g((*sigma_a)[1]);
    output.mutable_sigma_a()->set_b((*sigma_a)[2]);
  }

  if (std::optional<std::array<double, 3>> sigma_s =
          TryRemoveRgb(parameters, "sigma_s");
      sigma_s.has_value()) {
    output.mutable_sigma_s()->set_r((*sigma_s)[0]);
    output.mutable_sigma_s()->set_g((*sigma_s)[1]);
    output.mutable_sigma_s()->set_b((*sigma_s)[2]);
  }

  if (std::optional<double> g = TryRemoveFloat(parameters, "g");
      g.has_value()) {
    output.set_g(*g);
  }

  if (std::optional<absl::Span<std::array<double, 3>>> le =
          TryRemoveRgbs(parameters, "Le");
      le.has_value()) {
    if (le->size() > std::numeric_limits<int>::max()) {
      return absl::ResourceExhaustedError(
          "Le is too large to be stored in a proto array");
    }

    for (const auto& src : *le) {
      auto& dest = *output.add_le();
      dest.set_r(src[0]);
      dest.set_g(src[1]);
      dest.set_b(src[2]);
    }
  }

  if (std::optional<double> lescale = TryRemoveFloat(parameters, "Lescale");
      lescale.has_value()) {
    output.set_lescale(*lescale);
  }

  if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
      scale.has_value()) {
    output.set_scale(*scale);
  }

  if (std::optional<std::array<double, 3>> p0 =
          TryRemovePoint3(parameters, "p0");
      p0.has_value()) {
    output.mutable_p0()->set_x((*p0)[0]);
    output.mutable_p0()->set_y((*p0)[1]);
    output.mutable_p0()->set_z((*p0)[2]);
  }

  if (std::optional<std::array<double, 3>> p1 =
          TryRemovePoint3(parameters, "p1");
      p1.has_value()) {
    output.mutable_p1()->set_x((*p1)[0]);
    output.mutable_p1()->set_y((*p1)[1]);
    output.mutable_p1()->set_z((*p1)[2]);
  }

  if (std::optional<int32_t> nx = TryRemoveInteger(parameters, "nx");
      nx.has_value() && *nx > 0) {
    output.set_nx(*nx);
  }

  if (std::optional<int32_t> ny = TryRemoveInteger(parameters, "ny");
      ny.has_value() && *ny > 0) {
    output.set_ny(*ny);
  }

  if (std::optional<int32_t> nz = TryRemoveInteger(parameters, "nz");
      nz.has_value() && *nz > 0) {
    output.set_nz(*nz);
  }

  return absl::OkStatus();
}

absl::Status RemoveUniformGridMedium(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, UniformGridMedium& output) {
  RemoveBounds(parameters, output);

  if (absl::Status status = TryRemoveSpectrum(
          parameters, "Le", std::bind(&UniformGridMedium::mutable_le, &output));
      !status.ok()) {
    return status;
  }

  if (std::optional<double> g = TryRemoveFloat(parameters, "g");
      g.has_value()) {
    output.set_g(*g);
  }

  if (std::optional<int32_t> nx = TryRemoveInteger(parameters, "nx");
      nx.has_value() && *nx > 0) {
    output.set_nx(*nx);
  }

  if (std::optional<int32_t> ny = TryRemoveInteger(parameters, "ny");
      ny.has_value() && *ny > 0) {
    output.set_ny(*ny);
  }

  if (std::optional<int32_t> nz = TryRemoveInteger(parameters, "nz");
      nz.has_value() && *nz > 0) {
    output.set_nz(*nz);
  }

  if (std::optional<absl::Span<double>> density =
          TryRemoveFloats(parameters, "density");
      density.has_value()) {
    if (density->size() > std::numeric_limits<int>::max()) {
      return absl::ResourceExhaustedError(
          "density is too large to be stored in a proto array");
    }

    output.mutable_density()->Add(density->begin(), density->end());
  }

  if (pbrt_version >= 3) {
    if (std::optional<absl::string_view> preset =
            TryRemoveString(parameters, "preset");
        preset.has_value()) {
      auto iter = kNamedMeasuredScatteringPresets.find(*preset);
      if (iter != kNamedMeasuredScatteringPresets.end()) {
        output.set_preset(iter->second);
      }
    }

    if (std::optional<double> scale = TryRemoveFloat(parameters, "scale");
        scale.has_value()) {
      output.set_scale(*scale);
    }
  }

  if (pbrt_version >= 4) {
    if (std::optional<double> lescale = TryRemoveFloat(parameters, "Lescale");
        lescale.has_value()) {
      output.set_lescale(*lescale);
    }

    if (std::optional<absl::Span<double>> temperature =
            TryRemoveFloats(parameters, "temperature");
        temperature.has_value()) {
      if (temperature->size() > std::numeric_limits<int>::max()) {
        return absl::ResourceExhaustedError(
            "temperature is too large to be stored in a proto array");
      }

      output.mutable_temperature()->Add(temperature->begin(),
                                        temperature->end());
    }

    if (std::optional<double> temperatureoffset =
            TryRemoveFloat(parameters, "temperatureoffset");
        temperatureoffset.has_value()) {
      output.set_temperatureoffset(*temperatureoffset);
    }

    if (std::optional<double> temperaturescale =
            TryRemoveFloat(parameters, "temperaturescale");
        temperaturescale.has_value()) {
      output.set_temperaturescale(*temperaturescale);
    }
  }

  return RemoveSigma(parameters, pbrt_version, output);
}

}  // namespace pbrt_proto
