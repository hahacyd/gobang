#include <iostream>
#include <list>
#include <vector>
#include "Define.h"
#include "Square.h"
#include "evaluate.h"
using namespace std;

extern int ownColor, oppositeColor;
int cur_color = -1;
Score high, high_color, high_other;
list<Point> row_ana;
extern int STEPS;
extern Square board[ROWS][COLS];
/*
#define FIVE 100000
#define FOUR 5000
#define THR 2500
#define TWO 200
#define ONE 5

#define DOU_THR 100000
#define DE_FOUR 2500
#define DE_THR 200
#define DE_TWO 5
#define DE_ONE 1
*/
bool act = 0, last = 0;
bool in_wall(int i, int j);
int book[15][15] = { 0 };
int big_count = 0;
bool win(int color);
bool in_local(int sx, int ex, int sy, int ey, int x, int y);
int is_five(int count, int last); //这里做了修改，就是 活四也算进去 算成 成五，反正都是必胜的局么。
int get_evaulate_point(vector<Point>::iterator first, vector<Point>::iterator last, int color, Score& score);

int is_five(int count, int last)
{
    return ((1 == last) && (4 == count)) || (count == 5);
}
int evaluate(int color, int x, int y)
{
    big_count++;
    int point = 0;
    int count = 0;
    vector<Point> eva_row;

    //横着数
    Score score;
    for (int i = 0; i < ROWS; i++) {
        act = 0;
        last = 0;
        for (int j = 0; j < COLS; j++) {
            eva_row.push_back(Point(i, j, board[i][j].get_color())); //将一行棋子信息导入到eva_row以单独分析 这个容器的内容，
        }
        eva_row.push_back(Point(99, 99, 1 - color)); //再在最后一位填入对手棋子，方便后面考虑“活动空间”的事情，

        point += get_evaulate_point(eva_row.begin(), eva_row.end(), color, score);
        eva_row.clear();
    }
    count = 0;

    score.reset();
    for (int j = 0; j < COLS; j++) { //竖着数
        act = 0;
        last = 0;
        for (int i = 0; i < ROWS; i++) {
            eva_row.push_back(Point(i, j, board[i][j].get_color())); //将一行棋子信息导入到eva_row以单独分析 这个容器的内容，
        }
        eva_row.push_back(Point(99, 99, 1 - color)); //再在最后一位填入对手棋子，方便后面考虑“活动空间”的事情，

        point += get_evaulate_point(eva_row.begin(), eva_row.end(), color, score);

        eva_row.clear();
    }
    score.reset();

    for (int i = 0; i < ROWS; i++) { //左斜着数   [i + j][j]       左上 到 右下扫描 扫描上三角
        act = 0;
        last = 0;
        for (int j = 0; j < COLS; j++) {
            if (in_wall(i + j, j)) {
                eva_row.push_back(Point(i + j, j, board[i + j][j].get_color()));
            }
        }
        if (eva_row.size() > 4) {
            eva_row.push_back(Point(99, 99, 1 - color));

            point += get_evaulate_point(eva_row.begin(), eva_row.end(), color, score);
        }
        eva_row.clear();
    }
    score.reset();

    for (int j = 1; j < COLS; j++) { //左斜着数     [i][i + j]    左上 到 右下扫描 扫描上三角
        act = 0;
        last = 0;
        for (int i = 0; i < ROWS; i++) {
            if (in_wall(i, i + j)) {
                eva_row.push_back(Point(i, i + j, board[i][i + j].get_color()));
            }
        }
        if (eva_row.size() > 4) {
            eva_row.push_back(Point(99, 99, 1 - color));

            point += get_evaulate_point(eva_row.begin(), eva_row.end(), color, score);
        }
        eva_row.clear();
    }
    score.reset();

    for (int j = COLS - 1; j >= 0; j--) { //右斜   从上沿开始 [i][j - i]    左下右上  上三角
        act = 0;
        last = 0;
        for (int i = 0; i < ROWS; i++) {
            if (in_wall(i, j - i)) {
                eva_row.push_back(Point(i, j - i, board[i][j - i].get_color()));
            }
        }
        if (eva_row.size() > 4) {
            eva_row.push_back(Point(99, 99, 1 - color));

            point += get_evaulate_point(eva_row.begin(), eva_row.end(), color, score);
        }
        eva_row.clear();
    }
    score.reset();
    for (int i = 1; i < ROWS; i++) { //右斜线 沿右沿 向下 [i + COLS - j][j]   左下右上扫描  下三角
        act = 0;
        last = 0;
        for (int j = COLS - 1; j >= 0; j--) {
            if (in_wall(i + COLS - j, j)) {
                eva_row.push_back(Point(i + COLS - j, j, board[i + COLS - j][j].get_color()));
            }
        }
        if (eva_row.size() > 4) {
            eva_row.push_back(Point(99, 99, 1 - color));

            point += get_evaulate_point(eva_row.begin(), eva_row.end(), color, score);
        }
        eva_row.clear();
    }
    if (color == ownColor) {
        return point;
    } else {
        return -point;
    }
}

