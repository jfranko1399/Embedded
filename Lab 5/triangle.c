#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define VLEN 15
#define FLEN 13

double *cameraRot(double *cu, double **r);
double *scalarMult(double scalar, double *array);
double dotProd(double *A, double *B);
double *crossProd(double *A, double *B);
double *vectorAdd(double *A, double *B);
double *vectorSub(double *A, double *B);

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

double *scalarMult(double scalar, double *array)
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
  int maxVert, maxFace, len, diff, i, j, r, c;
  double **vertices, *tempV, max[3], min[3];
  double center[3], E, xRad, yRad, zRad, *temp, *temp2, a, *image, *rl, *bt, *ic, *abc;
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

  // double Rx[3][3] = {{1, 0, 0}, {0, cos(xRad), -sin(xRad)}, {0, sin(xRad), cos(xRad)}};
  // double Ry[3][3] = {{cos(yRad), 0, sin(yRad)}, {0, 1, 0}, {-sin(yRad), 0, cos(yRad)}};
  // double Rz[3][3] = {{cos(zRad), -sin(zRad), 0}, {sin(zRad), cos(zRad), 0}, {0, 0, 1}};

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
    //printf("%lf\n", Rx[i]);
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
  // for (i = 0; i < 3; i++)
  // {
  //   temp[i] = camera[0]*Rx[0][i] + camera[1]*Rx[1][i] + camera[2]*Rx[2][i];
  //   temp2[i] = up[0]*Rx[0][i] + up[1]*Rx[1][i] + up[2]*Rx[2][i];
  // }
  // for (i = 0; i < 3; i++)
  // {
  //   temp[i] = camera[0]*Ry[0][i] + camera[1]*Ry[1][i] + camera[2]*Ry[2][i];
  //   temp2[i] = up[0]*Ry[0][i] + up[1]*Ry[1][i] + up[2]*Ry[2][i];
  // }
  // for (i = 0; i < 3; i++)
  // {
  //   temp[i] = camera[0]*Rz[0][i] + camera[1]*Rz[1][i] + camera[2]*Rz[2][i];
  //   temp2[i] = up[0]*Rz[0][i] + up[1]*Rz[1][i] + up[2]*Rz[2][i];
  // }

  // for (i = 0; i < 3; i++)
  // {
  //   camera[i] = temp[i];
  //   up[i] = temp2[i];
  // }

  camera = cameraRot(camera, Rx);
  camera = cameraRot(camera, Ry);
  camera = cameraRot(camera, Rz);

  up = cameraRot(up, Rx);
  up = cameraRot(up, Ry);
  up = cameraRot(up, Rz);

  printf("%lf %lf %lf\n", camera[0], camera[1], camera[2]);

  temp = scalarMult(1.5 * E, camera);
  //printf("%lf\n", 1.5 * E);
  camera = vectorAdd(temp, center);
  //printf("camera: %lf %lf %lf\n", camera[0], camera[1], camera[2]);

  /////////////////////////////////////
  /// 3D Coordinates Bounding Image ///
  /////////////////////////////////////

  ////////////////////////////////////
  temp = vectorSub(center, camera);
  double* left = crossProd(up, temp);
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
  temp = scalarMult(E/(2 * a), left);
  left = vectorAdd(temp, center);
  //printf("left: %lf %lf %lf\n", left[0], left[1], left[2]);
  ////////////////////////////////////
  temp = vectorSub(center, camera);
  double *right = crossProd(temp, up);
  //printf("right: %lf %lf %lf\n", right[0], right[1], right[2]);
  ////////////////////////////////////
  temp = scalarMult(E/(2 * a), right);
  right = vectorAdd(temp, center);
  //printf("right: %lf %lf %lf\n", right[0], right[1], right[2]);
  ////////////////////////////////////
  temp = scalarMult(E/2, up);
  double *top = vectorAdd(temp, center);
  //printf("top: %lf %lf %lf\n", top[0], top[1], top[2]);
  ////////////////////////////////////
  temp = scalarMult(-E/2, up);
  double *bottom = vectorAdd(temp, center);
  //printf("bottom: %lf %lf %lf\n", bottom[0], bottom[1], bottom[2]);
  ////////////////////////////////////
  temp = scalarMult(E/2, up);
  double *topLeft = vectorAdd(temp, left);
  //printf("topLeft: %lf %lf %lf\n", topLeft[0], topLeft[1], topLeft[2]);

  rl = (double *)calloc(3, sizeof(double));
  bt = (double *)calloc(3, sizeof(double));
  ic = (double *)calloc(3, sizeof(double));
  abc = (double *)calloc(3, sizeof(double));

  final = (unsigned char *)calloc(256*256, sizeof(unsigned char));
  //printf("Rendering...\n");
  for (r = 0; r < 256; r++)
  {
    for (c = 0; c < 256; c++)
    {
      final[r * 256 + c] = 0;
      zBuffer = 999999;

      rl = vectorSub(right, left);
      rl = scalarMult((double)c/(256 - 1), rl);

      bt = vectorSub(bottom, top);
      bt = scalarMult((double)r/(256 - 1), bt);

      image = vectorAdd(topLeft, rl);
      image = vectorAdd(image, bt);

      // printf("rl: %lf %lf %lf\n", rl[0], rl[1], rl[2]);
      // printf("bt: %lf %lf %lf\n", bt[0], bt[1], bt[2]);
      // printf("image: %lf %lf %lf\n", image[0], image[1], image[2]);

      for (i = 0; i < maxFace; i++)
      {
        rl = vectorSub(vertices[faces[i][2]], vertices[faces[i][1]]);
        bt = vectorSub(vertices[faces[i][3]], vertices[faces[i][1]]);
        abc = crossProd(rl, bt);

        d = -dotProd(abc, vertices[faces[i][1]]);

        n = -dotProd(abc, camera) - d;
        ic = vectorSub(image, camera);
        d = dotProd(abc, ic);

        if (d > .1)
        {
          temp = scalarMult(n/d, ic);
          ic = vectorAdd(camera, temp);

          dot1 = dotProd(crossProd(vectorSub(vertices[faces[i][3]], vertices[faces[i][1]]), vectorSub(vertices[faces[i][2]], vertices[faces[i][1]])), crossProd(vectorSub(ic, vertices[faces[i][1]]), vectorSub(vertices[faces[i][2]], vertices[faces[i][1]])));
          if (dot1 >= 0)
          {
            dot2 = dotProd(crossProd(vectorSub(vertices[faces[i][1]], vertices[faces[i][2]]), vectorSub(vertices[faces[i][3]], vertices[faces[i][2]])), crossProd(vectorSub(ic, vertices[faces[i][2]]), vectorSub(vertices[faces[i][3]], vertices[faces[i][2]])));
            if (dot2 >= 0)
            {
              dot3 = dotProd(crossProd(vectorSub(vertices[faces[i][2]], vertices[faces[i][3]]), vectorSub(vertices[faces[i][1]], vertices[faces[i][3]])), crossProd(vectorSub(ic, vertices[faces[i][3]]), vectorSub(vertices[faces[i][1]], vertices[faces[i][3]])));
              if (dot3 >= 0)
              {
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
    //printf("%d \n", r);
  }
  //printf("\n");

  fpt2 = fopen("output.ppm", "wb");
  fprintf(fpt2, "P5 256 256 255\n");

  fwrite(final, 256*256, sizeof(unsigned char), fpt2);
}
