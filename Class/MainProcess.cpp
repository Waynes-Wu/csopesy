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
		scheduler->generateProcesses(CPUCOUNTER, processList);




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
				newScreen->isRunning = true;
				system("cls");
				newScreen->open();
				system("cls");
				printHeader();
			}
			else if (arg_command == "-ls") {

				//queue<ScreenProcess*> tempQueue = scheduler->readyQueue;
				//while (!tempQueue.empty()) {
				//	ScreenProcess* process = tempQueue.front();
				//	//cout << process->getProcessName() << endl;
				//	cout << "process:   " << process->getProcessName() << "\t" <<
				//		"(" << "time" << ")\t" <<
				//		process->coreID << "\t" <<
				//		process->linesCompleted << " / " << process->numberOfProcess << endl;

				//	tempQueue.pop();
				//}

				printActiveProcesses();
			}
		}
		else if (main_command == "scheduler-test") {
			scheduler->generate = true;
		}
		else if (main_command == "scheduler-stop") {
			scheduler->generate = false;
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

