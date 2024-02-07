#include <stdcxx/array-list.h>
#include <stdcxx/hash.h>
#include <stdcxx/type-traits.h>

namespace Std {
    template <class K, class V>
        requires Std::isBaseOf<IHashCode, K>::value
    class HashMap
    {
    public:
        class Entry
        {
        public:
            Entry(K key, V value)
              : m_key(key)
              , m_value(value)
            {
            }

            inline K getKey() const { return m_key; }
            inline V getValue() const { return m_value; }
        };

        HashMap()
          : m_entries(nullptr)
          , m_capacity(0)
          , m_size(0)
        {
        }
        HashMap(uint64_t capacity)
          : m_entries(nullptr)
          , m_capacity(capacity)
          , m_size(0)
        {
            m_entries = new Entry*[capacity];
        }

    private:
        Std::ArrayList<Entry> m_entries;
    };
}