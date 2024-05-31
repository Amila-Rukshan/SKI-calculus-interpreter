#pragma once

#include <memory>
#include <string>
#include <vector>

namespace Ski {

class Expr {
public:
  virtual ~Expr() = default;
  virtual operator std::string() const = 0;
};

inline std::ostream& operator<<(std::ostream& os, const Expr& expr) {
  os << static_cast<std::string>(expr);
  return os;
}

class Var : public Expr {
public:
  Var(std::string identifier) : identifier(std::move(identifier)) {}
  operator std::string() const override { return identifier; }

private:
  std::string identifier;
};

class S : public Expr {
public:
  operator std::string() const override { return "S"; }
};

class K : public Expr {
public:
  operator std::string() const override { return "K"; }
};

class I : public Expr {
public:
  operator std::string() const override { return "I"; }
};

class App : public Expr {
public:
  App(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right)
      : left(std::move(left)), right(std::move(right)) {}
  operator std::string() const override {
    return "(" + static_cast<std::string>(*left) + " " + static_cast<std::string>(*right) + ")";
  }

private:
  std::unique_ptr<Expr> left;
  std::unique_ptr<Expr> right;
};

class Defn {
public:
  Defn(std::string identifier, std::unique_ptr<Expr> expr)
      : identifier(identifier), expr(std::move(expr)) {}
  const std::string& get_identifier() const { return identifier; }
  const Expr& get_expr() const { return *expr; }
  operator std::string() const {
    return "def " + identifier + " = " + static_cast<std::string>(*expr);
  }

private:
  std::string identifier;
  std::unique_ptr<Expr> expr;
};

class Ski {
public:
  Ski(std::vector<std::unique_ptr<Defn>> defns, std::vector<std::unique_ptr<Expr>> exprs)
      : defns(std::move(defns)), exprs(std::move(exprs)) {}
  const std::vector<std::unique_ptr<Defn>>& get_defns() const { return defns; }
  const std::vector<std::unique_ptr<Expr>>& get_exprs() const { return exprs; }
  operator std::string() const {
    std::string result;
    for (const auto& defn : defns)
      result += static_cast<std::string>(*defn) + ";\n";
    result += "\n";
    for (const auto& expr : exprs)
      result += static_cast<std::string>(*expr) + ";\n";
    return result;
  }

private:
  std::vector<std::unique_ptr<Defn>> defns;
  std::vector<std::unique_ptr<Expr>> exprs;
};

} // namespace Ski
