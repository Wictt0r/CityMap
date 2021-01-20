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
		throw std::logic_error("Could not load map from file\n");
		return;
	}
	while (file.good())
	{
		std::string name;
		file >> name;
		add_new_connection(name);
		while (file.good() && file.peek()!='\n')//look until end of line
		{
			std::string intersection_name;
			int weight;
			file >> intersection_name >> weight;
			add_connection(name,intersection_name, weight);
			add_new_connection(intersection_name);
			if ((bool)file == false)
			{
				file.close();
				throw std::logic_error("Invalid file input");
			}
		}
	}
	file.close();
}

void CityMap::print_current_intersection() const
{
	std::cout <<"Current intersection is:"<< current->name()<<'\n';
}

void CityMap::print_current_neightbours() const
{
	std::cout << "Current neighbours are:\n";
	current->print_neighbours();
}

void CityMap::change_current_intersection(const std::string& name)
{
	for (List &it : graph)
	{
		if (it.name() == name)
		{
			current = &it;
			std::cout << "Current intersection is now: " << it.name() << std::endl;
			return;
		}
	}
	throw std::logic_error("No such intersection\n");
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
	current = find_intersection(destination);
}

void CityMap::print_all_closed_intersections() const
{
	if (closed_intersections.size() == 0)
	{
		std::cout << "No closed intersections\n";
		return;
	}
	std::cout << "Closed intersections are:";
	for (std::string it : closed_intersections)
	{
		std::cout << it << ' ';
	}
	std::cout << std::endl;
}

void CityMap::close_intersection(const std::string& _name)
{
	for(List it : graph)
	{
		if (it.name() == _name)
		{
			closed_intersections.push_back(_name);
			std::cout << _name << " closed\n";
			return;
		}
	}
	std::cout << "No such intersection\n";
	
}

void CityMap::open_intersection(const std::string& _name)
{
	for (size_t i = 0; i < closed_intersections.size(); ++i)
	{
		if (closed_intersections[i] == _name)
		{
			closed_intersections.erase(closed_intersections.begin() + i);
			std::cout << _name << " opened\n";
			return;
		}
	}
	std::cout << "No such closed intersection\n";
}

void CityMap::print_three_fastest_routes(const std::string& source, const std::string& destination) 
{
	if (!are_connected(source, destination))
	{
		std::cout << "There is no path between " << source << " and " << destination << std::endl;
		return;
	}
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

void CityMap::print_three_alternate_routes(const std::string& source, const std::string& destination, std::vector<std::string> _closed_intersections)
{
	if (!are_connected(source, destination))
	{
		std::cout << "There is no path between " << source << " and " << destination << std::endl;
		return;
	}
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
		if (i >= paths.size())
		{
			if (paths_count == 3)
				std::cout << "There are no open paths\n";
			else
				std::cout << "There are no more open	paths\n";
			break;
		}
		bool has_closed_intersection = false;
		for (const std::string& it : _closed_intersections)
		{
			if (paths[i].first.find(it) != std::string::npos)
				has_closed_intersection = true;
		}
		if (has_closed_intersection == true)
		{
			++i;
			continue;
		}
		std::cout << paths[i].first << " distance: " << paths[i].second << std::endl;
		++i;
		--paths_count;
	}
}

