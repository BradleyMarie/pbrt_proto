#include "pbrt_proto/shared/integrators.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <optional>

#include "absl/container/flat_hash_map.h"
#include "absl/status/status.h"
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
      std::cerr << "WARNING: Unsupported value for 'directlighting' Integrator "
                   "parameter 'strategy': \""
                << *strategy << "\"" << std::endl;
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

template <typename T>
void RemovePixelBounds(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters, T& output) {
  std::optional<absl::Span<int32_t>> pixelbounds;
  if (absl::Status status =
          TryRemoveIntegers(parameters, "pixelbounds", 4, pixelbounds);
      status.ok() && pixelbounds.has_value()) {
    output.mutable_pixelbounds()->set_x_min((std::max(0, (*pixelbounds)[0])));
    output.mutable_pixelbounds()->set_x_max((std::max(0, (*pixelbounds)[1])));
    output.mutable_pixelbounds()->set_y_min((std::max(0, (*pixelbounds)[2])));
    output.mutable_pixelbounds()->set_y_max((std::max(0, (*pixelbounds)[3])));
  }
}

template <typename T>
void RemoveLightSampleStrategy(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view type, absl::string_view parameter, bool pbrt_v4,
    T& output) {
  if (std::optional<absl::string_view> lightsamplestrategy =
          TryRemoveString(parameters, parameter);
      lightsamplestrategy.has_value()) {
    if (*lightsamplestrategy == "power") {
      output.set_lightsampler(LightSampler::POWER);
    } else if (*lightsamplestrategy == "uniform") {
      output.set_lightsampler(LightSampler::UNIFORM);
    } else if (!pbrt_v4 && *lightsamplestrategy == "spatial") {
      output.set_lightsampler(LightSampler::BVH);
    } else if (pbrt_v4 && *lightsamplestrategy == "bvh") {
      output.set_lightsampler(LightSampler::BVH);
    } else {
      std::cerr << "WARNING: Unsupported value for '" << type
                << "' Integrator parameter '" << parameter << "': \""
                << *lightsamplestrategy << "\"" << std::endl;
      output.set_lightsampler(LightSampler::BVH);
    }
  }
}

}  // namespace

void RemoveAmbientOcclusionIntegratorV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    AmbientOcclusionIntegrator& output) {
  if (std::optional<double> maxdist = TryRemoveFloat(parameters, "maxdist");
      maxdist.has_value()) {
    output.set_maxdistance(*maxdist);
  }

  if (std::optional<int32_t> nsamples =
          TryRemoveInteger(parameters, "nsamples");
      nsamples.has_value()) {
    output.set_nsamples(std::max(0, *nsamples));
  }
}

void RemoveAmbientOcclusionIntegratorV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    AmbientOcclusionIntegrator& output) {
  if (std::optional<int32_t> nsamples =
          TryRemoveInteger(parameters, "nsamples");
      nsamples.has_value()) {
    output.set_nsamples(std::max(0, *nsamples));
  }

  if (std::optional<bool> cossample = TryRemoveBool(parameters, "cossample");
      cossample.has_value()) {
    output.set_cossample(*cossample);
  }

  RemovePixelBounds(parameters, output);
}

void RemoveBdptIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    BdptIntegrator& output) {}

void RemoveBdptIntegratorV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    BdptIntegrator& output) {
  if (std::optional<int32_t> maxdepth =
          TryRemoveInteger(parameters, "maxdepth");
      maxdepth.has_value()) {
    output.set_maxdepth(std::max(0, *maxdepth));
  }

  if (std::optional<bool> visualizestrategies =
          TryRemoveBool(parameters, "visualizestrategies");
      visualizestrategies.has_value()) {
    output.set_visualizestrategies(*visualizestrategies);
  }

  if (std::optional<bool> visualizeweights =
          TryRemoveBool(parameters, "visualizeweights");
      visualizeweights.has_value()) {
    output.set_visualizeweights(*visualizeweights);
  }

  RemoveLightSampleStrategy(parameters, "bdpt", "lightsamplestrategy",
                            /*pbrt_v4=*/false, output);
  RemovePixelBounds(parameters, output);
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
      std::cerr << "WARNING: Unsupported value for 'debug' Integrator "
                   "parameter 'red': \""
                << *red << "\"" << std::endl;
      output.set_red(DebugIntegrator::ZERO);
    }
  }

  if (std::optional<absl::string_view> green =
          TryRemoveString(parameters, "green");
      green.has_value()) {
    if (auto iter = values.find(*green); iter != values.end()) {
      output.set_green(iter->second);
    } else {
      std::cerr << "WARNING: Unsupported value for 'debug' Integrator "
                   "parameter 'green': \""
                << *green << "\"" << std::endl;
      output.set_green(DebugIntegrator::ZERO);
    }
  }

  if (std::optional<absl::string_view> blue =
          TryRemoveString(parameters, "blue");
      blue.has_value()) {
    if (auto iter = values.find(*blue); iter != values.end()) {
      output.set_blue(iter->second);
    } else {
      std::cerr << "WARNING: Unsupported value for 'debug' Integrator "
                   "parameter 'blue': \""
                << *blue << "\"" << std::endl;
      output.set_blue(DebugIntegrator::ZERO);
    }
  }
}

