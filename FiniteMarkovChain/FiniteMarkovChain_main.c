#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ARGV_LACK_FAILURE      -10
#define NEGATIVE_INPUT_FAILURE -20
#define NOT_BI_FAILURE         -30
#define NOT_PROB_FAILURE       -31

#define N 6
#define EPSILON 1E-20

typedef long double PROB;

struct PointOfTheChain
{
  //Numero identificativo (è utile utilizzare numeri per facilitare i confronti)
  int id;
  //Probabilità del Punto allo stato "t" (Prob(0) = 1./NumberOfPoints)
  PROB Probt;
  PROB P[3];
  //Probabilità asintotica
  PROB Pi;
  
  //Interazioni
  //Numero delle Interazioni tra il punto i e gli altri punti
  int NumberOfLinks;
  //Puntatori della rete di puntatori
  struct PointOfTheChain **PointerArrayToAnotherPoint;
};

typedef struct PointOfTheChain POINT;
typedef long long int BI; //Big Int numbers


//Funzioni Inizializzatrici
void Init_Params (int, char **, BI *, PROB *);

void Init_Points (POINT *); //Inizializza gli elementi della struttura Point
void Create_Links (POINT *, int *); //Creo i link e la matrice u_ij

//Funzioni Principali
void Create_U_ij (POINT *, int *, PROB *); //Funzione calcolatrice della matrice di proposta nel punto i,j
PROB Create_A_ij (POINT *, int, int); //Funzione calcolatrice della matrice di accettazione nel punto i,j
void Create_W_ij (POINT *, PROB *, PROB*, PROB); //Funzione calcolatrice della matrice di transizione w_ij
int  MC_STEP (POINT *, PROB *); //Funzione che esegue un passo della Markov Chain
  
//Funzioni di stampa
void Print_Matrix(PROB *);
void Print_Prob (POINT *);

int main (int argv, char **argc)
{
  BI i, NumberOfSteps;
  int Links[N*N]={0}, cc=0;
  PROB Wij[N*N]={0.}, Uij[N*N]={0.}, w63;
  POINT Set[N];

  //Inizializzazione parametri da riga di comando
  Init_Params(argv, argc, &NumberOfSteps, &w63);
  
  //Inizializzazione dei punti della Catena con assegnazione Probabilità Asitotica
  Init_Points (Set);

  //Creazione delle connessioni del grafo 
  Create_Links (Set, Links);

  //Calcolo U_ij
  Create_U_ij (Set, Links, Uij);
  //Calcolo W_ij
  Create_W_ij (Set, Uij, Wij, w63);

  //Stampa Matrice U_Ij
  //Print_Matrix(Uij);
  //Stampa Matrice W_ij
  //Print_Matrix(Wij);
  
  for (i=0; i<NumberOfSteps; i++)
    {
      unsigned char res=0;
      res = MC_STEP(Set, Wij);
      printf ("#P# %lld \t %Lf \t %Lf \t %Lf \t %Lf \t %Lf \t %Lf \n", i+1, (*(Set+0)).Probt, (*(Set+1)).Probt, (*(Set+2)).Probt, (*(Set+3)).Probt, (*(Set+4)).Probt, (*(Set+5)).Probt);  
      printf ("#PI# %lld \t %Lf \t %Lf \t %Lf \t %Lf \t %Lf \t %Lf\n", i+1, (*(Set+0)).Pi, (*(Set+1)).Pi, (*(Set+2)).Pi, (*(Set+3)).Pi, (*(Set+4)).Pi, (*(Set+5)).Pi);
      printf ("#W63# %lld \t %Lf \t %Lf\n", i+1, (*(Set+2)).Probt, (*(Set+2)).Pi);
      if (res == 1 && cc!=1)
	{
	  cc=1;
	  printf ("#PHASE-END# %Lf \t %lld %Lf \t %Lf\n", w63, i, (*(Set+2)).Probt, (*(Set+2)).Pi);
	};  
    };
  //Print_Matrix(Wij);
  //Print_Prob(Set);
};

