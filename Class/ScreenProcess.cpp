#include "../Headers/ScreenProcess.h"

ScreenProcess::ScreenProcess(string name) {
	processName = name;
	timeMade = getTime();
	linesCompleted = 0;
}

string ScreenProcess::getProcessName() {
	return processName;
}

void ScreenProcess::showProcessInfo() {
	cout << "Process Name: " << processName << endl;
	cout << "Time Created: " << timeMade << endl;
	cout << "Lines Completed: " << linesCompleted << endl;
}

void ScreenProcess::open() {

	showProcessInfo();


	while (true) {
		cout << "INSIDE_" << processName << "> ";
		getline(cin, input);
		if (!inputChecker(input))
			break;
	}

}


void ScreenProcess::printActiveProcesses() {
	cout << ESC << YELLOW_TXT << "------------------------------" << RESET << endl;
	cout << processList.size() << " ACTIVE PROCESSES" << endl;  // Print vector size

	for (auto element : processList) {
		element.showProcessInfo();
		cout << endl;
	}
	cout << ESC << YELLOW_TXT << "------------------------------" << RESET << endl;
}


bool ScreenProcess::inputChecker(string& input) {
	vector<string> tokenizedInput = splitInput(input);

	int commandCount = tokenizedInput.size();
	string main_command = tokenizedInput[0];
	
	if (main_command == "screen") {
		if (commandCount == 1) { cout << "use -r -s or -ls" << endl; return true; }
		string arg_command = tokenizedInput[1];
		//RE-ATTACH TO THE GIVEN THINGY
		if (arg_command == "-r") {
			if (commandCount < 3) { cout << "no arguments after using -r" << endl; return true; }
			bool isFound = false;
			for (auto element : processList) {
				if (element.getProcessName() == tokenizedInput[2]) {
					isFound = true;
					system("cls");
					element.open();
					system("cls");
					printHeader();
				}
			}
			if (!isFound) {
				cout << tokenizedInput[2] << " not found" << endl;;
			}
		}
		else if (arg_command == "-s") {
			// CREATE NEW PROCESS
			if (commandCount < 3) { cout << "no arguments after using -s" << endl; }
			ScreenProcess newScreen(tokenizedInput[2]);
			processList.push_back(newScreen);
			cout << "new process added!" << endl;
			system("cls");
			newScreen.open();
			system("cls");
			printHeader();
		}
		else if (arg_command == "-ls") {
			printActiveProcesses();
		}
	}
	else if (main_command == "scheduler-test");
	else if (main_command == "scheduler-stop");
	else if (main_command == "report-util");
	else if (main_command == "clear");

	else if (main_command == "exit")
		return false;

	else {
		cout << ESC << GREEN_TXT;
		cout << input << " command unrecognized" << endl;
		cout << RESET;
	}


	return true;
}