#include "SimThread.h"



SimThread::SimThread()
{
}

SimThread::~SimThread()
{
	requestInterruption();
	quit();
	wait();
}

void SimThread::run()
{
	CPU simCPU(simSto);
	while (!isInterruptionRequested()) {

	}
}