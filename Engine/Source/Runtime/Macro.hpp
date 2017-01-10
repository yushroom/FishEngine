#pragma once

//#define InjectSerializationFunctions(T) \
//    template <typename Archive> \
//    friend Archive & operator << (Archive & archive, T const & t); \
//    \
//    template <typename Archive> \
//    friend Archive & operator >> (Archive & archive, T & t);

#define InjectSerializationFunctions(T) \
    template <typename Archive> \
    friend void Save (Archive & archive, T const & t); \
    \
    template <typename Archive> \
    friend void Load (Archive & archive, T & t);

#define InjectClassName(T) \
    static const std::string StaticClassName() { return #T; }  \
    virtual const std::string ClassName() const override { return StaticClassName(); } \
    InjectSerializationFunctions(T)
