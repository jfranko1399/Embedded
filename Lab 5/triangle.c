#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define VLEN 15
#define FLEN 13

int main(int argc, char *argv[])
{
  FILE *fpt;
  FILE *fpt2;
  char *str, *buff;
  int maxVert, maxFace, len, diff, i, j;
  double **vertices, *tempV, max[3], min[3];
  double xCenter, yCenter, zCenter, E, xRad, yRad, zRad;
  double Rx[3][3], Ry[3][3], Rz[3][3];
  int **faces, *tempF;

  if (argc != 5)
  {
    printf("Usage: ./tri [file.ply] [x rotation] [y rotation] [z rotation]\n");
    exit(0);
  }

  if ((fpt = fopen(argv[1], "rb")) == NULL)
  {
    printf("Unable to open %s for reading\n", argv[1]);
    exit(0);
  }

  str = (char *)calloc(100, sizeof(char));
  buff = (char *)calloc(10, sizeof(char));

  ///////////////////////////////
  /// # of Vertices and Faces ///
  ///////////////////////////////

  fgets(str, 100, fpt);
  fgets(str, 100, fpt);
  fgets(str, 100, fpt);
  len = strlen(str);

  strncpy(buff, str + VLEN, len - VLEN - 2);
  //printf("%d\n", strlen(buff));

  maxVert = atoi(buff);
  //printf("%d\n", vertices);

  fgets(str, 100, fpt);
  fgets(str, 100, fpt);
  fgets(str, 100, fpt);
  fgets(str, 100, fpt);

  len = strlen(str);

  strncpy(buff, str + FLEN, len - FLEN - 2);
  //printf("%d\n", strlen(buff));

  maxFace = atoi(buff);
  //printf("%d\n", faces);

  ///////////////////////////////
  /// Read Vertices and Faces ///
  ///////////////////////////////

  vertices = (double **)calloc(maxVert, sizeof(double *));
  faces = (int **)calloc(maxFace, sizeof(int *));

  for (i = 0; i < 3; i++)
  {
    max[i] = 0;
    min[i] = 1000000;
  }

  fgets(str, 100, fpt);
  fgets(str, 100, fpt);

  for (i = 0; i < maxVert; i++)
  {
    vertices[i] = (double *)calloc(3, sizeof(double));
    fgets(str, 100, fpt);

    vertices[i][0] = strtod(strtok(str, " "), NULL);
    //printf("%f ", vertices[i][0]);
    for (j = 1; j < 3; j++)
    {
      vertices[i][j] = strtod(strtok(NULL, " "), NULL);
      //printf("%f ", vertices[i][j]);
    }

    for (j = 0; j < 3; j++)
    {
      if (vertices[i][j] > max[j])
      {
        max[j] = vertices[i][j];
      }
      if (vertices[i][j] < min[j])
      {
        min[j] = vertices[i][j];
      }
    }
    //printf("\n");
  }

  // for (i = 0; i < 3; i++)
  // {
  //   printf("%f ", max[i]);
  // }
  // printf("\n");
  // for (i = 0; i < 3; i++)
  // {
  //   printf("%f ", min[i]);
  // }
  // printf("\n");

  xCenter = (max[0] + min[0])/2;
  yCenter = (max[1] + min[1])/2;
  zCenter = (max[2] + min[2])/2;
  E = -1000000;
  for (i = 0; i < 3; i++)
  {
    if ((max[i] - min[i]) > E)
    {
      E = (max[i] - min[i]);
    }
  }

  for (i = 0; i < maxFace; i++)
  {
    faces[i] = (int *)calloc(4, sizeof(int));
    fgets(str, 100, fpt);

    faces[i][0] = atoi(strtok(str, " "));
    //printf("%d ", faces[i][0]);
    for (j = 1; j < 4; j++)
    {
      faces[i][j] = atoi(strtok(NULL, " "));
      //printf("%d ", faces[i][j]);
    }
    //printf("\n");
  }

  ////////////////////////////
  /// Calc Camera Position ///
  ////////////////////////////

  xRad = strtod(argv[2], NULL) * (M_PI/180);
  yRad = strtod(argv[3], NULL) * (M_PI/180);
  zRad = strtod(argv[4], NULL) * (M_PI/180);

  Rx = {{1, 0, 0}, {0, cos(xRad), -sin(xRad)}, {0, sin(xRad), cos(xRad)}};
  Ry = {{cos(yRad), 0, sin(yRad)}, {0, 1, 0}, {-sin(yRad), 0, cos(yRad)}};
  Rz = {{cos(zRad), -sin(zRad), 0}, {sin(zRad), cos(zRad, 0)}, {0, 0, 1}};

  
}
