#include <Device/Device.h>

namespace Input
{
    class PS2LegacyKeyboard : public Device
    {
    public:
        PS2LegacyKeyboard();
        ~PS2LegacyKeyboard();

        virtual void Enable();
        virtual void Disable();
    };
}