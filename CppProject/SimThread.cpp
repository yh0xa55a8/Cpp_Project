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
	simGPU.init(simSto,simSto.VRamTileSet);
	while (!isInterruptionRequested()) {
		int deltaTime = simCPU.step();
		simGPU.step(deltaTime);
	}
}