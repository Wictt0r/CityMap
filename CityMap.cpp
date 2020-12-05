#include "CityMap.h"

#include<fstream>

CityMap::CityMap():graph(),closed_intersections(),current(nullptr)
{
}

CityMap::~CityMap()
{
	del();
}

CityMap::CityMap(const CityMap& other)
{
	copy(other);
}

CityMap& CityMap::operator=(const CityMap& other)
{
	if (this != &other)
	{
		del();
		copy(other);
	}
	return *this;
}

void CityMap::read_from_file(const std::string& file_name)
{
	std::ifstream file(file_name);
	if (file.is_open() == false)
	{
		//TODO
		return;
	}
	while (file.good())
	{
		std::string name;
		file >> name;
		add_new_connection(name);
		while (file.good() && file.peek()!='\n')//look untill end of line
		{
			
			std::string intersection_name;
			int weight;
			file >> intersection_name >> weight;
			add_connection(name, intersection_name, weight);
			if ((bool)file == false)
			{
				throw std::logic_error("Invalid input");
			}
		}
	}
	file.close();
}

void CityMap::print_current_intersection() const
{
	std::cout << current->name();
}

void CityMap::print_current_neightbours() const
{
	current->print_neightbours();
}

void CityMap::change_current_intersection(const std::string& name)
{
	for (List &it : graph)
	{
		if (it.name() == name)
		{
			current = &it;
			break;
		}
	}
}

void CityMap::print_all_closed_intersections() const
{
	for (std::string it : closed_intersections)
	{
		std::cout << it << ' ';
	}
	std::cout << std::endl;
}

void CityMap::close_intersection(const std::string& _name)
{
	closed_intersections.push_back(_name);
}

void CityMap::open_intersection(const std::string& _name)
{
	for (size_t i = 0; i < closed_intersections.size(); ++i)
	{
		if (closed_intersections[i] == _name)
		{
			closed_intersections.erase(closed_intersections.begin() + i);
			return;
		}
	}
	std::cout << "No such closed intersection\n";
}


void CityMap::print() const
{
	for (List it : graph)
	{
		it.print();
	}
}

void CityMap::add_new_connection(const std::string& _name)
{
	for (List it : graph)
	{
		if (it.name() == _name)
		{
			throw std::logic_error("connection already exists");
			break;
		}
	}
	graph.push_back(List(_name));
}

void CityMap::add_connection(const std::string& source, const std::string& destination,const int& weight)
{
	if (source == destination || weight < 1)
	{
		throw std::logic_error("Invalid input");
	}
	for (List& it : graph)
	{
		if (it.name() == source)
		{
			if (it.has_member(destination) == false)
			{
				it.push_back(destination, weight);
			}
			else
			{
				throw std::logic_error("Connection already exists");
			}
			break;
		}

	}
}

void CityMap::copy(const CityMap& other)
{
}

void CityMap::del()
{
}
