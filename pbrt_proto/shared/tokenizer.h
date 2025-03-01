#ifndef _PBRT_PROTO_SHARED_TOKENIZER_
#define _PBRT_PROTO_SHARED_TOKENIZER_

#include <expected>
#include <filesystem>
#include <istream>
#include <optional>
#include <string_view>
#include <system_error>

namespace pbrt_proto {

class Tokenizer {
 public:
  Tokenizer(std::istream& stream) noexcept : stream_(&stream) {}

  Tokenizer(Tokenizer&& moved_from) noexcept;
  Tokenizer& operator=(Tokenizer&& moved_from) noexcept;

  Tokenizer(const Tokenizer&) = delete;
  Tokenizer& operator=(const Tokenizer&) = delete;

  std::expected<std::optional<std::string_view>, std::error_code> Peek();
  std::expected<std::optional<std::string_view>, std::error_code> Next();

 private:
  std::expected<bool, std::error_code> ParseNext(std::string& output);

  std::istream* stream_;
  std::string next_;
  std::string peeked_;
  std::optional<bool> peeked_valid_;
};

}  // namespace pbrt_proto

#endif  // _IRIS_FRONTEND_TOKENIZER_