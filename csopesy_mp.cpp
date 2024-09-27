//our functions
#include "Headers/Utils.h"
//import all libraries
#include "Headers/stdc++.h"

using namespace std;


class ScreenProcess;

void printActiveProcesses(vector <ScreenProcess> processList);


//------------------------------------------

class ScreenProcess {
private:
	string processName;
	string timeMade;
	int linesCompleted;

public:
	ScreenProcess(const string& name) {
		processName = name;
		timeMade = getTime();
		linesCompleted = 0;
	}
	void showProcessInfo() {
		cout << "Process Name: " << processName << endl;
		cout << "Time Created: " << timeMade << endl;
		cout << "Lines Completed: " << linesCompleted << endl;
	}
	void open() {
		//takeOver
		showProcessInfo();

		string input;
		vector <ScreenProcess> processList;
		while (true) {
			cout << "INSIDE_"<<processName <<">";
			getline(cin, input);
			vector<string> tokenizedInput = splitInput(input);
			string temp_command = tokenizedInput[0];

			//----------------------------------------------
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
							showProcessInfo();
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
					showProcessInfo();
				}
				else if (temp_arg == "-ls") {
					printActiveProcesses(processList);
				}
			}
			else if (temp_command == "clear") {
				system("cls");
			}
			else {
				if (!handleInput(input)) { break; }
			}
			//----------------------------------------------
		}

	}
	
	string getProcessName(){
		return processName;
	}
};


void printActiveProcesses(vector <ScreenProcess> processList){
	cout << ESC << YELLOW_TXT << "------------------------------" << RESET << endl;
	cout << processList.size() << " ACTIVE PROCESSES" << endl;  // Print vector size

	for (auto element : processList) {
		element.showProcessInfo();
		cout << endl;
	}
	cout << ESC << YELLOW_TXT << "------------------------------" << RESET << endl;
}

int main() {

	string input;
	printHeader();

	vector <ScreenProcess> processList;
	while (true) {
		cout << "Enter a command: ";
		getline(cin, input);

		vector<string> tokenizedInput = splitInput(input);
		//splits the input by space and puts it in vector

		// DUBUGGING: prints all elements of a vector
		//printVector(tokenizedInput);

		//LAZY IMPLEMENTATION
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
			if (!handleInput(input)) { return 0; }
		}



	}
}