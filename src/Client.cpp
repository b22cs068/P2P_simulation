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

vector<int> Server::generate_array()
{
    const int size = 100;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 1000);

    std::vector<iint> randomVector(size);
    for (int &num :randomVector)
    {
        num = dist(gen);
    }
    return randomVector;

}

vector<Server*> Server:: randomlySelectServers()
{
    std::vector<Server*>  allServers = Server::getAllServers();
    std::vector<Server*> selectedServers;
    int numselected = allServers.size()/2 +1;
    std:: shuffle (allServers.begin(), allServers.end(), std::mt19937(std::random_device()()));
    selectedServers.assign(allServers.begin(), allServers.begin()+ numSelected);
    return selectedServers;
}
void divideandassigntask()
{

}
