#include "pbrt_proto/shared/integrators.h"

#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <optional>

#include "absl/container/flat_hash_map.h"
#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/common.h"
#include "pbrt_proto/shared/parser.h"
#include "pbrt_proto/shared/version.h"

namespace pbrt_proto {
namespace {

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
std::optional<LightSampler::Type> TryRemoveLightSampleStrategy(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, absl::string_view type, absl::string_view parameter,
    T& output) {
  if (std::optional<absl::string_view> lightsamplestrategy =
          TryRemoveString(parameters, parameter);
      lightsamplestrategy.has_value()) {
    if (*lightsamplestrategy == "power") {
      return LightSampler::POWER;
    } else if (*lightsamplestrategy == "uniform") {
      return LightSampler::UNIFORM;
    } else if (pbrt_version <= 3 && *lightsamplestrategy == "spatial") {
      return LightSampler::BVH;
    } else if (pbrt_version >= 4 && *lightsamplestrategy == "bvh") {
      return LightSampler::BVH;
    } else {
      std::cerr << "WARNING: Unsupported value for '" << type
                << "' Integrator parameter '" << parameter << "': \""
                << *lightsamplestrategy << "\"" << std::endl;
      return LightSampler::BVH;
    }
  }

  return std::nullopt;
}

}  // namespace

absl::Status RemoveAmbientOcclusionIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, AmbientOcclusionIntegrator& output) {
  assert(IsSupported(pbrt_version, output));

  if (std::optional<int32_t> nsamples =
          TryRemoveInteger(parameters, "nsamples");
      nsamples.has_value()) {
    output.set_nsamples(std::max(0, *nsamples));
  }

  if (pbrt_version == 2) {
    if (std::optional<double> maxdist = TryRemoveFloat(parameters, "maxdist");
        maxdist.has_value()) {
      output.set_maxdistance(*maxdist);
    }
  } else if (pbrt_version >= 4) {
    if (std::optional<double> maxdist =
            TryRemoveFloat(parameters, "maxdistance");
        maxdist.has_value()) {
      output.set_maxdistance(*maxdist);
    }
  }

  if (pbrt_version >= 3) {
    if (std::optional<bool> cossample = TryRemoveBool(parameters, "cossample");
        cossample.has_value()) {
      output.set_cossample(*cossample);
    }

    RemovePixelBounds(parameters, output);
  }

  return absl::OkStatus();
}

absl::Status RemoveBdptIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, BdptIntegrator& output) {
  assert(IsSupported(pbrt_version, output));

  if (pbrt_version >= 3) {
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

    if (std::optional<LightSampler::Type> lightsamplestrategy =
            TryRemoveLightSampleStrategy(parameters, pbrt_version, "bdpt",
                                         "lightsamplestrategy", output);
        lightsamplestrategy.has_value()) {
      output.set_lightsamplestrategy(*lightsamplestrategy);
    }
  }

  if (pbrt_version == 3) {
    RemovePixelBounds(parameters, output);
  }

  return absl::OkStatus();
}

absl::Status RemoveDebugIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, DebugIntegrator& output) {
  assert(IsSupported(pbrt_version, output));

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

  return absl::OkStatus();
}

absl::Status RemoveDiffusePrtIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, DiffusePrtIntegrator& output) {
  assert(IsSupported(pbrt_version, output));

  if (std::optional<int32_t> nsamples =
          TryRemoveInteger(parameters, "nsamples");
      nsamples.has_value()) {
    output.set_nsamples(std::max(0, *nsamples));
  }

  if (std::optional<int32_t> lmax = TryRemoveInteger(parameters, "lmax");
      lmax.has_value()) {
    output.set_lmax(std::max(0, *lmax));
  }

  return absl::OkStatus();
}

absl::Status RemoveDipoleSubsurfaceIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, DipoleSubsurfaceIntegrator& output) {
  assert(IsSupported(pbrt_version, output));

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

  return absl::OkStatus();
}

absl::Status RemoveDirectLightingIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, DirectLightingIntegrator& output) {
  assert(IsSupported(pbrt_version, output));

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
    } else if (pbrt_version == 1 && *strategy == "weighted") {
      output.set_strategy(DirectLightingIntegrator::WEIGHTED);
    } else {
      std::cerr << "WARNING: Unsupported value for 'directlighting' Integrator "
                   "parameter 'strategy': \""
                << *strategy << "\"" << std::endl;
      output.set_strategy(DirectLightingIntegrator::ALL);
    }
  }

  if (pbrt_version == 3) {
    RemovePixelBounds(parameters, output);
  }

  return absl::OkStatus();
}

absl::Status RemoveGlossyPrtIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, GlossyPrtIntegrator& output) {
  assert(IsSupported(pbrt_version, output));

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

  if (absl::Status status = TryRemoveSpectrum(
          parameters, "Kd",
          std::bind(&GlossyPrtIntegrator::mutable_kd, &output));
      !status.ok()) {
    return status;
  }

  if (absl::Status status = TryRemoveSpectrum(
          parameters, "Ks",
          std::bind(&GlossyPrtIntegrator::mutable_ks, &output));
      !status.ok()) {
    return status;
  }

  return absl::OkStatus();
}

