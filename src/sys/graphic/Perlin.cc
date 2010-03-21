#include "Perlin.hh"
#include "../includes/random.hh"

unsigned char
Perlin::rand_range(int a)
{
  return sys_rand() % a;
}

Perlin::Perlin(GraphEnv* graphical_env)
{
  graphical_env_ = graphical_env;
  graphical_env_->logger("Perlin Init");

  octaves_ = 3;
  frequence_ = 4;
  persistence_ = .5;
  output_size_ = 200;
  lissage_ = 20; //3

//   octaves_ = 2;
//   frequence_ = 4;
//   persistence_ = .2;
//   output_size_ = 512;
//   lissage_ = 3;

  layer_ = NULL;

  output_picture_ = "perlin_final.bmp";
  output_picture_random_ = "perlin_rand.bmp";
  output_picture_liss_ = "perlin_liss.bmp";
}

void
Perlin::ColoratePixels(SDL_Surface* s, int x, int y, Uint32 color)
{
  *((Uint32*)(s->pixels) + x + y * s->w) = color;
}

s_layer*
Perlin::init_layer(int t, float p)
{
  s_layer *layer = NULL;

  if (!(layer = (s_layer*)malloc(sizeof(s_layer))))
    return NULL;

  if (!(layer->v = (int **)malloc(t*sizeof(int*))))
    return NULL;

  // FIXME
  // Use memcpy here !
  for (int i = 0; i < t ; i++)
  {
    if (!(layer->v[i] = (int *)malloc(t*sizeof(int))))
      return NULL;

    for (int j = 0; j < t; j++)
      layer->v[i][j] = 0;
  }

  layer->size = t;
  layer->persistance = p;

  return layer;
}

void
Perlin::save_bmp(s_layer *c, const std::string filename)
{
  SDL_Surface *s = SDL_CreateRGBSurface(SDL_SWSURFACE,c->size, c->size, 32,0, 0, 0, 0);
  Uint32 u;
  SDL_PixelFormat *fmt = NULL;

  if (!s)
    exit(1);

  fmt = s->format;

  for (int i = 0; i < c->size; i++)
    for (int j = 0; j < c->size; j++)
    {
      u = SDL_MapRGB (fmt, (char)c->v[i][j], (char)c->v[i][j], (char)c->v[i][j]);
      ColoratePixels(s, i, j, u);
    }

  SDL_SaveBMP(s, filename.c_str());
  SDL_FreeSurface(s);
}

int
Perlin::process_perlin(void)
{
  // FIXME
  // Remove after improving of random
  srand((int)time(NULL));
  srand(6);

  layer_ = init_layer(output_size_, 1);
  if (!layer_)
    return 0;

  generate_layer();

  save_bmp(layer_, output_picture_);

  return 1;
}

void
Perlin::free_layer(s_layer* s)
{
  for (int j = 0; j < s->size; j++)
    free(s->v[j]);

  free(s->v);
}

void
Perlin::generate_layer()
{
  int size = layer_->size;

  /* Random Layer */
  s_layer *random = init_layer(size, 1);

  // FIXME : Try to improve the picture randomization by dumping memory
  for (int i = 0; i < size; i++)
    for (int j = 0; j < size; j++)
      random->v[i][j] = rand_range(256);

  save_bmp(random, output_picture_random_.c_str());


  /* Working Layers */
  float persistance_courante = persistence_;
  s_layer **working_layers = (s_layer **)malloc(octaves_ * sizeof(s_layer*));
  for (int i = 0; i < octaves_; i++)
  {
    working_layers[i] = init_layer(size, persistance_courante);
    if (!working_layers[i])
      return;
    persistance_courante *= persistence_;
  }

  int current_freq = frequence_;
  // Fill all the layers
  for (int n = 0; n < octaves_; n++)
  {
    for(int i = 0; i < size; i++)
      for(int j = 0; j < size; j++)
	working_layers[n]->v[i][j] = interpolate_value(i, j, current_freq, random);
    current_freq *= frequence_;
  }

  float sum_persistances = 0;
  for (int i = 0; i < octaves_; i++)
    sum_persistances += working_layers[i]->persistance;

  // Successive add of layers
  for (int i = 0; i < size; i++)
    for (int j = 0; j < size; j++)
    {
      for (int n = 0; n < octaves_; n++)
	layer_->v[i][j] += working_layers[n]->v[i][j] * working_layers[n]->persistance;

      // normalize
      layer_->v[i][j] = layer_->v[i][j] / sum_persistances;
    }


  /* Lissage */
  s_layer *lissage = init_layer(size, 0);
  if(!lissage)
    return;

  for (int x = 0; x < size; x++)
    for (int y = 0; y < size; y++)
    {
      int a = 0;
      int n = 0;
      for (int k = x - lissage_; k <= x + lissage_; k++)
	for (int l = y - lissage_; l <= y + lissage_; l++)
	  if ((((unsigned) (k)) < size) && (((unsigned) (l)) < size))
	  {
	    n++;
	    a += layer_->v[k][l];
	  }
      lissage->v[x][y] = (float)a/n;
    }

  save_bmp(lissage, output_picture_liss_.c_str());

  // Federation of Free Memory !
  free_layer(random);
  free_layer(lissage);
  for (int i = 0; i < octaves_; i++)
    free_layer(working_layers[i]);
  free(working_layers);
}

int
Perlin::interpolate(int y1, int y2, int n, int delta)
{
  // Not linear interpolations
  if (n == 0)
    return y1;
  if (n == 1)
    return y2;

  float a = (float)delta / n;

  float fac1 = 3*pow(1-a, 2) - 2*pow(1-a, 3);
  float fac2 = 3*pow(a  , 2) - 2*pow(a  , 3);

  return y1*fac1 + y2*fac2;

  // Linear interpolations
  //////////////////////////////////////////////
  // TO TRY !
//   if (n!=0)
//     return y1+delta*((float)y2-(float)y1)/(float)n;
//     else
//     return y1;
}

int
Perlin::interpolate_value(int i, int j, int frequence, s_layer *r)
{
  float pas = (float)r->size / frequence;
  int	q = (float)i / pas;
  int	borne1x =   q   * pas;
  int	borne2x = (q+1) * pas;

  if (borne2x >= r->size)
    borne2x = r->size - 1;

  q = (float)j / pas;
  int borne1y =   q   * pas;
  int borne2y = (q+1) * pas;

  if (borne2y >= r->size)
    borne2y = r->size - 1;

  int b00 = r->v[borne1x][borne1y];
  int b01 = r->v[borne1x][borne2y];
  int b10 = r->v[borne2x][borne1y];
  int b11 = r->v[borne2x][borne2y];

  int v1  = interpolate(b00, b01, borne2y-borne1y, j-borne1y);
  int v2  = interpolate(b10, b11, borne2y-borne1y, j-borne1y);
  int fin = interpolate(v1, v2,   borne2x-borne1x, i-borne1x);

  return fin;
}

// FIXME
// MAKES NO SENS !
void
Perlin::set_layer_size(int size)
{
  output_size_ = size;
}

// FIXME
// MAKES NO SENS !
void
Perlin::set_out_pic_name(std::string filename)
{
  output_picture_ = filename;
}

s_layer*
Perlin::get_layer()
{
  return layer_;
}

void
Perlin::set_layer(s_layer *layer)
{
  layer_ = layer;
}
