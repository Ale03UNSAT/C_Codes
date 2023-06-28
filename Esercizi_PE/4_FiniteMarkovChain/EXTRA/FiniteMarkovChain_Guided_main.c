#include <stdio.h>
#include <stdlib.h>

#define ZERO_INPUT -1

struct PointOfTheChain
{
  //Numero identificativo (è utile utilizzare numeri per facilitare i confronti)
  int id;
  //Probabilità del Punto allo stato "t" (Prob(0) = 1./NumberOfPoints)
  double Probt;
  //Probabilità asintotica
  double Pi;
  
  //Interazioni
  //Numero delle Interazioni tra il punto i e gli altri punti
  int NumberOfLinks;
  //Puntatori della rete di puntatori
  struct PointOfTheChain **PointerArrayToAnotherPoint;
};

typedef struct PointOfTheChain POINT;
typedef long long int BI; //Big Int numbers

//Funzioni di interazione utente da terminale
int  Get_INT ( void );
double Get_PROB (void);

//Funzioni Inizializzatrici
void Init_Points (POINT *, int); //Inizializza gli elementi della struttura Point
void Create_Links (POINT *, int, int *); //Creo i link e la matrice u_ij

//Funzioni Principali
double A_ij (POINT *, int, int, int); //Funzione calcolatrice della matrice di accettazione nel punto i,j
void Create_W_ij (POINT *, int *, int, double *); //Funzione calcolatrice della matrice di transizione w_ij
void MC_STEP (POINT *, double *, int); //Funzione che esegue un passo della Markov Chain

  
//Funzioni di stampa
void Print_Matrix(double *, int);
void Print_Prob (POINT *, int);

int main (int argv, char **argc)
{
  BI i, NumberOfSteps;
  int NumberOfPoints=-1, *Links;
  double *Wij;
  POINT *Set;
  
  //Acquaisizione del numero di punti della catena
  printf ("Enter the NUMBER OF POINTS of the chain:\n");
  NumberOfPoints = (int) Get_INT();
  if (NumberOfPoints==0)
    {
      fprintf (stderr, "# <NumberOfPoint> must be greater then 0.EXITING...\n\a");
      exit(ZERO_INPUT);
    }
  printf("# NumberOfPoints = %d\n", NumberOfPoints);
  
  //Acquaisizione del numero di punti della catena
  printf ("Enter the NUMBER OF STEPS:\n");
  NumberOfSteps = (BI) Get_INT();
  if (NumberOfSteps==0)
    {
      fprintf ( stderr, "# <NumberOfSteps> must be greater then 0.EXITING...\n\a");
      exit(ZERO_INPUT);
    }
  printf("# NumberOfSteps = %lld\n", NumberOfSteps);

  
  //Allocazione degli array
  Wij   = (double *) malloc ( NumberOfPoints * NumberOfPoints * sizeof (double) );
  Links = (int *)    malloc ( NumberOfPoints * NumberOfPoints * sizeof (int) );
  Set   = (POINT *)  malloc ( NumberOfPoints *                  sizeof (POINT)  );

  //Inizializzazione dei punti della Catena con assegnazione Probabilità Asitotica
  Init_Points (Set, NumberOfPoints);

  //Creazione delle connessioni del grafo 
  Create_Links (Set, NumberOfPoints, Links);

  //Calcolo W_ij
  Create_W_ij (Set, Links, NumberOfPoints, Wij);
	       
  //Stampa Matrice U_ij e PRobabilità iniziale (1/link)
  Print_Matrix (Wij, NumberOfPoints);
  printf("#############################################################\n");
  Print_Prob(Set, NumberOfPoints);
  printf("#############################################################\n");
  
  for (i=0; i<NumberOfSteps; i++)
    {
      MC_STEP(Set, Wij, NumberOfPoints);
    };
  Print_Prob(Set, NumberOfPoints);
  printf("#############################################################\n");
  
  free(Wij);
  free(Links);
  free(Set);
};

int Get_INT (void)
{
  int input, cc=0;
  do {
    cc=scanf("%d", &input);
    while (getchar()!='\n');
  } while (cc!=1 || input<0);
  return input;
};

double Get_PROB (void) 
{
  int cc=0;
  double input;
  do {
    cc=scanf("%lf", &input);
    while (getchar()!='\n');
    if (input<0 || input >1)
      {
	fprintf( stderr, "# %lf cannot be a probability\n", input);
      };
  } while (cc!=1 || (input<0 || input  >1) );
  return input;
};

