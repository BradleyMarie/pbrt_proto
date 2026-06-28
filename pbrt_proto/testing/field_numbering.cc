#include "pbrt_proto/testing/field_numbering.h"

#include <algorithm>
#include <set>

#include "absl/log/absl_check.h"
#include "google/protobuf/descriptor.pb.h"

namespace pbrt_proto {
namespace {

using ::google::protobuf::Descriptor;
using ::google::protobuf::FieldDescriptor;

bool IsVersionedDirective(const Descriptor& descriptor) {
  return descriptor.full_name().substr(0, 12) == "pbrt_proto.v";
}

void ReportNoncontiguous(const Descriptor* m0, const Descriptor* m1) {
  std::cout << "NONCONTIGUOUS: ";
  if (m0) {
    std::cout << m0->full_name();
  } else {
    std::cout << "nullptr";
  }

  std::cout << " AND ";

  if (m1) {
    std::cout << m1->full_name();
  } else {
    std::cout << "nullptr";
  }

  std::cout << std::endl;
}

}  // namespace

bool FieldsContinueContiguously(const Descriptor* base,
                                const Descriptor* next) {
  // Directives that were removed in future PBRT versions
  if (base && !next) {
    return true;
  }

  // Non-versioned directives cannot be checked in this way, so ignore them.
  if ((base && !IsVersionedDirective(*base)) ||
      (next && !IsVersionedDirective(*next))) {
    return true;
  }

  // Special handling for the Material in MakeNamedMaterial
  if (base == nullptr && next &&
      next->full_name() == "pbrt_proto.v2.Material") {
    return true;
  }

  std::set<int> fields_to_explore;

  int base_max_field_number = 0;
  if (base) {
    for (int f = 0; f < base->field_count(); f++) {
      const FieldDescriptor* field_descriptor = base->field(f);
      ABSL_CHECK(field_descriptor);

      base_max_field_number =
          std::max(base_max_field_number, field_descriptor->number());
      fields_to_explore.insert(field_descriptor->number());
    }
  }

  int next_max_field_number = 0;
  int num_new_fields = 0;
  if (next) {
    for (int f = 0; f < next->field_count(); f++) {
      const FieldDescriptor* field_descriptor = next->field(f);
      ABSL_CHECK(field_descriptor);

      if (field_descriptor->number() > base_max_field_number) {
        num_new_fields += 1;
      }

      next_max_field_number =
          std::max(next_max_field_number, field_descriptor->number());
      fields_to_explore.insert(field_descriptor->number());
    }
  }

  if (base_max_field_number + num_new_fields != next_max_field_number) {
    ReportNoncontiguous(base, next);
    return false;
  }

  for (int number : fields_to_explore) {
    const Descriptor* next_base = nullptr;
    if (base) {
      const FieldDescriptor* field_descriptor = base->FindFieldByNumber(number);
      if (field_descriptor) {
        next_base = field_descriptor->message_type();
      }
    }

    const Descriptor* next_next = nullptr;
    if (next) {
      const FieldDescriptor* field_descriptor = next->FindFieldByNumber(number);
      if (field_descriptor) {
        next_next = field_descriptor->message_type();
      }
    }

    if (!FieldsContinueContiguously(next_base, next_next)) {
      return false;
    }
  }

  return true;
}

}  // namespace pbrt_proto
