#include "Terrain.hh"

Terrain::Terrain(GraphEnv* graphical_env)
{
  graphical_env_ = graphical_env;

  graphical_env_->logger("Terrain Init");

  perlin_ = new Perlin(graphical_env_);

  //  lighting_mode_ = L_LDOTN4;
  lighting_mode_ = L_RAYPATCH;
  is_watered_ = 1;
  is_wired_ = 1;
  is_displayed_ = 1;
  water_level_ = 5;
  max_height_ = 15.0;

  lum_ = 1;
  l_veci_ = -1;
  l_vecj_ = 0;
  l_vecz_ = -0.02;

  l_min_ = 0.2;
  l_max_ = 1;

  l_adouc_ = 0.01;
  l_ldotnmultiple_ = 47;

  re_process_ = 0;
}

Terrain::~Terrain()
{
}

void
Terrain::TriangleDrawing(char is_textured)
{
  double adding = 40.0 / MAX_POINTS;
  double posi;
  double posj;
  int i;
  int j;

  glBegin(GL_TRIANGLES);
  for(i = 0, posi = -20; i < MAX_POINTS-1; i++, posi += adding)
  {
    for(j = 0, posj = -20; j < MAX_POINTS-1; j++, posj += adding)
    {
      if (is_textured)
	glTexCoord2f(((float)j)/MAX_POINTS,   ((float) i)/MAX_POINTS);
      glVertex3d(posi,        posj,        terrain_->hauteur[i][j]);

      if (is_textured)
	glTexCoord2f(((float)j+1)/MAX_POINTS, ((float) i)/MAX_POINTS);
      glVertex3d(posi,        posj+adding, terrain_->hauteur[i][j+1]);

      if (is_textured)
	glTexCoord2f(((float)j+1)/MAX_POINTS, ((float) i+1)/MAX_POINTS);
      glVertex3d(posi+adding, posj+adding, terrain_->hauteur[i+1][j+1]);


      if (is_textured)
	glTexCoord2f(((float)j)/MAX_POINTS,   ((float) i)/MAX_POINTS);
      glVertex3d(posi,        posj,        terrain_->hauteur[i][j]);

      if (is_textured)
	glTexCoord2f(((float)j+1)/MAX_POINTS, ((float) i+1)/MAX_POINTS);
      glVertex3d(posi+adding, posj+adding, terrain_->hauteur[i+1][j+1]);

      if (is_textured)
	glTexCoord2f(((float)j)/MAX_POINTS,   ((float) i+1)/MAX_POINTS);
      glVertex3d(posi+adding, posj,        terrain_->hauteur[i+1][j]);
    }
  }
  glEnd();
}

void
Terrain::draw3D()
{
  // Empty Polygons
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // Green
  glColor3f(0.0, 1.0, 0.0);

    TriangleDrawing(0);

  // On black
  glColor3f(0.0, 0.0, 0.0);

  /* Full Polygons */
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  /* Back draw Polygons */
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(1.0, 1.0);

    TriangleDrawing(0);

  glDisable(GL_POLYGON_OFFSET_FILL);
}

/* Dessin en 3D plein */
void
Terrain::drawFull3D()
{
  int i,j;
  double ajout = 40.0 / MAX_POINTS,posi,posj;

  /* Full Polygons */
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, terrain_->tex);

  glColor3f(11.0f, 11.0f, 11.0f);

    TriangleDrawing(1);

  glDisable(GL_TEXTURE_2D);
}

unsigned char
Terrain::GetPixelColor(SDL_Surface *image, int i, int j,int k)
{
  return ((unsigned char*)image->pixels)[i*image->w*3+j*3+k];
}

/*
 * Recuperation de la hauteur d'un point
 * Puisque notre texture generee n'est pas forcement de la meme taille que l'image de niveaux,
 * nous calculons un ratio entre [0,1] ensuite on remultiplie par la hauteur et la largeur de l'image
 */
double
Terrain::GetHauteur(SDL_Surface *image, int i, int j, int maxi, int maxj)
{
  double di, dj;
  double res;
  di = i;
  dj = j;

  di /= maxi;
  dj /= maxj;

  di *= image->h;
  dj *= image->w;

  i = di;
  j = dj;

  res = (GetPixelColor(image, i, j, 0)/256.0) * max_height_;

  return res;
}

