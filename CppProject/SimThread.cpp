#include "SimThread.h"


SimThread::~SimThread()
{
	requestInterruption();
	quit();
	wait();
}

void SimThread::run()
{
	simSto.init();
	CPU simCPU;
	GPU simGPU;
	simCPU.init(simSto);
	simGPU.init(simSto);
	while (!isInterruptionRequested()) {
		int deltaTime = simCPU.step();
	}
}