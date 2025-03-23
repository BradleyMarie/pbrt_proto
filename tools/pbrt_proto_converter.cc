#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
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

pbrt_proto::v3::PbrtProto ConvertFile(
    const std::filesystem::path& file,
    std::vector<std::filesystem::path>& included_files) {
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

    included_files.push_back(included_path);

    if (absl::GetFlag(FLAGS_textproto)) {
      *directive.mutable_include()->mutable_path() += ".v3.txtpb";
    } else {
      *directive.mutable_include()->mutable_path() += ".v3.binpb";
    }
  }

  return *std::move(to_v3);
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

  std::vector<std::filesystem::path> included_files;
  pbrt_proto::v3::PbrtProto parsed_input_file =
      ConvertFile(input_path, included_files);

  absl::flat_hash_map<std::filesystem::path, pbrt_proto::v3::PbrtProto>
      parsed_files;
  parsed_files[input_path] = std::move(parsed_input_file);

  while (!included_files.empty()) {
    std::filesystem::path next_file = included_files.back();
    included_files.pop_back();

    if (parsed_files.contains(next_file)) {
      continue;
    }

    parsed_files[next_file] = ConvertFile(next_file, included_files);
  }

  for (const auto& [path, proto] : parsed_files) {
    std::filesystem::path output_path = path;
    if (absl::GetFlag(FLAGS_textproto)) {
      output_path += ".v3.txtpb";
    } else {
      output_path += ".v3.binpb";
    }

    std::ofstream output(output_path, std::ios::binary | std::ios::out);
    if (!output) {
      std::cerr << "ERROR: Could not open output file" << std::endl;
      return EXIT_FAILURE;
    }

    if (absl::GetFlag(FLAGS_textproto)) {
      google::protobuf::io::OstreamOutputStream zero_copy_output(&output);
      if (!google::protobuf::TextFormat::Print(proto, &zero_copy_output)) {
        std::cerr << "ERROR: Could not serialize proto to output" << std::endl;
        return EXIT_FAILURE;
      }
    } else {
      if (!proto.SerializeToOstream(&output)) {
        std::cerr << "ERROR: Could not serialize proto to output" << std::endl;
        return EXIT_FAILURE;
      }
    }
  }

  return EXIT_SUCCESS;
}