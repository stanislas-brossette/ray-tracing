#pragma once
#include <iostream>
#include "LightRay.hh"

class Item
{
public:
  Item ();
  virtual ~Item ();
  virtual std::string describe() const = 0;
  virtual bool intersect(const LightRay&) const = 0;

  friend std::ostream& operator<<(std::ostream& os, const Item& s);

private:
};
