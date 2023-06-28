#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ARGV_LACK_FAILURE         -1
#define NEGATIVE_INPUT_FAILURE   -20
#define NOT_BI_FAILURE           -21
#define NOT_CONVERGENT_L_FAILURE -22

#define SEED 1235678910ULL

typedef long long int BI; //Big Int numbers

void InitParams ( int, char **, BI *, BI *, double *);
double S (double, double);
double mean (BI, double);
double mean_error (BI, double, double);

int main (int argv, char **argc)
{
  BI i, j, N_mcs, N_int;
  double L, NormP, Svalue, Ivalue, Sums_S, Sums_S2, Sums_I=0.0, Sums_I2=0.0;

  //Inizializzazione Parametri 
  InitParams (argv, argc, &N_mcs, &N_int, &L);

  //Inizializzazione funzione generatrice con SEED dato
  srand48(SEED);
  //Costante di Normalizzazione P(x,y)
  NormP = (double) (4.*L*L);

  for (j=0; j<N_int; j++)
    {
      Sums_S  = 0.0;
      Sums_S2 = 0.0;
      for (i=0; i<N_mcs; i++)
	{
	  Svalue   = S(NormP, L);
	  Sums_S  += Svalue;
	  Sums_S2 += Svalue*Svalue;
	};
      //printf ("I=%lf \t SIGMA_I=%lf \n\a", mean(N_mcs, Sums_S), mean_error (N_mcs, Sums_S, Sums_S2) );
      Ivalue  =  mean(N_mcs, Sums_S);
      Sums_I  += Ivalue;
      Sums_I2 += Ivalue*Ivalue;
    };
  printf ("%lld \t %lld \t %lf \t %lf \t %lf \n", N_mcs, N_int, L, mean(N_int, Sums_I), mean_error (N_int, Sums_I, Sums_I2) );
 
};

void InitParams ( int argv, char **argc, BI *N_mcs, BI *N_int, double *L)
{
  if (argv < 4)
    {
      fprintf ( stderr, "# YOU HAVE ENTERED FEW ARGUMENTS.\n");
      fprintf ( stderr, "# Please enter: %s <N_mcs> (Number of MonteCarlo Steps) <N_int> (Number of Integrations) <L> (integration from [-L,L]x[-L,L])", argc[0]);
      exit (ARGV_LACK_FAILURE);
    };
  //Assegnazione dei parametri di input da terminale
  
  //Assegnazione N_mcs e N_int, con controllo N_mcs > 10 e N_int > 3
  *N_mcs = (BI) atoi(argc[1]);
  *N_int = (BI) atoi(argc[2]);
  if ( *N_mcs <= 10 || *N_int <= 3)
    {
      if (*N_mcs<0)
	{
	  fprintf (stderr, "# Please enter a positive number for <N_mcs>. EXITING...\n\a");
	  exit (NEGATIVE_INPUT_FAILURE);
	};
      if (*N_int<0)
	{
	  fprintf (stderr, "# Please enter a positive number for <N_int>. EXITING...\n\a");
	  exit (NEGATIVE_INPUT_FAILURE);
	};
      exit (NOT_BI_FAILURE);
    };
  
  //Assegnazione L con controllo |L|<1
  *L     =      atof(argc[3]);
  if (*L<1)
    {
      
      if (*L<0)
	{
	  fprintf (stderr, "# Please enter a positive number. EXITING...\n\a");
	  exit (NEGATIVE_INPUT_FAILURE);
	};
      
      fprintf(stderr, "# Please enter L>=1. EXITING...\n\a");
      exit (NOT_CONVERGENT_L_FAILURE);
      };
};

double S (double NormP, double L)
{
  double x = L*(1-2*drand48()), y = L*(1-2*drand48());
  
  if (x*x+y*y<=1)
    {
      return NormP;
    }
  else
    {
      return 0.0;
    };
  
  /* Per il DEBUG
     printf("S(%.2lf L, %.2lf L) = %lf\n", x, y, s);
     return s;
  */
};

double mean (BI N, double S)
{
  return  S/N;
};

double mean_error (BI N, double S, double S2)
{
  return sqrt(  1./(double)(N-1.) * (mean(N, S2) - mean(N, S)*mean(N, S)) );
};

