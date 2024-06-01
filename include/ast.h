#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace Ski {

class Expr {
public:
  Expr() = default;
  Expr(const Expr&) = default;
  virtual ~Expr() = default;
  virtual operator std::string() const = 0;
  virtual std::unique_ptr<Expr> clone() const = 0;
};

inline std::ostream& operator<<(std::ostream& os, const Expr& expr) {
  os << static_cast<std::string>(expr);
  return os;
}

class Var : public Expr {
public:
  Var(const Var&) = default;
  Var(std::string identifier) : identifier(std::move(identifier)) {}
  operator std::string() const override { return identifier; }
  const std::string& get_identifier() const { return identifier; }
  std::unique_ptr<Expr> clone() const override { return std::make_unique<Var>(*this); }

private:
  std::string identifier;
};

class S : public Expr {
public:
  S() = default;
  S(const S&) = default;
  operator std::string() const override { return "S"; }
  std::unique_ptr<Expr> clone() const override { return std::make_unique<S>(*this); }

private:
  char val = 'S';
};

class K : public Expr {
public:
  K() = default;
  K(const K&) = default;
  operator std::string() const override { return "K"; }
  std::unique_ptr<Expr> clone() const override { return std::make_unique<K>(*this); }

private:
  char val = 'K';
};

class I : public Expr {
public:
  I() = default;
  I(const I&) = default;
  operator std::string() const override { return "I"; }
  std::unique_ptr<Expr> clone() const override { return std::make_unique<I>(*this); }

private:
  char val = 'I';
};

class App : public Expr {
public:
  App(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right)
      : left(std::move(left)), right(std::move(right)) {}
  App(const App& app) : left(app.left->clone()), right(app.right->clone()) {}
  operator std::string() const override {
    return "(" + (left ? static_cast<std::string>(*left) : "") + " " +
           (right ? static_cast<std::string>(*right) : "") + ")";
  }
  std::unique_ptr<Expr> move_left() { return std::move(left); }
  std::unique_ptr<Expr> move_right() { return std::move(right); }
  Expr* get_left() const { return left.get(); }
  Expr* get_right() const { return right.get(); }
  void set_left(std::unique_ptr<Expr> expr) { left = std::move(expr); }
  void set_right(std::unique_ptr<Expr> expr) { right = std::move(expr); }
  std::unique_ptr<Expr> clone() const override { return std::make_unique<App>(*this); }

private:
  std::unique_ptr<Expr> left;
  std::unique_ptr<Expr> right;
};

class Defn {
public:
  Defn(std::string identifier, std::unique_ptr<Expr> expr)
      : identifier(identifier), expr(std::move(expr)) {}
  const std::string& get_identifier() const { return identifier; }
  Expr* get_expr() const { return expr.get(); }
  operator std::string() const {
    return "def " + identifier + " = " + static_cast<std::string>(*expr);
  }

private:
  std::string identifier;
  std::unique_ptr<Expr> expr;
};

class Ski {
public:
  Ski(std::vector<std::unique_ptr<Defn>> defns, std::vector<std::unique_ptr<Expr>> exprs,
      std::vector<std::string> ordered_defs, std::unordered_map<std::string, Expr*> def_map)
      : defns(std::move(defns)), exprs(std::move(exprs)), ordered_defs(std::move(ordered_defs)),
        def_map(std::move(def_map)) {}
  const std::vector<std::unique_ptr<Defn>>& get_defns() const { return defns; }
  const std::vector<std::unique_ptr<Expr>>& get_exprs() const { return exprs; }
  const std::vector<std::string>& get_ordered_defs() const { return ordered_defs; }
  const std::unordered_map<std::string, Expr*>& get_def_map() const { return def_map; }
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
  std::unordered_map<std::string, Expr*> def_map;
  std::vector<std::string> ordered_defs;
};

} // namespace Ski
