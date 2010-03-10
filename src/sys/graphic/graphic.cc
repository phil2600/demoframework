#include "../lib.hh"

void draw_repere(unsigned int scale)
{
  glPushMatrix();

  glScalef(scale, scale, scale);

  glBegin(GL_LINES);
   glColor3ub(0,0,255);
   glVertex3i(0,0,0);
   glVertex3i(1,0,0);
   glColor3ub(0,255,0);
   glVertex3i(0,0,0);
   glVertex3i(0,1,0);
   glColor3ub(255,0,0);
   glVertex3i(0,0,0);
   glVertex3i(0,0,1);

  glEnd();

  glPopMatrix();
}

