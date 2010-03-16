#include "../includes/EventFactory.hh"
#include "../includes/Event.hh"

Event*
EventFactory::createInstance(std::string type)
{
  //   if(type=="empty")
  //     return new EventEmpty();

  if (type == "cube")
    return new EventCube();

//   return new EventGeneric;
    return NULL;
}
