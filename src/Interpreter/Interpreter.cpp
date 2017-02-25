#include <bits/stdc++.h>
#include "include/Parser.h"
#include "include/Symbol.h"
using namespace std;

// 全局变量
unordered_map<string, Symbol> symbols;   // 符号表
vector<string> program;                  // 程序代码
char buffer[2][1024];                    // 临时缓存

// 辅助函数
// 检验某字符串是否以模式串开头
bool beginWith(const string& source, const string& pattern) {
  return source.substr(0, pattern.size()) == pattern;
}

// 解析表达式
int parseExpression (string expression) {
  int cursor = 0;
  // 防止nextToken越过表达式末尾
  expression.push_back('$');
  return Parser::expr(expression, cursor).resolve(symbols);
}

// 读取代码
void readProgram () {
  string temp;
  while(getline(cin, temp)) {
    // 当代码行不以字母开头时结束
    if(temp != "" && isalpha(temp[0])) {
      program.push_back(temp);
    }
  }
}

// 语句执行函数
bool executeStatement (const string& statement, unsigned& lineno);
bool executeLine (const string& line, unsigned& lineno);
void executeAssign (const string& statement, unsigned& lineno);
void executeDefine (const string& statement, unsigned& lineno);
void executeCall (const string& statement, unsigned& lineno);

// 赋值
void executeAssign (const string& statement, unsigned& lineno) {
  sscanf(statement.c_str(), "%*s%s %[^\n]", buffer[0], buffer[1]);
  string symbol(buffer[0]), expression(buffer[1]);
  int value = parseExpression(expression);
  symbols[symbol] = Symbol(symbol, Symbol::Integer, value);
  cout << "Assigning " << value << " to " << symbol << endl;
  lineno++;
}

// 定义函数
void executeDefine (const string& statement, unsigned& lineno) {
  sscanf(statement.c_str(), "%*s%s", buffer[0]);
  string symbol(buffer[0]);
  symbols.emplace(make_pair(symbol, Symbol(symbol, Symbol::Function, lineno)));
  cout << "Defining " << symbol << endl;
  // 跳转到函数定义结束
  while(!beginWith(program[lineno], "END")) {
    if(lineno >= program.size()) {
      cout << "ERROR: \"END\" not found in function definition" << endl;
      exit(0);
    }
    lineno++;
  }
  lineno++;
}

// 执行函数
void executeCall (const string& statement, unsigned& lineno) {
  sscanf(statement.c_str(), "%*s%s", buffer[0]);
  string symbol(buffer[0]);
  cout << "Calling " << symbol << endl;
  unsigned sl = symbols[symbol].value + 1;
  while(executeLine(program[sl], sl))
  ;
  lineno++;
}

// 执行语句
bool executeStatement (const string& statement, unsigned& lineno) {
  if(beginWith(statement, "ASSIGN")) {
    executeAssign(statement, lineno);
  } else if(beginWith(statement, "DEFINE")) {
    executeDefine(statement, lineno);
  } else if(beginWith(statement, "END")) {
    // 函数块结束
    return false;
  } else if(beginWith(statement, "CALL")) {
    executeCall(statement, lineno);
  }
  // 函数块未结束
  return true;
}

// 执行代码行
bool executeLine (const string& line, unsigned& lineno) {
  if(!beginWith(line, "FOR")) {
    // 若不以FOR开头，则为单一语句
    return executeStatement(line, lineno);
  } else {
    int cursor = 0;
    sscanf(line.c_str(), "%*s %[^\n]", buffer[0]);
    string blendExpr(buffer[0]);
    int times = Parser::expr(blendExpr, cursor).resolve(symbols);
    // 不需要传递代码行， 用dummy代替
    unsigned dummy = 0;
    for(int i = 0; i < times; i++) {
      executeLine(blendExpr.substr(cursor), dummy);
    }
    lineno++;
    return true;
  }
}

int main () {
  readProgram();
  // 当前执行行号
  unsigned lp = 0;
  // 主循环
  while(lp < program.size()) {
    executeLine(program[lp], lp);
  }
}
