#ifndef GRAPHIC_HH_
# define GRAPHIC_HH_

# include "SDL.h"
# include "SDL_opengl.h"

# include "../lib.hh"
# include "Algebre.hh"
# include "Camera.hh"

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
  CCamera* getActiveCamera();
  void     setActiveCamera(CCamera *activeCamera);
private:
  int  height_;
  int  width_;
  int  bpp_;
  bool fullscreen_;
  std::string m_windowTitle;

  CCamera m_dummyCamera;
  CCamera *m_activeCamera;

};

int	init_GL(GraphEnv *graphical_env);

void	Reshape(int w, int h);

void	draw_repere(unsigned int scale = 1);

int	takeScreenshot(const char *filename);

void	drawTVNoise(void);

/* Vector3D */
#define EPSILON  	1e-8
#define ZERO  	 	EPSILON

class Vector3D
{
public:
  double X;
  double Y;
  double Z;

  Vector3D();
  Vector3D(double x,double y,double z);
  Vector3D(const Vector3D & v);
  Vector3D(const Vector3D & from,const Vector3D & to);

  Vector3D & operator= (const Vector3D & v);

  Vector3D & operator+= (const Vector3D & v);
  Vector3D operator+ (const Vector3D & v) const;

  Vector3D & operator-= (const Vector3D & v);
  Vector3D operator- (const Vector3D & v) const;

  Vector3D & operator*= (const double a);
  Vector3D operator* (const double a)const;
  friend Vector3D operator* (const double a,const Vector3D & v);

  Vector3D & operator/= (const double a);
  Vector3D operator/ (const double a)const;

  Vector3D crossProduct(const Vector3D & v)const;
  double length()const;
  Vector3D & normalize();
};


/* FlyCam */
class FreeFlyCamera
{
public:
  FreeFlyCamera(const Vector3D & position = Vector3D(0,0,0));

  virtual void OnMouseMotion(const SDL_MouseMotionEvent & event);
  virtual void OnMouseButton(const SDL_MouseButtonEvent & event);
  virtual void OnKeyboard(const SDL_KeyboardEvent & event);

  virtual void animate(Uint32 timestep);
  virtual void setSpeed(double speed);
  virtual void setSensivity(double sensivity);

  virtual void setPosition(const Vector3D & position);

  virtual void look();

  virtual ~FreeFlyCamera();
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

  Vector3D _position;
  Vector3D _target;
  Vector3D _forward;
  Vector3D _left;
  double _theta;
  double _phi;

  void VectorsFromAngles();
};

#endif /* !GRAPHIC_HH_ */
