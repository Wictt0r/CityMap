#include<iostream>

#include"CityMap.h"
#include"List.h"

int main()
{
	CityMap map;
	try
	{
		map.read_from_file("test.txt");
		map.print();
		std::cout<<map.are_connected("popa", "ndk");
	}
	catch (std::logic_error err)
	{
		std::cout << err.what();
	}
};