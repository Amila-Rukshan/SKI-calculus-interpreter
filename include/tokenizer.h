#pragma once

#include <string>
#include <vector>

#include "token.h"

namespace Ski {

class Tokenizer {

public:
  Tokenizer(std::string ski_string, std::string ski_filename);
  [[nodiscard]]
  std::unique_ptr<std::vector<Token>> tokenize();

private:
  Token find_next_token();
  Token find_identifier();
  Token find_line_comment();
  char get_current_char();

  std::unique_ptr<std::vector<Token>> tokens;
  std::string ski_string;
  std::string ski_filename;
  std::string lexeme;
  int position;
  int line;
  int column;
};

} // namespace Ski
