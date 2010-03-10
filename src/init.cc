#include "sys/lib.hh"

SDL_Surface *fenetre;

float LastFrame;
float last_frame_fps;
float fps;

float rot1  = 0.0f;

float rot_angle = 0.0f;
float rot_mov = 0.0f;

float rescaler = 0.0f;

float resize_x = 1.0f;
float resize_y = 1.0f;
float resize_z = 1.0f;

char stop = 0;
char sens = 0;
char mov = 1;

float key_up_down, key_left_right, keyzoom, lr, ud = 0;

s_timer		*timer;


ParticleList particles_list (MAX_PARTICLES);


/* =================
** ==== Reshape ====
** =================
** Rescale the OpenGL window
*/
void Reshape(int w, int h)
{
  glViewport (0, 0, w, h);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  gluPerspective (45.0, (float) w / h, 0.1, 1000.0);

  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
}

/* ==============
** ==== Init ====
** ==============
** Initialize the OpenGL window
*/
int		init(int		width,
		     int		height,
		     int		bpp,
		     char		fullscreen)
{
  Uint32	flags =
    SDL_OPENGL
    | SDL_GL_DOUBLEBUFFER
    | SDL_HWPALETTE
    | SDL_RESIZABLE
    | SDL_HWSURFACE
    | SDL_HWACCEL;

  if (SDL_Init(SDL_INIT_VIDEO) == -1)
    exit (1);

  glCullFace(GL_FRONT); // Display the top of the surfaces
  //   glEnable(GL_DEPTH_TEST);
  //   glEnable(GL_NORMALIZE);

  //   SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
  //   SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, 16);

  atexit(SDL_Quit);

  if (fullscreen)
    flags |= SDL_FULLSCREEN;

  if (!(fenetre = SDL_SetVideoMode(width, height, bpp, flags)))
    exit (1);

  //   glShadeModel(GL_SMOOTH);
  //   glClearColor(0.0,0.0,0.0,0.0);
  //   glClearDepth(1.0);
  //   glDepthFunc(GL_LEQUAL);
  //   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(70.0, width / height, 1.0, 100.0);
  glEnable(GL_DEPTH_TEST);
  glPointSize(2.0);

  return 0;
}


void update_tempo ()
{
  timer->cur_time = SDL_GetTicks();

  timer->diff_time = timer->cur_time - timer->last_time;
  timer->accumulator += timer->diff_time;

  timer->last_time = timer->cur_time;
}

void update_events ()
{
  update_tempo ();
  rot_angle += .008;

  if (!stop)
    if (!sens)
    {
      rot1 += 0.5f;
      if (rescaler < 2)
	rescaler += .008;

      if (rescaler > 2 && resize_x < 4)
      {
	resize_x += .008;
	keyzoom -= 0.5;
      }

      if (rescaler > 2 && resize_y < 4)
      {
	resize_y += .008;
	keyzoom -= 0.5;
      }

      if (resize_y > 4 && resize_z < 4)
      {
	mov = 0;

	resize_z += .008;
	keyzoom -= 0.3 ;
      }
      if (resize_z > 4)
	sens = 1;
    }
    else
    {
      rot1 -= 0.5f;
      if (rot1 == 180)
	stop = 1;
    }
  else
  {
    keyzoom += 0.7 ;
    resize_z += .1;
    resize_y += .008;
    resize_x += .008;
    lr -= 0.1;
    ud -= 0.07;
    //        printf ("zoom: %f\n", keyzoom);
  }
  if (!mov)
  {
    if (rot_mov > 0)
      rot_mov -= 0.2;
  }
  else
    rot_mov += 0.2;
}

void		update_particle()
{
  update_events ();
  //   rescaler = 0.5;
  //   rot1 += 0.5f;
  //   keyzoom = 30;
  //   rot_angle = 2;
  particles_list.update_particles(rot_angle, rot_mov);
}


char catch_events (SDL_Event event, char quit)
{
  while (SDL_PollEvent(&event))
  {
    switch(event.type)
    {
      case SDL_QUIT:
	quit = 1;
	break;

      case SDL_VIDEORESIZE:
	Reshape(event.resize.w,event.resize.h);

      case SDL_KEYDOWN:
	switch(event.key.keysym.sym)
	{
	  case SDLK_LEFT:
	    key_left_right -= 1;
	    break;
	  case SDLK_RIGHT:
	    key_left_right += 1;
	    break;
	  case SDLK_UP:
	    key_up_down += 1;
	    break;
	  case SDLK_DOWN:
	    key_up_down -= 1;
	    break;

	  case SDLK_KP_MINUS:
	    keyzoom -= 1.0;
	    break;
	  case SDLK_KP_PLUS:
	    keyzoom += 1.0;
	    break;
	  case SDLK_ESCAPE:
	    quit = 1;
	    break;
	  default:
	    break;
	}
    }
  }
  return quit;
}


void		draw_particle(void)
{
  glPushMatrix();

  glTranslatef(-1.0 + key_left_right + lr, -1.0 + key_up_down + ud, -70.0 + keyzoom);

  glRotatef(rot1, 0, 0, 1);
  glRotatef(rot1, 0, 1, 0);
  glRotatef(rot1, 1, 0, 0);
  glScalef(1.0 * rescaler * resize_x, 1.0 * rescaler * resize_y, 1.0 * rescaler * resize_z);

  glBegin(GL_POINTS);

  particles_list.display_particles();

  glEnd();
  glPopMatrix();
}

void Display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(-1.0, -1.0, -5.0);

  draw_particle ();

  glFlush();
  SDL_GL_SwapBuffers();
}


// static Uint32 callback(Uint32 interval, void *event)
// {
//   SDL_PollEvent((SDL_Event*)event);
//   //   update_particle();
//   //   Display();
//   //   FPS();

//   //  printf("Timer %d : param = %d\n", interval, (int) param);

//   return interval;
// }

/* =============
** ==== Fps ====
** =============
** Display the number of frames per seconds
*/
void FPS()
{
  fps++;

  if (last_frame_fps + 1000.0f <= SDL_GetTicks ())
  {
    last_frame_fps = SDL_GetTicks ();
    printf ("fps: %i\n", (int)fps / 1000 * 14);
    fps = 0;
  }
}

void disp_routine()
{
  //  FPS();
  fps++;
  if (LastFrame + 10.0f <= SDL_GetTicks ())
  {
    LastFrame = SDL_GetTicks ();
    update_particle();
    Display();
    //    printf ("fps: %i\n", (int)fps / 1000 * 14);
    printf ("fps: %i\n", (int)fps);
    fps = 0;
  }
}

int main (int argc,
	  char** argv)
{
  char		quit = 0;
  SDL_Event	event;
  SDL_TimerID   t1;

  if (!sys_init(0))
    exit(1);

  //  initcossin();
  timer = (s_timer*) malloc (sizeof (s_timer));

  if (init (WIDTH, HEIGHT, BPP, FULLSCREEN) == -1)
    exit(1);

  Reshape (WIDTH, HEIGHT);

  SDL_EnableKeyRepeat(10, 10);

  while (!(quit = catch_events (event, quit)) && !quit)
  {
    disp_routine();
    SDL_PollEvent(&event);
  }

  return 0;
}
