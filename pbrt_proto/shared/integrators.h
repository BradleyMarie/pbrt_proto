#ifndef _PBRT_PROTO_SHARED_INTEGRATORS_
#define _PBRT_PROTO_SHARED_INTEGRATORS_

#include "absl/container/flat_hash_map.h"
#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {

//
// Standard Integrators
//

void RemoveAmbientOcclusionIntegratorV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    AmbientOcclusionIntegrator& output);

void RemoveAmbientOcclusionIntegratorV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    AmbientOcclusionIntegrator& output);

void RemoveBdptIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    BdptIntegrator& output);

void RemoveBdptIntegratorV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    BdptIntegrator& output);

void RemoveDebugIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DebugIntegrator& output);

void RemoveDiffusePrtIntegratorV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DiffusePrtIntegrator& output);

void RemoveDipoleSubsurfaceIntegratorV2(
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

absl::Status RemoveGlossyPrtIntegratorV2(
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

void RemoveMltIntegratorV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    MltIntegrator& output);

void RemovePathIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PathIntegrator& output);

void RemovePathIntegratorV3(
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

void RemoveSppmIntegratorV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SppmIntegrator& output);

void RemoveUseProbesIntegratorV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    UseProbesIntegrator& output);

void RemoveVolPathIntegratorV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    VolPathIntegrator& output);

void RemoveWhittedIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    WhittedIntegrator& output);

void RemoveWhittedIntegratorV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    WhittedIntegrator& output);

//
// Volume Integrators
//

void RemoveEmissionVolumeIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    EmissionVolumeIntegrator& output);

void RemoveSingleVolumeIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SingleVolumeIntegrator& output);

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_INTEGRATORS_
