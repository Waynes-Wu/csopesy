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

    // Constructor
    Scheduler(Config config);

    // Function to add a process to the ready queue
    void addProcess(ScreenProcess* process);
    //generate random values [config.min-ins, config.max - ins]
    void generateProcesses();

    //MAKE THREADING

    // Scheduling algorithms make empty first
    void startRoundRobin(int timeQuantum);
    void startFCFS();
    void startSJF();

    // Function to stop the scheduler
    void stop();

    // Optional: function to report CPU utilization
    void reportUtilization();

private:
    int nextPid;
    
    int numCPUs; 
    Config config;

    
    //not sure if needed
    vector<thread> cpuThreads;  
    mutex queueMutex;                
    

    bool running;                          

    //?
    condition_variable queueCondVar; 

    // Internal method to execute a process on a specific CPU
    //void executeProcess(int cpuID, ScreenProcess process);
};

