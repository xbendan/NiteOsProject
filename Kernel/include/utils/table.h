#pragma once

#include <utils/collection.h>
#include <utils/optional.h>

namespace utils {
    template <typename K, typename V>
    class Entry
    {
    public:
        Entry() = default;
        Entry(K key, V value)
          : m_key(key)
          , m_value(value)
        {
        }
        ~Entry() = default;

        K*          getKey() { return m_key; }
        V*          getValue() { return m_value; }
        Optional<V> getValueSupplier() { return Supplier(value); }

    private:
        K* m_key;
        V* m_value;
    };

    template <typename K, typename V>
    class Table : public Collection<Entry<K, V>>
    {
    public:
        Table()
          : Table(16)
        {
        }

        Table(unsigned initial) { m_tableEntries = new Entry<K, V>[initial]; }

        ~Table() { delete m_tableEntries; }

        void set(K* key, V* value);
        void setIfAbsent(K* key, V* value);
        V*   get(K* key);
        bool isEmpty();
        bool contains(K* key);

        Optional<V> operator[](K& key);

    private:
        Entry<K, V>* m_tableEntries;
        unsigned     m_count;
        float        m_factor = 0.75f;
    };
}