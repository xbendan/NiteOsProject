#include <mixins/meta/opt.h>
#include <mixins/std/type_traits.h>
#include <mixins/types.h>
#include <mixins/utils/pair.h>

namespace mixins::utils {
    template <typename _Tp>
        requires(!std::isTrivial<_Tp>::value)
    class ICollection
    {
    public:
        /* --- Methods --- */

        virtual void   add(_Tp* value)      = 0;
        virtual void   remove(_Tp* value)   = 0;
        virtual bool   contains(_Tp* value) = 0;
        virtual void   clear()              = 0;
        virtual size_t size()               = 0;
        virtual bool   isEmpty()            = 0;
    };

    template <typename _Tp>
        requires(!std::isTrivial<_Tp>::value)
    class IList : public ICollection<_Tp>
    {
    public:
        /* --- Methods --- */

        virtual size_t indexOf(_Tp* value)    = 0;
        virtual void   removeAt(size_t index) = 0;
        virtual _Tp*   getAt(size_t index)    = 0;

        /* --- Operators --- */

        virtual _Tp&       operator[](size_t index)       = 0;
        virtual _Tp const& operator[](size_t index) const = 0;
    };

    template <typename _Tp>
        requires(!std::isTrivial<_Tp>::value)
    class ISet : public ICollection<_Tp>
    {};

    template <typename _K, typename _V>
    class IMap : protected ICollection<KeyValuePair<_K, _V>>
    {
    public:
        /* --- Methods --- */

        virtual Opt<_V>         put(_K key, _V value)         = 0;
        virtual Opt<_V>         putIfAbsent(_K key, _V value) = 0;
        // virtual Opt<_V>         compute(_K key, _V value)         = 0;
        // virtual Opt<_V>         computeIfAbsent(_K key, _V value) = 0;
        virtual Opt<_V>         remove(_K key)                = 0;
        virtual bool            contains(_K key)              = 0;
        virtual virtual Opt<_V> get(_K key)                   = 0;
        virtual size_t          size()                        = 0;

        virtual ICollection<_K>                   keys()    = 0;
        virtual ICollection<_V>                   values()  = 0;
        virtual ICollection<KeyValuePair<_K, _V>> entries() = 0;
    };
}