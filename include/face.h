#include <iostream>
// #include<conio.h>
#include <vector>

#include "Square.h"
#include "evaluate.h"
#include "max_min.h"
#include "think.h"
using namespace std;
extern int oppositeColor, ownColor;
int trans(char a);

extern int STEPS;
enum state {
    change,
    quit,
    normal,
    end_of,
};
state read(string* filename);
bool gamestart();
bool test_evaulate();
state read(string* filename);
int trans(char a);