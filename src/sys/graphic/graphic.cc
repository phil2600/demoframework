#include "../lib.hh"

void draw_repere(unsigned int scale)
{
  //  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glPushMatrix();
  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  //glEnable(GL_LINE_SMOOTH);
  glLineWidth(2);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  glScalef(scale,scale,scale);
  glBegin(GL_LINES);
  glColor3ub(255,0,0);
  glVertex3i(0,0,0);
  glVertex3i(1,0,0);
  glColor3ub(0,255,0);
  glVertex3i(0,0,0);
  glVertex3i(0,1,0);
  glColor3ub(0,0,255);
  glVertex3i(0,0,0);
  glVertex3i(0,0,1);
  glEnd();
  glPopMatrix();
  //  glPopAttrib();
}


// GLubyte rarray[256*256];

// static void captureArea (const uint format, const uint x, const uint y, GLubyte **pix)
// {
//    *pix = (GLubyte*) calloc (512*512, sizeof(GLubyte));
//    glReadPixels (x,y,512,512,format,GL_UNSIGNED_BYTE,*pix);
// }


// static void newTexture (const uint tn, const GLint ncomp, const GLint s, const GLenum format,
//                         const GLubyte *pix)
// {
//      glBindTexture (GL_TEXTURE_2D, tn);
//      gluBuild2DMipmaps (GL_TEXTURE_2D, ncomp, s, s, format, GL_UNSIGNED_BYTE, pix);
// }

// void captureNoiseTexture (const uint texid, const uint per,
//                                 uint noct, GLubyte **pix)
// {
// uint s=512;
// uint i;
//   // enable blending
//   glEnable (GL_BLEND);
//   // set standard blending function
//   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//   // persistance in alpha
//   glColor4ub (255,255,255,per);

//   do
//   {
//      // we could use s*s but its more bytes...
//      i = 256*256;
//      // an array of random values...
//      while (i) {rarray[--i]=rand();}

//      // halve the resolution of texture each octave
//      s >>= 1;
//      newTexture (1, 1, s, GL_LUMINANCE, &(rarray[0]));

//      GLUquadric* params = gluNewQuadric();
//      gluQuadricDrawStyle(params,0);
//      gluSphere(params,0.75,20,20);

// //     drawCircle();
//   } while (--noct);
//   // now capture what we drew and put it into a texture
//   captureArea (GL_RED, 256, 256, pix);
//   newTexture  (texid, 1, 512, GL_LUMINANCE, *pix);
//   glDisable (GL_BLEND);
// }

void drawTVNoise(void)
{
  /* esta es muy parecida a las scanlines, pero aqui lo que hago es dibujar
     'quads' para hacer ruido de television ... */

  float ancho, alto,anchocuad,altocuad,brillo;
  float i,j;

  // Preparo mis datillos
  ancho = WIDTH;
  alto = HEIGHT;
  float halfalto = ancho/2, halfancho = ancho/2;

  anchocuad = ancho / 256.0f;
  altocuad = alto / 256.0f;

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glBegin(GL_QUADS);

  for(i=-halfalto; i<= halfalto; i+=altocuad) {
    for(j=-halfancho;j<=halfancho;j+=anchocuad) {

      brillo=((float)rand())/RAND_MAX;
      glColor4f(brillo,brillo,brillo,0.50);

      glVertex3f(j,i,0);
      glVertex3f(j+anchocuad,i,0);
      glVertex3f(j+anchocuad,i-altocuad,0);
      glVertex3f(j,i-altocuad,0);
    }
  }
  glEnd();
  glDisable(GL_BLEND);
  glPopMatrix(); // Restauro lo que hubiera del modelo ...
}

