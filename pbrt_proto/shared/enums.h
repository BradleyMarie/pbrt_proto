#ifndef _PBRT_PROTO_SHARED_ENUMS_
#define _PBRT_PROTO_SHARED_ENUMS_

#include <variant>

#include "absl/container/flat_hash_map.h"
#include "absl/functional/function_ref.h"
#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {

absl::Status RemoveEnum(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, absl::string_view type_name,
    absl::string_view parameter_name,
    absl::FunctionRef<void(BvhAccelerator::SplitMethod)> set_output,
    std::variant<bool*, BvhAccelerator::SplitMethod> mismatch = nullptr);

absl::Status RemoveEnum(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, absl::string_view type_name,
    absl::string_view parameter_name,
    absl::FunctionRef<void(SphericalCamera::Mapping)> set_output,
    std::variant<bool*, SphericalCamera::Mapping> mismatch = nullptr);

absl::Status RemoveEnum(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, absl::string_view type_name,
    absl::string_view parameter_name,
    absl::FunctionRef<void(FilmSensor::Type)> set_output,
    std::variant<bool*, FilmSensor::Type> mismatch = nullptr);

absl::Status RemoveEnum(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, absl::string_view type_name,
    absl::string_view parameter_name,
    absl::FunctionRef<void(GBufferFilm::CoordinateSystem)> set_output,
    std::variant<bool*, GBufferFilm::CoordinateSystem> mismatch = nullptr);

absl::Status RemoveEnum(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, absl::string_view type_name,
    absl::string_view parameter_name,
    absl::FunctionRef<void(LightSampler::Type)> set_output,
    std::variant<bool*, LightSampler::Type> mismatch = nullptr);

absl::Status RemoveEnum(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, absl::string_view type_name,
    absl::string_view parameter_name,
    absl::FunctionRef<void(DebugIntegrator::ChannelValue)> set_output,
    std::variant<bool*, DebugIntegrator::ChannelValue> mismatch = nullptr);

absl::Status RemoveEnum(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, absl::string_view type_name,
    absl::string_view parameter_name,
    absl::FunctionRef<void(DirectLightingIntegrator::Strategy)> set_output,
    std::variant<bool*, DirectLightingIntegrator::Strategy> mismatch = nullptr);

absl::Status RemoveEnum(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, absl::string_view type_name,
    absl::string_view parameter_name,
    absl::FunctionRef<void(ScatteringPreset::Type)> set_output,
    std::variant<bool*, ScatteringPreset::Type> mismatch = nullptr);

absl::Status RemoveEnum(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, absl::string_view type_name,
    absl::string_view parameter_name,
    absl::FunctionRef<void(SamplerRandomization::Type)> set_output,
    std::variant<bool*, SamplerRandomization::Type> mismatch = nullptr);

absl::Status RemoveEnum(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, absl::string_view type_name,
    absl::string_view parameter_name,
    absl::FunctionRef<void(AdaptiveSampler::Method)> set_output,
    std::variant<bool*, AdaptiveSampler::Method> mismatch = nullptr);

absl::Status RemoveEnum(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, absl::string_view type_name,
    absl::string_view parameter_name,
    absl::FunctionRef<void(CurveShape::Basis)> set_output,
    std::variant<bool*, CurveShape::Basis> mismatch = nullptr);

absl::Status RemoveEnum(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, absl::string_view type_name,
    absl::string_view parameter_name,
    absl::FunctionRef<void(CurveShape::Degree)> set_output,
    std::variant<bool*, CurveShape::Degree> mismatch = nullptr);

absl::Status RemoveEnum(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, absl::string_view type_name,
    absl::string_view parameter_name,
    absl::FunctionRef<void(CurveShape::Type)> set_output,
    std::variant<bool*, CurveShape::Type> mismatch = nullptr);

absl::Status RemoveEnum(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, absl::string_view type_name,
    absl::string_view parameter_name,
    absl::FunctionRef<void(CheckerboardAaMode::Type)> set_output,
    std::variant<bool*, CheckerboardAaMode::Type> mismatch = nullptr);

absl::Status RemoveEnum(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, absl::string_view type_name,
    absl::string_view parameter_name,
    absl::FunctionRef<void(ImageWrap::Type)> set_output,
    std::variant<bool*, ImageWrap::Type> mismatch = nullptr);

absl::Status RemoveEnum(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, absl::string_view type_name,
    absl::string_view parameter_name,
    absl::FunctionRef<void(TextureFilter::Type)> set_output,
    std::variant<bool*, TextureFilter::Type> mismatch = nullptr);

absl::Status RemoveEnum(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, absl::string_view type_name,
    absl::string_view parameter_name,
    absl::FunctionRef<void(TextureMapping::Type)> set_output,
    std::variant<bool*, TextureMapping::Type> mismatch = nullptr);

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_ENUMS_
