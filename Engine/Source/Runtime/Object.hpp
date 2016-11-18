#ifndef Object_hpp
#define Object_hpp

#include "FishEngine.hpp"
#include <string>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_serialize.hpp>
//#include <boost/serialization/nvp.hpp>
//#include <boost/serialization/base_object.hpp>

namespace FishEngine
{
    using UUID = boost::uuids::uuid;
    
    class Object
    {
    public:
        Object();
        virtual ~Object() = 0;
        
        // The name of the object.
        virtual std::string name() const { return m_name; }
        void setName(const std::string& name) { m_name = name; }


        //virtual std::string ToString() const;

        // Removes a gameobject.
        static void Destroy(GameObjectPtr obj, const float t = 0.0f);

        // Removes a component.
        static void Destroy(ComponentPtr component, float t = 0.0f);

        static void Destroy(ScriptPtr script, float t = 0.0f);

        static void DestroyImmediate(GameObjectPtr obj);
        // Removes a component.
        static void DestroyImmediate(ComponentPtr component);
        static void DestroyImmediate(ScriptPtr script);

    protected:
        std::string m_name;
        UUID        m_uuid;

        friend class Serialization;

    //private:
    //    friend class boost::serialization::access;
    //    template<class Archive>
    //    inline void serialize(Archive& ar, const unsigned int version)
    //    {
    //        ar & BOOST_SERIALIZATION_NVP(m_name);
    //        ar & BOOST_SERIALIZATION_NVP(m_uuid);
    //    }
    };
}

#endif // Object_hpp
