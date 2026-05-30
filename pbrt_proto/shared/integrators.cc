#include "pbrt_proto/shared/integrators.h"

#include <algorithm>
#include <functional>
#include <optional>

#include "absl/container/flat_hash_map.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/common.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {
namespace {

void RemoveDirectLightingIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DirectLightingIntegrator& output, bool pbrt_v1) {
  if (std::optional<int32_t> maxdepth =
          TryRemoveInteger(parameters, "maxdepth");
      maxdepth.has_value()) {
    output.set_maxdepth(std::max(0, *maxdepth));
  }

  if (std::optional<absl::string_view> strategy =
          TryRemoveString(parameters, "strategy");
      strategy.has_value()) {
    if (*strategy == "all") {
      output.set_strategy(DirectLightingIntegrator::ALL);
    } else if (*strategy == "one") {
      output.set_strategy(DirectLightingIntegrator::ONE);
    } else if (pbrt_v1 && *strategy == "weighted") {
      output.set_strategy(DirectLightingIntegrator::WEIGHTED);
    } else {
      output.set_strategy(DirectLightingIntegrator::ALL);
    }
  }
}

void RemovePhotonMapIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PhotonMapIntegrator& output) {
  if (std::optional<int32_t> causticphotons =
          TryRemoveInteger(parameters, "causticphotons");
      causticphotons.has_value()) {
    output.set_causticphotons(std::max(0, *causticphotons));
  }

  if (std::optional<int32_t> indirectphotons =
          TryRemoveInteger(parameters, "indirectphotons");
      indirectphotons.has_value()) {
    output.set_indirectphotons(std::max(0, *indirectphotons));
  }

  if (std::optional<int32_t> nused = TryRemoveInteger(parameters, "nused");
      nused.has_value()) {
    output.set_nused(std::max(0, *nused));
  }

  if (std::optional<int32_t> finalgathersamples =
          TryRemoveInteger(parameters, "finalgathersamples");
      finalgathersamples.has_value()) {
    output.set_finalgathersamples(std::max(0, *finalgathersamples));
  }

  if (std::optional<double> maxdist = TryRemoveFloat(parameters, "maxdist");
      maxdist.has_value()) {
    output.set_maxdist(*maxdist);
  }

  if (std::optional<bool> finalgather =
          TryRemoveBool(parameters, "finalgather");
      finalgather.has_value()) {
    output.set_finalgather(*finalgather);
  }

  if (std::optional<double> rrthreshold =
          TryRemoveFloat(parameters, "rrthreshold");
      rrthreshold.has_value()) {
    output.set_rrthreshold(*rrthreshold);
  }
}

}  // namespace

void RemoveAmbientOcclusionIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    AmbientOcclusionIntegrator& output) {
  if (std::optional<double> maxdist = TryRemoveFloat(parameters, "maxdist");
      maxdist.has_value()) {
    output.set_maxdist(*maxdist);
  }

  if (std::optional<int32_t> nsamples =
          TryRemoveInteger(parameters, "nsamples");
      nsamples.has_value()) {
    output.set_nsamples(std::max(0, *nsamples));
  }
}

void RemoveBdptIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    BdptIntegrator& output) {
  // Empty
}

void RemoveDebugIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DebugIntegrator& output) {
  static const absl::flat_hash_map<absl::string_view,
                                   DebugIntegrator::ChannelValue>
      values = {
          {"zero", DebugIntegrator::ZERO}, {"one", DebugIntegrator::ONE},
          {"hit", DebugIntegrator::HIT},   {"nx", DebugIntegrator::NX},
          {"ny", DebugIntegrator::NY},     {"nz", DebugIntegrator::NZ},
          {"snx", DebugIntegrator::SNX},   {"sny", DebugIntegrator::SNY},
          {"snz", DebugIntegrator::SNZ},   {"u", DebugIntegrator::U},
          {"v", DebugIntegrator::V},
      };

  if (std::optional<absl::string_view> red = TryRemoveString(parameters, "red");
      red.has_value()) {
    if (auto iter = values.find(*red); iter != values.end()) {
      output.set_red(iter->second);
    } else {
      output.set_red(DebugIntegrator::ZERO);
    }
  }

  if (std::optional<absl::string_view> green =
          TryRemoveString(parameters, "green");
      green.has_value()) {
    if (auto iter = values.find(*green); iter != values.end()) {
      output.set_green(iter->second);
    } else {
      output.set_green(DebugIntegrator::ZERO);
    }
  }

  if (std::optional<absl::string_view> blue =
          TryRemoveString(parameters, "blue");
      blue.has_value()) {
    if (auto iter = values.find(*blue); iter != values.end()) {
      output.set_blue(iter->second);
    } else {
      output.set_blue(DebugIntegrator::ZERO);
    }
  }
}

void RemoveDiffusePrtIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DiffusePrtIntegrator& output) {
  if (std::optional<int32_t> nsamples =
          TryRemoveInteger(parameters, "nsamples");
      nsamples.has_value()) {
    output.set_nsamples(std::max(0, *nsamples));
  }

  if (std::optional<int32_t> lmax = TryRemoveInteger(parameters, "lmax");
      lmax.has_value()) {
    output.set_lmax(std::max(0, *lmax));
  }
}

void RemoveDipoleSubsurfaceIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DipoleSubsurfaceIntegrator& output) {
  if (std::optional<int32_t> maxdepth =
          TryRemoveInteger(parameters, "maxdepth");
      maxdepth.has_value()) {
    output.set_maxdepth(std::max(0, *maxdepth));
  }

  if (std::optional<double> maxerror = TryRemoveFloat(parameters, "maxerror");
      maxerror.has_value()) {
    output.set_maxerror(*maxerror);
  }

  if (std::optional<double> minsampledistance =
          TryRemoveFloat(parameters, "minsampledistance");
      minsampledistance.has_value()) {
    output.set_minsampledistance(*minsampledistance);
  }

  if (std::optional<absl::string_view> pointsfile =
          TryRemoveString(parameters, "pointsfile");
      pointsfile.has_value()) {
    output.set_pointsfile(*pointsfile);
  }
}

void RemoveDirectLightingIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DirectLightingIntegrator& output) {
  RemoveDirectLightingIntegrator(parameters, output, /*pbrt_v1=*/true);
}

void RemoveDirectLightingIntegratorV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DirectLightingIntegrator& output) {
  RemoveDirectLightingIntegrator(parameters, output, /*pbrt_v1=*/false);
}

void RemoveGlossyPrtIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    GlossyPrtIntegrator& output) {
  if (std::optional<int32_t> nsamples =
          TryRemoveInteger(parameters, "nsamples");
      nsamples.has_value()) {
    output.set_nsamples(std::max(0, *nsamples));
  }

  if (std::optional<int32_t> lmax = TryRemoveInteger(parameters, "lmax");
      lmax.has_value()) {
    output.set_lmax(std::max(0, *lmax));
  }

  if (std::optional<double> roughness = TryRemoveFloat(parameters, "roughness");
      roughness.has_value()) {
    output.set_roughness(*roughness);
  }

  TryRemoveSpectrumV1(parameters, "Kd",
                      std::bind(&GlossyPrtIntegrator::mutable_kd, &output));
  TryRemoveSpectrumV1(parameters, "Ks",
                      std::bind(&GlossyPrtIntegrator::mutable_ks, &output));
}

void RemoveIgiIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    IgiIntegrator& output) {
  if (std::optional<int32_t> nlights = TryRemoveInteger(parameters, "nlights");
      nlights.has_value()) {
    output.set_nlights(std::max(0, *nlights));
  }

  if (std::optional<int32_t> nsets = TryRemoveInteger(parameters, "nsets");
      nsets.has_value()) {
    output.set_nsets(std::max(0, *nsets));
  }

  if (std::optional<double> rrthreshold =
          TryRemoveFloat(parameters, "rrthreshold");
      rrthreshold.has_value()) {
    output.set_rrthreshold(*rrthreshold);
  }

  if (std::optional<double> indirectscale =
          TryRemoveFloat(parameters, "indirectscale");
      indirectscale.has_value()) {
    output.set_indirectscale(*indirectscale);
  }

  if (std::optional<double> mindist = TryRemoveFloat(parameters, "mindist");
      mindist.has_value()) {
    output.set_mindist(*mindist);
  }
}

void RemoveIgiIntegratorV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    IgiIntegrator& output) {
  if (std::optional<int32_t> maxdepth =
          TryRemoveInteger(parameters, "maxdepth");
      maxdepth.has_value()) {
    output.set_maxdepth(std::max(0, *maxdepth));
  }

  if (std::optional<int32_t> nlights = TryRemoveInteger(parameters, "nlights");
      nlights.has_value()) {
    output.set_nlights(std::max(0, *nlights));
  }

  if (std::optional<int32_t> nsets = TryRemoveInteger(parameters, "nsets");
      nsets.has_value()) {
    output.set_nsets(std::max(0, *nsets));
  }

  if (std::optional<double> rrthreshold =
          TryRemoveFloat(parameters, "rrthreshold");
      rrthreshold.has_value()) {
    output.set_rrthreshold(*rrthreshold);
  }

  if (std::optional<double> glimit = TryRemoveFloat(parameters, "glimit");
      glimit.has_value()) {
    output.set_glimit(*glimit);
  }

  if (std::optional<int32_t> gathersamples =
          TryRemoveInteger(parameters, "gathersamples");
      gathersamples.has_value()) {
    output.set_gathersamples(std::max(0, *gathersamples));
  }
}

void RemoveIrradianceCacheIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    IrradianceCacheIntegrator& output) {
  if (std::optional<double> minweight = TryRemoveFloat(parameters, "minweight");
      minweight.has_value()) {
    output.set_minweight(*minweight);
  }

  if (std::optional<double> minpixelspacing =
          TryRemoveFloat(parameters, "minpixelspacing");
      minpixelspacing.has_value()) {
    output.set_minpixelspacing(*minpixelspacing);
  }

  if (std::optional<double> maxpixelspacing =
          TryRemoveFloat(parameters, "maxpixelspacing");
      maxpixelspacing.has_value()) {
    output.set_maxpixelspacing(*maxpixelspacing);
  }

  if (std::optional<double> maxangledifference =
          TryRemoveFloat(parameters, "maxangledifference");
      maxangledifference.has_value()) {
    output.set_maxangledifference(*maxangledifference);
  }

  if (std::optional<int32_t> maxspeculardepth =
          TryRemoveInteger(parameters, "maxspeculardepth");
      maxspeculardepth.has_value()) {
    output.set_maxspeculardepth(std::max(0, *maxspeculardepth));
  }

  if (std::optional<int32_t> maxindirectdepth =
          TryRemoveInteger(parameters, "maxindirectdepth");
      maxindirectdepth.has_value()) {
    output.set_maxindirectdepth(std::max(0, *maxindirectdepth));
  }

  if (std::optional<int32_t> nsamples =
          TryRemoveInteger(parameters, "nsamples");
      nsamples.has_value()) {
    output.set_nsamples(std::max(0, *nsamples));
  }
}

void RemovePathIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PathIntegrator& output) {
  if (std::optional<int32_t> maxdepth =
          TryRemoveInteger(parameters, "maxdepth");
      maxdepth.has_value()) {
    output.set_maxdepth(std::max(0, *maxdepth));
  }
}

void RemovePhotonMapIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PhotonMapIntegrator& output) {
  RemovePhotonMapIntegrator(parameters, output);

  if (std::optional<int32_t> directphotons =
          TryRemoveInteger(parameters, "directphotons");
      directphotons.has_value()) {
    output.set_directphotons(std::max(0, *directphotons));
  }

  if (std::optional<int32_t> maxdepth =
          TryRemoveInteger(parameters, "maxdepth");
      maxdepth.has_value()) {
    output.set_maxdepth(std::max(0, *maxdepth));
  }

  if (std::optional<bool> directwithphotons =
          TryRemoveBool(parameters, "directwithphotons");
      directwithphotons.has_value()) {
    output.set_directwithphotons(*directwithphotons);
  }
}

void RemoveExPhotonMapIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PhotonMapIntegrator& output) {
  RemovePhotonMapIntegratorV1(parameters, output);

  if (std::optional<double> gatherangle =
          TryRemoveFloat(parameters, "gatherangle");
      gatherangle.has_value()) {
    output.set_gatherangle(*gatherangle);
  }
}

void RemovePhotonMapIntegratorV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PhotonMapIntegrator& output) {
  RemovePhotonMapIntegrator(parameters, output);

  if (std::optional<double> gatherangle =
          TryRemoveFloat(parameters, "gatherangle");
      gatherangle.has_value()) {
    output.set_gatherangle(*gatherangle);
  }

  if (std::optional<int32_t> maxspeculardepth =
          TryRemoveInteger(parameters, "maxspeculardepth");
      maxspeculardepth.has_value()) {
    output.set_maxspeculardepth(std::max(0, *maxspeculardepth));
  }

  if (std::optional<int32_t> maxphotondepth =
          TryRemoveInteger(parameters, "maxphotondepth");
      maxphotondepth.has_value()) {
    output.set_maxphotondepth(std::max(0, *maxphotondepth));
  }
}

void RemoveUseProbesIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    UseProbesIntegrator& output) {
  if (std::optional<absl::string_view> filename =
          TryRemoveString(parameters, "filename");
      filename.has_value()) {
    output.set_filename(*filename);
  }
}

void RemoveWhittedIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    WhittedIntegrator& output) {
  if (std::optional<int32_t> maxdepth =
          TryRemoveInteger(parameters, "maxdepth");
      maxdepth.has_value()) {
    output.set_maxdepth(std::max(0, *maxdepth));
  }
}

}  // namespace pbrt_proto
