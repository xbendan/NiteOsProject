#include <common/arch.h>
#include <siberix/mm/page.hpp>

class X64RuntimeSupport : public RuntimeSupport {
public:
    X64RuntimeSupport();
    ~X64RuntimeSupport();

    void setup() override;
    void loadMemory() override;
    void loadDevices() override;
    void setupPaging();
};