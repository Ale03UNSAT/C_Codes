#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ARGV_LACK_FAILURE         -1
#define NEGATIVE_INPUT_FAILURE   -20
#define NOT_BI_FAILURE           -21
#define NOT_CONVERGENT_L_FAILURE -22

#define FALSE 0
#define TRUE  1
#define DIM   2
#define SEED  123567910ULL


struct point
{
  //Coordinate del punto in D dimensioni
  double x[DIM];
};
typedef struct point Point;

typedef long long int BI; //Big Int numbers
typedef unsigned char bool; //boolean type
  
void InitParams ( int, char **, BI *, BI *, double *, double *);
void InitPointStruct (Point *, double, double);
bool Is_InTheBox (double *, double *, double, BI, BI);
bool Is_InTheCircle (double *, double);
void MetropolisMCSTEP (Point *, Point *, double *, double, double, BI, BI);
double S (double, Point *);
double mean (BI, double);
double mean_error (BI, double, double);

int main (int argv, char **argc)
{
  BI i, j, p_counter=0, N_mcmc, N_int;
  double L, eps, NormP, Svalue, Ivalue, Sums_S, Sums_S2, Sums_I=0.0, Sums_I2=0.0, Sums_Pacc=0.0;
  Point Prev, Next;
  
  //Inizializzazione Parametri 
  InitParams (argv, argc, &N_mcmc, &N_int, &L, &eps);

  //Inizializzazione funzione generatrice con SEED dato
  srand48(SEED);
  //Costante di Normalizzazione P(x,y)
  NormP = (double) (4.*L*L);

  //Inizializza gli elementi della struttura point
  InitPointStruct(&Prev, L*(1-2*drand48()),  L*(1-2*drand48()));
  InitPointStruct(&Next, 0.0, 0.0);
  
  printf ("###############################################\n");
  printf ("## N_mcmc = %lld \t N_int = %lld \t L = %.5lf \t eps = %.5lf\n", N_mcmc, N_int, eps, L );
  printf ("## Prev.x = %.5lf \t Prev.y = %.5lf\n", *(Prev.x+0), *(Prev.x+1) );
  printf ("## Next.x = %.5lf \t Next.y = %.5lf \n", *(Next.x+0), *(Next.x+1) );
  printf ("###############################################\n");
  

  printf ("# N_MCMC \t N_INT \t EPS \t L \t MEAN_I \t ERROR_MEAN_I\n");
  for (j=0; j<N_int; j++)
    {
      //Inizializzazione delle somme Sums_S e Sums_S2
      Sums_S  = 0.0;
      Sums_S2 = 0.0;
      
      for (i=0; i<N_mcmc; i++)
	{
	  p_counter+=1;
	  //Passo di MetropolisMC
	  MetropolisMCSTEP (&Prev, &Next, &Sums_Pacc, eps, L, N_mcmc, p_counter);
	  //Calcolo valore di s
	  Svalue   = S(NormP, &Next);
	  
	  //Somme
	  Sums_S  += Svalue;
	  Sums_S2 += Svalue*Svalue;
	};
      Ivalue   = mean(N_mcmc, Sums_S);
      Sums_I  += Ivalue;
      Sums_I2 += Ivalue*Ivalue;
    };
  
  printf ("%lld \t %lld \t %lf \t %lf \t %lf \t %lf\n", N_mcmc, N_int, eps, mean(N_int, Sums_I), mean_error(N_int, Sums_I, Sums_I2), mean(N_mcmc, Sums_Pacc)); 
};

