#ifndef _LIST_
#define _LIST_

#include<iostream>
#include<string>
#include<exception>

class List
{
public:
//	List();
	List(const std::string&);
	List(const List&);
	~List();

	List& operator=(const List&);

	void push_back(std::string, int);
	void pop_back();
	bool has_member(const std::string&)const;//does not count first member
	int distance_between(const std::string&)const;
	void print()const;
	void print_neightbours()const;
	std::string name()const;
	int get_connections_count()const;
	std::pair<std::string, int> operator[](const int&)const;

private:
	struct Node
	{
		std::string name;
		int weight;
		Node* next;

		Node(std::string _name,int _weight,Node* _next=nullptr):name(_name),weight(_weight),next(_next){}
		Node(const Node* other)
		{
			name = other->name;
			weight = other->weight;
			next = nullptr;
		}
	};
	Node* first;
	Node* last;

	int connections_count;//does not count first elem

	void copy(const List&);
	void del();
};

#endif