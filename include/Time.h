#include <iostream>
#include <string>
#include <time.h>
using namespace std;

class Time {
private:
    time_t rawtime;
    struct tm* timeinfo;

    clock_t start, finish;
    double duration;

public:
    Time()
    {
        start = clock();
    }
    string get_time_str()
    { //获得表示时间的字符串
        time(&rawtime);
        // localtime_s(timeinfo,&rawtime);
        string temp;
        char asc[50];
        // asctime_s(asc,timeinfo);
        temp = asc;
        //cout << temp << endl;
        int found = 0;
        found = temp.find(':', found);
        while (found != -1 && found < (int)temp.size() + 1) {
            temp.replace(found, 1, "-");
            found = temp.find(':', found + 1);
        }
        return temp;
    }

    bool reset()
    {
        start = clock();
        return 1;
    }
    double get_time()
    {
        finish = clock();
        duration = (double)(finish - start) / CLOCKS_PER_SEC;
        return duration;
    }
    double get_time(int i)
    {
        finish = clock();
        duration = (double)(finish - start) / CLOCKS_PER_SEC;
        cout << endl
             << "用时: " << duration << "秒" << endl
             << endl;
        return duration;
    }
};

// class UI {
// public:
// 	//对应的颜色码表：
// 	//1. 0 = 黑色       8 = 灰色
// 	//1 = 蓝色       9 = 淡蓝色
// 	//2 = 绿色       10 = 淡绿色
// 	//3 = 浅绿色     11 = 淡浅绿色
// 	//4 = 红色       12 = 淡红色
// 	//5 = 紫色       13 = 淡紫色
// 	//6 = 黄色       14 = 淡黄色
// 	//7 = 白色       15 = 亮白色
// 	//定义设置颜色的函数，方便我们使用
// 	//第一个参数是字体颜色，第二个参数是背景颜色
// 	void setColor(unsigned short ForeColor = 7, unsigned short BackGroundColor = 0)
// 	{
// 		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);//获取当前窗口句柄
// 		SetConsoleTextAttribute(handle, ForeColor + BackGroundColor * 0x10);//设置颜色
// 	}
// 	//定义设置光标位置的函数
// 	void setPos(int x, int y)
// 	{
// 		COORD pos;
// 		HANDLE handle;
// 		pos.X = x;
// 		pos.Y = y;
// 		handle = GetStdHandle(STD_OUTPUT_HANDLE);
// 		SetConsoleCursorPosition(handle, pos);
// 	}
// };
