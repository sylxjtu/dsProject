#include "include/Parser.h"
#include <cctype>
#include <array>

// 表达式解析器
namespace Parser {
  // 将指针移向下一个单元(运算符，变量名，数字)的第一个字符
  void nextToken (const std::string& expression, int& cursor) {
    // 跳过当前单元
    if(isalnum(expression[cursor])) {
      while(isalnum(expression[cursor])) {
        cursor++;
      }
    }
    else {
      cursor++;
    }

    // 跳过空白字符
    while(expression[cursor] == ' ') {
      cursor++;
    }
  }

  // 解析一个表达式
  ASTNode expr (const std::string& expression, int& cursor) {
    return expn(expression, cursor, 0);
  }

  // 解析一个只含有优先级大于等于n的运算符的表达式
  ASTNode expn (const std::string& expression, int& cursor, int n) {
    // n大于最大优先级时不可能有运算符满足，转为求解原子表达式
    if(n > maxLevel) {
      return atom(expression, cursor);
    }

    // 先解析当前位置的高阶表达式
    ASTNode node = expn(expression, cursor, n + 1);

    // 当前符号为优先级n
    while(operators[n].count(expression[cursor])) {
      // 解析剩余表达式
      ASTNode tmpnode = rxpn(expression, cursor, n);
      // 构造AST节点
      node.children = std::vector<ASTNode>{tmpnode.children[0], node, tmpnode.children[1]};
      node.type = ASTNode::Type::HUB;
    }
    return node;
  }

  // 解析一个以符号开头的n级剩余表达式
  ASTNode rxpn (const std::string& expression, int& cursor, int n) {
    ASTNode node, op, value;

    // 读取运算符
    op.type = ASTNode::Type::OPERATOR;
    op.value = expression[cursor];
    nextToken(expression, cursor);

    // 读取剩余部分
    value = expn(expression, cursor, n + 1);
    node.children.push_back(op);
    node.children.push_back(value);

    // 返回临时AST节点
    node.type = ASTNode::Type::HUB;
    return node;
  }

  ASTNode atom (const std::string& expression, int& cursor) {
    ASTNode node;

    if(expression[cursor] == '(') {
      // 略过括号，解析括号内表达式
      nextToken(expression, cursor);
      node = expr(expression, cursor);
      nextToken(expression, cursor);
    } else if(isdigit(expression[cursor])) {
      // 解析整数字面值
      int ret;
      sscanf(expression.c_str() + cursor, "%d", &ret);
      node.type = ASTNode::Type::INTEGER;
      node.value = ret;
      nextToken(expression, cursor);
    } else if(isalpha(expression[cursor])) {
      // 解析变量名
      node.type = ASTNode::Type::SYMBOL;
      int tmpcursor = cursor;
      while(isalpha(expression[tmpcursor])) {
        node.symbol.push_back(expression[tmpcursor]);
        tmpcursor++;
      }
      nextToken(expression, cursor);
    } else if(expression[cursor] == '-') {
      // 解析负号表达式
      node = rxpn(expression, cursor, 99);
    }
    return node;
  }
}
