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

void RemoveBdptIntegratorV1(
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

void RemovePathIntegratorV1(
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

void RemoveUseProbesIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    UseProbesIntegrator& output);

void RemoveWhittedIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    WhittedIntegrator& output);

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_INTEGRATORS_