absl::Status RemoveIgiIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, IgiIntegrator& output) {
  assert(IsSupported(pbrt_version, output));

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

  if (pbrt_version == 1) {
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

  if (pbrt_version >= 2) {
    if (std::optional<int32_t> maxdepth =
            TryRemoveInteger(parameters, "maxdepth");
        maxdepth.has_value()) {
      output.set_maxdepth(std::max(0, *maxdepth));
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

  return absl::OkStatus();
}

absl::Status RemoveIrradianceCacheIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, IrradianceCacheIntegrator& output) {
  assert(IsSupported(pbrt_version, output));

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

  return absl::OkStatus();
}

absl::Status RemoveMltIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, MltIntegrator& output) {
  assert(IsSupported(pbrt_version, output));

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

  return absl::OkStatus();
}

absl::Status RemovePathIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, PathIntegrator& output) {
  assert(IsSupported(pbrt_version, output));

  if (std::optional<int32_t> maxdepth =
          TryRemoveInteger(parameters, "maxdepth");
      maxdepth.has_value()) {
    output.set_maxdepth(std::max(0, *maxdepth));
  }

  if (pbrt_version >= 3) {
    if (std::optional<LightSampler::Type> lightsamplestrategy =
            TryRemoveLightSampleStrategy(parameters, pbrt_version, "bdpt",
                                         "lightsamplestrategy", output);
        lightsamplestrategy.has_value()) {
      output.set_lightsampler(*lightsamplestrategy);
    }
  }

  if (pbrt_version == 3) {
    if (std::optional<double> rrthreshold =
            TryRemoveFloat(parameters, "rrthreshold");
        rrthreshold.has_value()) {
      output.set_rrthreshold(*rrthreshold);
    }

    RemovePixelBounds(parameters, output);
  }

  return absl::OkStatus();
}

absl::Status RemovePhotonMapIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, PhotonMapIntegrator& output) {
  assert(IsSupported(pbrt_version, output));

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

  if (pbrt_version == 1) {
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

  if (pbrt_version >= 2) {
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

  return absl::OkStatus();
}

absl::Status RemoveExPhotonMapIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, PhotonMapIntegrator& output) {
  assert(IsSupported(pbrt_version, output));

  if (std::optional<double> gatherangle =
          TryRemoveFloat(parameters, "gatherangle");
      gatherangle.has_value()) {
    output.set_gatherangle(*gatherangle);
  }

  return RemovePhotonMapIntegrator(parameters, pbrt_version, output);
}

absl::Status RemoveSppmIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, SppmIntegrator& output) {
  assert(IsSupported(pbrt_version, output));

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

  return absl::OkStatus();
}

absl::Status RemoveUseProbesIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, UseProbesIntegrator& output) {
  assert(IsSupported(pbrt_version, output));

  if (std::optional<absl::string_view> filename =
          TryRemoveString(parameters, "filename");
      filename.has_value()) {
    output.set_filename(*filename);
  }

  return absl::OkStatus();
}

absl::Status RemoveVolPathIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, VolPathIntegrator& output) {
  assert(IsSupported(pbrt_version, output));

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

  if (std::optional<LightSampler::Type> lightsamplestrategy =
          TryRemoveLightSampleStrategy(parameters, pbrt_version, "bdpt",
                                       "lightsamplestrategy", output);
      lightsamplestrategy.has_value()) {
    output.set_lightsampler(*lightsamplestrategy);
  }

  if (pbrt_version == 3) {
    RemovePixelBounds(parameters, output);
  }

  return absl::OkStatus();
}

absl::Status RemoveWhittedIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, WhittedIntegrator& output) {
  assert(IsSupported(pbrt_version, output));

  if (std::optional<int32_t> maxdepth =
          TryRemoveInteger(parameters, "maxdepth");
      maxdepth.has_value()) {
    output.set_maxdepth(std::max(0, *maxdepth));
  }

  if (pbrt_version == 3) {
    RemovePixelBounds(parameters, output);
  }

  return absl::OkStatus();
}

absl::Status RemoveEmissionVolumeIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, EmissionVolumeIntegrator& output) {
  assert(IsSupported(pbrt_version, output));

  if (std::optional<double> stepsize = TryRemoveFloat(parameters, "stepsize");
      stepsize.has_value()) {
    output.set_stepsize(*stepsize);
  }

  return absl::OkStatus();
}

absl::Status RemoveSingleVolumeIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, SingleVolumeIntegrator& output) {
  assert(IsSupported(pbrt_version, output));

  if (std::optional<double> stepsize = TryRemoveFloat(parameters, "stepsize");
      stepsize.has_value()) {
    output.set_stepsize(*stepsize);
  }

  return absl::OkStatus();
}

}  // namespace pbrt_proto
