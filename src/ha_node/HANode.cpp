#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <utility>

#include <Properties.hpp>
#include "types.h"
#include "HANode.hpp"

static Logger *log = Logger::forClass<HANode>(LogLevelWarning);

static HANode *haNode = new HANode(new Properties("/etc/homeautomation/ha_node/default.properties"),
                                   vector<IPAddress>(1, {10, 0, 0, 0}));

HANode::HANode(Properties *properties, vector<IPAddress> permittedClients) {
    this->port = properties->get("ha_node.port")->valueAsNumber();
    this->permittedClients = std::move(permittedClients);
}

HANode::~HANode() {
    delete properties;
}

int HANode::startServer() {
    struct sockaddr_in server_addr {};

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        log->fatal("Failed to open socket");
        stopServer();
        return -1;
    }
    bzero((char*) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr));

    if (listen(sockfd, 10) < 0) {
        log->fatal("Failed to start listening on socket");
        stopServer();
        return -1;
    }
    return 0;
}

void HANode::stopServer() {
    if (client_sockfd) {
        close(client_sockfd);
    }
    if (sockfd) {
        close(sockfd);
    }
}

bool HANode::isStopping() {
    return shouldStop;
}

int HANode::waitForRequest() {
    struct sockaddr_in client_addr {};
    socklen_t client_len = sizeof(client_addr);
    char client_addr_str[INET_ADDRSTRLEN];

    if ((client_sockfd = accept(sockfd, (struct sockaddr *) &client_addr, &client_len)) < 0) {
        log->error("Failed to accept request from client: %s",
                   inet_ntop(AF_INET, &(client_addr.sin_addr), client_addr_str, INET_ADDRSTRLEN));
        return -1;
    } else {
        return processRequest(client_addr.sin_addr.s_addr);
    }
}

int HANode::processRequest(in_addr_t client_addr) {
    char input_buf[2048];

    for (auto permittedClient : permittedClients) {
        if (ipton(permittedClient) == client_addr) {
            bzero(input_buf, 2048);
            if (read(client_sockfd, input_buf, 2047) < 0) {
                log->error("Error reading from socket");
                break;
            }
            switch (input_buf[0]) {
                case RequestTypeIdentify:
                    return identifyResponse(); // closes client_sockfd
                case RequestTypeStop:
                    return stopResponse();
                default:
                    return errorResponse(400, "Unsupported request type"); // closes client_sockfd
            }
        }
    }
    return errorResponse(403, "Forbidden");
}

int HANode::identifyResponse() {
    char buf[2048];
    sprintf(buf,
            "{success: true, statusCode: 200, statusLine: OK, {netAddr: %d, os: %s, deviceType: %s, deviceName: %s}}",
            getNetAddr(),
            getOS(),
            getDeviceType(),
            getDeviceName());

    return response(buf);
}

int HANode::stopResponse() {
    int res = response(const_cast<char *>("Stopping server"));
    shouldStop = true;
    return res;
}

int HANode::errorResponse(int statusCode, const char *statusMessage) {
    char res[2048];
    sprintf(res, "{success: false, statusCode: %d, statusLine: %s}", statusCode, statusMessage);
    return response(res);
}

int HANode::response(char *buf) {
    int res = 0;

    if (write(client_sockfd, buf, sizeof(buf)) < 0) {
        log->error("Error writing buffer to socket");
        log->info("Buffer contents: ", buf);
        res = 500;
    } else {
        res = 200;
    }
    close(client_sockfd);
    return res;
}

NetAddr HANode::getNetAddr() {
    return properties->get("ha_node.net_addr")->valueAsNumber();
}

const char *HANode::getOS() {
    return properties->get("ha_node.os")->value().c_str();
}

const char *HANode::getDeviceType() {
    return properties->get("ha_node.device.type")->value().c_str();
}

const char *HANode::getDeviceName() {
    return properties->get("ha_node.device.name")->value().c_str();
}

void quit() {
    haNode->stopServer();
    delete haNode;
    exit(0);
}

void handle_sigint(int sig) {
    signal(sig, SIG_IGN);
    quit();
}

int main(int argc, char *argv[]) {
    // Need to properly handle SIGINT in case we need to close any socket connections
    signal(SIGINT, handle_sigint);

    haNode->startServer();

    while (!haNode->isStopping()) {
        // Blocks until a request comes though,
        // then proceeds though processing and issuing a response
        haNode->waitForRequest();
    }
    quit();
}
