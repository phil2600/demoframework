/* From Spanish Demo */

#include "../lib.hh"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../includes/graphic.hh"

Mesh::Mesh(void)
{
  this->numVertex=0;
  this->numFaces=0;
}

Mesh::~Mesh()
{
  if(this->numVertex>0)
  {
    delete(this->x);
    delete(this->y);
    delete(this->z);
  }
}


void Mesh::draw(void)
{
  int i,v1,v2,v3;

  glBegin(GL_TRIANGLES);
  for(i=0;i<this->numFaces;i++)
{
    v1=this->faceList[0][i];
    v2=this->faceList[1][i];
    v3=this->faceList[2][i];

    glNormal3f(this->normalList[0][i],this->normalList[1][i],this->normalList[2][i]);
    glVertex3f(this->x[v1],this->y[v1],this->z[v1]);

    glVertex3f(this->x[v2],this->y[v2],this->z[v2]);

    glVertex3f(this->x[v3],this->y[v3],this->z[v3]);
  }

  glEnd();

}


void Mesh::drawWire(float linewidth,int numcar)
{
  int i,v1,v2,v3,num;
  GLfloat prevLineWidth;

  glGetFloatv(GL_LINE_WIDTH,&prevLineWidth);
  glLineWidth(linewidth);
  glPointSize(4);

  if(numcar > this->numFaces)
    num = this->numFaces;
  else
    num = numcar;

  for(i=0;i<num;i++)
  {
    glBegin(GL_LINE_STRIP);
    v1=this->faceList[0][i];
    v2=this->faceList[1][i];
    v3=this->faceList[2][i];
    glVertex3f(this->x[v1],this->y[v1],this->z[v1]);
    glVertex3f(this->x[v2],this->y[v2],this->z[v2]);
    glVertex3f(this->x[v3],this->y[v3],this->z[v3]);
    glEnd();
  }

  glLineWidth(prevLineWidth);

}

int Mesh::loadMesh(char *filename)
{
  FILE *handle;
  char s[255],buf[255];
  int numvert,numfaces,i,temp,ok;
  int v1,v2,v3;
  float x,y,z;

  if(filename != NULL)
  {
    handle = fopen(filename, "rb");
    if(handle==NULL)
    {
      printf("Mesh::loadMesh - can't open %s\n",filename);
      return -1;
    }
    fgets(buf,255,handle);

    if(!strstr(buf,"*3DSMAX_ASCIIEXPORT"))
    {
      printf("Mesh::loadMesh - %s not a valid ase file\n",filename);
      return -1;
    }

    ok = 0;
    strcpy(buf,"");
    while(!feof(handle))
    {
      fgets(buf,255,handle);
      if(strstr(buf, "*MESH {")) {
	ok=1;
	break;
      }
    }

    if(!ok)
    {
      printf("Mesh::loadMesh - %s not a mesh found ??\n",filename);
      return -1;
    }

    fgets(buf,255,handle);
    //printf("%s",buf);
    // Número de vértices
    fgets(buf,255,handle);
    sscanf(buf,"%s %d",s,&numvert);
    //printf("%s %d",s,numvert);
    this->numVertex=numvert;

    //Número de caras
    fgets(buf,255,handle);
    sscanf(buf,"%s %d",s,&numfaces);
    this->numFaces=numfaces;

    this->x = new float[this->numVertex];
    this->y = new float[this->numVertex];
    this->z = new float[this->numVertex];

    this->faceList[0] = new int[this->numFaces];
    this->faceList[1] = new int[this->numFaces];
    this->faceList[2] = new int[this->numFaces];

    this->normalList[0] = new float[this->numFaces];
    this->normalList[1] = new float[this->numFaces];
    this->normalList[2] = new float[this->numFaces];

    fgets(buf,255,handle);
    if(!strstr(buf, "*MESH_VERTEX_LIST {"))
    {
      printf("Mesh::loadMesh - %s not vertexlist found ??\n",filename);
      return -1;
    }

    for(i=0;i<this->numVertex;i++)
    {
      fgets(buf,255,handle);
      //printf("%f %f %f\n",x,y,z);
      sscanf(buf,"%s %d %f %f %f",s,&temp,&x,&y,&z);
      this->x[i]=x;
      this->y[i]=y;
      this->z[i]=z;
    }

    fgets(buf,255,handle);

    fgets(buf,255,handle);

    if(!strstr(buf,"*MESH_FACE_LIST {"))
    {
      printf("Mesh::loadMesh - %s not face list found ??\n",filename);
      return -1;
    }

    for(i=0;i<this->numFaces;i++)
    {
      //			*MESH_FACE    0:    A:    0 B:    2 C:    3 AB:    1 BC:    1 CA:    0	 *MESH_SMOOTHING 2 	*MESH_MTLID 1
      strcpy(buf,"");
      fgets(buf,255,handle);

      //sscanf(buf,"%s %d %f %f %f",s,&temp,&x,&y,&z);
      //             0  a  0  b  2  c  3
      //sscanf(buf,"%* %* %* %* %d %* %d %* %d",&v1,&v2,&v3);
      char s1[255],s2[255],s3[255],s4[255];
      sscanf(buf,"%s %d %s %s %d %s %d %s %d",s,&temp,s1,s2,&v1,s3,&v2,s4,&v3);
      this->faceList[0][i]=v1;
      this->faceList[1][i]=v2;
      this->faceList[2][i]=v3;
    }

    ok=0;
    strcpy(buf,"");
    while(!feof(handle))
    {
      fgets(buf,255,handle);
      //printf("%s",buf);
      if(strstr(buf,"*MESH_NORMALS {"))
      {
	ok=1;
	break;
      }
    }

    if(strstr(buf,"*MESH_NORMALS {"))
    {
      //printf("Encontre la malla\n");
    }
    else
    {
      printf("Mesh::loadMesh - %s not normal list found ??\n",filename);
      return -1;
    }

    for(i=0;i<this->numFaces;i++) {
      strcpy(buf,"");
      fgets(buf,255,handle);
      // *MESH_FACENORMAL 0	0.0000	0.0000	-1.0000
      sscanf(buf,"%s %d %f %f %f",s,&temp,&x,&y,&z);
      this->normalList[0][i]=x;
      this->normalList[1][i]=y;
      this->normalList[2][i]=z;
      // tres fgets para tres lineas un poco inutilizadas
      fgets(buf,255,handle);
      fgets(buf,255,handle);
      fgets(buf,255,handle);
    }
  }
  this->normalize();

  return 0;
}

void Mesh::normalize(void)
{
  int i;
  float maxX=0,maxY=0,maxZ=0;

  for(i=0;i<this->numVertex;i++)
  {
    if(abs(this->x[i])>maxX)
      maxX=abs(this->x[i]);

    if(this->y[i]>maxY)
      maxY=this->y[i];

    if(this->z[i]>maxZ)
      maxZ=this->z[i];
  }

  for(i=0;i<=this->numVertex;i++)
  {
    if(maxX>1) this->x[i]/=maxX;
    if(maxY>1) this->y[i]/=maxY;
    if(maxZ>1) this->z[i]/=maxZ;
  }
}
