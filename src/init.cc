#include "sys/includes/defines.hh"
#include "sys/includes/Algebre.hh"
#include "sys/includes/GraphEnv.hh"
#include "sys/includes/Camera.hh"
#include "sys/includes/physics.hh"
#include "sys/includes/graphic.hh"
#include "sys/includes/EventFactory.hh"
#include "sys/includes/ShapeFactory.hh"

// Debugging STUFF
float rot1  = 0.0f;

/* FIXME  -  A Nettoyer */

GraphEnv graphical_env(WIDTH, HEIGHT, BPP, FULLSCREEN);

ParticleList particle_cube (MAX_PARTICLES);
ParticleList particle_cube2 (MAX_PARTICLES);

Event *event_cube;
Shape *shape_grid;
Shape *shape_ball;

/* FIXME */

void	display_process();
void	update_events();
void	event_management(SDL_Event *event, char *quit);
void	display();
void	update_shapes();
void	draw_particles(void);

int	main(int argc,char** argv)
{
  EventFactory event_factory;
  ShapeFactory shape_factory;

  Camera * camera;
  graphical_env.init_GL();
  graphical_env.setActiveCamera(new Camera(CPoint(0, 0, 20)));

  event_cube = event_factory.createInstance("cube");
  event_cube->set_shape(new ParticleList (MAX_PARTICLES));

  shape_grid = shape_factory.createInstance("grid");
  shape_ball = shape_factory.createInstance("ball");


  display_process();

  return 0;
}

void	update_shapes()
{
  static float color_rot = 0.0f;
  static float moves = 0.0f;

//   graphical_env.getActiveCamera()->rotate(2, 0);
//   graphical_env.getActiveCamera()->moveStrafeLeft(2, 1);

  /* Update Particle Cubes */
  color_rot += 0.02;
  moves += 0.2;

  event_cube->get_shape()->update_special();

  particle_cube.update_particles_cube(color_rot, moves);
  particle_cube2.update_particles_cube(-color_rot, -moves);
}

void	draw_shapes(void)
{
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  graphical_env.drawAxis3D();

  /* Draw Sphere */
  glPushMatrix();
    GLUquadric* params = gluNewQuadric();
    gluQuadricDrawStyle(params,GLU_LINE);
    gluSphere(params,0.75,20,20);
  glPopMatrix();


  /* The EventCube */
  glPushMatrix();
   event_cube->get_shape()->rotation(0, 0, 0);
   event_cube->get_shape()->rotation_cst(20, 0, 0);
   event_cube->get_shape()->position(1,-15,0);
   event_cube->update();
   glBegin(GL_POINTS);
    event_cube->display(0.0);
   glEnd();
  glPopMatrix();

  /* Just a surface */
  glPushMatrix();
    glBegin(GL_QUADS);
  glColor3f(0.1, 0.3, 0.1);
    glVertex3d(0,10,0);
    glVertex3d(0,0,0);
  glColor3f(0.3, 0.1, 0.1);
    glVertex3d(5,0,0);
  glColor3f(0.1, 0.1, 0.3);
    glVertex3d(5,10,0);
    glEnd();
  glPopMatrix();

  /* Draw Particle Cube */
  glPushMatrix();

   particle_cube.rotation(rot1, rot1, rot1);
   particle_cube.rotation_cst(0, 0, 0);
   particle_cube.position(0,0,0);
   particle_cube.update();

   glBegin(GL_POINTS);
    particle_cube.display();
   glEnd();

  glPopMatrix();

  /* Draw Particle Cube 2 */
  glPushMatrix();

   rot1 -= 1.0f;
   particle_cube2.rotation(-rot1, -rot1, -rot1);
   particle_cube2.rotation_cst(0, 0, 0);
   particle_cube2.position(0,13,0);
   particle_cube2.update();

   glBegin(GL_POINTS);
    particle_cube2.display();
   glEnd();

  glPopMatrix();

  /* Draw Grid */
   shape_grid->display();

  /* Draw Ball */
  shape_ball->display();

}

void	display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  graphical_env.getActiveCamera()->look();

  //  gluLookAt(1.5, 1.5, 5, 1.5, 1.5, 0, 0, 1, 0);

  draw_shapes();

  glFlush();
  SDL_GL_SwapBuffers();
}

void	event_management(SDL_Event *event, char *quit)
{
  while (SDL_PollEvent(event))
    switch(event->type)
    {
      case SDL_QUIT:
	*quit = 1;
	break;

      case SDL_VIDEORESIZE:
	graphical_env.reshape(event->resize.w,event->resize.h);

      case SDL_KEYDOWN:
	switch (event->key.keysym.sym)
	{
	  case SDLK_p:
	    takeScreenshot("test.bmp");
	    break;
 	  case SDLK_ESCAPE:
 	    *quit = 1;
 	    break;
	  default :
	    graphical_env.getActiveCamera()->OnKeyboard(event->key);
	}
 	break;
      case SDL_KEYUP:
	    graphical_env.getActiveCamera()->OnKeyboard(event->key);
	break;
      case SDL_MOUSEMOTION:
	    graphical_env.getActiveCamera()->OnMouseMotion(event->motion);
	break;
      case SDL_MOUSEBUTTONUP:
      case SDL_MOUSEBUTTONDOWN:
	    graphical_env.getActiveCamera()->OnMouseButton(event->button);
	break;
    }
}

void	display_process()
{
  SDL_Event event;
  char quit = 0;

  const Uint32 time_per_frame = 1000/FPS;
  Uint32 last_time = SDL_GetTicks();
  Uint32 current_time,elapsed_time;
  Uint32 start_time, stop_time;

  while (!quit)
  {
    start_time = SDL_GetTicks();

    event_management(&event, &quit);

    current_time = SDL_GetTicks();
    elapsed_time = current_time - last_time;
    last_time = current_time;

    graphical_env.getActiveCamera()->animate(elapsed_time);

    //update_events();
    update_shapes();
    display();

    stop_time = SDL_GetTicks();
    if ((stop_time - last_time) < time_per_frame)
      SDL_Delay(time_per_frame - (stop_time - last_time));
  }
}