void Init_Points (POINT *S, int N)
{
  for (int i=0; i<N; i++)
    {
      //ID
      (*(S+i)).id            = i+1;
      //Prob(0)
      (*(S+i)).Probt         = 1./N;
      //Azzeramento NumberOfLinks
      (*(S+i)).NumberOfLinks = 0;
      printf ("# Enter the ASYMPTOTIC PROBABILITY PI_%d\n", i+1);
      (*(S+i)).Pi            = Get_PROB();
    };
};

void Create_Links (POINT *S, int N, int *Links)
{
  //Intro
  printf ("# Construction of the Graph's Links\n");
  printf ("# Enter:\n");
  printf ("- 1 if there's a link between Point-i and Point-j;\n");
  printf ("- 0 if there is not a link between Point-i and Point-j\n"); 
  
  //Ciclo Righe
  for (int i=0; i<N; i++)
    {
      //Ciclo Colonne
      for (int j=0; j<N; j++)
	{
	  printf ("# %d --> %d:\n", i+1, j+1);
	  
	  //Acquisizione 0/1 con coversione di qualuque numero diverso da 0 in 1
	  *(Links+i*N+j) = Get_INT();
	  if ( *(Links+i*N+j) != 0 && *(Links+i*N+j) != 1)
	    {
	      fprintf (stderr, "# Turning %d into 1...\n", *(Links+i*N+j) );
	      *(Links+i*N+j) = 1;
	    };

	  //Conto il numero delle connessioni del punto i-esimo del grafo
	  (*(S+i)).NumberOfLinks += *(Links+i*N+j);
	};
    };
};


double A_ij (POINT *S, int N, int i, int j)
{
  if ( (*(S+j)).Pi >= (*(S+i)).Pi )
    {
      return  ( 1.0 );
    }
  else
    {
      return ( (*(S+j)).Pi / (*(S+i)).Pi ); 
    };
};
 
void Create_W_ij (POINT *S, int *L, int N, double *W)
{
  
  for (int i=0; i<N; i++)
    {
      for (int j=0; j<N; j++)
	{
	  *(W+i*N+j) = (double)(*(L+i*N+j)) / (*(S+i)).NumberOfLinks * A_ij(S, N, i, j);
	};
    };

  //Normalizzazione Righe w_ij
  for (int i=0; i<N; i++)
    {
      //Calcolo costante di normalizzazione
      double Norm_W=0.0;
      for (int j=0; j<N; j++)
	{
	  Norm_W += *(W+i*N+j);
	};
      //Normalizzazione di ogni riga
      for (int j=0; j<N; j++)
	{
	  *(W+i*N+j) /= Norm_W;
	};
    };
};
 
void MC_STEP (POINT *S, double *W, int N)
{
  //Array per contenere le nuove probabilità allocato dinamicamente
  double *Probt_NEW, Norm_P=0.0;
  Probt_NEW = (double *) calloc ( N, sizeof(double));
  
  //Ciclo su ogni punto
  for (int i=0; i<N; i++)
    {
      //Calcolo P_i(t+1) pr ogni i
      for (int j=0; j<N; j++)
	{
	  //P_i(t+1) = Somme  su j (P_j(t)*W_ji)
	  //OSS: W_ji è sommata lungo le colonne e ciò equivale a sommare W_ij lungo le righe
	  *(Probt_NEW+i) += (*(S+j)).Probt * (*(W+j*N+i));
	};
      //Costruzione della cosrante di normalizzazione della probabilità P(t+1)
      Norm_P += *(Probt_NEW+i);
    };

  //Sostituzione delle probabilità P(t)-->P(t+1) co normalizzaizone 
  for (int i=0; i<N; i++)
    {      
      (*(S+i)).Probt = *(Probt_NEW+i)/Norm_P; 
    };  
  free(Probt_NEW);
};

void Print_Matrix(double *Mat, int N)
{
  printf("#---------------------------------\n");	
  for (int i=0; i<N; i++)
    {
      printf("\n");
      for (int j=0; j<N; j++)
	{
	  printf("%.3lf\t", *(Mat+i*N+j));
	};
      printf("\n");
    };
  printf("#---------------------------------\n");
};

void Print_Prob (POINT *S, int N)
{
  printf("#---------------------------------\n");
  for (int i=0; i<N; i++)
    {
      printf("# P_%d = %.11lf\n", i+1, (*(S+i)).Probt);
    };
  printf("#---------------------------------\n");
};
