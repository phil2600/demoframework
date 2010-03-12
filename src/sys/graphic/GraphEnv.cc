#include "../lib.hh"


GraphEnv::GraphEnv()
{

}

GraphEnv::GraphEnv(int width, int height, int bpp, char fullscreen)
{
  width_ = width;
  height_ = height;
  bpp_ = bpp;
  fullscreen_ = fullscreen;
}

GraphEnv::~GraphEnv()
{

}

int
GraphEnv::get_height()
{
  return height_;
}

void
GraphEnv::set_height(int height)
{
  height_ = height;
}

int

GraphEnv::get_width()
{
  return width_;
}

void
GraphEnv::set_width(int width)
{
  width_ = width;
}

char
GraphEnv::get_fullscreen()
{
  return fullscreen_;
}

void
GraphEnv::set_fullscreen(char fullscreen)
{
  fullscreen_ = fullscreen;
}

int
GraphEnv::get_bpp()
{
  return bpp_;
}

void
GraphEnv::set_bpp(int bpp)
{
  bpp_ = bpp;
}

void
GraphEnv::OrtoOn(float xres, float yres )
{
  float xrespartido2 = 0;
  float yrespartido2 = 0;

  if(xres == -1)
    xres = width_;

  if(yres == -1)
    yres == height_;

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glViewport(0,0,width_, height_);
  xrespartido2 = xres * 0.5f;
  yrespartido2 = yres * 0.5f;
  glOrtho(-xrespartido2, xrespartido2, -yrespartido2, yrespartido2, 0.10f, 200.0f);

  glMatrixMode(GL_MODELVIEW);
}

void
GraphEnv::OrtoOff(void)
{
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}
