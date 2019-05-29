//Liudmila Zyrianova
//CSCI 340
//05/12/2019
//Programming Assignment

#ifndef CSCI340_OS_CPU_H
#define CSCI340_OS_CPU_H

#include "ready_queue.h"
#include <iostream>

class cpu {
private:
    int runningProcess = 1; //CPU contains the pid of the running process
public:
    bool isIdle() { //Checks if CPU is idle
        if (runningProcess == 1)
            return true;
        else
            return false;
    }

    void set(int pid){  //Set CPU to run process (pid)
        runningProcess = pid;
    }

    int getPID(){   //Returns pid of running process
        return runningProcess;
    }

    void showCPU() const { //Cout CPU
        if (runningProcess == 1) std:: cout << "CPU is idle \n";
        else std:: cout << "CPU: "<< runningProcess << "\n";
    }
};


#endif //CSCI340_OS_CPU_H
