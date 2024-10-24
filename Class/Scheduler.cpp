#include "../Headers/Scheduler.h"
#include "../Headers/Scheduler.h"

// Constructor
Scheduler::Scheduler(Config config) : config(config), nextPid(0), generate(false), numCPUs(config.num_cpu), running(false)  {}

// Function to add a process to the ready queue
void Scheduler::addProcess(ScreenProcess *process) {
    lock_guard<mutex> lock(queueMutex); 
    readyQueue.push(process);             // Add process to the queue
    //queueCondVar.notify_one();           
}

// Function to generate processes infinitely until stopped
void Scheduler::generateProcesses() {
    generate = true;
    srand(static_cast<unsigned>(time(0)));
    while (generate) {
        // Generate a random process (this is a placeholder)
        int randomSteps = rand() % (config.max_ins - config.min_ins + 1) + config.min_ins;
        ScreenProcess *newProcess = new ScreenProcess("p" + to_string(nextPid++), randomSteps);
        addProcess(newProcess);
        // Simulate a delay between process generations 
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
}

// Start Round Robin scheduling
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

// Start First-Come-First-Serve (FCFS) scheduling
void Scheduler::startFCFS() {
    //running = true;
    //vector<thread> threads;

    //for (int i = 0; i < numCPUs; ++i) {
    //    threads.emplace_back([this, i]() {
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
    //            while (!process.isFinished()) {
    //                process.runStep(); // Run steps of the process
    //            }
    //        }
    //        });
    //}

    //// Join threads after running
    //for (auto& thread : threads) {
    //    thread.join();
    //}
}

// Start Shortest Job First (SJF) scheduling
void Scheduler::startSJF() {
    // Implement SJF scheduling logic similar to the others,
    // but prioritize processes based on their remaining steps.
}

// Function to stop the scheduler
void Scheduler::stop() {
    running = false; // Set running to false to stop threads
    queueCondVar.notify_all(); // Wake up all threads
}

// Optional: Function to report CPU utilization
void Scheduler::reportUtilization() {
    // Implement reporting logic here
}
