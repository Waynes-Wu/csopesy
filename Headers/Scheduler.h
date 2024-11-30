#pragma once
#include "ScreenProcess.h"
#include "ConfigReader.h"
#include "MemoryAllocator.h"
#include "BackingStore.h"

using namespace std;
#include <filesystem>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <cstdlib>
#include <ctime>
#include <atomic>
#include "CPU.h"

class Scheduler {
public:

    atomic<bool> generate;
    queue<ScreenProcess*> readyQueue;
    queue<ScreenProcess*> inMemoryQueue;
    bool newProcessFlag = false;

    // Constructor
    Scheduler(Config config, vector<ScreenProcess*>* processList, MemoryAllocator* allocator);

    //generate random values [config.min-ins, config.max - ins]
    void generateProcesses(int CPUCOUNTER);
    void stopGenerateProcesses();

    vector<CPU> cpus;

    void start();
    bool newProcessAdded = false;

    // Optional: function to report CPU utilization
    void reportUtilization();

    int getAvailCoreCount();
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
    void startSJF(bool preemptive);

    //?
    condition_variable queueCondVar; 
    MemoryAllocator* allocator = nullptr;
};