void Init_Params (int argv, char **argc, BI *N_Steps, PROB *w63)
{
  if (argv < 3)
    {
      fprintf ( stderr, "# YOU HAVE ENTERED FEW ARGUMENTS.\n");
      fprintf ( stderr, "# Please enter: %s <N_STEPS> <W_63> \n", argc[0]);
      fprintf ( stderr, "- <N_STEPS> (Number of Metropolis Markov Chain Steps)\n");
      fprintf ( stderr, "- <W_63> (Weight in the link 6-->3)\n");
      exit (ARGV_LACK_FAILURE);
    };
  
  //Assegnazione dei parametri di input da terminale                                                                                                                                                                                                                             
  //Assegnazione N_STEPS, con controllo N_mcmc > 10
  *N_Steps = (BI) atoi(argc[1]);
  if ( *N_Steps <= 10)
    {
      if (*N_Steps<0)
	{
	  fprintf ( stderr, "# Please enter a positive number for <N_Steps>. EXITING...\n\a");
	  exit (NEGATIVE_INPUT_FAILURE);
	};
      fprintf ( stderr, "# Plase enter N_Steps > 10. EXITING...\n\a");
      exit (NOT_BI_FAILURE);
    };
  
  //Assegnazione N_int, con controllo N_int > 3
  *w63  = (PROB) atof(argc[2]);
  if ( *w63<=0. || *w63 >= 1.)
    {
      if (*w63<0)
        {
          fprintf ( stderr, "# Please enter a positive number for <W_63>. EXITING...\n\a");
          exit (NEGATIVE_INPUT_FAILURE);
        };
      fprintf ( stderr, "# Plase enter 0 < W_63 < 1. EXITING...\n\a");
      exit (NOT_PROB_FAILURE);
    };
};

void Init_Points (POINT *S)
{
  for (int i=0; i<N; i++)
    {
      //ID
      (*(S+i)).id            = i+1;
      //Prob(0)
      (*(S+i)).Probt         = 1./N;
      //Inizializzazione di NumberOfLinks
      (*(S+i)).NumberOfLinks = 0;
      //PRobabilità Asintotica
      (*(S+0)).Pi            = 0.24795;
      (*(S+1)).Pi            = 0.25195;
      (*(S+2)).Pi            = 0.25995;
      (*(S+3)).Pi            = 0.23995;
      (*(S+4)).Pi            = 0.0001;
      (*(S+5)).Pi            = 0.0001;
    };
};

void Create_Links (POINT *S, int *Links)
{
  //Costruisco la matrice delle connessioni
  //1-4 FC
  for (int i=0; i<4; i++)
    {
      for (int j=0; j<4; j++)
	{
	  *(Links+i*N+j) = 1;
	};
    };
  //5-6 FC
  for (int i=4; i<6; i++)
    {
      for (int j=4; j<6; j++)
	{
	  *(Links+i*N+j) = 1;
	};
    };
  
  //6->3
  *(Links+5*N+2) = 1;
  
  /*METODO PER ELENCAZIONE
  //1
  *(Links+0*N+0)=1;
  *(Links+0*N+1)=1;
  *(Links+0*N+3)=1;
  //2
  *(Links+1*N+1)=1;
  *(Links+1*N+0)=1;
  *(Links+1*N+2)=1;
  //3
  *(Links+2*N+2)=1;
  *(Links+2*N+1)=1;
  *(Links+2*N+3)=1;
  //4
  *(Links+3*N+3)=1;
  *(Links+3*N+0)=1;
  *(Links+3*N+2)=1;
  //5
  *(Links+4*N+4)=1;
  *(Links+4*N+5)=1;
  //6
  *(Links+5*N+5)=1;
  *(Links+5*N+2)=1;
  *(Links+5*N+4)=1;
  
  
  for (int i=0; i<4; i++)
    {
      for (int j=0; j<4; j++)
	{
	  if ( *(Links+i*N+j)==1 )
	    {
	      *(Links+j*N+i)=1;
	    };
	};
    };
  for (int i=4; i<6; i++)
    {
      for (int j=4; j<6; j++)
	{
	  if ( *(Links+i*N+j)==1 )
	    {
	      *(Links+j*N+i)=1;
	    };
	};
    };
  */
  
  //Ciclo Righe
  for (int i=0; i<N; i++)
    {
      //Ciclo Colonne
      for (int j=0; j<N; j++)
	{
	  //Conto il numero delle connessioni del punto i-esimo del grafo
	  (*(S+i)).NumberOfLinks += *(Links+i*N+j);
	};
    };
};
void Create_U_ij (POINT *S, int *L, PROB *U)
{
   for (int i=0; i<N; i++)
    {
      for (int j=0; j<N; j++)
	{
	  //Creo la matrice Uij (Simmetrica)
	  *(U+i*N+j) = 0.5 * ( ((PROB)(*(L+i*N+j)) / (*(S+i)).NumberOfLinks) + ((PROB)(*(L+j*N+i)) / (*(S+j)).NumberOfLinks) );
	};
    };
   
   //OSS: La normalizzazione di Uij NON è necessaria
};

