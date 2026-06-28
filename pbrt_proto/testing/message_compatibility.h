#ifndef _PBRT_PROTO_TESTING_MESSAGE_COMPATIBILITY_
#define _PBRT_PROTO_TESTING_MESSAGE_COMPATIBILITY_

#include "google/protobuf/descriptor.pb.h"

namespace pbrt_proto {

bool MessagesAreCompatible(const google::protobuf::Descriptor& d0,
                           const google::protobuf::Descriptor& d1);

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_TESTING_MESSAGE_COMPATIBILITY_
