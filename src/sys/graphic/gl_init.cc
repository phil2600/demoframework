#include "../lib.hh"

#include <string.h>

#include "../includes/graphic.hh"

/* ==============
** ==== Init ====
** ==============
** Initialize the OpenGL window
*/
int init_GL(int	width, int height, int bpp, char fullscreen)
{
  SDL_Surface *fenetre;

  Uint32 flags =
      SDL_OPENGL
    | SDL_GL_DOUBLEBUFFER
    | SDL_HWPALETTE
    | SDL_RESIZABLE
    | SDL_HWSURFACE
    | SDL_HWACCEL;

  if (SDL_Init(SDL_INIT_VIDEO) == -1)
    return 0;

  glCullFace(GL_FRONT); // Display the top of the surfaces

  //   glEnable(GL_DEPTH_TEST);
  //   glEnable(GL_NORMALIZE);

  //   SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
  //   SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, 16);

  atexit(SDL_Quit);

  if (fullscreen)
    flags |= SDL_FULLSCREEN;

  if (!(fenetre = SDL_SetVideoMode(width, height, bpp, flags)))
    return 0;

  //   glShadeModel(GL_SMOOTH);
  //   glClearColor(0.0,0.0,0.0,0.0);
  //   glClearDepth(1.0);
  //   glDepthFunc(GL_LEQUAL);
  //   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(70, width / height, 1, 1000);
  glEnable(GL_DEPTH_TEST);
  glPointSize(2.0);

  SDL_EnableKeyRepeat(10, 10);

  return 1;
}

/* =================
** ==== Reshape ====
** =================
** Rescale the OpenGL window
*/
void	Reshape(int w, int h)
{
  glViewport (0, 0, w, h);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  gluPerspective (70, (float) w / h, 1, 1000);

  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
}
