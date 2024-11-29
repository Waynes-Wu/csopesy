#include "../Headers/PagingAllocator.h"

// Constructor: Initialize Paging Allocator
PagingAllocator::PagingAllocator(size_t maxMemorySize, size_t frameSize)
    : maxMemorySize(maxMemorySize), frameSize(frameSize) {
    numFrames = maxMemorySize / frameSize; // Calculate total number of frames
    for (size_t i = 0; i < numFrames; ++i) {
        freeFrameList.push_back(to_string(i)); // Populate free frame list
    }
}

// Allocate memory for a process
void* PagingAllocator::allocate(ScreenProcess* process) {
    size_t numFramesNeeded = process->memoryRequired / frameSize;

    if (numFramesNeeded > freeFrameList.size()) {
        return nullptr; // Not enough free frames available
    }

    size_t startFrame = allocateFrames(numFramesNeeded, process->processName);
    return reinterpret_cast<void*>(startFrame); // Return starting frame index as pointer
}

// Deallocate memory for a process
void PagingAllocator::deallocate(ScreenProcess* process) {
    size_t numFramesNeeded = process->memoryRequired / frameSize;
    string processId = process->processName;

    // Find all frames allocated to this process
    for (auto it = frameMap.begin(); it != frameMap.end();) {
        if (it->second == processId) {
            freeFrameList.push_back(to_string(it->first)); // Add frame back to free list
            it = frameMap.erase(it); // Erase entry and update iterator
        }
        else {
            ++it;
        }
    }
    sort(freeFrameList.begin(), freeFrameList.end());
}

// Visualize the current memory allocation
string PagingAllocator::visualizeMemory() {
    string visualization = "Memory Visualization:\n";
    for (size_t frameIndex = 0; frameIndex < numFrames; ++frameIndex) {
        auto it = frameMap.find(frameIndex);
        if (it != frameMap.end()) {
            visualization += "Frame " + to_string(frameIndex) + " -> Process " + it->second + "\n";
        }
        else {
            visualization += "Frame " + to_string(frameIndex) + " -> Free\n";
        }
    }
    return visualization;
}

// Allocate specific number of frames
size_t PagingAllocator::allocateFrames(size_t numFrames, string processId) {
    size_t startFrame = stoi(freeFrameList.back()); // Get the last free frame
    for (size_t i = 0; i < numFrames; ++i) {
        size_t frameIndex = stoi(freeFrameList.back());
        frameMap[frameIndex] = processId; // Map frame to process
        freeFrameList.pop_back();         // Remove from free list
    }
    return startFrame;
}

// Deallocate specific frames (implementation unused but defined)
void PagingAllocator::deallocateFrames(size_t numFrames, size_t frameIndex) {
    for (size_t i = 0; i < numFrames; ++i) {
        frameMap.erase(frameIndex + i);            // Remove from frame map
        freeFrameList.push_back(to_string(frameIndex + i)); // Return to free list
    }
    sort(freeFrameList.begin(), freeFrameList.end());
}
