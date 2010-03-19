#include "../includes/tools.hh"

int	mwrite(char	*str)
{
  int	i = 0;

  for (; str[i]; i++)
    ;
  write(STDOUT_FILENO, str, i);

  return i;
}

void test_cpu_instructions()
{
  unsigned int cpeinfo;
  unsigned int cpsse3;

  asm volatile ("mov $1, %%eax\n"			\
		"cpuid\n"				\
		"mov %%edx, %0\n"			\
		"mov %%ecx, %1\n"			\
		: "=r" (cpeinfo), "=r" (cpsse3));

  std::cout << "MMX:  "
 	    << ((cpeinfo >> 23 & 0x1) == 1 ? "[OK]" : "[FAIL]")
	    << std::endl
	    << "SSE:  " << ((cpeinfo >> 25 & 0x1) == 1 ? "[OK]" : "[FAIL]")
	    << std::endl
	    << "SSE2: " << ((cpeinfo >> 26 & 0x1) == 1 ? "[OK]" : "[FAIL]")
	    << std::endl
	    << "SSE3: " << ((cpsse3 & 0x1) == 1 ? "[OK]" : "[FAIL]")
	    << std::endl;
}

