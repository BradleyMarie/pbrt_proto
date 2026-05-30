#ifndef _PBRT_PROTO_SHARED_INTEGRATORS_
#define _PBRT_PROTO_SHARED_INTEGRATORS_

#include "absl/container/flat_hash_map.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {

void RemoveBdptIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    BdptIntegrator& output);

void RemoveDebugIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DebugIntegrator& output);

void RemoveDirectLightingIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DirectLightingIntegrator& result);

void RemoveDirectLightingIntegratorV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    DirectLightingIntegrator& result);

void RemoveIgiIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    IgiIntegrator& result);

void RemoveIrradianceCacheIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    IrradianceCacheIntegrator& result);

void RemovePathIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PathIntegrator& result);

void RemovePhotonMapIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PhotonMapIntegrator& result);

void RemoveExPhotonMapIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    PhotonMapIntegrator& result);

void RemoveWhittedIntegratorV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    WhittedIntegrator& result);

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_INTEGRATORS_
