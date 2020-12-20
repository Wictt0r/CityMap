#include "List.h"

//List::List():first(nullptr),last(nullptr),connections_count(0){}

List::List(const std::string& _name):connections_count(0)
{
	first = last = new(std::nothrow)Node(_name,0);
	if (first == nullptr)
		throw std::bad_alloc();
}

List::List(const List& other)
{
	copy(other);
}

List::~List()
{
	del();
}

List& List::operator=(const List& other)
{
	if (this != &other)
	{
		del();
		copy(other);
	}
	return *this;
}

void List::push_back(std::string _name, int _weight)
{
	if (has_member(_name))
	{
		throw std::logic_error("Connection already exists");
	}
	Node* temp= new (std::nothrow)Node(_name, _weight);
	if (temp == nullptr)
		throw std::bad_alloc();
	if (last == nullptr)
	{
		first = temp;
		last = temp;
		return;
	}
	last->next = temp;
	last = last->next;
	++connections_count;
}

void List::pop_back()
{
	if (first == last)
	{
		throw std::logic_error("Cant delete first element");
	}
	Node* current = first;
	while (current->next->next!=nullptr)
	{
		current = current->next;
	}
	delete last;
	last = current;
	last->next = nullptr;
	--connections_count;
}

bool List::has_member(const std::string& _name) const
{
	if (first == nullptr)
		return false;
	Node* current = first;
	while (current != nullptr && current->name != _name)
	{
		current = current->next;
	}
	return current != nullptr;
}

int List::distance_between(const std::string& _name) const
{
	if (first == nullptr || first->next == nullptr)
		return -1;
	Node* current = first->next;
	while (current != nullptr && current->name!=_name)
	{
		current = current->next;
	}
	if (current == nullptr)
		return -1;
	return current->weight;
}

void List::print() const
{
	std::cout << first->name << ": ";
	print_neightbours();
	std::cout << std::endl;
}

void List::print_neightbours() const
{
	Node* current = first->next;
	while (current != nullptr)
	{
		std::cout << '(' << current->name << ',' << current->weight << ") ";
		current = current->next;
	}
}

std::string List::name() const
{
	return std::string(first->name);
}

int List::get_connections_count() const
{
	return connections_count;
}

std::pair<std::string, int> List::operator[](const int& _postion) const
{
	if (_postion < 0 || _postion>connections_count-1)
		throw std::out_of_range("Invalid element");
	int postion = _postion;
	Node* current = first->next;//first element is the name of the list
	while (postion > 0)
	{
		current = current->next;
		--postion;
	}
	return std::pair<std::string, int>(current->name,current->weight);
}

void List::copy(const List& other)
{
	first = new(std::nothrow) Node(other.first);
	if (first == nullptr)
		throw std::bad_alloc();
	Node* current = first,* other_current = other.first;
	while (other_current->next != nullptr)
	{
		current->next = new (std::nothrow) Node(other_current->next);
		if (current->next == nullptr)
		{
			del();
			throw std::bad_alloc();
		}
		current = current->next;
		other_current = other_current->next;
	}
	last = current;
	connections_count = other.connections_count;
}

void List::del()
{
	if (first == nullptr)
		return;
	while (first->next != nullptr)
	{
		Node* temp = first->next;
		delete first;
		first = temp;
	}
	first = nullptr;
	last = nullptr;
}

