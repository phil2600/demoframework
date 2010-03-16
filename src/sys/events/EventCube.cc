#include "EventCube.hh"

EventCube::EventCube()
{
}

EventCube::~EventCube()
{
}

void
EventCube::init()
{
}

void
EventCube::display(float time)
{
  //  printf("In EventCube::display(float time)\n");
  shape_->display();
}

void
EventCube::update()
{
  shape_->update();
}

void
EventCube::change_params(std::string name, std::string value)
{
}

std::string
EventCube::get_type()
{
}
