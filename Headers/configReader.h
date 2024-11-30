#pragma once

#include "../Headers/stdc++.h"
using namespace std;

struct Config {
    int num_cpu;
    string scheduler;
    unsigned int quantum_cycles;
    unsigned int batch_process_freq;
    unsigned int min_ins;
    unsigned int max_ins;
    unsigned int delays_per_exec;
    bool isInitialized = false;
    size_t max_overall_mem;
    size_t mem_per_frame;
    size_t min_mem_per_proc;
    size_t max_mem_per_proc;
};

bool readConfig(Config &config);
