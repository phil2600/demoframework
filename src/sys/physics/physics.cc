#include "../includes/physics.hh"


Particle::Particle(int x, int y, int z, int id)
  :posX_ (x),
   posY_ (y),
   posZ_ (z),
   id_ (id)
{
  r_ = sys_sinf (x);
  g_ = sys_sinf (y);
  b_ = sys_sinf (z);
}

Particle::Particle(int id)
  :id_ (id)
{
}


Particle::~Particle()
{
}

void
Particle::colorate(float r, float g, float b)
{
  r_ = r;
  g_ = g;
  b_ = b;
}

void
Particle::position(float x, float y, float z)
{
  posX_ = x;
  posY_ = y;
  posZ_ = z;
}

void
Particle::rotation(float x, float y, float z)
{
  movX_ = x;
  movY_ = y;
  movZ_ = z;
}

void
Particle::move(float x, float y, float z)
{
  posX_ += movX_;
  posY_ += movY_;
  posZ_ += movZ_;
}

void
Particle::display()
{
    glColor3f(r_, g_, b_);
    glVertex3f(posX_, posY_, posZ_);
}

/* ==================================== */
ParticleList::ParticleList(unsigned int partic_nb)
  :partic_nb_ (partic_nb)
{
  for (int id = 0; id < partic_nb; id++)
    particles_map_.insert(std::make_pair (id, new Particle(0, 0, 0, id++)));
}

ParticleList::~ParticleList()
{
  //
  // FIXME : Delete all the Particle ?
  //
  std::map<unsigned int, Particle*>::iterator i = particles_map_.begin ();
  for (;i != particles_map_.end (); i++)
    delete (i->second);
  particles_map_.clear();
}

void
ParticleList::update_particles(float rot_angle, float rot_mov)
{
  int		id = 0;
  Particle	*particle = 0;
  int x, y, z = 0;

  std::map<unsigned int, Particle*>::iterator i = particles_map_.begin ();

  for (;i != particles_map_.end (); i++)
  {
    id = i->first;
    particle = i->second;

    x = id / 100;
    y = (id / 10) % 10;
    z = id % 10;

    i->second->position(x, y, z);
    i->second->colorate(sys_sinf (x + rot_angle), \
			sys_sinf (y + rot_angle), \
			sys_sinf (z + rot_angle));

    i->second->rotation(-sys_cosf (y * rot_mov / 90), \
			sys_sinf (-x * rot_mov / 90), \
			-sys_sinf (y * rot_mov / 90));

    i->second->move(x, y, -z);
  }

}

void
ParticleList::display_particles()
{
  std::map<unsigned int, Particle*>::iterator i = particles_map_.begin ();

  for (;i != particles_map_.end (); i++)
    i->second->display();
}
