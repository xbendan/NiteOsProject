#include <Device/Device.h>
#include <Timer.h>

#include <Arch/x86_64/Ports.h>
#include <macros>

#define PS2_DATA_PORT 0x60
#define PS2_COMMAND_PORT 0x64

namespace Input
{
    enum PS2StateRegister
    {
        PS2RegOutputBuffer = 0x01,
        PS2RegInputBuffer = 0x02,
        PS2RegSystemFlag = 0x04,
        PS2RegCommandData = 0x08,
        PS2RegKeyboardLock = 0x10,
        PS2RegReceiveTimeout = 0x20,
        PS2RegTimeoutError = 0x40,
        PS2RegParityError = 0x80
    };

    enum PS2Command
    {
        PS2CmdReadByteZero = 0x20,
        PS2CmdWriteByteZero = 0x60,
        PS2CmdDisableSecondPort = 0xA7,
        PS2CmdEnableSecondPort = 0xA8,
        PS2CmdTestSecondPort = 0xA9,
        PS2CmdTestPS2Controller = 0xAA,
        PS2CmdTestFirstPort = 0xAB,
        PS2CmdDiagnosticDump = 0xAC,
        PS2CmdDisableFirstPort = 0xAD,
        PS2CmdEnableFirstPort = 0xAE,
        PS2CmdReadControllerInput = 0xC0,
        PS2CmdCopyBits03To47 = 0xC1,
        PS2CmdCopyBits47To47 = 0xC2,
        PS2CmdReadControllerOutput = 0xD0
    };

    enum PS2ConfigByte
    {
        PS2CfgFirstPortInterrupt = 0x01,
        PS2CfgSecondPortInterrupt = 0x02,
        PS2CfgSystemFlag = 0x04,
        PS2CfgReserved_0 = 0x08,
        PS2CfgFirstPortClock = 0x10,
        PS2CfgSecondPortClock = 0x20,
        PS2CfgFirstPortTranslation = 0x40,
        PS2CfgReserved_1 = 0x80
    };

    class PS2LegacyController : public Device
    {
    public:
        PS2LegacyController();
        ~PS2LegacyController();

        void SendCommand(uint8_t command);

        virtual void Enable();
        virtual void Disable();

        inline void WaitData()
        {
            int timeout = 100;
            while (timeout--)
            {
                if (Ports::ReadByte8(PS2_COMMAND_PORT) & PS2RegOutputBuffer)
                    return;

                Sleep(1);
            }
        }
    };
} // namespace Input
