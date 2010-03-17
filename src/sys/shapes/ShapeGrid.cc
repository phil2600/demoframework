#include "ShapeGrid.hh"

ShapeGrid::ShapeGrid()
{
}

ShapeGrid::~ShapeGrid()
{
}

void
ShapeGrid::display()
{
  unsigned char G_color_grid[4] = {241, 239, 180, 128};
  unsigned int grid_number_ = 30;
  int grid_inc = 4 * WIDTH / grid_number_;
  float grid_top = WIDTH  * 2;
  float xpos = -grid_top, ypos;
  int wider_each = 5;

  glColor4ubv(G_color_grid);

  glEnable(GL_BLEND);
  // glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  for (int k = 0; k < grid_number_; k++)
  {
    for(int i = 0; i < grid_number_; i++)
    {
      ypos = -grid_top;
      for(int j = 0; j < grid_number_; j++)
      {

	if((i % wider_each == 0) && (j % wider_each == 0))
	{
	  glDisable(GL_LINE_STIPPLE);
	  glLineWidth(2);
	}
	else
	{
	  glEnable(GL_LINE_STIPPLE);
	  glLineStipple(1, 0x6666);
	  glLineWidth(2);
	}
	glBegin(GL_LINES);
	glVertex3f(xpos, ypos, -grid_top);
	glVertex3f(xpos, ypos, grid_top);

	glVertex3f(xpos, -grid_top, ypos);
	glVertex3f(xpos, grid_top, ypos);
	glEnd();
	ypos += grid_inc;
      }
      xpos += grid_inc;
    }
  }
  glDisable(GL_BLEND);

  glDisable(GL_LINE_STIPPLE);
}
