#include "SimThread.h"


SimThread::~SimThread()
{
	requestInterruption();
	quit();
	wait();
}

void SimThread::run()
{
	CPU simCPU;

	simCPU.init(simSto);
	simGPU.init(simSto);
	while (!isInterruptionRequested()) {
		int deltaTime = simCPU.step();
		simGPU.step(deltaTime);
	}
}

void SimThread::restart() {
	QThread::requestInterruption();
	
}