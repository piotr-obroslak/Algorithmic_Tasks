#include "BST.h"
#include <iostream>


using Map = BST<int, std::string>;

int main(int argc, char* argv[])
{
	try
	{
		//Map::Node n(nullptr, {1, std::string("abc")});
		//std::cout << n.first << std::endl;
		Map m;
		m.find(10);
	}
	catch(const Map::Exception & e)
	{
		std::cout << "\033[1;31m" << "ERROR!" << "\033[0m" << e.what() << std::endl;
		return -1;
	}

	return 0;
}

