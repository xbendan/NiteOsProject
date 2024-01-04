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

        template <typename T>
            requires Std::IsBaseOf<ISvcHost, T>::value
        static class StaticLoader : public SvcLoader<T>
        {
        public:
            T* build() override
            {
                static T svc = T();
                return &svc;
            }
        };

        using Static = StaticLoader;
    };

    Std::Array<SvcLoader<ISvcHost>*>* allLoaders();

    template <typename T>
        requires Std::IsBaseOf<ISvcHost, T>::value
    class StaticSvcLoader : public SvcLoader<T>
    {
    public:
        T* build() override
        {
            static T svc = T();
            return &svc;
        }

    private:
        T* m_svcHost;
    };

    Std::Array<SvcLoader<ISvcHost>> getAllLoaders();
}