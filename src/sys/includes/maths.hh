#ifndef MATHS_HH_
# define MATHS_HH_

#ifdef CALC_MATHS
# include <math.h>
# include "maths/trigo.hh"
#else
# ifdef FAST_MATHS

#  ifdef IA32
inline float sys_sinf( const float x) { float r; _asm fld  dword ptr [x];
  _asm fsin;
  _asm fstp dword ptr [r];
  return r; }
inline float sys_cosf( const float x) { float r; _asm fld  dword ptr [x];
  _asm fcos;
  _asm fstp dword ptr [r];
  return r; }
inline float sys_sqrtf(const float x) { float r; _asm fld  dword ptr [x];
  _asm fsqrt;
  _asm fstp dword ptr [r];
  return r; }
inline float sys_fabsf(const float x) { float r; _asm fld  dword ptr [x];
  _asm fabs;
  _asm fstp dword ptr [r];
  return r; }
inline float sys_tanf( const float x) { float r; _asm fld  dword ptr [x];
  _asm fptan; _asm fstp st(0)
    _asm fstp dword ptr [r];
  return r; }

inline void sys_sincosf( float x, float *r ) {   _asm fld dword ptr [x];
  _asm fsincos;
  _asm fstp dword ptr [r+0];
  _asm fstp dword ptr [r+4];
}
float sys_log2f( const float x );
float sys_expf( const float x );
float sys_fmodf( const float x, const float y );
float sys_powf( const float x, const float y );
float sys_floorf( const float x );
int   sys_ifloorf( const float x );

#  else // x64
extern "C" float sys_sinf(  const float x );
extern "C" float sys_cosf(  const float x );
extern "C" float sys_sqrtf( const float x );
extern "C" float sys_fabsf( const float x );
extern "C" float sys_tanf(  const float x );
extern "C" void  sys_sincosf( float x, float *r );
extern "C" float sys_log2f( const float x );
extern "C" float sys_expf( const float x );
extern "C" float sys_fmodf( const float x, const float y );
extern "C" float sys_powf( const float x, const float y );
extern "C" float sys_floorf( const float x );
extern "C" int   sys_ifloorf( const float x );
#  endif /* !IA32 */


# else // SLOW_MATHS
# include <math.h>
#  define sys_sinf(a)       sinf(a)
#  define sys_cosf(a)       cosf(a)
#  define sys_sqrtf(a)      sqrtf(a)
#  define sys_atanf(a)      atanf(a)
#  define sys_atan2f(a,b)   atan2f(a,b)
#  define sys_tanf(a)       tanf(a)
#  define sys_fabsf(a)      fabsf(a)
#  define sys_logf(a)       logf(a)
#  define sys_log10f(a)     log10f(a)
#  define sys_expf(a)       expf(a)
# endif /* !FAST_MATHS */

#endif /* !CALC_MATHS */
#endif /* !MATHS_HH_ */
