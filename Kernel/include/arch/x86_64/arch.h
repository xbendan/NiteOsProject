#include <common/runtime.hpp>
#include <siberix/mm/page.hpp>

class X64SystemRuntime : public SystemRuntime {
public:
    X64SystemRuntime();
    ~X64SystemRuntime();

    void setup() override;
    void loadMemory() override;
    void loadDevices() override;
};