#pragma once

#include "stdc++.h"
#include "Utils.h" 

using namespace std;

class ScreenProcess { 
private:
    string processName;
    string timeMade;
    string input;

    int linesCompleted;
    int numberOfProcess;

    //this is not important, this is for screen
    vector<ScreenProcess> processList;

public:
    // Constructor
    ScreenProcess(string name);

    // Getters
    string getProcessName();

    void open();

    void showProcessInfo();
    void printActiveProcesses();
private:
    bool inputChecker(string& input);
};

