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
            }
        }
    }
    config.isInitialized = true;
    return true;
}