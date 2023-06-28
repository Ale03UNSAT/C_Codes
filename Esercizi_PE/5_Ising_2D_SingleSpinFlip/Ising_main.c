#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef __RandomSrandInit__
#include <time.h>
#endif

#include "Ising_C_Library/__StructsAndDefs_Ising.c"
#include "Ising_C_Library/__Allocate_Functions_Ising.c"
#include "Ising_C_Library/__Get_Functions_Ising.c"
#include "Ising_C_Library/__Upgrade_Functions_Ising.c"
#include "Ising_C_Library/__Init_Functions_Ising.c"
#include "Ising_C_Library/__Statistical_Functions_Ising.c"

#include "Ising_C_Library/__Print_Functions_Ising.c"

//INIT-Functions
void Init_Params (int, char **, PAR *);
void Init_Lattice (point *, size_i, const char *, const char *);
void Init_Neighbourhood (point *, size_i, const char *);
void Init_Phi (stats *, size_i, size_i);
void Init_Stats_ExtensiveObs (point *, PAR, stats *);
void Init_Stats (point *, PAR, stats *);
//ALLOCATE-Functions
point* Allocate_calloc_point (size_i);
point** Allocate_calloc_PointerArray_to_point (size_i);
Ldouble* Allocate_calloc_Ldouble (size_i);
Ldouble* Allocate_malloc_Ldouble (size_i);
//SAVE-Functions
void Copy_Lattice (point *, point *, size_i, const char *); 
//UPGRADE-Functions
void Upgrade_SumOfNeighboursEta (point *, size_i, const char *);
void Upgrade_ExtensiveObs (point *, stats *, size_i);
void Upgrade_IntensiveObs (stats *, size_i);
//GET-Functions
int Get_PhySpin (point *);
Ldouble Get_DeltaE (point *, stats *);

//ISING
Ldouble A_ij (point *, stats *);
size_i SingleSpinFlip_MetropolisMCS (point *, stats *, PAR);
bool EndOf_TransientM (stats *, size_i);
bool EndOf_TransientE (stats *, size_i);

//STATISTICAL-Functions
Ldouble mean (size_i, Ldouble);
Ldouble vriance (size_i, Ldouble, Ldouble);
Ldouble block_error (PAR, size_i, Ldouble, Ldouble);

//PRINT-Functions
void Print_LatticeSpin (point *, size_i); 
void Print_SumOfNeighbours (point *);


