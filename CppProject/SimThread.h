#pragma once
#include <QThread>
#include "Storage.h"
#include <CPU.h>
#include <GPU.h>
class SimThread:public QThread
{
public:
	GPU simGPU;
	~SimThread();
	void run();
	Storage & getSto() { return simSto; }
private:
	Storage simSto;
};

