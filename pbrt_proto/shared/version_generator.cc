#include <iostream>
#include <memory>
#include <optional>
#include <regex>
#include <string>

#include "google/protobuf/compiler/code_generator.h"
#include "google/protobuf/compiler/plugin.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/io/printer.h"
#include "google/protobuf/io/zero_copy_stream.h"

using ::google::protobuf::Descriptor;
using ::google::protobuf::EnumDescriptor;
using ::google::protobuf::EnumValueDescriptor;
using ::google::protobuf::FieldDescriptor;
using ::google::protobuf::FileDescriptor;
using ::google::protobuf::SourceLocation;
using ::google::protobuf::compiler::CodeGenerator;
using ::google::protobuf::compiler::GeneratorContext;
using ::google::protobuf::io::Printer;
using ::google::protobuf::io::ZeroCopyOutputStream;

static const char* kHeader = R"(
#include "absl/container/flat_hash_map.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/shared/version_set.h"

namespace pbrt_proto {
namespace {

enum PbrtVersion {
  PBRTv1 = 1,
  PBRTv2 = 2,
  PBRTv3 = 3,
  PBRTv4 = 4,
};

)";

static const char* kMessageMapStart =
    R"(static const absl::flat_hash_map<std::string_view, VersionSet>
    kMessageMap = {
)";

static const char* kFieldMapStart =
    R"(static const absl::flat_hash_map<std::string_view, VersionSet>
    kFieldMap = {
)";

static const char* kEnumMapStart =
    R"(static const absl::flat_hash_map<std::string_view, VersionSet>
    kEnumMap = {
)";

static const char* kMapEnd = "};\n\n";

static const char* kFooter = R"(
}  // namespace

VersionSet GetMessageSupportedVersions(absl::string_view full_path) {
  if (auto iter = kMessageMap.find(full_path); iter != kEnumMap.end()) {
    return iter->second;
  }

  return NoVersions();
}

VersionSet IsFieldSupported(absl::string_view full_path, int pbrt_version) {
  if (auto iter = kFieldMap.find(full_path); iter != kEnumMap.end()) {
    return iter->second;
  }

  return AllVersions();
}

VersionSet IsEnumValueSupported(absl::string_view full_path, int pbrt_version) {
  if (auto iter = kEnumMap.find(full_path); iter != kEnumMap.end()) {
    return iter->second;
  }

  return AllVersions();
}

}  // namespace pbrt_proto
)";

std::optional<std::string> GetSupportedVersion(const SourceLocation& location) {
  std::regex pattern(R"(@supported_versions[ \t]*(.*))");

  std::smatch matches;
  if (std::regex_search(location.leading_comments, matches, pattern)) {
    return matches[1].str();
  }

  if (std::regex_search(location.trailing_comments, matches, pattern)) {
    return matches[1].str();
  }

  return std::nullopt;
}

std::optional<std::string> GetMinVersion(const SourceLocation& location) {
  std::regex pattern(R"(@min_version[ \t]*(.*))");

  std::smatch matches;
  if (std::regex_search(location.leading_comments, matches, pattern)) {
    return matches[1].str();
  }

  if (std::regex_search(location.trailing_comments, matches, pattern)) {
    return matches[1].str();
  }

  return std::nullopt;
}

std::optional<std::string> GetMaxVersion(const SourceLocation& location) {
  std::regex pattern(R"(@max_version[ \t]*(.*))");

  std::smatch matches;
  if (std::regex_search(location.leading_comments, matches, pattern)) {
    return matches[1].str();
  }

  if (std::regex_search(location.trailing_comments, matches, pattern)) {
    return matches[1].str();
  }

  return std::nullopt;
}

std::optional<std::string> GetVersions(const SourceLocation& location) {
  if (std::optional<std::string> supported_versions =
          GetSupportedVersion(location)) {
    return "SupportedVersions(" + *supported_versions + ")";
  }

  if (std::optional<std::string> min_version = GetMinVersion(location)) {
    return "MinVersion(" + *min_version + ")";
  }

  if (std::optional<std::string> max_version = GetMaxVersion(location)) {
    return "MaxVersion(" + *max_version + ")";
  }

  return std::nullopt;
}

bool ValidateComment(const SourceLocation& location) {
  int count = 0;

  if (location.leading_comments.find("@supported_versions") !=
      std::string::npos) {
    count += 1;
  }

  if (location.trailing_comments.find("@supported_versions") !=
      std::string::npos) {
    count += 1;
  }

  if (location.leading_comments.find("@min_version") != std::string::npos) {
    count += 1;
  }

  if (location.trailing_comments.find("@min_version") != std::string::npos) {
    count += 1;
  }

  if (location.leading_comments.find("@max_version") != std::string::npos) {
    count += 1;
  }

  if (location.trailing_comments.find("@max_version") != std::string::npos) {
    count += 1;
  }

  return count < 2;
}

