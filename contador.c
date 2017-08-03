#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
  int i, aux;
  char msg[20];
  clock_t inicio, final;
  inicio = clock();
  for (i=0; i < 10000; i++)
  {
    final = clock();
    aux = (final - inicio)/1000;
    printf("%d\n%d\n", i , aux );
  }
  return 0;
}
