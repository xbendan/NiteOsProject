#include <siberix/core/runtimes.h>
#include <siberix/core/time.h>

Month months[] = {
    {"January",    "Jan", 1,  31},
    { "Febuary",   "Feb", 2,  28},
    { "March",     "Mar", 3,  31},
    { "April",     "Apr", 4,  30},
    { "May",       "May", 5,  31},
    { "June",      "Jun", 6,  30},
    { "July",      "Jul", 7,  31},
    { "August",    "Aug", 8,  31},
    { "September", "Sep", 9,  30},
    { "October",   "Oct", 10, 31},
    { "November",  "Nov", 11, 30},
    { "December",  "Dec", 12, 31}
};

void KernelExecutive::sleep(u64 ms) { m_defaultTimer->sleep(ms); }