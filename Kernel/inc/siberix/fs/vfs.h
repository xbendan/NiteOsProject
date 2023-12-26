#include <siberix/fs/file.h>
#include <siberix/fs/volume.h>
#include <stdcxx/linked-list.h>

namespace Kern::Io {
    class VirtualFileSystem
    {
    public:
        VirtualFileSystem();
        ~VirtualFileSystem();

        RootFsNode& getRoot() { return m_root; }
        Volume*     getVolume(uint64_t volumeId);
        Volume*     getVolume(FsNode* mountPoint);

    private:
        RootFsNode               m_root;
        Std::LinkedList<Volume*> m_volumes;
    };
}