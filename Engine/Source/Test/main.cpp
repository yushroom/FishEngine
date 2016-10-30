#include <iostream>
#include <boost/filesystem.hpp>

using namespace std;

int main()
{
    std::cout << boost::filesystem::current_path() << endl;
}
