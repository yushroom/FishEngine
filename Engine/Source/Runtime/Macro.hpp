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
    static constexpr const char * StaticClassName() { return #T; }  \
    virtual const std::string ClassName() const override { return StaticClassName(); } \
	virtual int ClassID() const override { return FishEngine::ClassID<T>(); } \
    InjectSerializationFunctions(T)

