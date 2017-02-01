#ifndef EditorTime_hpp
#define EditorTime_hpp

#include "FishEditor.hpp"
#include <time.h>
#include <chrono>

namespace FishEditor {
	
	class FishEditorWindow;
	
    class EditorTime
    {
    public:
        EditorTime() = delete;
        
        static float deltaTime() {
            return m_deltaTime;
        }
        
        static float time() {
            return m_time;
        }
        
    private:
        friend class FishEditorWindow;
        
        static float m_deltaTime;
        //static float m_fixedDeltaTime;
        static float m_time;
        //static float m_timeScale;
    };

}

#endif /* EditorTime_hpp */
