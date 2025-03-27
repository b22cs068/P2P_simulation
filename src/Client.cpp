#include <omnetpp.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include "Server.h"

using namespace omnetpp;
Define_Module(Client);

class Client : public cSimpleModule
{
    public:
         Client() {};


};

vector<int> Client::generate_array()
{
    const int size = 100;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 1000);

    std::vector<int> randomVector(size);
    for (int &num :randomVector)
    {
        num = dist(gen);
    }
    return randomVector;

}

vector<Server*> Client:: randomlySelectServers()
{
    std::vector<Server*>  allServers = Server::getAllServers();
    std::vector<Server*> selectedServers;
    int numselected = allServers.size()/2 +1;
    std:: shuffle (allServers.begin(), allServers.end(), std::mt19937(std::random_device()()));
    selectedServers.assign(allServers.begin(), allServers.begin()+ numSelected);
    return selectedServers;
}
std:: vector<std::vector<int>> divideIntoSubtasks(const std::vector<int>& data, int n)
{
    std::vector<std::vector<int>> subtasks(n);
    int dataSize=data.size();

    if (n<=0 || dataSize==0) return subtasks;
    int subTaskSize = dataSize/n;

    int startIndex=0;
    int remainder= dataSize%n;
    for (int i=0; i<n; i++)
    {
        int currentSubtaskSize = subtaskSize + (i<remainder?1:0);
        subtasks[i]= std::vector<int>(data.begin()+startIndex, data.begin()+startIndex+currentSubtaskSize);
        startIndex+=subtaskSize();
    }
    return subtasks;

}

Client::sendSubtasks(vector<int> subtask)
{
    std:: vector<Server*> serversSubtask =randomlySelectedServers();


}
