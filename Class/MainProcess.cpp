#include "../Headers/MainProcess.h"

thread generatorThread;

MainProcess::MainProcess(string name) {
	processName = name;
	timeMade = getTime();
}

string MainProcess::getProcessName() {
	return processName;
}

void MainProcess::open() {

	printHeader();
	cout << "Enter a command: ";
	while (true) {


		//ask for user char, when string complete return true
		if (getInput(input)) {
			//check string input/command and execute
			//false = exit
			if (!inputChecker(input))
				break;
			cout << "Enter a command: ";
		}
		if (initialized) {
			scheduler->generateProcesses(CPUCOUNTER);
			scheduler->start();
		}

		// slow down counter
		this_thread::sleep_for(chrono::milliseconds(2));
		/*cout << CPUCOUNTER << " | ";*/
		CPUCOUNTER++;

	}

}

void MainProcess::addProcess(ScreenProcess* process) {
	processList.push_back(process);
	//flag that new process was added
	scheduler->newProcessFlag = true;
}



void MainProcess::printActiveProcesses() {
	cout << ESC << YELLOW_TXT << "------------------------------" << RESET << endl;
	cout << processList.size() << " ACTIVE PROCESSES" << endl;  // Print vector size

	for (auto element : processList) {
		element->showProcessInfo();
		cout << endl;
	}
	cout << ESC << YELLOW_TXT << "------------------------------" << RESET << endl;
}

// PRIVATE FUNCTIONS-------------------

