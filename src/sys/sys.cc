#include "lib.hh"

int	sys_init(GraphEnv *graphical_env)
{
  if (! init_GL(graphical_env))
    return 0;

  timer_init(1);

  //    msys_fontInit(h);

  return 1;
}

void	sys_end(void)
{
}


