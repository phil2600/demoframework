#include "../includes/physics.hh"


Particle::Particle(int x, int y, int z, int id)
{
  posX_ = x;
  posY_ = y;
  posZ_ = z;
  id_ = id;
  r_ = sys_sinf (x);
  g_ = sys_sinf (y);
  b_ = sys_sinf (z);
}

Particle::Particle(int id)
{
  id_ = id;
}


Particle::~Particle()
{
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
ParticleList::update_particles_cube(float color_rot, float rot_mov)
{
  int		id = 0;
  Particle	*particle = 0;
  int		x, y, z = 0;

  std::map<unsigned int, Particle*>::iterator i = particles_map_.begin ();

  for (;i != particles_map_.end(); i++)
  {
    id = i->first;
    particle = i->second;

    x = id / 100;
    y = (id / 10) % 10;
    z = id % 10;

    i->second->position(x, y, z);
    i->second->colorate(sys_sinf (x + color_rot),	\
			sys_sinf (y + color_rot),	\
			sys_sinf (z + color_rot));

    i->second->move(-sys_cosf (y * rot_mov / 90),	\
		    sys_sinf (-x * rot_mov / 90),	\
		    -sys_sinf (y * rot_mov / 90));
  }

}

void
ParticleList::display()
{
  std::map<unsigned int, Particle*>::iterator i = particles_map_.begin ();

  for (;i != particles_map_.end (); i++)
    i->second->display();
}
