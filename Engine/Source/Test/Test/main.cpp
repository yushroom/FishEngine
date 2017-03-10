#include <algorithm>
#include <iostream>
#include <sstream>
using namespace std;

int main()
{
	float v[3] = {0.1, 0.4, -0.3};
	std::cout << *std::min_element(std::begin(v), std::end(v)) << std::endl;
	std::cout << std::to_string(3.0000000f) << endl;
	{
		stringstream ss;
		ss << 42.7600000f;
		cout << ss.str() << endl;
	}
	{
		stringstream ss;
		ss << 0.12345678f;
		cout << ss.str() << endl;
	}
	{
		stringstream ss;
		ss << 180.0001f;
		cout << ss.str() << endl;
	}
	return 0;
}