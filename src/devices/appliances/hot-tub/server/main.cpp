#include <devices/appliances/hot-tub/server/HotTub.hpp>

int main() {
    HotTub *hotTub = new HotTub();
    hotTub->selfTest();
    return 0;
}