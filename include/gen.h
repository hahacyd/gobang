#include<iostream>
#include<algorithm>

#include"Square.h"
#include"Define.h"
using namespace std;
extern Square board[ROWS][COLS];
extern int ownColor, oppositeColor;

bool has_nor(int x, int y, int distance, int count);
bool sort_by_point_up(Point v1, Point v2);
bool sort_by_point_down(Point v1, Point v2);

extern int STEPS;
vector<Point> gen(int deep, int color);
bool print(vector<Point>& v);
bool sort_by_point_up(Point v1, Point v2);
bool sort_by_point_down(Point v1, Point v2);
bool has_nor(int x, int y, int distance, int count);