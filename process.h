//Liudmila Zyrianova
//CSCI 340
//05/12/2019
//Programming Assignment

#ifndef CSCI340_OS_PROCESS_H
#define CSCI340_OS_PROCESS_H


#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "cpu.h"
#include "ready_queue.h"
#include "drive.h"
#include "ram.h"


class process {
private:
    int pid;
    int parent;
    std::vector<int> children;
public:
    explicit process (int npid, cpu& CPU, ready_queue& RQ, std::unordered_map<int,process>& allProcesses, int nparent = 1 ): pid (npid), parent (nparent) {
        //Creates new process. By default, its parent is 1.
        if (CPU.isIdle()){  //If CPU is idle, put this process in CPU
            CPU.set(npid);
        }
        else {  //If CPU is not idle, put this process to the end of Ready-Queue
            RQ.toEndOfRQ(npid);
        }
        if (nparent != 1) {     //If parent is not 1, add newly created pid to its parent
            allProcesses.at(nparent).addChild(npid);
        }
        std:: pair<int, process> pair1 (npid, *this);
        allProcesses.emplace(pair1);    //Add this process to allProcesses

    };

    int getID(){    //returns PID of the process
        return pid;
    }

    void addChild (int childid) {   //Adds children
        children.push_back(childid);
    }

    void removeChild (int childid) {    //Removes child
        for (auto it = children.begin(); it != children.end(); ++it){
            if (*it == childid){
                children.erase(it);
                break;
            }
        }
    }

    void quit(cpu& CPU, ready_queue& RQ){   //Quit by moving to the end of Ready-Queue
        RQ.toEndOfRQ(pid);
        CPU.set(RQ.toCPU());
    }

    void terminateProcess(cpu& CPU, ready_queue& RQ, std::unordered_map<int,process>& allProcesses, std::unordered_set<int>& zombies, std::unordered_set<int>& wait, ram& R, drive& D){
        // Erases process from everywhere
        if (CPU.getPID() == this->pid) {
            CPU.set(RQ.toCPU());
        }
        R.release(this->pid);   //Release memory
        RQ.remove(this->pid);
        wait.erase(this->pid);
        D.remove(this->pid);
        allProcesses.erase(this->pid);
    }

    void cascadingTermination(cpu& CPU, ready_queue& RQ, std::unordered_map<int,process>& allProcesses, std::unordered_set<int>& zombies, std::unordered_set<int>& wait, ram& R, drive& D) {
        //Cascading termination
        for (int i = 0; i< children.size(); i++){
            if ( zombies.count(children[i]) != 0 ) {    //Child is already a zombie
                zombies.erase(children[i]); //Remove it form zombies
            } else {    //If child is not a zombie
                allProcesses.at(children[i]).cascadingTermination(CPU, RQ, allProcesses, zombies, wait, R, D);    //Do cascadingTermination for all its children
            }
        }
        this->terminateProcess(CPU, RQ, allProcesses, zombies, wait,R, D); //Finally, terminate process itself
    }

    void exit(cpu& CPU, ready_queue& RQ, std::unordered_map<int,process>& allProcesses, std::unordered_set<int>& zombies, std::unordered_set<int>& wait, ram& R, drive& D ){
        if (parent == 1){    //If parent is 1
            this->cascadingTermination(CPU, RQ, allProcesses, zombies, wait, R, D);
        } else {    //If parent is not 1
            if ( wait.count(parent) == 0 ) {    //Parent hasn't called wait yet
                zombies.emplace(this->pid);     //Turn this process into zombie
                this->cascadingTermination(CPU, RQ, allProcesses, zombies, wait, R, D);     //And terminates everything except its pid
            } else{ //Parent has already called wait
                allProcesses.at(this->parent).removeChild(this->pid);   //Remove this PID from it's parent children
                wait.erase(this->parent);   //Remove it's parent PID from wait
                RQ.toEndOfRQ(this->parent); // Put parent in ready-queue
                if (CPU.isIdle()){
                    CPU.set(RQ.toCPU());
                }
                this->cascadingTermination(CPU, RQ, allProcesses, zombies, wait, R, D); //Terminate process
            };
        }

    }

    void wait(cpu& CPU, ready_queue& RQ, std::unordered_map<int,process>& allProcesses, std::unordered_set<int>& zombies, std::unordered_set<int>& wait ){
        //Process in waitinf stage
        for (int i = 0; i< children.size(); i++) {
            if (zombies.count(children[i]) != 0) {  //Zombie-child exists
                zombies.erase(children[i]); //Zombie-child disappears; process keeps using CPU
                this->removeChild(children[i]); //Removes this zombie from parent's children
                return;
            }
        }
        //No zombie-children => wait
        wait.emplace(this->pid);
        CPU.set(RQ.toCPU());
    }

    void requestDisk(int dnum, std::string& filename,drive& D, cpu& CPU, ready_queue& RQ){  //Process wants to use a disk
        D.request(dnum, this->pid, filename, CPU, RQ);
    }

    void requestRam(int memory, int time, ram& R){  //Process wants to use RAM
        R.request(this->pid, memory, time);
    }

    void showProcess() const { //Cout process
        std::cout << "PID: "<< pid << "     ";
        std::cout << "Parent: "<< parent << "     ";
        std::cout << "Children: ";
        for (int i= 0 ; i <children.size(); i++){
            std::cout<< children[i]<< " ";
        }
        std::cout<< "\n";
    }

};


#endif //CSCI340_OS_PROCESS_H
