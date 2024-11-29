#pragma once
#include "MemoryAllocator.h"
#include "stdc++.h"

#include "ScreenProcess.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

class PagingAllocator : public MemoryAllocator {
public:
	PagingAllocator(size_t maxMemorySize, size_t frameSize);
	//~PagingAllocator() override = default;

	void* allocate(ScreenProcess* process) override;
	void deallocate(ScreenProcess* process) override;
	string visualizeMemory() override;

private:
	size_t maxMemorySize;
	size_t frameSize;
	size_t numFrames;
	vector<string> freeFrameList;
	unordered_map<size_t, string> frameMap;

	size_t allocateFrames(size_t numFrames, string processId);
	void deallocateFrames(size_t numFrames, size_t frameIndex);
};
