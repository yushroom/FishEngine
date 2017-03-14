#pragma once

//#define InjectSerializationFunctions(T) \
//    template <typename Archive> \
//    friend Archive & operator << (Archive & archive, T const & t); \
//    \
//    template <typename Archive> \
//    friend Archive & operator >> (Archive & archive, T & t);

#define InjectSerializationFunctions(T) \
	virtual void Serialize(FishEngine::OutputArchive & archive) const override; \
	virtual void Deserialize(FishEngine::InputArchive & archive) override; \

#define InjectSerializationFunctionsNonPolymorphic(T) \
	friend FishEngine::OutputArchive & operator << (FishEngine::OutputArchive & archive, T const & t); \
	friend FishEngine::InputArchive & operator >> (FishEngine::InputArchive & archive, T & t); \

#define InjectClassName(T) \
	static constexpr const char * StaticClassName() { return #T; }  \
	virtual const std::string ClassName() const override { return StaticClassName(); } \
	virtual int ClassID() const override { return FishEngine::ClassID<T>(); } \
	InjectSerializationFunctions(T)

#define DefineComponent(T) \
	friend class CloneUtility; \
	InjectClassName(T) \
	virtual FishEngine::ComponentPtr Clone(FishEngine::CloneUtility & cloneUtility) const override; \
	void CopyValueTo(std::shared_ptr<T> target, FishEngine::CloneUtility & cloneUtility) const;
