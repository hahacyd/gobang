#pragma once
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>
using namespace std;

#define SERVER_PORT 10087 // server port
#define PASSWORD "123456" // your password


//#define  ID          "111111112"        // your id
//#define  ID          "161150037"        // your id
//#define  ID          "161220001"
//#define  ID         "123456789"

//#define  SERVER_IP   "192.168.1.100"	    // server ip
//#define  SERVER_IP   "114.212.141.21"	    // server ip
//#define  SERVER_IP   "114.212.135.60"	    // server ip
//#define  SERVER_IP   "114.212.129.245"	    // server ip
#define SERVER_IP "127.0.0.1" // server ip

#define random(x) (rand() % x)
#define ROWS 15
#define COLS 15

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

#define DEEP 8
#define CHOOSE_LIMIT 10
#define DEFEND 1
#define TIME_LIMITED 5

#define EMPTY -1
#define SMART 1

class Score {
public:
    int five, four, thr, two, one;
    int de_five, de_four, de_thr, de_two, de_one;
    int count = 0;
    int appri = 0; //用于微调   比如隔空下子分数不给高等等
    bool state = 0;
    int pre_oppo_dou = 0;
    Score()
    {
        count = 0;
        five = 0, four = 0, thr = 0, two = 0, one = 0;
        de_five = 0, de_four = 0, de_thr = 0, de_two = 0, de_one = 0;
        appri = 0; //这个参数用来微调一些情况的数据；
        state = 0;
        pre_oppo_dou = 0;
    }
    int sum()
    {
        bool flag = 0;
        if (1 < de_four + thr + four) {
            flag = 1;
            de_four = 0;
            count += FOUR;
        }
        if (SMART && pre_oppo_dou > 1) {
            //pre_oppo_dou = 0;
            count += FOUR - 600;
            return count;
        }

        count += five * FIVE + four * FOUR + thr * THR + two * TWO + one * ONE
            + de_four * DE_FOUR + de_thr * DE_THR + de_two * DE_TWO + de_one * DE_ONE;
        if (pre_oppo_dou > 1) {
            return count;
        }
        return count + appri;
    }
    bool set_state(bool st)
    {
        state = st;
        return 1;
    }
    bool reset()
    {
        five = 0, four = 0, thr = 0, two = 0, one = 0;
        de_five = 0, de_four = 0, de_thr = 0, de_two = 0, de_one = 0;
        count = 0;
        appri = 0;
        pre_oppo_dou = 0;
        return 1;
    }
};

class Point {
public:
    int x, y, color;
    int point;
    Point()
    {
        x = -1;
        y = -1;
        point = 0;
    }
    Point(int x1, int y1)
    {
        x = x1;
        y = y1;
    }
    Point(int x1, int y1, int color1)
    {
        x = x1;
        y = y1;
        color = color1;
    }
    Point set(int x1, int y1)
    {
        x = x1;
        y = y1;
        return *this;
    }
    int get_color()
    {
        return color;
    }
    int get_empty()
    {
        return EMPTY == color;
    }
    bool operator==(const Point& b)
    {
        return (x == b.x && y == b.y);
    }
    bool operator!=(const Point& b)
    {
        return !((*this) == b);
    }
    Point operator=(const Point& b)
    {
        (*this).x = b.x;
        (*this).y = b.y;
        (*this).color = b.color;
        (*this).point = b.point;
        return (*this);
    }
};

class Log {
private:
    string filename;
    ofstream fout;
    ifstream fin;

public:
    bool init(int me, int op); //日志初始化，用于说明我方和敌方的棋子颜色，me表示我方，op表示敌方；
    bool open_out(string* file);
    bool open_in(string* file);
    bool close_out();
    bool close_in();
    bool write_hu_bot(int x, int y, int color);
    bool write_bot_bot(int x, int y, int color);
    Point read();
};
