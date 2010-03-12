#ifndef MESH_HH_
# define MESH_HH_

class Mesh
{
 public:
  Mesh(void);
  ~Mesh();
  void draw(void);
  int loadMesh(char *filename);
  void drawWire(float linewidth,int numcar);

 private:
  int numVertex;
  int numFaces;
  float *x,*y,*z;
  int *faceList[3];
  float *normalList[3];

  void normalize(void);
};

#endif /* !MESH_HH_ */
