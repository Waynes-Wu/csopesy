#include "../Headers/Scheduler.h"

// Constructor
Scheduler::Scheduler(Config config, vector<ScreenProcess*>* processList) : config(config), processList(processList), nextPid(0), generate(false), numCPUs(config.num_cpu), isRunning(false) {
    srand(static_cast<unsigned>(time(0)));
    for (int i = 0; i < numCPUs; ++i) {
        cpus.emplace_back(CPU(i));
    }
}



// Function to generate processes infinitely until stopped
//vector<ScreenProcess*>& processList
void Scheduler::generateProcesses(int CPUCOUNTER) {
    //while (generate && CPUCOUNTER % config.batch_process_freq == 0) {
    if (generate && CPUCOUNTER % config.batch_process_freq == 0) {
        // Generate a random process (this is a placeholder)
        unsigned int randomSteps = rand() % (config.max_ins - config.min_ins + 1) + config.min_ins;
        ScreenProcess *newProcess = new ScreenProcess("p" + to_string(nextPid++), randomSteps);

        processList->push_back(newProcess);
        newProcessAdded = true;
        //this_thread::sleep_for(chrono::milliseconds(1000));
    }
    return;
}
void Scheduler::stopGenerateProcesses() {
    isRunning = false; // Set running to false to stop threads
    //queueCondVar.notify_all(); // Wake up all threads
}


void Scheduler::start() {
   
    if (config.scheduler == "fcfs") {
        startFCFS();
    }
    else if (config.scheduler == "rr") {
        startRoundRobin(config.quantum_cycles);
    }
    else if (config.scheduler == "sjf") {
        startSJF(false);
    }
}

int Scheduler::getAvailCoreCount() {
    int availCPU = 0;
    for (auto& cpu : cpus) {
        if (!cpu.isBusy())
            availCPU++;
    }
    return availCPU;
}


//if flag = true then new process has arrived 
void Scheduler::startRoundRobin(int timeQuantum) {
    isRunning = true;

    if (newProcessAdded) {
        readyQueue.push(processList->back());
        newProcessAdded = false;
    }

    int availCPU = getAvailCoreCount();

    for (auto& cpu : cpus) {
        if (!cpu.isBusy()) {
            //if cpu is not busy and has process
            if (cpu.currentProcess != nullptr) {
            readyQueue.push(cpu.currentProcess);
            }

            //do this after its been pushed
            if (!readyQueue.empty()) {
            cpu.assignProcess(readyQueue.front());
            readyQueue.pop();
             
            // Threaded execution with time slice
            thread(static_cast<void (CPU::*)(unsigned int, unsigned int)>(&CPU::run), &cpu, config.delays_per_exec, timeQuantum).detach();

            }

        }
    }
}

void Scheduler::startFCFS() {
    isRunning = true;
   
    if (newProcessAdded) {
        readyQueue.push(processList->back());
        newProcessAdded = false;
    }
    
    int availCPU = getAvailCoreCount();

    for (auto& cpu : cpus) {
        if (!cpu.isBusy() && !readyQueue.empty()) {
            cpu.assignProcess(readyQueue.front());
            readyQueue.pop();

            //threaded
            thread(static_cast<void (CPU::*)(unsigned int)>(&CPU::run), &cpu, config.delays_per_exec).detach();
        }
        ////non-threaded running
        //cpu.run();
    }
}

void Scheduler::startSJF(bool preemptive) {
    //isRunning = true;

    //if (newProcessAdded) {
    //    readyQueue.push(processList->back());
    //    newProcessAdded = false;
    //}
    //int availCPU = getAvailCoreCount();


    //for (auto& cpu : cpus) {
    //    if (!cpu.isBusy()) {
    //        if (!readyQueue.empty()) {
    //            ScreenProcess* shortestProcess = nullptr;

    //            for (size_t i = 0; i < readyQueue.size(); ++i) {
    //                ScreenProcess* process = readyQueue.front();
    //                readyQueue.pop();
    //                if (shortestProcess == nullptr || process->burstTime < shortestProcess->burstTime) {
    //                    if (shortestProcess != nullptr) {
    //                        readyQueue.push(shortestProcess);
    //                    }
    //                    shortestProcess = process;
    //                }
    //                else {
    //                    readyQueue.push(process);
    //                }
    //            }

    //            cpu.assignProcess(shortestProcess);

    //            if (preemptive) {
    //                thread(&CPU::run, &cpu, config.delays_per_exec, shortestProcess->burstTime).detach();
    //            }
    //            else {
    //                thread(&CPU::run, &cpu, config.delays_per_exec, shortestProcess->burstTime).detach();
    //            }

    //            readyQueue.pop();
    //        }
    //    }
    //    else {
    //        if (preemptive) {
    //            ScreenProcess* currentProcess = cpu.currentProcess;
    //            for (size_t i = 0; i < readyQueue.size(); ++i) {
    //                ScreenProcess* process = readyQueue.front();
    //                readyQueue.pop();
    //                if (process->burstTime < currentProcess->burstTime) {
    //                    readyQueue.push(currentProcess);
    //                    cpu.assignProcess(process);
    //                    thread(&CPU::run, &cpu, config.delays_per_exec, process->burstTime).detach();
    //                    break;
    //                }
    //                readyQueue.push(process);
    //            }
    //        }
    //    }
    //}
}


// Optional: Function to report CPU utilization
void Scheduler::reportUtilization() {
    // Implement reporting logic here
}
