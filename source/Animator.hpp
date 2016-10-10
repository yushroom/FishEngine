#ifndef Animator_hpp
#define Animator_hpp

#include "FishEngine.hpp"
#include "Component.hpp"
#include "Vector3.hpp"
#include "Quaternion.hpp"
#include "Matrix4x4.hpp"

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

    struct TransformationKey
    {
        float time;
        Matrix4x4 value;
    };
    
    struct AnimationNode
    {
        std::string name;
        std::vector<Vector3Key> positionKeys;
        std::vector<QuaternionKey> rotationKeys;
        std::vector<Vector3Key> scalingKeys;
        //std::vector<TransformationKey> transformationKeys;
    };
    
    struct Animation
    {
        typedef std::shared_ptr<Animation> PAnimation;
        std::string name;
        float duration;
        float ticksPerSecond;
        std::map<std::string, AnimationNode> channels;
    };
    
    struct Avatar
    {
        std::map<std::string, int> m_boneToIndex;
    };

    class Animator : public Component
    {
    public:
        InjectClassName(Animator);
        
        float m_time;   // temp
        bool m_playing = false;
        bool m_playingOnce = false;
        bool m_playOneFrame = false;
        int m_currentFrame = 0;
        std::shared_ptr<Animation> m_animation;
        
        //virtual void OnInspectorGUI() override;
        
        void Play() {
            m_playing = true;
            m_playingOnce = false;
            m_playOneFrame = false;
        }
        
        void PlayOnce() {
            m_playing = true;
            m_playingOnce = true;
            m_playOneFrame = false;
        }

        void NextFrame();
        
        void Stop() {
            m_playing = false;
            m_playingOnce = false;
            m_playOneFrame = false;
            //m_time = 0;
        }
        
        virtual void Update() override;

        std::shared_ptr<Avatar>& avatar() {
            return m_avatar;
        }

        void setAvatar(std::shared_ptr<Avatar> avatar) {
            m_avatar = avatar;
        }
        
    private:
        std::shared_ptr<Avatar> m_avatar;
        void RecursivelyUpdate(const std::shared_ptr<GameObject>& go);
        //void RecursivelyUpdate2(const std::shared_ptr<GameObject>& go);
    };
}


#endif //Animator_hpp
