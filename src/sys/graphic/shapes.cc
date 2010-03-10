#include "../includes/physics.hh"

void
Shape::colorate(float r, float g, float b)
{
  r_ = r;
  g_ = g;
  b_ = b;
}

void
Shape::position(float x, float y, float z)
{
  posX_ = x;
  posY_ = y;
  posZ_ = z;
}

void
Shape::rotation_cst(float x, float y, float z)
{
  rotX_ += x;
  rotY_ += y;
  rotZ_ += z;
}

void
Shape::rotation(float x, float y, float z)
{
  rotX_ = x;
  rotY_ = y;
  rotZ_ = z;
}

void
Shape::move(float x, float y, float z)
{
  posX_ += x;
  posY_ += y;
  posZ_ += z;
}

void
Shape::update()
{
  glRotatef(rotX_, 0, 0, 1);
  glRotatef(rotY_, 0, 1, 0);
  glRotatef(rotZ_, 1, 0, 0);

  glTranslatef(posX_, posY_, posZ_);
}
