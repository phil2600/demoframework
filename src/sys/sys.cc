#include "lib.hh"

int	sys_init(unsigned int	h)
{
  if (!glextInit())
    return 0;
#ifdef DEBUG
  mwrite("== glextInit : OK ==\n");
#endif

  timer_init(1);

  //    msys_fontInit(h);

  return 1;
}

void	sys_end(void)
{
}
