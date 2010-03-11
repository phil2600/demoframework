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
  float X;
  float Y;
  float Z;

  Vector3D();
  Vector3D(float x,float y,float z);
  Vector3D(const Vector3D &v);
  Vector3D(const Vector3D &a,const Vector3D &b);

  Vector3D &operator= (const Vector3D &v);

  Vector3D &operator+= (const Vector3D &v);
  Vector3D operator+ (const Vector3D &v) const;

  Vector3D &operator-= (const Vector3D &v);
  Vector3D operator- (const Vector3D &v) const;

  Vector3D &operator*= (const float a);
  Vector3D operator* (const float a)const;
  friend Vector3D operator* (const float a,const Vector3D &v);

  Vector3D &operator/= (const float a);
  Vector3D operator/ (const float a)const;

  Vector3D crossProduct(const Vector3D &v)const;
  float length()const;
  Vector3D &normalize();

  float normalize_float();
  void  Zero();
  void  Scale(float s);
  void  Combine(const Vector3D &other, float s);
  void  Lerp(const Vector3D &a, const Vector3D &b, float fPercent);
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

  virtual void setSpeed(float speed);
  virtual void setSensivity(float sensivity);
  virtual void setPosition(const Vector3D &position);

  virtual void look();

protected:
  float speed_;
  float sensivity_;

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
  float theta_;
  float phi_;

  void VectorsFromAngles();
};


#endif /* !GRAPHIC_HH_ */
