#pragma once

#include "stdc++.h"
#include "Utils.h" 

class ScreenProcess { 
private:
    string processName;
    string timeMade;
    int linesCompleted;
    string input;
    vector<ScreenProcess> processList;

public:
    // Constructor
    ScreenProcess(string name);

    // Getters
    string getProcessName();

    void open();

    void showProcessInfo();
    static void printActiveProcesses(const vector<ScreenProcess> processList);



};

