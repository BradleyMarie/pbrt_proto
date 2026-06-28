#include "pbrt_proto/testing/field_compatibility.h"

#include <initializer_list>

#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "google/protobuf/descriptor.pb.h"

namespace pbrt_proto {
namespace {

using ::google::protobuf::FieldDescriptor;

class DisjointSet {
 public:
  class Builder {
   public:
    // This implementation is not correct for the general case, but as long as
    // all of the sets added are mutually disjoint it will work as intended.
    Builder& AddSet(std::initializer_list<absl::string_view> strings) {
      for (const absl::string_view& str0 : strings) {
        for (const absl::string_view& str1 : strings) {
          map_[str0].insert(str1);
        }
      }

      return *this;
    }

    DisjointSet Build() { return DisjointSet(map_); }

   private:
    absl::flat_hash_map<absl::string_view,
                        absl::flat_hash_set<absl::string_view>>
        map_;
  };

  bool AreInSameSet(absl::string_view v0, absl::string_view v1) const {
    auto iter = map_.find(v0);
    if (iter == map_.end()) {
      return false;
    }

    return iter->second.contains(v1);
  }

 private:
  DisjointSet(absl::flat_hash_map<absl::string_view,
                                  absl::flat_hash_set<absl::string_view>>
                  map)
      : map_(std::move(map)) {}

