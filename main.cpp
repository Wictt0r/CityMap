#include<iostream>

#include"CityMap.h"
#include"List.h"

int main()
{
	CityMap map;
	try
	{
		map.read_from_file("test.txt");
		//map.print();
		map.change_current_intersection("popa");
		map.print_tour();
	}
	catch (std::logic_error err)
	{
		std::cout << err.what();
	}
	catch (std::bad_alloc())
	{
		std::cout << "Bad memmory allocation\n";
	}
};