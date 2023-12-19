#include <stdcxx/string.h>
#include <stdcxx/types.h>

namespace Std {
    struct UUID
    {
        union
        {
            UInt8  _bytes[16];
            UInt16 _words[8];
            UInt32 _dwords[4];
            UInt64 _qwords[2];
        };

        inline bool operator==(const UUID& uuid) const
        {
            return this->_qwords[0] == uuid._qwords[0] &&
                   this->_qwords[1] == uuid._qwords[1];
        }

        String<Utf8> getAsString() {}
    };
}