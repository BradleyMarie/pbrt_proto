#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <optional>
#include <vector>

#include "absl/container/flat_hash_set.h"
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/status/status.h"
#include "google/protobuf/arena.h"
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

class ScopedArena {
 public:
  ~ScopedArena() { arena_.Reset(); }

  pbrt_proto::v3::PbrtProto* Allocate() {
    return google::protobuf::Arena::Create<pbrt_proto::v3::PbrtProto>(&arena_);
  }

 private:
  static google::protobuf::Arena arena_;
};

google::protobuf::Arena ScopedArena::arena_;

std::string FileExtension() {
  if (absl::GetFlag(FLAGS_textproto)) {
    return ".v3.txtpb";
  } else {
    return ".v3.binpb";
  }
}

void Serialize(std::filesystem::path output_path, size_t file_index,
               const pbrt_proto::v3::PbrtProto& proto) {
  std::string prefix;
  if (file_index != 0) {
    prefix = "." + std::to_string(file_index);
  }

  output_path.replace_extension(prefix + ".pbrt" + FileExtension());

  std::ofstream output(output_path, std::ios::binary | std::ios::out);
  if (!output) {
    std::cerr << "ERROR: Could not open output file " << output_path
              << std::endl;
    exit(EXIT_FAILURE);
  }

  if (absl::GetFlag(FLAGS_textproto)) {
    google::protobuf::io::OstreamOutputStream zero_copy_output(&output);
    if (!google::protobuf::TextFormat::Print(proto, &zero_copy_output)) {
      std::cerr << "ERROR: Could not serialize proto to output" << std::endl;
      exit(EXIT_FAILURE);
    }
  } else {
    if (!proto.SerializeToOstream(&output)) {
      std::cerr << "ERROR: Could not serialize proto to output" << std::endl;
      exit(EXIT_FAILURE);
    }
  }
}

void ConvertFile(const std::filesystem::path& search_root,
                 const std::filesystem::path& file,
                 const std::string& partial_file_name,
                 std::vector<std::pair<std::filesystem::path, std::string>>&
                     included_files) {
  ScopedArena arena;

  std::ifstream input(file.c_str(), std::ios_base::in | std::ios_base::binary);
  if (!input) {
    std::cerr << "ERROR: Could not open file: " << file << std::endl;
    exit(EXIT_FAILURE);
  }

  pbrt_proto::v3::PbrtProto* to_v3 = arena.Allocate();
  if (absl::Status error = pbrt_proto::v3::Convert(input, *to_v3);
      !error.ok()) {
    std::cerr << "ERROR: " << error.message() << std::endl;
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

    if (included_path.is_relative()) {
      included_path = search_root / included_path;
    }

    included_files.emplace_back(included_path,
                                directive.include().path().substr(
                                    0, directive.include().path().size() - 5));
    *directive.mutable_include()->mutable_path() += FileExtension();
  }

  if (to_v3->ByteSizeLong() < kMaxProtoSize) {
    Serialize(file, 0, *to_v3);
    return;
  }

  pbrt_proto::v3::PbrtProto* parent = arena.Allocate();
  pbrt_proto::v3::PbrtProto* child = arena.Allocate();

  size_t current_size = 0;
  size_t child_index = 1;
  for (const auto& directive : to_v3->directives()) {
    if (current_size + directive.ByteSizeLong() > kMaxProtoSize) {
      parent->add_directives()->mutable_include()->set_path(
          partial_file_name + "." + std::to_string(child_index) + ".pbrt" +
          FileExtension());
      Serialize(file, child_index++, *child);

      child->clear_directives();
      current_size = 0;
    }

    current_size += directive.ByteSizeLong();
    *child->add_directives() = directive;
  }

  parent->add_directives()->mutable_include()->set_path(
      partial_file_name + "." + std::to_string(child_index) + ".pbrt" +
      FileExtension());
  Serialize(file, child_index, *child);

  Serialize(file, 0, *parent);
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
  std::filesystem::path canonical_input_path =
      std::filesystem::canonical(input_path, error_code);
  if (error_code) {
    std::cerr << "ERROR: Could not resolve input file to a canonical path"
              << std::endl;
    return EXIT_FAILURE;
  }

  std::vector<std::pair<std::filesystem::path, std::string>> included_files;
  ConvertFile(input_path.parent_path(), input_path, input_path.stem(),
              included_files);

  absl::flat_hash_set<std::filesystem::path> parsed_files;
  parsed_files.insert(canonical_input_path);

  while (!included_files.empty()) {
    auto [next_file, next_partial_file_name] = included_files.back();
    included_files.pop_back();

    std::filesystem::path canonical_next_file =
        std::filesystem::canonical(next_file, error_code);
    if (error_code) {
      std::cerr << "ERROR: Could not resolve included file to a canonical path"
                << std::endl;
      return EXIT_FAILURE;
    }

    if (parsed_files.contains(canonical_next_file)) {
      continue;
    }

    ConvertFile(input_path.parent_path(), next_file, next_partial_file_name,
                included_files);
    parsed_files.insert(next_file);
  }

  return EXIT_SUCCESS;
}
