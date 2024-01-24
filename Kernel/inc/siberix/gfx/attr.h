namespace Kern::Gfx {
    enum DisplayCapability
    {
        ColorVGA    = 0x1,
        Color8Bit   = 0x2,
        Color10Bit  = 0x4,
        Color12Bit  = 0x8,
        HDRDisplay  = 0x10,
        TextFormat  = 0x20,
        PixelFormat = 0x40,
        Gamma       = 0x80,
        Brightness  = 0x100,
        Contrast    = 0x200,
        Hue         = 0x400,
        Saturation  = 0x800,
        Sharpness   = 0x1000,
    };
}