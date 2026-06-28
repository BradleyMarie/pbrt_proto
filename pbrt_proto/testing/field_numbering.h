#ifndef _PBRT_PROTO_TESTING_FIELD_NUMBERING_
#define _PBRT_PROTO_TESTING_FIELD_NUMBERING_

#include "google/protobuf/descriptor.pb.h"

namespace pbrt_proto {

bool FieldsContinueContiguously(const google::protobuf::Descriptor* base,
                                const google::protobuf::Descriptor* next);

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_TESTING_FIELD_NUMBERING_