void CityMap::print_tour()
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
					for (List intersection : graph)
					{
						if ((has_member(closed_intersections, intersection.name()) == true && //if the intersection is closed make sure it isnt in the path
							path.first.find(intersection.name()) != std::string::npos)||
							(has_member(closed_intersections, intersection.name()) == false && 
							path.first.find(intersection.name()) == std::string::npos))//if the intersection is open make sure it is in the path
						{
							has_all_locations = false;
							break;
						}
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

bool CityMap::can_partially_tour_from_current()
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

bool CityMap::can_current_reach_all_other() 
{
	for (List it : graph)
	{
		if (it.name() != current->name())
		{
			if (!are_connected(current->name(), it.name()))
				return false;
		}
	}
	return true;
}

bool CityMap::are_connected(const std::string& source, const std::string& destination)
{
	std::vector<std::string> visited;
	std::stack<std::string> st;
	st.push(source);
	while (!st.empty() && st.top()!=destination)
	{
		std::string stack_top = st.top();
		visited.push_back(stack_top);
		st.pop();
		List* list = find_intersection(stack_top);
		if (list->name() == "")
			return false;
		for (int i=0;i<list->get_connections_count();++i)
		{
			if (!has_member(visited,(*list)[i].first))
			{
				st.push((*list)[i].first);
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

void CityMap::print_all_dead_ends()
{
	size_t counter=0;
	for (const List destination : graph)
	{
		if (destination.get_connections_count() == 0)
		{
			for (const List source : graph)
			{
				if (source.name() != destination.name() && source.has_member(destination.name()))
				{
					std::cout << source.name() << "->" << destination.name() << " is a dead end\n";
					++counter;
				}
			}
		}
	}
	if (counter == 0)
	{
		std::cout << "There are no dead ends\n";
	}
	return;
}

void CityMap::detect_function(const std::vector<std::string>& split_input)
{
		const size_t input_word_count = split_input.size();

		if (input_word_count == 1 && split_input[0] == "location")
		{
			print_current_intersection();
			return;
		}
		if (input_word_count == 2 && split_input[0] == "change")
		{
			change_current_intersection(split_input[1]);
			return;
		}
		if (input_word_count == 1 && split_input[0] == "neighbours")
		{
			print_current_neightbours();
			return;
		}
		if (input_word_count == 2 && split_input[0] == "move")
		{
			move_current_intersection(split_input[1]);
			return;
		}
		if (input_word_count == 2 && split_input[0] == "close")
		{
			close_intersection(split_input[1]);
			return;
		}
		if (input_word_count == 2 && split_input[0] == "open")
		{
			open_intersection(split_input[1]);
			return;
		}
		if (input_word_count == 1 && split_input[0] == "closed")
		{
			print_all_closed_intersections();
			return;
		}
		if (input_word_count == 1 && split_input[0] == "tour")
		{
			print_tour();
			return;
		}
		if (input_word_count == 2 && split_input[0] == "partial" && split_input[1] == "tour")
		{
			std::cout << (can_partially_tour_from_current() ? "It is possible\n" : "It isn't possible\n");
			return;
		}
		if (input_word_count == 2 && split_input[0] == "reach" && split_input[1] == "all")
		{
			std::cout << (can_current_reach_all_other() ? "It is possible\n" : "It isn't possible\n");
			return;
		}
		if (input_word_count == 2 && split_input[0] == "paths")
		{
			print_three_fastest_routes(current->name(), split_input[1]);
			return;
		}
		if (input_word_count == 2 && split_input[0] == "alternate")
		{
			print_three_alternate_routes(current->name(), split_input[1],closed_intersections);
			return;
		}
		if (input_word_count == 2 && split_input[0] == "dead" && split_input[1] == "ends")
		{
			print_all_dead_ends();
			return;
		}

		if (input_word_count != 1 || split_input[0] != "end")
		std::cout << "Invalid command\n";
}

void CityMap::add_new_connection(const std::string& _name)
{
	for (List it : graph)
	{
		if (it.name() == _name)
		{
			return;
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

void CityMap::interactive_mode()
{
	std::string input;
	do
	{
		try
		{
			std::getline(std::cin, input);
			detect_function(split_input(input)); 
		}
		catch (std::logic_error err)
		{
			std::cout << err.what();
		}
		catch (std::bad_alloc())
		{
			std::cout << "Bad memmory allocation\n";
		}
	} 
	while (input!="end");
}

std::vector<std::string> CityMap::split_input(std::string input)
{
	remove_space(input);
	std::vector<std::string> split_input;
	int pos = 0;
	size_t length = input.find_first_of(" \n");
	while (pos!=std::string::npos)
	{
		split_input.push_back(input.substr(pos, length));
		pos = input.find_first_of(" \n", pos+1)+1;
		if (pos == 0)
			pos = std::string::npos;
		length = input.find_first_of(" \n", pos+1) - pos;
		
	}
	return split_input;
}

List* CityMap::find_intersection(const std::string&_name)
{
	for (List &it : graph)
	{
		if (it.name() == _name)
			return &it;
	}
	return nullptr;
}

void CityMap::find_all_paths(std::string source, std::string destination, std::vector<std::string> visited,std::string path, int weight, std::vector<std::pair<std::string, int>>& result)
{
	if (source == destination)
	{
		result.push_back(std::pair<std::string, int>(path, weight));
		return;
	}
	List* current=find_intersection(source);
	visited.push_back(source);
	for (int i = 0; i < current->get_connections_count(); ++i)
	{
		if(!has_member(visited,(*current)[i].first))
		{ 
			find_all_paths((*current)[i].first, destination, visited, path +"->" + (*current)[i].first, weight + (*current)[i].second, result);
		}
		
	}
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
	arr=MergeSort(arr);
}

void CityMap::remove_space(std::string& str)
{
	for (size_t i = 0; i < str.size(); ++i)
	{
		if (str[i] == ' ')
		{
			++i;
			while (i<str.size() && str[i] == ' ')
			{
				str.erase(i, 1);
			}
		}
	}
}

std::vector<std::pair<std::string, int>> CityMap::MergeSort(std::vector<std::pair<std::string, int>> arr)const
{
	if (arr.size()==1)
	{
		return arr;
	}
	std::vector<std::pair<std::string, int>> first, second;
	for (size_t i = 0; i < arr.size() / 2; ++i)
	{
		first.push_back(arr[i]);
	}
	for (size_t i = arr.size()/2; i < arr.size(); ++i)
	{
		second.push_back(arr[i]);
	}

	first=MergeSort(first);
	second=MergeSort(second);

	 return Merge(first, second);
}

std::vector<std::pair<std::string, int>> CityMap::Merge(std::vector<std::pair<std::string, int>> first , std::vector<std::pair<std::string, int>> second) const
{
	if (first.size() == 0)
		return second;
	if (second.size() == 0)
		return first;

	std::vector<std::pair<std::string, int>> sorted;
	size_t first_index=0,second_index=0;
	while (first_index < first.size() && second_index < second.size())
	{
		if (first[first_index].second > second[second_index].second)
		{
			sorted.push_back(second[second_index]);
			++second_index;
		}
		else
		{
			sorted.push_back(first[first_index]);
			++first_index;
		}
	}

	while (first_index < first.size())
	{
		sorted.push_back(first[first_index]);
		++first_index;
	}
	while (second_index < second.size())
	{
		sorted.push_back(second[second_index]);
		++second_index;
	}
	return sorted;
}

void CityMap::copy(const CityMap& other)
{
	for (List it : other.graph)
	{
		graph.push_back(it);
	}
	for (std::string it : other.closed_intersections)
	{
		graph.push_back(it);
	}
	current = find_intersection(other.current->name());
}

void CityMap::del()
{
}
