#include "../Headers/BackingStore.h"
#include <iostream> // For debug prints

// File path for the backing store in the `csopesy` main directory
const std::string BackingStore::backingFilePath = "backing_store.txt";

void BackingStore::push(ScreenProcess* process) {
    //std::cout << "[DEBUG] BackingStore::push - Pushing process: " << process->getProcessName() << std::endl;

    // Serialize the process
    std::string serializedProcess = serialize(process);

    // Read the current file contents
    std::ifstream file(backingFilePath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    // Write the new process at the top, followed by the old contents
    std::ofstream outFile(backingFilePath, std::ios::trunc);
    outFile << serializedProcess << "\n" << buffer.str();
    outFile.close();

    //std::cout << "[DEBUG] BackingStore::push - Process pushed successfully." << std::endl;
}

ScreenProcess* BackingStore::pop() {
    //std::cout << "[DEBUG] BackingStore::pop - Attempting to pop a process." << std::endl;

    std::ifstream file(backingFilePath);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open backing store file for reading.");
    }

    std::string firstLine;
    if (!std::getline(file, firstLine)) {
        file.close();
        std::cout << "[DEBUG] BackingStore::pop - No processes in the backing store." << std::endl;
        return nullptr;
    }

    // Read the rest of the file
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    // Rewrite the file without the first line
    std::ofstream outFile(backingFilePath, std::ios::trunc);
    outFile << buffer.str();
    outFile.close();

    // Deserialize the popped process
    ScreenProcess* process = deserialize(firstLine);
    //std::cout << "[DEBUG] BackingStore::pop - Popped process: " << process->getProcessName() << std::endl;
    return process;
}

bool BackingStore::empty() {
    std::ifstream file(backingFilePath);
    bool isEmpty = file.peek() == EOF;
    file.close();

    //std::cout << "[DEBUG] BackingStore::empty - Backing store is " << (isEmpty ? "empty" : "not empty") << "." << std::endl;
    return isEmpty;
}

std::string BackingStore::serialize(ScreenProcess* process) {
    if (!process) return "";

    //std::cout << "[DEBUG] BackingStore::serialize - Serializing process: " << process->getProcessName() << std::endl;

    std::stringstream ss;
    ss << process->processName << ","
        << process->linesCompleted << ","
        << process->numberOfProcess << ","
        << process->memoryRequired << ","
        << process->timeMade;

    return ss.str();
}

ScreenProcess* BackingStore::deserialize(const std::string& line) {
    //std::cout << "[DEBUG] BackingStore::deserialize - Deserializing process line: " << line << std::endl;

    std::stringstream ss(line);
    std::string name;
    unsigned int linesCompleted, numberOfProcess;
    size_t memoryRequired;
    std::string timeMade;
    std::getline(ss, name, ',');
    ss >> linesCompleted;
    ss.ignore(1); // Ignore the comma
    ss >> numberOfProcess;
    ss.ignore(1); // Ignore the comma
    ss >> memoryRequired;
    ss.ignore(1); // Ignore the comma
    ss >> timeMade;

    ScreenProcess* process = new ScreenProcess(name, numberOfProcess, memoryRequired);
    process->linesCompleted = linesCompleted;
    process->timeMade = timeMade;

    //std::cout << "[DEBUG] BackingStore::deserialize - Deserialized process: " << name << std::endl;
    return process;
}
