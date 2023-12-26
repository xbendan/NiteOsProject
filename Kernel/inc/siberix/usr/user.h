#include <stdcxx/string.h>
#include <stdcxx/uuid.h>

namespace Kern {
    class User
    {
    public:
        Std::String<Utf16>& getName() { return m_name; }

    private:
        Std::String<Utf16> m_name;
        Std::UUID          m_uuid;
    };
}