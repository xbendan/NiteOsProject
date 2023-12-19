#include <stdcxx/types.h>
#include <xtra-concurrent/atomic.h>

namespace Xtra::Concurrent {
    class Spinlock
    {
    public:
        Boolean acquireTest()
        {
            Boolean result = m_lock.compareAndExchange(false, true);
            // Memory barrier

            if (!result) {
                // leave interrupt disabled
            }
            return result;
        }

        Boolean acquire()
        {
            while (!acquireTest()) {
                // wait
            }
        }

        Void release()
        {
            // Memory barrier
            m_lock.store(false);
            // Leave interrupt disabled
        }

    private:
        Atomic<Boolean> m_lock{};
    };
}