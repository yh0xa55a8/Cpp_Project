#pragma once
#include <QThread>
#include "Storage.h"
#include <CPU.h>
class SimThread:public QThread
{
public:
	SimThread();
	~SimThread();
	void run();
	Storage & getSto() { return simSto; }
public slots:
	void appClosing();
private:
	Storage simSto;
};

