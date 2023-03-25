#include <Drivers/Input/LegacyKeyboard.h>

#include <Arch/x86_64/Interrupts.h>

namespace Input
{
    void KeyHandler(InterruptData *data, RegisterContext *context)
    {

    }

    LegacyKeyboard::LegacyKeyboard()
      : Device("PS/2 Legacy Keyboard Input Device", DeviceBusPS2, DeviceTypeKeyboard)
    {
        
    }

    LegacyKeyboard::~LegacyKeyboard()
    {

    }
} // namespace Input
