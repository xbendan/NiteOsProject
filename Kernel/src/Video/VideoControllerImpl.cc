#include <Video/Video.h>
#include <Video/TextModePresenter.h>

namespace Video
{
    VideoController::VideoController()
      : m_VideoMode(new TextModePresenter())
    {
    
    }

    VideoController::~VideoController()
    {

    }
} // namespace Video
