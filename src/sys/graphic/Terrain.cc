#include "Terrain.hh"

Perlin perlin(NULL);

/* Fonction d'initialisation */
STerrain* Terrain_Init(SDL_Surface*);

//SParam param;
STerrain *terrain;

/* Structure pour les parametres et leur valeurs par defauts */
SParam param = {0,1,1,0,10.0,1,-65.0,0.0,-6.0,1.0,0,3.0,
		L_AUCUN,-1,0,-0.02,0.2,1.0,0.01,47.0,
		4,2,.2, NULL};


/* Dessiner en 3D filaire */
void Graphique_Dessin3D()
{
  int i,j;

  double ajout = 40.0 / MAX_POINTS,
    posi,posj;

  /* On tracera uniquement les contours des polygones */
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  /* Contour vert */
  glColor3f(0.0,1.0,0.0);

  /* Dessin des triangles :
   *
   * - On dessine les contours en vert
   * - On dessine l'interieur en noir pour avoir un joli rendu
   */

  glBegin(GL_TRIANGLES);
  for(i=0,posi=-20 ;i<MAX_POINTS-1;i++, posi += ajout)
  {
    for(j=0, posj=-20;j<MAX_POINTS-1;j++, posj += ajout)
    {
      glVertex3d(posi,posj,terrain->hauteur[i][j]);
      glVertex3d(posi,posj+ajout,terrain->hauteur[i][j+1]);
      glVertex3d(posi+ajout,posj+ajout,terrain->hauteur[i+1][j+1]);

      glVertex3d(posi,posj,terrain->hauteur[i][j]);
      glVertex3d(posi+ajout,posj+ajout,terrain->hauteur[i+1][j+1]);
      glVertex3d(posi+ajout,posj,terrain->hauteur[i+1][j]);
    }
  }
  glEnd();

  /* Fond noir */
  glColor3f(0.0,0.0,0.0);

  /* On tracera des polygones "pleins" */
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  /* On décalera les polygones vers l'arrière */
  glEnable(GL_POLYGON_OFFSET_FILL);
  /* Coefficients du décalage */
  glPolygonOffset(1.0,1.0);

  /* On va tracer des triangles */
  glBegin(GL_TRIANGLES);
  for(i=0,posi=-20 ;i<MAX_POINTS-1;i++, posi += ajout)
  {
    for(j=0, posj=-20;j<MAX_POINTS-1;j++, posj += ajout)
    {
      glVertex3d(posi,posj,terrain->hauteur[i][j]);
      glVertex3d(posi,posj+ajout,terrain->hauteur[i][j+1]);
      glVertex3d(posi+ajout,posj+ajout,terrain->hauteur[i+1][j+1]);

      glVertex3d(posi,posj,terrain->hauteur[i][j]);
      glVertex3d(posi+ajout,posj+ajout,terrain->hauteur[i+1][j+1]);
      glVertex3d(posi+ajout,posj,terrain->hauteur[i+1][j]);
    }
  }
  glEnd();
  /* On ne décale plus */
  glDisable(GL_POLYGON_OFFSET_FILL);
}

/* Dessin en 3D plein */
void Graphique_Dessin3DFull()
{
  int i,j;
  double ajout = 40.0 / MAX_POINTS,posi,posj;

  /* On tracera des polygones "pleins" */
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  /* On va utiliser des textures */
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D,terrain->tex);

  glColor3f(11.0f,11.0f,11.0f);

  /* On va tracer des triangles */
  glBegin(GL_TRIANGLES);
  for(i=0,posi=-20 ;i<MAX_POINTS-1;i++, posi += ajout)
  {
    for(j=0, posj=-20;j<MAX_POINTS-1;j++, posj += ajout)
    {
      glTexCoord2f( ((float) j)/MAX_POINTS, ((float) i)/MAX_POINTS);
      glVertex3d(posi,posj,terrain->hauteur[i][j]);

      glTexCoord2f( ((float) j+1)/MAX_POINTS, ((float) i)/MAX_POINTS);
      glVertex3d(posi,posj+ajout,terrain->hauteur[i][j+1]);

      glTexCoord2f( ((float) j+1)/MAX_POINTS, ((float) i+1)/MAX_POINTS);
      glVertex3d(posi+ajout,posj+ajout,terrain->hauteur[i+1][j+1]);

      glTexCoord2f( ((float) j)/MAX_POINTS, ((float) i)/MAX_POINTS);
      glVertex3d(posi,posj,terrain->hauteur[i][j]);

      glTexCoord2f( ((float) j+1)/MAX_POINTS, ((float) i+1)/MAX_POINTS);
      glVertex3d(posi+ajout,posj+ajout,terrain->hauteur[i+1][j+1]);

      glTexCoord2f( ((float) j)/MAX_POINTS, ((float) i+1)/MAX_POINTS);
      glVertex3d(posi+ajout,posj,terrain->hauteur[i+1][j]);
    }
  }
  glEnd();

  /* On ne va pas utiliser des textures */
  glDisable(GL_TEXTURE_2D);
}

