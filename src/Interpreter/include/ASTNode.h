#ifndef AST_NODE_H
#define AST_NODE_H

#include "Symbol.h"
#include <unordered_map>
#include <string>
#include <vector>

// 抽象语法树节点
class ASTNode {

public:
  // 节点类型
  enum Type {
    INVALID,  // 无效节点
    HUB,      // 非叶节点
    INTEGER,  // 整数字面值节点
    OPERATOR, // 符号节点
    SYMBOL,   // 变量节点
  };
  Type type;

  // 节点值(针对符号节点和整数字面值节点)
  uint64_t value;

  // 节点名称(针对变量节点)
  std::string symbol;

  // 子节点(针对非叶节点)
  std::vector<ASTNode> children;

  // 打印AST(仅用作debug)
  void display();

  // 求值(传入作用域)
  int resolve(std::unordered_map<std::string, Symbol>& scope);
};

#endif
