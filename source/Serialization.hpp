#ifndef Serialization_hpp
#define Serialization_hpp

#include "FishEngine.hpp"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

namespace boost {
    namespace serialization {
        //template<class Archive>
        //void serialize
    }
}

namespace FishEngine
{
    class Serialization
    {
    public:
        Serialization() = delete;

    };
}

#endif //Serialization_hpp
