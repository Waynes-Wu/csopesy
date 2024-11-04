#pragma once

#include "stdc++.h"
#include "Utils.h" 
#include <thread>
#include "ScreenProcess.h"

class CPU {
public:
    // Constructor
    CPU(int i);

    // Public Methods
    bool isBusy();            // Check if the CPU is busy with a process
    void assignProcess(ScreenProcess* process); // Assign a new process to the CPU
    void run(unsigned int executionDelay);                     // Run one step of the assigned process
    void run(unsigned int executionDelay, unsigned int maxLines);
    void clearProcess();            // Clear the current process when finished
    int getID();
    ScreenProcess* currentProcess;  // Pointer to the current process assigned to this CPU

private:
    bool busy;                      // Status flag to indicate if the CPU is busy
    int number;
};

