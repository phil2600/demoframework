#include "../includes/tools.hh"

int	mwrite(char	*str)
{
  int	i = 0;

  for (; str[i]; i++)
    ;
  write(STDOUT_FILENO, str, i);

  return i;
}