unsigned char Terrain_GetPixelColor(SDL_Surface *image, int i, int j,int k)
{
  return ((unsigned char*)image->pixels)[i*image->w*3+j*3+k];
}

/*
 * Recuperation de la hauteur d'un point
 * Puisque notre texture generee n'est pas forcement de la meme taille que l'image de niveaux,
 * nous calculons un ratio entre [0,1] ensuite on remultiplie par la hauteur et la largeur de l'image
 */
double Terrain_GetHauteur(SDL_Surface *image, int i, int j, int maxi, int maxj)
{
  double di,dj;
  double res;
  di = i;
  dj = j;

  di /= maxi;
  dj /= maxj;

  di *= image->h;
  dj *= image->w;

  i = di;
  j = dj;

  res = (Terrain_GetPixelColor(image,i,j,0)/256.0)*param.hautmax;
  return res;
}

/* Cette fonction remplit le pourcentage que ce pixel doit avoir dependant de l'hauteur associe
 * Si l'hauteur est faible, c'est que de l'herbe
 * Sinon c'est un melange herbe - roche
 * Plus haut, c'est que de la roche
 * Ensuite un melange roche - neige
 * Et enfin, c'est que de la neige
 */
void Terrain_RemplitPerc(float *perc, unsigned char haut)
{
  /* On utilise la fonction rand pour mettre de l'aleatoire */
  int add = haut + (rand()%30)-15;

  if(add<0)
    add = 0;

  if(add>255)
    add = 255;

  haut = add;

  /* Que de la prairie */
  if(haut<60)
  {
    perc[0] = 1.0f;
    perc[1] = 0.0f;
    perc[2] = 0.0f;
  }
  /* Melange entre prairie et roche */
  else if(haut<130)
  {
    perc[0] = 1.0f - (haut-60.0f)/70.0f;
    perc[1] = (haut-60.0f)/70.0f;
    perc[2] = 0.0f;
  }
  /* Que de la roche */
  else if(haut<180)
  {
    perc[0] = 0.0f;
    perc[1] = 1.0f;
    perc[2] = 0.0f;
  }
  /* Melange entre roche et la neige */
  else if(haut<220)
  {
    perc[0] = 0.0f;
    perc[1] = 1.0f - (haut-180.0f)/40.0f;
    perc[2] = (haut-180.0f)/40.0f;
  }
  /* Que de la neige */
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
int Terrain_AppliquePatch(double **lightmap, int maxi, int maxj, int taille_patch)
{
  int i,j,k,l,cnt;
  double **tmplightmap;

  /* Allocation du tableau temporaire, fait en dynamique puisque le static peut planter :
   * taille du tableau si grand...
   */

  tmplightmap = (double **)malloc(maxi*sizeof(double*));
  if(tmplightmap==NULL)
    return 1;

  for(i=0;i<maxi;i++)
  {
    tmplightmap[i] = (double*)malloc(maxj*sizeof(double));
    if(tmplightmap[i]==NULL)
    {
      i--;
      while(i>=0)
      {
	free(tmplightmap[i]);
      }
      free(tmplightmap);
      return 1;
    }
  }

  /* Calcul de moyenne */
  for(i=0;i<maxi;i++)
  {
    for(j=0;j<maxj;j++)
    {
      /* Valeur par défaut */
      tmplightmap[i][j] = 0.0;

      /* Compteur pour savoir combien d'éléments ont été sommés */
      cnt = 0;

      for(k=i-taille_patch;k<=i+taille_patch;k++)
      {
	for(l=j-taille_patch;l<=j+taille_patch;l++)
	{
	  /* Si les coordonnées sont bons */
	  if((k>=0)&&(l>=0)&&(k<maxi)&&(l<maxj))
	  {
	    tmplightmap[i][j] += lightmap[k][l];
	    cnt++;
	  }
	}
      }

      /* Calcul de la moyenne */
      if(cnt)
      {
	tmplightmap[i][j] /= cnt;
      }
      else
	tmplightmap[i][j] = 1.0f;

    }
  }

  /* Recopie */
  for(i=0;i<maxi;i++)
  {
    for(j=0;j<maxj;j++)
    {
      lightmap[i][j] = tmplightmap[i][j];
    }
  }

  return 0;
}

/*
 * Fonction qui calcule le coefficient de luminosite
 *
 * Version la plus simple, on regarde localement si, dans la direction de la source de lumiere,
 * le terrain n'est pas plus haut. Si c'est le cas, alors cet endroit sera moins eclaire
 */

double Terrain_calcLightMap_Simple(SDL_Surface *image, int i, int j, int maxi, int maxj)
{
  if( (i-param.l_veci>=0)&&(i-param.l_veci<maxi) && (j-param.l_vecj>=0) &&(j-param.l_vecj<maxj) )
    return 1.0 - (Terrain_GetHauteur(image, i-param.l_veci, j-param.l_vecj, maxi, maxj)
		  - Terrain_GetHauteur(image, i, j, maxi, maxj))/param.l_adouc;
  else
    return 1.0f;
}

/*
 * Fonction qui calcule le coefficient de luminosite
 *
 * On utilise le vecteur normal et un produit scalaire. Nettement plus mathematique et precis que la
 * version de Terrain_calcLightMap_Simple.
 */

double Terrain_calcLightMap_LDOTN(SDL_Surface *image, int i, int j, int maxi, int maxj)
{
  CPoint v1,v2,light,n;
  double tmp;

  /* On doit récupérer la normale de ce point. Le plus simple est de définir deux vecteurs.
   * On va prendre les hauteurs de trois coins pour le faire
   */
  light.x = param.l_veci;
  light.y = param.l_vecj;
  light.z = param.l_vecz;
  light.normalize();
  //  Vecteur_Normalise(&light);

  if((i>0)&&(j>0)&&(i<maxi-1)&&(j<maxj-1))
  {
    /* Complétons ces vecteurs
     * Premier vecteur sera le vecteur (i-1,j-1) vers (i+1,j-1)
     * Deuxième vecteur sera le vecteur (i+1,j-1) vers (i+1,j+1)
     */
    v1.x = 2; v1.y = 0;
    v1.z = Terrain_GetHauteur(image,i+1,j-1, maxi, maxj) - Terrain_GetHauteur(image,i-1,j-1, maxi, maxj);

    v2.x = 0; v2.y = 2;
    v2.z = Terrain_GetHauteur(image,i+1,j+1, maxi, maxj) - Terrain_GetHauteur(image,i+1,j-1, maxi, maxj);

    /* Normalise */
    v1.normalize();
    v2.normalize();
    //     Vecteur_Normalise(&v1);
    //     Vecteur_Normalise(&v2);

    // Normal
    n = v1 ^ v2;
    // Normalize the Normal
    n.normalize();
    // Vecteur_Normalise(&n);
    /* On vérifie que le vecteur est dans le bon sens */
    if(n.z<0)
    {
      n.x *=-1;
      n.y *=-1;
      n.z *=-1;
    }

    /* On a la normale, on calcule maintenant L dot N */
    tmp = light.ScalarProduct(n);

    if(tmp < 0)
      return -param.l_ldotnmultiple*tmp;
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
double Terrain_calcLightMap_Ray(SDL_Surface *image, int i, int j, int maxi, int maxj, CPoint *light)
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
  cur.z = Terrain_GetHauteur(image,i,j,maxi,maxj);

  /* Tant qu'on est dans le terrain */
  while( (cur.x>=0) && (cur.y>=0) && (cur.x<image->h) && (cur.y<image->w) )
  {
    /* On recupere la hauteur courante */
    tmp = Terrain_GetHauteur(image,(int) cur.x, (int) cur.y, image->h, image->w);

    /* Si c'est au-dessus du maximum possible du terrain */
    if(cur.z>param.hautmax)
    {
      /* On sort, pas d'intersection possible */
      break;
    }

    /* Si le terrain est au-dessus du vecteur de lumiere, le terrain cache la lumiere */
    if(tmp > cur.z)
    {
      return 0.0;
    }

    /* Sinon, on continue avec le vecteur de lumiere */
    cur.x += light->x;
    cur.y += light->y;
    cur.z += light->z;
  }

  /* Sinon pas d'intersection, on calcule la luminosite avec le calcul L dot N */
  return Terrain_calcLightMap_LDOTN(image,i,j,maxi,maxj);
}

/*
 * Fonction qui calcule les coefficients de luminosite de tout le terrain et remplit le tableau lightmap
 * Dependant de la valeur de param.light, cette fonction decidera quel algorithme utilise
 */

void Terrain_calcLightMap(SDL_Surface *image, double **lightmap, int maxi, int maxj)
{
  int i,j;
  CPoint light;

  light.x = -param.l_veci;
  light.y = -param.l_vecj;
  light.z = -param.l_vecz;
  light.normalize();
  //   Vecteur_Normalise(&light);

  /*
   * Calcul du vecteur lumiere normalise, donc le vecteur qui va vers la source de lumiere
   * Ceci sert pour le calcul d'ombre
   */
  light.x = -param.l_veci;
  light.y = -param.l_vecj;
  light.z = -param.l_vecz;
  light.normalize();
  //   Vecteur_Normalise(&light);

  /* Première passe */
  for(i=0;i<maxi;i++)
  {
    for(j=0;j<maxj;j++)
    {
      switch(param.light)
      {
	case L_PATCH:
	case L_SIMPLE:
	  lightmap[i][j] = Terrain_calcLightMap_Simple(image,i,j,maxi,maxj);
	  break;
	case L_LDOTN1:
	case L_LDOTN4:
	  lightmap[i][j] = Terrain_calcLightMap_LDOTN(image,i,j,maxi,maxj);
	  break;
	case L_RAYSIMPLE:
	case L_RAYPATCH:
	  lightmap[i][j] = Terrain_calcLightMap_Ray(image,i,j,maxi,maxj,&light);
	  break;
	default:
	  lightmap[i][j] = 1.0f;
	  break;
      }
    }
  }

  /* Deuxième passe (possible) */
  switch(param.light)
  {
    case L_PATCH:
    case L_RAYPATCH:
    case L_LDOTN4:
      /* On applique un lissage 4*4 sur le lightmap */
      Terrain_AppliquePatch(lightmap,maxi,maxj,4);
      break;
    case L_LDOTN1:
      /* On applique un lissage 1*1 sur le lightmap */
      Terrain_AppliquePatch(lightmap,maxi,maxj,1);
      break;
    default: /* On ne fait rien */
      break;
  }


}

/* Detruire le terrain */
void Terrain_Destruction(STerrain *terrain)
{
  if(terrain)
  {
    /* Destruction des textures OpenGL */
    glDeleteTextures(1,&terrain->tex);
    glDeleteTextures(1,&terrain->watertxt);
    free(terrain);
  }
}

/* Initialiser le terrain */
STerrain* Terrain_Init(SDL_Surface *image)
{
  SDL_Surface *terraintxt,
    *prairies,
    *rocheuses,
    *neige,
    *water;
  int i,j,tmpi,tmpj;
  double diff;
  double r,g,b;
  STerrain* res;
  float perc[3];

  /* Pour calculer le temps de generation */
  int start = time(NULL),end;

  /* Affichage dans la console */
  printf("Initialisation avec lumière: %d\n",param.light);

  if(image==NULL)
    return NULL;

  res = (STerrain*)malloc(sizeof *res);

  if(res==NULL)
  {
    printf("Malloc du terrain implicite a echoue\n");
    return NULL;
  }

  printf("Getting points\n");
  for(i=0;i<MAX_POINTS;i++)
  {
    for(j=0;j<MAX_POINTS;j++)
    {
      res->hauteur[i][j] = Terrain_GetHauteur(image,i,j, MAX_POINTS, MAX_POINTS);
    }
  }

  printf("Generating terrain texture\n");

  /*
   * On a trois textures, celle des prairies, rocheuses, neige
   */
  prairies = SDL_LoadBMP("data/grassm.bmp");
  if(prairies==NULL)
  {
    printf("Error avec texture prairie\n");
    free(res);
    return NULL;
  }

  rocheuses = SDL_LoadBMP("data/rock.bmp");
  if(rocheuses==NULL)
  {
    printf("Error avec texture rocheuse\n");
    SDL_FreeSurface(prairies);
    free(res);
    return NULL;
  }

  neige = SDL_LoadBMP("data/snow.bmp");

  if(neige==NULL)
  {
    printf("Error avec texture neige\n");
    SDL_FreeSurface(prairies);
    SDL_FreeSurface(rocheuses);
    SDL_FreeSurface(image);
    free(res);
    return NULL;
  }

  /* Creer une surface pour la texture, pour la passer a OpenGL */
  terraintxt = SDL_CreateRGBSurface(SDL_HWSURFACE, 1024, 1024, 32, 8, 8, 8, 0);
  if(terraintxt==NULL)
  {
    printf("Error avec texture terrain\n");
    SDL_FreeSurface(prairies);
    SDL_FreeSurface(rocheuses);
    SDL_FreeSurface(neige);
    SDL_FreeSurface(image);
    free(res);
    return NULL;
  }


  /* Allouer un tableau pour les coefficients de luminosite */
  double **lightmap = (double**)malloc(sizeof(double)*terraintxt->h);
  if(lightmap==NULL)
  {
    printf("Error avec l'allocation de la lightmap\n");
    SDL_FreeSurface(prairies);
    SDL_FreeSurface(rocheuses);
    SDL_FreeSurface(neige);
    SDL_FreeSurface(terraintxt);
    SDL_FreeSurface(image);
    free(res);
    return NULL;
  }

  /* Allocation de la 2eme dimension */
  for(i=0;i<terraintxt->h;i++)
  {
    lightmap[i] = (double*)malloc(sizeof(double)*terraintxt->w);
    if(lightmap[i]==NULL)
    {
      SDL_FreeSurface(prairies);
      SDL_FreeSurface(rocheuses);
      SDL_FreeSurface(neige);
      SDL_FreeSurface(terraintxt);
      SDL_FreeSurface(image);
      free(res);
      while(i>=0)
      {
	free(lightmap[i]);
	i--;
      }
      free(lightmap);
      return NULL;
    }
  }

  /* On calcule le lightmap */
  Terrain_calcLightMap(image,lightmap,terraintxt->h,terraintxt->w);

  /*Pour chaque pixel, on calcule la composition des 3 textures*/
  for(i=0;i<terraintxt->h;i++)
    for(j=0;j<terraintxt->w;j++)
    {
      /* Recuperation du coefficient de luminosite */
      diff = lightmap[i][j];

      /* Verification des seuils de luminosite */
      if(diff<param.l_min)
	diff =param.l_min;
      else if(diff>param.l_max)
	diff = 1.0;

      /* Recuperation des participations de couleurs pour le pixel courant */
      tmpi =	(int ) (( ((float) i)/terraintxt->h) * image->h) ;
      tmpj =	(int ) (( ((float) j)/terraintxt->w) * image->w) ;
      Terrain_RemplitPerc(perc,((unsigned char*) image->pixels)[tmpi*image->w*3 + tmpj*3]);


      /* On recupere les couleurs */
      tmpi =	i%prairies->h;
      tmpj =	j%prairies->w;

      b = perc[0] * Terrain_GetPixelColor(prairies,tmpi,tmpj,0);
      g = perc[0] * Terrain_GetPixelColor(prairies,tmpi,tmpj,1);
      r = perc[0] * Terrain_GetPixelColor(prairies,tmpi,tmpj,2);

      tmpi =	i%rocheuses->h;
      tmpj =	j%rocheuses->w;

      b += perc[1] * Terrain_GetPixelColor(rocheuses,tmpi,tmpj,0);
      g += perc[1] * Terrain_GetPixelColor(rocheuses,tmpi,tmpj,1);
      r += perc[1] * Terrain_GetPixelColor(rocheuses,tmpi,tmpj,2);

      tmpi =	i%neige->h;
      tmpj =	j%neige->w;

      b += perc[2] * Terrain_GetPixelColor(neige,tmpi,tmpj,0);
      g += perc[2] * Terrain_GetPixelColor(neige,tmpi,tmpj,1);
      r += perc[2] * Terrain_GetPixelColor(neige,tmpi,tmpj,2);

      /* Calcul d'ombre */
      b *= diff*param.lum;
      g *= diff*param.lum;
      r *= diff*param.lum;

      if(b>255)
	b=255;
      if(g>255)
	g=255;
      if(r>255)
	r=255;

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
  for(i=0;i<terraintxt->h;i++)
  {
    free(lightmap[i]);
  }
  free(lightmap);

  SDL_FreeSurface(terraintxt);
  SDL_FreeSurface(image);
  SDL_FreeSurface(prairies);
  SDL_FreeSurface(rocheuses);
  SDL_FreeSurface(neige);

  /* Afficage du temps de generation */
  end = time(NULL);
  printf("Temps pour la generation : %d\n",end-start);

  /* Avant de retourner la main, on va charger l'eau */

  water = SDL_LoadBMP("data/water.bmp");
  if(water)
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

STerrain *makeTerrain(STerrain *terrain)
{
  //   SDL_WM_SetCaption("Création du terrain, patientez...",NULL);
  //   Terrain_Destruction(terrain);
  //  GenererLayerPerlin(param.frequence, param.octaves, param.persistance, param.random);
  //  GenererLayerPerlin(param.frequence, param.octaves, param.persistance, param.random);
  perlin.set_layer_size(NB_PTS_LAYER);
  perlin.set_out_pic_name(IMAGE_FILE);

  perlin.set_layer(perlin.init_layer(NB_PTS_LAYER, 1));
  perlin.generate_layer();

  param.random = perlin.get_layer();
  terrain = Terrain_Init(SDL_LoadBMP(IMAGE_FILE));
  //   SDL_WM_SetCaption("Affichage du terrain",NULL);
  return terrain;
}

s_layer* InitLayerAlea(){
  s_layer *c = perlin.init_layer(NB_PTS_LAYER, 1);
  if (!c)
    return NULL;
  int i,j;
  for (i=0; i<c->size; i++)
    for (j=0; j<c->size; j++)
      c->v[i][j]=perlin.rand_range(256);

  return c;
}


void myterrain()
{
  static int i = 0;
  if (i == 0)
  {
    srand(6);

    perlin.set_layer_size(NB_PTS_LAYER);
    perlin.set_out_pic_name(IMAGE_FILE);

    perlin.set_layer(perlin.init_layer(NB_PTS_LAYER, 1));
    perlin.generate_layer();

    param.random = perlin.get_layer();
    //    terrain = Terrain_Init(SDL_LoadBMP(IMAGE_FILE));
    terrain = Terrain_Init(SDL_LoadBMP("perlin_liss.bmp"));

    i++;
  }

  if(false)
    Graphique_Dessin3D();
  else
  {
    Graphique_Dessin3DFull();
    /* On va dessiner une texture */
    glEnable(GL_TEXTURE_2D);
    glColor4f(0.0,0.2,1.0f,0.8f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    glBindTexture(GL_TEXTURE_2D,terrain->watertxt);
    glBegin(GL_QUADS);
    glTexCoord2i(0,5);
    glVertex3d(-2*MAX_POINTS,2*MAX_POINTS,param.w_cur);
    glTexCoord2i(5,5);
    glVertex3d(2*MAX_POINTS,2*MAX_POINTS,param.w_cur);
    glTexCoord2i(5,0);
    glVertex3d(2*MAX_POINTS,-2*MAX_POINTS,param.w_cur);
    glTexCoord2i(0,0);
    glVertex3d(-2*MAX_POINTS,-2*MAX_POINTS,param.w_cur);
    glEnd();
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
  }
}