PROB Create_A_ij (POINT *S, int i, int j)
{
  //Algoritmo Metropolis min(1, Pi_j/Pi_i)
  if ( (*(S+j)).Pi >= (*(S+i)).Pi )
    {
      return  ( 1.0 );
    }
  else
    {
      return (*(S+j)).Pi / (*(S+i)).Pi;
    };
};
 
void Create_W_ij (POINT *S, PROB *U, PROB *W, PROB W63)
{
  for (int i=0; i<N; i++)
    {
      for (int j=0; j<N; j++)
	{
	  *(W+i*N+j) =  (*(U+i*N+j)) * Create_A_ij(S, i, j);
	};
    };

  //Aggiungo il peso 63
  *(W+5*N+2) = W63;
  *(W+5*N+4) = 0.5*(1.-W63);
  *(W+5*N+5) = 0.5*(1.-W63);
  //Normalizzazione Righe w_ij
  for (int i=0; i<N; i++)
    {
      //Calcolo costante di normalizzazione
      PROB Norm_W=0.0;
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
 
int  MC_STEP (POINT *S, PROB *W)
{
  //Array per contenere le nuove probabilità allocato dinamicamente
  PROB Probt_NEW[N]={0.0}, Norm_P=0.0;
  
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
  
  //Sostituzione delle probabilità P(t)-->P(t+1) con normalizzaizone 
  for (int i=0; i<N; i++)
    {
      (*(S+i)).Probt = *(Probt_NEW+i) / Norm_P; 
    };

  //Riempimento dell'array P
  for (int i=0; i<N; i++)
    {
      *((*(S+i)).P+0) = *((*(S+i)).P+1);
      *((*(S+i)).P+1) = *((*(S+i)).P+2);
      *((*(S+i)).P+2) = (*(S+i)).Probt;
    };
  //Controllo del transiente
  int transient=0;
  for (int i=0; i<N; i++)
    {
      if ( ( *((*(S+i)).P+0) - *((*(S+i)).P+2) ) < EPSILON  && fabs( (*(S+i)).Probt - (*(S+i)).Pi ) <= 0.05 )
	{
	  //printf("#C1=%Lf \t C2=%Lf\n", ( *((*(S+i)).P+0) - *((*(S+i)).P+2) ) , fabs( (*(S+i)).Probt - (*(S+i)).Pi ) );
	  transient+=1;
	};
    };
  
  if (transient==6)
    {
      return 1;
    }
  else
    {
      return 0;
    };
};

void Print_Matrix(PROB *Mat)
{
  printf("#---------------------------------\n");	
  for (int i=0; i<N; i++)
    {
      printf("\n");
      for (int j=0; j<N; j++)
	{
	  printf("%.11Lf\t", *(Mat+i*N+j));
	};
      printf("\n");
    };
  printf("#---------------------------------\n");
};

void Print_Prob (POINT *S)
{
  printf("#---------------------------------\n");
  for (int i=0; i<N; i++)
    {
      printf("# P[%d] = %.11Lf\n", i+1, (*(S+i)).Probt);
    };
  printf("#---------------------------------\n");
};
