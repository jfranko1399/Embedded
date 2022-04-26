#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define tolerance 0.000001

void main(int argc, char *argv[])
{
  double period[] = {10.56, 40.96, 61.44, 100, 165, 285, 350, 700};
  double result[] = {0, 0, 6, 6, 2, 2, 2, 0};
  double io[] = {.3, .3, .3, .2, .2, .2, 0, 0};
  double disk[] = {3, 3, 3, 3, 3, 3, 0, 0};
  double compare = 0, overhead = .153;
  double min = 0, max = 0;
  int i, k, l, j;
  bool flag = false;

  double b[] = {3.3, 3.3, 9.3, 9.2, 5.2, 5.2, 2, 0};
  double r[] = {1.3, 4.7, 9, 23, 38.3, 10, 3, 2};

  for (i = 1; i <= 8; i++)
  {
    for (k = 1; k <= i; k++)
    {
      flag = false;
      for (l = 1; l <= floor(period[i-1]/period[k-1]); l++)
      {
        for (j = 1; j <= (i - 1); j++)
        {
          compare += ((r[j-1] + overhead) * ceil((l * period[k-1])/period[j-1]));
        }
        compare += (r[i-1] + b[i-1]);

        min = (l * period[k-1] - tolerance);
        max = (l * period[k-1] + tolerance);
        //printf("min: %lf max: %lf\n", compare - min, max - compare);
        //printf("abs: %lf\n", abs(compare - l * period[k-1]));
        if ((abs(compare - (l * period[k-1])) <= tolerance) || (compare <= (l * period[k-1])))
        {
          flag = true;
          printf("i: %d   k: %d   l: %d\n", i, k, l);
        }

        compare = 0;

        if (flag == true)
        {
          break;
        }
      }
    }
  }
}

//((compare <= (l * period[k-1] + tolerance)) && (compare >= (l * period[k-1] - tolerance)))
//(abs(compare - (l * period[k-1])) <= tolerance)
