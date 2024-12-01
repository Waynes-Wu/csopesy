#include "../Headers/CPU.h"

// Constructor
CPU::CPU(int i) : currentProcess(nullptr), busy(false), number(i) {}

// Check if CPU is busy
bool CPU::isBusy() {
    return busy;
}

// Assign a process to the CPU
void CPU::assignProcess(ScreenProcess* process) {

    if (currentProcess != nullptr) {
        currentProcess->stop();
    }
    currentProcess = process;
    process->coreID = number;

    busy = true;
}

// Run one step of the assigned process
void CPU::run(unsigned int executionDelay, unsigned int maxLines) {

    //if (currentProcess != nullptr) {
    //    currentProcess->runStep(); // Execute one time slice of the process

    //    // Check if the process is finished
    //    if (currentProcess->isFinished) {
    //        clearProcess(); // Clear the process if it's complete
    //    }
    //}


    //threaded
    if (currentProcess != nullptr) {

        // Run the process in steps until finished
        while (maxLines-- && !currentProcess->isFinished) {
            //switching can be done just by reassigning?
            currentProcess->runStep(); 
            this_thread::sleep_for(chrono::milliseconds(executionDelay));
        }
        // Clear the process if it finishes
        if (currentProcess->isFinished) {
            clearProcess();
        }
        else {
            // If the process isn't finished, mark the CPU as free for scheduling
            busy = false;
        }
    }

}
void CPU::run(unsigned int executionDelay) {
    if (currentProcess != nullptr) {

        while (!currentProcess->isFinished) {
            currentProcess->runStep(); // Execute one step of the process
            this_thread::sleep_for(chrono::milliseconds(executionDelay)); // Introduce delay
        }
        busy = false;

    //    // Check if the process is finished
    //    if (currentProcess->isFinished) {
    //        clearProcess();
    //    } 
    //    else {
    //        busy = false;
    //    }
    }

}


// Clear the current process when finished
void CPU::clearProcess() {
    currentProcess = nullptr;
    busy = false;
}
int CPU::getID() {
    return number;
}