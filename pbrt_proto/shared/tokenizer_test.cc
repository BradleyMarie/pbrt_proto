#include "pbrt_proto/shared/tokenizer.h"

#include <sstream>

#include "googletest/include/gtest/gtest.h"

namespace pbrt_proto {
namespace {

TEST(Tokenizer, Empty) {
  std::stringstream input;
  Tokenizer tokenizer(input);
  EXPECT_FALSE(tokenizer.Peek().value());
  EXPECT_FALSE(tokenizer.Next().value());
}

TEST(Tokenizer, MoveConstruct) {
  std::stringstream input("hello");
  Tokenizer tokenizer0(input);
  EXPECT_EQ("hello", tokenizer0.Peek().value());

  Tokenizer tokenizer1(std::move(tokenizer0));
  EXPECT_FALSE(tokenizer0.Peek().value());
  EXPECT_FALSE(tokenizer0.Next().value());
  EXPECT_EQ("hello", tokenizer1.Peek().value());
  EXPECT_EQ("hello", tokenizer1.Next().value());
}

TEST(Tokenizer, Move) {
  std::stringstream input("hello");
  Tokenizer tokenizer0(input);
  EXPECT_EQ("hello", tokenizer0.Peek().value());

  std::stringstream empty;
  Tokenizer tokenizer1(empty);
  tokenizer1 = std::move(tokenizer0);

  EXPECT_FALSE(tokenizer0.Peek().value());
  EXPECT_FALSE(tokenizer0.Next().value());
  EXPECT_EQ("hello", tokenizer1.Peek().value());
  EXPECT_EQ("hello", tokenizer1.Next().value());
}

TEST(Tokenizer, QuotedString) {
  std::stringstream input("\"hello world!\"");
  Tokenizer tokenizer(input);
  EXPECT_EQ("\"hello world!\"", tokenizer.Next().value());
}

TEST(Tokenizer, ValidEscapeCodes) {
  std::string escaped_characters[8] = {"b", "f",  "n", "r",
                                       "t", "\\", "'", "\""};
  std::string escaped_values[8] = {"\b", "\f", "\n", "\r",
                                   "\t", "\\", "'",  "\""};
  for (size_t i = 0; i < 8; i++) {
    std::string contents = "\"\\" + escaped_characters[i] + "\"";
    std::stringstream input(contents);
    Tokenizer tokenizer(input);
    EXPECT_EQ("\"" + escaped_values[i] + "\"", tokenizer.Next().value());
  }
}

TEST(Tokenizer, IllegalEscape) {
  std::stringstream input("\"\\!\"");
  Tokenizer tokenizer(input);
  EXPECT_EQ("Illegal escape character", tokenizer.Next().error().message());
}

TEST(Tokenizer, IllegalNewline) {
  std::stringstream input("\"\n\"");
  Tokenizer tokenizer(input);
  EXPECT_EQ("New line found before end of quoted string",
            tokenizer.Next().error().message());
}

TEST(Tokenizer, UnusedEscapeCharacter) {
  std::stringstream input("\"\n\"");
  Tokenizer tokenizer(input);
  EXPECT_EQ("New line found before end of quoted string",
            tokenizer.Next().error().message());
}

TEST(Tokenizer, UnterminatedQuote) {
  std::stringstream input("\"");
  Tokenizer tokenizer(input);
  EXPECT_EQ("Unterminated quoted string", tokenizer.Next().error().message());
}

TEST(Tokenizer, IgnoresComments) {
  std::stringstream input("#ignored one\n#ignored two\nAbc");
  Tokenizer tokenizer(input);
  EXPECT_EQ("Abc", tokenizer.Next().value());
}

TEST(Tokenizer, Array) {
  std::stringstream input("[]");
  Tokenizer tokenizer(input);
  EXPECT_EQ("[", tokenizer.Next().value());
  EXPECT_EQ("]", tokenizer.Next().value());
}

TEST(Tokenizer, MultipleTokens) {
  std::stringstream input("Token1 [1.0] Two \"hello world\" [3] [\"a\"]");
  Tokenizer tokenizer(input);
  EXPECT_EQ("Token1", tokenizer.Peek().value());
  EXPECT_EQ("Token1", tokenizer.Peek().value());
  EXPECT_EQ("Token1", tokenizer.Next().value());
  EXPECT_EQ("[", tokenizer.Peek().value());
  EXPECT_EQ("[", tokenizer.Next().value());
  EXPECT_EQ("1.0", tokenizer.Peek().value());
  EXPECT_EQ("1.0", tokenizer.Next().value());
  EXPECT_EQ("]", tokenizer.Peek().value());
  EXPECT_EQ("]", tokenizer.Next().value());
  EXPECT_EQ("Two", tokenizer.Peek().value());
  EXPECT_EQ("Two", tokenizer.Next().value());
  EXPECT_EQ("\"hello world\"", tokenizer.Peek().value());
  EXPECT_EQ("\"hello world\"", tokenizer.Next().value());
  EXPECT_EQ("[", tokenizer.Peek().value());
  EXPECT_EQ("[", tokenizer.Next().value());
  EXPECT_EQ("3", tokenizer.Peek().value());
  EXPECT_EQ("3", tokenizer.Next().value());
  EXPECT_EQ("]", tokenizer.Peek().value());
  EXPECT_EQ("]", tokenizer.Next().value());
  EXPECT_EQ("[", tokenizer.Peek().value());
  EXPECT_EQ("[", tokenizer.Next().value());
  EXPECT_EQ("\"a\"", tokenizer.Peek().value());
  EXPECT_EQ("\"a\"", tokenizer.Next().value());
  EXPECT_EQ("]", tokenizer.Peek().value());
  EXPECT_EQ("]", tokenizer.Next().value());
  EXPECT_EQ(std::nullopt, tokenizer.Peek().value());
  EXPECT_EQ(std::nullopt, tokenizer.Next().value());
}

}  // namespace
}  // namespace pbrt_proto