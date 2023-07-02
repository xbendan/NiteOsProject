enum ArchType 
{
    CA_UNDEFINED = 0,
    CA_X86_64 = 1,
    CA_ARMV7 = 2,
    CA_RISCV = 2
};

class RuntimeArchSupport
{
public:
    RuntimeArchSupport(ArchType _type)
      : type(_type),
        initialized(false) { }

    virtual bool setup() = 0;
    ArchType getArchType();
    bool isInitialized() { return isInitialized; }

protected:
    const ArchType archType;
    bool initialized;
};

RuntimeArchSupport& getRuntimeArch();