#include<iostream>

#include"CityMap.h"
#include"List.h"

int main()
{
	std::cout << "To enter interactive mode enter: -i <file_name> <current_intersection>\n";
	CityMap map;
	std::string input;
	std::getline(std::cin,input);
	std::vector<std::string> split_input = CityMap::split_input(input);
	if (split_input.size() == 3 && split_input[0] == "-i")
	{
		try
		{
			map.read_from_file(split_input[1]);//split_input[1] should be the file name
			map.change_current_intersection(split_input[2]);//split_input[2] should be the current intersection
			map.interactive_mode();
		}
		catch (std::logic_error err)
		{
			std::cout << err.what();
			return 0;
		}
	}
	else
	{
		std::cout << "Invalid Input\n";
	}
}