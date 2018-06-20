#pragma once
#include <QThread>
#include "Storage.h"
#include <CPU.h>
#include <GPU.h>
class SimThread:public QThread
{
	Q_OBJECT
public:
	//simulation hardwares
	Storage simSto;
	CPU simCPU;
	GPU simGPU;

	//prevent terminate thread
	~SimThread();

	//main simulation cycle
	void run();

public slots:
	//slot for restart after close a rom
	void restart();

};

