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
};

bool readConfig(Config &config);
