#pragma once

enum AtomicOpcode
{
    Relaxed                = __ATOMIC_RELAXED,
    Consume                = __ATOMIC_CONSUME,
    Acquired               = __ATOMIC_ACQUIRE,
    Release                = __ATOMIC_RELEASE,
    AcquireRelease         = __ATOMIC_ACQ_REL,
    SequentiallyConsistent = __ATOMIC_SEQ_CST
};

template <typename T>
class Atomic
{
public:
    Atomic(T const& val)
      : m_value(val)
    {
    }

    Atomic(T&& val)
      : m_value(val)
    {
    }

    T exchange(T            desired,
               AtomicOpcode order = AtomicOpcode::SequentiallyConsistent)
    {
        return __atomic_exchange_n(&m_value, desired, order);
    }

    bool compareAndExchange(
      T            expected,
      T            desired,
      AtomicOpcode order = AtomicOpcode::SequentiallyConsistent)
    {
        if (order == AcquireRelease || order == Relaxed)
            return __atomic_compare_exchange_n(
              &m_value, &expected, desired, false, Relaxed, Acquired);

        return __atomic_compare_exchange_n(
          &m_value, &expected, desired, false, order, order);
    }

    T fetchAdd(T val, AtomicOpcode order = AtomicOpcode::SequentiallyConsistent)
    {
        return __atomic_fetch_add(&m_value, val, order);
    }

    T fetchSub(T val, AtomicOpcode order = AtomicOpcode::SequentiallyConsistent)
    {
        return __atomic_fetch_sub(&m_value, val, order);
    }

    T fetchAnd(T val, AtomicOpcode order = AtomicOpcode::SequentiallyConsistent)
    {
        return __atomic_fetch_and(&m_value, val, order);
    }

    T fetchOr(T val, AtomicOpcode order = AtomicOpcode::SequentiallyConsistent)
    {
        return __atomic_fetch_or(&m_value, val, order);
    }

    T fetchXor(T val, AtomicOpcode order = AtomicOpcode::SequentiallyConsistent)
    {
        return __atomic_fetch_xor(&m_value, val, order);
    }

    T fetchInc(AtomicOpcode order = AtomicOpcode::SequentiallyConsistent)
    {
        return __atomic_fetch_add(&m_value, 1, order);
    }

    T fetchDec(AtomicOpcode order = AtomicOpcode::SequentiallyConsistent)
    {
        return __atomic_fetch_sub(&m_value, 1, order);
    }

    void inc(AtomicOpcode order = AtomicOpcode::SequentiallyConsistent)
    {
        __atomic_add_fetch(&m_value, 1, order);
    }

    void dec(AtomicOpcode order = AtomicOpcode::SequentiallyConsistent)
    {
        __atomic_sub_fetch(&m_value, 1, order);
    }

    T load(AtomicOpcode order = AtomicOpcode::SequentiallyConsistent)
    {
        return __atomic_load_n(&m_value, order);
    }

    void store(T val, AtomicOpcode order = AtomicOpcode::SequentiallyConsistent)
    {
        __atomic_store_n(&m_value, val, order);
    }

    bool isLockFree() { return __atomic_is_lock_free(sizeof(T), &m_value); }

private:
    T m_value;
};