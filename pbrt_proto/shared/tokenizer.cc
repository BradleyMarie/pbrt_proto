#include "pbrt_proto/shared/tokenizer.h"

#include <istream>
#include <optional>
#include <string>

#include "absl/base/nullability.h"
#include "absl/status/statusor.h"

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

absl::StatusOr<bool> Tokenizer::ParseNext(std::string& output) {
  output.clear();

  if (!stream_) {
    return absl::FailedPreconditionError("Bad Stream");
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
          return absl::InvalidArgumentError(
              "New line found before end of quoted string");
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
              return absl::InvalidArgumentError("Illegal escape character");
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
        return absl::InvalidArgumentError("Unterminated quoted string");
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

absl::StatusOr<absl::Nullable<const std::string*>> Tokenizer::Peek() {
  if (peeked_valid_) {
    if (*peeked_valid_) {
      return &peeked_;
    } else {
      return nullptr;
    }
  }

  absl::StatusOr<bool> found = ParseNext(peeked_);
  if (!found.ok()) {
    return found.status();
  }

  if (*found) {
    peeked_valid_ = *found;
    return &peeked_;
  }

  return nullptr;
}

absl::StatusOr<absl::Nullable<const std::string*>> Tokenizer::Next() {
  std::optional<bool> next_valid;
  if (peeked_valid_) {
    std::swap(next_, peeked_);
    next_valid = *peeked_valid_;
    peeked_valid_ = std::nullopt;
  } else {
    absl::StatusOr<bool> found = ParseNext(next_);
    if (!found.ok()) {
      return found.status();
    }

    next_valid = *found;
  }

  if (*next_valid) {
    return &next_;
  }

  return nullptr;
}

}  // namespace pbrt_proto