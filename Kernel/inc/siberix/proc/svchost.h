#include <siberix/proc/process.h>
#include <siberix/proc/sched.h>
#include <siberix/svc/svc-host.h>

namespace Kern::Svc {
    using namespace Kern::Task;

    class TaskSvcHost : public Svc::ISvcHost
    {
    public:
        TaskSvcHost()
          : ISvcHost("KERN.TASK", nullptr)
          , m_scheduler(new Scheduler())
          , m_processFactory(nullptr)
        {
        }
        virtual ~TaskSvcHost() = default;

        Scheduler*      getScheduler() { return m_scheduler; }
        ProcessFactory* getProcessFactory() { return m_processFactory; }
        RefPtr<Process> getProcess(uint32_t pid) { return m_processes[pid]; }
        RefPtr<Process> getProcess(Std::String<Utf8> name);
        RefPtr<Process> getProcess(Std::UUID uuid);

        void onLoad() override;
        void onEnable() override;
        void onDisable() override;

    private:
        Scheduler*                  m_scheduler;
        ProcessFactory*             m_processFactory;
        Std::Array<RefPtr<Process>> m_processes;
    };
}