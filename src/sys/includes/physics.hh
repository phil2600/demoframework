#ifndef PHYSICS_HH_
# define PHYSICS_HH_

# include "../lib.hh"
// # include <fstream>
// # include <iostream>
// # include <string>
// # include <iomanip>
// # include <stdlib.h>
// # include <fstream>
// # include <sstream>

class Particle
{
public:
  Particle(int x, int y, int z, int id);
  Particle(int id);
  ~Particle();
  void position(float x, float y, float z);
  void colorate(float r, float g, float b);
  void rotation(float x, float y, float z);
  void move(float x, float y, float z);

  void display();
private:
  int   id_;
  float	posX_;
  float posY_;
  float posZ_;
  float movX_;
  float movY_;
  float movZ_;
  float r_;
  float g_;
  float b_;
  float life_;
};


class ParticleList
{
public:
  ParticleList(unsigned int partic_nb);
  ~ParticleList();
  void update_particles(float rot_angle, float rot_mov);
  void display_particles();
private:
  unsigned int				partic_nb_;
  std::map<unsigned int, Particle*>	particles_map_;
};


#endif /* !PHYSICS_HH_ */
