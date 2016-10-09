#ifndef Serialization_hpp
#define Serialization_hpp

#include "FishEngine.hpp"
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/base_object.hpp>
#include "Vector3.hpp"
#include "Quaternion.hpp"

namespace boost {
    namespace serialization {
        template<class Archive>
        inline void serialize(Archive& ar, FishEngine::Vector3& v, const unsigned int version)
        {
            auto& x = v.x;  // hack
            auto& y = v.y;
            auto& z = v.z;
            ar & BOOST_SERIALIZATION_NVP(x)
               & BOOST_SERIALIZATION_NVP(y)
               & BOOST_SERIALIZATION_NVP(z);
        }
        
        template<class Archive>
        inline void serialize(Archive& ar, FishEngine::Quaternion& q, const unsigned int version)
        {
            auto& x = q.x;  // hack
            auto& y = q.y;
            auto& z = q.z;
            auto& w = q.w;
            ar  & BOOST_SERIALIZATION_NVP(x)
                & BOOST_SERIALIZATION_NVP(y)
                & BOOST_SERIALIZATION_NVP(z)
                & BOOST_SERIALIZATION_NVP(w);
        }
        
        template<class Archive>
        inline void serialize(Archive& ar, FishEngine::Vector4& v, const unsigned int version)
        {
            auto& x = v.x;  // hack
            auto& y = v.y;
            auto& z = v.z;
            auto& w = v.w;
            ar  & BOOST_SERIALIZATION_NVP(x)
            & BOOST_SERIALIZATION_NVP(y)
            & BOOST_SERIALIZATION_NVP(z)
            & BOOST_SERIALIZATION_NVP(w);
        }
    }
}

namespace FishEngine
{
    class Serialization
    {
    public:
        Serialization() = delete;
        
        template<typename T>
        static void Serialize(T object);
        
//        template<typename T>
//        void Deserialize();
    };
}

#endif //Serialization_hpp
