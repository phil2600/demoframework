#ifndef EVENTCUBE_HH_
# define EVENTCUBE_HH_

# include "../includes/Event.hh"

class EventCube : public Event
{
public:
  EventCube();
  ~EventCube();
  void init();
  void display(float time);
  void update();
  void change_params(std::string name, std::string value);
  std::string get_type();
};

#endif /* !EVENTCUBE_HH_ */
