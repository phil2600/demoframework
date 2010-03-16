#ifndef EVENT_HH_
# define EVENT_HH_

# include <cstdlib>
# include <iostream>
# include "physics.hh"
# include "Camera.hh"

class Event
{
public:
  virtual ~Event() {};

  virtual void init() = 0;
  virtual void display(float time) = 0;
  virtual void update() = 0;

  virtual void change_params(std::string name, std::string value) = 0;

  virtual std::string get_type() = 0;


  void set_shape(Shape* shape);
  void set_time(float start_time, float end_time);
  void set_id(unsigned int id);
  unsigned int get_id();

  void set_camera(Camera* camera);

  Shape* get_shape();

protected:

  unsigned int id_;
  float start_time_;
  float end_time_;

  Shape *shape_;
  Camera *camera_;
};

#endif /* !EVENT_HH_ */
