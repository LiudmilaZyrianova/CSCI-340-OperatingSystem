//Liudmila Zyrianova
//CSCI 340
//05/12/2019
//Programming Assignment

#ifndef CSCI340_OS_TOOLS_H
#define CSCI340_OS_TOOLS_H

#include <iostream>

class timer {   //Timer which is used as timestamp in RAM to determine LRU process
private:
    int time;
public:
    explicit timer(): time(-1){};

    int useTime(){
        time++;
        return time;
    }

    void showTime() const{
        std::cout<<"Time: "<<time<< " \n";
    }
};




class newpid {  //New PID - keeps track of PIDs of newly arrived processes.
private:
    int pid;
public:
    explicit newpid(): pid(1){};

    int usePid(){
        pid++;
        return pid;
    }

    void showPid(){
        std::cout<<"PID: "<<pid<< " \n";
    }
};


#endif //CSCI340_OS_TOOLS_H
