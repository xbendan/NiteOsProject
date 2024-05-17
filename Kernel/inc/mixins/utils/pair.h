#include <mixins/std/type_traits.h>

namespace mixins::utils {
    template <typename _K, typename _V>
    class KeyValuePair
    {
    public:
        _K const& key() const { return m_key; }
        _V const& value() const { return m_value; }

    private:
        _K m_key;
        _V m_value;
    };
}