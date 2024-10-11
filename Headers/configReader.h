#pragma once

#include "../Headers/stdc++.h"
using namespace std;

struct Config {
    int num_cpu;
    string scheduler;
    int quantum_cycles;
    int batch_process_freq;
    int min_ins;
    int max_ins;
    int delays_per_exec;
    bool isInitialized = false;
};

bool readConfig(Config &config);
