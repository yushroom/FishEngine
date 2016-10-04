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
    
    /********** Editor **********/
    
    // Reset is called in the Editor when the script is attached or reset
    virtual void Reset() {}
    
    
    /********** Initializtion **********/
    
    // Awake is called when the script instance is being loaded.
    virtual void Awake() {}
    //
    virtual void OnEnable() {}
    // Start is only called once for a given script
    virtual void Start() {}

    
    /********** Physics **********/
    
    //
    virtual void FixedUpdate() {}

    
    /********** Input events **********/
    
    
    /********** Game Logic **********/

    //virtual void Update() {}

    virtual void LateUpdate() {}

    /********** Scene rendering **********/
    //virtual void OnInspectorGUI() {};
    
    /********** Gizmo rendering **********/
    virtual void OnDrawGizmos() {}
    virtual void OnDrawGizmosSelected() {}
    
    /********** GUI rendering **********/
    virtual void OnGUI() {}
    
    /********** End of frame **********/
    
    /********** Pausing **********/
    virtual void OnApplicationPause() {}
    
    /********** Disable/enable **********/
    virtual void OnDisable() {}
    
    /********** Decommissioning **********/
    virtual void OnApplicationQuit() {};
    //virtual void OnDisable() {}
    virtual void OnDestroy() {}
};

NAMESPACE_FISHENGINE_END

#endif // Script_hpp
