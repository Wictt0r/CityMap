#ifndef _LIST_
#define _LIST_

#include<iostream>
#include<string>
#include<exception>

class List
{
public:
	List();
	List(const std::string&);
	List(const List&);
	~List();

	List& operator=(const List&);

	void push_back(std::string, int);
	void pop_back();
	bool is_connected(const std::string&)const;//does not count first member
	int distance_between(const std::string&)const;
	void print()const;

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

	void copy(const List&);
	void del();
};

#endif