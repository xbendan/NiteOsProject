#pragma once

#include <utils/list.h> 

namespace Proc
{
    typedef struct Activity
    {
        char *m_Name;
        uint16_t m_ProcessCount;
    } activity_t;

    /**
     * @brief 
     * 
     * @param name 
     * @return activity_t* 
     */
    Activity *NewActivity(const char* name);
} // namespace Proc


