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

std::ostream& operator<<(std::ostream& os, const Expr& expr) {
  os << static_cast<std::string>(expr);
  return os;
}

class Var : public Expr {
public:
  Var(std::string identifier) : identifier(std::move(identifier)) {}
  operator std::string() { return identifier; }

private:
  std::string identifier;
};

class S : public Expr {
public:
  operator std::string() const { return "S"; }
};

class K : public Expr {
public:
  operator std::string() const { return "K"; }
};

class I : public Expr {
public:
  operator std::string() const { return "I"; }
};

class App : public Expr {
public:
  App(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right)
      : left(std::move(left)), right(std::move(right)) {}
  operator std::string() const {
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

private:
  std::string identifier;
  std::unique_ptr<Expr> expr;
};

class Ski {
public:
  Ski(std::vector<std::unique_ptr<Defn>> defns, std::vector<std::unique_ptr<Expr>> exprs)
      : defns(std::move(defns)), exprs(std::move(exprs)) {}

private:
  std::vector<std::unique_ptr<Defn>> defns;
  std::vector<std::unique_ptr<Expr>> exprs;
};

} // namespace Ski
