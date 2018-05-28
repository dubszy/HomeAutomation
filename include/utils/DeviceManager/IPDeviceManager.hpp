#include <vector>

#include <devices/core/IPDevice.hpp>
#include "DeviceManager.hpp"

#include <logger/c++/Logger.hpp>

#define PING_VERBOSE    1
#define PING_QUIET      2
#define PING_FLOOD      4
#define PING_MAX_WAIT   10
#define PING_MAX_PACKET 4096
#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN  64
#endif

typedef struct {
    NetAddr net_addr;
    IPDevice *device;
} IPDeviceMap;

typedef struct {
    int byteAmount;
    int icmpType;
    char *icmpTypeName;
    int icmpCode;
} PingReply;

typedef struct {
    int txPacketAmount;
    int rxPacketAmount;
    int packetLossPercent;
    int timingMin;
    int timingMax;
    int timingAverage;
    vector<PingReply> replies;
    IPDevice *ipDevice;
} PingStats;

class IPDeviceManager : DeviceManager {

    public:
        explicit IPDeviceManager(uint32_t maxDevices) : DeviceManager(maxDevices) {}

        static IPDeviceManager *getSharedIPDeviceManager();
        int addDevice(IPDevice *device, NetAddr addr);
        int removeDevice(NetAddr addr);
        IPDevice *getDevice(NetAddr addr);

        vector<IPDeviceMap> discoverDevicesOnLAN(IPAddress netmask);

        PingStats ping(const char *who, int pingCount, int dataLength, int packetAmount, int preloadAmount,
                       bool socketDebug, bool socketDontRoute, bool flood);

protected:
    static IPDeviceManager *sharedIPDeviceManager;
    Logger *log = Logger::forClass<IPDeviceManager>(LogLevelInfo);

private:

    //*** Ping Utility Methods ***//

    /**
     * Get a friendly name for an ICMP type
     *
     * @param t ICMP type to get the name of
     * @return friendly name of the supplied ICMP type
     */
    const char *getICMPType(register int t);

    /**
     * Get the stats of a packet if it came from us. This logic is necessary
     * because all readers of the ICMP socket get a copy of all ICMP
     * packets that arrive. This permits multiple copies of ping to be
     * run without having intermingled output or statistics.
     *
     * @param buffer
     * @param cc
     * @param from
     */
    void extractReply(char *buffer, int cc, struct sockaddr_in *from);

    /**
     * Checksum routine for Internet Protocol family headers.
     *
     * @param addr
     * @param length
     */
    u_short ipChecksum(u_short *addr, int length);

    /**
     * Subtract one timeval struct from another: out = out - in.
     *
     * @param out timeval struct to subtract from
     * @param in timeval struct to subtract
     */
    void timevalSubtract(register struct timeval *out, register struct timeval *in);
};
