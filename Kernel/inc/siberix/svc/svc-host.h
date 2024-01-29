#pragma once

#include <stdcxx/array.h>
#include <stdcxx/string.h>

namespace Kern::Svc {
    class ISvcHost
    {
    public:
        ISvcHost(Std::String<Utf8>  svcName,
                 Std::String<Utf8>* svcDepends = nullptr)
          : m_svcName(svcName)
          , m_svcDepends(svcDepends)
        {
        }

        virtual Std::String<Utf8>  getSvcName() { return m_svcName; }
        virtual Std::String<Utf8>* getSvcDepends() { return m_svcDepends; }

        virtual void onLoad() = 0;
        virtual void onEnable() {}
        virtual void onDisable() {}

    protected:
        Std::String<Utf8>  m_svcName;
        Std::String<Utf8>* m_svcDepends;
    };
}