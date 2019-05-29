//Liudmila Zyrianova
//CSCI 340
//05/12/2019
//Programming Assignment


#ifndef CSCI340_OS_DRIVE_H
#define CSCI340_OS_DRIVE_H

#include <iostream>
#include <vector>
#include <deque>
#include "cpu.h"
#include "ready_queue.h"

class drive {
private:
    std::vector<std::deque<std::pair<int, std::string>>> d;   //First process in queue uses disk, rest are in I/O queue. Pair - pid and file_name
public:
    explicit drive(int num){    //Create drive with (num) number of pages
        for (int i = 0; i< num; i++){
            std::deque<std::pair<int, std::string>> temp;
            d.push_back(temp);
        }
    }

    void request(int dnum, int pid, std::string& filename, cpu& CPU, ready_queue& RQ){  //Process wants to use certain disk with certain file
        if ((dnum > d.size()-1) || (dnum < 0)) {
            std:: cout<< "Error! Incorrect drive number!\n";
            return;
        }
        CPU.set(RQ.toCPU());    //Remove process from CPU
        std::pair<int, std::string> temp = {pid, filename};
        d[dnum].push_back(temp);    //Add process to I/O queue
    }

    void remove(int pid){   //Remove process from all disks
        for (int i = 0; i< d.size(); i++){
            if (!d[i].empty()) {
                for (auto it = d[i].begin(); it!=d[i].end(); ++it) {
                    if ((*it).first == pid) {
                        d[i].erase(it);
                        break;
                    }
                }
            }
        }
    }

    void done(int dnum, cpu& CPU, ready_queue& RQ){ //Drive with (dnum) number finishes its work.
        if ((dnum > d.size()-1) || (dnum < 0)) {
            std:: cout<< "Error! Incorrect drive number!\n";
            return;
        }
        if (d[dnum].size()==0){
            std:: cout<< "Error! This drive is idle!\n";
            return;
        }
        int poppedPID = d[dnum].front().first;
        d[dnum].pop_front();
        RQ.toEndOfRQ(poppedPID);    //Push this process to Ready-Queue
        if (CPU.isIdle()){          //If CPU is idle, put this process in CPU
            CPU.set(RQ.toCPU());
        }
    }

    void showDrive() const {   //Cout the drive with all disks
        for (int i = 0; i< d.size(); i++){
            if (d[i].size()>0){
                std::cout<<"Drive "<<i<< " is used by process " <<d[i][0].first<<" with file "<<d[i][0].second<<" . ";
                for (int j = 1; j <d[i].size(); j++){
                    std::cout<< " <- (process " <<d[i][j].first<<" with file "<<d[i][j].second<< ") ";
                }
                std::cout<< "\n";
            } else {
                std::cout<<"Drive "<<i<< " is idle ";
                std::cout<< "\n";
            }

        }
    }
};


#endif //CSCI340_OS_DRIVE_H
