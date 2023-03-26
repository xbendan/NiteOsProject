#include <Drivers/Input/PS2LegacyController.h>
#include <Drivers/Input/PS2LegacyKeyboard.h>
#include <Drivers/Generic/APIC.h>

#include <Arch/x86_64/Interrupts.h>
#include <Arch/x86_64/Ports.h>

namespace Input
{
    using namespace Ports;

    void KeyHandler(InterruptData *data, RegisterContext *context)
    {
        while (!(ReadByte8(PS2_COMMAND_PORT) & PS2RegOutputBuffer))
            ;

        uint8_t keyCode = ReadByte8(PS2_DATA_PORT);
        uint8_t scanCode = keyCode & 0x7F, keyState = keyCode & 0x80;

        System::Out("Key: %x", keyCode);
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
