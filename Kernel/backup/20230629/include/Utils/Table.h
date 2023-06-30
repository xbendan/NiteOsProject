#include <Utils/LinkedList.h>

#include <macros>

template <typename K, typename V>
class KeyValuePair
{
public:
    K m_Key;
    V m_Value;

    KeyValuePair(K key, V value)
      : m_Key(key),
        m_Value(value) { }
};

template <typename K, typename V>
class KeyValueTable
{
private:
    LinkedList<KeyValuePair<K, V> *> m_PairList;

public:
    void Add(K key, V value)
    {
        if (ContainsKey(key))
            return;

        m_PairList.Add(new KeyValuePair<K, V>(key, value));
    }

    KeyValuePair<K, V> *ContainsKey(K key)
    {
        int index = 0;
        ListNode<KeyValuePair<K, V> *> *obj = m_PairList.First();
        while (index < m_PairList.Count())
        {
            if (obj->obj->m_Key == key) 
                return obj->obj;
            else
            {
                if (obj->next != nullptr)
                {
                    obj = obj->next;
                } else return nullptr;
            }
            index++;
        }
        return nullptr;
    }

    V *Get(K key)
    {
        KeyValuePair<K, V> *pair = ContainsKey(key);
        return pair != nullptr ?
            &pair->m_Value :
            nullptr;
    }
};