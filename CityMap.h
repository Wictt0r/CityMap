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

	void print_current_intersection();
	void print_current_neightbours();
	void change_current_intersection(const std::string&);//just change current
	void move_current_intersection(const std::string&);//print the way
	void print_all_closed_intersections();
	void close_intersection(const std::string&);
	void open_intersection(const std::string&);
	void print_three_fastest_routes();
	void print_three_alternate_routes();
	void print_tour();
	bool can_partially_tour_from_current();
	bool can_current_reach_all_other();
	bool are_connected();//are 2 locations connected

	void add_new_connection(std::string);//create new Intersection
	void add_connection(std::string, std::string ,int);//add a connection from source to destination

private:
	//pointer to current intersection ??? current
	//the map implemented as a list

	std::vector<List> graph;

	void copy(const CityMap&);
	void del();
};

#endif