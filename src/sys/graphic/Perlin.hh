#ifndef PERLIN_HH_
# define PERLIN_HH_

# include "../includes/graphic.hh"
# include "../includes/Algebre.hh"
# include "../includes/GraphEnv.hh"

typedef struct layer
{
  int **v;
  int size;
  float persistance;
} s_layer;

class Perlin
{
public:
  Perlin(GraphEnv* graphical_env);
  ~Perlin(){};

  /*!
  ** \brief Initialisazion of the layer
  **
  ** @param int Size
  ** @param float Persistance
  **
  ** @return The initilialized layer
  */
  s_layer* init_layer(int, float);
  void free_layer(s_layer*);

  /*!
  ** \brief Generate the layer and 3 BMP for the process
  **
  */
  void generate_layer();
  int interpolate(int, int, int, int);
  int interpolate_value(int, int, int, s_layer*);

  /*!
  ** \brief Launch the Perlin processing
  **
  ** @return 0 if the function failed else 1
  */
  int process_perlin(void);

  void set_layer_size(int size);
  void set_out_pic_name(std::string filename);

  s_layer *get_layer();
  void     set_layer(s_layer *layer);

  unsigned char rand_range(int a);
  void ColoratePixels(SDL_Surface* s, int x, int y, Uint32 coul);

private:

  /*!
  ** \brief Save a layer to a BMP file
  **
  ** @param c The layer
  ** @param filename The path of the BMP
  */
  void save_bmp(s_layer *c, const std::string filename);

  int octaves_;
  int frequence_;
  float persistence_;
  int output_size_;
  int lissage_;

  s_layer *layer_;

  std::string output_picture_;
  std::string output_picture_random_;
  std::string output_picture_liss_;

  GraphEnv* graphical_env_;
};


#endif /* !PERLIN_HH_ */
