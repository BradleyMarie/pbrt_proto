#ifndef _PBRT_PROTO_V2_CONVERT_
#define _PBRT_PROTO_V2_CONVERT_

#include <istream>

#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "pbrt_proto/v2/v2.pb.h"

namespace pbrt_proto::v2 {

absl::Status Convert(std::istream& input, PbrtProto& output);
absl::StatusOr<PbrtProto> Convert(std::istream& input);

}  // namespace pbrt_proto::v2

#endif  // _PBRT_PROTO_V2_CONVERT_
