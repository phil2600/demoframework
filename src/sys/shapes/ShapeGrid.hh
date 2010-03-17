#ifndef SHAPEGRID_HH_
# define SHAPEGRID_HH_

# include "../includes/Shape.hh"

class ShapeGrid : public Shape
{
public:
  ShapeGrid();
  ~ShapeGrid();
  void display();

private:
  unsigned int grid_number_;
};

#endif /* !SHAPEGRID_HH_ */
