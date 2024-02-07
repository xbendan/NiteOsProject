#include <stdcxx/opt.h>

template <typename T>
class Res : public Opt<T>
{
public:
    enum class Respond
    {
        Ok,
        Error
    };
};