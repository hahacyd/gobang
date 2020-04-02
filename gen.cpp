#include "gen.h"
#include "evaluate.h"
vector<Point> gen(int deep, int color)
{
    vector<Point> gen;
    //gen.clear();
    gen.clear();
    Point x;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (board[i][j].get_empty() == 0) {
                continue;
            }
            if (has_nor(i, j, 2, 1)) {
                gen.push_back(Point(i, j));
            }
        }
    }
    vector<Point>::iterator it = gen.begin();
    for (; it != gen.end(); it++) {
        it->point = tem_point_score(it->x, it->y, color);
    }
    sort(gen.begin(), gen.end(), sort_by_point_down);

    int length = gen.size();
    if (length > CHOOSE_LIMIT) {
        gen.erase(gen.begin() + CHOOSE_LIMIT, gen.end());
    }
    return gen;
}
bool print(vector<Point>& v)
{
    vector<Point>::iterator it = v.begin();
    for (; it != v.end(); it++) {
        cout << it->point << "  ";
    }
    return 1;
}
bool sort_by_point_up(Point v1, Point v2)
{
    return v1.point < v2.point;
}
bool sort_by_point_down(Point v1, Point v2)
{
    return v1.point > v2.point;
}
bool has_nor(int x, int y, int distance, int count)
{
    int startx = x - distance,
        endx = x + distance,
        starty = y - distance,
        endy = y + distance;
    for (int i = startx; i <= endx; i++) {
        if (i >= ROWS || i < 0)
            continue;
        for (int j = starty; j <= endy; j++) {
            if (j >= COLS || j < 0)
                continue;
            if (j == y && i == x)
                continue;
            if (board[i][j].get_empty() == 0) {
                count--;
                if (count < 1) {
                    return 1;
                }
            }
        }
    }
    return 0;
}