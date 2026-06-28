#include "pbrt_proto/testing/message_compatibility.h"

#include <iostream>

#include "absl/log/absl_check.h"
#include "google/protobuf/descriptor.pb.h"
#include "pbrt_proto/testing/field_compatibility.h"

namespace pbrt_proto {
namespace {

using ::google::protobuf::Descriptor;
using ::google::protobuf::FieldDescriptor;

void ReportIncompatible(const FieldDescriptor* f0, const FieldDescriptor* f1) {
  if (f0 && f1) {
    std::cout << "INCOMPATIBLE: " << f0->full_name() << " AND "
              << f1->full_name() << std::endl;
  }
}

bool MessagesAreCompatible(const Descriptor* d0, const Descriptor* d1) {
  if (!d0 || !d1) {
    return true;
  }

  for (int f = 0; f < d0->field_count(); f++) {
    const FieldDescriptor* f0 = d0->field(f);
    ABSL_CHECK(f0);

    const FieldDescriptor* f1_by_number = d1->FindFieldByNumber(f0->number());
    if (!FieldsAreEquivalent(f0, f1_by_number)) {
      ReportIncompatible(f0, f1_by_number);
      return false;
    }

    const FieldDescriptor* f1_by_name = d1->FindFieldByName(f0->name());
    if (!FieldsAreEquivalent(f0, f1_by_name) &&
        !AllowedNameCollision(f0, f1_by_name)) {
      ReportIncompatible(f0, f1_by_name);
      return false;
    }

    if (f1_by_number && !MessagesAreCompatible(f0->message_type(),
                                               f1_by_number->message_type())) {
      ReportIncompatible(f0, f1_by_number);
      return false;
    }
  }

  return true;
}

}  // namespace

bool MessagesAreCompatible(const Descriptor& d0, const Descriptor& d1) {
  return MessagesAreCompatible(&d0, &d1) && MessagesAreCompatible(&d1, &d0);
}

}  // namespace pbrt_proto