void RemoveDiffusePrtIntegratorV2(
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

void RemoveDipoleSubsurfaceIntegratorV2(
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

void RemoveDirectLightingIntegratorV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DirectLightingIntegrator& output) {
  RemoveDirectLightingIntegrator(parameters, output, /*pbrt_v1=*/false);
  RemovePixelBounds(parameters, output);
}

absl::Status RemoveGlossyPrtIntegratorV2(
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

  if (absl::Status status = TryRemoveSpectrumV1(
          parameters, "Kd",
          std::bind(&GlossyPrtIntegrator::mutable_kd, &output));
      !status.ok()) {
    return status;
  }

  if (absl::Status status = TryRemoveSpectrumV1(
          parameters, "Ks",
          std::bind(&GlossyPrtIntegrator::mutable_ks, &output));
      !status.ok()) {
    return status;
  }

  return absl::OkStatus();
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
  if (std::optional<double> maxerror = TryRemoveFloat(parameters, "maxerror");
      maxerror.has_value()) {
    output.set_maxerror(*maxerror);
  }

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

void RemoveMltIntegratorV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    MltIntegrator& output) {
  if (std::optional<int32_t> maxdepth =
          TryRemoveInteger(parameters, "maxdepth");
      maxdepth.has_value()) {
    output.set_maxdepth(std::max(0, *maxdepth));
  }

  if (std::optional<int32_t> bootstrapsamples =
          TryRemoveInteger(parameters, "bootstrapsamples");
      bootstrapsamples.has_value()) {
    output.set_bootstrapsamples(std::max(0, *bootstrapsamples));
  }

  if (std::optional<int32_t> chains = TryRemoveInteger(parameters, "chains");
      chains.has_value()) {
    output.set_chains(std::max(0, *chains));
  }

  if (std::optional<int32_t> mutationsperpixel =
          TryRemoveInteger(parameters, "mutationsperpixel");
      mutationsperpixel.has_value()) {
    output.set_mutationsperpixel(std::max(0, *mutationsperpixel));
  }

  if (std::optional<double> largestepprobability =
          TryRemoveFloat(parameters, "largestepprobability");
      largestepprobability.has_value()) {
    output.set_largestepprobability(*largestepprobability);
  }

  if (std::optional<double> sigma = TryRemoveFloat(parameters, "sigma");
      sigma.has_value()) {
    output.set_sigma(*sigma);
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

void RemovePathIntegratorV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PathIntegrator& output) {
  RemovePathIntegratorV1(parameters, output);

  if (std::optional<double> rrthreshold =
          TryRemoveFloat(parameters, "rrthreshold");
      rrthreshold.has_value()) {
    output.set_rrthreshold(*rrthreshold);
  }

  RemoveLightSampleStrategy(parameters, "path", "lightsamplestrategy",
                            /*pbrt_v4=*/false, output);
  RemovePixelBounds(parameters, output);
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

void RemoveSppmIntegratorV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SppmIntegrator& output) {
  if (std::optional<int32_t> maxdepth =
          TryRemoveInteger(parameters, "maxdepth");
      maxdepth.has_value()) {
    output.set_maxdepth(std::max(0, *maxdepth));
  }

  if (std::optional<int32_t> numiterations =
          TryRemoveInteger(parameters, "numiterations");
      numiterations.has_value()) {
    output.set_numiterations(std::max(0, *numiterations));
  }

  if (std::optional<int32_t> photonsperiteration =
          TryRemoveInteger(parameters, "photonsperiteration");
      photonsperiteration.has_value()) {
    output.set_photonsperiteration(std::max(0, *photonsperiteration));
  }

  if (std::optional<int32_t> imagewritefrequency =
          TryRemoveInteger(parameters, "imagewritefrequency");
      imagewritefrequency.has_value()) {
    output.set_imagewritefrequency(std::max(0, *imagewritefrequency));
  }

  if (std::optional<double> radius = TryRemoveFloat(parameters, "radius");
      radius.has_value()) {
    output.set_radius(*radius);
  }
}

void RemoveUseProbesIntegratorV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    UseProbesIntegrator& output) {
  if (std::optional<absl::string_view> filename =
          TryRemoveString(parameters, "filename");
      filename.has_value()) {
    output.set_filename(*filename);
  }
}

void RemoveVolPathIntegratorV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    VolPathIntegrator& output) {
  if (std::optional<int32_t> maxdepth =
          TryRemoveInteger(parameters, "maxdepth");
      maxdepth.has_value()) {
    output.set_maxdepth(std::max(0, *maxdepth));
  }

  if (std::optional<double> rrthreshold =
          TryRemoveFloat(parameters, "rrthreshold");
      rrthreshold.has_value()) {
    output.set_rrthreshold(*rrthreshold);
  }

  RemoveLightSampleStrategy(parameters, "volpath", "lightsamplestrategy",
                            /*pbrt_v4=*/false, output);
  RemovePixelBounds(parameters, output);
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

void RemoveWhittedIntegratorV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    WhittedIntegrator& output) {
  RemoveWhittedIntegratorV1(parameters, output);
  RemovePixelBounds(parameters, output);
}

absl::Status RemoveEmissionVolumeIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, EmissionVolumeIntegrator& output) {
  if (std::optional<double> stepsize = TryRemoveFloat(parameters, "stepsize");
      stepsize.has_value()) {
    output.set_stepsize(*stepsize);
  }

  return absl::OkStatus();
}

absl::Status RemoveSingleVolumeIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, SingleVolumeIntegrator& output) {
  if (std::optional<double> stepsize = TryRemoveFloat(parameters, "stepsize");
      stepsize.has_value()) {
    output.set_stepsize(*stepsize);
  }

  return absl::OkStatus();
}

}  // namespace pbrt_proto
