typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;
typedef unsigned long  u64;

#ifndef TYPE_INTEGERS
#define TYPE_INTEGERS
typedef unsigned char  UInt8;
typedef unsigned short UInt16;
typedef unsigned int   UInt32;
typedef unsigned long  UInt64;
typedef char           Int8;
typedef short          Int16;
typedef int            Int32;
typedef long           Int64;
#endif

#ifndef TYPE_VOID
#define TYPE_VOID
struct Void
{};
#endif