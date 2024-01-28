#include <stdcxx/type-traits.h>

namespace Std {

    template <typename T>
        requires Std::IsIntegral<T>
    struct Bitmap
    {
        uint8_t  _bits = sizeof(T) * 8;
        uint64_t _size;
        T*       _data;

        constexpr Bitmap() {}

        constexpr Bitmap(uint64_t size)
          : _size(size)
        {
            _data = new T[size / _bits];
        }

        virtual ~Bitmap() { delete[] _data; }

        virtual void set(uint64_t pos)
        {
            _data[pos / _bits] |= (1 << (pos % _bits));
        }

        virtual void setAll(uint64_t pos, uint64_t amount)
        {
            while (amount) {
                uint8_t   bitsOffset = pos % _bits;
                uint64_t& data       = at(pos);
                if (!bitsOffset && amount >= _bits) {
                    /*
                        Set all bits in the data
                        Move position pointer to the next data
                     */
                    data    = -1;
                    amount -= _bits;
                    pos    += _bits;
                } else {
                    data |= (1 << bitsOffset);
                    amount--;
                    pos++;
                }
            }
        }

        virtual void clearAll(uint64_t pos, uint64_t amount)
        {
            while (amount) {
                uint8_t   bitsOffset = pos % _bits;
                uint64_t& data       = at(pos);
                if (!bitsOffset && amount >= _bits) {
                    /*
                        Clear all bits in the data
                        Move position pointer to the next data
                     */
                    data    = 0;
                    amount -= _bits;
                    pos    += _bits;
                } else {
                    data &= ~(1 << bitsOffset);
                    pos++;
                }
            }
        }

        virtual void clear(uint64_t pos)
        {
            _data[pos / _bits] &= ~(1 << (pos % _bits));
        }

        virtual bool test(uint64_t pos)
        {
            return _data[pos / _bits] & (1 << (pos % _bits));
        }

        virtual T& at(uint64_t pos) { return _data[pos / _bits]; }

        [[nodiscard]] virtual uint64_t findFree(uint64_t amount)
        {
            uint64_t pos  = 0;
            uint64_t free = 0;
            while (free < amount && pos < _size) {
                uint64_t& data = at(pos);
                if (~data == 0) {
                    free  = 0;
                    pos  += _bits;
                    continue;
                } else if (!data) {
                    free += _bits;
                    pos  += _bits;
                } else {
                    /*
                        If the bit is set, then the free counter is
                        cleared, otherwise it is incremented.
                     */
                    free += (data & (1 << (pos % _bits))) ? -free : 1;
                    pos++;
                }

                if (free >= amount) {
                    pos -= free;
                    setAll(pos, amount);
                    return pos;
                }
            }
            return 0;
        }
    };

    template <typename T>
        requires Std::IsIntegral<T>
    struct BitmapDouble : Bitmap<T>
    {
        uint8_t _bits = sizeof(T) * 8;

        T**      _data;
        uint32_t _dimensionL1;
        uint32_t _dimensionL2;

        constexpr BitmapDouble(uint32_t dimensionL1, uint32_t dimensionL2)
          : _dimensionL1(dimensionL1)
          , _dimensionL2(dimensionL2)
        {
            _data    = new T*[_dimensionL2];
            _data[0] = new T[_dimensionL1];
        }

        ~BitmapDouble() override
        {
            for (uint32_t i = 0; i < _dimensionL2; i++) {
                delete[] _data[i];
            }
            delete[] _data;
        }

        void set(uint64_t pos) override { at(pos) |= (1 << (pos % _bits)); }

        void clear(uint64_t pos) override { at(pos) &= ~(1 << (pos % _bits)); }

        bool test(uint64_t pos) override
        {
            return at(pos) & (1 << (pos % _bits));
        }

        T& at(uint64_t pos) override
        {
            pos            /= _bits;
            uint32_t dimL1  = pos & _dimensionL1;
            uint32_t dimL2  = (pos / _dimensionL1) & _dimensionL2;

            if (!_data[dimL2]) {
                _data[dimL2] = new T[_dimensionL1];
            }
            return _data[dimL2][dimL1];
        }
    };
}