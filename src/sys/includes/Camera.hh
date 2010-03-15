#ifndef CAMERA_HH_
# define CAMERA_HH_

#include "../lib.hh"

struct key
{
  //  SDLKey key;
  char   state;
};

struct key_management
{
  struct key forward;
  struct key backward;
  struct key strafe_left;
  struct key strafe_right;
  struct key boost;
};

/* FlyCam */
class Camera
{
public:

  Camera(const CPoint & position = CPoint(0,0,0));

  virtual void OnMouseMotion(const SDL_MouseMotionEvent & event);
  virtual void OnMouseButton(const SDL_MouseButtonEvent & event);
  virtual void OnKeyboard(const SDL_KeyboardEvent & event);

  virtual void animate(Uint32 timestep);
  virtual void setSpeed(double speed);
  virtual void setSensivity(double sensivity);

  virtual void setPosition(const CPoint & position);

  virtual void look();

  virtual ~Camera();

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

  struct key_management keys_;

  // Use a fucking lot of memory !!!!
//   typedef std::map<SDLKey,bool> KeyStates;
//   KeyStates _keystates;
//   typedef std::map<std::string,SDLKey> KeyConf;
//   KeyConf _keyconf;

  CPoint _position;
  CPoint _target;
  CPoint _forward;
  CPoint _left;
  double _theta;
  double _phi;

  void VectorsFromAngles();
  void recalc();
};

#endif /* !CAMERA_HH_ */