/* Cette fonction remplit le pourcentage que ce pixel doit avoir dependant de l'hauteur associe
 * Si l'hauteur est faible, c'est que de l'herbe
 * Sinon c'est un melange herbe - roche
 * Plus haut, c'est que de la roche
 * Ensuite un melange roche - neige
 * Et enfin, c'est que de la neige
 */
void
Terrain::RemplitPerc(float *perc, unsigned char haut)
{
  /* On utilise la fonction rand pour mettre de l'aleatoire */
  int add = haut + (rand() % 30) - 15;

  /* 0 <= add <= 255 */
  add = (add < 0) ? 0 : ((add > 255) ? 255 : add);
  haut = add;

  /* prairie */
  if (haut < 60)
  {
    perc[0] = 1.0f;
    perc[1] = 0.0f;
    perc[2] = 0.0f;
  }
  /* prairie & roche */
  else if (haut < 130)
  {
    perc[0] = 1.0f - (haut-60.0f)/70.0f;
    perc[1] = (haut-60.0f)/70.0f;
    perc[2] = 0.0f;
  }
  /* roche */
  else if (haut < 180)
  {
    perc[0] = 0.0f;
    perc[1] = 1.0f;
    perc[2] = 0.0f;
  }
  /* roche & neige */
  else if (haut < 220)
  {
    perc[0] = 0.0f;
    perc[1] = 1.0f - (haut-180.0f)/40.0f;
    perc[2] = (haut-180.0f)/40.0f;
  }
  /* neige */
  else
  {
    perc[0] = 0.0f;
    perc[1] = 0.0f;
    perc[2] = 1.0f;
  }
}

/*
 * Fonction qui applique un filtre de moyenne sur le tableau lightmap, la taille du filtre
 * depend de la valeur de taille_patch
 */
int
Terrain::AppliquePatch(double **lightmap, int maxi, int maxj, int taille_patch)
{
  double **tmplightmap;

  tmplightmap = (double **)malloc(maxi*sizeof(double*));
  if (! tmplightmap)
    return 1;

  for (int i = 0; i < maxi; i++)
  {
    tmplightmap[i] = (double*)malloc(maxj*sizeof(double));
    if(! tmplightmap[i])
    {
      i--;
      while(i >= 0)
	free(tmplightmap[i--]);
      free(tmplightmap);
      return 1;
    }
  }

  /* Average */
  for(int i = 0; i < maxi; i++)
    for(int j = 0; j < maxj; j++)
    {
      tmplightmap[i][j] = 0.0;

      int cnt = 0;

      for(int k   = i-taille_patch; k <= i+taille_patch; k++)
	for(int l = j-taille_patch; l <= j+taille_patch; l++)
	  if ((((unsigned) (k)) < maxi) && (((unsigned) (l)) < maxj))
	  {
	    tmplightmap[i][j] += lightmap[k][l];
	    cnt++;
	  }

      /* Average calculate */
      if(cnt)
	tmplightmap[i][j] /= cnt;
      else
	tmplightmap[i][j] = 1.0f;
    }

  /* Copy */
  for(int i = 0; i < maxi; i++)
    for(int j = 0; j < maxj; j++)
      lightmap[i][j] = tmplightmap[i][j];

  return 0;
}

/*
 * Fonction qui calcule le coefficient de luminosite
 *
 * Version la plus simple, on regarde localement si, dans la direction de la source de lumiere,
 * le terrain n'est pas plus haut. Si c'est le cas, alors cet endroit sera moins eclaire
 */
double
Terrain::calcLightMap_Simple(SDL_Surface *image, int i, int j, int maxi, int maxj)
{
  if ((((unsigned) (i-l_veci_)) < maxi) && (((unsigned) (j-l_vecj_)) < maxj))
    return 1.0
      - (GetHauteur(image, i-l_veci_, j-l_vecj_, maxi, maxj)
      -  GetHauteur(image, i,              j,              maxi, maxj)) / l_adouc_;
  else
    return 1.0f;
}

/*
 * Fonction qui calcule le coefficient de luminosite
 *
 * On utilise le vecteur normal et un produit scalaire. Nettement plus mathematique et precis que la
 * version de calcLightMap_Simple.
 */
