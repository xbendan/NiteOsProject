#include <siberix/display/video.h>

VideoCompositionDevice::VideoCompositionDevice()
    : Device(
          "Video Compositor", DeviceBus::Software, DeviceType::DisplayAdapter) {
}

VideoCompositionDevice::~VideoCompositionDevice() {}