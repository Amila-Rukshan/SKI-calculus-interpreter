#include <iostream>

#include "../include/interpreter.h"

namespace Ski {

Interpreter::Interpreter(std::unique_ptr<Ski> ski_ast) : ski_ast(std::move(ski_ast)) {
  for (auto& def : this->ski_ast->get_ordered_defs()) {
    std::unique_ptr<Expr> cloned_expr = this->ski_ast->get_def_map().at(def)->clone();
    resolved_definitions_map[def] =
        substitute_identifiers(std::move(cloned_expr), resolved_definitions_map);
  }
}

std::unique_ptr<Expr> Interpreter::substitute_identifiers(
    std::unique_ptr<Expr> expr,
    std::unordered_map<std::string, std::unique_ptr<Expr>>& resolved_definitions_map) {
  if (auto var = dynamic_cast<Var*>(expr.get())) {
    if (resolved_definitions_map.find(var->get_identifier()) != resolved_definitions_map.end())
      return resolved_definitions_map[var->get_identifier()]->clone();
    else
      return expr->clone();
  } else if (auto app = dynamic_cast<App*>(expr.get())) {
    return std::make_unique<App>(
        substitute_identifiers(app->move_left(), resolved_definitions_map),
        substitute_identifiers(app->move_right(), resolved_definitions_map));
  }
  return expr->clone();
}

std::vector<std::string> Interpreter::interpret_exprs() {
  std::vector<std::string> output;
  for (auto& expr : ski_ast->get_exprs()) {
    std::unique_ptr<Expr> cloned_expr = expr->clone();
    std::unique_ptr<Expr> result =
        substitute_identifiers(std::move(cloned_expr), resolved_definitions_map);
    std::unique_ptr<Expr> rewritten_expr = std::move(result);
    std::string expr_string = "";
    while (expr_string != static_cast<std::string>(*rewritten_expr)) {
      expr_string = static_cast<std::string>(*rewritten_expr);
      rewritten_expr = rewite_expr(std::move(rewritten_expr));
    }
    output.push_back(expr_string);
  }
  return output;
}

std::unique_ptr<Expr> Interpreter::rewite_expr(std::unique_ptr<Expr> expr) {
  if (auto app = dynamic_cast<App*>(expr.get())) {
    app->set_left(rewite_expr(std::move(app->move_left())));
    app->set_right(rewite_expr(std::move(app->move_right())));
    // I x = x
    if (dynamic_cast<I*>(app->get_left()))
      return app->move_right();
    // K x y = x
    if (auto app_1 = dynamic_cast<App*>(app->get_left())) {
      if (dynamic_cast<K*>(app_1->get_left())) {
        return app_1->move_right();
      }
    }
    // S x y z = x z (y z)
    if (auto app_1 = dynamic_cast<App*>(app->get_left())) {
      if (auto app_2 = dynamic_cast<App*>(app_1->get_left())) {
        if (dynamic_cast<S*>(app_2->get_left())) {
          std::unique_ptr<Expr> x = app_2->move_right();
          std::unique_ptr<Expr> y = app_1->move_right();
          std::unique_ptr<Expr> z = app->move_right();
          std::unique_ptr<Expr> x_z = std::make_unique<App>(std::move(x), std::move(z->clone()));
          std::unique_ptr<Expr> y_z = std::make_unique<App>(std::move(y), std::move(z));
          return std::make_unique<App>(std::move(x_z), std::move(y_z));
        }
      }
    }
  }
  return expr;
}

} // namespace Ski
