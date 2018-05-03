#include <stdio.h>
#include <errno.h>
#include <sys/time.h>

#include <sys/param.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/file.h>

#include <netinet/in_systm.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>

#include <Logger.hpp>
#include <arpa/inet.h>
#include <zconf.h>

#include "IPDeviceManager.hpp"

IPDeviceManager *IPDeviceManager::getSharedIPDeviceManager() {
    if (sharedIPDeviceManager == NULL) {
        sharedIPDeviceManager = new IPDeviceManager(65536);
    }
    return sharedIPDeviceManager;
}

int IPDeviceManager::addDevice(IPDevice *device, NetAddr addr) {
    return DeviceManager::addDevice(device, addr);
}

int IPDeviceManager::removeDevice(NetAddr addr) {
    return DeviceManager::removeDevice(addr);
}

IPDevice *IPDeviceManager::getDevice(NetAddr addr) {
    return (IPDevice *)DeviceManager::getDevice(addr);
}

vector<IPDeviceMap> IPDeviceManager::discoverDevicesOnLAN(IPAddress netmask) {
    vector<IPDeviceMap> lanDevices = vector<IPDeviceMap>(1024);

    return lanDevices;
}

PingStats IPDeviceManager::ping(const char *who, int pingCount, int dataLength, int packetAmount, int preloadAmount,
                                bool socketDebug, bool socketDontRoute, bool flood) {

    PingStats pingStats;

    u_char packet[PING_MAX_PACKET];         // Storage for data sent per ping

    int socket_fd;                          // FD for socket
    struct hostent *pHostInfo;              // Host info
    struct timezone tz;                     // Current timezone

    struct sockaddr whereto;                // Where to send this ping to

    char *hostname;                         // Hostname
    char hostnameBuffer[MAXHOSTNAMELEN];    // Hostname buffer (for copying)

    int txPacketAmount;                     // Transmitted packet amount
    int rxPacketAmount;                     // Received packet amount
    bool useTiming = false;                 // Should the packets be timed?
    int timeMin = 999999999;                // Minimum time
    int timeMax;                            // Maximum time
    int timeSum;                            // Sum of all times

    int ident;                              // Process ID

    struct sockaddr_in from;                // Who this ping is from
    struct sockaddr_in *to                  // Who this ping is to
            = (struct sockaddr_in *) &whereto;
    struct protoent *proto;                 // Protocol (ICMP)

    int on = 1;                             // "true"

    // May not be necessary as whereto is a method-local variable
    bzero((char *)&whereto, sizeof(struct sockaddr));

    /* Build out who we're sending this ping to */
    to->sin_family = AF_INET;
    if (!inet_aton(who, reinterpret_cast<in_addr *>(to->sin_addr.s_addr))) {
        strcpy(hostnameBuffer, who);
        hostname = hostnameBuffer;
    } else {
        pHostInfo = gethostbyname(hostname);
        if (pHostInfo) {
            to->sin_family = pHostInfo->h_addrtype;
            bcopy(pHostInfo->h_addr, (caddr_t)&to->sin_addr, pHostInfo->h_length);
            hostname = pHostInfo->h_name;
        } else {
            log->error("Unknown host: %s", hostname);
            return pingStats;
        }
    }

    /* Default to 56 if dataLength is less than 1 */
    if (dataLength < 1) {
        dataLength = 56;
    }

    /* Make sure the dataLength isn't too big */
    if (dataLength > PING_MAX_PACKET) {
        log->error("Packet size too large: %d", dataLength);
        return pingStats;
    }

    // Can we time the packets?
    if (dataLength >= sizeof(struct timeval)) {
        useTiming = true;
    }

    /* Get our process ID for tagging our requests with an ID */
    ident = getpid() & 0xFFFF;

    // Is the ICMP protocol available?
    if ((proto = getprotobyname("icmp")) == NULL) {
        log->error("Unknown protocol: ICMP");
        return pingStats;
    }

    // Open the socket
    if ((socket_fd = socket(AF_INET, SOCK_RAW, proto->p_proto)) < 0) {
        log->error("Failed to open socket");
    }

    // Set the socket options
    if (socketDebug) {
        setsockopt(socket_fd, SOL_SOCKET, SO_DEBUG, &on, sizeof(on));
    }
    if (socketDontRoute) {
        setsockopt(socket_fd, SOL_SOCKET, SO_DONTROUTE, &on, sizeof(on));
    }

    // Log who we're pinging
    if (to->sin_family == AF_INET) {
        log->info("Ping %s (%s) : %d data bytes", hostname, inet_ntoa(to->sin_addr), dataLength);
    } else {
        log->info("Ping %s : %d data bytes", hostname, dataLength);
    }

    /* Fire off the pings */
    for (int i = 0; i < preloadAmount + packetAmount; i++) {
        static u_char outpack[PING_MAX_PACKET];                             // Data storage for outbound packet
        register struct icmp *icp = (struct icmp *)outpack;                 // Cast outpack to icmp data
        int dataIndex, dataCount;                                           // For sending data one byte at a time
        register struct timeval *pTimeval = (struct timeval *)&outpack[8];  // Timeval pointer
        register u_char *pData = &outpack[8 + sizeof(struct timeval)];      // Data pointer

        icp->icmp_type = ICMP_ECHO;         // This is an ECHO request
        icp->icmp_code = 0;                 // No code
        icp->icmp_cksum = 0;                // No checksum (for request)
        icp->icmp_seq = txPacketAmount++;   // Which byte this is
        icp->icmp_id = ident;               // Tag with ID (our process ID)

        dataCount = dataLength + 8;         // Start at byte index 7 (timing data takes first 8 bytes)

        if (useTiming) {    // If we're using timing, get the current time
            gettimeofday(pTimeval, &tz);
        }

        // Starting at byte 8, increase the data pointer 1 byte at a time
        for (dataIndex = 8; dataIndex < dataLength; i++) {
            *pData++ = dataIndex;
        }

        icp->icmp_cksum = ipChecksum(reinterpret_cast<u_short *>(icp), dataCount); // Calculate the checksum

        // Send the data over the socket
        dataIndex = sendto(socket_fd, outpack, dataCount, 0, &whereto, sizeof(struct sockaddr));

        if (dataIndex < 0 || dataIndex != dataCount) {
            if (dataIndex < 0) {
                log->error("Send to failed");
            }
            log->info("Ping: wrote %s %d chars, returned = %d", hostname, dataCount, dataIndex);
        }

        if (i >= preloadAmount) {
            int length = sizeof(packet);
            int fromLength = sizeof(from);
            ssize_t recvCount;
            struct timeval timeout;
            int fdMask = 1 << socket_fd;

            if (!flood) { // If we're not reading the reply ASAP, wait for 1 second
                sleep(1);
            } else { // ... otherwise wait until a reply has been received
                while (select(32, reinterpret_cast<fd_set *>(&fdMask), 0, 0, &timeout) == 0);
            }

            // Read the socket
            if ((recvCount = recvfrom(socket_fd, packet, length, 0, reinterpret_cast<sockaddr *>(&from),
                                      reinterpret_cast<socklen_t *>(&fromLength))) < 0) {
                log->error("Receive from failed");
                continue;
            }

            /* Extract reply data from the contents of the reply packet */
            struct sockaddr_in *rFrom = &from;
            struct ip *ip;
            icp = NULL; // We're reading the received packet, so nullify this
            register long *lpPacket = (long *)packet; // Packet as a long
            register int i; // Don't care that we're redefining this
            struct timeval tval;
            struct timeval *pTimeval;
            int hLength, tripTime;

            rFrom->sin_addr.s_addr = ntohl(rFrom->sin_addr.s_addr);
            gettimeofday(&tval, &tz);

            ip = (struct ip *)packet;
            hLength = ip->ip_hl << 2;
            if (recvCount < hLength + ICMP_MINLEN) {
                log->debug("Packet too short (%d bytes) from %s", recvCount, inet_ntoa(rFrom->sin_addr));
                return pingStats;
            }

            recvCount -= hLength;
            icp = (struct icmp *)(packet + hLength);

            // If the ICMP type was not an ECHO reply
            if (icp->icmp_type != ICMP_ECHOREPLY) {
                log->info("%d bytes from %s: icmp_type = %d (%s) icmp_code = %d",
                          recvCount, inet_ntoa(rFrom->sin_addr),
                          icp->icmp_type, getICMPType(icp->icmp_type), icp->icmp_code);
                for (i = 0; i < 12; i++) {
                    log->debug("x%2.2x: x%8.8x", i * sizeof(long), *lpPacket++);
                }
                return pingStats;
            }

            // Check if the ECHO was ours
            if (icp->icmp_id != ident) {
                return pingStats;
            }

            /* Calculate packet timing, if applicable */
            if (useTiming) {
                pTimeval = (struct timeval *)&icp->icmp_data[0];
                timevalSubtract(&tval, pTimeval);
                tripTime = tval.tv_sec * 100 + (tval.tv_usec / 1000);
                timeSum += tripTime;
                if (tripTime < timeMin) {
                    timeMin = tripTime;
                }
                if (tripTime > timeMax) {
                    timeMax = tripTime;
                }
            }

            if (!flood) {
                log->info("%d bytes from %s: icmp_seq = %d", recvCount,
                inet_ntoa(from.sin_addr), icp->icmp_seq);

                if (useTiming) {
                    log->info("\ttime = %d ms", tripTime);
                }
            }
        }
        rxPacketAmount++;
    }
    return pingStats;
}

