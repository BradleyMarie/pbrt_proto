#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <optional>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/status/statusor.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "google/protobuf/text_format.h"
#include "pbrt_proto/v3/convert.h"
#include "pbrt_proto/v3/pbrt.pb.h"

ABSL_FLAG(bool, recursive, false,
          "If true, recursively converts PBRT files that are included or "
          "imported and the files the directives will be updated to reference "
          "the converted files. If not set, encountering these directives will "
          "not be traversed and will be left as-is.");

ABSL_FLAG(bool, textproto, false,
          "If true, output a text proto instead of a binary proto");

ABSL_FLAG(std::optional<uint16_t>, pbrt_version, std::nullopt,
          "The version of pbrt input specified.");

constexpr int kMaxProtoSize = std::numeric_limits<int32_t>::max() / 2;

std::string FileExtension() {
  if (absl::GetFlag(FLAGS_textproto)) {
    return ".v3.txtpb";
  } else {
    return ".v3.binpb";
  }
}

std::vector<pbrt_proto::v3::PbrtProto> ConvertFile(
    const std::filesystem::path& file, const std::string& partial_file_name,
    std::vector<std::pair<std::filesystem::path, std::string>>&
        included_files) {
  std::ifstream input(file.c_str(), std::ios_base::in | std::ios_base::binary);
  if (!input) {
    std::cerr << "ERROR: Could not open file: " << file << std::endl;
    exit(EXIT_FAILURE);
  }

  absl::StatusOr<pbrt_proto::v3::PbrtProto> to_v3 =
      pbrt_proto::v3::Convert(input);
  if (!to_v3.ok()) {
    std::cerr << "ERROR: " << to_v3.status().message() << std::endl;
    exit(EXIT_FAILURE);
  }

  for (auto& directive : *to_v3->mutable_directives()) {
    if (!directive.has_include() || !absl::GetFlag(FLAGS_recursive)) {
      continue;
    }

    std::filesystem::path included_path(directive.include().path());
    if (included_path.extension() != ".pbrt") {
      std::cerr << "ERROR: Included files must be pbrt files" << std::endl;
      exit(EXIT_FAILURE);
    }

    std::error_code error_code;
    included_path = std::filesystem::canonical(included_path, error_code);
    if (error_code) {
      std::cerr << "ERROR: Could not resolve an included file path to a "
                   "canonical path"
                << std::endl;
      exit(EXIT_FAILURE);
    }

    included_files.emplace_back(included_path,
                                directive.include().path().substr(
                                    0, directive.include().path().size() - 5));
    *directive.mutable_include()->mutable_path() += FileExtension();
  }

  std::vector<pbrt_proto::v3::PbrtProto> result;
  if (to_v3->ByteSizeLong() < kMaxProtoSize) {
    result.push_back(std::move(*to_v3));
    return result;
  }

  result.emplace_back();

  size_t current_size = 0;
  result[0].add_directives()->mutable_include()->set_path(
      partial_file_name + "." + std::to_string(1) + ".pbrt" + FileExtension());
  result.emplace_back();
  for (const auto& directive : to_v3->directives()) {
    if (current_size + directive.ByteSizeLong() > kMaxProtoSize) {
      result[0].add_directives()->mutable_include()->set_path(
          partial_file_name + "." + std::to_string(result.size()) + ".pbrt" +
          FileExtension());
      result.emplace_back();
      current_size = 0;
    }

    current_size += directive.ByteSizeLong();
    *result.back().add_directives() = directive;
  }

  return result;
}

int main(int argc, char** argv) {
  auto unparsed = absl::ParseCommandLine(argc, argv);
  if (2 != unparsed.size()) {
    std::cerr << "ERROR: Missing input file argument " << std::endl;
    return EXIT_FAILURE;
  }

  if (!absl::GetFlag(FLAGS_pbrt_version).has_value()) {
    std::cerr << "ERROR: PBRT version was not specified" << std::endl;
    return EXIT_FAILURE;
  }

  if (*absl::GetFlag(FLAGS_pbrt_version) != 3) {
    std::cerr << "ERROR: PBRT version was not recognized" << std::endl;
    return EXIT_FAILURE;
  }

  std::filesystem::path input_path(unparsed[1]);
  if (input_path.extension() != ".pbrt") {
    std::cerr << "ERROR: Input file was not a pbrt file" << std::endl;
    return EXIT_FAILURE;
  }

  std::error_code error_code;
  input_path = std::filesystem::canonical(input_path, error_code);
  if (error_code) {
    std::cerr << "ERROR: Could not resolve input file to a canonical path"
              << std::endl;
    return EXIT_FAILURE;
  }

  std::vector<std::pair<std::filesystem::path, std::string>> included_files;
  std::vector<pbrt_proto::v3::PbrtProto> parsed_input_file =
      ConvertFile(input_path, input_path.stem(), included_files);

  absl::flat_hash_map<std::filesystem::path,
                      std::vector<pbrt_proto::v3::PbrtProto>>
      parsed_files;
  parsed_files[input_path] = std::move(parsed_input_file);

  while (!included_files.empty()) {
    auto [next_file, next_partial_file_name] = included_files.back();
    included_files.pop_back();

    if (parsed_files.contains(next_file)) {
      continue;
    }

    parsed_files[next_file] =
        ConvertFile(next_file, next_partial_file_name, included_files);
  }

  for (const auto& [path, protos] : parsed_files) {
    for (size_t i = 0; i < protos.size(); i++) {
      std::filesystem::path output_path = path;

      std::string prefix;
      if (i != 0) {
        prefix = "." + std::to_string(i);
      }

      output_path.replace_extension(prefix + ".pbrt" + FileExtension());

      std::ofstream output(output_path, std::ios::binary | std::ios::out);
      if (!output) {
        std::cerr << "ERROR: Could not open output file" << std::endl;
        return EXIT_FAILURE;
      }

      if (absl::GetFlag(FLAGS_textproto)) {
        google::protobuf::io::OstreamOutputStream zero_copy_output(&output);
        if (!google::protobuf::TextFormat::Print(protos[i],
                                                 &zero_copy_output)) {
          std::cerr << "ERROR: Could not serialize proto to output "
                    << output_path << std::endl;
          return EXIT_FAILURE;
        }
      } else {
        if (!protos[i].SerializeToOstream(&output)) {
          std::cerr << "ERROR: Could not serialize proto to output "
                    << output_path << std::endl;
          return EXIT_FAILURE;
        }
      }
    }
  }

  return EXIT_SUCCESS;
}
