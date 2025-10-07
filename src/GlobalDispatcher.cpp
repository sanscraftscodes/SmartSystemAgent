#include "GlobalDispatcher.h"
#include "DeviceManager.h"

static DeviceManager* g_device_manager = nullptr;

void setGlobalDeviceManager(DeviceManager* mgr) {
    g_device_manager = mgr;
}

bool globalDispatch(const std::string &action, const std::string &args) {
    if (!g_device_manager) return false;
    return g_device_manager->dispatch(action, args);
}
