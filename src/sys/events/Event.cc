#include "../includes/EventFactory.hh"
#include "../includes/Event.hh"

void
Event::set_shape(Shape* shape)
{
  shape_ = shape;
}

void
Event::set_time(float start_time, float end_time)
{
  start_time_ = start_time;
  end_time_ = end_time;
}

void
Event::set_id(unsigned int id)
{
  id_ = id;
}

unsigned int
Event::get_id()
{
  return id_;
}

void
Event::set_camera(Camera* camera)
{
  camera_ = camera;
}

Shape*
Event::get_shape()
{
  return shape_;
}

void
Event::update_pos()
{
  shape_->update_pos();
}

void
Event::update_rot()
{
  shape_->update_rot();
}
