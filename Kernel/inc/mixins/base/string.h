#include <mixins/base/encode.h>

namespace mixins {
    template <Encoding _E>
    class String
    {
        using Encoding = _E;
        using Unit     = typename Encoding::Unit;

    public:
        /* --- Constructors --- */

        String(const char* str)
          : m_data(reinterpret_cast<Unit*>(const_cast<char*>(str)))
        {
            int i = 0;
            while (str[i] != '\0') {
                i++;
            }
            m_length = i;
        }
        String(Unit const*, uint64_t);
        String(String const& other)
          : m_data(other.m_data)
          , m_length(other.m_length)
        {
        }
        String(String&& other) noexcept
          : m_data(other.m_data)
          , m_length(other.m_length)
        {
            other.m_data   = nullptr;
            other.m_length = 0;
        }

        /* --- Methods --- */

        bool equals(String<E> const& other) const
        {
            if (m_length != other.m_length || !m_data || !other.m_data) {
                return false;
            } else {
                for (int i = 0; i < m_length; i++) {
                    if (m_data[i] != other.m_data[i]) {
                        return false;
                    }
                }
                return true;
            }
        }

        bool equalsIgnoreCase(String<E> const& other) const
        {
            if (m_length != other.length() || !m_data || !other.m_data) {
                return false;
            }
            // for (int i = 0; i < m_length; i++) {
            //     if (E::toLower(m_data[i]) != Utf8::toLower(other.m_data[i]))
            //     {
            //         return false;
            //     }
            // }
            return true;
        }

        bool isStartWith(String<_E> const&) const;
        bool isEndWith(String<_E> const&) const;
        bool contains(String<_E> const&) const;

        /* --- Operators --- */

        String<Encoding>& operator=(const char* str)
        {
            m_data = reinterpret_cast<Unit*>(const_cast<char*>(str));
            int i  = 0;
            while (str[i] != '\0') {
                i++;
            }
            m_length = i;
            return *this;
        }
        String<Encoding>& operator=(String<Encoding> const& other)
        {
            m_data   = other.m_data;
            m_length = other.m_length;
            return *this;
        }
        String<Encoding>& operator=(String<Encoding>&& other)
        {
            m_data   = other.m_data;
            m_length = other.m_length;

            other.m_data   = nullptr;
            other.m_length = 0;
            return *this;
        }

        Unit const& operator[](uint64_t i) const { return m_data[i]; }

        bool operator==(String const& other) const { return equals(other); }
        bool operator!=(String const&) const;
    };
}