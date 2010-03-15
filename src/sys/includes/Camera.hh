#ifndef CAMERA_HH_
# define CAMERA_HH_

#include "../lib.hh"

class CCamera
{
public:
  CPoint m_vposition;
  CPoint m_vlook;
  CPoint m_vtarget;

  CPoint m_vup;
  CPoint m_vright;

  float fov;

  float zNear,zFar;
  float aspect;

  CCamera();

  void Place(CPoint position, CPoint targetPosition, CPoint upVector);

  void SetFov(float _fov);
  float GetFov(void);
  void SetFar(float nearz,float farz);
  void SetAspect(float aspect);
  void MoveForward(float distance);
  void StrafeRight(float distance);
  void MoveUpward(float distance);
  void rotateX(float degrees);
  void rotateY(float degrees);
  void rotateZ(float degrees);
  CPoint getUp();
  CPoint getRight();
  CPoint getTarget();

  void toOGL(void);

  bool inFrustrum(CPoint p);

private:

  void recalc(void);
  unsigned int isReady;

  void SetPos(float x,float y, float z);
  void SetPos(CPoint _position);
  void SetLook(float _x,float _y, float _z);
};

#endif /* !CAMERA_HH_ */
