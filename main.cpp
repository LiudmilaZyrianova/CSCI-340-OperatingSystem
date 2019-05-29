//Liudmila Zyrianova
//CSCI 340
//05/12/2019
//Programming Assignment

#include <iostream>
#include "process.h"
#include "tools.h"
#include "drive.h"
#include <regex>
#include <unordered_map>
#include <unordered_set>


void showEverything(const timer& T, const ram& R, const drive& D, const cpu& CPU, const ready_queue& RQ, const std::unordered_map<int,process>& allProcesses, const std::unordered_set<int>& zombies, const std::unordered_set<int>& wait ){
    //Command "show all" shows the full picture of what is going on in simulation
    for (auto& x: allProcesses) {x.second.showProcess();}
    std::cout << "Zombies: ";
    for (auto& x: zombies) {std::cout << " " << x;}
    std::cout << "\n";
    std::cout << "Wait: ";
    for (auto& x: wait) {std::cout << " " << x;}
    std::cout << "\n";
    CPU.showCPU();
    RQ.showReadyQueue();
    D.showDrive();
    R.showRam();
    T.showTime();
    std::cout << "\n";
}

int main() {
    long long ramSize;
    std::cout << "Type Ram size: ";
    std::cin>>ramSize;
    int pageSize;
    std::cout << "Type page/frame size: ";
    std::cin>>pageSize;
    int numDisks;
    std::cout << "Type the number of hard disks: ";
    std::cin>>numDisks;
    cpu CPU;
    ready_queue RQ;
    drive D(numDisks);
    ram R(ramSize,pageSize);
    timer T;
    newpid PID;
    std::unordered_map<int,process> allProcesses;
    std::unordered_set<int> zombies;
    std::unordered_set<int> wait;


    //Read commands
    std::string command = "start";
    int commandCounter = 0;
    while(command != "QUIT")
    {
        if (commandCounter!=0) {std::cout << "Next command (type QUIT to quit): ";}
        std::getline(std::cin, command);
        commandCounter++;
        if (command == "S r") { //"S r"
            CPU.showCPU();
            RQ.showReadyQueue();
        }
        else if (command == "S i") D.showDrive(); //"S i"
        else if (command == "S m") R.showRam(); //"S m"
        else if (command == "show all") showEverything(T, R, D, CPU, RQ, allProcesses, zombies, wait);   //Shows everything
        else if (std::regex_match (command, std::regex("(m)(\\s)((\\d)+)") )) { //m address
            char char_array[command.length() + 1];
            strcpy(char_array, command.c_str());
            char *token = strtok(char_array, " ");
            token = strtok(NULL, " ");  //split the string to find address
            int address = std::atoi(token);
            allProcesses.at(CPU.getPID()).requestRam(address, T.useTime(), R);
        }
        else if (std::regex_match (command, std::regex("(D)(\\s)((\\d)+)") )) { //D number
            char char_array[command.length() + 1];
            strcpy(char_array, command.c_str());
            char *token = strtok(char_array, " ");
            token = strtok(NULL, " ");  //split the string to find number
            int number = std::atoi(token);
            D.done(number,CPU, RQ);

        }
        else if (std::regex_match (command, std::regex("(d)(\\s)((\\d)+)(\\s)(([(\\\\.)(\\w)])+)") )) { //d number file_name
            char char_array[command.length() + 1];
            strcpy(char_array, command.c_str());
            char *token = strtok(char_array, " ");
            token = strtok(NULL, " ");  //split the string to find number
            int number = std::atoi(token);
            token = strtok(NULL, " ");  //split the string to find file_name
            std::string file_name = token;
            allProcesses.at(CPU.getPID()).requestDisk(number, file_name, D, CPU, RQ);
        }
        else if (command == "wait") allProcesses.at(CPU.getPID()).wait(CPU, RQ, allProcesses, zombies, wait);   //"wait"
        else if (command == "exit") allProcesses.at(CPU.getPID()).exit(CPU, RQ, allProcesses, zombies, wait, R, D); //"exit"
        else if (command == "fork") process p0(PID.usePid(), CPU, RQ, allProcesses, CPU.getPID());  //"fork"
        else if (command == "Q") allProcesses.at(CPU.getPID()).quit(CPU, RQ);   //"Q"
        else if (command == "A") process p1(PID.usePid(), CPU, RQ, allProcesses);   //"A"
        else if (command == "QUIT") std::cout << "Goodbye!\n";  //"QUIT"
        else if (commandCounter != 1) std::cout << "Unknown command! Type again (there should be no space at the end of the command)!\n";

    };



    return 0;
}