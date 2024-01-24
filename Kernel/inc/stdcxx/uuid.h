#pragma once

#include <stdcxx/string.h>
#include <stdcxx/types.h>

namespace Std {
    struct UUID
    {
        union
        {
            uint8_t  _bytes[16];
            uint16_t _words[8];
            uint32_t _dwords[4];
            uint64_t _qwords[2];
        };

        inline bool operator==(const UUID& uuid) const
        {
            return this->_qwords[0] == uuid._qwords[0] &&
                   this->_qwords[1] == uuid._qwords[1];
        }

        // String<Utf8> getAsString() {}
    };
}