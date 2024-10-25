#include "../Headers/Scheduler.h"

// Constructor
Scheduler::Scheduler(Config config) : config(config), nextPid(0), generate(false), numCPUs(config.num_cpu), isRunning(false) {}


// Function to generate processes infinitely until stopped
void Scheduler::generateProcesses(int CPUCOUNTER, vector<ScreenProcess*>& processList) {
    srand(static_cast<unsigned>(time(0)));
    //while (generate && CPUCOUNTER % config.batch_process_freq == 0) {
    if (generate && CPUCOUNTER % config.batch_process_freq == 0) {
        // Generate a random process (this is a placeholder)
        int randomSteps = rand() % (config.max_ins - config.min_ins + 1) + config.min_ins;
        ScreenProcess *newProcess = new ScreenProcess("p" + to_string(nextPid++), randomSteps);

        processList.push_back(newProcess);
        
        //this_thread::sleep_for(chrono::milliseconds(1000));
    }
}
void Scheduler::stopGenerateProcesses() {
    isRunning = false; // Set running to false to stop threads
    //queueCondVar.notify_all(); // Wake up all threads
}


//if flag = true then new process has arrived 

void Scheduler::startRoundRobin(int timeQuantum) {
    //running = true;
    //vector<thread> threads;

    //for (int i = 0; i < numCPUs; ++i) {
    //    threads.emplace_back([this, i, timeQuantum]() {
    //        while (running) {
    //            ScreenProcess process;

    //            {
    //                unique_lock<mutex> lock(queueMutex);
    //                queueCondVar.wait(lock, [this] { return !readyQueue.empty() || !running; });

    //                if (!running) break; // Exit if not running
    //                process = readyQueue.front(); // Get the process from the front
    //                readyQueue.pop();              // Remove it from the queue
    //            }

    //            // Execute the process
    //            process.start(i); // Start the process on CPU i
    //            for (int j = 0; j < timeQuantum && !process.isFinished(); ++j) {
    //                process.runStep(); // Run steps of the process
    //            }
    //            if (!process.isFinished()) {
    //                addProcess(process); // Re-add to the queue if not finished
    //            }
    //        }
    //        });
    //}

    //// Join threads after running
    //for (auto& thread : threads) {
    //    thread.join();
    //}
}

void Scheduler::startFCFS() {
    isRunning = true;
    //vector<thread> threads;

    //if new process added
        //add to queue

    for (int i = 0; i < numCPUs; ++i) {
        //check for vacant cpu
    }
    
            



}

void Scheduler::startSJF() {
    // Implement SJF scheduling logic similar to the others,
    // but prioritize processes based on their remaining steps.
}


// Optional: Function to report CPU utilization
void Scheduler::reportUtilization() {
    // Implement reporting logic here
}
