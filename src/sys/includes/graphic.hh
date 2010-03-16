#ifndef GRAPHIC_HH_
# define GRAPHIC_HH_

# include "GraphEnv.hh"
# include "defines.hh"


int	init_GL(GraphEnv *graphical_env);
void	Reshape(int w, int h);
void	draw_repere(unsigned int scale = 1);
int	takeScreenshot(const char *filename);
void	drawTVNoise(void);

#endif /* !GRAPHIC_HH_ */
