#ifndef Animator_hpp
#define Animator_hpp

#include "FishEngine.hpp"
#include "Component.hpp"
#include "Vector3.hpp"
#include "Quaternion.hpp"

namespace FishEngine {
    
    
    struct Vector3Key
    {
        float time;
        Vector3 value;
    };
    
    struct QuaternionKey
    {
        float time;
        Quaternion value;
    };
    
    struct AnimationNode
    {
        std::string name;
        std::vector<Vector3Key> positionKeys;
        std::vector<QuaternionKey> rotationKeys;
        std::vector<Vector3Key> scalingKeys;
    };
    
    struct Animation
    {
        typedef std::shared_ptr<Animation> PAnimation;
        std::string name;
        float duration;
        float ticksPerSecond;
        std::map<std::string, AnimationNode> channels;
    };
    
    class Animator : public Component
    {
    public:
        InjectClassName(Animator);
        
        float m_time;   // temp
        bool m_playing = false;      // false
        std::shared_ptr<Animation> m_animation;
        
        virtual void OnInspectorGUI() override;
        
        void Play() {
            
        }
        
        void PlayOnce() {
            m_playing = true;
            m_time = 0;
        }
        
        void Stop() {
            m_playing = false;
            //m_time = 0;
        }
        
        virtual void Update() override;
        
        void RecursivelyUpdate(const std::shared_ptr<GameObject>& go);
    };
}


#endif //Animator_hpp
