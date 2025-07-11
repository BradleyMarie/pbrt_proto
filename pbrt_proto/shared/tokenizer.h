#ifndef _PBRT_PROTO_SHARED_TOKENIZER_
#define _PBRT_PROTO_SHARED_TOKENIZER_

#include <istream>
#include <optional>
#include <string>

#include "absl/base/nullability.h"
#include "absl/status/statusor.h"

namespace pbrt_proto {

class Tokenizer {
 public:
  Tokenizer(std::istream& stream) noexcept : stream_(&stream) {}

  Tokenizer(Tokenizer&& moved_from) noexcept;
  Tokenizer& operator=(Tokenizer&& moved_from) noexcept;

  Tokenizer(const Tokenizer&) = delete;
  Tokenizer& operator=(const Tokenizer&) = delete;

  absl::StatusOr<const std::string * absl_nullable> Peek();
  absl::StatusOr<const std::string * absl_nullable> Next();

 private:
  absl::StatusOr<bool> ParseNext(std::string& output);

  std::istream* stream_;
  std::string next_;
  std::string peeked_;
  std::optional<bool> peeked_valid_;
};

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_TOKENIZER_
