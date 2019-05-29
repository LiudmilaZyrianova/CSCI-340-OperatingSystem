//Liudmila Zyrianova
//CSCI 340
//05/12/2019
//Programming Assignment

#ifndef CSCI340_OS_READY_QUEUE_H
#define CSCI340_OS_READY_QUEUE_H

#include <iostream>
#include <deque>


class ready_queue {
private:
    std::deque<int> queue;
public:
    void toEndOfRQ (int pid) {  //Pushes process to the end of Ready-Queue
        queue.push_back(pid);
    }

    void remove(int pid){   //Remove process from R/Q
        for (auto it = queue.begin(); it!=queue.end(); ++it)
            if (*it == pid) {
                queue.erase(it);
                break;
            }

    }

    int toCPU () {  //Pops front pid to put it in CPU. If idle, returns 1
        if (queue.size()>0){
            int poppedPID = queue.front();
            queue.pop_front();
            return poppedPID;
        } else {
            return 1;
        }

    }

    void showReadyQueue() const {  //Cout Ready-Queue
        std::cout << "Ready-queue: ";
        for (int i= 0 ; i <queue.size(); i++){
            std::cout<< " <- "<< queue[i];
        }
        std::cout<< "\n";
    }
};

#endif //CSCI340_OS_READY_QUEUE_H
