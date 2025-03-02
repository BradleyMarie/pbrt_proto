#ifndef _PBRT_PROTO_V3_CONVERT_
#define _PBRT_PROTO_V3_CONVERT_

#include <istream>

#include "absl/status/statusor.h"
#include "pbrt_proto/v3/pbrt.pb.h"

namespace pbrt_proto::v3 {

absl::StatusOr<ScenePart> Convert(std::istream& input);

}  // namespace pbrt_proto::v3

#endif  // _PBRT_PROTO_V3_CONVERT_