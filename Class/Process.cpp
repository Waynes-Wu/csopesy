#include "../Headers/Process.h"
#include "../Headers/stdc++.h"


Process::Process(int id, int steps) : pid(id), totalSteps(steps), currentStep(0), isRunning(false), isFinished(false), coreID(-1) {}

void Process::start(int assignedCoreID) {
    coreID = assignedCoreID;
    isRunning = true;
    startTime = getTime();

    //if possible you could link this to runStep have it thread?
}

void Process::stop() {
    coreID = -1;
    isRunning = false;
    finishTime = getTime();

}

void Process::runStep() {
    if (currentStep < totalSteps && isRunning) {
        currentStep++;
        //doSomething like
        //cout << executed at getTime()
        //maybe write in file or whatever
    }
    else finish();
}

void Process::finish() {
    //confirm if its finished
    if (currentStep == totalSteps) {
        stop();
        isFinished = true;
    }
}

void Process::printStatus(){
    string displayTime;
    //displayTime = getTime();  //if current time
    //displayTime = startTime;  //if started running/resume

    string displayCore;
    displayCore = (isFinished) ? "Finished" : ("Core: " + to_string(coreID));

    cout << "process" << pid << "\t" <<
        "(" << displayTime << ")\t" << 
        displayCore << "\t" << 
        currentStep << " / " << totalSteps;
};