#pragma once
#include <map>
#include <macros.h>
class Timer
{
public:
	Timer(byte *_IF) {
		IF = _IF;
	};
	byte* IF{ NULL };
	byte divider{0};
	int counter{ 0 };
	byte tma{ 0 };
	int clock{ 0 };
	int basic{ 0 };
	byte tima{ 0 };
	int divSub{ 0 };
	std::map<int, int> speed{ {0,64}, {1,1}, {2,4}, {3,16} };
	int speedUsing{ 0 };
	bool timeEnable{ true };
	void step(int);
};