double
Terrain::calcLightMap_LDOTN(SDL_Surface *image, int i, int j, int maxi, int maxj)
{
  CPoint v1,v2,light,n;
  double tmp;

  /* On doit récupérer la normale de ce point. Le plus simple est de définir deux vecteurs.
   * On va prendre les hauteurs de trois coins pour le faire
   */
  light.x = l_veci_;
  light.y = l_vecj_;
  light.z = l_vecz_;
  light.normalize();

  if((i > 0) &&
     (j > 0) &&
     (i < maxi-1) &&
     (j < maxj-1))
  {
    /* Vectorization
     * First Vector (i-1,j-1) to (i+1,j-1)
     * Second Vector (i+1,j-1) to (i+1,j+1)
     */
    v1.x = 2; v1.y = 0;
    v1.z = GetHauteur(image,i+1,j-1, maxi, maxj) - GetHauteur(image,i-1,j-1, maxi, maxj);

    v2.x = 0; v2.y = 2;
    v2.z = GetHauteur(image,i+1,j+1, maxi, maxj) - GetHauteur(image,i+1,j-1, maxi, maxj);

    v1.normalize();
    v2.normalize();

    // Get Normal
    n = v1 ^ v2;
    // Normalize the Normal
    n.normalize();

    if (n.z < 0)
    {
      n.x *= -1;
      n.y *= -1;
      n.z *= -1;
    }

    /* L dot N */
    tmp = light.ScalarProduct(n);

    if (tmp < 0)
      return -l_ldotnmultiple_*tmp;
    else
      return 0.0f;
  }

  return 1.0f;
}

/*
 * Fonction qui calcule le coefficient de luminositédu pixel (i,j) de la texture de taille maxi*maxj
 * Utilisant l'image de niveaux image et le vecteur de lumiere light, il regarde s'il y a une intersection
 * entre le rayon de vecteur light partant point de depart (i,j) et du terrain.
 */
double
Terrain::calcLightMap_Ray(SDL_Surface *image, int i, int j, int maxi, int maxj, CPoint *light)
{
  CPoint cur;
  float tmp;

  /*
   * Verification s'il y a une intersection avec le terrain, si
   * c'est le cas, il n'y aura pas de lumiere ici
   */

  /*
   * Point de depart, le point(i,j) avec son hauteur
   */
  tmp = i;
  tmp /= maxi;
  tmp *= image->h;

  cur.x = tmp;

  tmp = j;
  tmp /= maxj;
  tmp *= image->w;

  cur.y = tmp;
  cur.z = GetHauteur(image, i, j,maxi, maxj);

  /* While into the terrain */
  while((cur.x >= 0) &&
	(cur.y >= 0) &&
	(cur.x < image->h) &&
	(cur.y < image->w))
  {
    /* Current hight */
    tmp = GetHauteur(image, (int)cur.x, (int)cur.y, image->h, image->w);

    if(cur.z > max_height_)
      /* No interssection possible */
      break;

    /* If the terrain is upper the light vector, the terrain'll hide the light */
    if(tmp > cur.z)
      return 0.0;

    cur.x += light->x;
    cur.y += light->y;
    cur.z += light->z;
  }

  /*No interssectin => L dot N */
  return calcLightMap_LDOTN(image, i, j, maxi, maxj);
}

/*
 * Fonction qui calcule les coefficients de luminosite de tout le terrain et remplit le tableau lightmap
 * Dependant de la valeur de param.light, cette fonction decidera quel algorithme utilise
 */
void
Terrain::calcLightMap(SDL_Surface *image, double **lightmap, int maxi, int maxj)
{
  CPoint light;

  light.x = -l_veci_;
  light.y = -l_vecj_;
  light.z = -l_vecz_;
  light.normalize();

  /*
   * Calcul du vecteur lumiere normalise, donc le vecteur qui va vers la source de lumiere
   * Ceci sert pour le calcul d'ombre
   */
  light.x = -l_veci_;
  light.y = -l_vecj_;
  light.z = -l_vecz_;
  light.normalize();

  /* First pass */
  for(int i = 0; i < maxi; i++)
    for(int j = 0; j < maxj; j++)
      switch(lighting_mode_)
      {
	case L_PATCH:
	case L_SIMPLE:
	  lightmap[i][j] = calcLightMap_Simple(image, i, j, maxi, maxj);
	  break;
	case L_LDOTN1:
	case L_LDOTN4:
	  lightmap[i][j] = calcLightMap_LDOTN(image, i, j, maxi, maxj);
	  break;
	case L_RAYSIMPLE:
	case L_RAYPATCH:
	  lightmap[i][j] = calcLightMap_Ray(image, i, j, maxi, maxj, &light);
	  break;
	default:
	  lightmap[i][j] = 1.0f;
	  break;
      }

  /* Second pass */
  switch(lighting_mode_)
  {
    case L_PATCH:
    case L_RAYPATCH:
    case L_LDOTN4:
      /* On applique un lissage 4*4 sur le lightmap */
      AppliquePatch(lightmap, maxi, maxj, 4);
      break;
    case L_LDOTN1:
      /* On applique un lissage 1*1 sur le lightmap */
      AppliquePatch(lightmap, maxi, maxj, 1);
      break;
    default:
      break;
  }
}

