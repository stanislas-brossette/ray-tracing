#pragma once
#include <iostream>
#include "Pixel.hh"
#include "LightRay.hh"

class Item
{
public:
  Item ();
  virtual ~Item ();
  virtual std::string describe() const = 0;
  virtual Pixel intersect(const LightRay&, bool& contact, double& distance) const = 0;

  friend std::ostream& operator<<(std::ostream& os, const Item& s);

private:
};
