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
    Scheduler(Config config, vector<ScreenProcess*>* processList);

    //generate random values [config.min-ins, config.max - ins]
    void generateProcesses(int CPUCOUNTER);
    void stopGenerateProcesses();


    void start();
    bool newProcessAdded = false;

    // Optional: function to report CPU utilization
    void reportUtilization();

private:
    int nextPid;
    int numCPUs; 
    Config config;

    vector<ScreenProcess*>* processList;

    //not sure if needed
    vector<thread> cpuThreads;  
    mutex queueMutex;                
    
    bool isRunning;                          

    // Scheduling algorithms make empty first
    void startRoundRobin(int timeQuantum);
    void startFCFS();
    void startSJF();

    //?
    condition_variable queueCondVar; 

};