  absl::flat_hash_map<absl::string_view, absl::flat_hash_set<absl::string_view>>
      map_;
};

static const DisjointSet kEquivalentTypes =
    DisjointSet::Builder()
        .AddSet({"pbrt_proto.v1.Accelerator", "pbrt_proto.v2.Accelerator",
                 "pbrt_proto.v3.Accelerator", "pbrt_proto.v4.Accelerator"})
        .AddSet(
            {"pbrt_proto.v1.AreaLightSource", "pbrt_proto.v2.AreaLightSource",
             "pbrt_proto.v3.AreaLightSource", "pbrt_proto.v4.AreaLightSource"})
        .AddSet({"pbrt_proto.v1.Camera", "pbrt_proto.v2.Camera",
                 "pbrt_proto.v3.Camera", "pbrt_proto.v4.Camera"})
        .AddSet({"pbrt_proto.v1.Film", "pbrt_proto.v2.Film",
                 "pbrt_proto.v3.Film", "pbrt_proto.v4.Film"})
        .AddSet({"pbrt_proto.v1.FloatTexture", "pbrt_proto.v2.FloatTexture",
                 "pbrt_proto.v3.FloatTexture", "pbrt_proto.v4.FloatTexture"})
        .AddSet({"pbrt_proto.v1.Directive", "pbrt_proto.v2.Directive",
                 "pbrt_proto.v3.Directive", "pbrt_proto.v4.Directive"})
        .AddSet({"pbrt_proto.v1.LightSource", "pbrt_proto.v2.LightSource",
                 "pbrt_proto.v3.LightSource", "pbrt_proto.v4.LightSource"})
        .AddSet({"pbrt_proto.v2.MakeNamedMaterial",
                 "pbrt_proto.v3.MakeNamedMaterial",
                 "pbrt_proto.v4.MakeNamedMaterial"})
        .AddSet(
            {"pbrt_proto.v3.MakeNamedMedium", "pbrt_proto.v4.MakeNamedMedium"})
        .AddSet({"pbrt_proto.v1.Material", "pbrt_proto.v2.Material",
                 "pbrt_proto.v3.Material", "pbrt_proto.v4.Material"})
        .AddSet({"pbrt_proto.v1.PixelFilter", "pbrt_proto.v2.PixelFilter",
                 "pbrt_proto.v3.PixelFilter", "pbrt_proto.v4.PixelFilter"})
        .AddSet({"pbrt_proto.v1.Sampler", "pbrt_proto.v2.Sampler",
                 "pbrt_proto.v3.Sampler", "pbrt_proto.v4.Sampler"})
        .AddSet({"pbrt_proto.v1.Shape", "pbrt_proto.v2.Shape",
                 "pbrt_proto.v3.Shape", "pbrt_proto.v4.Shape"})
        .AddSet({"pbrt_proto.v1.Shape.MaterialOverrides",
                 "pbrt_proto.v2.Shape.MaterialOverrides",
                 "pbrt_proto.v3.Shape.MaterialOverrides"})
        .AddSet(
            {"pbrt_proto.v1.SpectrumTexture", "pbrt_proto.v2.SpectrumTexture",
             "pbrt_proto.v3.SpectrumTexture", "pbrt_proto.v4.SpectrumTexture"})
        .AddSet({"pbrt_proto.v1.SurfaceIntegrator",
                 "pbrt_proto.v2.SurfaceIntegrator", "pbrt_proto.v3.Integrator",
                 "pbrt_proto.v4.Integrator"})
        .AddSet({"pbrt_proto.v1.Volume", "pbrt_proto.v2.Volume"})
        .AddSet({"pbrt_proto.v1.VolumeIntegrator",
                 "pbrt_proto.v2.VolumeIntegrator"})
        .Build();

static const DisjointSet kAllowedNameChanges =
    DisjointSet::Builder()
        .AddSet({"pbrt_proto.AmbientOcclusionIntegrator.maxdistance",
                 "pbrt_proto.PhotonMapIntegrator.maxdist"})
        .AddSet({
            "pbrt_proto.BdptIntegrator.lightsamplestrategy",
            "pbrt_proto.PathIntegrator.lightsampler",
            "pbrt_proto.VolPathIntegrator.lightsampler",
        })
        .AddSet({"pbrt_proto.v1.AreaLightSource.area",
                 "pbrt_proto.v2.AreaLightSource.diffuse",
                 "pbrt_proto.v3.AreaLightSource.diffuse",
                 "pbrt_proto.v4.AreaLightSource.diffuse"})
        .AddSet({"pbrt_proto.v1.Camera.environment",
                 "pbrt_proto.v2.Camera.environment",
                 "pbrt_proto.v3.Camera.environment",
                 "pbrt_proto.v4.Camera.spherical"})
        .AddSet({"pbrt_proto.v1.Directive.surface_integrator",
                 "pbrt_proto.v2.Directive.surface_integrator",
                 "pbrt_proto.v3.Directive.integrator",
                 "pbrt_proto.v4.Directive.integrator"})
        .AddSet({"pbrt_proto.v1.Film.image", "pbrt_proto.v2.Film.image",
                 "pbrt_proto.v3.Film.image", "pbrt_proto.v4.Film.rgb"})
        .AddSet({"pbrt_proto.v3.MakeNamedMedium.heterogeneous",
                 "pbrt_proto.v4.MakeNamedMedium.uniformgrid"})
        .AddSet(
            {"pbrt_proto.MetalMaterial.eta",
             "pbrt_proto.v2.Shape.MaterialOverrides.eta_as_spectrum_texture",
             "pbrt_proto.v3.Shape.MaterialOverrides.eta_as_spectrum_texture"})
        .AddSet({"pbrt_proto.v1.SurfaceIntegrator.bidirectional",
                 "pbrt_proto.v3.Integrator.bdpt",
                 "pbrt_proto.v4.Integrator.bdpt"})
        .AddSet({
            "pbrt_proto.AdaptiveSampler.maxsamples",
            "pbrt_proto.BestCandidateSampler.pixelsamples",
            "pbrt_proto.HaltonSampler.pixelsamples",
            "pbrt_proto.IndependentSampler.pixelsamples",
            "pbrt_proto.MaxMinDistSampler.pixelsamples",
            "pbrt_proto.PaddedSobolSampler.pixelsamples",
            "pbrt_proto.PMJ02BNSampler.pixelsamples",
            "pbrt_proto.SobolSampler.pixelsamples",
            "pbrt_proto.ZSobolSampler.pixelsamples",
            "pbrt_proto.ZeroTwoSequenceSampler.pixelsamples",
        })
        .AddSet({"pbrt_proto.v1.Sampler.lowdiscrepancy",
                 "pbrt_proto.v2.Sampler.lowdiscrepancy",
                 "pbrt_proto.v3.Sampler.zerotwosequence"})
        .AddSet({"pbrt_proto.v2.Sampler.random", "pbrt_proto.v3.Sampler.random",
                 "pbrt_proto.v4.Sampler.independent"})
        .Build();

static const DisjointSet kAllowedNameCollisions =
    DisjointSet::Builder()
        .AddSet({"pbrt_proto.CloudMedium.sigma_a",
                 "pbrt_proto.ExponentialMedium.sigma_a",
                 "pbrt_proto.HomogeneousMedium.sigma_a",
                 "pbrt_proto.NanoVdbMedium.sigma_a",
                 "pbrt_proto.RgbGridMedium.sigma_a",
                 "pbrt_proto.UniformGridMedium.sigma_a"})
        .AddSet({"pbrt_proto.CloudMedium.sigma_s",
                 "pbrt_proto.ExponentialMedium.sigma_s",
                 "pbrt_proto.HomogeneousMedium.sigma_s",
                 "pbrt_proto.NanoVdbMedium.sigma_s",
                 "pbrt_proto.RgbGridMedium.sigma_s",
                 "pbrt_proto.UniformGridMedium.sigma_s"})
        .AddSet({"pbrt_proto.ExponentialMedium.Le",
                 "pbrt_proto.HomogeneousMedium.Le",
                 "pbrt_proto.RgbGridMedium.Le",
                 "pbrt_proto.UniformGridMedium.Le"})
        .AddSet({"pbrt_proto.DisneyMaterial.eta",
                 "pbrt_proto.GlassMaterial.eta", "pbrt_proto.HairMaterial.eta",
                 "pbrt_proto.KdSubsurfaceMaterial.eta",
                 "pbrt_proto.MetalMaterial.eta",
                 "pbrt_proto.SubsurfaceMaterial.eta",
                 "pbrt_proto.UberMaterial.eta",
                 "pbrt_proto.v2.Shape.MaterialOverrides.eta",
                 "pbrt_proto.v3.Shape.MaterialOverrides.eta"})
        .AddSet({"pbrt_proto.v1.Sampler.random", "pbrt_proto.v2.Sampler.random",
                 "pbrt_proto.v3.Sampler.random",
                 "pbrt_proto.v4.Sampler.random"})
        .Build();

}  // namespace

bool FieldsAreEquivalent(const google::protobuf::FieldDescriptor* f0,
                         const google::protobuf::FieldDescriptor* f1) {
  if (!f0 || !f1) {
    return true;
  }

  if (f0->number() != f1->number() || f0->type() != f1->type()) {
    return false;
  }

  if (f0->type() == FieldDescriptor::TYPE_MESSAGE &&
      f0->message_type()->full_name() != f1->message_type()->full_name() &&
      !kEquivalentTypes.AreInSameSet(f0->message_type()->full_name(),
                                     f1->message_type()->full_name())) {
    return false;
  }

  if (f0->name() == f1->name()) {
    return true;
  }

  return kAllowedNameChanges.AreInSameSet(f0->full_name(), f1->full_name());
}

bool AllowedNameCollision(const google::protobuf::FieldDescriptor* f0,
                          const google::protobuf::FieldDescriptor* f1) {
  if (!f0 || !f1 || f0->number() == f1->number()) {
    return false;
  }

  if (f0->type() == f1->type() && f0->type() != FieldDescriptor::TYPE_MESSAGE &&
      f1->type() != FieldDescriptor::TYPE_MESSAGE) {
    return false;
  }

  return kAllowedNameCollisions.AreInSameSet(f0->full_name(), f1->full_name());
}

}  // namespace pbrt_proto
