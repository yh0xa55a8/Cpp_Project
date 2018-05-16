#pragma once
#include <QThread>
#include "Storage.h"
class SimThread:public QThread
{
public:
	SimThread();
	~SimThread();
	void run();
	Storage & getSto() { return sto; }
private:
	Storage sto;
};

