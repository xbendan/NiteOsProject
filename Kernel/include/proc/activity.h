#pragma once

#include <utils/list.h> 

namespace Task
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
    Activity *CreateActivity(
        const char*                     name);
}


