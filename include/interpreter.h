#pragma once

#include <memory>
#include <unordered_map>

#include "ast.h"

namespace Ski {

class Interpreter {
public:
  Interpreter(std::unique_ptr<Ski> ski_ast);
  std::unordered_map<std::string, std::unique_ptr<Expr>>& get_resolved_definitions_map() {
    return resolved_definitions_map;
  }
  std::vector<std::string> interpret_exprs();

private:
  std::unique_ptr<Expr> substitute_identifiers(
      std::unique_ptr<Expr> expr,
      std::unordered_map<std::string, std::unique_ptr<Expr>>& resolved_definitions_map);
  std::unique_ptr<Expr> rewite_expr(std::unique_ptr<Expr> expr);

  std::unique_ptr<Ski> ski_ast;
  std::unordered_map<std::string, std::unique_ptr<Expr>> resolved_definitions_map;
};

} // namespace Ski
