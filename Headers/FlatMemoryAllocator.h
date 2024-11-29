#pragma once
#include "MemoryAllocator.h"
#include "stdc++.h"

#include "ScreenProcess.h"

#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <sstream>

using namespace std;

class FlatMemoryAllocator : public MemoryAllocator {
public:
	FlatMemoryAllocator(size_t maximumSize);
	~FlatMemoryAllocator();

	void* allocate(ScreenProcess* process) override;
	void deallocate(ScreenProcess *process) override;
	string visualizeMemory() override;

private:
	size_t maximumSize;
	size_t allocatedSize;
	vector<char> memory;
	unordered_map<size_t, bool> allocationMap;
	int fileCounter = 0;
	void initializeMemory();
	bool canAllocateAt(size_t index, size_t size) const;
	void allocateAt(size_t index, size_t size);
	void deallocateAt(size_t index, size_t size);

	unordered_map<size_t, size_t> allocationSizes;  // Maps starting index to allocated size
};
