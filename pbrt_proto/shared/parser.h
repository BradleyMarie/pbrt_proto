#ifndef _PBRT_PROTO_SHARED_PARSER_
#define _PBRT_PROTO_SHARED_PARSER_

#include <array>
#include <cstdint>
#include <istream>
#include <optional>
#include <variant>

#include "absl/container/flat_hash_map.h"
#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "absl/types/span.h"

namespace pbrt_proto {

enum class ParameterType {
  BLACKBODY_V1,  // float2
  BLACKBODY_V2,  // float
  BOOL,          // bool
  FLOAT,         // float
  INTEGER,       // integer
  NORMAL3,       // float3
  POINT2,        // float2
  POINT3,        // float3
  RGB,           // float3
  SPECTRUM,      // float2 or string
  STRING,        // string
  TEXTURE,       // string
  VECTOR2,       // float2
  VECTOR3,       // float3
  XYZ,           // float3
};

using ParameterValues =
    std::variant<absl::Span<std::array<double, 3>>,
                 absl::Span<std::array<double, 2>>, absl::Span<double>,
                 absl::Span<int32_t>, absl::Span<absl::string_view>,
                 absl::Span<bool>>;

struct Parameter {
  absl::string_view directive;
  ParameterType type;
  absl::string_view type_name;
  ParameterValues values;
};

enum class ActiveTransformation {
  ALL,
  START_TIME,
  END_TIME,
};

class Parser {
 public:
  absl::Status ReadFrom(std::istream& stream);

 protected:
  Parser(const absl::flat_hash_map<absl::string_view, ParameterType>&
             parameter_type_names)
      : parameter_type_names_(parameter_type_names) {}

 private:
  virtual absl::Status Accelerator(
      absl::string_view accelerator_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) = 0;

  virtual absl::Status ActiveTransform(ActiveTransformation active) = 0;

  virtual absl::Status AttributeBegin() = 0;

  virtual absl::Status AttributeEnd() = 0;

  virtual absl::Status ConcatTransform(double m00, double m01, double m02,
                                       double m03, double m10, double m11,
                                       double m12, double m13, double m20,
                                       double m21, double m22, double m23,
                                       double m30, double m31, double m32,
                                       double m33) = 0;

  virtual absl::Status CoordinateSystem(absl::string_view name) = 0;

  virtual absl::Status CoordSysTransform(absl::string_view name) = 0;

  virtual absl::Status Film(
      absl::string_view film_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) = 0;

  virtual absl::Status Filter(
      absl::string_view filter_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) = 0;

  virtual absl::Status Identity() = 0;

  virtual absl::Status Include(absl::string_view path) = 0;

  virtual absl::Status Integrator(
      absl::string_view integrator_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) = 0;

  virtual absl::Status Import(absl::string_view path) = 0;

  virtual absl::Status LookAt(double eye_x, double eye_y, double eye_z,
                              double look_x, double look_y, double look_z,
                              double up_x, double up_y, double up_z) = 0;

  virtual absl::Status ReverseOrientation() = 0;

  virtual absl::Status Rotate(double angle, double x, double y, double z) = 0;

  virtual absl::Status Sampler(
      absl::string_view accelerator_type,
      absl::flat_hash_map<absl::string_view, Parameter>& parameters) = 0;

  virtual absl::Status Scale(double x, double y, double z) = 0;

  virtual absl::Status Transform(double m00, double m01, double m02, double m03,
                                 double m10, double m11, double m12, double m13,
                                 double m20, double m21, double m22, double m23,
                                 double m30, double m31, double m32,
                                 double m33) = 0;

  virtual absl::Status TransformBegin() = 0;

  virtual absl::Status TransformEnd() = 0;

  virtual absl::Status TransformTimes(double start_time, double end_time) = 0;

  virtual absl::Status Translate(double x, double y, double z) = 0;

  virtual absl::Status WorldBegin() = 0;

  virtual absl::Status WorldEnd() = 0;

  const absl::flat_hash_map<absl::string_view, ParameterType>&
      parameter_type_names_;
};

absl::Status TryRemoveFloats(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name, size_t required_size,
    std::optional<absl::Span<double>>& result);

absl::Status TryRemoveIntegers(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name, size_t required_size,
    std::optional<absl::Span<int32_t>>& result);

std::optional<bool> TryRemoveBool(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name);

std::optional<double> TryRemoveFloat(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name);

std::optional<int32_t> TryRemoveInteger(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name);

std::optional<absl::string_view> TryRemoveString(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    absl::string_view parameter_name);

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_PARSER_