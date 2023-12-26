#include <siberix/sec/access.h>
#include <stdcxx/string.h>

namespace Kern {
    class UserGroup
    {
    public:
        UserGroup(Std::String<Utf16> name)
          : m_name(name)
        {
        }

        Std::String<Utf16>& getName() { return m_name; }

    private:
        Std::String<Utf16> m_name;
        Sec::AccessLevel   m_accessLevel;
    };
}