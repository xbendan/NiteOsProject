#pragma once

#include <Utils/LinkedList.h> 

namespace Task
{
    class Process;

    class Activity
    {
        const char *m_Name;
        const char *m_Description;
        LinkedList<Process *> m_ChildrenProcessList;

    public:
        Activity(
            const char                 *name,
            Process                    *proc);

        Activity(
            Process                    *proc);
        
        /**
         * @brief Add process to this activity
         * If the process already owned an activity, this function
         * do nothing and return.
         * 
         * @param proc 
         */
        void Add(
            Process                    *proc);
        
        /**
         * Get the activity of process, if it doesn't exist. This 
         * function will create one and return it.
         * 
         * @param proc 
         * @return Activity* 
         */
        static Activity *CreateIfNull(
            Process                    *proc);
    };

    extern Activity g_SystemActivity;
}