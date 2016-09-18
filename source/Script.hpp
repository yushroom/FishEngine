#ifndef Script_hpp
#define Script_hpp

#include "Behaviour.hpp"

NAMESPACE_FISHENGINE_BEGIN

// Script is the base class every script derives from.
// http://docs.unity3d.com/ScriptReference/MonoBehaviour.html
// http://docs.unity3d.com/Manual/ExecutionOrder.html
class Script : public Behaviour
{
public:

    virtual ~Script() = default;

    //InjectClassName(Script);
    //virtual onEnable();

    // Awake is called when the script instance is being loaded.
    virtual void Awake() {}

    virtual void Start() {};

    virtual void FixedUpdate() {};

    virtual void Update() {};

    virtual void LateUpdate() {};

    //virtual void OnGUI() {};
};

NAMESPACE_FISHENGINE_END

#endif // Script_hpp