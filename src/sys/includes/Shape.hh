#ifndef SHAPE_HH_
# define SHAPE_HH_

# include <cstdlib>
# include <iostream>
# include "Camera.hh"
# include "defines.hh"

class Shape
{
public:
  virtual ~Shape() {};
  void position(float x, float y, float z);
  void colorate(float r, float g, float b);
  void rotation_cst(float x, float y, float z);
  void rotation(float x, float y, float z);
  void move(float x, float y, float z);
  void translate(float x, float y, float z);
  virtual void update();
  virtual void update_special();

  virtual void display() = 0;

protected:
  int   id_;
  float	posX_;
  float posY_;
  float posZ_;
  float movX_;
  float movY_;
  float movZ_;
  float rotX_;
  float rotY_;
  float rotZ_;
  float r_;
  float g_;
  float b_;
  float life_;
};

#endif /* !SHAPE_HH_ */