int cal_score(int count, bool act)
{
    int point = 0;
    if (count > 4) {
        high.five++;
        point += FIVE;
        return point;
    } else if (act) {
        switch (count) {
        case 0:
            point += 0;
        case 1:
            point += ONE;
            high.one++;
            break;
        case 2:
            point += TWO;
            high.two++;
            break;
        case 3:
            point += THR;
            high.thr++;
            break;
        case 4:
            point += FOUR;
            high.four++;
            break;
        case 5:
            point += FIVE;
            high.five++;
            break;
        case 6:
            point += FIVE;
            break;
        case 7:
            point += FIVE;
            break;
        case 8:
            point += FIVE;
            break;
        case 9:
            point += FIVE;
            break;

        default:
            cout << "横" << endl;
            break;
        }
    } else {
        switch (count) {
        case 0:
            point += 0;
        case 1:
            point += DE_ONE;
            high.de_one;
            break;
        case 2:
            point += DE_TWO;
            high.de_two;
            break;
        case 3:
            point += DE_THR;
            high.de_thr;
            break;
        case 4:
            point += DE_FOUR;
            high.de_four;
            break;
        case 5:
            point += FIVE;
            high.de_five;
            break;
        case 6:
            point += FIVE;
            break;
        case 7:
            point += FIVE;
            break;
        case 8:
            point += FIVE;
            break;
        case 9:
            point += FIVE;
            break;
        default:
            cout << "横？" << endl;
            break;
        }
    }
    return point;
}

bool in_wall(int i, int j)
{
    return i > -1 && i < ROWS && j < COLS && j >= 0;
}
bool in_local(int sx, int ex, int sy, int ey, int x, int y)
{
    return sx <= x && ex >= x && sy <= y && ey >= y;
}

