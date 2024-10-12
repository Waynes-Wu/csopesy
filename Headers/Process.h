#pragma once

#include <mutex>
#include <thread>
#include <chrono>
#include "Utils.h" 

using namespace std;


class Process {
public:
	int pid;
	int currentStep;
	int totalSteps;

	bool isRunning;
	bool isFinished;

	int coreID;

	string startTime;
	string finishTime;
	mutex logMutex;

	//functions
	Process(int id, int steps);
	
	void start(int assignedCoreID);

	void stop();

	void runStep();

	void finish();

	void printStatus();
};