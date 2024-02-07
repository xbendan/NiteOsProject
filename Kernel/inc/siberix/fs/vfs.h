#include <siberix/fs/file.h>
#include <siberix/fs/volume.h>
#include <stdcxx/linked_list.h>

namespace Kern::Io {
    class VirtualFileSystem
    {
    public:
        VirtualFileSystem();
        ~VirtualFileSystem();

        RootFsNode* getRoot() { return m_root; }
        FsNode*     getPath(Std::String<Utf16> path);
        Volume*     getVolume(uint64_t volumeId);
        Volume*     getVolume(FsNode* mountPoint);

    private:
        RootFsNode*             m_root;
        Std::LinkedList<Volume> m_volumes;
    };
}