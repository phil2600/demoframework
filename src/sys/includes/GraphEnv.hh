/*!
** @file   GraphEnv.hh
** @author Philippe Faure <faure.phil@gmail.com>
** @date   Mon Mar 15 21:38:51 2010
**
** @brief Graphical Environment Management
**
**
*/

#ifndef GRAPHENV_HH_
# define GRAPHENV_HH_

# include "Algebre.hh"
# include "Camera.hh"

class GraphEnv
{
public:
  GraphEnv(int width, int height, int bpp, char fullscreen);
  ~GraphEnv();

  /// Display configuration routines.
  /// \{
  /// \brief Getter & Setter for display configuration
  int  get_bpp();
  char get_fullscreen();
  int  get_height();
  int  get_width();
  void set_bpp(int bpp);
  void set_fullscreen(char fullscreen);
  void set_height(int height);
  void set_width(int width);
  /// \}


  /*!
  ** \brief Initialize SDL/OpenGL
  */
  void init_GL();
  /*!
  ** \brief Rescale the OpenGL window
  */
  void reshape(int width, int height);

  /*!
  ** \brief Permits to pass on Orthogonal view
  */
  void orthoOn(float xres = 640, float yres = 480);
  /*!
  ** \brief Permits to pass on Projection view
  */
  void orthoOff(void);

  void cameraFovLH(float fov, float aspect, float fNear, float fFar);
  void cameraLookAtLH(CPoint camPosition, CPoint camTarget, CPoint camUp);

  void     loadMatrix(CMatrix m);
  CMatrix  getProjectionMatrix();
  CMatrix  getModelViewMatrix();

  void drawAxis3D();

  Camera*  getActiveCamera();
  void     setActiveCamera(Camera *activeCamera);

  void indent_log(int indent);
  void print_indent();
  void logger(std::string str, unsigned int indent = 0);

private:
  int  bpp_;
  bool fullscreen_;
  int  height_;
  int  width_;

  Camera dummyCamera_;
  Camera *activeCamera_;

  char is_logging_;
  unsigned int log_indent_;
  float start_time_;
};

#endif /* !GRAPHENV_HH_ */
