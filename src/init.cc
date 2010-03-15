#include "sys/lib.hh"

// Debugging STUFF
float rot1  = 0.0f;
float rot_mov = 0.0f;

float rescaler = 0.0f;

float resize_x = 1.0f;
float resize_y = 1.0f;
float resize_z = 1.0f;

float key_up_down, key_left_right, keyzoom, lr, ud = 0;


/* FIXME  -  A Nettoyer */

GraphEnv graphical_env(WIDTH, HEIGHT, BPP, FULLSCREEN);

#ifdef FREE_LOOK
FreeFlyCamera * camera;
#endif

ParticleList particle_cube (MAX_PARTICLES);
ParticleList particle_cube2 (MAX_PARTICLES);
/* FIXME */


int	process_display();
void	update_events();
void	event_management(SDL_Event *event, char *quit);
void	display();
void	update_shapes();
void	draw_particles(void);


int	main(int argc,char** argv)
{
  if (!sys_init(&graphical_env))
    exit(1);

#ifdef FREE_LOOK
  camera = new FreeFlyCamera(CPoint(1,1,5));
#endif

  process_display();

  return 0;
}

void		update_shapes()
{
  static float color_rot = 0.0f;
  static float moves = 0.0f;

  /* Update Particle Cubes */
  color_rot += 0.02;
  moves += 0.2;

  particle_cube.update_particles_cube(color_rot, moves);
  particle_cube2.update_particles_cube(-color_rot, -moves);
}

void		draw_shapes(void)
{
  graphical_env.auxAxis();

  /* Draw Sphere */
  glPushMatrix();
    GLUquadric* params = gluNewQuadric();
    gluQuadricDrawStyle(params,GLU_LINE);
    gluSphere(params,0.75,20,20);
  glPopMatrix();

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

  //  rot1 += 0.5f;
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
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

#ifdef FREE_LOOK
  camera->look();
#else
  gluLookAt(1.5, 1.5, 5 + keyzoom, 1.5, 1.5, 0, 0, 1, 0);
#endif

  draw_shapes();

  glFlush();
  SDL_GL_SwapBuffers();
}


void event_management(SDL_Event *event, char *quit)
{
  while (SDL_PollEvent(event))
    switch(event->type)
    {
      case SDL_QUIT:
	*quit = 1;
	break;

      case SDL_VIDEORESIZE:
	Reshape(event->resize.w,event->resize.h);

      case SDL_KEYDOWN:
	switch (event->key.keysym.sym)
	{
	  case SDLK_p:
	    takeScreenshot("test.bmp");
	    break;
 	  case SDLK_ESCAPE:
 	    *quit = 1;
 	    break;
#ifndef FREE_LOOK
	  case SDLK_LEFT:
	    key_left_right += 1;
	    break;
	  case SDLK_RIGHT:
	    key_left_right -= 1;
	    break;
	  case SDLK_UP:
	    key_up_down -= 1;
	    break;
	  case SDLK_DOWN:
	    key_up_down += 1;
	    break;

	  case SDLK_KP_MINUS:
	    keyzoom -= 1.0;
	    break;
	  case SDLK_KP_PLUS:
	    keyzoom += 1.0;
	    break;
	  default:
	    break;
	}
#else /* FREE_LOOK */
	  default :
	    camera->OnKeyboard(event->key);
	}
 	break;
      case SDL_KEYUP:
	camera->OnKeyboard(event->key);
	break;
      case SDL_MOUSEMOTION:
	camera->OnMouseMotion(event->motion);
	break;
      case SDL_MOUSEBUTTONUP:
      case SDL_MOUSEBUTTONDOWN:
	camera->OnMouseButton(event->button);
	break;
#endif /* ! FREE_LOOK */
    }
}

int process_display()
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

#ifdef FREE_LOOK
    camera->animate(elapsed_time);
#endif

    //update_events();
    update_shapes();
    display();

    stop_time = SDL_GetTicks();
    if ((stop_time - last_time) < time_per_frame)
      SDL_Delay(time_per_frame - (stop_time - last_time));
  }

  return 0;
}
