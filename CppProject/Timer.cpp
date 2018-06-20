#include "Timer.h"


void Timer::step(int deltaTime)
{
	clock += deltaTime;
	if (clock >= 16) {
		clock -= 16;
		basic++;
		divSub++;
	}
	if (divSub >= 16) {
		divider++;
		divSub -= 16;
	}
	if (basic >= speed[speedUsing]) {
		basic -= speed[speedUsing];
		tima++;
		if (tima == 0) {
			tima = tma;
			*IF = (*IF) | 0b100;
		}
	}
}
