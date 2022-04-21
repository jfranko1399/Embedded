#include <stdio.h>
#include <stdint.h>

void main()
{
  uint8_t byte, reverse;

  byte = 170;
  reverse = 0;

  int i;
  for (i = 7; i > 0; i--)
  {
    reverse += (((byte >> i) & 1) << (7 - i));
  }
  printf("%d\n", reverse);
}
