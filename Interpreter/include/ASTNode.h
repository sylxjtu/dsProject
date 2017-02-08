#ifndef AST_NODE_H
#define AST_NODE_H

#include "Symbol.h"
#include <unordered_map>
#include <string>
#include <vector>

class ASTNode {
public:
  enum Type {
    INVALID,
    HUB,
    INTEGER,
    OPERATOR,
    SYMBOL,
  };
  Type type;
  uint64_t value;
  std::string symbol;
  std::vector<ASTNode> children;

  // AST display
  // For debug only
  void display();
  int resolve(std::unordered_map<std::string, Symbol>& scope);
};

#endif