void
Terrain::Destruction()
{
  if (terrain_)
  {
    glDeleteTextures(1, &terrain_->tex);
    glDeleteTextures(1, &terrain_->watertxt);
    free(terrain_);
  }
}

STerrain*
Terrain::FreeTextures(SDL_Surface *a, SDL_Surface *b, SDL_Surface *c, SDL_Surface *d, SDL_Surface *e, STerrain* res)
{
  if (a)
    SDL_FreeSurface(a);
  if (b)
    SDL_FreeSurface(b);
  if (c)
    SDL_FreeSurface(c);
  if (d)
    SDL_FreeSurface(d);
  if (e)
    SDL_FreeSurface(e);
  if (res)
    free(res);

  return NULL;
}

STerrain*
Terrain::Init(SDL_Surface *image)
{
  SDL_Surface	*terraintxt, *prairies, *rocheuses, *neige, *water;
  int		tmpi, tmpj;
  double	diff;
  double	r, g, b;
  STerrain	*res;
  float		perc[3];

  graphical_env_->logger("Terrain : Light Init", 1);
  if (! image)
    return NULL;

  res = (STerrain*)malloc(sizeof *res);
  if (! res)
    return NULL;

  graphical_env_->logger("Terrain : Getting Points", 1);
  for (int i = 0; i < MAX_POINTS; i++)
    for (int j = 0; j < MAX_POINTS; j++)
      res->hauteur[i][j] = GetHauteur(image, i, j, MAX_POINTS, MAX_POINTS);

  graphical_env_->logger("Terrain : Loading terrain textures", 1);


  prairies = SDL_LoadBMP("data/grassm.bmp");
  if (! prairies)
    return FreeTextures(NULL, NULL, NULL, NULL, NULL, res);

  rocheuses = SDL_LoadBMP("data/rock.bmp");
  if (! rocheuses)
    return FreeTextures(prairies, NULL, NULL, NULL, NULL, res);

  neige = SDL_LoadBMP("data/snow.bmp");
  if(! neige)
    return FreeTextures(prairies, rocheuses, image, NULL, NULL, res);

  /* OpenGL texture */
  terraintxt = SDL_CreateRGBSurface(SDL_HWSURFACE, 1024, 1024, 32, 8, 8, 8, 0);
  if (! terraintxt)
    return FreeTextures(prairies, rocheuses, image, neige, NULL, res);

  /* coefficients de luminosite */
  double **lightmap = (double**)malloc(sizeof(double)*terraintxt->h);
  if (lightmap == NULL)
    return FreeTextures(prairies, rocheuses, image, neige, terraintxt, res);

  /* Second dimension allocation */
  for (int i = 0; i < terraintxt->h; i++)
  {
    lightmap[i] = (double*)malloc(sizeof(double)*terraintxt->w);
    if (lightmap[i] == NULL)
    {
      FreeTextures(prairies, rocheuses, image, neige, terraintxt, res);
      while( i >= 0)
	free(lightmap[i--]);
      free(lightmap);
      return NULL;
    }
  }

  /* Lightmap calcul */
  calcLightMap(image,lightmap,terraintxt->h,terraintxt->w);

  /* For each pixel => 3 texture composition calcul */
  for (int i = 0; i < terraintxt->h; i++)
    for (int j = 0; j < terraintxt->w; j++)
    {
      /* Recuperation du coefficient de luminosite */
      diff = lightmap[i][j];

      /* Verification des seuils de luminosite */
      if (diff < l_min_)
	diff = l_min_;
      else if (diff > l_max_)
	diff = 1.0;

      /* Recuperation des participations de couleurs pour le pixel courant */
      tmpi =	(int) (( ((float)i)/terraintxt->h) * image->h);
      tmpj =	(int) (( ((float)j)/terraintxt->w) * image->w);

      RemplitPerc(perc, ((unsigned char*) image->pixels)[tmpi*image->w*3 + tmpj*3]);

      /* On recupere les couleurs */
      tmpi =	i%prairies->h;
      tmpj =	j%prairies->w;

      b = perc[0] * GetPixelColor(prairies,tmpi,tmpj,0);
      g = perc[0] * GetPixelColor(prairies,tmpi,tmpj,1);
      r = perc[0] * GetPixelColor(prairies,tmpi,tmpj,2);

      tmpi =	i%rocheuses->h;
      tmpj =	j%rocheuses->w;

      b += perc[1] * GetPixelColor(rocheuses,tmpi,tmpj,0);
      g += perc[1] * GetPixelColor(rocheuses,tmpi,tmpj,1);
      r += perc[1] * GetPixelColor(rocheuses,tmpi,tmpj,2);

      tmpi =	i%neige->h;
      tmpj =	j%neige->w;

      b += perc[2] * GetPixelColor(neige,tmpi,tmpj,0);
      g += perc[2] * GetPixelColor(neige,tmpi,tmpj,1);
      r += perc[2] * GetPixelColor(neige,tmpi,tmpj,2);

      /* Calcul d'ombre */
      b *= diff*lum_;
      g *= diff*lum_;
      r *= diff*lum_;

      if (b > 255)
	b = 255;
      if (g > 255)
	g = 255;
      if (r > 255)
	r = 255;

      /* On inverse pour avoir la couleur dans le bon sens */
      ((unsigned char*) terraintxt->pixels)[i*3*terraintxt->w + j*3] = (unsigned char) r;
      ((unsigned char*) terraintxt->pixels)[i*3*terraintxt->w + j*3+1] = (unsigned char) g;
      ((unsigned char*) terraintxt->pixels)[i*3*terraintxt->w + j*3+2] = (unsigned char) b;
    }

  /* On passe la texture a OpenGL */
  glGenTextures(1,&(res->tex));
  glBindTexture(GL_TEXTURE_2D,res->tex);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
  gluBuild2DMipmaps(GL_TEXTURE_2D, 3, terraintxt->w, terraintxt->h, GL_RGB, GL_UNSIGNED_BYTE, terraintxt->pixels);

  /* Désallocation */
  for (int i = 0; i < terraintxt->h; i++)
    free(lightmap[i]);
  free(lightmap);

  FreeTextures(prairies, rocheuses, image, neige, terraintxt, NULL);

  /* Water loading */
  water = SDL_LoadBMP("data/water.bmp");
  if (water)
  {
    glGenTextures(1,&(res->watertxt));
    glBindTexture(GL_TEXTURE_2D,res->watertxt);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, water->w, water->h, GL_RGB, GL_UNSIGNED_BYTE, water->pixels);
  }
  SDL_FreeSurface(water);

  return res;
}

