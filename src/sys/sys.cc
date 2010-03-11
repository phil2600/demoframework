#include "lib.hh"

int	sys_init(unsigned int	h)
{
  if (! init_GL(WIDTH, HEIGHT, BPP, FULLSCREEN))
    return 0;

  timer_init(1);

  //    msys_fontInit(h);

  return 1;
}

void	sys_end(void)
{
}


