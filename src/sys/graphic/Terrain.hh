#ifndef TERRAIN_HH_
# define TERRAIN_HH_

# include "../includes/graphic.hh"
# include "../includes/Algebre.hh"
# include "Perlin.hh"

#define NB_PTS_LAYER 512
/* Pour la lumière */
#define L_AUCUN 0
#define L_SIMPLE 1
#define L_PATCH 2
#define L_LDOTN1 3
#define L_LDOTN4 4
#define L_RAYSIMPLE 5
#define L_RAYPATCH 6
#define L_MAX 7

#define IMAGE_FILE "data/heightmap.bmp"

typedef struct sParam
{
	int changer;			/* Est-ce qu'il faudrait refaire le terrain */
	int f1;				/* Est-ce que F1 est active (donc l'aide) */
	int text; 			/* Dessine-t-on le texte */
	int fps;			/* Pour le calcul du nombre d'images par seconde */
	double hautmax; 		/* Hauteur maximal pour le terrain */

	/* Parametres pour l'affichage */
	int filaire;			/* Filaire ou non */

	double angle;			/* Le premier angle */
	double angle2;			/* Le 2eme angle */
	double trans;			/* Le Zoom */
	double lum;			/* Multiplicateur de lumiere */

	int water;			/* Ajoute-t-on de l'eau */
	double w_cur;			/* Niveau courant de l'eau */

	int light;			/* Type de lumière */
	double l_veci, l_vecj,l_vecz; 	/* Vecteur de lumière */
	double l_min, l_max; 		/* Minimum et Max de lumière de la scène */
	double l_adouc; 		/* Facteur d'adoucissant */
	double l_ldotnmultiple; 	/* Facteur multiplicatif */

    /* paramètres de l'algo de Perlin */
	int frequence;
	int octaves;
	float persistance;

	s_layer *random;
}SParam;
//nombre maximum de points dans le terrain
#define MAX_POINTS 64

/* Structure generale pour le terrain */
typedef struct sTerrain
{
	unsigned int tex,watertxt;
	double hauteur[MAX_POINTS][MAX_POINTS];
}STerrain;

void colorerPixel(SDL_Surface*, int, int, Uint32);
unsigned char aleatoire(float);
void enregistrer_bmp(s_layer *, const char *);



#endif /* !TERRAIN_HH_ */
