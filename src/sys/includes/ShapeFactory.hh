#ifndef SHAPEFACTORY_HH_
# define SHAPEFACTORY_HH_

#include "Shape.hh"
#include "../shapes/ShapeGrid.hh"
#include "../shapes/ShapeBall.hh"

class ShapeFactory
{
public:
  ShapeFactory() {};
  ~ShapeFactory() {};
  Shape *createInstance(std::string type);
};

#endif /* !SHAPEFACTORY_HH_ */
