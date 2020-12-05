#include "CityMap.h"

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

