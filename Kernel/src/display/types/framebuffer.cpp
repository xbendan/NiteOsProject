#include <siberix/display/types/framebuffer.hpp>

FramebufferVideoOutput::FramebufferVideoOutput() {}

FramebufferVideoOutput::~FramebufferVideoOutput() {}

void FramebufferVideoOutput::drawRect(Point point,
                                      u32   width,
                                      u32   height,
                                      Color color) {}

void FramebufferVideoOutput::drawEllipse(Point point,
                                         u32   width,
                                         u32   height,
                                         Color) {}

void FramebufferVideoOutput::drawText(Point       point,
                                      const char* text,
                                      Color       color) {}