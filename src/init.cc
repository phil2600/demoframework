#include "sys/lib.hh"

/* FIXME */
float rot1  = 0.0f;

float rot_mov = 0.0f;

float rescaler = 0.0f;

float resize_x = 1.0f;
float resize_y = 1.0f;
float resize_z = 1.0f;

float key_up_down, key_left_right, keyzoom, lr, ud = 0;
/* FIXME */

#ifdef FREE_LOOK
FreeFlyCamera * camera;
#endif

ParticleList particle_cube (MAX_PARTICLES);

int	process_display();

void	update_events();
void	event_management(SDL_Event *event, char *quit);
void	display();
void	update_shapes();
void	draw_particles(void);


int	main (int argc,char** argv)
{
  if (! sys_init(0))
    exit(1);

#ifdef FREE_LOOK
  camera = new FreeFlyCamera(Vector3D(0,0,2));
#endif

  process_display();

  return 0;
}


void		update_shapes()
{
  static float color_rot = 0.0f;
  static float moves = 0.0f;

  /* Update Particle Cube */
  color_rot += 0.01;
  moves += 0.2;

  particle_cube.update_particles_cube(color_rot, moves);
}

void		draw_shapes(void)
{
  /* Draw Orthogonal Repere */
  draw_repere(50);

    GLUquadric* params = gluNewQuadric(); //création du quadrique
    gluQuadricDrawStyle(params,GLU_LINE);
    gluSphere(params,0.75,20,20);

  /* Draw Particle Cube */
  glPushMatrix();

  rot1 += 0.5f;
  particle_cube.rotation(rot1, rot1, rot1);
  particle_cube.rotation_cst(0, 0, 0);
  particle_cube.position(1,1,1);
  particle_cube.update();

  glBegin(GL_POINTS);
  particle_cube.display();
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

  //gluLookAt(camX, camY, camZ, cibleX, cibleY, cibleZ, vertX, vertY, vertZ);

  //  glTranslatef(key_left_right, key_up_down, -70.0 + keyzoom);

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

  Uint32 last_time = SDL_GetTicks();
  Uint32 current_time,elapsed_time;
  Uint32 start_time;

  Reshape(WIDTH, HEIGHT);

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

    //    update_events();
    update_shapes();
    display();

    elapsed_time = SDL_GetTicks() - start_time;
    if (elapsed_time < 10)
      SDL_Delay(10 - elapsed_time);
  }

  return 0;
}



// void update_events ()
// {
//   char stop = 0;
//   char mov  = 1;
//   char sens = 0;

//   if (!stop)
//     if (!sens)
//     {
//       rot1 += 0.5f;
//       if (rescaler < 2)
// 	rescaler += .008;

//       if (rescaler > 2 && resize_x < 4)
//       {
// 	resize_x += .008;
// 	keyzoom -= 0.5;
//       }

//       if (rescaler > 2 && resize_y < 4)
//       {
// 	resize_y += .008;
// 	keyzoom -= 0.5;
//       }

//       if (resize_y > 4 && resize_z < 4)
//       {
// 	mov = 0;

// 	resize_z += .008;
// 	keyzoom -= 0.3 ;
//       }
//       if (resize_z > 4)
// 	sens = 1;
//     }
//     else
//     {
//       rot1 -= 0.5f;
//       if (rot1 == 180)
// 	stop = 1;
//     }
//   else
//   {
//     keyzoom += 0.7 ;
//     resize_z += .1;
//     resize_y += .008;
//     resize_x += .008;
//     lr -= 0.1;
//     ud -= 0.07;
//   }

//   if (!mov)
//   {
//     if (rot_mov > 0)
//       rot_mov -= 0.2;
//   }
//   else
//     rot_mov += 0.2;
// }
