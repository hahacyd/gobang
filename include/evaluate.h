#include"Define.h"
enum mode {
	own, oppo,interpr,profit,
};
int evaluate(int color,int x = 0,int y = 0);
bool in_wall(int i, int j);
int cal_score(int count, bool act);
bool win(int color);
int tem_point_score(int x, int y, int color);
int get_score(int count, int empty, int block, Score &score,mode m);
int point_sum(list<Point>::iterator first, list<Point>::iterator last, list<Point>::iterator central, mode m, int color, Score &score);
int get_evaulate_point(vector<Point> ::iterator first, vector<Point>::iterator last, int color, Score &score);
