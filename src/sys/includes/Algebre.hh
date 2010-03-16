#ifndef ALGEBRE_HH_
# define ALGEBRE_HH_

# include <stdio.h>
# include <cmath>
# include <cstdlib>
# include <iostream>

#define EPSILON  	1e-8
#define ZERO  	 	EPSILON

class CPoint;
class CMatrix;
class CTransformation;
class CMatrix3d;

class CPoint
{
public:
  float x,y,z,w;

  CPoint(float xx=0.0, float yy=0.0,
	 float zz=0.0, float ww=1.0);

  CPoint(const CPoint & from,const CPoint & to);

  CPoint & operator= (const CPoint & v);
  CPoint & operator+= (const CPoint & v);
  CPoint operator+ (const CPoint & v) const;

  CPoint & operator-= (const CPoint & v);
  CPoint operator- (const CPoint & v) const;

  CPoint & operator*= (const double a);
  CPoint operator* (const double a)const;
  friend CPoint operator* (const double a,const CPoint & v);

  CPoint & operator/= (const double a);
  CPoint operator/ (const double a)const;

  CPoint crossProduct(const CPoint & v)const;
  double length()const;

  CPoint operator +(CPoint p);
  CPoint operator -(CPoint p);
  float  operator [](int i);
  CPoint operator *(CMatrix m);

  CPoint transform(CTransformation &t);
  CPoint negate();
  CPoint normalize();
  CPoint unitary();

  float module();
  float longitude();
  float latitude();

  void write();

  CPoint operator ^(CPoint p);
  CPoint homogeneus();
  void randomize(CPoint p);
private:
  int isUnitary;
};


class CMatrix
{
public:
  float e[4][4];

  CMatrix();
  CMatrix(CPoint e1,CPoint e2,
	  CPoint e3,CPoint e4=CPoint());
  CPoint row(int i);
  CPoint column(int i);
  CMatrix operator =(const CMatrix &);
  CMatrix operator *(CMatrix m);
  CPoint operator *(CPoint p);
  CMatrix3d operator *(const CMatrix3d &);
  CMatrix operator *(float k);
  CMatrix operator !();
  void escribir();
  void identity();
};



class CTransformation: public CMatrix
{
public:
  void translate(CPoint d);
  void translate(float x, float y, float z, float w);

  void scale(float sx=1.0,float sy=1.0,float sz=1.0, CPoint centro=CPoint());

  void rotate(float ang, CPoint vectorEje, CPoint enRecta=CPoint() );

  void rotateX(float ang);
  void rotateY(float ang);
  void rotateZ(float ang);

  void rotate(CPoint u,CPoint v,CPoint w);
  void rotateQuaternion(float x, float y, float z, float w);
  void rotateQuaternion(CPoint p);

};



class CMatrix3d: public CMatrix
{
public:
  float e[4][4][4];

  CMatrix3d();
  CMatrix3d(CMatrix , CMatrix , CMatrix , CMatrix );

  CPoint rowCol(int i,int j);
  CMatrix3d putIn(int i, int j, CPoint p);
  CMatrix3d operator =(const CMatrix3d &);
  CMatrix3d operator *(const CMatrix &);
  void escribir();
};


#endif /* !ALGEBRE_HH_ */
