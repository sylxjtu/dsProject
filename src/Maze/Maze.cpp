#include <bits/stdc++.h>
using namespace std;

// Union-Find 节点
class Node {

  // 父节点
  Node* parent;

  // 查询根节点
  Node* findparent () {
    if(this == parent) {
      return this;
    } else {
      return this->parent = this->parent->findparent();
    }
  }

public:
  // 初始化根节点为自己
  Node () {
    parent = this;
  }

  // 合并两个节点所在的集
  void merge (Node& that) {
    findparent()->parent = that.findparent();
  }

  // 检验两个节点是否在同一个集合中
  bool operator== (Node& that) {
    return findparent() == that.findparent();
  }
  bool operator!= (Node& that) {
    return !(*this == that);
  }

};

// 连接迷宫节点的边
class Edge {

  // 边的起点，终点
  Node *s, *e;

public:
  // 边所代表的障碍所在位置
  int x, y;

  // 初始化
  Edge (Node* s, Node *e, int x, int y):
  s(s), e(e), x(x), y(y){}

  // 判断一条边上两个节点是否连通，若不连通则合并节点并删除边
  bool destroy () {
    if(*s != *e) {
      s->merge(*e);
      return true;
    }
    return false;
  }

};

// 泛化二维数组
template < class T >
class Array2d {

  T* data;

public:

  // 二维数组的宽高(x, y)
  int width, height;

  // 二维数组构造析构
  Array2d (int width, int height) : width(width), height(height) {
    data = new T[width * height];
  }
  ~Array2d () {
    delete[] data;
  }

  // 操作二维数组(类似于array[y][x])
  T& operator () (int x, int y) {
    return data[y * width + x];
  }

  // 验证下标是否越界
  bool valid(int x, int y) {
    return 0 <= x && x < width && 0 <= y && y < height;
  }

  // 填充二维数组
  void clear(const T& value) {
    for(int i = 0; i < width * height; i++) {
      data[i] = value;
    }
  }

};

// 迷宫地图节点类型
enum dispType {
  WALL,     // 墙
  PASSABLE, // 空地
  WAY,      // 最短路上的节点
};

// 读取输入
void readInput (int& width, int& height) {
  cerr << "Enter width and height" << endl;
  cerr << "Width: ";
  cin >> width;
  cerr << "Height: ";
  cin >> height;
}

int main () {
  int width, height;
  // 输入并检查合法性(width > 0 && height > 0)
  readInput(width, height);
  try {
    if(!(width > 0 && height > 0)) throw("Width and height must be greater than 0");
  } catch(const char* s) {
    printf("Error: %s\n", s);
    return 0;
  }

  // 分配空间
  Array2d<Node> nodes(width, height);  // 迷宫中的节点
  Array2d<int> disp((width * 2) + 1, (height * 2) + 1);  // 最终显示出的地图
  vector<Edge> edges;  // 连接节点的边

  // 初始化地图
  disp.clear(dispType::WALL);
  for(int i = 0; i < height; i++) {
    for(int j = 0; j < width; j++) {
      disp(j * 2 + 1, i * 2 + 1) = dispType::PASSABLE;
    }
  }
  disp(0, 1) = disp(disp.width - 1, disp.height - 2) = dispType::WAY;

  for(int i = 0; i < height; i++) {
    for(int j = 0; j < width - 1; j++) {
      edges.emplace_back(&nodes(j, i), &nodes(j + 1, i), j * 2 + 2, i * 2 + 1);
    }
  }
  for(int i = 0; i < height - 1; i++) {
    for(int j = 0; j < width; j++) {
      edges.emplace_back(&nodes(j, i), &nodes(j, i + 1), j * 2 + 1, i * 2 + 2);
    }
  }

  // 随机拆除边使图最终成为树
  random_shuffle(edges.begin(), edges.end());
  for(auto& e: edges) {
    if(e.destroy()) {
      disp(e.x, e.y) = dispType::PASSABLE;
    }
  }

  // 通过BFS查找最短路径
  Array2d< pair<int, int> > parent(disp.width, disp.height);
  Array2d<bool> visited(disp.width, disp.height);
  visited.clear(0);
  queue<pair<int, int>> bfsq;
  bfsq.emplace(disp.width - 1, disp.height - 2);
  while(!bfsq.empty()) {
    pair<int, int> p = bfsq.front();
    bfsq.pop();
    if(p == make_pair(0, 1)) break;
    visited(p.first, p.second) = 1;
    for(int i = -1; i <= 1; i += 2) {
      if( parent.valid(p.first + i, p.second) && !visited(p.first + i, p.second) && disp(p.first + i, p.second) ) {
        parent(p.first + i, p.second) = make_pair(p.first, p.second);
        bfsq.emplace(p.first + i, p.second);
      }
    }
    for(int i = -1; i <= 1; i += 2) {
      if( parent.valid(p.first, p.second + i) && !visited(p.first, p.second + i) && disp(p.first, p.second + i) ) {
        parent(p.first, p.second + i) = make_pair(p.first, p.second);
        bfsq.emplace(p.first, p.second + i);
      }
    }
  }

  // 标记最短路径
  int curx = 0, cury = 1;
  string route;
  while(make_pair(curx, cury) != make_pair(disp.width - 1, disp.height - 2)){
    disp(curx, cury) = dispType::WAY;
    int nxtx, nxty;
    tie(nxtx, nxty) = parent(curx, cury);
    route.push_back("NW ES"[(nxtx - curx) + (nxty - cury) * 2 + 2]);
    tie(curx, cury) = tie(nxtx, nxty);
  }

  // 打印迷宫
  for(int i = 0; i < disp.height; i++) {
    for(int j = 0; j < disp.width; j++) {
      cout << (disp(j, i) ? "  " : "██");
    }
    cout << endl;
  }
  cout << "Press <Enter> to print route" << endl;

  // 等待用户确认后打印最短路
  getchar();
  getchar();
  for(int i = 0; i < disp.height; i++) {
    for(int j = 0; j < disp.width; j++) {
      cout << (
        disp(j, i) == dispType::PASSABLE ?
          "  " :
          (disp(j, i) == dispType::WAY) ?
            "\e[0;31m██\e[0;0m" :       // 红色
            "██"
      );
    }
    cout << endl;
  }
  cout << route << endl;

  return 0;
}
