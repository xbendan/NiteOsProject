#include <siberix/svc/svc-host.h>

namespace Kern::Svc {
    class ServiceManager
    {};

    template <typename T>
        requires Std::isBaseOf<ISvcHost, T>::value
    class ServiceLoader
    {
    public:
        virtual T* get() = 0;
    };

    template <typename T>
        requires Std::isBaseOf<ISvcHost, T>::value
    class RegisteredServiceLoader : public ServiceLoader<T>
    {};
}