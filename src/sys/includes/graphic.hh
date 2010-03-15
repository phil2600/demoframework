#ifndef GRAPHIC_HH_
# define GRAPHIC_HH_

# include "SDL.h"
# include "SDL_opengl.h"

# include "../lib.hh"
# include "Algebre.hh"
# include "Camera.hh"

#ifdef FREE_LOOK
/* FlyCam */
class FreeFlyCamera
{
public:
  // FIXME
  // Utiliser une camera directement dans l'environement
  FreeFlyCamera(const CPoint & position = CPoint(0,0,0));

  virtual void OnMouseMotion(const SDL_MouseMotionEvent & event);
  virtual void OnMouseButton(const SDL_MouseButtonEvent & event);
  virtual void OnKeyboard(const SDL_KeyboardEvent & event);

  virtual void animate(Uint32 timestep);
  virtual void setSpeed(double speed);
  virtual void setSensivity(double sensivity);

  virtual void setPosition(const CPoint & position);

  virtual void look();

  virtual ~FreeFlyCamera();

  void moveForward(Uint32 distance, double speed);
  void moveBackward(Uint32 distance, double speed);
  void moveStrafeRight(Uint32 distance, double speed);
  void moveStrafeLeft(Uint32 distance, double speed);

  void rotate(float x, float y);
protected:
  double _speed;
  double _sensivity;

  //vertical motion stuffs
  Uint32 _timeBeforeStoppingVerticalMotion;
  bool _verticalMotionActive;
  int _verticalMotionDirection;

  typedef std::map<SDLKey,bool> KeyStates;
  KeyStates _keystates;
  typedef std::map<std::string,SDLKey> KeyConf;
  KeyConf _keyconf;

  CPoint _position;
  CPoint _target;
  CPoint _forward;
  CPoint _left;
  double _theta;
  double _phi;

  void VectorsFromAngles();
  void recalc();
};
#endif

/* GraphicalEnvironment */
class GraphEnv
{
public:
  GraphEnv();
  GraphEnv(int width, int height, int bpp, char fullscreen);
  ~GraphEnv();

  int  get_height();
  void set_height(int height);
  int  get_width();
  void set_width(int width);
  char get_fullscreen();
  void set_fullscreen(char fullscreen);
  int  get_bpp();
  void set_bpp(int bpp);

  void OrtoOn(float xres = 640, float yres = 480);
  void OrtoOff(void);

  void auxAxis();

  void cameraFovLH(float fov, float aspect, float fNear, float fFar);
  void cameraLookAtLH(CPoint camPosition, CPoint camTarget, CPoint camUp);

  void     loadMatrix(CMatrix m);
  CMatrix  getProjectionMatrix();
  CMatrix  getModelViewMatrix();

#ifdef FREE_LOOK
  FreeFlyCamera* getActiveCamera();
  void     setActiveCamera(FreeFlyCamera *activeCamera);
#endif
private:
  int  height_;
  int  width_;
  int  bpp_;
  bool fullscreen_;

  // FIXME
  // NOT used !
  std::string windowTitle_;

#ifdef FREE_LOOK
  FreeFlyCamera dummyCamera_;
  FreeFlyCamera *activeCamera_;
#endif
};


int	init_GL(GraphEnv *graphical_env);
void	Reshape(int w, int h);
void	draw_repere(unsigned int scale = 1);
int	takeScreenshot(const char *filename);
void	drawTVNoise(void);

#endif /* !GRAPHIC_HH_ */
