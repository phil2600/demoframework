#ifndef TRIGO_HH_

# define TRIGO_HH
# define EXTRA_PRECISION
# define PI 3.14159265358979323846
# define PIQ PI*PI
# define PI2 PI/2
# define B 4/PI
# define C -4/(PI*PI)
# define P 0.225
# define SIZE 360
# define INTER (PI*2)/SIZE
# define cosine(X) sine((X) + PI2)
//# define abs(X) (X) > 0 ? (X) : (-X)

float tabcos[SIZE];
float tabsin[SIZE];

float	abs(float x)
{
  if (x >= 0)
    return x;
  return -x;
}


// float	cosine(float x)
// {
//   return sine(x + PI2);
// }

float	sine(float x)
{
  float y = B * x + C * x * abs(x);

#ifdef EXTRA_PRECISION
  y = P * (y * abs(y) - y) + y;
#endif
  return y;
}

void initcossin()
{
  int j = 0;

  for (float i = 0; i < PI && j < SIZE; i += INTER)
  {
    tabsin[j] = sin(i);
    ++j;
  }
  for (float i = -PI; i < 0 && j < SIZE; i += INTER)
  {
    tabsin[j] = sin(i);
    ++j;
  }
  j = 0;
  for (float i = 0; i < PI && j < SIZE; i += INTER)
  {
    tabcos[j] = cos(i);
    ++j;
  }
  for (float i = -PI; i < -PI2 && j < SIZE; i += INTER)
  {
    tabcos[j] = cos(i);
    ++j;
  }
  for (float i = 0; i < PI2 && j < SIZE; i += INTER)
  {
    tabcos[j] = sin(i);
    ++j;
  }
//   tabcos[90] = 0;
//   tabcos[270] = 0;
//   tabcos[271] = 0;
//   tabsin[0] = 0;
//   tabsin[180] = 0;
//   tabsin[181] = 0;
}

#endif
