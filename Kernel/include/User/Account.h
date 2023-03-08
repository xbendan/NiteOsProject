#include <proc/proc.h>
#include <utils/list.h>
#include <resource.h>
#include <macros>

enum UserAccountState
{
    UserAccountActive = 0;
    UserAccountLocked = 1;
    UserAccountInactive = 2;
    UserAccountSleep = 3;
};

class UserAccount
{
    char* m_Name;
    uint8_t m_UserId;
    uint8_t m_UUID[16];
    UserAccountState m_AccountState;
    SystemUsageProfile m_Usages;
    LinkedList<Task::Process *> m_ProcessList;
};