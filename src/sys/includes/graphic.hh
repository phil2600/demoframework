#ifndef GRAPHIC_HH_
# define GRAPHIC_HH_

# include "SDL.h"
# include "SDL_opengl.h"

# include "../lib.hh"

int	init_GL(int	width, int height, int bpp, char fullscreen);

void	Reshape(int w, int h);

void	draw_repere(unsigned int scale = 1);

/* Vector3D */

#define EPSILON  	1e-8
#define ZERO  	 	EPSILON

class Vector3D
{
public:
  double X;
  double Y;
  double Z;

  Vector3D() {};
  Vector3D(double x,double y,double z);
  Vector3D(const Vector3D &v);
  Vector3D(const Vector3D &a,const Vector3D &b);

  Vector3D &operator= (const Vector3D &v);

  Vector3D &operator+= (const Vector3D &v);
  Vector3D operator+ (const Vector3D &v) const;

  Vector3D &operator-= (const Vector3D &v);
  Vector3D operator- (const Vector3D &v) const;

  Vector3D &operator*= (const double a);
  Vector3D operator* (const double a)const;
//   friend Vector3D operator* (const double a,const Vector3D &v);

  Vector3D &operator/= (const double a);
  Vector3D operator/ (const double a)const;

  Vector3D crossProduct(const Vector3D &v)const;
  double length()const;
  Vector3D &normalize();

  float Normalize();
  void Zero();
  void Scale(float s);
  void Add(const Vector3D &other);
  void Subtract(const Vector3D &other);
  void Combine(const Vector3D &other, float s);
  void Lerp(const Vector3D &a, const Vector3D &b, float fPercent);

  void CrossProduct(const Vector3D &a, const Vector3D &b);
};


/* FlyCam */

class FreeFlyCamera
{
public:
  FreeFlyCamera(const Vector3D &position = Vector3D(0,0,0));
  virtual ~FreeFlyCamera();

  virtual void OnMouseMotion(const SDL_MouseMotionEvent &event);
  virtual void OnMouseButton(const SDL_MouseButtonEvent &event);
  virtual void OnKeyboard(const SDL_KeyboardEvent &event);

  virtual void animate(Uint32 timestep);

  virtual void setSpeed(double speed);
  virtual void setSensivity(double sensivity);
  virtual void setPosition(const Vector3D &position);

  virtual void look();

protected:
  double speed_;
  double sensivity_;

  Uint32 timeBeforeStoppingVerticalMotion_;
  bool verticalMotionActive_;
  int verticalMotionDirection_;

  typedef std::map<SDLKey,bool> KeyStates;
  KeyStates _keystates;
  typedef std::map<std::string,SDLKey> Keyconf;
  Keyconf keyconf_;

  Vector3D position_;
  Vector3D target_;
  Vector3D forward_;
  Vector3D left_;
  double theta_;
  double phi_;

  void VectorsFromAngles();
};


#endif /* !GRAPHIC_HH_ */
