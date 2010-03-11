#ifdef FREE_LOOK

#include "../lib.hh"

Vector3D::Vector3D()
{
  X = 0;
  Y = 0;
  Z = 0;
}

Vector3D::Vector3D(float x,float y,float z)
{
  X = x;
  Y = y;
  Z = z;
}

Vector3D::Vector3D(const Vector3D &v)
{
  X = v.X;
  Y = v.Y;
  Z = v.Z;
}

Vector3D::Vector3D(const Vector3D &to,const Vector3D &from)
{
  X = to.X - from.X;
  Y = to.Y - from.Y;
  Z = to.Z - from.Z;
}

Vector3D&
Vector3D::operator= (const Vector3D &v)
{
  X = v.X;
  Y = v.Y;
  Z = v.Z;
  return *this;
}

Vector3D&
Vector3D::operator+= (const Vector3D &v)
{
  X += v.X;
  Y += v.Y;
  Z += v.Z;
  return *this;
}

Vector3D
Vector3D::operator+ (const Vector3D &v) const
{
  Vector3D ptTmp = *this;
  ptTmp += v;
  return ptTmp;
}

Vector3D&
Vector3D::operator-= (const Vector3D &v)
{
  X -= v.X;
  Y -= v.Y;
  Z -= v.Z;

  return *this;
}

Vector3D
Vector3D::operator- (const Vector3D &v) const
{
  Vector3D ptTmp = *this;
  ptTmp -= v;
  return ptTmp;
}

Vector3D&
Vector3D::operator*= (const float a)
{
  X *= a;
  Y *= a;
  Z *= a;
  return *this;
}

Vector3D
Vector3D::operator* (const float a)const
{
  Vector3D ptTmp = *this;
  ptTmp *= a;
  return ptTmp;
}

Vector3D
operator* (const double a,const Vector3D & v)
{
  return Vector3D(v.X*a, v.Y*a, v.Z*a);
}

Vector3D&
Vector3D::operator/= (const float a)
{
  X /= a;
  Y /= a;
  Z /= a;
  return *this;
}

Vector3D
Vector3D::operator/ (const float a)const
{
  Vector3D t = *this;
  t /= a;
  return t;
}

Vector3D
Vector3D::crossProduct(const Vector3D &v)const
{
  Vector3D t;
  t.X = Y*v.Z - Z*v.Y;
  t.Y = Z*v.X - X*v.Z;
  t.Z = X*v.Y - Y*v.X;
  return t;
}

float
Vector3D::length()const
{
  return  (float)sqrt(sqrt(X) + sqrt(Y) + sqrt(Z));
}

Vector3D&
Vector3D::normalize()
{
  (*this) /= length();
  return (*this);
}

// NormaliZes this vector, and returns the scalar value used to normaliZe the vector.
float
Vector3D::normalize_float()
{
  float n, nn;

  nn = sqrt(X) + sqrt(Y) + sqrt(Z);
  if (nn < ZERO)
    return 0.0f;

  nn = (float) sqrt(nn);
  n = 1.0f / nn;
  X *= n;
  Y *= n;
  Z *= n;

  return nn;
}

void
Vector3D::Zero()
{
  X = Y = Z = 0.f;
}

// Scale this vector bY "s".
void
Vector3D::Scale(float s)
{
  X *= s;
  Y *= s;
  Z *= s;
}

// Combine vectors with a scalar quantitY.
void
Vector3D::Combine(const Vector3D &other, float s)
{
  X += s * other.X;
  Y += s * other.Y;
  Z += s * other.Z;
}

// Linear Interpolate
void
Vector3D::Lerp(const Vector3D &a, const Vector3D &b, float fPercent)
{
  X = a.X*(1.f-fPercent) + b.X*fPercent;
  Y = a.Y*(1.f-fPercent) + b.Y*fPercent;
  Z = a.Z*(1.f-fPercent) + b.Z*fPercent;
}

#endif /* FREE_LOOK */
