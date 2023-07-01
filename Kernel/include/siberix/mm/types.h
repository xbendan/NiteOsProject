#include <common/typedefs.h>

enum AddressSegmentType
{
    AST_AVAILABLE,
    AST_KERNEL,
    AST_RESERVED,
    AST_BADRAM
};

struct AddressSegment
{
    u64 start, end;
    AddressSegmentType type;

    AddressSegment(u64 start, u64 end, AddressSegmentType type)
      : start(start),
        end(end),
        type(type) { }
};
