#ifndef RANDOM_HH_
# define RANDOM_HH_

# include <cstdlib>

float	random_float();

void  sys_srand( int semilla );
int   sys_rand( void );
float sys_frand( void );
float sys_sfrand( void );


#endif /* !RANDOM_HH_ */
