#include <siberix/dvc/device.h>

namespace Kern {
    Device::Device(Std::String<Utf8> name, DeviceType type)
      : m_name(name)
      , m_type(type)
      , m_flags(0)
    {
    }

    Device::Device(Std::String<Utf8> name)
      : m_name(name)
      , m_type(DeviceType::Unknown)
      , m_flags(0)
    {
    }

}