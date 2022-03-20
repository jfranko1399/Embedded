#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define VLEN 15
#define FLEN 13

double *scalarMult(int scalar, double *array);
double dotProd(double *A, double *B);
double *crossProd(double *A, double *B);
double *vectorAdd(double *A, double *B);
double *vectorSub(double *A, double *B);

double *scalarMult(int scalar, double *array)
{
  double *result = (double *)calloc(3, sizeof(double));
  int i;

  for (i = 0; i < 3; i++)
  {
    result[i] = scalar * array[i];
  }

  return result;
}

double dotProd(double *A, double *B)
{
  double result = 0.0;
  int i;

  for (i = 0; i < 3; i++)
  {
    result += A[i] * B[i];
  }

  return result;
}

double *crossProd(double *A, double *B)
{
  double *result = (double *)calloc(3, sizeof(double));

  result[0] = (A[1] * B[2]) - (A[2] * B[1]);
  result[1] = (A[2] * B[0]) - (A[0] * B[2]);
  result[2] = (A[0] * B[1]) - (A[1] * B[0]);

  return result;
}

double *vectorAdd(double *A, double *B)
{
  double *result = (double *)calloc(3, sizeof(double));
  int i;

  for (i = 0; i < 3; i++)
  {
    result[i] = A[i] + B[i];
  }

  return result;
}

double *vectorSub(double *A, double *B)
{
  double *result = (double *)calloc(3, sizeof(double));
  int i;

  for (i = 0; i < 3; i++)
  {
    result[i] = A[i] - B[i];
  }

  return result;
}

int main(int argc, char *argv[])
{
  FILE *fpt;
  FILE *fpt2;
  char *str, *buff;
  int maxVert, maxFace, len, diff, i, j;
  double **vertices, *tempV, max[3], min[3];
  double center[3], E, xRad, yRad, zRad, *temp, a;
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

  center[0] = (max[0] + min[0])/2;
  center[1] = (max[1] + min[1])/2;
  center[2] = (max[2] + min[2])/2;
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

  double Rx[3][3] = {{1, 0, 0}, {0, cos(xRad), -sin(xRad)}, {0, sin(xRad), cos(xRad)}};
  double Ry[3][3] = {{cos(yRad), 0, sin(yRad)}, {0, 1, 0}, {-sin(yRad), 0, cos(yRad)}};
  double Rz[3][3] = {{cos(zRad), -sin(zRad), 0}, {sin(zRad), cos(zRad), 0}, {0, 0, 1}};

  double camera[3] = {1, 0, 0};
  double up[3] = {0, 0, 1};
  temp = scalarMult(1.5 * E, camera);
  for (i = 0; i < 3; i++)
  {
    camera[i] = temp[i] + center[i];
  }

  ////////////////////////////////////
  temp = vectorSub(center, camera);
  double* left = crossProd(up, temp);
  ////////////////////////////////////
  a = 0;
  for (i = 0; i < 3; i++)
  {
    a += left[i] * left[i];
  }
  a = sqrt(a);
  ////////////////////////////////////
  temp = scalarMult(E/(2 * a), left);
  left = vectorAdd(temp, center);
  ////////////////////////////////////
  temp = vectorSub(center, camera);
  double *right = crossProd(temp, up);
  ////////////////////////////////////
  temp = scalarMult(E/(2 * a), right);
  right = vectorAdd(temp, center);
  ////////////////////////////////////
  temp = scalarMult(E/2, up);
  double *top = vectorAdd(temp, center);
  ////////////////////////////////////
  temp = scalarMult(-E/2, up);
  double *bottom = vectorAdd(temp, center);
  ////////////////////////////////////
  temp = scalarMult(E/2, up);
  double *topLeft = vectorAdd(temp, left);


}
