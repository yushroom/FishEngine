#pragma once

#include "../helper.hpp"

namespace FishEngine
{
    template<class Archive, typename T>
    inline void Save (Archive& archive, std::list<T> const & v)
    {
        archive << make_size_tag(v.size());
        for (auto const & i : v)
            archive << i;
    }

    template<class Archive, typename T>
    inline void Load (Archive& archive, std::list<T> & v)
    {
       std::size_t size;
       archive >> make_size_tag(size);
       v.resize(size);
       for (auto & i : v)
           archive >> i;
    }
}
