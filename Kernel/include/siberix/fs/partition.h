class Partition {
private:
    const char* m_name;
    u8          m_diskId;
    u8          m_partitionId;
    u64         m_capacity;
    u64         m_spaceUsed;
};