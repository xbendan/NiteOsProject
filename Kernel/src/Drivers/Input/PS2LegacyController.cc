#include <Drivers/Input/PS2LegacyController.h>
#include <Drivers/Input/PS2LegacyKeyboard.h>
#include <Drivers/Generic/ACPI.h>

using namespace Ports;

namespace Input
{
    PS2LegacyKeyboard *g_PS2Keyboard;

    PS2LegacyController::PS2LegacyController()
    {
        // Determine if PS2 Controller actually exists.
        /* PS2 Controller is not supported while the bit 1 of
           'IA PC Boot Architecture Flags' is clear (0). */
        if (ACPI::g_FADT != nullptr && (ACPI::g_FADT->BootArchFlags & 0x02))
        {
            System::Out("Error, PS/2 Controller is not supported on this device.");
            return;
        }

        /* Disable ports */
        SendCommand(PS2CmdDisableFirstPort);
        SendCommand(PS2CmdDisableSecondPort);

        /* Flush The Output Buffer */
        while (ReadByte8(PS2_COMMAND_PORT) & PS2RegOutputBuffer)
            ReadByte8(PS2_DATA_PORT);

        /* Set the Controller Configuration Byte */
        SendCommand(PS2CmdReadByteZero);
        uint8_t status = ReadByte8(PS2CmdReadByteZero)
            & ~(PS2CfgFirstPortClock | PS2CfgSecondPortClock | PS2CfgFirstPortTranslation)
            | PS2CfgFirstPortInterrupt;
        SendCommand(PS2CmdWriteByteZero);
        WriteByte8(PS2_DATA_PORT, status);

        /* Perform Controller Self Test */
        SendCommand(PS2CmdTestPS2Controller);
        WaitData();
        if (ReadByte8(PS2_DATA_PORT) != 0x55)
        {
            System::Out("Error, failed to reset PS/2 Controller");
            return;
        }

        /* Test first port and register PS2 Keyboard device */
        SendCommand(PS2CmdTestFirstPort);
        WaitData();
        if (!ReadByte8(PS2_DATA_PORT))
        {
            SendCommand(PS2CmdEnableFirstPort);
            (g_PS2Keyboard = new PS2LegacyKeyboard())->Register();
        }
    }

    PS2LegacyController::~PS2LegacyController()
    {

    }

    void PS2LegacyController::SendCommand(uint8_t command)
    {
        while (ReadByte8(PS2_COMMAND_PORT) & PS2RegInputBuffer)
            ;
        WriteByte8(PS2_COMMAND_PORT, command);
    }
} // namespace Input
