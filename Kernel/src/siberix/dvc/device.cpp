#include <siberix/dvc/device.h>

namespace Kern {
    Device::Device(Std::String<Utf8> name, DeviceType type)
      : m_name(name)
      , m_flags(0)
      , m_type(type)
    {
    }

    Device::Device(Std::String<Utf8> name)
      : m_name(name)
      , m_flags(0)
      , m_type(DeviceType::Unknown)
    {
    }

}