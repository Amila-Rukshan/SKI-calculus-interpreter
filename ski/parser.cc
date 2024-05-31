#include <memory>
#include <iostream>

#include "../include/parser.h"

namespace Ski {

Parser::Parser(std::unique_ptr<std::vector<Token>> tokens, std::string ski_filename)
    : tokens(std::move(tokens)), ski_filename(std::move(ski_filename)), token_index(0) {}

std::unique_ptr<Ski> Parser::parse() {
  try {
    std::vector<std::unique_ptr<Defn>> defns;
    parse_dfns(defns);
    std::vector<std::unique_ptr<Expr>> exprs;
    parse_exprs(exprs);
    if (defns.empty() && exprs.empty())
      return nullptr;
    return std::make_unique<Ski>(std::move(defns), std::move(exprs));
  } catch (std::exception& e) {
    std::cout << e.what() << "\n";
    return nullptr;
  }
}

void Parser::parse_dfns(std::vector<std::unique_ptr<Defn>>& defns) {
  while (has_tokens() && current_token_kind() == Kind::kDef) {
    defns.push_back(std::move(parse_dfn()));
    read_and_ignore_token(Kind::kSemiColon);
  }
}

std::unique_ptr<Defn> Parser::parse_dfn() {
  read_and_ignore_token(Kind::kDef);
  std::string def_idenritifier;
  if (current_token_kind() == Kind::kIdentifier) {
    def_idenritifier = tokens->at(token_index).lexeme;
    read_and_ignore_token(Kind::kIdentifier);
  }
  read_and_ignore_token(Kind::kEqual);
  auto expr = parse_expr();
  return std::make_unique<Defn>(def_idenritifier, std::move(expr));
}

void Parser::parse_exprs(std::vector<std::unique_ptr<Expr>>& exprs) {
  while (has_tokens() && is_subexpr_start()) {
    exprs.push_back(std::move(parse_expr()));
    read_and_ignore_token(Kind::kSemiColon);
  }
}

std::unique_ptr<Expr> Parser::parse_expr() {
  std::unique_ptr<Expr> expr = parse_subexpr();
  while (is_subexpr_start()) {
    std::unique_ptr<Expr> next_expr = parse_subexpr();
    expr = std::make_unique<App>(std::move(expr), std::move(next_expr));
  }
  return expr;
}

std::unique_ptr<Expr> Parser::parse_subexpr() {
  std::unique_ptr<Expr> expr = nullptr;
  switch (current_token_kind()) {
  case Kind::kIdentifier:
    return read_and_create_ast_node(Kind::kIdentifier);
  case Kind::kSCombinator:
    return read_and_create_ast_node(Kind::kSCombinator);
  case Kind::kKCombinator:
    return read_and_create_ast_node(Kind::kKCombinator);
  case Kind::kICombinator:
    return read_and_create_ast_node(Kind::kICombinator);
  case Kind::kOpenParanthesis:
    read_and_ignore_token(Kind::kOpenParanthesis);
    expr = parse_expr();
    read_and_ignore_token(Kind::kCloseParanthesis);
    return expr;
  default:
    throw std::runtime_error(ski_filename + ":" + std::to_string(tokens->at(token_index).line) +
                             ":" + std::to_string(tokens->at(token_index).column) + ": " +
                             "Invalid token found!");
  }
}

Kind Parser::current_token_kind() { return tokens->at(token_index).kind; }

inline void Parser::read_and_ignore_token(Kind kind) {
  if (tokens->at(token_index).kind != kind) {
    throw std::runtime_error(ski_filename + ":" + std::to_string(tokens->at(token_index).line) +
                             ":" + std::to_string(tokens->at(token_index).column) + ": " +
                             "Expected: '" + kind_to_name[kind] + "'");
  }
  token_index++;
}

std::unique_ptr<Expr> Parser::read_and_create_ast_node(Kind kind) {
  auto curr_token = tokens->at(token_index);
  if (tokens->at(token_index).kind != kind) {
    throw std::runtime_error(ski_filename + ":" + std::to_string(tokens->at(token_index).line) +
                             ":" + std::to_string(tokens->at(token_index).column) + ": " +
                             "Expected: '" + kind_to_name[kind] + "'");
  }
  token_index++;
  switch (kind) {
  case Kind::kIdentifier:
    return std::make_unique<Var>(curr_token.lexeme);
  case Kind::kSCombinator:
    return std::make_unique<S>();
  case Kind::kKCombinator:
    return std::make_unique<K>();
  case Kind::kICombinator:
    return std::make_unique<I>();
  default:
    throw std::runtime_error(ski_filename + ":" + std::to_string(tokens->at(token_index).line) +
                             ":" + std::to_string(tokens->at(token_index).column) + ": " +
                             "Invalid token found!");
  }
}

inline bool Parser::has_tokens() {
  if (token_index < tokens->size()) {
    return true;
  }
  return false;
}

inline bool Parser::is_subexpr_start() {
  return current_token_kind() == Kind::kIdentifier || current_token_kind() == Kind::kSCombinator ||
         current_token_kind() == Kind::kKCombinator || current_token_kind() == Kind::kICombinator ||
         current_token_kind() == Kind::kOpenParanthesis;
}

std::unordered_map<Kind, std::string> Parser::kind_to_name = {{Kind::kIdentifier, "identifier"},
                                                              {Kind::kSCombinator, "S"},
                                                              {Kind::kKCombinator, "K"},
                                                              {Kind::kICombinator, "I"},
                                                              {Kind::kOpenParanthesis, "("},
                                                              {Kind::kCloseParanthesis, ")"},
                                                              {Kind::kDef, "def"},
                                                              {Kind::kSemiColon, ";"},
                                                              {Kind::kEqual, "="}};

} // namespace Ski
