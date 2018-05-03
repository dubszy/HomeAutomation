#include <string>

#include "../types/network_types.h"

using namespace std;

typedef enum {
    RequestTypeIdentify = 0x01,
    RequestTypeStop = 0x0F
} RequestType;

typedef struct {
    NetAddr netAddr;
    string os;
    string deviceType;
    string deviceName;
} IdentifyResponse;