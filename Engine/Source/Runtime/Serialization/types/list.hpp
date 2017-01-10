#pragma once

namespace FishEngine
{
    template<class Archive, typename T>
    inline void Save (Archive& archive, std::list<T> const & v)
    {
        archive << v.size();
        for (auto const & i : v)
            archive << i;
    }

    template<class Archive, typename T>
    static Archive & operator >> (Archive& archive, std::list<T> & v)
    {
        std::size_t size;
        archive >> size;
        v.resize(size);
        for (auto & i : v)
            archive >> i;
        return archive;
    }
}
