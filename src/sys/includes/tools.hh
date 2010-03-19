#ifndef TOOLS_HH_
# define TOOLS_HH_

# include <unistd.h>
# include <iostream>

// __asm__ ("assembly code string"
//          : [output list]
//          : [input list]
//          : [clobbered registers list] );
// output : "=r" (a) read on a, %0
// input  : "=r" ... and "a" for eax ...


# define INT80(A,B)			   \
  asm volatile ("int $0x80\n"		   \
		:			   \
		: "a" (A), "b" (B));

int	mwrite(char	*str);

void test_cpu_instructions();

#endif /* !TOOLS_HH_ */