void InitParams ( int argv, char **argc, BI *N_mcmc, BI *N_int, double *L, double *eps)
{
  if (argv < 5)
    {
      fprintf ( stderr, "# YOU HAVE ENTERED FEW ARGUMENTS.\n");
      fprintf ( stderr, "# Please enter: %s <N_mcmc> <N_int> <L> <eps> <mode>\n", argc[0]);
      fprintf ( stderr, "- <N_mcs> (Number of Metropolis Markov Chain MonteCarlo Steps)\n");
      fprintf ( stderr, "- <N_int> (Number of Integration for the MEAN of the Integral and the related ERROR)\n");
      fprintf ( stderr, "- <L>     (Length of the Integration Square Frame)\n");
      fprintf ( stderr, "- <eps>   (Radius of the Around used in the Metropolis Markov Chain Step)\n");
      exit (ARGV_LACK_FAILURE);
    };
  //Assegnazione dei parametri di input da terminale
  
  //Assegnazione N_mcmc, con controllo N_mcmc > 10
  *N_mcmc = (BI) atoi(argc[1]);
  if ( *N_mcmc <= 10)
    {
      if (*N_mcmc<0)
	{
	  fprintf ( stderr, "# Please enter a positive number for <N_mcc>. EXITING...\n\a");
	  exit (NEGATIVE_INPUT_FAILURE);
	};
      fprintf ( stderr, "# Plase enter N_mcmc > 10. EXITING...\n\a");
      exit (NOT_BI_FAILURE);
    };

  //Assegnazione N_int, con controllo N_int > 3
  *N_int  = (BI) atoi(argc[2]);
  if ( *N_int <= 3)
    {
      if (*N_int<0)
	{
	  fprintf ( stderr, "# Please enter a positive number for <N_int>. EXITING...\n\a");
	  exit (NEGATIVE_INPUT_FAILURE);
	};
      fprintf ( stderr, "# Plase enter N_int > 3. EXITING...\n\a");
      exit (NOT_BI_FAILURE);
    };  
  
  //Assegnazione L con controllo L>=1
  *L      =      atof(argc[3]);
  if (*L<1)
    {
      
      if (*L<0)
	{
	  fprintf (stderr, "# Please enter a positive number. EXITING...\n\a");
	  exit (NEGATIVE_INPUT_FAILURE);
	};
      
      fprintf ( stderr, "# Please enter L>=1. EXITING...\n\a");
      exit (NOT_CONVERGENT_L_FAILURE);
      };

  //Assegnazione eps con controllo 0<eps<L
  *eps    =      atof(argc[4]);
  if (*eps<0)
    {
      fprintf ( stderr, "# Please enter a positive number. EXITING...\n\a");
      exit (NEGATIVE_INPUT_FAILURE);
    };
  if (*eps>=2*sqrt(2)*(*L))
    {
      fprintf ( stderr, "# Please enter 0<eps<2sqrt(2)L. EXITING...\n\a");
      exit (NOT_CONVERGENT_L_FAILURE);
      };
};

void InitPointStruct (Point *P, double x1, double x2)
{
  *((*P).x+0) = x1;
  *((*P).x+1) = x2;
};

bool Is_InTheBox (double *v , double *Sums_P, double L, BI N_STEP, BI p_index)
{
  if ( ( (v[0]>=-L) && (v[0]<=L) ) && ( (v[1]>=-L) && (v[1]<=L) ) )
    {
      if (p_index <= N_STEP)
	{
	  *Sums_P += 1.0;
	};
      return TRUE;
    }
  else
    {return FALSE;};
};

bool Is_InTheCircle (double *v , double r)
{
  if ( (*(v+0)) * (*(v+0)) + (*(v+1)) * (*(v+1)) <= r*r )
    {return TRUE;}
  else
    {return FALSE;};
};

void MetropolisMCSTEP (Point *P, Point *N, double *Sums_P, double eps, double L, BI N_STEP, BI p_index)
{
  double r = eps*(1.-2.*drand48()), theta = 2.*M_PI*drand48(), xnew[DIM];

  *(xnew+0) = *((*P).x+0) + r * cos(theta);
  *(xnew+1) = *((*P).x+1) + r * sin(theta);

  if ( Is_InTheBox( xnew , Sums_P, L, N_STEP, p_index) ) //Is_InTheBox rappresenta la probabilità di accettazione
    {
      // Next.x 
      *((*N).x+0) = *(xnew+0);
      // Next.y
      *((*N).x+1) = *(xnew+1);      
    };
  
  for (int d=0; d<DIM; d++)
    {
      *((*P).x+d) = *((*N).x+d);
    };  
};

double S (double NormP, Point *P)
{  
  if ( Is_InTheCircle( (*P).x , 1.) )
    {return NormP;}
  else
    {return 0.0;};
};

double mean (BI N, double S)
{
  return  S/( (double)N );
};

double mean_error (BI N, double S, double S2)
{
  return sqrt(  1./( (double)N-1. ) * ( mean ( N, S2) - mean(N, S)*mean(N, S)) );
  //return sqrt(  S2/( (double)N-1. ) -   mean ( (N-1)*(N-1), S*S) );
  //return sqrt(1./(N-1)*(S2 - S*S/N) );
};

