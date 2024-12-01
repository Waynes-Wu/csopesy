#include "../Headers/PagingAllocator.h"

// Constructor: Initialize Paging Allocator
PagingAllocator::PagingAllocator(size_t maxMemorySize, size_t frameSize)
    : maxMemorySize(maxMemorySize), frameSize(frameSize) {
    numFrames = maxMemorySize / frameSize; // Calculate total number of frames
    for (size_t i = 0; i < numFrames; ++i) {
        freeFrameList.push_back(to_string(i)); // Populate free frame list
    }
    logDebug("PagingAllocator initialized with " + to_string(numFrames) + " frames.");
}

// Allocate memory for a process
void* PagingAllocator::allocate(ScreenProcess* process) {
    size_t numFramesNeeded = (process->memoryRequired + frameSize - 1) / frameSize;

    size_t memoryNeeded = numFramesNeeded * frameSize;

    size_t usedMemory = (numFrames - freeFrameList.size()) * frameSize;

    if (usedMemory + memoryNeeded > maxMemorySize) {
        logDebug("Memory allocation failed for process " + process->getProcessName() +
            " due to insufficient memory. Used: " + to_string(usedMemory) +
            " KB, Max: " + to_string(maxMemorySize) + " KB.");
        return nullptr; 
    }


    if (numFramesNeeded > freeFrameList.size()) {
        logDebug("Not enough free frames to allocate process " + process->getProcessName());
        return nullptr; // Not enough free frames available 
    }

    size_t startFrame = allocateFrames(numFramesNeeded, process->processName);
    pageInCounter += numFramesNeeded; // Increment pageIn counter
    logDebug("Process " + process->getProcessName() + " allocated " + to_string(numFramesNeeded) +
        " frames. Total page-ins: " + to_string(pageInCounter));

    return reinterpret_cast<void*>(startFrame); // Return starting frame index as pointer

}

// Deallocate memory for a process
void PagingAllocator::deallocate(ScreenProcess* process) {
    size_t numFramesNeeded = process->memoryRequired / frameSize;
    string processId = process->processName;

    size_t framesFreed = 0;

    // Find all frames allocated to this process
    for (auto it = frameMap.begin(); it != frameMap.end();) {
        if (it->second == processId) {
            freeFrameList.push_back(to_string(it->first)); // Add frame back to free list
            it = frameMap.erase(it); // Erase entry and update iterator
            framesFreed++;

        }
        else {
            ++it;
        }
    }

    pageOutCounter += framesFreed; // Increment pageOut counter
    logDebug("Process " + process->getProcessName() + " deallocated " + to_string(framesFreed) +
        " frames. Total page-outs: " + to_string(pageOutCounter));
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

// Get page-in count
size_t PagingAllocator::getPageInCount() const {
    return pageInCounter;
}

// Get page-out count
size_t PagingAllocator::getPageOutCount() const {
    return pageOutCounter;
}

// Helper to log debug messages
void PagingAllocator::logDebug(const string& message) {
    ofstream logFile("paging_allocator.log", ios::app); // Append mode
    if (logFile.is_open()) {
        logFile << "[DEBUG] " << message << endl;
        logFile.close();
    }
    else {
        cerr << "Error: Unable to open log file for writing." << endl;
    }
}