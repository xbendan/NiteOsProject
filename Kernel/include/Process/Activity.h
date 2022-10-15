#pragma once

#include <Utils/List.h> 

typedef struct Activity
{
    char *m_Name;
    uint16_t m_ProcessCount;
    lklist_node_t listnode;
} activity_t;

/**
 * @brief 
 * 
 * @param name 
 * @return activity_t* 
 */
struct Activity *NewActivity(const char* name);