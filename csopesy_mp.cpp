#define ESC "\033["

#define YELLOW_TXT "93m"
#define WHITE_TXT "37m"
#define GREEN_TXT "92m"

#define RESET "\033[m"


#include <iostream>
#include <string>
using namespace std;


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

int main() {
    int endProgram = 0;
    string input;
    printHeader();
    while (!endProgram) {
        cout << "Enter a command: ";
        cin >> input;

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
            endProgram = !endProgram;
        }

        else {
            cout << ESC << GREEN_TXT;
            cout << input << " command unrecognized" << endl;
            cout << RESET;
        }
    }
}