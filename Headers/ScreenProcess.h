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

    string processName;
    int linesCompleted;
    int numberOfProcess;
    // Constructor
    ScreenProcess(string name);
    ScreenProcess(string name, int numberOfProcesses);

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