int main (int argv, char **argc)
{
  //Inizializzazione srand48()
#ifdef __RandomSrandInit__
  srand48(time(NULL));
#else
  srand48(SEED);
#endif 
  //*****************************************************EXT 
#ifdef __Int__
  PAR P;
  point *lattice;
  stats Lab;
  size_i t=0, next_meastime=1, prev_meastime=0;

  //Assegnazioni elementi di P da terminale 
  Init_Params (argv, argc, &P);
  //Allocazione del reticolo
  lattice        = Allocate_calloc_point ( P.Length*P.Length );
  //Asegnazione elementi di lattice (eta, position, col, row, Neighborhood)
#if defined ( __RandomStart__ )
  Init_Lattice (lattice, P.Length, "HyperCube", "RandomStart");
#elif defined ( __PositiveStart__ )
  Init_Lattice (lattice, P.Length, "HyperCube", "PositiveStart");
#elif defined ( __NegativeStart__ )
  Init_Lattice (lattice, P.Length, "HyperCube", "NegativeStart");
#else
  fprintf ( stderr, "Error in Init_Lattice\n.Exiting..\n\a");
  exit(LATTICE_START_FAILURE);
#endif  //Inizialzzazione elementi di Lab (beta, E, M, Look Up Table Spin Configuration)
  Init_Stats (lattice, P, &Lab);
  
  //Evoluzione temporale
  //Aggiornamento nuovo tempo sulla scala logaritmica
  printf ("#N# # t-STEP \t e \t m\n");
  printf ("#N# %lld \t %Lf \t %Lf\n", t, Lab.E/(P.Length*P.Length), Lab.M/(P.Length*P.Length));

  for (t=1; t<=P.MaxTime; t++)
    {
      size_i flipped_spot;
      //SPIN-FLIP con riassegnazione del vicinato
      flipped_spot = SingleSpinFlip_MetropolisMCS (lattice, &Lab, P);
      //Aggiornamento delle osservabili estensive E-M
      Upgrade_ExtensiveObs (lattice, &Lab, flipped_spot);

      //Se t è sulla scala dei tempi logaritmici
      if ( t == next_meastime)
	{     
	  //Stampa dei valori delle osservabili intensive e-m
	  printf ("#N# %lld \t %Lf \t %Lf\n", t, Lab.E/(P.Length*P.Length), Lab.M/(P.Length*P.Length));

	  //Calcolo medie dei valori delle osservabli intensive e-m tra t_(n-1) e t_(n) (t=meastime)
	  Upgrade_IntensiveObs (&Lab, P.Length);

	  //Stampa dei valori medi delle osservabili intensive e-m tra t_(n-1) e t_(n)-1
	  printf ("#E-M# %lld \t %Lf \t %Lf \t", t, Lab.e/(next_meastime-(prev_meastime-1)), Lab.m/(next_meastime-(prev_meastime-1)) );
	  //Stampa degli errrori sui valori medi delle osservabili intensive e-m tra t_(n-1) e t_(n)-1
	  printf (" %Lf \t %Lf\n", block_error(P, next_meastime-(prev_meastime-1), Lab.e, Lab.e2), block_error(P, next_meastime-(prev_meastime-1), Lab.m, Lab.m2) );
	  
	  //Aggiornamento nuovo tempo sulla scala logaritmica
	  next_meastime = (size_i)(t*1.1+1.0);
	  //Azzeramento osseravbili estensive 
	  Init_Stats_ExtensiveObs (lattice, P, &Lab);
	  //Aggiornamento del vecchio tempo di misura sulla scala logaritmica
	  prev_meastime = t;
	};
    };
  //Print_LatticeSpin (lattice, P.Length);
  free(lattice);
#endif
  //*****************************************************TRANSIENT-PHASE 
#ifdef __TransientStudy__
  PAR P;
  point *latticeP, *latticeN, *latticeR;
  stats LabP, LabN, LabR;
  size_i t=0;
#ifdef __Print_em__    
  size_i next_meastime=1;
#endif
  bool __EndOfTransientPhaseM__=0, __EndOfTransientPhaseE__=0;

  //Assegnazioni elementi di P da terminale 
  Init_Params (argv, argc, &P);
  //Allocazione dei 3 reticoli
  latticeP        = Allocate_calloc_point ( P.Length*P.Length );
  latticeN        = Allocate_calloc_point ( P.Length*P.Length );
  latticeR        = Allocate_calloc_point ( P.Length*P.Length );

  //Asegnazione elementi di lattice (eta, position, col, row, Neighborhood)
  Init_Lattice (latticeP, P.Length, "HyperCube", "PositiveStart");
  Init_Lattice (latticeN, P.Length, "HyperCube", "NegativeStart"); 
  Init_Lattice (latticeR, P.Length, "HyperCube", "RandomStart");
  
  //Inizialzzazione elementi di Lab (beta, E, M, Look Up Table Spin Configuration)
  Init_Stats (latticeP, P, &LabP);
  Init_Stats (latticeN, P, &LabN);
  Init_Stats (latticeR, P, &LabR);

#ifdef __Print_em__
  //Evoluzione temporale
  //Aggiornamento nuovo tempo sulla scala logaritmica
  printf ("#N# # t-STEP \t e \t m\n");
  printf ("#N# %lld \t %Lf \t %Lf \t %Lf \t %Lf \t %Lf \t %Lf\n", t, LabP.E/(P.Length*P.Length), LabP.M/(P.Length*P.Length), LabN.E/(P.Length*P.Length), LabN.M/(P.Length*P.Length), LabR.E/(P.Length*P.Length), LabR.M/(P.Length*P.Length));
#endif

  for (t=1; t<=P.MaxTime; t++)
    {
      size_i flipped_spotP, flipped_spotN, flipped_spotR;
      //SPIN-FLIP con riassegnazione del vicinato
      flipped_spotP = SingleSpinFlip_MetropolisMCS (latticeP, &LabP, P);
      //Aggiornamento delle osservabili estensive E-M
      Upgrade_ExtensiveObs (latticeP, &LabP, flipped_spotP);
      
      //Le altre traiettorie vengono calcolate solo se il sistema è in fase transiente se non è definita l'opzione __Print_em__
#ifndef __Print_em__
      if ( __EndOfTransientPhaseE__ != 1 || __EndOfTransientPhaseM__ != 1)
	{
	  flipped_spotN = SingleSpinFlip_MetropolisMCS (latticeN, &LabN, P);
	  flipped_spotR = SingleSpinFlip_MetropolisMCS (latticeR, &LabR, P);

	  Upgrade_ExtensiveObs (latticeN, &LabN, flipped_spotN);
	  Upgrade_ExtensiveObs (latticeR, &LabR, flipped_spotR);
	};

      if ( __EndOfTransientPhaseE__ != 1 )
	{
	  if ( fabs(LabN.E-LabP.E)<=0.05*fabs(LabP.E) && fabs(LabR.E-LabP.E)<=0.05*fabs(LabP.E) ) 
	    {
	      __EndOfTransientPhaseE__ = 1;
	      printf ("#TPE# %Lf \t %lld\n", P.Temperature, t);
	    };
	};
      if ( __EndOfTransientPhaseM__ != 1 )
	{
	  if ( fabs(LabN.M-LabP.M)<=0.05*fabs(LabP.M) && fabs(LabR.M-LabP.M)<=0.05*fabs(LabP.M) && fabs(LabR.M-LabN.M)<=0.05*fabs(LabP.M) )
	  //if ( fabs(LabR.M-LabP.M)<=0.1*fabs(LabP.M) && fabs(LabN.M-LabP.M)<=0.1*fabs(LabP.M) && fabs(LabR.M-LabN.M)<=0.1*fabs(LabN.M) )
	  //if ( fabs(LabR.M-LabP.M)<=0.05*fabs(LabP.M) && fabs(LabN.M-LabP.M)<=0.05*fabs(LabP.M)  ) 
	    {
	      __EndOfTransientPhaseM__ = 1;
	      printf ("#TPM# %Lf \t %lld\n", P.Temperature, t);
	    };
	};
      if (__EndOfTransientPhaseM__ == 1 && __EndOfTransientPhaseE__ == 1)
	{
	  //Esci dal programma se tutte le osservabili sono fuori il transiente
	  fprintf (stdout, "USCITO A %lld\n", t); 
	  exit(-5);
	};
#endif
      
      //Le altre traiettorie vengono calcolate sempre e stampate solo per t logaritmici se è definita l'opzione __Print_em__
#ifdef __Print_em__

      flipped_spotN = SingleSpinFlip_MetropolisMCS (latticeN, &LabN, P);
      flipped_spotR = SingleSpinFlip_MetropolisMCS (latticeR, &LabR, P);
      
      Upgrade_ExtensiveObs (latticeN, &LabN, flipped_spotN);
      Upgrade_ExtensiveObs (latticeR, &LabR, flipped_spotR);

      if ( t == next_meastime)
	{
	  //Stampa dei valori delle osservabili intensive e-m
	  printf ("#N# %lld \t %Lf \t %Lf \t %Lf \t %Lf \t %Lf \t %Lf\n", t, LabP.E/(P.Length*P.Length), LabP.M/(P.Length*P.Length), LabN.E/(P.Length*P.Length), LabN.M/(P.Length*P.Length), LabR.E/(P.Length*P.Length), LabR.M/(P.Length*P.Length));
	  
	  //Aggiornamento nuovo tempo sulla scala logaritmica
	  next_meastime = (size_i)(t*1.1+1.0);
	};      
#endif
    };
#ifndef __Print_em__
  if (__EndOfTransientPhaseM__ == 0)
    {
      printf ("#TPM# %Lf \t %lld\n", P.Temperature, t); //1./0);
    };
  if (__EndOfTransientPhaseE__ == 0)
    {
      printf ("#TPE# %Lf \t %lld\n", P.Temperature, t);//1./0);
    };
#endif
  //Print_LatticeSpin (lattice, P.Length);
  free(latticeP);
  free(latticeN);
  free(latticeR);
#endif
  //*****************************************************BINDER 
#ifdef __Binder__
  PAR P;
  point *latticeP, *latticeN, *latticeR;
  stats LabP, LabN, LabR;
  size_i t=0, rep, t_eqM=0;
  bool __EndOfTransientPhaseM__=0;
  Ldouble SumOfBinder=0.0;

  //Assegnazioni elementi di P da terminale 
  Init_Params (argv, argc, &P);
  
  for (rep=0; rep<P.NumberOfReps; rep++)
    {
      t_eqM = 0;
      __EndOfTransientPhaseM__ = 0;

      //Allocazione dei 3 reticoli
      latticeP        = Allocate_calloc_point ( P.Length*P.Length );
      latticeN        = Allocate_calloc_point ( P.Length*P.Length );
      latticeR        = Allocate_calloc_point ( P.Length*P.Length );
      
      //Asegnazione elementi di lattice (eta, position, col, row, Neighborhood)
      Init_Lattice (latticeP, P.Length, "HyperCube", "PositiveStart");
      Init_Lattice (latticeN, P.Length, "HyperCube", "NegativeStart");
      Init_Lattice (latticeR, P.Length, "HyperCube", "RandomStart"); 
      
      //Inizialzzazione elementi di Lab (beta, E, M, Look Up Table Spin Configuration)
      Init_Stats (latticeP, P, &LabP);
      Init_Stats (latticeN, P, &LabN);
      Init_Stats (latticeR, P, &LabR);
      
      
      for (t=1; t<=P.MaxTime; t++)
	{
	  size_i flipped_spotP, flipped_spotN, flipped_spotR;
	  //SPIN-FLIP con riassegnazione del vicinato
	  flipped_spotP = SingleSpinFlip_MetropolisMCS (latticeP, &LabP, P);
	  //Aggiornamento delle osservabili estensive E-M
	  Upgrade_ExtensiveObs (latticeP, &LabP, flipped_spotP);
	  
	  //forzo la fine della fase transiente a Tmax/2
	  if (t==P.MaxTime/2 && __EndOfTransientPhaseM__ != 1)
	    {
	      __EndOfTransientPhaseM__ = 1;
	      t_eqM=t;
	    };
	  //Studio transiente classico
	  if ( __EndOfTransientPhaseM__ != 1 )
	    {
	      flipped_spotN = SingleSpinFlip_MetropolisMCS (latticeN, &LabN, P);
	      flipped_spotR = SingleSpinFlip_MetropolisMCS (latticeR, &LabR, P);
	      
	      Upgrade_ExtensiveObs (latticeN, &LabN, flipped_spotN);
	      Upgrade_ExtensiveObs (latticeR, &LabR, flipped_spotR);
	      
	      if ( fabs(LabN.M-LabP.M)<=0.05*fabs(LabP.M) && fabs(LabR.M-LabP.M)<=0.05*fabs(LabP.M) && fabs(LabR.M-LabN.M)<=0.05*fabs(LabP.M) )
		{
		  __EndOfTransientPhaseM__ = 1;
		  t_eqM=t;
		  //Azzeramento osseravbili estensive 
		  Init_Stats_ExtensiveObs (latticeP, P, &LabP);
		};
	    };
	  
	  if ( __EndOfTransientPhaseM__ == 1)
	    {
	      LabP.m2 += (LabP.M)*(LabP.M)/(P.Length*P.Length*P.Length*P.Length);
	      LabP.m4 += pow(LabP.M/(P.Length*P.Length), 4);
	    };
	};
      SumOfBinder+=0.5*(3.-(P.MaxTime-t_eqM)*LabP.m4/(LabP.m2*LabP.m2) );
    };
  printf ("#B# %Lf \t %Lf \t %lld \t %lld\n", P.Temperature, SumOfBinder/P.NumberOfReps, (P.MaxTime-t_eqM), t_eqM );
  
  //Print_LatticeSpin (lattice, P.Length);
  free(latticeP);
  free(latticeN);
  free(latticeR);
#endif
};

