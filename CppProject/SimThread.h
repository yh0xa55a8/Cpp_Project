#pragma once
#include <QThread>
#include "Storage.h"
#include <CPU.h>
class SimThread:public QThread
{
public:
	~SimThread();
	void run();
	Storage & getSto() { return simSto; }
private:
	Storage simSto;
};

