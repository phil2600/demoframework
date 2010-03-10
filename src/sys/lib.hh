#ifndef LIB_HH_
# define LIB_HH_

# include <list>
# include <map>
# include <time.h>
# include <iostream>
# include <stdio.h>


#include "includes/defines.hh"

#include "includes/timer.hh"
#include "includes/maths.hh"
#include "includes/graphic.hh"
#include "includes/random.hh"
#include "includes/tools.hh"
#include "includes/physics.hh"

#include "SDL.h"
#include "SDL_opengl.h"

int	sys_init(unsigned int	h);
void	sys_end(void);

#endif /* !LIB_HH_ */
