#ifndef PARSER_H
#define PARSER_H

#include "ASTNode.h"
#include <vector>
#include <string>
#include <unordered_set>

// 表达式解析器
namespace Parser {
  // 最高运算符优先级
  constexpr int maxLevel = 1;

  // 运算符表
  const std::array<std::unordered_set<char>, maxLevel + 1> operators{
    std::unordered_set<char>{'+', '-'},       // 优先级 0
    std::unordered_set<char>{'*', '/', '%'}   // 优先级 1
  };

  // 将指针移向下一个单元(运算符，变量名，数字)的第一个字符
  void nextToken (const std::string& expression, int& cursor);

  // 解析器函数
  ASTNode expr (const std::string& expression, int& cursor);          // 解析一个表达式
  ASTNode expn (const std::string& expression, int& cursor, int n);   // 解析一个只含有优先级大于等于n的运算符的表达式
  ASTNode rxpn (const std::string& expression, int& cursor, int n);   // 解析一个以运算符开头的只含有优先级大于等于n的运算符的表达式
  ASTNode atom (const std::string& expression, int& cursor);          // 解析一个数字、变量、负号表达式
}

#endif
