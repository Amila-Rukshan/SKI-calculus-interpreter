#include <gtest/gtest.h>

#include "../include/tokenizer.h"

using namespace Ski;

TEST(SkiTokenizerTest, TestSCombinatorToken) {
  Tokenizer tokenizer("S", "test");
  auto tokens = tokenizer.tokenize();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kSCombinator);
  ASSERT_EQ(tokens->at(0).lexeme, "S");
}

TEST(SkiTokenizerTest, TestKCombinatorToken) {
  Tokenizer tokenizer("K", "test");
  auto tokens = tokenizer.tokenize();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kKCombinator);
  ASSERT_EQ(tokens->at(0).lexeme, "K");
}

TEST(SkiTokenizerTest, TestICombinatorToken) {
  Tokenizer tokenizer("I", "test");
  auto tokens = tokenizer.tokenize();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kICombinator);
  ASSERT_EQ(tokens->at(0).lexeme, "I");
}

TEST(SkiTokenizerTest, TestDefinitionToken) {
  Tokenizer tokenizer("def", "test");
  auto tokens = tokenizer.tokenize();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kDef);
  ASSERT_EQ(tokens->at(0).lexeme, "def");
}

TEST(SkiTokenizerTest, TestIdentifierToken) {
  Tokenizer tokenizer("inc", "test");
  auto tokens = tokenizer.tokenize();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kIdentifier);
  ASSERT_EQ(tokens->at(0).lexeme, "inc");
}

TEST(SkiTokenizerTest, TestIdentifierTokenWithUnderscoreAndDigits) {
  Tokenizer tokenizer("_1", "test");
  auto tokens = tokenizer.tokenize();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kIdentifier);
  ASSERT_EQ(tokens->at(0).lexeme, "_1");
}

TEST(SkiTokenizerTest, TestInvalidIdentifierToken) {
  Tokenizer tokenizer("Wzx", "test");
  auto tokens = tokenizer.tokenize();
  ASSERT_EQ(tokens, nullptr);
}

TEST(SkiTokenizerTest, TestEqualToken) {
  Tokenizer tokenizer("=", "test");
  auto tokens = tokenizer.tokenize();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kEqual);
  ASSERT_EQ(tokens->at(0).lexeme, "=");
}

TEST(SkiTokenizerTest, TestOpenParanthesisToken) {
  Tokenizer tokenizer("(", "test");
  auto tokens = tokenizer.tokenize();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kOpenParanthesis);
  ASSERT_EQ(tokens->at(0).lexeme, "(");
}

TEST(SkiTokenizerTest, TestCloseParanthesisToken) {
  Tokenizer tokenizer(")", "test");
  auto tokens = tokenizer.tokenize();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kCloseParanthesis);
  ASSERT_EQ(tokens->at(0).lexeme, ")");
}

TEST(SkiTokenizerTest, TestSemiColonToken) {
  Tokenizer tokenizer(";", "test");
  auto tokens = tokenizer.tokenize();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kSemiColon);
  ASSERT_EQ(tokens->at(0).lexeme, ";");
}

TEST(SkiTokenizerTest, TestTokensForAnExpression) {
  Tokenizer tokenizer("S(KI)K", "test");
  auto tokens = tokenizer.tokenize();
  ASSERT_EQ(tokens->size(), 6);
  ASSERT_EQ(tokens->at(0).kind, Kind::kSCombinator);
  ASSERT_EQ(tokens->at(0).lexeme, "S");
  ASSERT_EQ(tokens->at(1).kind, Kind::kOpenParanthesis);
  ASSERT_EQ(tokens->at(1).lexeme, "(");
  ASSERT_EQ(tokens->at(2).kind, Kind::kKCombinator);
  ASSERT_EQ(tokens->at(2).lexeme, "K");
  ASSERT_EQ(tokens->at(3).kind, Kind::kICombinator);
  ASSERT_EQ(tokens->at(3).lexeme, "I");
  ASSERT_EQ(tokens->at(4).kind, Kind::kCloseParanthesis);
  ASSERT_EQ(tokens->at(4).lexeme, ")");
  ASSERT_EQ(tokens->at(5).kind, Kind::kKCombinator);
  ASSERT_EQ(tokens->at(5).lexeme, "K");
}

TEST(SkiTokenizerTest, TestTokensForAnExpressionWithSpaces) {
  Tokenizer tokenizer("S ( K I ) K", "test");
  auto tokens = tokenizer.tokenize();
  ASSERT_EQ(tokens->size(), 6);
  ASSERT_EQ(tokens->at(0).kind, Kind::kSCombinator);
  ASSERT_EQ(tokens->at(0).lexeme, "S");
  ASSERT_EQ(tokens->at(1).kind, Kind::kOpenParanthesis);
  ASSERT_EQ(tokens->at(1).lexeme, "(");
  ASSERT_EQ(tokens->at(2).kind, Kind::kKCombinator);
  ASSERT_EQ(tokens->at(2).lexeme, "K");
  ASSERT_EQ(tokens->at(3).kind, Kind::kICombinator);
  ASSERT_EQ(tokens->at(3).lexeme, "I");
  ASSERT_EQ(tokens->at(4).kind, Kind::kCloseParanthesis);
  ASSERT_EQ(tokens->at(4).lexeme, ")");
  ASSERT_EQ(tokens->at(5).kind, Kind::kKCombinator);
  ASSERT_EQ(tokens->at(5).lexeme, "K");
}

TEST(SkiTokenizerTest, TestTokensForADefinition) {
  Tokenizer tokenizer("def _1 = inc _0;", "test");
  auto tokens = tokenizer.tokenize();
  ASSERT_EQ(tokens->size(), 6);
  ASSERT_EQ(tokens->at(0).kind, Kind::kDef);
  ASSERT_EQ(tokens->at(0).lexeme, "def");
  ASSERT_EQ(tokens->at(1).kind, Kind::kIdentifier);
  ASSERT_EQ(tokens->at(1).lexeme, "_1");
  ASSERT_EQ(tokens->at(2).kind, Kind::kEqual);
  ASSERT_EQ(tokens->at(2).lexeme, "=");
  ASSERT_EQ(tokens->at(3).kind, Kind::kIdentifier);
  ASSERT_EQ(tokens->at(3).lexeme, "inc");
  ASSERT_EQ(tokens->at(4).kind, Kind::kIdentifier);
  ASSERT_EQ(tokens->at(4).lexeme, "_0");
  ASSERT_EQ(tokens->at(5).kind, Kind::kSemiColon);
  ASSERT_EQ(tokens->at(5).lexeme, ";");
}
