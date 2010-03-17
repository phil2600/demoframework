#include "../includes/ShapeFactory.hh"
#include "../includes/Shape.hh"

Shape*
ShapeFactory::createInstance(std::string type)
{
  //   if(type=="empty")
  //     return new ShapeEmpty();

  if (type == "grid")
    return new ShapeGrid();

  if (type == "ball")
    return new ShapeBall();

//   return new ShapeGeneric;
    return NULL;
}
