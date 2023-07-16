#include <siberix/core/runtimes.hpp>
#include <siberix/device/connectivity.hpp>
#include <siberix/device/device.hpp>

Device::install() { runtime()->getConnectivity().install(this); }