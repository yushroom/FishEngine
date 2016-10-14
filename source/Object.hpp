#ifndef Object_hpp
#define Object_hpp

#include "FishEngine.hpp"
#include <string>
#include "Serialization.hpp"

namespace FishEngine
{
    class Object
    {
    public:
        // The name of the object.
        virtual std::string name() const { return m_name; }
        void setName(const std::string& name) { m_name = name; }

        virtual ~Object() = 0;

        //virtual std::string ToString() const;

        // Removes a gameobject.
        static void Destroy(std::shared_ptr<GameObject> obj, const float t = 0.0f);

        // Removes a component.
        static void Destroy(std::shared_ptr<Component> component, float t = 0.0f);

        static void Destroy(std::shared_ptr<Script> script, float t = 0.0f);

        // Removes a component.
        static void DestroyImmediate(std::shared_ptr<Component> component);

        static void DestroyImmediate(std::shared_ptr<Script> script);

    protected:
        std::string m_name;

    private:
        friend class boost::serialization::access;
        template<class Archive>
        inline void serialize(Archive& ar, const unsigned int version)
        {
            ar & BOOST_SERIALIZATION_NVP(m_name);
        }
    };
}

#endif // Object_hpp
