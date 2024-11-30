#include "../Headers/ConfigReader.h"

bool readConfig(Config &config) {
    ifstream configFile("config.txt");
    if (!configFile) {
        cout << "\033[91mError: config.txt file not found!\033[m" << endl;
        return false;
    }

    string line;
    while (getline(configFile, line)) {
        istringstream iss(line);
        string param;
        if (iss >> param) {
            if (param == "num-cpu") {
                iss >> config.num_cpu;
            } else if (param == "scheduler") {
                iss >> config.scheduler;
            } else if (param == "quantum-cycles") {
                iss >> config.quantum_cycles;
            } else if (param == "batch-process-freq") {
                iss >> config.batch_process_freq;
            } else if (param == "min-ins") {
                iss >> config.min_ins;
            } else if (param == "max-ins") {
                iss >> config.max_ins;
            } else if (param == "delays-per-exec") {
                iss >> config.delays_per_exec;
            } else if (param == "max-overall-mem") {
                iss >> config.max_overall_mem;
            } else if (param == "mem-per-frame") {
                iss >> config.mem_per_frame;
            } else if (param == "min-mem-per-proc") {
                iss >> config.min_mem_per_proc;
            } else if (param == "max-mem-per-proc") {
                iss >> config.max_mem_per_proc;
            }
        }
    }
    config.isInitialized = true;
    return true;
}