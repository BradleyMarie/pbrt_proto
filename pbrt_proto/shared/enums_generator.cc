#include <iostream>
#include <memory>
#include <optional>
#include <regex>
#include <string>

#include "absl/strings/str_replace.h"
#include "absl/strings/str_split.h"
#include "google/protobuf/compiler/code_generator.h"
#include "google/protobuf/compiler/plugin.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/io/printer.h"
#include "google/protobuf/io/zero_copy_stream.h"
#include "pbrt_proto/shared/version.h"

using ::google::protobuf::Descriptor;
using ::google::protobuf::EnumDescriptor;
using ::google::protobuf::EnumValueDescriptor;
using ::google::protobuf::FileDescriptor;
using ::google::protobuf::compiler::CodeGenerator;
using ::google::protobuf::compiler::GeneratorContext;
using ::google::protobuf::io::Printer;
using ::google::protobuf::io::ZeroCopyOutputStream;

static const char* kHeader = R"(
#include <string>
#include <utility>
#include <variant>

#include "absl/container/flat_hash_map.h"
#include "absl/functional/function_ref.h"
#include "absl/status/status.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"
#include "pbrt_proto/shared/version_set.h"

namespace pbrt_proto {
namespace {

std::string MakeMessage(int pbrt_version, absl::string_view directive,
                        absl::string_view directive_type,
                        absl::string_view parameter_name,
                        absl::string_view value) {
  return absl::StrCat("Unsupported value for '", directive_type, "' ",
                      directive, " parameter ", "'", parameter_name,
                      "' in PBRTv", pbrt_version, ": \"", value, "\"");
}

absl::Status InvalidEnumError(absl::string_view message, int pbrt_version) {
  if (pbrt_version >= 4) {
    return absl::InvalidArgumentError(message);
  }

  std::cerr << "WARNING: " << message << std::endl;
  return absl::OkStatus();
}

absl::string_view ScatteringDirective(absl::string_view type_name) {
  if (type_name == "subsurface") {
    return "Material";
  }

  if (type_name == "homogeneous" || type_name == "heterogeneous" ||
      type_name == "uniformgrid") {
    return "Medium";
  }

  return "Shape";
}

}  // namespace
)";

static const char* kFooter = "\n} // namespace pbrt_proto\n";

std::string ToStringValue(absl::string_view enum_literal,
                          bool is_scattering_preset) {
  std::string result;
  if (!is_scattering_preset) {
    for (char c : enum_literal) {
      result += std::tolower(c);
    }
    return result;
  }

  for (const auto& token : absl::StrSplit(enum_literal, '_')) {
    if (!result.empty()) {
      result += " ";
    }

    if (token == "AND") {
      result += "&";
      continue;
    }

    for (size_t i = 0; i < token.size(); i++) {
      char c = token[i];
      if (i > 0) {
        c = std::tolower(c);
      }

      result += c;
    }
  }

  return result;
}

absl::string_view GetDirective(absl::string_view type_name) {
  const static absl::flat_hash_map<absl::string_view, absl::string_view>
      kTypes = {
          {"pbrt_proto.BvhAccelerator.SplitMethod", R"("Accelerator")"},
          {"pbrt_proto.SphericalCamera.Mapping", R"("Camera")"},
          {"pbrt_proto.FilmSensor.Type", R"("Film")"},
          {"pbrt_proto.GBufferFilm.CoordinateSystem", R"("Film")"},
          {"pbrt_proto.LightSampler.Type", R"("LightSource")"},
          {"pbrt_proto.DebugIntegrator.ChannelValue", R"("SurfaceIntegrator")"},
          {"pbrt_proto.DirectLightingIntegrator.Strategy",
           R"((pbrt_version <= 2) ? "SurfaceIntegrator" : "Integrator")"},
          {"pbrt_proto.ScatteringPreset.Type",
           "ScatteringDirective(type_name)"},
          {"pbrt_proto.SamplerRandomization.Type", R"("Sampler")"},
          {"pbrt_proto.AdaptiveSampler.Method", R"("Sampler")"},
          {"pbrt_proto.CurveShape.Basis", R"("Shape")"},
          {"pbrt_proto.CurveShape.Degree", R"("Shape")"},
          {"pbrt_proto.CurveShape.Type", R"("Shape")"},
          {"pbrt_proto.CheckerboardAaMode.Type", R"("Texture")"},
          {"pbrt_proto.ImageWrap.Type", R"("Texture")"},
          {"pbrt_proto.TextureFilter.Type", R"("Texture")"},
          {"pbrt_proto.TextureMapping.Type", R"("Texture")"},
      };
  return kTypes.at(type_name);
}

