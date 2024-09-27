#pragma once

#define ESC "\033["

#define YELLOW_TXT "93m"
#define WHITE_TXT "37m"
#define GREEN_TXT "92m"

#define RESET "\033[m"


#include "stdc++.h"

using namespace std;

void printHello() {
	cout << "Hellow" << endl;
}


void printHeader() {

    string header = R"(OObtained from: https://patorjk.com/software/taag/#p=display&f=Jazmine&t=CSOPESY
    .oPYo. .oPYo. .oPYo.  .oPYo. .oPYo. .oPYo. o   o 
    8    8 8      8    8  8    8 8.     8      `b d' 
    8      `Yooo. 8    8 o8YooP' `boo   `Yooo.  `b'  
    8          `8 8    8  8      .P         `8   8   
    8    8      8 8    8  8      8           8   8   
    `YooP' `YooP' `YooP'  8      `YooP' `YooP'   8   
    :.....::.....::.....::..::::::.....::.....:::..::)";

    cout << ESC << header << endl;

    cout << ESC << GREEN_TXT << "Hello, Welcome to CSOPESY commandline!" << RESET << endl;

    cout << ESC << YELLOW_TXT << "Type 'exit' to quit, 'clear' to clear the screen" << RESET << endl;

}

vector<string> splitInput(string input) {

    vector<string> tokens;


    stringstream ss(input);
    string temp;

    //split by spaces
    while (ss >> temp) {
        //add to vector
        tokens.push_back(temp);
    }
    return tokens;
}

void printVector(vector<string> vec) {
    cout << ESC << YELLOW_TXT << "------------------------------" << RESET <<endl;
    cout << "Vector size: " << vec.size() << endl;  // Print vector size
    cout << "Vector elements:" << endl;

    for (auto element : vec) { 
        cout << "  " << element << endl;
    }
    cout << ESC << YELLOW_TXT << "------------------------------" << RESET << endl;
}

string getTime() {
    // current time
    time_t t = time(nullptr);
    tm now;
    localtime_s(&now, &t);
    // format
    char buffer[100];
    strftime(buffer, sizeof(buffer), "%m/%d/%Y, %I:%M:%S %p", &now);

    return string(buffer);
}

bool handleInput(string input) {

    if (input == "initialize")
        cout << "initialize command recognized. Doing something." << endl;

    else if (input == "screen")
        cout << "screen command recognized. Doing something." << endl;

    else if (input == "scheduler-test")
        cout << "scheduler-test command recognized. Doing something." << endl;

    else if (input == "scheduler-stop")
        cout << "scheduler-stop command recognized. Doing something." << endl;

    else if (input == "report-util")
        cout << "report-util command recognized. Doing something." << endl;

    else if (input == "clear") {
        cout << "clear command recognized. Doing something." << endl;
        system("cls");
        printHeader();
    }

    else if (input == "exit") {
        cout << "exit command recognized. Doing something." << endl;
        return false;
    }

    else {
        cout << ESC << GREEN_TXT;
        cout << input << " command unrecognized" << endl;
        cout << RESET;
    }
    return true;
}