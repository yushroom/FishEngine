#include <iostream>
#include <sstream>
#include <cassert>

#include <Archive.hpp>
#include <Serialization.hpp>

//template<class T>
//class NameValuePair
//{
//private:
//	// If we get passed an array, keep the type as is, otherwise store
//	// a reference if we were passed an l value reference, else copy the value
//	using Type = typename std::conditional<std::is_array<typename std::remove_reference<T>::type>::value,
//		typename std::remove_cv<T>::type,
//		typename std::conditional<std::is_lvalue_reference<T>::value,
//		T,
//		typename std::decay<T>::type>::type>::type;
//	static_assert(!std::is_base_of<NameValuePair, T>::value,
//		"Cannot pair a name to a NameValuePair");
//
//	NameValuePair & operator=(NameValuePair const &) = delete;
//
//public:
//	//! Constructs a new NameValuePair
//	/*! @param n The name of the pair
//	@param v The value to pair.  Ideally this should be an l-value reference so that
//	the value can be both loaded and saved to.  If you pass an r-value reference,
//	the NameValuePair will store a copy of it instead of a reference.  Thus you should
//	only pass r-values in cases where this makes sense, such as the result of some
//	size() call.
//	@internal */
//	NameValuePair(char const * n, T && v) : name(n), value(std::forward<T>(v)) {}
//
//	char const * name;
//	Type value;
//};


// ######################################################################
// Common BinaryArchive serialization functions

//template<class T> inline
//void Serialize(OutputArchive & archive, T const & t);
//
//template<class T> inline
//void Deserialize(InputArchive & archive, T & t);


////! Serializing NVP types to binary
//template<class T> inline
//void Serialize(OutputArchive & archive, NameValuePair<T> const & t)
//{
//	archive(t.value);
//}

using namespace std;

template<typename T>
void test(const T& t)
{
	cout << typeid(T).name() << endl;
	ostringstream ss;
	FishEngine::OutputArchive bar(ss);
	bar << t;
	istringstream s_in(ss.str());
	FishEngine::InputArchive bin(s_in);
	T x;
	bin >> x;
	assert(x == t);
	//if (x != t)
	//{
	//	cout << "Error: " << t;
	//	abort();
	//}
}


int main()
{
	test(3110103671);
	test(true);
	test(false);
	test(3.1415826);
	test(3.1415926f);
	test(-1);
	test(0);
	test(std::string("Hello World!"));
	//test("Hello World!");
	test(FishEngine::Vector3(1, 2, 3));
	return 0;
}