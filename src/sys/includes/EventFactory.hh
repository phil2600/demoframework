#ifndef EVENTFACTORY_HH_
# define EVENTFACTORY_HH_

#include "Event.hh"
#include "../events/EventCube.hh"

class EventFactory
{
public:
  EventFactory() {};
  ~EventFactory() {};
  Event *createInstance(std::string type);
};

#endif /* !EVENTFACTORY_HH_ */
