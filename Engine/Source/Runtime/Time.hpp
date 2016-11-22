#ifndef Time_hpp
#define Time_hpp

#include "FishEngine.hpp"
#include <time.h>
#include <chrono>

namespace FishEngine
{
    class FE_EXPORT Time
    {
    public:
        Time() = delete;

        // The time in seconds it took to complete the last frame (Read Only).
        static float deltaTime() {
            return m_deltaTime;
        }

        // The interval in seconds at which physics and other fixed frame rate updates (like MonoBehaviour's FixedUpdate) are performed.
        static float fixedDeltaTime() {
            return m_fixedDeltaTime;
        }

        // The time at the beginning of this frame (Read Only). This is the time in seconds since the start of the game.
        static float time() {
            return m_time;
        }

        // The scale at which the time is passing. This can be used for slow motion effects.
        static float timeScale() {
            return m_timeScale;
        }

    private:
        friend class RenderSystem;
        friend class GameLoop;
        friend class FishEditor::FishEditorWindow;

        static float m_deltaTime;
        static float m_fixedDeltaTime;
        static float m_time;
        static float m_timeScale;
    };
}

#endif /* Time_hpp */
