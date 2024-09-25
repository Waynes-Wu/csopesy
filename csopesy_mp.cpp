//our functions
#include "Utils.h"
//import all libraries
#include "stdc++.h"

using namespace std;

int main() {

    string input;
    printHeader();

    while (true) {
        cout << "Enter a command: ";
        getline(cin, input);

        //splits the input by space and puts it in vector
        vector<string> tokenizedInput = splitInput(input);

        // DUBUGGING: prints all elements of a vector
        printVector(tokenizedInput);

        if (!handleInput(input)) {
            // exit
            return 0;
        }

    }
}