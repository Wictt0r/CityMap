#ifndef _CITY_MAP_
#define _CITY_MAP_

#include<string>
#include<vector>

#include"List.h"

class CityMap
{
public:
	CityMap();
	~CityMap();
	CityMap(const CityMap&);

	CityMap& operator=(const CityMap&);

	void read_from_file(const std::string&);
	void print_current_intersection()const;
	void print_current_neightbours()const;
	void change_current_intersection(const std::string&);//just change current
	void move_current_intersection(const std::string&);//print the way
	void print_all_closed_intersections()const;
	void close_intersection(const std::string&);
	void open_intersection(const std::string&);
	void print_three_fastest_routes(const std::string&, const std::string&);
	void print_three_alternate_routes(const std::string&,const std::string&,std::vector<std::string>);
	void print_tour();
	bool can_partially_tour_from_current();
	bool can_current_reach_all_other();
	bool are_connected(const std::string&, const std::string&);//are 2 locations connected
	void print()const;
	void print_all_dead_ends();
	void detect_function(const std::vector<std::string>&);
	void add_new_connection(const std::string&);//create new Intersection
	void add_connection(const std::string&, const std::string&,const int&);//add a connection from source to destination
	void interactive_mode();

	static std::vector<std::string> split_input(std::string);


private:

	std::vector<List> graph;
	std::vector<std::string> closed_intersections;
	List* current;

	List* find_intersection(const std::string&);
	void find_all_paths(std::string, std::string, std::vector<std::string>,std::string, int, std::vector<std::pair<std::string, int>>&);//call with source, destination, empty vector, source,0,empty vector
	bool has_member(std::vector<std::string>,const std::string&)const;//is there a member with that name
	void sort_paths(std::vector<std::pair<std::string, int>>&)const;
	static void remove_space(std::string&);

	std::vector<std::pair<std::string, int>> MergeSort(std::vector<std::pair<std::string, int>>)const;
	std::vector<std::pair<std::string, int>> Merge(std::vector<std::pair<std::string, int>>, std::vector<std::pair<std::string, int>>)const;


	void copy(const CityMap&);
	void del();
};

#endif