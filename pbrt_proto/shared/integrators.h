#ifndef _PBRT_PROTO_SHARED_INTEGRATORS_
#define _PBRT_PROTO_SHARED_INTEGRATORS_

#include "absl/container/flat_hash_map.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {

void RemoveAmbientOcclusionIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    AmbientOcclusionIntegrator& output);

void RemoveAmbientOcclusionIntegratorV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    AmbientOcclusionIntegrator& output);

void RemoveBdptIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    BdptIntegrator& output);

void RemoveBdptIntegratorV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    BdptIntegrator& output);

void RemoveDebugIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DebugIntegrator& output);

void RemoveDiffusePrtIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DiffusePrtIntegrator& output);

void RemoveDipoleSubsurfaceIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DipoleSubsurfaceIntegrator& output);

void RemoveDirectLightingIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DirectLightingIntegrator& output);

void RemoveDirectLightingIntegratorV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DirectLightingIntegrator& output);

void RemoveDirectLightingIntegratorV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DirectLightingIntegrator& output);

void RemoveDiffusePrtIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DiffusePrtIntegrator& output);

void RemoveGlossyPrtIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    GlossyPrtIntegrator& output);

void RemoveIgiIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    IgiIntegrator& output);

void RemoveIgiIntegratorV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    IgiIntegrator& output);

void RemoveIrradianceCacheIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    IrradianceCacheIntegrator& output);

void RemoveMltIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    MltIntegrator& output);

void RemovePathIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PathIntegrator& output);

void RemovePathIntegratorV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PathIntegrator& output);

void RemovePhotonMapIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PhotonMapIntegrator& output);

void RemoveExPhotonMapIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PhotonMapIntegrator& output);

void RemovePhotonMapIntegratorV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PhotonMapIntegrator& output);

void RemoveSppmIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SppmIntegrator& output);

void RemoveUseProbesIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    UseProbesIntegrator& output);

void RemoveVolPathIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    VolPathIntegrator& output);

void RemoveWhittedIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    WhittedIntegrator& output);

void RemoveWhittedIntegratorV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    WhittedIntegrator& output);

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_INTEGRATORS_
