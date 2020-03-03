#include "Item.hh"

Item::Item()
{
}

Item::~Item()
{
}

std::ostream& operator<<(std::ostream& os, const Item& i)
{
  os << "=== generic item ===";
}
