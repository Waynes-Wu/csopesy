#include "../Headers/Scheduler.h"

// Constructor
Scheduler::Scheduler(Config config, vector<ScreenProcess*>* processList, MemoryAllocator* allocator) : config(config), processList(processList), nextPid(0), generate(false), numCPUs(config.num_cpu), isRunning(false), allocator(allocator) {
    srand(static_cast<unsigned>(time(0)));
    for (int i = 0; i < numCPUs; ++i) {
        cpus.emplace_back(CPU(i));
    }
}



// Function to generate processes infinitely until stopped
//vector<ScreenProcess*>& processList
void Scheduler::generateProcesses(int CPUCOUNTER) {
    int freq = config.batch_process_freq * 5;   
    //while (generate && CPUCOUNTER % config.batch_process_freq == 0) {
    if (generate && CPUCOUNTER % config.batch_process_freq == 0) {
        // Generate a random process (this is a placeholder)
        unsigned int randomSteps = rand() % (config.max_ins - config.min_ins + 1) + config.min_ins;
        
        // TODO: how much memory needed for a process?
        ScreenProcess *newProcess = new ScreenProcess("p" + to_string(nextPid++), randomSteps, config.min_mem_per_proc + rand() % (config.max_mem_per_proc - config.min_mem_per_proc + 1));

        processList->push_back(newProcess);
        newProcessAdded = true;
        
    }
    return;
}
void Scheduler::stopGenerateProcesses() {
    isRunning = false; 
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
        if (cpu.currentProcess == nullptr)
            availCPU++;
    }
    return availCPU;
}


//if flag = true then new process has arrived 
void Scheduler::startRoundRobin(int timeQuantum) {
    isRunning = true;

    // Step 1: Add new processes to the readyQueue
    if (newProcessAdded) {
        readyQueue.push(processList->back());
        newProcessAdded = false;
    }

    while (!BackingStore::empty()) {
        ScreenProcess* processFromStore = BackingStore::pop();

        void* allocatedMemory = allocator->allocate(processFromStore);
        if (allocatedMemory) {
            processFromStore->memoryPointer = allocatedMemory;
            inMemoryQueue.push(processFromStore);
        }
        else {
            // If memory allocation fails, push back to the backing store
            BackingStore::push(processFromStore);
            break; // Stop trying to load further processes
        }
    }


    while (!readyQueue.empty()) {
        ScreenProcess* process = readyQueue.front();
        readyQueue.pop();

        // Attempt to allocate memory
        void* allocatedMemory = allocator->allocate(process);

        if (!allocatedMemory) {
            // Memory allocation failed; move process to the backing store
            BackingStore::push(process);
        }
        else {
            // Memory allocation succeeded; move process to inMemoryQueue
            process->memoryPointer = allocatedMemory;
            inMemoryQueue.push(process);
        }
    }


    // Step 3: Handle processes in the inMemoryQueue (Round Robin Logic)
    for (auto& cpu : cpus) {
        if (!cpu.isBusy()) {
            // Process management logic for the CPU
            if (cpu.currentProcess != nullptr) {

                allocator->deallocate(cpu.currentProcess);

                if (cpu.currentProcess->isFinished) {
                    
                    cpu.clearProcess();
                }
                else {
                    inMemoryQueue.push(cpu.currentProcess);
                }
            }

            // Assign a new process to the CPU from inMemoryQueue
            if (!inMemoryQueue.empty()) {
                ScreenProcess* nextProcess = inMemoryQueue.front();
                inMemoryQueue.pop();
                cpu.assignProcess(nextProcess);
            }

            // Execute the process
            thread(
                static_cast<void (CPU::*)(unsigned int, unsigned int)>(&CPU::run),
                &cpu,
                config.delays_per_exec,
                timeQuantum
            ).detach();
        }
        if (!cpu.isBusy()) {
            cpu.idleTicks++;
        }
        else {
            cpu.activeTicks++;
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
