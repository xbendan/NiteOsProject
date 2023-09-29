#include <common/cstring.h>

class InstructionSet
{
public:
    virtual void enable()  = 0;
    virtual void disable() = 0;
    bool         isEnabled() { return m_isEnabled; }

private:
    String m_name;
    bool   m_isEnabled;
};