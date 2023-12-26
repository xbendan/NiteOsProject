namespace Std {
    class HashCode
    {};

    class IHashCode
    {
    public:
        virtual HashCode hashCode() = 0;
    };
}