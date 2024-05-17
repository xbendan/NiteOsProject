#include <mixins/utils/hash_map.h>
#include <sbrx/user/privilege.h>

namespace sbrx::user {
    class UserAccount
    {};

    class UserGroup
    {
    private:
        privilege::PrivilegeLevel m_privilegeLevel;
    };
}