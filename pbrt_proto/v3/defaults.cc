#include "pbrt_proto/v3/defaults.h"

#include "pbrt_proto/v3/pbrt.pb.h"

namespace pbrt_proto::v3 {
namespace {

PbrtProto BuildDefaults() {
  PbrtProto defaults;

  // Accelerator
  defaults.add_directives()->mutable_accelerator()->mutable_bvh();

  return defaults;
}

}  // namespace

const PbrtProto& GetDefaults() {
  static const PbrtProto defaults = BuildDefaults();
  return defaults;
}

void Canonicalize(PbrtProto& proto) {
  for (Directive& directive : *proto.mutable_directives()) {
    switch (directive.directive_type_case()) {
      case Directive::kAccelerator:
        if (directive.accelerator().accelerator_type_case() ==
            Accelerator::ACCELERATOR_TYPE_NOT_SET) {
          directive.mutable_accelerator()->mutable_bvh();
        }
        break;
      default:
        break;
    }
  }
}

}  // namespace pbrt_proto::v3