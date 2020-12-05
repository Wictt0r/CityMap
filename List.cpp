#include "List.h"

List::List():first(nullptr),last(nullptr){}

List::List(const std::string& _name)
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

