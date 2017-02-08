#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>

class Symbol {
public:
  enum Type {
    Integer, Function
  };
  std::string name;
  Type type;
  int value;
  Symbol() = default;
  Symbol(std::string name, Type type, int value):
    name(name),
    type(type),
    value(value){}
  // For Integer the value
  // For Function the line number
};

#endif
