#pragma once

#include "stdc++.h"
#include "Utils.h" 
#include "ScreenProcess.h"
#include "configReader.h"

using namespace std;

class Process {
private:
    string processName;
    string timeMade;
    int linesCompleted;
    string input;
    vector<ScreenProcess> processList;
    Config config;

public:
    // Constructor
    Process(string name);

    // Getters
    string getProcessName();

    void open();

    void printActiveProcesses();

 private:
     bool inputChecker(string &input);
};

