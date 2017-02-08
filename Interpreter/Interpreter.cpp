#include <bits/stdc++.h>
#include "include/Parser.h"
#include "include/Symbol.h"
using namespace std;

// Global datas
unordered_map<string, Symbol> symbols;
vector<string> program;

// Temp stores
char buffer[2][1024];

// Helper functions
bool beginWith(const string& source, const string& pattern) {
  return source.substr(0, pattern.size()) == pattern;
}

int parseExpression (string expression) {
  int cursor = 0;
  expression.push_back('$');
  return Parser::expr(expression, cursor).resolve(symbols);
}

void readProgram () {
  string temp;
  while(getline(cin, temp)) {
    if(temp != "" && isalpha(temp[0])) {
      program.push_back(temp);
    }
  }
}

// Executers
bool executeStatement (const string& statement, unsigned& lineno);
bool executeLine (const string& line, unsigned& lineno);

void executeAssign (const string& statement, unsigned& lineno) {
  sscanf(statement.c_str(), "%*s%s %[^\n]", buffer[0], buffer[1]);
  string symbol(buffer[0]), expression(buffer[1]);
  int value = parseExpression(expression);
  symbols[symbol] = Symbol(symbol, Symbol::Integer, value);
  cout << "Assigning " << value << " to " << symbol << endl;
  lineno++;
}

void executeDefine (const string& statement, unsigned& lineno) {
  sscanf(statement.c_str(), "%*s%s", buffer[0]);
  string symbol(buffer[0]);
  symbols.emplace(make_pair(symbol, Symbol(symbol, Symbol::Function, lineno)));
  cout << "Defining " << symbol << endl;
  while(!beginWith(program[lineno], "END")) {
    lineno++;
  }
  lineno++;
}

void executeCall (const string& statement, unsigned& lineno) {
  sscanf(statement.c_str(), "%*s%s", buffer[0]);
  string symbol(buffer[0]);
  cout << "Calling " << symbol << endl;
  unsigned sl = symbols[symbol].value + 1;
  while(executeLine(program[sl], sl))
  ;
  lineno++;
}

bool executeStatement (const string& statement, unsigned& lineno) {
  if(beginWith(statement, "ASSIGN")) {
    executeAssign(statement, lineno);
  } else if(beginWith(statement, "DEFINE")) {
    executeDefine(statement, lineno);
  } else if(beginWith(statement, "END")) {
    // Block end
    return false;
  } else if(beginWith(statement, "CALL")) {
    executeCall(statement, lineno);
  }
  // Block not finished
  return true;
}

bool executeLine (const string& line, unsigned& lineno) {
  if(!beginWith(line, "FOR")) {
    // If it doesn't begin with "FOR", it's a single statement
    return executeStatement(line, lineno);
  } else {
    int cursor = 0;
    sscanf(line.c_str(), "%*s %[^\n]", buffer[0]);
    string blendExpr(buffer[0]);
    int times = Parser::expr(blendExpr, cursor).resolve(symbols);
    unsigned dummy = 0;
    for(int i = 0; i < times; i++) {
      executeStatement(blendExpr.substr(cursor), dummy);
    }
    lineno++;
    return true;
  }
}

int main () {
  readProgram();
  unsigned lp = 0;
  while(lp < program.size()) {
    executeLine(program[lp], lp);
  }
}
