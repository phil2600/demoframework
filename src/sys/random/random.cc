#include "../lib.hh"

/* =====================
** ==== RandomFloat ====
** =====================
** Return: a randomized float
*/
float	random_float()
{
  float		res = 0.f;

  res = (rand() % 1000) / 1000.0f;

  return (res);
}

static int mirand  = 1;

void sys_srand( int semilla )
{
    mirand = semilla;
}

int sys_rand( void )
{
    mirand = mirand * 0x343FD + 0x269EC3;
    return( (mirand>>16)&32767 );
}


float sys_frand( void )
{
    //mirand = mirand * 0x343FD + 0x269EC3;
    mirand *= 16807;

    const unsigned int a = (mirand>>9) | 0x3f800000;
    const float res = (*((float*)&a)) - 1.0f;

    return( res );
}

float sys_sfrand( void )
{
    //mirand = mirand * 0x343FD + 0x269EC3;
    mirand *= 16807;

    const unsigned int a = (mirand>>9) | 0x40000000;
    const float res = (*((float*)&a)) - 3.0f;

    return( res );
}
