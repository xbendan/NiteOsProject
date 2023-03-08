#include <proc/activity.h>
#include <proc/proc.h>
#include <macros>

namespace Task
{
    Activity::Activity(
        const char                     *name,
        Process                        *proc
    ) : m_Name(name) {
        Add(proc);
    }

    Activity::Activity(
        Process                    *proc
    ) : m_Name(proc->m_Name) { 
        Add(proc);
    }

    void Activity::Add(
        Process                        *proc
    ) {
        if (!Objects::IsNull(proc->m_Activity) || Objects::Equals(proc->m_Activity, this)) {
            return;
        }

        this->m_ChildrenProcessList.Add(proc);
    }

    Activity *Activity::CreateIfNull(
        Process                        &proc
    ) {
        if (Objects::IsNull(proc.m_Activity))
        {
            proc.m_Activity = new Activity(&proc);
        }
        
        return proc.m_Activity;
    }
} // namespace Task
