#include <omnetpp.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace omnetpp;
Define_Module(Server);

class MaliciousServerManager {
private:
    int totalServers;
    int maxMaliciousServers;
    std::vector<bool> maliciousStatus;

public:
    MaliciousServerManager(int total_servers) :
        totalServers(total_servers),
        maxMaliciousServers(total_servers / 4)
    {
        // Initialize malicious server selection
        selectMaliciousServers();
    }

    void selectMaliciousServers() {
        // Reset previous status
        maliciousStatus.clear();
        maliciousStatus.resize(totalServers, false);

        // Randomly select servers to be malicious
        std::srand(std::time(nullptr));
        int maliciousCount = 0;

        for (int i = 0; i < totalServers && maliciousCount < maxMaliciousServers; ++i) {
            // Probabilistic selection with constraint
            if (maliciousCount < maxMaliciousServers && std::rand() % 2 == 0) {
                maliciousStatus[i] = true;
                maliciousCount++;
            }
        }

        // If not enough malicious servers selected,
        // forcibly mark some as malicious
//        while (maliciousCount < maxMaliciousServers) {
//            int index = std::rand() % totalServers;
//            if (!maliciousStatus[index]) {
//                maliciousStatus[index] = true;
//                maliciousCount++;
//            }
//        }
    }

    bool isMalicious(int serverId) const {
        if (serverId < 0 || serverId >= totalServers) {
            return false;
        }
        return maliciousStatus[serverId];
    }

    int getMaliciousCount() const {
        return std::count(maliciousStatus.begin(), maliciousStatus.end(), true);
    }
};

class Server : public cSimpleModule
{
private:
    std::ofstream logFile;
    std::string ip;
    bool isMalicious;
    int counter;
    int curr_counter;
    int totalServers;

    // Static instance of malicious server manager
    static MaliciousServerManager* maliciousManager;

    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

    int calculateSubArraySum(const std::vector<int>& arr);
    int introduceRandomError(int originalSum);
    void logMessage(const std::string& message);
    //Global LIst for storing all the servers
    static std::vector<Server*> allServers;
public:
    Server() {allServers.push_back(this);} // Default constructor
    Server(int n, int total_servers);
    static const std::vector<Server*>& getAllServers();

};

// Static member initialization
MaliciousServerManager* Server::maliciousManager = nullptr;

Define_Module(Server);

Server::Server(int n, int total_servers)
{
    // Ensure malicious server manager is created only once
    if (!maliciousManager) {
        maliciousManager = new MaliciousServerManager(total_servers);
    }

    // Determine malicious status based on server ID
    isMalicious = maliciousManager->isMalicious(getId());

    counter = n; // determines the frequency of giving an honest answer
    curr_counter = n; // act as the current counter
    totalServers = total_servers;
}

void Server::initialize()
{
    // Use module's full name or ID as identifier
    ip = std::to_string(getId());

    // Open log file
    std::string filename = "server_" + ip + ".log";
    logFile.open(filename);

    if (!logFile.is_open()) {
        EV << "Error opening the log file: " << filename << endl;
    } else {
        EV << "Log file opened: " << filename << endl;

        // Log malicious status and global malicious server count
        logMessage("Server initialized. Malicious status: " +
                   std::string(isMalicious ? "Yes" : "No") +
                   ". Total Malicious Servers: " +
                   std::to_string(maliciousManager->getMaliciousCount()));
    }
}

void Server::handleMessage(cMessage *msg)
{
    // Cast and validate message
    cPacket *pkt = check_and_cast<cPacket*>(msg);

    try {
        // Extract client ID and data
        int clientId = pkt->par("clientId").intValue();
        const char *dataStr = pkt->par("data").stringValue();

        // Parse subarray
        std::vector<int> arr;
        std::stringstream ss(dataStr);
        std::string item;

        while (std::getline(ss, item, ',')) {
            arr.push_back(std::stoi(item));
        }

        // Calculate result
        int result = calculateSubArraySum(arr);

        // Potentially introduce error if malicious
        if (isMalicious) {
            if(curr_counter == 1){
                result = introduceRandomError(result);
                            logMessage("Acting maliciously. Altered result.");
                            curr_counter = counter;
            }else{
                curr_counter--;
            }

        }

        // Log processing details
        logMessage("Received from client " + std::to_string(clientId) +
                   " | Data: " + dataStr +
                   " | Computed Sum: " + std::to_string(result));

        // Prepare reply
        cPacket *reply = new cPacket("SubarrayResult");
        reply->addPar("result") = result;
        reply->addPar("clientId") = clientId;
        reply->addPar("serverId") = getId();

        // Send reply back to specific client gate
        send(reply, "out", clientId);
    }
    catch (const std::exception& e) {
        logMessage("Error processing message: " + std::string(e.what()));
    }

    // Clean up
    delete msg;
}

int Server::calculateSubArraySum(const std::vector<int>& arr)
{
    int sum = 0;
    for (int val : arr) {
        sum += val;
    }
    return sum;
}

int Server::introduceRandomError(int originalSum)
{
    // Introduce a small random error when malicious
    int error = (std::rand() % 10) - 5;  // Random perturbation between -5 and 5
    return originalSum + error;
}

void Server::logMessage(const std::string& message)
{
    if (logFile.is_open()) {
        logFile << "[Server " << ip << "] " << message << std::endl;
    }
    EV << "[Server " << ip << "] " << message << endl;
}

void Server::finish()
{
    logMessage("Simulation finished");

    if (logFile.is_open()) {
        logFile.close();
    }

    // Optional: Clean up static manager at simulation end
    if (maliciousManager) {
        delete maliciousManager;
        maliciousManager = nullptr;
    }
}

const std::vector<Server*> & Server:: getAllServers()
{
    return allServers;
}











































