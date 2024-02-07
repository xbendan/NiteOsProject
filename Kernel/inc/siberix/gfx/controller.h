#include <siberix/dvc/device.h>
#include <siberix/gfx/device.h>
#include <stdcxx/linked_list.h>

namespace Kern::Gfx {
    enum class VideoProjection
    {
        Primary,    // Output the graphics to the primary display only
        NonPrimary, // Output the graphics to all displays except the primary
        Extended,   // Consider all displays as one combined large display
        Duplicated, // Consider all displays as one display with duplicated
                    // content, probably with different resolutions
    };

    class GfxOutputControllerDevice : Device
    {
    public:
        GfxOutputControllerDevice()
          : Device("Siberix Basic Video Controller", DeviceType::DisplayAdapter)
          , m_outputs(new Std::LinkedList<IVideoOutputDevice>())
          , m_projection(VideoProjection::Primary)
        {
        }

    private:
        Std::LinkedList<IVideoOutputDevice>* m_outputs;
        IVideoOutputDevice*                  m_primaryOutput;
        VideoProjection                      m_projection;
    };
}