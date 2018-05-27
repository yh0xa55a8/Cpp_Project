#pragma once
#include <QThread>
#include "Storage.h"
#include <CPU.h>
#include <GPU.h>
class SimThread:public QThread
{
	Q_OBJECT
public:
	GPU simGPU;
	~SimThread();
	void run();
	Storage & getSto() { return simSto; }
public slots:
	void restart();
private:
	Storage simSto;
};

