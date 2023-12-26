#include <siberix/fs/file.h>
#include <stdcxx/types.h>

namespace Kern::Io {
    class Volume
    {
    public:
        inline uint64_t getVolumeId() const { return m_volumnId; }
        FsNode*         getMountPoint() const { return m_mountPoint; }
        bool            mount(FsNode* mountPoint);
        bool            unmount();

    private:
        uint64_t m_volumnId;
        FsNode*  m_mountPoint;
    };
}