bool win(int color)
{
    int point = 0;
    int aim = 0;
    int must_win = 0;
    for (int i = 0; i < ROWS; i++) { //横着数
        act = 0;
        last = 0;
        for (int j = 0; j < COLS; j++) {
            //if (EMPTY == board[i][j].get_empty()) {
            //	continue;
            //}
            if (board[i][j].get_empty() == 1 || !in_wall(i, j)) {
                act = 1;
                continue;
            }
            if (!(board[i][j].get_color() == color)) {
                act = 0;
                continue;
            }
            for (; in_wall(i, j) && board[i][j].get_empty() == 0 && board[i][j].get_color() == color && j < COLS; aim++, j++)
                ;
            if (in_wall(i, j) && act == 1 && board[i][j].get_empty() == 1) {
                last = 1;
            } else {
                last = 0;
            }
            must_win += is_five(aim, last);
            //point += score(count, last);
            aim = 0;
            j--; //上面的for循环多加了一个j
        }
    }
    aim = 0;
    for (int j = 0; j < COLS; j++) { //竖着数
        act = 0;
        last = 0;

        for (int i = 0; i < ROWS; i++) {
            //if (EMPTY == board[i][j].get_empty()) {
            //	continue;
            //}
            if (board[i][j].get_empty() == 1 || !in_wall(i, j)) {
                act = 1;
                continue;
            }
            if (!(board[i][j].get_color() == color)) {
                act = 0;
                continue;
            }
            for (; in_wall(i, j) && board[i][j].get_empty() == 0 && board[i][j].get_color() == color && i < ROWS; aim++, i++)
                ;
            if (in_wall(i, j) && act == 1 && board[i][j].get_empty() == 1) {
                last = 1;
            } else {
                last = 0;
            }

            must_win += is_five(aim, last);

            //point += score(count, last);
            aim = 0;
            i--; //上面的for循环多加了一个j
        }
    }
    for (int i = 0; i < ROWS; i++) { //左斜着数   [i + j][j]
        act = 0;
        last = 0;
        for (int j = 0; j < COLS; j++) {
            //if (EMPTY == board[i][j].get_empty()) {
            //	continue;
            //}
            if (board[i + j][j].get_empty() == 1 || !in_wall(i + j, j)) {
                act = 1;
                continue;
            }
            if (!(board[i + j][j].get_color() == color)) {
                act = 0;
                continue;
            }
            for (; in_wall(i + j, j) && board[i + j][j].get_empty() == 0 && board[i + j][j].get_color() == color && j < COLS; aim++, j++)
                ;
            if (in_wall(i + j, j) && act == 1 && board[i + j][j].get_empty() == 1) {
                last = 1;
            } else {
                last = 0;
            }

            must_win += is_five(aim, last);

            //point += score(count, last);
            aim = 0;
            j--; //上面的for循环多加了一个j
        }
    }
    for (int j = 1; j < COLS; j++) {
        act = 0;
        last = 0;
        for (int i = 0; i < ROWS; i++) {
            //if (EMPTY == board[i][j].get_empty()) {
            //	continue;
            //}
            if (board[i][i + j].get_empty() == 1 || !(in_wall(i, i + j))) {
                act = 1;
                continue;
            }
            if (!(board[i][i + j].get_color() == color)) {
                act = 0;
                continue;
            }
            for (; in_wall(i, i + j) && board[i][i + j].get_empty() == 0 && board[i][i + j].get_color() == color; aim++, i++)
                ;
            if (in_wall(i, i + j) == 1 && act == 1 && board[i][i + j].get_empty()) {
                last = 1;
            } else {
                last = 0;
            }

            must_win += is_five(aim, last);

            //point += score(count, last);
            aim = 0;
            i--; //上面的for循环多加了一个j
        }
    }
    /**/
    for (int j = COLS - 1; j >= 0; j--) { //右斜   从上沿开始 [i][j - i]
        act = 0;
        last = 0;
        for (int i = 0; i < ROWS; i++) {
            //if (EMPTY == board[i][j].get_empty()) {
            //	continue;
            //}
            if (!in_wall(i, j - i)) {
                continue;
            }
            if (board[i][j - i].get_empty() == 1) {
                act = 1;
                continue;
            }
            if (!(board[i][j - i].get_color() == color)) {
                act = 0;
                continue;
            }
            for (; in_wall(i, j - i) && board[i][j - i].get_empty() == 0 && board[i][j - i].get_color() == color && i < ROWS; aim++, i++)
                ;
            if (in_wall(i, j - i) && act == 1 && board[i][j - i].get_empty() == 1) {
                last = 1;
            } else {
                last = 0;
            }

            must_win += is_five(aim, last);

            //point += score(count, last);
            aim = 0;
            i--; //上面的for循环多加了一个j
        }
    }
    /**/
    for (int i = 1; i < ROWS; i++) { //右斜线 沿右沿 向下 [i + COLS - j][j]
        act = 0;
        last = 0;
        for (int j = COLS - 1; j >= 0; j--) {
            //if (EMPTY == board[i][j].get_empty()) {
            //	continue;
            //}
            if (!(in_wall(i + COLS - j, j)) == 1 || board[i + COLS - j][j].get_empty()) {
                act = 1;
                continue;
            }
            if (!(board[i + COLS - j][j].get_color() == color)) {
                act = 0;
                continue;
            }
            for (; in_wall(i + COLS - j, j) && board[i + COLS - j][j].get_empty() == 0 && board[i + COLS - j][j].get_color() == color && j < COLS; aim++, j--)
                ;
            if (in_wall(i + COLS - j, j) && act == 1 && board[i + COLS - j][j].get_empty() == 1) {
                last = 1;
            } else {
                last = 0;
            }

            must_win += is_five(aim, last);

            //point += score(count, last);
            aim = 0;
            j--; //上面的for循环多加了一个j
        }
    } /**/
    return (must_win > 0);
}

