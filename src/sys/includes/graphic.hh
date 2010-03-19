#ifndef GRAPHIC_HH_
# define GRAPHIC_HH_

# include "GraphEnv.hh"
# include "defines.hh"
# include "SDL.h"
# include "SDL_opengl.h"
# include "SDL_image.h"

int	init_GL(GraphEnv *graphical_env);
void	Reshape(int w, int h);
void	draw_repere(unsigned int scale = 1);
int	takeScreenshot(const char *filename);
void	drawTVNoise(void);
GLuint loadTexture(const char * filename,bool useMipMap = true);

void draw_textured_ground(GLuint texture, unsigned int size = 100);
void draw_textured_cube(GLuint texture);

//void perlin();
void myterrain();

#endif /* !GRAPHIC_HH_ */
