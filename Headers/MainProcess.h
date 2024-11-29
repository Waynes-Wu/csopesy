#pragma once

#include "stdc++.h"
#include "Utils.h" 
#include "ScreenProcess.h"
#include "ConfigReader.h"
#include "Scheduler.h"
#include<conio.h>
#include <streambuf>
#include "MemoryAllocator.h"
#include "FlatMemoryAllocator.h"
#include "PagingAllocator.h"

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
     long long CPUCOUNTER = 1;
     bool initialized = false;
     MemoryAllocator* allocator = nullptr;
};