//此函数运行一次只分析一行中棋子的分数，
/*参数说明：
first：一行待分析棋子的首位置
last:   尾位置
central:   启发式搜索是要得出一个点的评分，这个参数说明此点在这一行中的位置
m:    模式：表示当前是要考虑 自己的连子 还是 阻击对方的连子，其实用两个函数分别表示这俩功能更恰当
color:   为哪一方分析
score:    判断形势；
X_OO			flag_right1
OX_O            flag_right2
O_XO            flag_left1
OO_X			flag_left2
*/
int point_sum(list<Point>::iterator first, list<Point>::iterator last, list<Point>::iterator central, mode m, int color, Score& score)
{ //总结分数，返回特定模式的int型分数

    int point = 0, test1 = 0;

    list<Point>::iterator cen = central;

    list<Point>::iterator it = first;
    for (; it != last; it++, test1++)
        ; //如果边界小于五就退出，用  test1  来表示
    if (test1 < 5) {
        return 0;
    }
    int left = 0, right = 0;
    if (own == m) { //计算我的连子
        int block = 0, empty = 0, empty2 = 0, count = 0, test2 = 0; //  这里count初始值为零， 待评分的点 会在下面自动加上的，
            //以下是计算“生存空间”，如果小于5，那显然是没有潜力的    先向左计算
        it = central;
        if (central != first) { //防止中间点就是起点情况的发生
            do {
                it--;
                test2++;
            } while (it != first && (it->get_color() == color || it->get_color() == EMPTY));
        }

        it = central;
        for (; it != last && (it->get_color() == color || it->get_color() == EMPTY); it++, test2++)
            ;
        if (test2 < 6) { //test2 就可以表示生存空间，若小于6，那后面就不用再考虑了，
            return 0;
        }

        it = central;
        //   向左***************************************************
        for (; it != first; it--) {
            if (it->get_color() != color && !it->get_empty()) { //敌方棋子的情况
                if ((++it)->get_color() == color) {
                    block++;
                }
                it--;
                break;
            } //出界或者是敌方棋子，阻碍加1，并退出循环，
            else if (it->get_color() == color) { //是自己棋子，当然计数加一，

                count++;
            }
            //接下来考虑空白的情况
            else {
                if (1 == empty) { //如果已经有一个空点了，那就退出不再考虑了，
                    break;
                } else {
                    if ((--it)->get_color() == color) { //如果隔这个空点是自己的点，  那这是真正 的 “空点” ，表示加载我连子中的空点
                        empty2 = 1;
                    }
                    empty = 1;
                    it++;
                }
            }
        }
        //横着计算 向右 ************************************************************
        cen = central;
        empty = 0;
        for (it = cen; it != last; it++) {
            if (it->get_color() != color && !it->get_empty()) { //敌方棋子的情况
                if ((--it)->get_color() == color) {
                    block++;
                }
                it++;
                break;
            } else if (it->get_color() == color) {
                count++;
            } else {
                if (1 == empty || 1 == empty2) {
                    break;
                } else {
                    empty = 1;
                    if ((++it)->get_color() == color) {
                        empty2 = 1;
                    }
                    it--;
                }
            }
        }
        count--; //因为向左向右都把待测点加上了，所以要减去一个，
        count = count < 0 ? 0 : count;
        get_score(count, empty2, block, score, m);
        return point;
    }
    //考虑阻击敌方棋子的情况    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>就是说我下这个点 能在多大程度上妨碍对手；哈哈哈啊
    else {
        int block = 0, empty = 0, empty2 = 0, count = 1, test2 = 1; //test2 = 1 是因为要加自己
        it = central;
        if (central != first) { //考察对手的生存空间，
            do {
                it--;
                test2++;
            } while (it != first && it->color != color);
        }

        cen = central;
        if (cen != last) {
            it = ++cen;
            for (; it != last && it->color != color; it++, test2++)
                ;
        }

        if (test2 < 5) { //如果没必要 没危险就不用防了；
            return 0;
        }

        //向左           *************************************************************
        int co2_flag_right2 = 0; //为 flag_right2 服务
        int co1_flag_right1 = 0;
        cen = central;
        bool flag_left1 = 0, flag_left2 = 0, flag_right1 = 0, flag_right2 = 0;
        if (cen != first) { //防止出现中间点cen又为右边界点的情况，否则会有“越界”的危险
            it = --cen;

            if (it != first && it->get_empty() == 1) { //例外考虑，空心三的情况
                --it;
                if (it != first) {
                    if (it->get_color() != color && it->get_empty() != 1) { //空心前已经有一个对方棋子了，进入
                        --it;
                        if (it != first && it->get_color() != color) {
                            flag_left1 = 1;
                        }
                        if (it != first) {
                            if (it->get_color() != color && it->get_empty() != 1) {
                                --it;
                                if (it != first && it->get_color() != color) {
                                    flag_left2 = 1;
                                    flag_left1 = 0; //到这里不需要用flag_left1 来解决了
                                }
                                it++;
                            }
                        }
                        it++;
                    }
                }
                it++;
            }

            if (!it->get_empty()) { // ****   贴近分析 如果左边是空的，对妨碍对手有卵用

                for (; it != first; it--) {
                    if (it->get_color() == color) { //意外遇见了自己的棋，没必要再往下走喽？
                        list<Point>::iterator temp = ++it;
                        if (temp->get_color() != color && !temp->get_empty()) {
                            block++;
                            co2_flag_right2 = 0;
                            //flag_left = 0;
                        }
                        it--;
                        break;
                    } else if (it->get_color() != color && it->get_empty() != 1) { //是敌方棋子，当然计数加一，
                        co2_flag_right2++;
                        count++;
                    }
                    //接下来考虑空白的情况
                    else {
                        if (1 == empty) {
                            empty = 0;
                            break;
                        } else {
                            list<Point>::iterator temp = --it;

                            if (temp->get_color() != color && temp->get_empty() != 1) {
                                empty2 = 1;
                            }
                            empty = 1;
                            it++;
                        }
                    }
                }
            } else {
                co1_flag_right1 = 1;
            }
        }

        //横着计算							*******************   向右   ****************************************************************
        cen = central;
        empty = 0;
        int co1_flag_left1 = 0; //为 flag_left1 服务 计算右边对手的子
        bool flag_right = 0;
        if (cen != last) { //防止出现中间点cen又为右边界点的情况，否则会有“越界”的危险
            it = ++cen;
            if (it != last && it->get_color() == color) {
                flag_left2 = 0;
                flag_left1 = 0;
            }
            if (it != last && it->get_empty() == 1) {
                it++;
                if (it != last && it->get_color() != color && it->get_empty() != 1) {
                    it++;
                    if (1 == co2_flag_right2 && it != last && it->get_color() != color) {
                        flag_right2++;
                    }
                    if (it != last && it->get_color() != color && it->get_empty() != 1) {
                        it++;
                        if (1 == co1_flag_right1 && it != last && it->get_color() != color) {
                            flag_right1++;
                        }
                        it--;
                    }

                    it--;
                }

                it--;
            }
            if (!it->get_empty()) { //如果我下到这个点，但我的右边没有对方子，并没有用
                for (it = cen; it != last; it++) {
                    if (it->get_color() == color) {
                        list<Point>::iterator temp = --it;
                        if (temp->get_color() != color && !temp->get_empty()) {
                            if (co1_flag_left1 + flag_left1 < 3) {

                                flag_left1 = 0;
                            }
                            block++;
                        }
                        it++;
                        break;
                    } else if (it->get_color() != color && !it->get_empty()) {
                        //it++;
                        //if(it != last && it->get_color() != color)
                        count++;
                        co1_flag_left1++;
                    } else {
                        if (1 == empty || 1 == empty2) {
                            break;
                        } else {
                            empty = 1;
                            if ((++it)->get_color() != color && !it->get_empty()) {

                                empty2 = 1;
                            }
                            it--;
                        }
                    }
                }
            }
        }
        if (flag_left1 + flag_left2 + flag_right1 + flag_right2 > 0) {
            score.pre_oppo_dou += flag_left1 + flag_left2 + flag_right1 + flag_right2;
        }
        get_score(count, empty2, block, score, m);
        return point;
    }
}

