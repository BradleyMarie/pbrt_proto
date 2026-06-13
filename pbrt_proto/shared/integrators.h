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

absl::Status RemoveAmbientOcclusionIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, AmbientOcclusionIntegrator& output);

absl::Status RemoveBdptIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, BdptIntegrator& output);

absl::Status RemoveDebugIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, DebugIntegrator& output);

absl::Status RemoveDiffusePrtIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, DiffusePrtIntegrator& output);

absl::Status RemoveDipoleSubsurfaceIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, DipoleSubsurfaceIntegrator& output);

absl::Status RemoveDirectLightingIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, DirectLightingIntegrator& output);

absl::Status RemoveGlossyPrtIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, GlossyPrtIntegrator& output);

absl::Status RemoveIgiIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, IgiIntegrator& output);

absl::Status RemoveIrradianceCacheIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, IrradianceCacheIntegrator& output);

absl::Status RemoveMltIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, MltIntegrator& output);

absl::Status RemovePathIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, PathIntegrator& output);

absl::Status RemoveExPhotonMapIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, PhotonMapIntegrator& output);

absl::Status RemovePhotonMapIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, PhotonMapIntegrator& output);

absl::Status RemoveSppmIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, SppmIntegrator& output);

absl::Status RemoveUseProbesIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, UseProbesIntegrator& output);

absl::Status RemoveVolPathIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, VolPathIntegrator& output);

absl::Status RemoveWhittedIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, WhittedIntegrator& output);

//
// Volume Integrators
//

absl::Status RemoveEmissionVolumeIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, EmissionVolumeIntegrator& output);

absl::Status RemoveSingleVolumeIntegrator(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, SingleVolumeIntegrator& output);

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_INTEGRATORS_
