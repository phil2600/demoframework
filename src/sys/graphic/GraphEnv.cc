# include "../includes/GraphEnv.hh"

GraphEnv::GraphEnv(int width, int height, int bpp, char fullscreen)
{
  width_ = width;
  height_ = height;
  bpp_ = bpp;
  fullscreen_ = fullscreen;
}

GraphEnv::~GraphEnv()
{
}

int
GraphEnv::get_bpp()
{
  return bpp_;
}

char
GraphEnv::get_fullscreen()
{
  return fullscreen_;
}

int
GraphEnv::get_height()
{
  return height_;
}

int
GraphEnv::get_width()
{
  return width_;
}

void
GraphEnv::set_bpp(int bpp)
{
  bpp_ = bpp;
}

void
GraphEnv::set_fullscreen(char fullscreen)
{
  fullscreen_ = fullscreen;
}

void
GraphEnv::set_height(int height)
{
  height_ = height;
}

void
GraphEnv::set_width(int width)
{
  width_ = width;
}

void
GraphEnv::init_GL()
{
  int multiSampling = 1;
  int multiSamplingBuffers = 4;
  SDL_Surface *fenetre;
  Uint32 flags = SDL_OPENGL
    | SDL_GL_DOUBLEBUFFER
    | SDL_HWPALETTE
    | SDL_RESIZABLE
    | SDL_HWSURFACE
    | SDL_HWACCEL;

  if (SDL_Init(SDL_INIT_VIDEO) == -1)
    exit (1);

  atexit(SDL_Quit);
  SDL_WM_SetCaption("Cubophile !", NULL);

  if (fullscreen_)
    flags |= SDL_FULLSCREEN;

  if (!(fenetre = SDL_SetVideoMode(width_, height_, bpp_, flags)))
    exit (1);

  if(multiSampling)
  {
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, true);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, multiSamplingBuffers);
  }

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(70, width_ / height_, 0.001, 1000);
  glEnable(GL_DEPTH_TEST);

  glPointSize(2.0);
}

void
GraphEnv::reshape(int width, int height)
{
  glViewport (0, 0, width_, height_);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(70, width_ / height_, 0.001, 1000);

  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity();
}

void
GraphEnv::orthoOn(float xres, float yres )
{
  float xrespartido2 = 0;
  float yrespartido2 = 0;

  if (xres == -1)
    xres = width_;

  if (yres == -1)
    yres = height_;

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glViewport(0,0,width_, height_);
  xrespartido2 = xres * 0.5f;
  yrespartido2 = yres * 0.5f;
  glOrtho(-xrespartido2, xrespartido2, -yrespartido2, yrespartido2, 0.10f, 200.0f);

  glMatrixMode(GL_MODELVIEW);
}

void
GraphEnv::orthoOff(void)
{
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}

void
GraphEnv::drawAxis3D()
{
  glLineWidth(3.0f);
  glColor3f(1.0f, 0.0, 0.0);
  glBegin(GL_LINES);
  glVertex3f(0.0f,0.0f,0.0f);
  glVertex3f(1000.0f,0.0f,0.0f); // X
  glEnd();

  glEnable(GL_LINE_STIPPLE);
  glLineStipple(1,0xaaaa);

  glBegin(GL_LINES);
  glVertex3f(0.0f,0.0f,0.0f);
  glVertex3f(-1000.0f,0.0f,0.0f); // X neg
  glEnd();
  glDisable(GL_LINE_STIPPLE);

  glColor3f(0.0f,1.0,0.0);
  glBegin(GL_LINES);
  glVertex3f(0.0f,0.0f,0.0f);
  glVertex3f(0.0f,1000.0f,0.0f); // y
  glEnd();

  glEnable(GL_LINE_STIPPLE);
  glBegin(GL_LINES);
  glVertex3f(0.0f,0.0f,0.0f);
  glVertex3f(0.0f,-1000.0f,0.0f); // y neg
  glEnd();
  glDisable(GL_LINE_STIPPLE);

  glColor3f(0.0f,0.0,1.0);
  glBegin(GL_LINES);
  glVertex3f(0.0f,0.0f,0.0f);
  glVertex3f(0.0f,0.0f,1000.0f); // z
  glEnd();

  glEnable(GL_LINE_STIPPLE);
  glBegin(GL_LINES);
  glVertex3f(0.0f,0.0f,0.0f);
  glVertex3f(0.0f,0.0f,-1000.0f); // z neg
  glEnd();
  glDisable(GL_LINE_STIPPLE);
  glLineWidth(1.0f);

}

