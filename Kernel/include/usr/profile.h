#include <macros>

typedef uint16_t uid_t;

enum UserAccountState
{
    UserAccountActive = 0;
    UserAccountLocked = 1;
    UserAccountInactive = 2;
    UserAccountSleep = 3;
}

class UserProfile
{
    char* m_Name;
    uid_t m_Uid;
    enum UserAccountState m_AccountState;
    struct
    {
        uint32_t m_Threads;
        uint16_t m_Processes;
        uint32_t m_Handles;
    };
} profile_t;