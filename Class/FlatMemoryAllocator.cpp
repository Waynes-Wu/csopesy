#include "../Headers/FlatMemoryAllocator.h"

FlatMemoryAllocator::FlatMemoryAllocator(size_t maximumSize)
    : maximumSize(maximumSize), allocatedSize(0) {
    memory.resize(maximumSize);  // Allocate and set the size
    initializeMemory(); // Initialize the memory contents
}

// Destructor
FlatMemoryAllocator::~FlatMemoryAllocator() {
    memory.clear();
}

// Allocate memory for a process and return a pointer to the allocated memory
void* FlatMemoryAllocator::allocate(ScreenProcess* process) {
    size_t size = process->memoryRequired;  // Get memory required by process

    // Find the first available block of sufficient size
    for (size_t i = 0; i <= maximumSize - size; ++i) {
        // Check if the block starting at index i is free and can fit the requested size
        if (!allocationMap[i] && canAllocateAt(i, size)) {
            // Allocate memory for the process
            allocateAt(i, size);
            return &memory[i];  // Return pointer to allocated memory
        }
    }

    // If no suitable block found, allocation fails
    cout << "Failed to allocate memory for process of size " << size << endl;
    return nullptr;
}

// Deallocate memory used by a process
void FlatMemoryAllocator::deallocate(ScreenProcess* process) {
    void * startAddress = process->memoryPointer;  // Get starting address (index)
    size_t index = static_cast<char*>(startAddress) - &memory[0];
    size_t size = process->memoryRequired;  // Get memory size to be deallocated

    // If memory was allocated at this address, deallocate it
    if (allocationMap[index]) { // Ensure the block was allocated
        // Find the size of the allocated block using the allocationSizes map
        auto it = allocationSizes.find(index);
        if (it != allocationSizes.end()) {
            size_t size = it->second; // Retrieve the size
            deallocateAt(index, size); // Deallocate the memory
            allocationSizes.erase(it);  // Remove entry from allocationSizes after deallocation
            //cout << "Deallocated memory at index " << index << " of size " << size << "\n";
        }
        else {
            cerr << "Warning: Attempted to deallocate untracked memory at index " << index << "\n";
        }
    }
    else {
        cerr << "Warning: Attempted to deallocate unallocated memory at index " << index << "\n";
    }
}

// Visualize memory status (optional for debugging)
string FlatMemoryAllocator::visualizeMemory() {
    ostringstream visualization;
    size_t freeSpace = maximumSize - allocatedSize;

    // Prepare the memory usage information for visualization
    visualization << "Total Memory: " << maximumSize << " bytes\n";
    visualization << "Allocated Memory: " << allocatedSize << " bytes\n";
    visualization << "Free Memory: " << freeSpace << " bytes\n";

    // Save visualization to file
    string filename = "./memory_visualization_" + to_string(fileCounter) + ".txt";
    ofstream file(filename);
    file << visualization.str();
    file.close();

    // Increment the counter for the next iteration
    fileCounter++;

    return filename;
}

void FlatMemoryAllocator::initializeMemory() {
    // Initialize memory vector with '.'
    fill(memory.begin(), memory.end(), '.');
    // Set each entry in allocationMap to false (unallocated)
    for (size_t i = 0; i < maximumSize; ++i) {
        allocationMap[i] = false;
    }
}

// Check if a block of memory of given size can be allocated at index
bool FlatMemoryAllocator::canAllocateAt(size_t index, size_t size) const {
    if (index + size > maximumSize) return false;  // Ensure allocation is within bounds
    for (size_t i = 0; i < size; ++i) {
        if (allocationMap.at(index + i)) return false;  // If any part of the block is allocated, return false
    }
    return true;
}

// Allocate memory starting at index for the given size
void FlatMemoryAllocator::allocateAt(size_t index, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        allocationMap[index + i] = true;  // Mark memory as allocated
        memory[index + i] = 'P';  // Mark memory as occupied by process ('P')
    }
    allocatedSize += size;
}

// Deallocate memory starting at index for the given size
void FlatMemoryAllocator::deallocateAt(size_t index, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        allocationMap[index + i] = false;  // Mark memory as free
        memory[index + i] = '.';  // Mark memory as free ('.')
    }
    allocatedSize -= size;
}