void
GraphEnv::cameraLookAtLH(CPoint camPosition, CPoint camTarget, CPoint camUp)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluLookAt(camPosition.x,camPosition.y,camPosition.z,camTarget.x,camTarget.y,camTarget.z,camUp.x,camUp.y,camUp.z);
}

void
GraphEnv::cameraFovLH(float fov, float aspect, float fNear, float fFar)
{
  glMatrixMode(GL_PROJECTION);
  gluPerspective(fov, aspect, fNear, fFar);
}

Camera*
GraphEnv::getActiveCamera()
{
  return activeCamera_;
}

void
GraphEnv::setActiveCamera(Camera *activeCamera)
{
  activeCamera_ = activeCamera;
}

CMatrix
GraphEnv::getModelViewMatrix()
{
  CMatrix* m = new CMatrix();
  static float v[16]={0};

  // Get the current modelview matrix
  glGetFloatv(GL_MODELVIEW_MATRIX , v);

  m->e[0][0] = v[0];
  m->e[1][0] = v[1];
  m->e[2][0] = v[2];
  m->e[3][0] = v[3];

  m->e[0][1] = v[4];
  m->e[1][1] = v[5];
  m->e[2][1] = v[6];
  m->e[3][1] = v[7];

  m->e[0][2] = v[8];
  m->e[1][2] = v[9];
  m->e[2][2] = v[10];
  m->e[3][2] = v[11];

  m->e[0][3] = v[12];
  m->e[1][3] = v[13];
  m->e[2][3] = v[14];
  m->e[3][3] = v[15];

  return *m;
}

CMatrix
GraphEnv::getProjectionMatrix()
{
  CMatrix* m = new CMatrix();
  static float v[16]={0};

  glGetFloatv(GL_PROJECTION_MATRIX , v);

  m->e[0][0] = v[0];
  m->e[1][0] = v[1];
  m->e[2][0] = v[2];
  m->e[3][0] = v[3];

  m->e[0][1] = v[4];
  m->e[1][1] = v[5];
  m->e[2][1] = v[6];
  m->e[3][1] = v[7];

  m->e[0][2] = v[8];
  m->e[1][2] = v[9];
  m->e[2][2] = v[10];
  m->e[3][2] = v[11];

  m->e[0][3] = v[12];
  m->e[1][3] = v[13];
  m->e[2][3] = v[14];
  m->e[3][3] = v[15];

  return *m;
}

void
GraphEnv::loadMatrix(CMatrix m)
{
  double values[16];

  /*
    ( m[0]   m[4]   m[8]    m[12] )
    |m[1]   m[5]    m[9]   m[13]  |
    |m[2]   m[6]   m[10]   m[14]  |
    (m[3]   m[7]   m[11]   m[15]  )
  */
  values[0] = m.e[0][0];
  values[1] = m.e[1][0];
  values[2] = m.e[2][0];
  values[3] = m.e[3][0];

  values[4] = m.e[0][1];
  values[5] = m.e[1][1];
  values[6] = m.e[2][1];
  values[7] = m.e[3][1];

  values[8] = m.e[0][2];
  values[9] = m.e[1][2];
  values[10] = m.e[2][2];
  values[11] = m.e[3][2];

  values[12] = m.e[0][3];
  values[13] = m.e[1][3];
  values[14] = m.e[2][3];
  values[15] = m.e[3][3];

  glLoadMatrixd(values);
}
