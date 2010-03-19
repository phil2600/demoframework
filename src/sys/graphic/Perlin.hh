#ifndef PERLIN_HH_
# define PERLIN_HH_

# include "../includes/graphic.hh"
# include "../includes/Algebre.hh"


struct calque
{
  int **v;
  int taille;
  float persistance;
};

struct calque* init_calque(int, float);
void free_calque(struct calque*);
void generer_calque(int, int, float, int, struct calque*);
int interpolate(int, int, int, int);
int valeur_interpolee(int, int, int, struct calque*);
int perlin(void);

#endif /* !PERLIN_HH_ */
