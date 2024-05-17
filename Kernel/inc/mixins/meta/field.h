namespace mixins {
    enum FieldPolicy : unsigned
    {
        Read  = 0b01,
        Write = 0b10,
        Full  = 0b11,
    };

    template <typename _Tp, FieldPolicy _Policy = Full>
    class Field
    {
    public:
        /* --- Constructors --- */

        Field() = default;
        Field(const _Tp& value)
          : m_value(value)
        {
        }
        Field(_Tp&& value)
          : m_value(std::move(value))
        {
        }
        ~Field() = default;

        /* --- Methods --- */

        _Tp& get()
            requires(_Policy& Read)
        {
            return m_value;
        }

        void set(const _Tp& value)
            requires(_Policy& Write)
        {
            m_value = value;
        }

        /* --- Operators --- */

        _Tp& operator()()
            requires(_Policy& Read)
        {
            return m_value;
        }

        Field<_Tp>& operator=(const _Tp& value)
            requires(_Policy& Write)
        {
            m_value = value;
            return *this;
        }
        Field<_Tp>& operator=(_Tp&& value)
            requires(_Policy& Write)
        {
            m_value = std::move(value);
            return *this;
        }
        Field<_Tp>& operator=(const Field<_Tp>& field)
            requires(_Policy& Write)
        {
            m_value = field.m_value;
            return *this;
        }
        Field<_Tp>& operator=(Field<_Tp>&& field)
            requires(_Policy& Write)
        {
            m_value = std::move(field.m_value);
            return *this;
        }

    private:
        _Tp m_value;
    };

    template <class _Tp>
    using ReadWrite = Field<_Tp, true, true>;

    template <class _Tp>
    using ReadOnly = Field<_Tp, true, false>;

}