#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#include "token.h"
#include "ast.h"

namespace Ski {

class Parser {
public:
  Parser(std::unique_ptr<std::vector<Token>> tokens, std::string ski_filename);
  std::unique_ptr<Ski> parse();

private:
  void parse_dfns(std::vector<std::unique_ptr<Defn>>& defns);
  std::unique_ptr<Defn> parse_dfn();
  void parse_exprs(std::vector<std::unique_ptr<Expr>>& exprs);
  std::unique_ptr<Expr> parse_expr();
  std::unique_ptr<Expr> parse_subexpr();

  std::unique_ptr<Expr> read_and_create_ast_node(Kind kind);
  inline Kind current_token_kind();
  inline void read_and_ignore_token(Kind kind);
  inline bool has_tokens();
  inline bool is_subexpr_start();

  std::unique_ptr<std::vector<Token>> tokens;
  std::string ski_filename;
  int token_index;
  std::vector<std::string> ordered_defs;
  std::unordered_map<std::string, Expr*> def_map;

  static std::unordered_map<Kind, std::string> kind_to_name;
};

} // namespace Ski
