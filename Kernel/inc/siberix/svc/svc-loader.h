#include <siberix/svc/svc-host.h>
#include <stdcxx/type-traits.h>

namespace Kern::Svc {
    template <typename T>
        requires Std::IsBaseOf<ISvcHost, T>::value
    class SvcLoader
    {
        using SvcHostType = T;

    public:
        ~SvcLoader() {}
        virtual SvcHostType* build() { return new T(); }

        static Std::Array<SvcLoader<ISvcHost>*>* getLoaders();
    };

    template <typename T>
        requires Std::IsBaseOf<ISvcHost, T>::value
    class StaticSvcLoader : public SvcLoader<T>
    {
    public:
        StaticSvcLoader(T* svcHost)
          : m_svcHost(svcHost)
        {
        }
        ~StaticSvcLoader() {}

        T* build() override { return m_svcHost; }

    private:
        T* m_svcHost;

        static Std::Array<SvcLoader<ISvcHost>*>* m_svcLoaders;
    };

    Std::Array<SvcLoader<ISvcHost>*>* getLoaders();
}