#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>

// 符号 (代表变量和函数)
class Symbol {
public:
  // 符号名
  std::string name;

  // 符号类型
  enum Type {
    Integer, Function
  };
  Type type;

  // 符号值 (变量的值或函数的起始行号)
  int value;

  Symbol() = default;
  Symbol(std::string name, Type type, int value):
    name(name),
    type(type),
    value(value){}
};

#endif