int tem_point_score(int x, int y, int color)
{
    //return 0;

    big_count++;

    int point = 0;
    Score score, score_over_oppo; //前者score表示我方棋子情况，后者表示阻击敌方棋子情况

    board[x][y].set_empty(0);
    board[x][y].color = color;

    int startx = x - 6, endx = x + 6, starty = y - 6, endy = y + 6;
    while (startx < 0) {
        startx++;
    }
    while (endx >= ROWS) {
        endx--;
    }
    while (starty < 0) {
        starty++;
    }
    while (endy >= COLS) {
        endy--;
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int j = y - 1;

    //横着计算 向左
    int row_count = 0, sum_count = 0;
    row_ana.clear();
    row_ana.push_back(Point(x, y, board[x][y].get_color()));

    for (int temp = 6; temp >= 0; temp--, j--) {

        if (in_local(startx, endx, starty, endy, x, j)) {
            //sum_count++;
            row_count++;
            row_ana.push_front(Point(x, j, board[x][j].get_color()));
        }
    }

    //横着计算 向右
    j = y + 1;

    for (int temp = 6; temp >= 0; temp--, j++) {

        if (in_local(startx, endx, starty, endy, x, j)) {
            //sum_count++;

            row_ana.push_back(Point(x, j, board[x][j].get_color()));
        }
    }
    list<Point>::iterator cen = row_ana.begin();

    for (int i = 0; i < row_count; i++, cen++)
        ;

    list<Point>::iterator temp = row_ana.end();
    temp--;
    point_sum(row_ana.begin(), temp, cen, own, color, score);
    point_sum(row_ana.begin(), temp, cen, oppo, color, score_over_oppo);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    row_ana.clear();
    sum_count = 0;

    row_count = 0;
    row_ana.push_back(Point(x, y, board[x][y].get_color()));

    int i = x - 1;
    //竖着计算 向上
    for (int temp = 6; temp >= 0; temp--, i--) {
        if (in_local(startx, endx, starty, endy, i, y)) {
            //sum_count++;

            row_count++;
            row_ana.push_front(Point(i, y, board[i][y].get_color()));
        }
    }
    //竖着计算 向下
    i = x + 1;
    for (int temp = 6; temp >= 0; temp--, i++) {

        if (in_local(startx, endx, starty, endy, i, y)) {
            //sum_count++;

            row_ana.push_back(Point(i, y, board[i][y].get_color()));
        }
    }
    cen = row_ana.begin();

    for (int i = 0; i < row_count; i++, cen++)
        ;

    temp = row_ana.end();
    temp--;
    point_sum(row_ana.begin(), temp, cen, own, color, score);
    point_sum(row_ana.begin(), temp, cen, oppo, color, score_over_oppo);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //左上右下 向左上
    row_count = 0;
    sum_count = 0;

    row_ana.clear();
    row_ana.push_back(Point(x, y, board[x][y].get_color()));

    i = x - 1, j = y - 1;
    for (int temp = 6; temp >= 0; temp--, i--, j--) {
        if (in_local(startx, endx, starty, endy, i, j)) {
            //sum_count++;

            row_count++;
            row_ana.push_front(Point(i, j, board[i][j].get_color()));
        }
    }
    //向右下
    i = x + 1, j = y + 1;
    for (int temp = 6; temp >= 0; temp--, i++, j++) {

        if (in_local(startx, endx, starty, endy, i, j)) {
            //sum_count++;

            row_ana.push_back(Point(i, j, board[i][j].get_color()));
        }
    }
    cen = row_ana.begin();

    for (int i = 0; i < row_count; i++, cen++)
        ;
    temp = row_ana.end();
    temp--;
    point_sum(row_ana.begin(), temp, cen, own, color, score);
    point_sum(row_ana.begin(), temp, cen, oppo, color, score_over_oppo);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    //左下右上
    //向左下
    row_count = 0;
    sum_count = 0;
    row_ana.clear();
    row_ana.push_back(Point(x, y, board[x][y].get_color()));

    i = x + 1, j = y - 1;
    for (int temp = 6; temp >= 0; temp--, i++, j--) {

        if (in_local(startx, endx, starty, endy, i, j)) {
            //sum_count++;

            row_count++;
            row_ana.push_front(Point(i, j, board[i][j].get_color()));
        }
    }
    i = x - 1, j = y + 1;
    // 向右上
    for (int temp = 6; temp >= 0; temp--, i--, j++) {

        if (in_local(startx, endx, starty, endy, i, j)) {
            //sum_count++;

            row_ana.push_back(Point(i, j, board[i][j].get_color()));
        }
    }
    cen = row_ana.begin();
    for (int i = 0; i < row_count; i++, cen++)
        ;

    temp = row_ana.end();
    temp--;
    point_sum(row_ana.begin(), temp, cen, own, color, score);
    point_sum(row_ana.begin(), temp, cen, oppo, color, score_over_oppo);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    board[x][y].reset();

    point = score.sum() + score_over_oppo.sum();
    return point;
}

#define FIVE 100000
#define FOUR 5000
#define THR 2500
#define TWO 200
#define ONE 5

#define DOU_THR 100000
#define DE_FOUR 2500
#define DE_THR 200
#define DE_TWO 5
#define DE_ONE 1

/*
此函数：只考虑有无block的情况来判断活棋还是死棋，再根据棋子个数最后算 分，
*/
int get_score(int count, int empty, int block, Score& score, mode m)
{
    int point = 0;
    if (oppo == m) { //如果是阻击，把分数计算状态变为一，因为对方比我慢一步；
        score.set_state(true);
    }
    if (count > 5) {
        score.five++;
        return 0;
    }
    if (count > 4 && 0 == empty) {
        score.five++;
        return 0;
        //return FIVE;
    } else if (own == m) {
        if (0 == block) {
            switch (count) {
            case 0:
                break;
            case 1:
                score.one++;
                break;
            case 2:
                point += TWO, score.two++;
                break;
            case 3: {
                point += THR;
                score.thr++;
                if (empty > 0) {
                    score.appri -= 1000;
                }
            } break;
            case 4: {
                point += FOUR;
                score.four++;
                if (empty) {
                    score.appri -= 3900;
                }
            } break;
            case 5: {
                point += FIVE, score.five++;
                if (empty > 0) {
                    score.appri -= 97800;
                }
            } break;
            default:
                cout << " 1  count ==" << count << endl;
                break;
            }
        } else {
            switch (count) {
            case 0:
                point += 0;
                break;
            case 1:
                point += DE_ONE, score.de_one;
                break;
            case 2:
                point += DE_TWO, score.de_two;
                break;
            case 3: {
                point += DE_THR;
                score.de_thr++;
                if (empty > 0) {
                    score.appri -= 30;
                }
            } break;
            case 4: {
                point += DE_FOUR;
                score.de_four++;

                if (empty) {
                    score.appri -= 900;
                }
            } break;
            case 5: {
                point += FIVE, score.de_five++;
                if (empty > 0) {
                    score.appri -= 97400;
                }
            } break;
            default:
                cout << "2  count ==" << count << endl;
                break;
            }
        }
    } else if (oppo == m) { //围堵对手的情况，
        if (0 == block) { //对方是活棋，
            switch (count) {
            case 0:
                break;
            case 1:
                score.one++;
                break;
            case 2:
                point += TWO, score.two++;
                break;
            case 3: {
                point += THR;
                score.pre_oppo_dou++;
            }; break;
            case 4: {
                point += FOUR;
                score.four++;
                if (oppo == m) {
                    score.pre_oppo_dou++;
                    if (empty > 0) {
                        score.appri -= 5000;
                    }
                }
            }; break;
            case 5: {
                if (oppo == m) {
                    score.pre_oppo_dou++;
                }
            }; break;
            default:
                cout << "oppo block == 0" << endl;
            }
        } else { //对方是死棋
            switch (count) {
            case 0:
                point += 0;
                break;
            case 1:
                point += DE_ONE, score.de_one;
                break;
            case 2:
                point += DE_TWO, score.de_two;
                break;
            case 3:
                break;
            case 4: {
                score.pre_oppo_dou++;
            } break;
            case 5: {
                score.pre_oppo_dou++;
            }
            }
        }
    }
    return point;
}
int get_evaulate_point(vector<Point>::iterator first, vector<Point>::iterator last, int color, Score& score)
{
    //从前向后遍历每一个棋子
    int block = 0, count = 0, empty = 0, empty2 = 0, point = 0;
    bool flag = 0;
    //if (first->get_empty() != EMPTY) {
    //	block = 1;
    //}
    vector<Point>::iterator it = first, temp = first;
    for (; it != last; it++) {

        if (it->get_color() == color) { //这个if 应该能保证我碰到第一个我的子才过来，
            if (first == it) {
                block++;
            } else {
                temp = --it;
                if (temp->get_empty() != 1 && temp->get_color() != color) {
                    block++;
                }
                it++;
            }
            //开始正式分析了    直接用了点评分那的函数，真爽，不然再想一遍真挺难想的；
            for (; it != last; it++) {

                if (it->get_color() != color && !it->get_empty()) { //敌方棋子的情况
                    if ((--it)->get_color() == color) {
                        if (block > 0 && empty + count < 5) {
                            count = 0;
                            break;
                        }
                        block++;
                    }
                    it++;
                    break;
                }

                else if (it->get_color() == color) {
                    count++;
                }

                else {
                    if (1 == empty) {
                        break;
                    } else {
                        empty = 1;
                        if (it != last) {
                            temp = ++it;
                            if (temp != last && temp->get_color() == color) {
                                empty2 = 1;
                            }
                            it--;
                        }
                    }
                }
            }
            /*if*/
            //现在开始总结分数；
            get_score(count, empty2, block, score, own);
            point += score.sum();
            score.reset();
            count = 0;
            empty2 = 0;
            block = 0;
        }
        if (it == last) {
            break;
        }
    }
    return point;
}
