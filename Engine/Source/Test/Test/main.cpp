#include <iostream>
#include <sstream>

template<class T>
class NameValuePair
{
private:
	// If we get passed an array, keep the type as is, otherwise store
	// a reference if we were passed an l value reference, else copy the value
	using Type = typename std::conditional<std::is_array<typename std::remove_reference<T>::type>::value,
		typename std::remove_cv<T>::type,
		typename std::conditional<std::is_lvalue_reference<T>::value,
		T,
		typename std::decay<T>::type>::type>::type;
	static_assert(!std::is_base_of<NameValuePair, T>::value,
		"Cannot pair a name to a NameValuePair");

	NameValuePair & operator=(NameValuePair const &) = delete;

public:
	//! Constructs a new NameValuePair
	/*! @param n The name of the pair
	@param v The value to pair.  Ideally this should be an l-value reference so that
	the value can be both loaded and saved to.  If you pass an r-value reference,
	the NameValuePair will store a copy of it instead of a reference.  Thus you should
	only pass r-values in cases where this makes sense, such as the result of some
	size() call.
	@internal */
	NameValuePair(char const * n, T && v) : name(n), value(std::forward<T>(v)) {}

	char const * name;
	Type value;
};

// base classes for type checking
/* The rtti virtual function only exists to enable an archive to
be used in a polymorphic fashion, if necessary.  See the
archive adapters for an example of this */
class OutputArchiveBase
{
public:
	OutputArchiveBase() = default;
	OutputArchiveBase(OutputArchiveBase &&) noexcept {}
	OutputArchiveBase & operator=(OutputArchiveBase &&) noexcept { return *this; }
	virtual ~OutputArchiveBase() noexcept = default;

private:
	virtual void rtti() {}
};

class InputArchiveBase
{
public:
	InputArchiveBase() = default;
	InputArchiveBase(InputArchiveBase &&) noexcept {}
	InputArchiveBase & operator=(InputArchiveBase &&) noexcept { return *this; }
	virtual ~InputArchiveBase() noexcept = default;

private:
	virtual void rtti() {}
};

template<class Archive, class T>
inline void prologue(Archive& archive, T const & data)
{

}

template<class Archive, class T>
inline void epilogue(Archive& archive, T const & data)
{

}


template<class ArchiveType>
class OutputArchive : public OutputArchiveBase
{
public:
	OutputArchive(ArchiveType * const derived) : self(derived) 
	{
	}

	OutputArchive& operator=(OutputArchive const &) = delete;

	template <class T> inline
	OutputArchive& operator<<( T && arg )
	{
		self->process( std::forward<T>( arg ) );
		return *self;
	}

private:
	template<class T>
	inline void process(T&& head)
	{
		prologue(*this, head);
		this->processImpl(head);
		epilogue(*this, head);
	}

	template<class T>
	inline void processImpl(T const & t)
	{
		Serialize(*self, const_cast<T &>(t));
	}

private:
	ArchiveType * const self;

};

template<class ArchiveType>
class InputArchive : public InputArchiveBase
{
public:
	InputArchive(ArchiveType * const derived ) : self( derived )
	{

	}

	InputArchive & operator=( InputArchive const & ) = delete;

	template<class T> inline
	ArchiveType & operator >> ( T && arg )
	{
		self->process( std::forward<T>( arg ) );
		return *self;
	}

private:
	template<class T> inline
	void process(T && head)
	{
		prologue(*self, head);
		self->processImpl(head);
		epilogue(*self, head);
	}

	template<class T>
	inline void processImpl(T & t)
	{
		Deserialize(*self, t);
	}

private:
	ArchiveType * self;
};

#include "./archives/binary.hpp"

using namespace std;

int main()
{
	ostringstream ss;
	FishEngine::Serialization::BinaryOutputArchive bar(ss);
	bar << 311010 << '\n';
	istringstream s_in(ss.str());
	FishEngine::Serialization::BinaryInputArchive bin(s_in);
	int x;
	bin >> x;
	cout << x;
}