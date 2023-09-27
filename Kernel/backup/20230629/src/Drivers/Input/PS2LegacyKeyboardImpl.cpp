#include <Drivers/Input/PS2LegacyController.h>
#include <Drivers/Input/PS2LegacyKeyboard.h>
#include <Drivers/Generic/APIC.h>
#include <Input/KeyCode.h>
#include <Input/Console.h>

#include <Arch/x86_64/Interrupts.h>
#include <Arch/x86_64/Ports.h>

namespace Input
{
    using namespace Ports;

    static volatile int keySet = 0;

    void KeyHandler(Interrupt *data, RegisterContext *context)
    {
        while (!(ReadByte8(PS2_COMMAND_PORT) & PS2RegOutputBuffer))
            ;

        uint8_t response = ReadByte8(PS2_DATA_PORT);
        uint8_t keyCode = response & 0x7F;

        if (keyCode == 0xE0)
        {
            keySet = 1;
            return;
        }

        KeyState keyState = (KeyState)(response & 0x80);
        Key key = c_KeyScanCode1[keySet][keyCode];

        // ConsoleKeyEventHandler(c_KeyScanCode1[keySet][keyCode], !(response & 0x80));
        
        keySet = 0;
    }

    PS2LegacyKeyboard::PS2LegacyKeyboard()
      : Device("PS/2 Legacy Keyboard Input Device", DeviceBusPS2, DeviceTypeKeyboard)
    {
        RegisterIRQ(0x21, KeyHandler);
        APIC::IO::EnableInterrupt(0x21);
    }

    PS2LegacyKeyboard::~PS2LegacyKeyboard()
    {

    }

    void PS2LegacyKeyboard::Enable()
    {

    }

    void PS2LegacyKeyboard::Disable()
    {
        
    }
} // namespace Input
