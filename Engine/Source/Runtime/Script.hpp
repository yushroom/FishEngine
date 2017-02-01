#ifndef Script_hpp
#define Script_hpp

#include "Behaviour.hpp"

namespace FishEngine
{

    // Script is the base class every script derives from.
    // http://docs.unity3d.com/ScriptReference/MonoBehaviour.html
    // http://docs.unity3d.com/Manual/ExecutionOrder.html
    class FE_EXPORT Script : public Behaviour
    {
    public:

        InjectClassName(Script)

        virtual ~Script() = default;

        //InjectClassName(Script);

        /********** Editor **********/

        // Reset is called in the Editor when the script is attached or reset
        virtual void Reset() {}


        /************************************************************************/
        /*                           Initialization                             */
        /************************************************************************/

        // Awake is called when the script instance is being loaded.
        // This function is always called before any Start functions and
        // also just after a prefab is instantiated.
        // (If a GameObject is inactive during start up Awake
        // is not called until it is made active.)
        virtual void Awake() {}

        // (only called if the Object is active): This function
        // is called just after the object is enabled.
        // This happens when a MonoBehaviour instance is created,
        // such as when a level is loaded or a GameObject with the script component is instantiated.
        virtual void OnEnable() {}

        // Start is only called once for a given script
        // Start is called before the first frame update only if the script instance is enabled.
		virtual void Start() override {}


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
        //virtual void OnDrawGizmos() {}
        //virtual void OnDrawGizmosSelected() {}

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
        //virtual void OnDestroy() {}
    };
}

#endif // Script_hpp
