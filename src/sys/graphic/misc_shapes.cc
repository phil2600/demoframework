#include "../includes/graphic.hh"

void draw_textured_cube(GLuint texture)
{
  glBindTexture(GL_TEXTURE_2D, texture);
  glPushMatrix();

  glBindTexture(GL_TEXTURE_2D, texture);
  for (int i = 0; i < 4; i++)
  {
    glBegin(GL_QUADS);
    glTexCoord2d(0,1);
    glVertex3d(1,1,1);
    glTexCoord2d(0,0);
    glVertex3d(1,1,-1);
    glTexCoord2d(1,0);
    glVertex3d(-1,1,-1);
    glTexCoord2d(1,1);
    glVertex3d(-1,1,1);
    glEnd();
    glRotated(90,0,0,1);
  }
  glPopMatrix();

  glBegin(GL_QUADS);
  glTexCoord2d(0,0);
  glVertex3d(1,-1,1);
  glTexCoord2d(1,0);
  glVertex3d(1,1,1);
  glTexCoord2d(1,1);
  glVertex3d(-1,1,1);
  glTexCoord2d(0,1);
  glVertex3d(-1,-1,1);
  glEnd();
}

void draw_textured_ground(GLuint texture, unsigned int size)
{
  glBindTexture(GL_TEXTURE_2D, texture);

  glBegin(GL_QUADS);
  glColor3ub(255,0,0);
  glTexCoord2i(0,0);
  glVertex3i(-size,-size,-1);
  glColor3ub(0,255,0);
  glTexCoord2i(size,0);
  glVertex3i(size,-size,-1);
  glColor3ub(255,255,0);
  glTexCoord2i(size,size);
  glVertex3i(size,size,-1);
  glColor3ub(255,0,255);
  glTexCoord2i(0,size);
  glVertex3i(-size,size,-1);
  glEnd();
}
