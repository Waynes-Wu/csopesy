#pragma once

#include <fstream>
#include <queue>
#include <string>
#include <vector>
#include <sstream>
#include "ScreenProcess.h"
#include <iostream>

class BackingStore {
public:
    // Push a process to the backing store (add to top of the file)
    static void push(ScreenProcess* process);

    // Pop a process from the backing store (read first line and remove it)
    static ScreenProcess* pop();

    // Check if the backing store is empty
    static bool empty();

private:
    // File path for the backing store
    static const std::string backingFilePath;

    // Helper methods to serialize and deserialize processes
    static std::string serialize(ScreenProcess* process);
    static ScreenProcess* deserialize(const std::string& line);
};

