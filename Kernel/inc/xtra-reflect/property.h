#include <stdcxx/func.h>
#include <stdcxx/string.h>

namespace Xtra::Reflect {
    template <typename Func>
        requires Std::IsFunction<Func>
    class FunctionProperty
    {
    public:
        String getNamespace() {}
        String getName() {}

    private:
        bool  _withTemplate;
        UInt8 _params;
    };

}