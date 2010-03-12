#include "../lib.hh"

#include <string.h>

#include "../includes/graphic.hh"

/* ==============
** ==== Init ====
** ==============
** Initialize the OpenGL window
*/
int init_GL(GraphEnv *graphical_env)
{
  int multiSampling = 0;
  int multiSamplingBuffers = 4;
  SDL_Surface *fenetre;

  Uint32 flags = SDL_OPENGL
    | SDL_GL_DOUBLEBUFFER
    | SDL_HWPALETTE
    | SDL_RESIZABLE
    | SDL_HWSURFACE
    | SDL_HWACCEL;

  if (SDL_Init(SDL_INIT_VIDEO) == -1)
    return 0;

  //  glCullFace(GL_FRONT); // Display the top of the surfaces // HERE

  //   glEnable(GL_NORMALIZE);

  //   SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
  //   SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, 16);

  atexit(SDL_Quit);
  SDL_WM_SetCaption("Cubophile", NULL);

  if (graphical_env->get_fullscreen())
    flags |= SDL_FULLSCREEN;

  if (!(fenetre = SDL_SetVideoMode(graphical_env->get_width(),
				   graphical_env->get_height(),
				   graphical_env->get_bpp(),
				   flags)))
    return 0;

  //   glShadeModel(GL_SMOOTH);
  //   glClearColor(0.0,0.0,0.0,0.0);
  //   glClearDepth(1.0);
  //   glDepthFunc(GL_LEQUAL);
  //   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  /* Get from spanish demo */
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  if(multiSampling)
  {
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, true);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, multiSamplingBuffers);
  }

  SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);//vsync, SDL manual says it's on by default but I highly doubt it
  /* \Get from spanish demo */

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(70,
		 graphical_env->get_width() / graphical_env->get_height(),
		 0.001, 1000);
  glEnable(GL_DEPTH_TEST);
  //glEnable(GL_TEXTURE_2D);
  glPointSize(2.0);

  //  SDL_EnableKeyRepeat(10, 10);

  return 1;
}

/* =================
** ==== Reshape ====
** =================
** Rescale the OpenGL window
*/
void	Reshape(int width, int height)
{
  glViewport (0, 0, width, height);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  gluPerspective(70, width / height, 0.001, 1000);

  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
}
