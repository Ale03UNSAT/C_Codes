#include <stdio.h>
#include <stdlib.h>

#define ARGV_LACK_FAILURE         -1
#define NEGATIVE_L_FAILURE       -20
#define NOT_CONVERGENT_L_FAILURE -21

typedef long long int division;

void InitParams ( int, char **, division *, double *);
unsigned int f (double, double);

int main (int argv, char **argc)
{
  unsigned int fvalue;
  division i, j, N, Sumf=0;
  double L, x=0., y, ds, I=0.;

  InitParams (argv, argc, &N, &L);

  ds = ((double) 2.*L)/(N-1);
  
  //printf("# N = %lld \t L = %f\n", N, L);

  x=-L;
  for (i=0; i<N; i++)
    {
      y=-L;
      for (j=0; j<N; j++)
	{
	  fvalue = f(x,y);
	  Sumf += fvalue;
	  //printf("# %.5f \t %.5f \t %d\n", x, y, fvalue);
	  y += ds;
	};
      x += ds;
    };
  I = ((double)4.*L*L*Sumf)/(N*N);
  printf ("#ANS# %lf \t %lld \t %lf\n", L, N, I);
};

void InitParams ( int argv, char **argc, division *N, double *L)
{
  if (argv < 3)
    {
      fprintf ( stderr, "# YOU HAVE ENTERED FEW ARGUMENTS.\n");
      fprintf ( stderr, "# Please enter: %s <N> (Number of divisions) <L> (integration from [-L,L]x[-L,L])", argc[0]);
      exit (ARGV_LACK_FAILURE);
    };
  //Assegnazione numero delle divisioni per ogni direzione (Totale N^(d) divisioni)
  *N = (division) atoi(argc[1]);
  *L =            atof(argc[2]);

  if (*L<1)
    {
      
      if (*L<0)
	{
	  fprintf (stderr, "# Please enter a positive number. EXITING...\n\a");
	  exit (NEGATIVE_L_FAILURE);
	};
      
      fprintf(stderr, "# Please enter L>=1. EXITING...\n\a");
      exit (NOT_CONVERGENT_L_FAILURE);
      };
};

unsigned int f (double x, double y)
{
  if ( x*x+y*y <= 1){return 1;}
  else {return 0;};
};
