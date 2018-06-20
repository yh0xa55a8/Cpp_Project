#include "SimThread.h"


SimThread::~SimThread()
{
	requestInterruption();
	quit();
	wait();
}

void SimThread::run()
{
	simCPU.init(simSto);
	simGPU.init(simSto);
	while (!isInterruptionRequested()) {
		int deltaTime = simCPU.step();
		simGPU.step(deltaTime);
		simSto.simTimer.step(deltaTime);
	}
}

void SimThread::restart() {
	QThread::requestInterruption();
}