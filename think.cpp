#include <iostream>

#include "Square.h"
#include "max_min.h"
#include "think.h"

using namespace std;

extern Square board[ROWS][COLS];
extern int ownColor, oppositeColor;
extern int own_cut, opp_cut, big_count;
vector<Point> opp_order;
vector<Point> own_order;

int STEPS = 0;
Time get_time;

Point think()
{
    get_time.reset();
    opp_cut = 0, own_cut = 0;
    int r = -1, c = -1;
    // printf("%s\n", lastMsg());
    Point me;
    big_count = 0;

    if (STEPS < 2) {
        r = -1, c = -1;
        while (!(r < 8 && r >= 6 && c >= 6 && c < 8 && board[r][c].get_empty() == 1)) {
            r = 6 + random(3);
            c = 6 + random(4);
        }
        me.set(r, c);
    } else {
        me = min_max(DEEP); //STEPS > 9?DEEP:(STEPS > 6?3:2)
    }
    get_time.get_time(1);
    STEPS++;
    return me;
}

bool printf()
{
    //system("cls");
    cout << "   ";
    for (int i = 0; i < COLS; i++) {
        cout << hex << " " << i;
    }
    cout << endl;
    for (int i = 0; i < ROWS; i++) {
        cout << hex << i << "  ";
        for (int j = 0; j < COLS; j++) {
            if (!board[i][j].get_empty()) {
                if (board[i][j].get_color() == ownColor) {
                    cout << " X";
                } else {
                    cout << " O";
                }
            } else {
                cout << " -";
            }
        }
        cout << dec << endl;
    }
    return 1;
}
