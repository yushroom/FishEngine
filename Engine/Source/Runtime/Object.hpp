#ifndef Object_hpp
#define Object_hpp

#include "FishEngine.hpp"
#include <string>
#include <boost/uuid/uuid.hpp>
#include "Macro.hpp"
//#include <Archive.hpp>

namespace FishEngine
{
    using UUID = boost::uuids::uuid;
	
	class InputArchive;
	class OutputArchive;
	
	enum class HideFlags
	{
		//A normal, visible object. This is the default.
		None,
		
		//The object will not appear in the hierarchy.
		HideInHierarchy,
		
		//It is not possible to view it in the inspector.
		HideInInspector,
		
		//The object will not be saved to the scene in the editor.
		DontSaveInEditor,
		
		//The object is not be editable in the inspector.
		NotEditable,
		
		//The object will not be saved when building a player.
		DontSaveInBuild,
		
		//The object will not be unloaded by Resources.UnloadUnusedAssets.
		DontUnloadUnusedAsset,
		
		//The object will not be saved to the scene. It will not be destroyed when a new scene is loaded. It is a shortcut for HideFlags.DontSaveInBuild | HideFlags.DontSaveInEditor | HideFlags.DontUnloadUnusedAsset.
		DontSave,
		
		//A combination of not shown in the hierarchy, not saved to to scenes and not unloaded by The object will not be unloaded by Resources.UnloadUnusedAssets.
		HideAndDontSave,
	};
    
    class FE_EXPORT Object
    {
    public:
        Object();
        virtual ~Object() = 0;

        static const std::string StaticClassName()
        {
            return "Object";
        }

        virtual const std::string ClassName() const
        {
            return "Object";
        }
		
		virtual int ClassID() const = 0;
        
        UUID GetGUID() const
        {
            return m_uuid;
        }
        
        //InjectSerializationFunctions(Object);
		template <typename Archive>
		friend void Save (Archive & archive, Object const & t);
		template <typename Archive>
		friend void Load (Archive & archive, Object & t);
		virtual void Serialize(OutputArchive & archive) const;
		virtual void Deserialize(InputArchive & archive);
		
        // The name of the object.
        virtual std::string name() const { return m_name; }
        void setName(const std::string& name) { m_name = name; }

        //virtual std::string ToString() const;

        // Removes a gameobject.
        static void Destroy(GameObjectPtr obj, const float t = 0.0f);

        // Removes a component.
        static void Destroy(ComponentPtr component, float t = 0.0f);

        static void DestroyImmediate(GameObjectPtr obj);
        // Removes a component.
        static void DestroyImmediate(ComponentPtr component);
        static void DestroyImmediate(ScriptPtr script);

	protected:
		// Should the object be hidden, saved with the scene or modifiable by the user?
		HideFlags	m_hideFlags;
        std::string m_name;
        UUID        m_uuid;

    };
}

#endif // Object_hpp
