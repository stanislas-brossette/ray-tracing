#pragma once
#include <iostream>

class Item
{
public:
  Item ();
  virtual ~Item ();
  virtual std::string describe() const = 0;

  friend std::ostream& operator<<(std::ostream& os, const Item& s);

private:
};
