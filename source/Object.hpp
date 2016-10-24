#ifndef Object_hpp
#define Object_hpp

#include "FishEngine.hpp"
#include <string>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_serialize.hpp>
#include "Serialization.hpp"

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
        static void Destroy(PGameObject obj, const float t = 0.0f);

        // Removes a component.
        static void Destroy(PComponent component, float t = 0.0f);

        static void Destroy(PScript script, float t = 0.0f);

        static void DestroyImmediate(PGameObject obj);
        // Removes a component.
        static void DestroyImmediate(PComponent component);
        static void DestroyImmediate(PScript script);

    protected:
        std::string m_name;
        UUID        m_uuid;

    private:
        friend class boost::serialization::access;
        template<class Archive>
        inline void serialize(Archive& ar, const unsigned int version)
        {
            ar & BOOST_SERIALIZATION_NVP(m_name);
            ar & BOOST_SERIALIZATION_NVP(m_uuid);
        }
    };
}

#endif // Object_hpp
