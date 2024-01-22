#define PS2_DATA_PORT 0x60
#define PS2_COMMAND_PORT 0x64

namespace Kern::Hal::Spec {
    enum class PS2StateReg
    {
        OutputBuffer   = 0x01,
        InputBuffer    = 0x02,
        SystemFlag     = 0x04,
        CommandData    = 0x08,
        KeyboardLock   = 0x10,
        ReceiveTimeout = 0x20,
        TimeoutError   = 0x40,
        ParityError    = 0x80
    };

    enum class PS2Command
    {
        ReadByteZero         = 0x20,
        WriteByteZero        = 0x60,
        DisableSecondPort    = 0xA7,
        EnableSecondPort     = 0xA8,
        TestSecondPort       = 0xA9,
        TestPs2Controller    = 0xAA,
        TestFirstPort        = 0xAB,
        DiagnosticDump       = 0xAC,
        DisableFirstPort     = 0xAD,
        EnableFirstPort      = 0xAE,
        ReadControllerInput  = 0xC0,
        CopyBits03To47       = 0xC1,
        CopyBits47To47       = 0xC2,
        ReadControllerOutput = 0xD0
    };

    enum class PS2ConfigByte
    {
        FirstPortInterrupt   = 0x01,
        SecondPortInterrupt  = 0x02,
        SystemFlag           = 0x04,
        Reserved_0           = 0x08,
        FirstPortClock       = 0x10,
        SecondPortClock      = 0x20,
        FirstPortTranslation = 0x40,
        Reserved_1           = 0x80
    };
}