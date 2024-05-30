#pragma once

#include <string>

namespace Ski {

enum class Kind {
  kIdentifier,
  kSCombinator,      // K
  kKCombinator,      // S
  kICombinator,      // I
  kOpenParanthesis,  // (
  kCloseParanthesis, // )
  kDef,              // Def
  kSemiColon,        // ;
  kEqual,            // =
  kLineComment,
  kIngored
};

struct Token {
  Kind kind;
  std::string lexeme;
  int line;
  int column;
};

} // namespace Ski
