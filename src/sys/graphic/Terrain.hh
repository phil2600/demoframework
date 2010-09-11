#ifndef TERRAIN_HH_
# define TERRAIN_HH_

# include "../includes/graphic.hh"
# include "../includes/Algebre.hh"
# include "Perlin.hh"

#define NB_PTS_LAYER 512

#define L_AUCUN 0
#define L_SIMPLE 1
#define L_PATCH 2
#define L_LDOTN1 3
#define L_LDOTN4 4
#define L_RAYSIMPLE 5
#define L_RAYPATCH 6
#define L_MAX 7

#define IMAGE_FILE "data/heightmap.bmp"

//nombre maximum de points dans le terrain
#define MAX_POINTS 200

/* Structure generale pour le terrain */
typedef struct sTerrain
{
  unsigned int tex, watertxt;
  double hauteur[MAX_POINTS][MAX_POINTS];
} STerrain;

class Terrain
{
public:
  Terrain(GraphEnv* graphical_env);
  ~Terrain();

  void display_terrain();
  void makeTerrain();

  int    get_lighting_mode() { return lighting_mode_; }
  int    get_is_watered() { return is_watered_; }
  int    get_is_wired() { return is_wired_; }
  double get_max_height() { return max_height_; }
  double get_water_level() { return water_level_; }
  void   set_lighting_mode(int lighting_mode) { lighting_mode_ = lighting_mode; re_process_ = 1; }
  void   set_is_watered(int is_watered) { is_watered_ = is_watered; }
  void   set_is_wired(int is_wired) { is_wired_ = is_wired; }
  void   set_max_height(double max_height) { max_height_ = max_height; re_process_ = 1; }
  void   set_water_level(double water_level) { water_level_ = water_level; }

  void   re_process() { re_process_ = 1; }
  void   set_is_textured(int is_textured) { is_textured_ = is_textured; }
  int    get_is_textured() { return is_textured_; }

private:

  Perlin   *perlin_;
  STerrain *terrain_;
  GraphEnv *graphical_env_;

  double max_height_;
  double water_level_;
  int is_wired_;
  int is_textured_;
  int is_watered_;
  int lighting_mode_;


  double lum_;
  double l_veci_;
  double l_vecj_;
  double l_vecz_;
  double l_min_;
  double l_max_;

  double l_adouc_;
  double l_ldotnmultiple_;

  char re_process_;

  STerrain*     Terrain_init(SDL_Surface*);

  void		draw3D();
  void		drawFull3D();

  unsigned char	GetPixelColor(SDL_Surface *image, int i, int j,int k);
  double	GetHauteur(SDL_Surface *image, int i, int j, int maxi, int maxj);

  void		RemplitPerc(float *perc, unsigned char haut);
  int		AppliquePatch(double **lightmap, int maxi, int maxj, int taille_patch);

  double	calcLightMap_Simple(SDL_Surface *image, int i, int j,      int maxi, int maxj);
  double	calcLightMap_LDOTN(SDL_Surface  *image, int i, int j,      int maxi, int maxj);
  double	calcLightMap_Ray(SDL_Surface    *image, int i, int j,      int maxi, int maxj, CPoint *light);
  void		calcLightMap(SDL_Surface        *image, double **lightmap, int maxi, int maxj);

  void		Destruction();
  STerrain*	Init(SDL_Surface *image);

  void		TriangleDrawing(char is_textured);
  STerrain*	FreeTextures(SDL_Surface *a, SDL_Surface *b, SDL_Surface *c, SDL_Surface *d, SDL_Surface *e, STerrain* res);
};

#endif /* !TERRAIN_HH_ */
