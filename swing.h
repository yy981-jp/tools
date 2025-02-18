class swing {
public:
	swing(int lower_bound, int upper_bound) : lower_bound(lower_bound), upper_bound(upper_bound) {}
	int operator()() {
		if (up) {
			if (value < upper_bound)
				return ++value;
			else {
				up = false;
				return --value;
			}
		} else {
			if (value > lower_bound)
				return --value;
			else {
				up = true;
				return ++value;
			}
		}
	}
	swing& operator=(int v) {
		value = v;
		return *this;
	}
	int operator+() const { return value; } // 以下のように値を抽出する i(int)+s(swing)
private:
	int value = 0;
	int lower_bound, upper_bound;
	bool up = true;
};