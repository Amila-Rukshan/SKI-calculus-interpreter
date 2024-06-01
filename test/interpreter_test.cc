#include <gtest/gtest.h>

#include "../include/tokenizer.h"
#include "../include/parser.h"
#include "../include/interpreter.h"

using namespace Ski;

TEST(SkiInterpreterTest, TestICombinatorExpression) {
  std::string ski_program = R"(I i;)";
  Tokenizer tokenizer(ski_program, "test.ski");
  Parser parser(std::move(tokenizer.tokenize()), "test.ski");
  auto ski_ast = parser.parse();
  Interpreter interpreter(std::move(ski_ast));
  auto outputs = interpreter.interpret_exprs();
  ASSERT_EQ(outputs.size(), 1);
  ASSERT_EQ(outputs[0], "i");
}

TEST(SkiInterpreterTest, TestKCombinatorExpression) {
  std::string ski_program = R"(K x y;)";
  Tokenizer tokenizer(ski_program, "test.ski");
  Parser parser(std::move(tokenizer.tokenize()), "test.ski");
  auto ski_ast = parser.parse();
  Interpreter interpreter(std::move(ski_ast));
  auto outputs = interpreter.interpret_exprs();
  ASSERT_EQ(outputs.size(), 1);
  ASSERT_EQ(outputs[0], "x");
}

TEST(SkiInterpreterTest, TestSCombinatorExpression) {
  std::string ski_program = R"(S p q r;)";
  Tokenizer tokenizer(ski_program, "test.ski");
  Parser parser(std::move(tokenizer.tokenize()), "test.ski");
  auto ski_ast = parser.parse();
  Interpreter interpreter(std::move(ski_ast));
  auto outputs = interpreter.interpret_exprs();
  ASSERT_EQ(outputs.size(), 1);
  EXPECT_STREQ(outputs[0].c_str(), "((p r) (q r))");
}

TEST(SkiInterpreterTest, TestMultipleCombinatorApplications) {
  std::string ski_program = R"(S I I I;)";
  Tokenizer tokenizer(ski_program, "test.ski");
  Parser parser(std::move(tokenizer.tokenize()), "test.ski");
  auto ski_ast = parser.parse();
  Interpreter interpreter(std::move(ski_ast));
  auto outputs = interpreter.interpret_exprs();
  ASSERT_EQ(outputs.size(), 1);
  EXPECT_STREQ(outputs[0].c_str(), "I");
}

TEST(SkiInterpreterTest, TestSKINaturalNumbers) {
  std::string ski_program = R"(
def inc = S (S (K S) K);
def _0  = S K;
def _1  = inc _0;

_0 f x;
_1 f x;
(inc _1) f x;
)";
  Tokenizer tokenizer(ski_program, "test.ski");
  Parser parser(std::move(tokenizer.tokenize()), "test.ski");
  auto ski_ast = parser.parse();
  Interpreter interpreter(std::move(ski_ast));
  auto outputs = interpreter.interpret_exprs();
  ASSERT_EQ(outputs.size(), 3);
  EXPECT_STREQ(outputs[0].c_str(), "x");
  EXPECT_STREQ(outputs[1].c_str(), "(f x)");
  EXPECT_STREQ(outputs[2].c_str(), "(f (f x))");
}

TEST(SkiInterpreterTest, TestSwapCombinator) {
  std::string ski_program = R"(
def swap = S (K (SI)) (S (K K) I);
swap a b;
)";
  Tokenizer tokenizer(ski_program, "test.ski");
  Parser parser(std::move(tokenizer.tokenize()), "test.ski");
  auto ski_ast = parser.parse();
  Interpreter interpreter(std::move(ski_ast));
  auto outputs = interpreter.interpret_exprs();
  ASSERT_EQ(outputs.size(), 1);
  EXPECT_STREQ(outputs[0].c_str(), "(b a)");
}

TEST(SkiInterpreterTest, TestIsOddCombinator) {
  std::string ski_program = R"(
def ff = S K;
def tt = K;
def c1 = S (K S) K;
def c2 = S (c1 S (c1 K (c1 S (S (c1 c1 I) (K I)))))(K (c1 K I));
def not = S(S I (K (S K))) (K K);
def is_odd = c2 (c2 I not) ff;

def inc = S (S (K S) K);
def _0  = S K;
def _1  = inc _0;
def _2  = inc _1;

is_odd _0;
is_odd _1;
is_odd _2;
)";
  Tokenizer tokenizer(ski_program, "test.ski");
  Parser parser(std::move(tokenizer.tokenize()), "test.ski");
  auto ski_ast = parser.parse();
  Interpreter interpreter(std::move(ski_ast));
  auto outputs = interpreter.interpret_exprs();
  ASSERT_EQ(outputs.size(), 3);
  EXPECT_STREQ(outputs[0].c_str(), "(S K)");
  EXPECT_STREQ(outputs[1].c_str(), "K");
  EXPECT_STREQ(outputs[2].c_str(), "(S K)");
}

TEST(SkiInterpreterTest, TestArithmeticOperations) {
  std::string ski_program = R"(
def c1 = S (K S) K;
def c2 = S (c1 S (c1 K (c1 S (S (c1 c1 I) (K I)))))(K (c1 K I));
def inc = S (S (K S) K);
def add = c2 ( c1 c1 ( c2 I inc) ) I;


def _0  = S K;
def _1  = inc _0;
def _2  = inc _1;
def _3  = inc _2;
def _4  = inc _3;
def _5  = inc _4;

_5;
add _3 _2;
)";
  Tokenizer tokenizer(ski_program, "test.ski");
  Parser parser(std::move(tokenizer.tokenize()), "test.ski");
  auto ski_ast = parser.parse();
  Interpreter interpreter(std::move(ski_ast));
  auto outputs = interpreter.interpret_exprs();
  ASSERT_EQ(outputs.size(), 2);
  EXPECT_STREQ(outputs[0].c_str(), "((S ((S (K S)) K)) ((S ((S (K S)) K)) ((S ((S (K S)) K)) ((S "
                                   "((S (K S)) K)) ((S ((S (K S)) K)) (S K))))))");
  EXPECT_STREQ(outputs[1].c_str(), "((S ((S (K S)) K)) ((S ((S (K S)) K)) ((S ((S (K S)) K)) ((S "
                                   "((S (K S)) K)) ((S ((S (K S)) K)) (S K))))))");
}
