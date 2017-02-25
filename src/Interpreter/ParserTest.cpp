#include <bits/stdc++.h>
#include "include/Parser.h"
using namespace std;

// 表达式解析测试

int main() {
  string s;
  cin >> s;
  int cursor = 0;
  Parser::expr(s, cursor).display();
  return 0;
}

//2333+666-23-12*5/6+(-(-(-2)-(-x)))/aLongVaribleName%aShortVaribleName
//-1+2*3
