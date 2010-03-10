#include "../lib.hh"

Vector3D::Vector3D(double x,double y,double z)
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

// Cross Product Constructor
Vector3D::Vector3D(const Vector3D &a,const Vector3D &b)
{
  X = a.Y * b.Z - b.Y * a.Z;
  Y = b.X * a.Z - a.X * b.Z;
  Z = a.X * b.Y - b.X * a.Y;
}

// Assignment opperator.
Vector3D&
Vector3D::operator= (const Vector3D &point)
{
  X = point.X;
  Y = point.Y;
  Z = point.Z;

  return *this;
}


Vector3D&
Vector3D::operator+= (const Vector3D &v)
{
  Add(v);

  return *this;
}

Vector3D
Vector3D::operator+ (const Vector3D &v) const
{
  Vector3D ptTmp(v);
  ptTmp.Add(*this);
  return ptTmp;
}

Vector3D&
Vector3D::operator-= (const Vector3D &v)
{
  Subtract(v);

  return *this;
}

Vector3D
Vector3D::operator- (const Vector3D &v) const
{
  Vector3D ptTmp(v);
  ptTmp.Subtract(*this);
  return ptTmp;
}

Vector3D&
Vector3D::operator*= (const double a)
{

}

Vector3D
Vector3D::operator* (const double a)const
{

}

// friend Vector3D
// Vector3D::operator* (const double a,const Vector3D &v)
// {

// }

Vector3D&
Vector3D::operator/= (const double a)
{

}

Vector3D
Vector3D::operator/ (const double a)const
{

}

Vector3D
Vector3D::crossProduct(const Vector3D &v)const
{
  Vector3D ptTmp(*this, v);

  return ptTmp;
}

double
Vector3D::length()const
{
  return  (float)sqrt(sqrt(X) + sqrt(Y) + sqrt(Z));
}

Vector3D&
Vector3D::normalize()
{

}

// NormaliZes this vector, and returns the scalar value used to normaliZe the vector.
float
Vector3D::Normalize()
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

// Add the other vector to this vector.
void
Vector3D::Add(const Vector3D &other)
{
  X += other.X;
  Y += other.Y;
  Z += other.Z;
}

// Subtract the other vector from this vector.
void
Vector3D::Subtract(const Vector3D &other)
{
  X -= other.X;
  Y -= other.Y;
  Z -= other.Z;
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
