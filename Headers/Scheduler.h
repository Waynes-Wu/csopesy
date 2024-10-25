#pragma once
#include "../Headers/ScreenProcess.h"
#include "../Headers/ConfigReader.h"

using namespace std;
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <cstdlib>
#include <ctime>
#include <atomic>

class Scheduler {
public:

    atomic<bool> generate;
    queue<ScreenProcess*> readyQueue;
    bool newProcessFlag = false;

    // Constructor
    Scheduler(Config config);

    //generate random values [config.min-ins, config.max - ins]
    void generateProcesses(int CPUCOUNTER, vector<ScreenProcess*>& processList);
    void stopGenerateProcesses();


    // Scheduling algorithms make empty first
    void startRoundRobin(int timeQuantum);
    void startFCFS();
    void startSJF();



    // Optional: function to report CPU utilization
    void reportUtilization();

private:
    int nextPid;
    int numCPUs; 
    Config config;
    
    //not sure if needed
    vector<thread> cpuThreads;  
    mutex queueMutex;                
    
    bool isRunning;                          


    //?
    condition_variable queueCondVar; 

    // Internal method to execute a process on a specific CPU
    //void executeProcess(int cpuID, ScreenProcess process);
};

