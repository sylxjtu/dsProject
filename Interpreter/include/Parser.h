#ifndef PARSER_H
#define PARSER_H

#include "ASTNode.h"
#include <vector>
#include <string>
#include <unordered_set>

namespace Parser {
  constexpr int maxLevel = 1;
  const std::array<std::unordered_set<char>, maxLevel + 1> operators{
    std::unordered_set<char>{'+', '-'},
    std::unordered_set<char>{'*', '/', '%'}
  };
  void nextToken (const std::string& expression, int& cursor);
  ASTNode expr (const std::string& expression, int& cursor);
  ASTNode expn (const std::string& expression, int& cursor, int n);
  ASTNode rxpn (const std::string& expression, int& cursor, int n);
  ASTNode atom (const std::string& expression, int& cursor);
}

#endif
