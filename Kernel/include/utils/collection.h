#pragma once

#include <utils/func/consumer.h>
#include <utils/optional.h>

namespace utils {
    template <typename T>
    class Stream;

    template <typename T>
    class Collection
    {
    public:
        Collection()  = default;
        ~Collection() = default;

        virtual T& operator[](unsigned index) = 0;

        virtual int         size()                            = 0;
        virtual Optional<T> get(unsigned index)               = 0;
        virtual int         indexOf(T& objRef)                = 0;
        virtual int         lastIndexOf(T& objRef)            = 0;
        virtual void        insert(T& objRef, unsigned index) = 0;
        virtual void        add(T& objRef)                    = 0;
        virtual bool        remove(unsigned index)            = 0;
        virtual bool        remove(T& objRef)                 = 0;
        virtual bool        contains(T& objRef)               = 0;
        virtual void        clear()                           = 0;

        virtual void forEach(utils::func::Consumer<T> consumer) = 0;
    };
}