void
Terrain::makeTerrain()
{
  graphical_env_->logger("Terrain : Terrain Making", 1);
  srand(6);

  Destruction();

  perlin_->set_layer_size(NB_PTS_LAYER);
  perlin_->set_out_pic_name("perlin_final.bmp");

  perlin_->process_perlin();
  graphical_env_->logger("Perlin : Processed", 1);

  terrain_ = Init(SDL_LoadBMP("perlin_liss.bmp"));
  if (!terrain_)
    exit (1);
  //terrain_ = Init(SDL_LoadBMP("perlin_final.bmp"));

  re_process_ = 0;
}

void
Terrain::display_terrain()
{

  if (re_process_)
  {
    makeTerrain();
    return ;
  }

  if (is_wired_)
    draw3D();
  if (is_displayed_)
    drawFull3D();

  if (is_watered_)
  {
    glEnable(GL_TEXTURE_2D);
    glColor4f(0.0,0.2,1.0f,0.8f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    glBindTexture(GL_TEXTURE_2D,terrain_->watertxt);
    glBegin(GL_QUADS);
    glTexCoord2i(0,5);
    glVertex3d(-2*MAX_POINTS,2*MAX_POINTS,water_level_);
    glTexCoord2i(5,5);
    glVertex3d(2*MAX_POINTS,2*MAX_POINTS,water_level_);
    glTexCoord2i(5,0);
    glVertex3d(2*MAX_POINTS,-2*MAX_POINTS,water_level_);
    glTexCoord2i(0,0);
    glVertex3d(-2*MAX_POINTS,-2*MAX_POINTS,water_level_);
    glEnd();
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
  }
}
