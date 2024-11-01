#pragma once

#include "stdc++.h"
#include "Utils.h" 
#include "ScreenProcess.h"
#include "ConfigReader.h"
#include "Scheduler.h"
#include<conio.h>

using namespace std;

class MainProcess {
private:
    Scheduler* scheduler = nullptr;
    string processName;
    string timeMade;
    string input;
    vector<ScreenProcess*> processList;
    Config config;

public:
    // Constructor
    MainProcess(string name);

    // Getters
    string getProcessName();

    void open();
    void addProcess(ScreenProcess* process);
    void printActiveProcesses();

 private:
     bool inputChecker(string &input);
     int CPUCOUNTER = 1;
     bool initialized = false;
};

