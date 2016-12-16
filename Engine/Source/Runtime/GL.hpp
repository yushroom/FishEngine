#include "Color.hpp"

namespace FishEngine
{
    // Low-level graphics library.
    class GL
    {
    public:
        GL() = delete;

        // Clear the current render buffer.
        static void Clear(bool clearDepth, bool clearColor, Color backgroundColor, float depth = 1.0f);

        // Sends queued-up commands in the driver's command buffer to the GPU.
        static void Flush();
    };
}