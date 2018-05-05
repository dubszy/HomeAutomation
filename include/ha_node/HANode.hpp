#include <netinet/in.h>

#include <types/ip_types.h>
#include <logger/c++/Logger.hpp>
#include <vector>

#include <Properties.hpp>

class HANode {

public:
    HANode(Properties *properties, vector<IPAddress> permittedClients);
    ~HANode();

    int startServer();
    void stopServer();
    bool isStopping();
    int waitForRequest();
    int processRequest(in_addr_t client_addr);

    int identifyResponse();
    int stopResponse();
    int errorResponse(int statusCode, const char *statusMessage);
    int response(char *buf);

    NetAddr getNetAddr();
    const char *getOS();
    const char *getDeviceType();
    const char *getDeviceName();

private:
    HANode(const HANode&);
    HANode& operator=(const HANode&);

    bool shouldStop = false;

    uint16_t port;
    int sockfd;
    int client_sockfd;
    vector<IPAddress> permittedClients;
    Properties *properties;
};
