#include "face.h"
#include "debug.h"
Log log_one;
Time temp;
vector<Point> step_record;
Square board[ROWS][COLS];
int ownColor = -1, oppositeColor = -1;
bool gamestart()
{
    int i, j;
    char x = 'a', y = 'b';
    oppositeColor = 0;
    ownColor = 1;

    state cur = normal;
    string filename;
    // switch (getc()) {
    // case 'r': {
    //     cin >> filename;
    //     cur = read(&filename);
    //     break;
    // }
    // default:
    //     break;
    // }
    // if (end_of == cur) {
    //     log_one.open_out(&filename);
    // } else {
    //     log_one.open_out(&string("cd"));
    //     log_one.init(ownColor, oppositeColor);
    // }
    int n = 1;
    if (1 == n) {
        printf();
        while (1) {

            cout << "你要下在 : ";
            cin >> x >> y;

            if ('q' == x && 'q' == y) {
                break;
            }
            if ('u' == x && 'u' == y && step_record.size() > 1) { //用于悔棋
                Point a = step_record.back();
                step_record.pop_back();
                board[a.x][a.y].reset();

                a = step_record.back();
                step_record.pop_back();
                board[a.x][a.y].reset();

                printf();
                STEPS--;
                getc(stdin);
                continue;
            }
            i = trans(x);
            j = trans(y);

            while (!in_wall(i, j) || board[i][j].get_empty() == 0) {
                //cout << "哦？ 你不想玩了？要认输？" << endl;
                cout << "你要下在 : ";
                cin >> i >> j;
            }
            board[i][j].set_empty(0);
            board[i][j].color = oppositeColor;

            log_one.write_hu_bot(i, j, oppositeColor);

            step_record.push_back(Point(i, j, ownColor));
            if (evaluate(oppositeColor) > FIVE) { //判对方是否赢了
                cout << "CONGRAT  你赢了" << endl;
                break;
            }

            Point me = think();
            if (-1 == me.x && -1 == me.y) {
                printf();
                cout << "这棋不好走啊，我跳一步，你再来？" << endl;
            } else {

                board[me.x][me.y].set_empty(0);
                board[me.x][me.y].color = ownColor;
                //system("cls");
                log_one.write_hu_bot(me.x, me.y, ownColor);

                step_record.push_back(Point(me.x, me.y, oppositeColor)); //把程序的下子位置记录下来

                printf();
                cout << "我下子: ( " << me.x << "," << me.y << " )" << endl;

                if (evaluate(ownColor) > FIVE) { //判我方是否赢了
                    cout << "haha  我赢了" << endl;
                    break;
                }
            }
        }
    }
    log_one.close_in();
    if (-1 == i && -1 == j) {
        return 0;
    }
    return 1;
}
bool test_evaulate()
{
    int i, j;
    printf();
    while (cin >> i >> j) {
        system("cls");
        board[i][j].color = ownColor;
        board[i][j].set_empty(0);
        printf();
#ifdef DEBUG
        cout << "目前得分: " << evaluate(ownColor) << endl;
#endif
    }
    return 1;
}
state read(string* filename)
{ //读入棋子的记录

    if (*filename == "q") {
        return quit;
    }
    log_one.open_in(filename);
    printf();
    while ('q' != getc(stdin)) {
        Point a = log_one.read();
        if (-1 == a.x && -1 == a.y) {
            return end_of;
        }
        if (ownColor == a.color) {
            STEPS++;
        }
        board[a.x][a.y].color = a.color;
        board[a.x][a.y].set_empty(0);

        step_record.push_back(Point(a.x, a.y, a.color));
        system("cls");
        printf();
    }
    return change;
}
int trans(char a)
{
    int i = 0;
    a = tolower(a);
    if (isalpha(a)) {
        i = a - 'a' + 10;
    } else {
        i = a - '0';
    }
    return i;
}
