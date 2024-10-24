#pragma once

#include "stdc++.h"
#include "Utils.h" 
#include "ScreenProcess.h"
#include "ConfigReader.h"
#include "Scheduler.h"

using namespace std;

class MainProcess {
private:
    string processName;
    string timeMade;
    string input;
    vector<ScreenProcess*> processList;
    Config config;
    Scheduler *scheduler;

public:
    // Constructor
    MainProcess(string name);

    // Getters
    string getProcessName();

    void open();

    void printActiveProcesses();

 private:
     bool inputChecker(string &input);
};

