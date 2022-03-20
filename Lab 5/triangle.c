#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define VLEN 15
#define FLEN 13

int main(int argc, char *argv[])
{
  FILE *fpt;
  FILE *fpt2;
  char *str, *buff;
  int maxVert, maxFace, len, diff, i, j;
  double **vertices, *tempV;
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

  vertices = (double **)calloc(maxVert, sizeof(double *));
  faces = (int **)calloc(maxFace, sizeof(int *));

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
    //printf("\n");
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
}
