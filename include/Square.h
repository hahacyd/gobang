#define ROWS 15
#define COLS 15
class Square {
public:
	bool empty;
	int color;
    Square(): empty(true), color(-1){}
	void reset();
	int get_color() const{
		return color;
	}
	bool get_empty()const {
		return empty;
	}
	bool set_empty(bool i) {
		empty = i;
		return 1;
	}
};
