#include "Define.h"
#include <iostream>
bool Log::write_bot_bot(int x, int y, int color)
{
    if (!fout) {
        return 0;
    }
    return 1;
}
bool Log::write_hu_bot(int x, int y, int color)
{
    if (!fout) {
        return 0;
    }
    fout << endl
         << x << " " << y << " " << color;
    return 1;
}
bool Log::close_in()
{
    if (!fin) {
        return 1;
    }
    fin.close();
    return 1;
}
bool Log::close_out()
{
    if (!fout) {
        return 1;
    }
    fout.close();
    return 1;
}
bool Log::open_in(string* file)
{
    fin.open(file->c_str(), ios::in);
    if (!fin) {
        cout << "打开文件失败！" << endl;
        return 0;
    }
    string temp;
    getline(fin, temp);
    getline(fin, temp);
    filename = *file;
    return 1;
}
bool Log::open_out(string* file)
{
    cout << "open_out:" << *file << endl;
    fout.open(file->c_str(), ios::out);
    if (!fout) {
        cout << "新建文件失败！" << endl;
        return 0;
    }
    filename = *file;
    return 1;
}
bool Log::init(int me, int op)
{
    if (!fout) {
        cout << "尚未调用 \"open_out\"函数" << endl;
        return 0;
    }
    fout << me << endl
         << op;
    return 1;
}
Point Log::read()
{
    if (!fin) {
        cout << "尚未调用 \"open_in\"函数" << endl;
        return Point(-1, -1);
    }
    if (fin.eof()) {
        return Point(-1, -1);
    }
    int x, y, color;

    fin >> x >> y >> color;
    if (x < 0 || x > ROWS || y < 0 || y > COLS) {
        return Point(-1, -1);
    }
    return Point(x, y, color);
}
