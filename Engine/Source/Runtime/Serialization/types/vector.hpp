#pragma once

namespace FishEngine
{
    /************************************************************************/
    /* std::vector                                                          */
    /************************************************************************/
    
    //! Serialization for std::vectors of arithmetic (but not bool) using binary serialization
    template<typename T, typename std::enable_if_t<std::is_arithmetic<T>::value && !std::is_same<T, bool>::value, int> = 0>
    static OutputArchive & operator << (OutputArchive& archive, const std::vector<T> & v)
    {
        archive << v.size();
        archive.SaveBinary(v.data(), v.size() * sizeof(T));
    }
    
    // std::vectors of arithmetic (but not bool)
    template<typename T, typename std::enable_if_t<std::is_arithmetic<T>::value && !std::is_same<T, bool>::value, int> = 0>
    static void Deserialize(InputArchive& archive, std::vector<T> & v)
    {
        std::size_t size;
        archive >> size;
        v.resize(size);
        archive.LoadBinary(v.data(), size);
    }
    
    //! Serialization for non-arithmetic vector types
    template<typename T, typename std::enable_if_t<!std::is_arithmetic<T>::value, int> = 0>
    static OutputArchive & operator << (OutputArchive& archive, const std::vector<T> & v)
    {
        archive << v.size();
        for (auto && x : v)
            archive << x;
    }
    
    // non-arithmetic vector types
    template<typename T, typename std::enable_if_t<!std::is_arithmetic<T>::value, int> = 0>
    static void Deserialize(InputArchive& archive, std::vector<T> & v)
    {
        std::size_t size;
        archive >> size;
        v.resize(size);
        for (auto && x : v)
            archive >> x;
    }
}