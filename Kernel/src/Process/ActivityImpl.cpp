#include <Proc/Activity.h>
#include <Proc/Process.h>
#include <System.h>
#include <macros>

namespace Task
{
    Activity::Activity(
        const char *name,
        Process *proc) : m_Name(name),
                         m_ChildrenProcessList(LinkedList<Process *>())
    {
        Add(proc);
    }

    Activity::Activity(Process *proc) : Activity(proc->m_Name, proc) {}

    void Activity::Add(
        Process *proc)
    {
        if (!Objects::IsNull(proc->m_Activity) || Objects::Equals(proc->m_Activity, this))
        {
            return;
        }

        this->m_ChildrenProcessList.Add(proc);
    }

    Activity *Activity::CreateIfNull(
        Process *proc)
    {
        if (Objects::IsNull(proc->m_Activity))
        {
            proc->m_Activity = new Activity(proc);
        }

        return proc->m_Activity;
    }
} // namespace Task
