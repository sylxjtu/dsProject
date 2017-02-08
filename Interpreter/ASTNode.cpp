#include "include/ASTNode.h"
#include <cstdio>

// AST display
// For debug only
void ASTNode::display() {
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

int ASTNode::resolve(std::unordered_map<std::string, Symbol>& scope) {
  if(type == HUB) {
    if(children.size() == 2) {
      if(children[0].value == '-') {
        return -children[1].resolve(scope);
      }
    }
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
      }
    }
  } else if(type == INTEGER) {
    return value;
  } else if(type == OPERATOR) {
    throw "Invalid resolve";
  } else if(type == SYMBOL) {
    return scope[symbol].value;
  }
  throw "Invalid resolve";
}
