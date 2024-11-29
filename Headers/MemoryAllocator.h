#pragma once
#include "stdc++.h"

#include <vector>
#include <unordered_map>
#include <memory>
#include <string>

#include "ScreenProcess.h"

using namespace std;

class MemoryAllocator {
public:
    virtual void* allocate(ScreenProcess* process) = 0;
    virtual void deallocate(ScreenProcess* process )= 0;
    virtual string visualizeMemory() = 0;
};