#ifndef _PBRT_PROTO_SHARED_PARSER_
#define _PBRT_PROTO_SHARED_PARSER_

#include <istream>

#include "absl/status/status.h"
#include "absl/strings/string_view.h"

namespace pbrt_proto {

enum class ActiveTransformation {
  ALL,
  START_TIME,
  END_TIME,
};

class Parser {
 public:
  absl::Status ReadFrom(std::istream& stream);

 private:
  virtual absl::Status ActiveTransform(ActiveTransformation active) = 0;

  virtual absl::Status ConcatTransform(double m00, double m01, double m02,
                                       double m03, double m10, double m11,
                                       double m12, double m13, double m20,
                                       double m21, double m22, double m23,
                                       double m30, double m31, double m32,
                                       double m33) = 0;

  virtual absl::Status CoordinateSystem(absl::string_view name) = 0;

  virtual absl::Status CoordSysTransform(absl::string_view name) = 0;

  virtual absl::Status Identity() = 0;

  virtual absl::Status Include(absl::string_view path) = 0;

  virtual absl::Status Import(absl::string_view path) = 0;

  virtual absl::Status LookAt(double eye_x, double eye_y, double eye_z,
                              double look_x, double look_y, double look_z,
                              double up_x, double up_y, double up_z) = 0;

  virtual absl::Status ReverseOrientation() = 0;

  virtual absl::Status Rotate(double angle, double x, double y, double z) = 0;

  virtual absl::Status Scale(double x, double y, double z) = 0;

  virtual absl::Status Transform(double m00, double m01, double m02, double m03,
                                 double m10, double m11, double m12, double m13,
                                 double m20, double m21, double m22, double m23,
                                 double m30, double m31, double m32,
                                 double m33) = 0;

  virtual absl::Status TransformTimes(double start_time, double end_time) = 0;

  virtual absl::Status Translate(double x, double y, double z) = 0;

  virtual absl::Status WorldBegin() = 0;

  virtual absl::Status WorldEnd() = 0;
};

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_PARSER_