//Liudmila Zyrianova
//CSCI 340
//05/12/2019
//Programming Assignment


#ifndef CSCI340_OS_RAM_H
#define CSCI340_OS_RAM_H


#include <iostream>
#include <vector>
#include <deque>
#include "cpu.h"
#include "ready_queue.h"

class ram {
private:
    int pageSize;
    std::vector<std::vector<int>> r;   //r[i] - RAM frame number i. It has three columns: 0)pid, 1)pageNum, 2)timestamp
public:
    explicit ram(long long memorySize, int page){   //Create RAM with (memorySize/page) number of pages
        std::vector<int> temp = { 0, 0, 0};
        pageSize = page;
        long frameNum = memorySize/page;
        for (long i = 0; i< frameNum; i++){
            r.push_back(temp);
        }
    }

    long getNeededFrame(int pid, int pageNum){  //When process requests memory slot, it find appropriate frame
        long processSlot = -1;
        long freeSlot = -1;
        int minTime = r[0][2];
        long slotWithMinTime = 0;
        for (long i = r.size()-1; i >-1 ; i--){
            if ((r[i][0] == pid)&&(r[i][1] == pageNum)){    //If process is already using ram with the same page
                processSlot = i;
            } else if (r[i][0] == 0) {  //If no process is already using this frame
                freeSlot = i;
            } else if (r[i][2] < minTime) {     //If someone is already using this frame, find LRU by finding minimal timestamp
                minTime = r[i][2];
                slotWithMinTime = i;
            }
        }
        if (processSlot != -1 ) return processSlot; //First choice - to substitute already existing process with the same page by updating timestamp
        else if (freeSlot != -1) return  freeSlot;  //Second choice - take free frame
        else return slotWithMinTime; //Third choice - take least recently used frame
    }

    void request(int pid, int mem, int time){
        int pageNum = (mem/pageSize);
        long frameNum = this->getNeededFrame(pid, pageNum); //Get appropriate frame
        //Update the frame
        r[frameNum][0] = pid;
        r[frameNum][1] = pageNum;
        r[frameNum][2] = time;
    }

    void release(int pid){      //Release the memory occupied by the process
        for (int i = 0; i < r.size(); i++){
            if (r[i][0] == pid){
                r[i][0] = 0;
                r[i][1] = 0;
                r[i][2] = 0;
            }
        }
    }

    void showRam() const { //Cout the Ram
        for (int i = 0; i< r.size(); i++){
            if (r[i][0] == 0) std::cout<<"RAM frame "<<i<< " is idle \n";
            else std::cout<<"RAM frame "<<i<< " is used by process " <<r[i][0]<<" with page "<<r[i][1]<<". Time: "<<r[i][2]<<". \n";
        }
    }
};


#endif //CSCI340_OS_RAM_H
