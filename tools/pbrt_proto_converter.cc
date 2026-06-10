#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <optional>
#include <streambuf>
#include <vector>

#include "absl/container/flat_hash_set.h"
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/status/status.h"
#include "google/protobuf/arena.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "google/protobuf/text_format.h"
#include "pbrt_proto/v1/convert.h"
#include "pbrt_proto/v1/v1.pb.h"
#include "pbrt_proto/v3/convert.h"
#include "pbrt_proto/v3/v3.pb.h"

ABSL_FLAG(bool, recursive, false,
          "If true, recursively converts PBRT files that are included or "
          "imported and the files the directives will be updated to reference "
          "the converted files. If not set, encountering these directives will "
          "not be traversed and will be left as-is.");

ABSL_FLAG(bool, validate_only, false,
          "If true, serialized files are deleted immediately after they are "
          "created in order to conserve disk space.");

ABSL_FLAG(bool, write_progress, false,
          "If true, progress is reported to the console.");

ABSL_FLAG(bool, textproto, false,
          "If true, output a text proto instead of a binary proto");

ABSL_FLAG(std::optional<uint16_t>, pbrt_version, std::nullopt,
          "The version of pbrt input specified.");

constexpr size_t kMaxProtoSize = std::numeric_limits<int32_t>::max() / 16;

class ScopedArena {
 public:
  ~ScopedArena() { arena_.Reset(); }

  template <typename T>
  T* Allocate() {
    return google::protobuf::Arena::Create<T>(&arena_);
  }

  void FreeAll() { arena_.Reset(); }

 private:
  google::protobuf::Arena arena_;
};

class NullOstream : public std::ostream, std::streambuf {
 public:
  NullOstream() : std::ostream(this) {}
  int overflow(int c) { return c; }
};

std::string FileExtension() {
  if (absl::GetFlag(FLAGS_textproto)) {
    return "." + std::to_string(*absl::GetFlag(FLAGS_pbrt_version)) + ".txtpb";
  } else {
    return "." + std::to_string(*absl::GetFlag(FLAGS_pbrt_version)) + ".binpb";
  }
}

std::unique_ptr<std::ostream> MakeOstream(
    const std::filesystem::path& output_path) {
  if (absl::GetFlag(FLAGS_validate_only)) {
    return std::make_unique<NullOstream>();
  }

  return std::make_unique<std::ofstream>(output_path,
                                         std::ios::binary | std::ios::out);
}

std::string MakePath(std::filesystem::path partial_file_name,
                     size_t child_index) {
  partial_file_name += ".";
  partial_file_name += std::to_string(child_index);
  partial_file_name += ".pbrt";
  partial_file_name += FileExtension();
  return partial_file_name.string();
}

template <typename T>
void Serialize(std::filesystem::path output_path, size_t file_index,
               const T& proto) {
  std::string prefix;
  if (file_index != 0) {
    prefix = "." + std::to_string(file_index);
  }

  output_path.replace_extension(prefix + ".pbrt" + FileExtension());

  std::unique_ptr<std::ostream> output = MakeOstream(output_path);
  if (!*output) {
    std::cerr << "ERROR: Could not open output file " << output_path
              << std::endl;
    exit(EXIT_FAILURE);
  }

  if (absl::GetFlag(FLAGS_write_progress)) {
    std::cout << "Writing to output: " << output_path.string() << std::endl;
  }

  if (absl::GetFlag(FLAGS_textproto)) {
    google::protobuf::io::OstreamOutputStream zero_copy_output(output.get());
    if (!google::protobuf::TextFormat::Print(proto, &zero_copy_output)) {
      std::cerr << "ERROR: Could not serialize proto to output" << std::endl;
      exit(EXIT_FAILURE);
    }
  } else {
    if (!proto.SerializeToOstream(output.get())) {
      std::cerr << "ERROR: Could not serialize proto to output" << std::endl;
      exit(EXIT_FAILURE);
    }
  }
}

template <typename T, absl::Status (*Convert)(std::istream&, T&)>
void ConvertFile(const std::filesystem::path& search_root,
                 const std::filesystem::path& file,
                 const std::filesystem::path& partial_file_name,
                 std::vector<std::pair<std::filesystem::path, std::string>>&
                     included_files) {
  std::ifstream input(file.c_str(), std::ios_base::in | std::ios_base::binary);
  if (!input) {
    std::cerr << "ERROR: Could not open file: " << file << std::endl;
    exit(EXIT_FAILURE);
  }

  ScopedArena parent_arena;
  T* to_output = parent_arena.Allocate<T>();
  if (absl::Status error = Convert(input, *to_output); !error.ok()) {
    std::cerr << "ERROR: " << error.message() << std::endl;
    exit(EXIT_FAILURE);
  }

  for (auto& directive : *to_output->mutable_directives()) {
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

  if (to_output->ByteSizeLong() < kMaxProtoSize) {
    Serialize(file, 0, *to_output);
    return;
  }

  ScopedArena child_arena;
  T* child = child_arena.Allocate<T>();

  T parent;
  size_t current_size = 0;
  size_t child_index = 1;
  for (const auto& directive : to_output->directives()) {
    if (current_size + directive.ByteSizeLong() > kMaxProtoSize &&
        current_size != 0) {
      parent.add_directives()->mutable_include()->set_path(
          MakePath(partial_file_name, child_index));
      Serialize(file, child_index++, *child);

      child_arena.FreeAll();
      child = child_arena.Allocate<T>();
      current_size = 0;
    }

    current_size += directive.ByteSizeLong();
    *child->add_directives() = directive;
  }

  parent.add_directives()->mutable_include()->set_path(
      MakePath(partial_file_name, child_index));
  Serialize(file, child_index, *child);

  Serialize(file, 0, parent);
}

void ConvertFile(const std::filesystem::path& search_root,
                 const std::filesystem::path& file,
                 const std::filesystem::path& partial_file_name,
                 std::vector<std::pair<std::filesystem::path, std::string>>&
                     included_files) {
  switch (*absl::GetFlag(FLAGS_pbrt_version)) {
    case 1:
      ConvertFile<pbrt_proto::v1::PbrtProto, pbrt_proto::v1::Convert>(
          search_root, file, partial_file_name, included_files);
      break;
    case 2:
      break;
    case 3:
      ConvertFile<pbrt_proto::v3::PbrtProto, pbrt_proto::v3::Convert>(
          search_root, file, partial_file_name, included_files);
      break;
  }
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

  if (*absl::GetFlag(FLAGS_pbrt_version) != 1 &&
      *absl::GetFlag(FLAGS_pbrt_version) != 3) {
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
