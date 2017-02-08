#include "include/Parser.h"
#include <cctype>
#include <array>

namespace Parser {
  void nextToken (const std::string& expression, int& cursor) {
    if(isalnum(expression[cursor])) {
      while(isalnum(expression[cursor])) {
        cursor++;
      }
    }
    else {
      cursor++;
    }
    while(expression[cursor] == ' ') {
      cursor++;
    }
  }

  ASTNode expr (const std::string& expression, int& cursor) {
    return expn(expression, cursor, 0);
  }

  ASTNode expn (const std::string& expression, int& cursor, int n) {
    if(n > maxLevel) {
      return atom(expression, cursor);
    }
    ASTNode node = expn(expression, cursor, n + 1);
    while(operators[n].count(expression[cursor])) {
      ASTNode tmpnode = rxpn(expression, cursor, n);
      node.children = std::vector<ASTNode>{tmpnode.children[0], node, tmpnode.children[1]};
      node.type = ASTNode::Type::HUB;
    }
    return node;
  }

  ASTNode rxpn (const std::string& expression, int& cursor, int n) {
    ASTNode node, op, value;
    op.type = ASTNode::Type::OPERATOR;
    op.value = expression[cursor];
    nextToken(expression, cursor);
    value = expn(expression, cursor, n + 1);
    node.children.push_back(op);
    node.children.push_back(value);
    node.type = ASTNode::Type::HUB;
    return node;
  }

  ASTNode atom (const std::string& expression, int& cursor) {
    ASTNode node;
    if(expression[cursor] == '(') {
      nextToken(expression, cursor);
      node = expr(expression, cursor);
      nextToken(expression, cursor);
    } else if(isdigit(expression[cursor])) {
      int ret;
      sscanf(expression.c_str() + cursor, "%d", &ret);
      node.type = ASTNode::Type::INTEGER;
      node.value = ret;
      nextToken(expression, cursor);
    } else if(isalpha(expression[cursor])) {
      node.type = ASTNode::Type::SYMBOL;
      int tmpcursor = cursor;
      while(isalpha(expression[tmpcursor])) {
        node.symbol.push_back(expression[tmpcursor]);
        tmpcursor++;
      }
      nextToken(expression, cursor);
    } else if(expression[cursor] == '-') {
      node = rxpn(expression, cursor, 99);
    }
    return node;
  }
}