bool MainProcess::inputChecker(string& input) {
	vector<string> tokenizedInput = splitInput(input);


	int commandCount = tokenizedInput.size();
	string main_command = tokenizedInput[0];

	if (!config.isInitialized) {
		if (main_command == "initialize") {
			if (readConfig(config)) {
				cout << "Configuration loaded successfully!" << endl;

				if (config.max_overall_mem == config.mem_per_frame) {
					allocator = new FlatMemoryAllocator(config.max_overall_mem);
				}
				else {
					allocator = new PagingAllocator(config.max_overall_mem, config.mem_per_frame);
				}

				scheduler = new Scheduler(config, &processList, allocator);
				initialized = true;
			}
			else {
				cout << "Something went wrong with initialization" << endl;
			}
		}
		else
			cout << "Not initialized" << endl;
		input.clear();
		return true;
	}

	//THIS POINT ONWARD CONFIG SHOULD BE INITIALIZED ALREADY

	if (main_command == "screen") {
		if (commandCount == 1) { cout << "use -r -s or -ls" << endl; return true; }
		string arg_command = tokenizedInput[1];
		//RE-ATTACH TO THE GIVEN THINGY
		if (arg_command == "-r") {
			if (commandCount < 3) { cout << "no arguments after using -r" << endl; return true; }
			bool isFound = false;
			for (auto element : processList) {
				//process name matches user input
				if (element->getProcessName() == tokenizedInput[2]) {
					isFound = true;

					if (element->isFinished) {
						cout << "Process finished cannot be opened/resumed" << endl;
						break;
					}
					// open unfinished process
					system("cls");
					element->open();
					system("cls");
					printHeader();
				}
			}
			if (!isFound) {
				cout << tokenizedInput[2] << " not found" << endl;
			}
		}
		else if (arg_command == "-s") {
			// CREATE NEW PROCESS
			if (commandCount < 3) { cout << "no arguments after using -s" << endl; }
			ScreenProcess* newScreen = new ScreenProcess(tokenizedInput[2]);
			addProcess(newScreen);
			scheduler->newProcessAdded = true;
			newScreen->isRunning = true;
			system("cls");
			newScreen->open();
			system("cls");
			printHeader();
		}
		else if (arg_command == "-ls") {
			vector<ScreenProcess*> activeProcesses;
			vector<ScreenProcess*> finishedProcesses;

			// Separate active and finished processes
			for (auto* process : processList) {
				if (process->isFinished) {
					finishedProcesses.push_back(process);
				}
				else {
					activeProcesses.push_back(process);
				}
			}
			int coreCount = scheduler->getAvailCoreCount();
			// Display CPU utilization and cores information
			cout << "CPU utilization: " << (config.num_cpu - coreCount) / config.num_cpu * 100 << "%" << endl;
			cout << "Cores used: " << config.num_cpu - coreCount << endl;
			cout << "Cores available: " << coreCount << endl;
			cout << ESC << YELLOW_TXT << "\n----------------------------------------------------------" << RESET << endl;

			//------------------RUNNING----------------------
			cout << "Running processes:" << endl;
			cout << setw(15) << "Process Name" << setw(30) << "(time)" << setw(10) << "Core" << setw(20) << "Lines" << endl;
			int displayLimit = 30;
			int displayCounter = 0;

			for (const auto& process : activeProcesses) {
				cout << setw(15) << process->getProcessName()
					<< setw(30) << process->timeMade
					<< setw(10) << (process->coreID == -1 ? "" : to_string(process->coreID))
					<< setw(20) << process->linesCompleted << "/" << process->numberOfProcess << endl;

				if (++displayCounter == displayLimit) {
					cout << "And " << (activeProcesses.size() - displayLimit) << " other more processes" << endl;
					break;
				}
			}

			//--------------------FINISHED---------------------

			cout << "\nFinished processes:" << endl;
			cout << setw(15) << "Process Name" << setw(30) << "(time)" << setw(10) << "Status" << setw(20) << "Lines" << endl;

			for (const auto& process : finishedProcesses) {
				cout << setw(15) << process->getProcessName()
					<< setw(30) << process->timeFinished
					<< setw(10) << "Finished"
					<< setw(20) << process->linesCompleted << "/" << process->numberOfProcess << endl;
			}

			cout << ESC << YELLOW_TXT << "\n----------------------------------------------------------" << RESET << endl;
		}
	}
	else if (main_command == "scheduler-test") {
		scheduler->generate = true;
	}
	else if (main_command == "scheduler-stop") {
		scheduler->generate = false;
	}
	else if (main_command == "process-smi") {
		// Gather memory information from all processes
		size_t totalMemory = config.max_overall_mem;
		size_t usedMemory = 0;
		struct ProcessMemoryInfo {
			std::string processName;
			size_t memoryUsed;
			bool isRunning;
		};
		std::vector<ProcessMemoryInfo> processMemoryList;

		size_t runningCPUs = 0;

		for (size_t i = 0; i < processList.size(); i++) {
			ScreenProcess* process = processList[i];
			if (process && process->memoryPointer != nullptr) {
				size_t memoryUsed = process->memoryRequired;
				usedMemory += memoryUsed;
				if (process->coreID != -1) {
					processMemoryList.push_back({ process->processName, memoryUsed, true });
				}
				else {
					processMemoryList.push_back({ process->processName, memoryUsed, false });
				}
			}
			if (process && process->coreID != -1 && process->memoryPointer != nullptr) {
				runningCPUs++;
			}
		}

		// CPU Utilization percentage
		int cpuUtil = runningCPUs * 100 / config.num_cpu;
		if (cpuUtil > 100) cpuUtil = 100;

		// Header
		std::cout << "----------------------\n";
		std::cout << "| PROCESS-SMI V01.00 |\n";
		std::cout << "----------------------\n";

		// Memory usage summary
		std::cout << "CPU Util.: " << cpuUtil << "%\n";
		std::cout << "Memory Usage: " << usedMemory << " KB / " << totalMemory << " KB\n";
		std::cout << "Memory Util.: " << (usedMemory * 100) / totalMemory << "%\n\n";

		int printLimit = runningCPUs;

		// Running processes and memory usage
		std::cout << "Running Processes and memory usage:\n";
		std::cout << "----------------------\n";
		for (const auto& processInfo : processMemoryList) {
			if (processInfo.isRunning && printLimit > 0) {
				std::cout << processInfo.processName << " | " << processInfo.memoryUsed << " KB\n";
				printLimit--;
			}
		}
		std::cout << "----------------------\n";
	}
	else if (main_command == "vmstat") {
			size_t totalMemory = config.max_overall_mem;
			size_t usedMemory = 0;
			struct ProcessMemoryInfo {
				std::string processName;
				size_t memoryUsed;
			};
			std::vector<ProcessMemoryInfo> processMemoryList; // Stores memory usage for each process

			for (size_t i = 0; i < processList.size(); i++) {
				ScreenProcess *process = processList[i];
				if (process && process->memoryPointer!=nullptr) {
					size_t memoryUsed = process->memoryRequired;
					usedMemory += memoryUsed;
					processMemoryList.push_back({ process->processName, memoryUsed });
				}
			}
			// CPU ticks and page information
			long activeTicks = 0;
			long idleTicks = 0;

			

			for (auto& cpu : scheduler->cpus) {
				activeTicks += cpu.activeTicks;
				idleTicks += cpu.idleTicks;
			}
			long totalTicks = activeTicks + idleTicks;


			size_t pagesIn =0 ;
			size_t pagesOut =0;
		
			// Retrieve page in and page out counts if allocator is PagingAllocator
			if (!(config.max_overall_mem == config.mem_per_frame)) {
				PagingAllocator* pagingAllocator = dynamic_cast<PagingAllocator*>(allocator);
				if (pagingAllocator) {
					pagesIn = pagingAllocator->getPageInCount();
					pagesOut = pagingAllocator->getPageOutCount();
				}
			}
			
			size_t freeMemory = totalMemory - usedMemory;

			if (usedMemory > totalMemory) {
				//cout << "[ERROR] Memory usage exceeds maximum memory size!" << endl;
				usedMemory = totalMemory; // Clamp to max memory size
				freeMemory = 0;
			}

			// Display memory and CPU information
			std::cout << "===== ===== ===== ===== =====\n";
			std::cout << "Total Memory: " << totalMemory << " KB\n";
			std::cout << "Used Memory: " << usedMemory << " KB\n";
			std::cout << "Free Memory: " << freeMemory << " KB\n";
			std::cout << "Idle CPU Ticks: " << idleTicks << "\n";
			std::cout << "Active CPU Ticks: " << activeTicks << "\n";
			std::cout << "Total CPU Ticks: " << totalTicks << "\n";
			std::cout << "Pages Paged In: " << pagesIn << "\n";
			std::cout << "Pages Paged Out: " << pagesOut << "\n";
			std::cout << "===== ===== ===== ===== =====\n";

	}
	else if (main_command == "report-util") {

		ofstream logFile("csopesy-log.txt"); // Open file for writing
		if (!logFile.is_open()) {
			cout << "Error opening log file." << endl;
			return true;
		}
		streambuf* coutBuf = cout.rdbuf();
		cout.rdbuf(logFile.rdbuf());

		// Print the process report to the file through cout
		vector<ScreenProcess*> activeProcesses;
		vector<ScreenProcess*> finishedProcesses;

		// Separate active and finished processes
		for (auto* process : processList) {
			if (process->isFinished) {
				finishedProcesses.push_back(process);
			}
			else {
				activeProcesses.push_back(process);
			}
		}
		int coreCount = scheduler->getAvailCoreCount();
		// Display CPU utilization and cores information
		cout << "CPU utilization: " << (config.num_cpu - coreCount) / config.num_cpu * 100 << "%" << endl;
		cout << "Cores used: " << config.num_cpu - coreCount << endl;
		cout << "Cores available: " << coreCount << endl;

		cout << "\n----------------------------------------------------------" << endl;
		//------------------RUNNING----------------------
		cout << "Running processes:" << endl;
		cout << setw(15) << "Process Name" << setw(30) << "(time)" << setw(10) << "Core" << setw(20) << "Lines" << endl;

		for (const auto& process : activeProcesses) {
			cout << setw(15) << process->getProcessName()
				<< setw(30) << process->timeMade
				<< setw(10) << (process->coreID == -1 ? "" : to_string(process->coreID))
				<< setw(20) << process->linesCompleted << "/" << process->numberOfProcess << endl;
		}

		//--------------------FINISHED---------------------
		cout << "\nFinished processes:" << endl;
		cout << setw(15) << "Process Name" << setw(30) << "(time)" << setw(10) << "Status" << setw(20) << "Lines" << endl;

		for (const auto& process : finishedProcesses) {
			cout << setw(15) << process->getProcessName()
				<< setw(30) << process->timeFinished
				<< setw(10) << "Finished"
				<< setw(20) << process->linesCompleted << "/" << process->numberOfProcess << endl;
		}

		cout << "\n----------------------------------------------------------" << endl;


		// Reset cout to console output
		cout.rdbuf(coutBuf);
		logFile.close();

		cout << "Process report saved to csopesy-log.txt." << endl;
	}

	else if (main_command == "clear")
		system("cls");
	else if (main_command == "exit")
		return false;

	//! ----------FOR DEBUGGING ONLY --------------------------------

	else if (main_command == "runProcess") {
		string processName = tokenizedInput[1];
		ScreenProcess* foundProcess = nullptr;
		for (auto element : processList) {
			if (element->getProcessName() == processName) {
				foundProcess = element;
			}
		}
		if (foundProcess != nullptr) {
			//foundProcess->start();
			while (!foundProcess->isFinished) {
				foundProcess->runStep();
			}
			foundProcess->printStatus();

		}
	}
	else {
		cout << ESC << GREEN_TXT;
		cout << input << " command unrecognized" << endl;
		cout << RESET;
	}
	input.clear();
	return true;
}

