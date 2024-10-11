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

		vector<string> tokenizedInput = splitInput(input);

		string temp_command = tokenizedInput[0];

		//override screen input handling
		if (temp_command == "screen") {
			if (tokenizedInput.size() == 1) {
				cout << "no arguments provided please use -r or -s" << endl;
				continue;
			}
			string temp_arg = tokenizedInput[1];
			if (temp_arg == "-r") {
				if (tokenizedInput.size() < 3) {
					cout << "no arguments after using -r" << endl;
					continue;
				}
				//reattach
				for (auto element : processList) {
					if (element.getProcessName() == tokenizedInput[2]) {
						system("cls");
						element.open();
						system("cls");
						printHeader();
					}
				}
			}
			else if (temp_arg == "-s") {
				//create new process
				if (tokenizedInput.size() < 3) {
					cout << "no arguments after using -s" << endl;
					continue;
				}
				ScreenProcess newScreen(tokenizedInput[2]);
				processList.push_back(newScreen);
				cout << "new process added!" << endl;
				system("cls");
				newScreen.open();
				system("cls");
				printHeader();

			}
			else if (temp_arg == "-ls") {
				printActiveProcesses(processList);
			}
		}
		else {
			if (!handleInput(input)) { return; }
		}

	}





}


void ScreenProcess::printActiveProcesses(vector <ScreenProcess> processList) {
	cout << ESC << YELLOW_TXT << "------------------------------" << RESET << endl;
	cout << processList.size() << " ACTIVE PROCESSES" << endl;  // Print vector size

	for (auto element : processList) {
		element.showProcessInfo();
		cout << endl;
	}
	cout << ESC << YELLOW_TXT << "------------------------------" << RESET << endl;
}