//-------------------------------------------------ISING
//A_ij
Ldouble A_ij (point *P, stats *S)
{
  // min (1, exp(-beta*dE) ) -->
  // dE <  0 :=> 1             
  // dE >= 0 :=> exp(-beta*dE) 
  // Le relazioni simboleggiano che: il sistema evolve spontaneamente verso il minimo dell'hamiltoniana
  // ma può evolvere in configurazioni che ne aumentano il valore
  
  if(  Get_DeltaH(P,S) >= 0 )
    {
      return ( exp( -(*S).beta * Get_DeltaH(P,S) ) );
    }
  else
    {
      return ( 1.0 );
    };
};

//SSF-Funtion
size_i SingleSpinFlip_MetropolisMCS (point *L, stats *S, PAR P)
{
  //ELezione random del sito k in cui eseguire il SSF s_k-->-s_k
  size_i selected_spot = (size_i) (P.Length*P.Length*drand48());
  
  //SSF
  Ldouble prob_spinflip = drand48();
  if ( prob_spinflip <= A_ij (L+selected_spot, S) )
    {
      //SpinFLip
      (*(L+selected_spot)).eta = !((*(L+selected_spot)).eta);
      //Riassegnazione somma degli spin eta del vicinato ai vicini del punto
      Upgrade_SumOfNeighboursEta(L+selected_spot, P.Length, "HyperCube");
      return selected_spot;
    }
  else
    {
      return -1;
    };
};
/*
//printf("SOMME ETA PRIMA = %lld \t%lld \t%d \t%d \n", (*(*(*(L+selected_site)).Neighborhood+0)).SumOfNeighbors_eta, (*(*(*(L+selected_site)).Neighborhood+1)).SumOfNeighbors_eta, (*(*(*(L+selected_site)).Neighborhood+2)).SumOfNeighbors_eta, (*(*(*(L+selected_site)).Neighborhood+3)).SumOfNeighbors_eta) ;
//printf("\nSOMME ETA DOPO = %lld \t%lld \t%d \t%d \n\n", (*(*(*(L+selected_site)).Neighborhood+0)).SumOfNeighbors_eta, (*(*(*(L+selected_site)).Neighborhood+1)).SumOfNeighbors_eta, (*(*(*(L+selected_site)).Neighborhood+2)).SumOfNeighbors_eta, (*(*(*(L+selected_site)).Neighborhood+3)).SumOfNeighbors_eta) ;
*/

 /*

#ifdef __Int__
  PAR P;
  point *lattice;
  stats Lab;
  size_i t=0, rep;
  
  //Inizializzazione srand48()
  //srand48(time(NULL));
  srand48(SEED);
  
  //Assegnazioni elementi di P da terminale 
  Init_Params (argv, argc, &P);
     
  for (rep=0; rep<P.NumberOfReps; rep++)
    {
       //Allocazione del reticolo e della copia del reticolo
      lattice        = Allocate_point ( P.Length*P.Length );      
      //Asegnazione elementi di lattice (eta, position, col, row, Neighborhood)
      Init_Lattice (lattice, P.Length, "HyperCube", "PositiveStart");
      //Inizialzzazione elementi di Lab (beta, E, M, Look Up Table Spin Configuration)
      Init_Stats (lattice, P, &Lab);
      
      //Evoluzione temporale
      for (t=1; t<=P.MaxTime; t++)
	{
	  bool EndoOfTransientPhaseM;
	  size_i flipped_spot, t_eq=0;
	  //SPIN-FLIP con riassegnazione del vicinato
	  flipped_spot = SingleSpinFlip_MetropolisMCS (lattice, &Lab, P);
	  //Aggiornamento delle osservabili estensive E-M
	  Upgrade_ExtensiveObs (lattice, &Lab, flipped_spot, t);

	  //Se t è sulla scala dei tempi logaritmici
	  if ( t == next_meastime )
	    {
	      //Aggiornamento nuovo tempo sulla scala logaritmica
	      next_meastime = (size_i)(t*1.1+1);
	     
	      //Azzeramento osseravbili estensive 
	      Init_Stats_ExtensiveObs (lattice, P, &Lab);
	      //Aggiornamento del vecchio tempo di misura sulla scala logaritmica
	      prev_meastime = t;
	    };
   	};
    };
  
  free(lattice);
#endif 
*/