const char *IPDeviceManager::getICMPType(register int t) {
    static const char *typeTable[] = {
            "Echo Reply",
            "ICMP 1",
            "ICMP 2",
            "Destination Unreachable",
            "Source Quench",
            "Redirect",
            "ICMP 6",
            "ICMP 7",
            "Echo",
            "ICMP 9",
            "ICMP 10",
            "Time Exceeded",
            "Parameter Problem",
            "Timestamp",
            "Timestamp Reply",
            "Info Request",
            "Info Reply"
    };

    if (t < 0 || t > 16) {
        return("OUT-OF-RANGE");
    }

    return(typeTable[t]);
}

u_short IPDeviceManager::ipChecksum(u_short *addr, int length) {
    register int leftAmount = length;
    register u_short *w = addr;
    register u_short answer;
    register int sum = 0;

    while (leftAmount > 1) {
        sum += *w++;
        leftAmount -= 2;
    }

    if (leftAmount == 1) {
        u_short u = 0;
        *(u_char *)(&u) = *(u_char *)w;
        sum += u;
    }

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    answer = ~sum;
    return (answer);
}

void IPDeviceManager::timevalSubtract(register struct timeval *out, register struct timeval *in) {
    if ((out->tv_usec -= in->tv_usec) < 0) {
        out->tv_sec--;
        out->tv_usec += 1000000;
    }
    out->tv_sec -= in->tv_sec;
}
