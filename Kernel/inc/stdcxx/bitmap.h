#include <stdcxx/concepts.h>

namespace Std {

    template <typename T>
        requires Std::IsIntegral<T>
    struct Bitmap
    {
        UInt8  _bits = sizeof(T) * 8;
        UInt64 _size;
        T*     _data;

        constexpr Bitmap(UInt64 size)
          : _size(size)
        {
            _data = new T[size / _bits];
        }

        ~Bitmap() { delete[] _data; }

        virtual Void set(UInt64 pos)
        {
            _data[pos / _bits] |= (1 << (pos % _bits));
        }

        virtual Void setAll(UInt64 pos, UInt64 amount)
        {
            while (amount) {
                UInt8   bitsOffset = pos % _bits;
                UInt64& data       = getDataAt(pos);
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

        virtual Void clearAll(UInt64 pos, UInt64 amount)
        {
            while (amount) {
                UInt8   bitsOffset = pos % _bits;
                UInt64& data       = getDataAt(pos);
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

        virtual Void clear(UInt64 pos)
        {
            _data[pos / _bits] &= ~(1 << (pos % _bits));
        }

        virtual Boolean test(UInt64 pos)
        {
            return _data[pos / _bits] & (1 << (pos % _bits));
        }

        virtual T& getDataAt(UInt64 pos) { return _data[pos / _bits]; }

        virtual UInt64 findFree(UInt64 amount)
        {
            UInt64 pos  = 0;
            UInt64 free = 0;
            while (free < amount) {
                UInt64& data = getDataAt(pos);
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
        }
    };

    template <typename T>
        requires Std::IsIntegral<T>
    struct BitmapDouble : Bitmap<T>
    {
        // UInt8  _bits = sizeof(T) * 8;

        T**    _data;
        UInt32 _dimensionL1;
        UInt32 _dimensionL2;

        constexpr BitmapDouble(UInt32 dimensionL1, UInt32 dimensionL2)
          : _dimensionL1(dimensionL1)
          , _dimensionL2(dimensionL2)
        {
            _data    = new T*[_dimensionL2];
            _data[0] = new T[_dimensionL1];
        }

        ~BitmapDouble()
        {
            for (UInt32 i = 0; i < _dimensionL2; i++) {
                delete[] _data[i];
            }
            delete[] _data;
        }

        Void set(UInt64 pos) override
        {
            getDataAt(pos) |= (1 << (pos & _bits));
        }

        Void clear(UInt64 pos) override
        {
            getDataAt(pos) &= ~(1 << (pos & _bits));
        }

        Boolean test(UInt64 pos) override
        {
            return getDataAt(pos) & (1 << (pos & _bits));
        }

        T& getDataAt(UInt64 pos) override
        {
            pos          /= _bits;
            UInt32 dimL1  = pos & _dimensionL1;
            UInt32 dimL2  = (pos / _dimensionL1) & _dimensionL2;

            if (!_data[dimL2]) {
                _data[dimL2] = new T[_dimensionL1];
            }
            return _data[dimL2][dimL1];
        }
    };
}