class VersionGenerator : public CodeGenerator {
 public:
  bool Generate(const FileDescriptor* file, const std::string& parameter,
                GeneratorContext* context, std::string* error) const override {
    std::unique_ptr<ZeroCopyOutputStream> output(context->Open("version.cc"));
    google::protobuf::io::Printer printer(output.get(), '$');

    // Header
    printer.Print(kHeader);

    // Messages
    printer.Print(kMessageMapStart);

    for (int m = 0; m < file->message_type_count(); ++m) {
      const Descriptor* message_descriptor = file->message_type(m);
      if (!message_descriptor) {
        return false;
      }

      SourceLocation loc;
      if (!message_descriptor->GetSourceLocation(&loc)) {
        continue;
      }

      if (!ValidateComment(loc)) {
        return false;
      }

      if (std::optional<std::string> versions = GetVersions(loc)) {
        printer.Print(R"(        {")");
        printer.Print(message_descriptor->full_name());
        printer.Print(R"(", )");
        printer.Print(*versions);
        printer.Print("},\n");
      }
    }

    printer.Print(kMapEnd);

    // Fields
    printer.Print(kFieldMapStart);

    for (int m = 0; m < file->message_type_count(); ++m) {
      const Descriptor* message_descriptor = file->message_type(m);
      if (!message_descriptor) {
        return false;
      }

      for (int f = 0; f < message_descriptor->field_count(); f++) {
        const FieldDescriptor* field_descriptor = message_descriptor->field(f);
        if (!field_descriptor) {
          return false;
        }

        SourceLocation loc;
        if (!field_descriptor->GetSourceLocation(&loc)) {
          continue;
        }

        if (!ValidateComment(loc)) {
          return false;
        }

        if (std::optional<std::string> versions = GetVersions(loc)) {
          printer.Print(R"(        {")");
          printer.Print(field_descriptor->full_name());
          printer.Print(R"(", )");
          printer.Print(*versions);
          printer.Print("},\n");
        } else {
          printer.Print(R"(        {")");
          printer.Print(field_descriptor->full_name());
          printer.Print(R"(", AllVersions()},)");
          printer.Print("\n");
        }
      }
    }

    printer.Print(kMapEnd);

    // Enum Values
    printer.Print(kEnumMapStart);

    for (int e = 0; e < file->enum_type_count(); ++e) {
      const EnumDescriptor* enum_descriptor = file->enum_type(e);
      if (!enum_descriptor) {
        return false;
      }

      for (int v = 0; v < enum_descriptor->value_count(); v++) {
        const EnumValueDescriptor* value_descriptor = enum_descriptor->value(v);

        SourceLocation loc;
        if (!value_descriptor->GetSourceLocation(&loc)) {
          continue;
        }

        if (!ValidateComment(loc)) {
          return false;
        }

        if (std::optional<std::string> versions = GetVersions(loc)) {
          printer.Print(R"(        {")");
          printer.Print(value_descriptor->full_name());
          printer.Print(R"(", )");
          printer.Print(*versions);
          printer.Print("},\n");
        } else {
          printer.Print(R"(        {")");
          printer.Print(value_descriptor->full_name());
          printer.Print(R"(", AllVersions()},)");
          printer.Print("\n");
        }
      }
    }

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

        for (int v = 0; v < enum_descriptor->value_count(); v++) {
          const EnumValueDescriptor* value_descriptor =
              enum_descriptor->value(v);

          SourceLocation loc;
          if (!value_descriptor->GetSourceLocation(&loc)) {
            continue;
          }

          if (!ValidateComment(loc)) {
            return false;
          }

          if (std::optional<std::string> versions = GetVersions(loc)) {
            printer.Print(R"(        {")");
            printer.Print(value_descriptor->full_name());
            printer.Print(R"(", )");
            printer.Print(*versions);
            printer.Print("},\n");
          } else {
            printer.Print(R"(        {")");
            printer.Print(value_descriptor->full_name());
            printer.Print(R"(", AllVersions()},)");
            printer.Print("\n");
          }
        }
      }
    }

    printer.Print(kMapEnd);

    // Footer
    printer.Print(kFooter);

    return true;
  }
};

int main(int argc, char* argv[]) {
  VersionGenerator generator;
  return google::protobuf::compiler::PluginMain(argc, argv, &generator);
}
