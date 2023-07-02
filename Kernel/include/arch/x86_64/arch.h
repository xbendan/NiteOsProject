#include <common/arch.h>

class X64RuntimeSupport : public RuntimeArchSupport {
public:
    X64RuntimeSupport()
      : RuntimeArchSupport(CA_X86_64) 
    {
        
    }

    void setup() override;
};