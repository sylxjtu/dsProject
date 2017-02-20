#include "include/ASTNode.h"
#include <cstdio>

// 打印AST(仅用作debug)
void ASTNode::display() {
  // 结果为LISP表达式
  if(type == HUB) {
    printf("(");
    for(auto n: children) {
      n.display();
      printf(" ");
    }
    printf(")");
  } else if(type == INTEGER) {
    printf("%d", value);
  } else if(type == OPERATOR) {
    printf("%c", value);
  } else if(type == SYMBOL) {
    printf("%s", symbol.c_str());
  } else {
    printf("!INVALID NODE!");
  }
}

// 求值(传入作用域)
int ASTNode::resolve(std::unordered_map<std::string, Symbol>& scope) {
  if(type == HUB) {
    // 对非叶节点递归求值
    // 一元表达式
    if(children.size() == 2) {
      if(children[0].value == '-') {
        return -children[1].resolve(scope);
      }
    }
    // 二元表达式
    if(children.size() == 3) {
      int a = children[1].resolve(scope);
      int b = children[2].resolve(scope);
      switch (children[0].value) {
        case '+':
          return a + b;
          break;
        case '-':
          return a - b;
          break;
        case '*':
          return a * b;
          break;
        case '/':
          return a / b;
          break;
        case '%':
          return a % b;
          break;
      }
    }
  } else if(type == INTEGER) {
    // 整数节点直接求值
    return value;
  } else if(type == OPERATOR) {
    // 符号节点不可求值
    throw "Invalid resolve";
  } else if(type == SYMBOL) {
    // 变量节点到作用域查询值
    return scope[symbol].value;
  }
  throw "Invalid resolve";
}
