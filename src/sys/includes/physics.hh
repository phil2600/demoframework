#ifndef PHYSICS_HH_
# define PHYSICS_HH_

# include <map>
# include "maths.hh"
# include "Shape.hh"

# include "SDL.h"
# include "SDL_opengl.h"

class Particle : public Shape
{
public:
  Particle(int x, int y, int z, int id);
  Particle(int id);
  ~Particle();

  void display();
};


class ParticleList : public Shape
{
public:
  ParticleList(unsigned int partic_nb);
  ~ParticleList();

  void update_special();
  void update_particles_cube(float rot_angle, float rot_mov);

  void display();
private:
  unsigned int				partic_nb_;
  std::map<unsigned int, Particle*>	particles_map_;
};


#endif /* !PHYSICS_HH_ */
