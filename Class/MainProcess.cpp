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

	while (true) {
		cout << "Enter a command: ";
		getline(cin, input);
		if (!inputChecker(input)) 
			break;
	}

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

bool MainProcess::inputChecker(string &input) {
		vector<string> tokenizedInput = splitInput(input);


		int commandCount = tokenizedInput.size();
		string main_command = tokenizedInput[0];
		if (!config.isInitialized) {
			if (main_command == "initialize") {
				if (readConfig(config)) {
					cout << "Configuration loaded successfully!" << endl;
					scheduler = new Scheduler(config);
				}
				else {
					cout << "Something went wrong with initialization" << endl;
				}
			}
			else
				cout << "Not initialized" << endl;
			return true;
		}

		//THIS POINT ONWARD CONFIG SHOULD BE INITIALIZED ALREADY

		if (main_command == "screen") {
			if (commandCount == 1) { cout << "use -r -s or -ls" << endl; return true; }
			string arg_command = tokenizedInput[1];
			//RE-ATTACH TO THE GIVEN THINGY
			if (arg_command == "-r") {
				if (commandCount < 3) { cout << "no arguments after using -r" << endl; return true;}
				bool isFound = false;
				for (auto element : processList) {
					if (element->getProcessName() == tokenizedInput[2]) {
						isFound = true;
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
				ScreenProcess newScreen(tokenizedInput[2]);
				processList.push_back(&newScreen);
				newScreen.isRunning = true;
				cout << "new process added! " << newScreen.getProcessName() << endl;
				system("cls");
				newScreen.open();
				system("cls");
				printHeader();
			}
			else if (arg_command == "-ls") {
				printActiveProcesses();
			}
		}
		else if (main_command == "scheduler-test") {

			// check if thread not running
			if (!generatorThread.joinable()) { 
				generatorThread = thread(&Scheduler::generateProcesses, scheduler);
			}
			else {
				cout << "Process generation is already running." << endl;
			}

			//ScreenProcess hi("lmao");
			//scheduler->addProcess(&hi);
			//scheduler->generateProcesses();


		}
		else if (main_command == "scheduler-stop") {
			scheduler->generate = false;
			if (generatorThread.joinable()) {
				generatorThread.join(); 
				cout << "Process generation stopped." << endl;
			}

			//for printing RQ/processes not sure where or how
			queue<ScreenProcess*> tempQueue = scheduler->readyQueue;
			while (!tempQueue.empty()) {
				ScreenProcess* process = tempQueue.front();
				//cout << process->getProcessName() << endl;
				cout << "process:   " << process->getProcessName() << "\t" <<
					"(" << "time" << ")\t" <<
					process->coreID << "\t" <<
					process->linesCompleted << " / " << process->numberOfProcess<< endl;

				tempQueue.pop();
			}
		}

		else if (main_command == "report-util");
		else if (main_command == "clear");

		else if (main_command == "exit") 
			return false;
		
		//! ----------FOR DEBUGGING ONLY --------------------------------

		else if (main_command == "runProcess") {
			string processName  = tokenizedInput[1];
			ScreenProcess* foundProcess = nullptr;
			for (auto element : processList) {
				if (element->getProcessName() == processName) {
					foundProcess = element;
				}
			}
			if (foundProcess != nullptr) {
				//foundProcess->start();
				while (!foundProcess->isFinished){
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
		
		
		return true;
}

