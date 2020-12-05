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
	}
	catch (std::logic_error err)
	{
		std::cout << err.what();
	}
};