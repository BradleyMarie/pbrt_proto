#include "pbrt_proto/shared/tokenizer.h"

#include <cassert>
#include <expected>
#include <filesystem>
#include <istream>
#include <optional>
#include <string_view>
#include <system_error>

namespace {

enum class ErrorCode {
  MIN_VALUE = 1,
  ILLEGAL_ESCAPE_CHARACTER = 1,
  UNEXPECTED_END_OF_LINE = 2,
  UNTERMINATED_STRING = 3,
  MAX_VALUE = 3,
};

static class ErrorCategory final : public std::error_category {
  const char* name() const noexcept override;
  std::string message(int condition) const override;
  std::error_condition default_error_condition(
      int value) const noexcept override;
} kErrorCategory;

const char* ErrorCategory::name() const noexcept {
  return "pbrt_proto::Tokenizer";
}

std::string ErrorCategory::message(int condition) const {
  ErrorCode error_code{condition};
  switch (error_code) {
    case ErrorCode::ILLEGAL_ESCAPE_CHARACTER:
      return "Illegal escape character";
    case ErrorCode::UNEXPECTED_END_OF_LINE:
      return "New line found before end of quoted string";
    case ErrorCode::UNTERMINATED_STRING:
      return "Unterminated quoted string";
  };

  return "Unknown Error";
}

std::error_condition ErrorCategory::default_error_condition(
    int value) const noexcept {
  if (value < static_cast<int>(ErrorCode::MIN_VALUE) ||
      value > static_cast<int>(ErrorCode::MAX_VALUE)) {
    return std::error_condition(value, *this);
  }

  return std::make_error_condition(std::errc::invalid_argument);
}

std::error_code make_error_code(ErrorCode code) {
  return std::error_code(static_cast<int>(code), kErrorCategory);
}

}  // namespace

namespace std {

template <>
struct is_error_code_enum<ErrorCode> : true_type {};

}  // namespace std

namespace pbrt_proto {

Tokenizer::Tokenizer(Tokenizer&& moved_from) noexcept
    : stream_(moved_from.stream_),
      next_(moved_from.next_),
      peeked_(moved_from.peeked_),
      peeked_valid_(moved_from.peeked_valid_) {
  moved_from.next_.clear();
  moved_from.peeked_.clear();
  moved_from.peeked_valid_.reset();
}

Tokenizer& Tokenizer::operator=(Tokenizer&& moved_from) noexcept {
  next_ = moved_from.next_;
  peeked_ = moved_from.peeked_;
  peeked_valid_ = moved_from.peeked_valid_;
  moved_from.next_.clear();
  moved_from.peeked_.clear();
  moved_from.peeked_valid_.reset();
  return *this;
}

std::expected<bool, std::error_code> Tokenizer::ParseNext(std::string& output) {
  output.clear();

  if (!stream_) {
    return std::unexpected(std::io_errc::stream);
  }

  for (int read = stream_->get(); read != EOF; read = stream_->get()) {
    if (std::isspace(read)) {
      continue;
    }

    char ch = static_cast<char>(read);

    if (ch == '#') {
      for (read = stream_->get(); read != EOF; read = stream_->get()) {
        ch = static_cast<char>(read);
        if (ch == '\r' || ch == '\n') {
          break;
        }
      }

      continue;
    }

    output.push_back(ch);

    if (ch == '"') {
      bool just_escaped = false;
      bool found_end = false;
      for (read = stream_->get(); read != EOF; read = stream_->get()) {
        ch = static_cast<char>(read);

        if (ch == '\n') {
          return std::unexpected(ErrorCode::UNEXPECTED_END_OF_LINE);
        }

        if (just_escaped) {
          switch (ch) {
            case 'b':
              ch = '\b';
              break;
            case 'f':
              ch = '\f';
              break;
            case 'n':
              ch = '\n';
              break;
            case 'r':
              ch = '\r';
              break;
            case 't':
              ch = '\t';
              break;
            case '\\':
              ch = '\\';
              break;
            case '\'':
              ch = '\'';
              break;
            case '"':
              ch = '"';
              break;
            default:
              return std::unexpected(ErrorCode::ILLEGAL_ESCAPE_CHARACTER);
          }
          just_escaped = false;
        } else if (ch == '\\') {
          just_escaped = true;
          continue;
        } else if (ch == '"') {
          found_end = true;
        }

        output.push_back(ch);

        if (found_end) {
          break;
        }
      }

      if (!found_end) {
        return std::unexpected(ErrorCode::UNTERMINATED_STRING);
      }

      return true;
    }

    if (ch == '[') {
      return true;
    }

    if (ch == ']') {
      return true;
    }

    for (int peeked = stream_->peek(); peeked != EOF;
         peeked = stream_->peek()) {
      if (std::isspace(peeked) || peeked == '"' || peeked == '[' ||
          peeked == ']') {
        break;
      }

      *stream_ >> ch;
      output.push_back(ch);
    }

    return true;
  }

  return false;
}

std::expected<std::optional<std::string_view>, std::error_code>
Tokenizer::Peek() {
  if (peeked_valid_) {
    if (*peeked_valid_) {
      return peeked_;
    } else {
      return std::nullopt;
    }
  }

  std::expected<bool, std::error_code> found = ParseNext(peeked_);
  if (!found.has_value()) {
    return std::unexpected(found.error());
  }

  if (*found) {
    peeked_valid_ = *found;
    return peeked_;
  }

  return std::nullopt;
}

std::expected<std::optional<std::string_view>, std::error_code>
Tokenizer::Next() {
  std::optional<bool> next_valid;
  if (peeked_valid_) {
    std::swap(next_, peeked_);
    next_valid = *peeked_valid_;
    peeked_valid_ = std::nullopt;
  } else {
    std::expected<bool, std::error_code> found = ParseNext(next_);
    if (!found.has_value()) {
      return std::unexpected(found.error());
    }

    next_valid = *found;
  }

  if (*next_valid) {
    return next_;
  }

  return std::nullopt;
}

}  // namespace pbrt_proto