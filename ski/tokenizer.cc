#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "../include/tokenizer.h"

namespace Ski {

Tokenizer::Tokenizer(std::string ski_string, std::string ski_filename)
    : ski_string(std::move(ski_string)), ski_filename(std::move(ski_filename)),
      tokens(std::make_unique<std::vector<Token>>()) {
  position = 0;
  line = 1;
  column = 0;
}

std::unique_ptr<std::vector<Token>> Tokenizer::tokenize() {
  while (position < ski_string.size()) {
    try {
      Token token = find_next_token();
      tokens->push_back(token);
    } catch (const std::runtime_error& e) {
      std::cout << e.what() << "\n";
      return nullptr;
    }
  }
  tokens->erase(std::remove_if(tokens->begin(), tokens->end(),
                               [](const Token& token) {
                                 return token.kind == Kind::kLineComment ||
                                        token.kind == Kind::kIngored;
                               }),
                tokens->end());
  return std::move(tokens);
}

char Tokenizer::get_current_char() { return ski_string[position]; }

Token Tokenizer::find_identifier() {
  int start = position;
  if (islower(get_current_char()) || get_current_char() == '_') {
    position++;
    column++;
    while (islower(get_current_char()) || isdigit(get_current_char()) ||
           get_current_char() == '_') {
      position++;
      column++;
    }
  } else {
    throw std::runtime_error(ski_filename + ":" + std::to_string(line) + ":" +
                             std::to_string(column) + ": " + "Invalid character found!");
  }
  std::string lexeme = ski_string.substr(start, position - start);
  if (lexeme == "def") {
    return {Kind::kDef, "def", line, column};
  }
  return {Kind::kIdentifier, lexeme, line, column};
}

Token Tokenizer::find_next_token() {
  char current_char = get_current_char();
  switch (current_char) {
  case 'S':
    position++;
    return {Kind::kSCombinator, "S", line, column++};
  case 'K':
    position++;
    return {Kind::kKCombinator, "K", line, column++};
  case 'I':
    position++;
    return {Kind::kICombinator, "I", line, column++};
  case '(':
    position++;
    return {Kind::kOpenParanthesis, "(", line, column++};
  case ')':
    position++;
    return {Kind::kCloseParanthesis, ")", line, column++};
  case ';':
    position++;
    return {Kind::kSemiColon, ";", line, column++};
  case '=':
    position++;
    return {Kind::kEqual, "=", line, column++};
  case '#':
    position++;
    return find_line_comment();
  case ' ':
  case '\t':
    position++;
    return {Kind::kIngored, " ", line, column++};
  case '\n':
    position++;
    // start a new line
    line++;
    column = 0;
    return {Kind::kIngored, "\n", line, column++};
  default:
    return find_identifier();
  }
}

Token Tokenizer::find_line_comment() {
  int start = position;
  position++;
  column++;
  while (get_current_char() != '\n') {
    position++;
    column++;
  }
  return Token{Kind::kLineComment, ski_string.substr(start, position - start), line, column};
}

} // namespace Ski
