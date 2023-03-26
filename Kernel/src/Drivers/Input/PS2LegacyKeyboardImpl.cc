#include <Drivers/Input/PS2LegacyController.h>
#include <Drivers/Input/PS2LegacyKeyboard.h>

#include <Arch/x86_64/Interrupts.h>
#include <Arch/x86_64/Ports.h>

namespace Input
{
    using namespace Ports;

    void KeyHandler(InterruptData *data, RegisterContext *context)
    {
        uint8_t state = ReadByte8(PS2_STATE_PORT);
        while ((state & PS2RegOutputBuffer) && !(state & PS2RegReceiveTimeout))
        {
            uint8_t keyCode = ReadByte8(PS2_PORT);
            uint8_t scanCode = keyCode & 0x7F;
            uint8_t keyState = keyCode & 0x80;

            
        }
        
    }

    PS2LegacyKeyboard::PS2LegacyKeyboard()
      : Device("PS/2 Legacy Keyboard Input Device", DeviceBusPS2, DeviceTypeKeyboard)
    {
        RegisterIRQ(0x21, KeyHandler);
    }

    PS2LegacyKeyboard::~PS2LegacyKeyboard()
    {

    }
} // namespace Input
