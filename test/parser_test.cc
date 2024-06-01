#include <gtest/gtest.h>

#include "../include/tokenizer.h"
#include "../include/parser.h"

using namespace Ski;

TEST(SkiParserTest, TestEmptyProgram) {
  std::string ski_program = "";
  Tokenizer tokenizer(ski_program, "empty.ski");
  Parser parser(std::move(tokenizer.tokenize()), "empty.ski");
  auto ski_ast = parser.parse();

  EXPECT_EQ(ski_ast, nullptr);
}

TEST(SkiParserTest, TestSingleDefinition) {
  std::string ski_program = R"(def c1 = S(KS)K;)";
  Tokenizer tokenizer(ski_program, "defn.ski");
  Parser parser(std::move(tokenizer.tokenize()), "defn.ski");
  auto ski_ast = parser.parse();
  EXPECT_STREQ(R"(def c1 = ((S (K S)) K);

)",
               std::string(*ski_ast).c_str());
}

TEST(SkiParserTest, TestDefinitions) {
  std::string ski_program = R"(def c1 = S (K S) K;
def c2 = S (c1 S (c1 K (c1 S (S (c1 c1 I) (K I)))))(K (c1 K I));
def swap = c1 (c2 I) I;)";
  Tokenizer tokenizer(ski_program, "defns.ski");
  Parser parser(std::move(tokenizer.tokenize()), "defns.ski");
  auto ski_ast = parser.parse();
  EXPECT_STREQ(R"(def c1 = ((S (K S)) K);
def c2 = ((S ((c1 S) ((c1 K) ((c1 S) ((S ((c1 c1) I)) (K I)))))) (K ((c1 K) I)));
def swap = ((c1 (c2 I)) I);

)",
               std::string(*ski_ast).c_str());
}

TEST(SkiParserTest, TestSingleExpression) {
  std::string ski_program = R"((S K) f x;)";
  Tokenizer tokenizer(ski_program, "expr.ski");
  Parser parser(std::move(tokenizer.tokenize()), "expr.ski");
  auto ski_ast = parser.parse();
  EXPECT_STREQ(R"(
(((S K) f) x);
)",
               std::string(*ski_ast).c_str());
}

TEST(SkiParserTest, TestExpressions) {
  std::string ski_program = R"(
(K K )(K K); # outputs K
(S I I) (S I I); # never-ending loop
S (K u) (K v) w; # outputs (u v)
)";
  Tokenizer tokenizer(ski_program, "exprs.ski");
  Parser parser(std::move(tokenizer.tokenize()), "exprs.ski");
  auto ski_ast = parser.parse();
  EXPECT_STREQ(R"(
((K K) (K K));
(((S I) I) ((S I) I));
(((S (K u)) (K v)) w);
)",
               std::string(*ski_ast).c_str());
}

TEST(SkiParserTest, TestDefinitionsAndExpressions) {
  std::string ski_program = R"(# You can write one-line comments in Python style.

# Definition block comes first.
def inc = S (S (K S) K);
def _0  = S K;
def _1  = inc _0;

# Expression block comes next.
_0 f x;
_1 f x;
(inc _1) f x;

# Expected output is as follows:
# x
# (f x)
# (f (f x))
)";
  Tokenizer tokenizer(ski_program, "defnsexprs.ski");
  Parser parser(std::move(tokenizer.tokenize()), "defnsexprs.ski");
  auto ski_ast = parser.parse();
  EXPECT_STREQ(
      R"(def inc = (S ((S (K S)) K));
def _0 = (S K);
def _1 = (inc _0);

((_0 f) x);
((_1 f) x);
(((inc _1) f) x);
)",
      std::string(*ski_ast).c_str());
}
