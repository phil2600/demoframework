#ifndef PHYSICS_HH_
# define PHYSICS_HH_

# include "../lib.hh"

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
  void update();

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

  void update_particles_cube(float rot_angle, float rot_mov);

  void display();
private:
  unsigned int				partic_nb_;
  std::map<unsigned int, Particle*>	particles_map_;
};


#endif /* !PHYSICS_HH_ */
