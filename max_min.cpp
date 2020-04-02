#include "max_min.h"
#include "Time.h"
#include "debug.h"
#include "evaluate.h"
#include "gen.h"
#include <iostream>
using namespace std;
extern Square board[ROWS][COLS];
extern int ownColor, oppositeColor;
extern vector<Point> opp_order;
extern vector<Point> own_order;
#define MAX 9999999
#define MIN -9999999
vector<Point> best_points;
#define random(x) (rand() % x)

extern Time time_get;

int ABcut, PVcut, total = 0, steps;
int own_cut = 0, opp_cut = 0;

Point min_max(int deep)
{
    if (0 == deep) {
        return Point(-1, -1);
    }
    best_points.clear();
    int best = -MAX * 10;
    ABcut = 0;
    PVcut = 0;
    if (deep <= 0) {
        return Point(-1, -1);
    }
    int alpha = MAX;
    int beta = -MAX;
    vector<Point> points = gen(deep, ownColor);

#ifdef DEBUG
    for (int i = 0; i < (int)points.size(); i++) { // && i < 6
        cout << "最优点 " << i << " :"
             << "( " << points[i].x << ", " << points[i].y << ")"
             << "   得分 ：" << points[i].point << endl;
    }
#endif
    int v = 0;
    for (vector<Point>::iterator it = points.begin(); it != points.end(); it++) {

        v = 0;
        Point p = *it;

        board[p.x][p.y].set_empty(0);
        board[p.x][p.y].color = ownColor;
        int v = min_opp(deep - 1, best, MIN);

        v += (evaluate(ownColor) + evaluate(oppositeColor)) / DEEP + it->point * 0.1; //用点搜的结果作为参考   + it->point / 2;
        board[p.x][p.y].reset();

        if (v < best) {
            continue;
        } else if (v > best + 200) {
            best = v;
            best_points.clear();

            best_points.push_back(p);
        } else if (abs(best - v) < 200) {
            best_points.push_back(p);
        }
        if (v > FIVE) { //如果能赢，直接退出
            break;
        }
    }
    if (best_points.size() == 0) {
        return Point(-1, -1);
    } else {
        int rent = random(best_points.size());
        return best_points[rent]; //在备选的方案中随机选择一种
    }
}
int max_own(int deep, int alpha, int beta)
{ //需要最大值alpha,   这个alpha来自传入的最小值，一旦有pv> alpha,就剪掉，

    int opp_score = evaluate(oppositeColor);
    int v = opp_score + evaluate(ownColor);

    if (deep <= 0 /* || opp_score > FIVE*/) {
        return v;
    }
    int best = MIN;
    vector<Point> points = gen(deep, ownColor);
    Point p;
    int pv = 0;
    for (vector<Point>::iterator it = points.begin(); it != points.end(); it++) {
        p = *it;
        pv = 0;
        board[p.x][p.y].color = ownColor;
        board[p.x][p.y].set_empty(0);

        pv = min_opp(deep - 1, MAX, best) + it->point * 0.1;

        board[p.x][p.y].reset();
        if (pv > best) {
            best = pv;
        }

        if (pv > alpha) { //如果pv 已经大于alpha了，那就直接退吧，
            own_cut++;
            break;
        }
    }
    best += v * (1 - deep / DEEP);

    return best;
}
int min_opp(int deep, int alpha, int beta)
{ //需要最小值beta                || win(oppositeColor
    int own_score = evaluate(ownColor);
    int v = evaluate(oppositeColor) + own_score;

    if (deep <= 0 /*|| own_score > FIVE*/) {
        return v;
    }
    int best = MAX;
    vector<Point> points = gen(deep, oppositeColor);
    Point p;
    int pv = 0;
    for (vector<Point>::iterator it = points.begin(); it != points.end(); it++) {
        p = *it;
        pv = 0;
        board[p.x][p.y].color = oppositeColor;
        board[p.x][p.y].set_empty(0);

        int pv = max_own(deep - 1, best, MIN) - it->point * 0.1;

        board[p.x][p.y].reset();

        if (best > pv) {
            best = pv;
        }
        if (pv < beta) {
            opp_cut++;
            break;
        }
    }
    best += v * (1 - deep / DEEP);
    return best;
}
