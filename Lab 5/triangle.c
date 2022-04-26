#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define VLEN 15
#define FLEN 13

double *cameraRot(double *cu, double **r);
void scalarMult(double scalar, double *array);
double dotProd(double *A, double *B);
void crossProd(double *A, double *B, double *out);
void vectorAdd(double *A, double *B, double *out);
void vectorSub(double *A, double *B, double *out);

double *cameraRot(double *cu, double **r)
{
  double *result = (double *)calloc(3, sizeof(double));
  int i;
  for (i = 0; i < 3; i++)
  {
    result[i] = cu[0]*r[0][i] + cu[1]*r[1][i] + cu[2]*r[2][i];
  }

  return result;
}

void scalarMult(double scalar, double *array)
{
  //double *result = (double *)calloc(3, sizeof(double));
  int i;

  for (i = 0; i < 3; i++)
  {
    array[i] = scalar * array[i];
  }

  //return result;
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

void crossProd(double *A, double *B, double *out)
{
  //double *result = (double *)calloc(3, sizeof(double));

  out[0] = (A[1] * B[2]) - (A[2] * B[1]);
  out[1] = (A[2] * B[0]) - (A[0] * B[2]);
  out[2] = (A[0] * B[1]) - (A[1] * B[0]);

  //return result;
}

void vectorAdd(double *A, double *B, double *out)
{
  //double *result = (double *)calloc(3, sizeof(double));
  int i;

  for (i = 0; i < 3; i++)
  {
    out[i] = A[i] + B[i];
  }

  //return result;
}

void vectorSub(double *A, double *B, double *out)
{
  //double *result = (double *)calloc(3, sizeof(double));
  int i;

  for (i = 0; i < 3; i++)
  {
    out[i] = A[i] - B[i];
  }

  //return result;
}

int main(int argc, char *argv[])
{
  FILE *fpt;
  FILE *fpt2;
  char *str, *buff;
  int maxVert, maxFace, len, diff, i, j, r, c;
  double **vertices, *tempV, max[3], min[3];
  double center[3], E, xRad, yRad, zRad;
  double *temp, *temp2, *temp3, *temp4, *temp5, *temp6, a, *image, *rl, *bt, *ic, *abc;
  double dot1, dot2, dot3, d, n, zBuffer;
  int **faces, *tempF;
  unsigned char *final;

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

  maxVert = atoi(buff);

  fgets(str, 100, fpt);
  fgets(str, 100, fpt);
  fgets(str, 100, fpt);
  fgets(str, 100, fpt);

  len = strlen(str);

  strncpy(buff, str + FLEN, len - FLEN - 2);

  maxFace = atoi(buff);

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
    for (j = 1; j < 3; j++)
    {
      vertices[i][j] = strtod(strtok(NULL, " "), NULL);
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
  }

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
    for (j = 1; j < 4; j++)
    {
      faces[i][j] = atoi(strtok(NULL, " "));
    }
  }

  ////////////////////////////
  /// Calc Camera Position ///
  ////////////////////////////

  xRad = strtod(argv[2], NULL) * (M_PI/180);
  yRad = strtod(argv[3], NULL) * (M_PI/180);
  zRad = strtod(argv[4], NULL) * (M_PI/180);

  double **Rx = (double **)calloc(3, sizeof(double *));
  double **Ry = (double **)calloc(3, sizeof(double *));
  double **Rz = (double **)calloc(3, sizeof(double *));

  for (i = 0; i < 3; i++)
  {
    Rx[i] = (double *)calloc(3, sizeof(double));
    Ry[i] = (double *)calloc(3, sizeof(double));
    Rz[i] = (double *)calloc(3, sizeof(double));
  }

  double tx[3][3] = {{1, 0, 0}, {0, cos(xRad), -sin(xRad)}, {0, sin(xRad), cos(xRad)}};
  double ty[3][3] = {{cos(yRad), 0, sin(yRad)}, {0, 1, 0}, {-sin(yRad), 0, cos(yRad)}};
  double tz[3][3] = {{cos(zRad), -sin(zRad), 0}, {sin(zRad), cos(zRad), 0}, {0, 0, 1}};

  for (i = 0; i < 3; i++)
  {
    Rx[i] = tx[i];
    Ry[i] = ty[i];
    Rz[i] = tz[i];
  }

  double *camera = (double *)calloc(3, sizeof(double));
  camera[0] = 1;
  camera[1] = 0;
  camera[2] = 0;
  double *up = (double *)calloc(3, sizeof(double));
  up[0] = 0;
  up[1] = 0;
  up[2] = 1;

  temp = (double *)calloc(3, sizeof(double));
  temp2 = (double *)calloc(3, sizeof(double));
  temp3 = (double *)calloc(3, sizeof(double));
  temp4 = (double *)calloc(3, sizeof(double));
  temp5 = (double *)calloc(3, sizeof(double));
  temp6 = (double *)calloc(3, sizeof(double));
  image = (double *)calloc(3, sizeof(double));

  camera = cameraRot(camera, Rx);
  camera = cameraRot(camera, Ry);
  camera = cameraRot(camera, Rz);

  up = cameraRot(up, Rx);
  up = cameraRot(up, Ry);
  up = cameraRot(up, Rz);

  //printf("%lf %lf %lf\n", camera[0], camera[1], camera[2]);
  //printf("%lf %lf %lf\n", up[0], up[1], up[2]);

  scalarMult(1.5 * E, camera);
  vectorAdd(camera, center, camera);
  //printf("%lf %lf %lf\n", camera[0], camera[1], camera[2]);

  /////////////////////////////////////
  /// 3D Coordinates Bounding Image ///
  /////////////////////////////////////

  ////////////////////////////////////
  vectorSub(center, camera, temp);
  double* left = (double *)calloc(3, sizeof(double));
  crossProd(up, temp, left);
  //printf("left: %lf %lf %lf\n", left[0], left[1], left[2]);
  ////////////////////////////////////
  a = 0;
  for (i = 0; i < 3; i++)
  {
    a += left[i] * left[i];
  }
  a = sqrt(a);
  //printf("a: %lf\n", a);
  ////////////////////////////////////
  scalarMult(E/(2 * a), left);
  vectorAdd(left, center, left);
  //printf("left: %lf %lf %lf\n", left[0], left[1], left[2]);
  ////////////////////////////////////
  vectorSub(center, camera, temp);
  double *right = (double *)calloc(3, sizeof(double));
  crossProd(temp, up, right);
  //printf("right: %lf %lf %lf\n", right[0], right[1], right[2]);
  ////////////////////////////////////
  scalarMult(E/(2 * a), right);
  vectorAdd(right, center, right);
  //printf("right: %lf %lf %lf\n", right[0], right[1], right[2]);
  ////////////////////////////////////
  scalarMult(E/2, up);
  double *top = (double *)calloc(3, sizeof(double));
  vectorAdd(up, center, top);
  //printf("top: %lf %lf %lf\n", top[0], top[1], top[2]);
  ////////////////////////////////////
  scalarMult(-1.0, up);
  double *bottom = (double *)calloc(3, sizeof(double));
  vectorAdd(up, center, bottom);
  //printf("bottom: %lf %lf %lf\n", bottom[0], bottom[1], bottom[2]);
  ////////////////////////////////////
  scalarMult(-1.0, up);
  double *topLeft = (double *)calloc(3, sizeof(double));
  vectorAdd(up, left, topLeft);
  //printf("topLeft: %lf %lf %lf\n", topLeft[0], topLeft[1], topLeft[2]);

  rl = (double *)calloc(3, sizeof(double));
  bt = (double *)calloc(3, sizeof(double));
  ic = (double *)calloc(3, sizeof(double));
  abc = (double *)calloc(3, sizeof(double));

  final = (unsigned char *)calloc(256*256, sizeof(unsigned char));
  printf("Rendering...\n");
  for (r = 0; r < 256; r++)
  {
    for (c = 0; c < 256; c++)
    {
      final[r * 256 + c] = 0;
      zBuffer = 999999;

      vectorSub(right, left, rl);
      scalarMult((double)c/(256 - 1), rl);

      vectorSub(bottom, top, bt);
      scalarMult((double)r/(256 - 1), bt);

      vectorAdd(topLeft, rl, image);
      vectorAdd(image, bt, image);

      for (i = 0; i < maxFace; i++)
      {
        vectorSub(vertices[faces[i][2]], vertices[faces[i][1]], rl);
        vectorSub(vertices[faces[i][3]], vertices[faces[i][1]], bt);
        crossProd(rl, bt, abc);

        d = -dotProd(abc, vertices[faces[i][1]]);

        n = -dotProd(abc, camera) - d;
        vectorSub(image, camera, ic);
        d = dotProd(abc, ic);

        if (d > .1)
        {
          scalarMult(n/d, ic);
          vectorAdd(camera, ic, ic);

          vectorSub(vertices[faces[i][3]], vertices[faces[i][1]], temp);
          vectorSub(vertices[faces[i][2]], vertices[faces[i][1]], temp2);
          vectorSub(ic, vertices[faces[i][1]], temp3);
          vectorSub(vertices[faces[i][2]], vertices[faces[i][1]], temp4);
          crossProd(temp, temp2, temp5);
          crossProd(temp3, temp4, temp6);
          dot1 = dotProd(temp5, temp6);
          // printf("dot1: %lf\n", dot1);
          // if (c == 1)
          // {
          //   exit(0);
          // }
          if (dot1 >= 0)
          {
            vectorSub(vertices[faces[i][1]], vertices[faces[i][2]], temp);
            vectorSub(vertices[faces[i][3]], vertices[faces[i][2]], temp2);
            vectorSub(ic, vertices[faces[i][2]], temp3);
            vectorSub(vertices[faces[i][3]], vertices[faces[i][2]], temp4);
            crossProd(temp, temp2, temp5);
            crossProd(temp3, temp4, temp6);
            //dot2 = dotProd(crossProd(vectorSub(vertices[faces[i][1]], vertices[faces[i][2]]), vectorSub(vertices[faces[i][3]], vertices[faces[i][2]])), crossProd(vectorSub(ic, vertices[faces[i][2]]), vectorSub(vertices[faces[i][3]], vertices[faces[i][2]])));
            dot2 = dotProd(temp5, temp6);
            //printf("dot2: %lf\n", dot2);
            if (dot2 >= 0)
            {
              vectorSub(vertices[faces[i][2]], vertices[faces[i][3]], temp);
              vectorSub(vertices[faces[i][1]], vertices[faces[i][3]], temp2);
              vectorSub(ic, vertices[faces[i][3]], temp3);
              vectorSub(vertices[faces[i][1]], vertices[faces[i][3]], temp4);
              crossProd(temp, temp2, temp5);
              crossProd(temp3, temp4, temp6);
              //dot3 = dotProd(crossProd(vectorSub(vertices[faces[i][2]], vertices[faces[i][3]]), vectorSub(vertices[faces[i][1]], vertices[faces[i][3]])), crossProd(vectorSub(ic, vertices[faces[i][3]]), vectorSub(vertices[faces[i][1]], vertices[faces[i][3]])));
              dot3 = dotProd(temp5, temp6);
              //printf("dot3: %lf\n", dot3);
              if (dot3 >= 0)
              {
                //printf("dick\n");
                if (((n/d) <= zBuffer))
                {
                  final[r * 256 + c] = 155 + (i % 100);
                  zBuffer = n/d;
                }
              }
            }
          }
        }
      }
    }
    printf("%d ", r);
  }
  printf("\n");

  fpt2 = fopen("output.ppm", "wb");
  fprintf(fpt2, "P5 256 256 255\n");

  fwrite(final, 256*256, sizeof(unsigned char), fpt2);
}