bool GenerateRemoveEnum(const EnumDescriptor& enum_descriptor,
                        bool is_scattering_preset, Printer& printer) {
  printer.Print(R"(
absl::Status RemoveEnum(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, absl::string_view type_name,
    absl::string_view parameter_name,
    absl::FunctionRef<void()");

  std::string_view proto_type_name = enum_descriptor.full_name();
  proto_type_name.remove_prefix(11);

  std::string type_name = absl::StrReplaceAll(proto_type_name, {{".", "::"}});
  printer.Print(absl::StrReplaceAll(type_name, {{".", "::"}}));

  printer.Print(R"()> set_output,
    std::variant<bool*, )");
  printer.Print(type_name);
  printer.Print(R"(> mismatch) {
  static const absl::flat_hash_map<std::string_view, std::pair<VersionSet, int>>
      kMap = {
)");

  for (int v = 0; v < enum_descriptor.value_count(); v++) {
    const EnumValueDescriptor* value_descriptor = enum_descriptor.value(v);
    if (value_descriptor == nullptr) {
      return false;
    }

    pbrt_proto::VersionSet version_set =
        pbrt_proto::GetEnumSupportedVersions(value_descriptor->full_name());
    if (version_set.Empty()) {
      continue;
    }

    printer.Print(R"(          {")");
    printer.Print(
        ToStringValue(value_descriptor->name(), is_scattering_preset));
    printer.Print(R"(", {)");
    printer.Print(version_set.AsCpp());
    printer.Print(", ");
    printer.Print(std::to_string(value_descriptor->number()));
    printer.Print("}},\n");
  }

  printer.Print(
      R"(      };

  std::optional<absl::string_view> value =
      TryRemoveString(parameters, parameter_name);
  if (!value.has_value()) {
    return absl::OkStatus();
  }

  if (auto iter = kMap.find(*value);
      iter != kMap.end() && iter->second.first.Supported(pbrt_version)) {
    set_output(static_cast<)");
  printer.Print(type_name);
  printer.Print(R"(>(iter->second.second));
    return absl::OkStatus();
  }

  std::string message =
      MakeMessage(pbrt_version, )");
  printer.Print(GetDirective(enum_descriptor.full_name()));
  printer.Print(R"(, type_name, parameter_name, *value);

  if ()");
  printer.Print(type_name);
  printer.Print(R"(* default_value =
          std::get_if<)");
  printer.Print(type_name);
  printer.Print(R"(>(&mismatch);
      default_value != nullptr) {
    std::cerr << "WARNING: " << message << std::endl;
    set_output(*default_value);
    return absl::OkStatus();
  }

  bool* mismatch_reporting = *std::get_if<bool*>(&mismatch);
  if (mismatch_reporting) {
    std::cerr << "WARNING: " << message << std::endl;
    *mismatch_reporting = true;
    return absl::OkStatus();
  }

  return InvalidEnumError(message, pbrt_version);
}
)");

  return true;
}

class VersionGenerator : public CodeGenerator {
 public:
  bool Generate(const FileDescriptor* file, const std::string& parameter,
                GeneratorContext* context, std::string* error) const override {
    std::unique_ptr<ZeroCopyOutputStream> output(context->Open("enums.cc"));
    Printer printer(output.get(), '$');

    // Header
    printer.Print(kHeader);

    // Enum Values
    for (int m = 0; m < file->message_type_count(); ++m) {
      const Descriptor* message_descriptor = file->message_type(m);
      if (!message_descriptor) {
        return false;
      }

      for (int e = 0; e < message_descriptor->enum_type_count(); ++e) {
        const EnumDescriptor* enum_descriptor =
            message_descriptor->enum_type(e);
        if (!enum_descriptor) {
          return false;
        }

        if (enum_descriptor->full_name() ==
            "pbrt_proto.ActiveTransform.ActiveTransformation") {
          continue;
        }

        if (enum_descriptor->full_name() == "pbrt_proto.Curve.Type") {
          continue;
        }

        if (enum_descriptor->full_name() == "pbrt_proto.ImageEncoding.Type") {
          continue;
        }

        if (!GenerateRemoveEnum(
                *enum_descriptor,
                /*is_scattering_preset=*/enum_descriptor->full_name() ==
                    "pbrt_proto.ScatteringPreset.Type",
                printer)) {
          return false;
        }
      }
    }

    // Footer
    printer.Print(kFooter);

    return true;
  }
};

int main(int argc, char* argv[]) {
  VersionGenerator generator;
  return google::protobuf::compiler::PluginMain(argc, argv, &generator);
}
