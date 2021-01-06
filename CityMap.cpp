#include "CityMap.h"

#include<fstream>
#include<stack>

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
		add_intersection(name);
		add_new_connection(name);
		while (file.good() && file.peek()!='\n')//look untill end of line
		{
			std::string intersection_name;
			int weight;
			file >> intersection_name >> weight;
			graph[graph.size() - 1].push_back(intersection_name, weight);
			add_intersection(intersection_name);
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

void CityMap::move_current_intersection(const std::string& destination)
{
	if (!are_connected(current->name(), destination))
	{
		std::cout << "Cant reach " << destination << " from current intersection\n";
		return;
	}
	std::vector<std::pair<std::string, int>> paths;
	find_all_paths(current->name(), destination, std::vector<std::string>(),current->name() ,0, paths);//call with source, destination, empty vector, source,0,empty vector
	sort_paths(paths);
	std::cout << paths[0].first << std::endl;//after sort paths[0] is the fastest path
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

void CityMap::print_three_fastest_routes(const std::string& source, const std::string& destination) const
{
	if (!are_connected(source, destination))
		std::cout << "There is no path between " << source << " and " << destination << std::endl;
	std::vector<std::pair<std::string, int>> paths;
	find_all_paths(current->name(), destination, std::vector<std::string>(), current->name(), 0, paths);
	sort_paths(paths);
	int i = 0, paths_count = 3;
	if (paths.size() < 3)
	{
		std::cout << "There are only " << paths.size() << " paths\n";
		paths_count = paths.size();
	}
	while (paths_count > 0)
	{
		std::cout << paths[i].first << " distance: " << paths[i].second << std::endl;
		++i;
		--paths_count;
	}

}

void CityMap::print_three_alternate_routes(const std::string& source, const std::string& destination) const
{
	if (!are_connected(source, destination))
		std::cout << "There is no path between " << source << " and " << destination << std::endl;
	std::vector<std::pair<std::string, int>> paths;
	find_all_paths(current->name(), destination, std::vector<std::string>(), current->name(), 0, paths);
	sort_paths(paths);
	int i = 0, paths_count = 3;
	if (paths.size() < 3)
	{
		paths_count = paths.size();
	}
	while (paths_count > 0)
	{
		bool flag = false;
		for (const std::string& it : closed_intersections)
		{
			if (paths[i].first.find(it) != -1)
				flag = true;
		}
		if (flag == true)
			continue;
		if (i > paths.size())
		{
			if (paths_count == 3)
				std::cout << "There are no open paths\n";
			else
				std::cout << "There are no more paths\n" ;
			break;
		}
		std::cout << paths[i].first << " distance: " << paths[i].second << std::endl;
		++i;
		--paths_count;
	}
}

void CityMap::print_tour() const
{
	for (List source : graph)
	{
		for (List destination : graph)
		{
			
			if (destination.has_member(source.name()) && are_connected(source.name(), destination.name()))
			{
				std::vector<std::pair<std::string, int>> paths;
				find_all_paths(source.name(), destination.name(), std::vector<std::string>(), source.name(), 0, paths);
				
				for (std::pair<std::string, int> path : paths)
				{
					bool has_all_locations = true;
					for(std::string intersection : all_intersections)
						if (path.first.find(intersection) == -1)
						{
							has_all_locations = false;
						}
					if (has_all_locations == true)
					{
						std::cout << "The tour is:" << path.first<<"->"<< source.name() << std::endl;
						return;
					}
				}

			}
		}
	}
	std::cout << "No possible tour\n";
}

bool CityMap::can_partially_tour_from_current() const
{
	for (List it : graph)
	{
		if (it.name()!=current->name() && it.has_member(current->name()) && are_connected(current->name(),it.name()))
		{
			return true;
		}
	}
	return false;
}

bool CityMap::can_current_reach_all_other() const
{
	for (std::string it : all_intersections)
	{
		if (it != current->name())
		{
			if (!are_connected(current->name(), it))
				return false;
		}
	}
	return true;
}

bool CityMap::are_connected(const std::string& source, const std::string& destination) const
{
	std::vector<std::string> visited;
	std::stack<std::string> st;
	st.push(source);
	while (!st.empty() && st.top()!=destination)
	{
		std::string stack_top = st.top();
		visited.push_back(stack_top);
		st.pop();
		List list = find_intersection(stack_top);
		for (int i=0;i<list.get_connections_count();++i)
		{
			if (!has_member(visited,list[i].first))
			{
				st.push(list[i].first);
			}
		}
	}
	return !st.empty();
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
			it.push_back(destination, weight);
			break;
		}

	}
}

List CityMap::find_intersection(const std::string&_name) const
{
	for (const List &it : graph)
	{
		if (it.name() == _name)
			return it;
	}
	return List("");
}

void CityMap::find_all_paths(std::string source, std::string destination, std::vector<std::string> visited,std::string path, int weight, std::vector<std::pair<std::string, int>>& result)const
{
	static std::vector<std::pair<std::string, int>> paths;
	if (source == destination)
	{
		paths.push_back(std::pair<std::string, int>(path, weight));
		return;
	}
	List current=find_intersection(source);
	visited.push_back(source);
	for (int i = 0; i < current.get_connections_count(); ++i)
	{
		if(!has_member(visited,current[i].first))
		{ 
			find_all_paths(current[i].first, destination, visited, path +"->" + current[i].first, weight + current[i].second, result);
		}
		
	}

	result = paths;
	return ;
}

bool CityMap::has_member(std::vector<std::string> arr,const std::string&_word) const
{
	for (std::string it : arr)
	{
		if (it == _word)
			return true;
	}
	return false;
}

void CityMap::sort_paths(std::vector<std::pair<std::string, int>>& arr)const
{
	
}

void CityMap::add_intersection(std::string new_intersection)
{
	for (std::string it : all_intersections)
	{
		if (it == new_intersection)
			return;
	}
	all_intersections.push_back(new_intersection);
}

void CityMap::copy(const CityMap& other)
{
}

void CityMap::del()
{
}
