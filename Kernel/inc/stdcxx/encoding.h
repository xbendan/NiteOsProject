#include <stdcxx/type-traits.h>
#include <stdcxx/types.h>

/// @brief The type of encoding rune, it has another name: code point
using EncodingRune = uint32_t;

template <typename T>
concept Encoding = requires(T t, EncodingRune& r, typename T::Unit unit) {
    {
        T::getUnitLength(unit)
    } -> Std::Same<uint8_t>;
    {
        T::getRuneLength(r)
    } -> Std::Same<uint8_t>;
};

struct Utf8
{
    using Unit = uint8_t;

    /**
     * @brief Get the byte length of specific char code
     *
     * This function returns how many bytes are needed for
     * current unit to represent a character. If the unit
     * is not a valid utf-8 unit, 0 will be returned.
     *
     * Compare the first unit with specific masks, if the
     * unit start with 0, it is a single byte character,
     * this is for the compatibility with ASCII. If the
     * unit start with 0xC0 (110 in binary format), it is
     * a two bytes character. The same rule applies to
     * three bytes (0xE0, 1110 in binary) and four bytes
     * character. (0xF0, 11110 in binary)
     *
     * According to the early design of utf-8, the maximum
     * length of a character is 6 bytes, but the range
     * shrunk to 4 bytes by RFC 3629 in 2003. So we only
     * need to handle with 1 to 4 bytes character.
     *
     * @param unit the first byte of the character
     * @return byte length of current character
     */
    static constexpr uint8_t getUnitLength(Unit unit)
    {
        if ((unit & 0x80) == 0x00) {
            return 1;
        } else if ((unit & 0xE0) == 0xC0) {
            return 2;
        } else if ((unit & 0xF0) == 0xE0) {
            return 3;
        } else if ((unit & 0xF8) == 0xF0) {
            return 4;
        } else
            return 0;
    }

    /**
     * @brief Get the rune length of specific rune
     *
     * Rune is an unicode character, it is a 32-bit integer
     * that represents a character. The rune length is the
     * byte length of the rune.
     *
     * The rune under 0x7F is a single byte character, the
     * rune under 0x7FF is a two bytes character, and the
     * same rule applies to three bytes (0xFFFF) and four
     * bytes (more).
     *
     * @param rune
     * @return rune length
     */
    static constexpr uint8_t getRuneLength(EncodingRune rune)
    {
        if (rune < 0x80) {
            return 1;
        } else if (rune < 0x800) {
            return 2;
        } else if (rune < 0x10000) {
            return 3;
        } else if (rune < 0x110000) {
            return 4;
        } else
            return 0;
    }

    static bool encode() { return false; }

    static bool decode() { return false; }
};

struct Utf16
{
    using Unit = uint16_t;

    static constexpr uint8_t getUnitLength(Unit unit)
    {
        if ((unit & 0xFC00) == 0xD800) {
            return 2;
        } else
            return 1;
    }

    static constexpr uint8_t getRuneLength(EncodingRune rune)
    {
        if (rune < 0x10000) {
            return 1;
        } else if (rune < 0x110000) {
            return 2;
        } else
            return 0;
    }

    static bool encode() { return false; }

    static bool decode() { return false; }
};