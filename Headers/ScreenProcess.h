#pragma once

#include "stdc++.h"
#include "Utils.h" 
#include <thread>

using namespace std;

class ScreenProcess { 
public:
    int coreID;
    bool isRunning;
    bool isFinished;

    string timeMade;
    string timeFinished;
    string input;

    void* memoryPointer;
    size_t memoryRequired;

    string processName;
    unsigned int linesCompleted;
    unsigned int numberOfProcess;
    // Constructor
    ScreenProcess(string name);
    ScreenProcess(string name, int numberOfProcesses, size_t memoryRequired);

    // Getters
    string getProcessName();

    //console switching
    void open();
    void showProcessInfo();
    void printActiveProcesses();

    void start();

    void stop();

    void runStep();

    void finish();

    void printStatus();


private:




    //this is not important, this is for screen
    vector<ScreenProcess> processList;

    bool inputChecker(string& input);
};

