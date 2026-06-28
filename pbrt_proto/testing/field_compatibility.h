#ifndef _PBRT_PROTO_TESTING_FIELD_COMPATIBILITY_
#define _PBRT_PROTO_TESTING_FIELD_COMPATIBILITY_

#include "google/protobuf/descriptor.pb.h"

namespace pbrt_proto {

bool FieldsAreEquivalent(const google::protobuf::FieldDescriptor* f0,
                         const google::protobuf::FieldDescriptor* f1);

bool AllowedNameCollision(const google::protobuf::FieldDescriptor* f0,
                          const google::protobuf::FieldDescriptor* f1);

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_TESTING_FIELD_COMPATIBILITY_
