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

TEST(SkiParserTest, TestDefinetionsAndExpressions) {
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
  Tokenizer tokenizer(ski_program, "defn.ski");
  Parser parser(std::move(tokenizer.tokenize()), "defn.ski");
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
