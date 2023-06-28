//INIT-FUNCTIONS

//##############################################(1)PARAMS
void Init_Params (int argv, char **argc, PAR *P)
{
  if (argv < 5)
    {
      fprintf ( stderr, "# YOU HAVE ENTERED FEW ARGUMENTS.\n");
      fprintf ( stderr, "# Please enter: \n%s <T_MAX> <N_REPS> <LEN> <T>\n", argc[0]);
      fprintf ( stderr, "- <T_MAX> (Latest unit time, or the maximum number of steps,  in the Markov Chain evolution.)\n");
      fprintf ( stderr, "- <N_REPS> (Number of repetitions for the statistical objects)\n");
      fprintf ( stderr, "- <LEN> (Length of the Lattice)\n");
      fprintf ( stderr, "- <T> [K] (Temperature, mesured in Kelvin of the System)\n");
      exit (ARGV_LACK_FAILURE);
    };
  //Assegnazione dei parametri di input da terminale
  //Assegnazione MaxTime, con controllo MaxTima > 10                                                                                                                                                                                                                
  (*P).MaxTime = (size_i) atoi(argc[1]);
  /*PER LA FASE INIZIALE DI SCRITTURA LEVO IL COTROLLO NUMERICO > 10
  if ( (*P).MaxTime <= 10 )
    {
      if ( (*P).MaxTime<0 )
        {
          fprintf (stderr, "# Please enter a positive number for <T_MAX>. EXITING...\n\a");
          exit (NEGATIVE_INPUT_FAILURE);
        };
      fprintf ( stderr, "# Please enter T_MAX > 10. EXITING...\n\a");
      exit (NOT_BI_FAILURE);
    };
  */

  (*P).NumberOfReps = (size_i) atoi(argc[2]);
  //PER LA FASE INIZIALE DI SCRITTURA LEVO IL COTROLLO NUMERICO > 3
  if ( (*P).NumberOfReps <= 3 )
    {
      if ( (*P).NumberOfReps<0 )
        {
          fprintf (stderr, "# Please enter a positive number for <N_REPS>. EXITING...\n\a");
          exit (NEGATIVE_INPUT_FAILURE);
        };
      fprintf ( stderr, "# Please enter N_REPS > 3. EXITING...\n\a");
      exit (NOT_BI_FAILURE);
    };
    
   //Assegnazione Length, con controllo Length > 1
  (*P).Length = (size_i) atoi(argc[3]);
  if ( (*P).Length <= 1 )
    {
      if ( (*P).Length < 0 )
        {
          fprintf ( stderr, "# Please enter a positive input for <LEN>. EXITING...\n\a");
          exit (NEGATIVE_INPUT_FAILURE);
        };
      fprintf ( stderr, "# Please enter LEN > 1. EXITING...\n\a");
      exit (NOT_BI_FAILURE);
    };

  //Assegnazione Length, con controllo T > 1E-10
  (*P).Temperature = (Ldouble) atof(argc[4]);
  if ( (*P).Temperature <= NEXT_TO_ZERO_KELVIN_VALUE)
    {
      if ( (*P).Length < 0)
        {
          fprintf ( stderr, "# Please enter a positive input for <T>. EXITING...\n\a");
          exit (NEGATIVE_INPUT_FAILURE);
        };
      fprintf ( stderr, "# Please enter T > %lf (This program cannot work at 0 K).EXITING...\n\a", NEXT_TO_ZERO_KELVIN_VALUE);
      exit (NEXT_TO_ZERO_KELVIN_VALUE_FAILURE);
    };
};
//##############################################(2)LATTICE
void Init_Neighbourhood (point *L, size_i N, const char *c)
{
  //Inizializzazione del vicinato per ogni punto del reticolo
  if ( !strcmp(c, "HyperCube" ) )
    {
      //Modalità reticolo bidimensionale
      for (size_i i=0; i<N*N; i++)
        {
          //Z = 2D = 4: 0=Right, 1=Bottom, 2=Left, 3=Up
	  //Assegnazione numero di vicini
	  (*(L+i)).NumberOfNeighbours = 4;
          ((*(L+i)).Neighbourhood) = Allocate_calloc_PointerArray_to_point ( 4 );

	  //Con condizioni periodiche al bordo (PBC), associo ad ogni puntatore il vicino del punto P
	  *((*(L+i)).Neighbourhood+RIGHT)  = (L + (*(L+i)).row * N          + ((*(L+i)).col+1)%N       );
	  *((*(L+i)).Neighbourhood+BOTTOM) = (L + (((*(L+i)).row+1)%N)*N    + (*(L+i)).col             );
	  *((*(L+i)).Neighbourhood+LEFT)   = (L + (*(L+i)).row * N          + ((*(L+i)).col + N - 1)%N );
	  *((*(L+i)).Neighbourhood+UP)     = (L + (((*(L+i)).row+N-1)%N)*N  + (*(L+i)).col             );
        };
      
      for (size_i i=0; i<N*N; i++)
	{
	  //Azzero la Somma degli eta dei vicini di L+i
	  (*(L+i)).SumOfNeighboursEta = 0;
	  for (size_i j=0; j<(*(L+i)).NumberOfNeighbours; j++)
	    {
	      //j-esimo vicino di L+i
	      (*(L+i)).SumOfNeighboursEta += (*(*((*(L+i)).Neighbourhood + j))).eta ;
	    };
	};
    }
    //Se è errata la stringa per l'inizializzazione del vicinato
   else
     {
       fprintf ( stderr, "# You have entered a wrong <mode> for:   void Init_Neighbourhood_ (point *, size_i, const char *).\nEXITING...\n\a");
       exit (WRONG_CONST_CHAR_MODE_FAILURE);
     };
};
/*
//Debug Neihborhood concluso 
fprintf (stdout, "#        %lld       \n",  (((*(L+i)).row+N-1)%N)*N  + (*(L+i)).col);
	  fprintf (stdout, "#  %lld <- %lld -> %lld \n", (*(L+i)).row * N          + ((*(L+i)).col + N - 1)%N, i , (*(L+i)).row * N          + ((*(L+i)).col+1)%N  );
	  fprintf (stdout, "#        %lld       \n",  (((*(L+i)).row+1)%N)*N    + (*(L+i)).col);
*/

/*
METODO ALTERNATIVO Neighborhood PBC
*((*(L+i)).Neighborhood+RIGHT)  = (L + ((i%N+N+1)%N)+N*(i/N));
*((*(L+i)).Neighborhood+BOTTOM) = (L + (N*(N+1)+i)%(N*N));
*((*(L+i)).Neighborhood+LEFT)   = (L + ((i%N+N-1)%N)+N*(i/N));
*((*(L+i)).Neighborhood+UP)     = (L + (N*(N-1)+i)%(N*N));
*/

/*
//Debug Neihborhood per metodo alternativo 
fprintf (stdout, "#        %lld       \n",  (N*(N-1)+i)%(N*N));
fprintf (stdout, "#  %lld <- %lld -> %lld \n", ((i%N+N-1)%N)+N*(i/N), i , ((i%N+N+1)%N)+N*(i/N) );
fprintf (stdout, "#        %lld       \n",  (N*(N+1)+i)%(N*N));
*/

void Init_Lattice (point *L, size_i N, const char *c1, const char *c2)
{
   if ( !strcmp(c1, "HyperCube" ) )
     {
        //Inizializzazioni elementi dei punti nel reticolo:
       for (size_i i=0; i<N*N; i++) //Ciclo sui ogni punto del reticolo
	{
	  //(1) posizione 
	  (*(L+i)).position = i;
	  //(2) colonna
	  (*(L+i)).col = i%N;
	  //(3) riga
	  (*(L+i)).row = i/N;
	  //(4) eta: vedere "__StructsAndDefs__Ising.c" per maggiori informazioni
	  if ( !strcmp (c2 ,"RandomStart") )
	    {
	      if ( drand48() <= 0.5 )
		{
		  (*(L+i)).eta = 1;
		}
	      else
		{
		  (*(L+i)).eta = 0;
		};
	    };
	  if  ( !strcmp (c2, "NegativeStart") )
	    {
	      (*(L+i)).eta = 1;
	    };
	  if ( !strcmp (c2, "PositiveStart") )
	    {
	      (*(L+i)).eta = 0;
	    };
	  if ( strcmp (c2, "PositiveStart")!=0 && strcmp (c2, "NegativeStart")!=0 && strcmp (c2, "RandomStart")!=0 ) 
	    {
	      fprintf ( stderr, "# You have entered a wrong <mode> for:   void Init_Lattice (point *, size_i, const char *, const char * <> ).\nEXITING...\n\a");
	      exit (WRONG_CONST_CHAR_MODE_FAILURE);
	    };     
	};
       //(5) Inizializzazione vicinato del reticolo
       Init_Neighbourhood (L, N, c1);
    }
   //Se è errata la stringa per l'inizializzazione del reticolo
   else
     {
       fprintf ( stderr, "# You have entered a wrong <mode> for:   void Init_Lattice (point *, size_i, const char * <>, const char *).\nEXITING...\n\a");
       exit (WRONG_CONST_CHAR_MODE_FAILURE);
     };     
};

//##############################################(3)STATS
void Init_Stats_ExtensiveObs (point *P, PAR Par, stats *S)
{
  //<E> e <M> che verranno aggiornate per ogni tempo 
  (*S).E    = 0.0;
  (*S).M    = 0.0;
  //Sums <e>, <m> e <|m|>
  (*S).e    = 0.0;
  (*S).m    = 0.0;
  (*S).e2   = 0.0;
  (*S).m2   = 0.0;
  (*S).m4   = 0.0;
  
  for (size_i i=0; i<Par.Length*Par.Length; i++)
    {
      (*S).M  += Get_PhySpin ( P+i );
      for (size_i j=0; j<(*(P+i)).NumberOfNeighbours; j++)
	{
	  (*S).E  -= Get_PhySpin ( P+i ) * Get_PhySpin (*((*(P+i)).Neighbourhood+j));
	};
    };
  (*S).E  *= 0.5;   
   //<e>, <m> e <|m|>
   (*S).e   = (*S).E/(Par.Length*Par.Length);
   (*S).m   = (*S).M/(Par.Length*Par.Length);
   (*S).e2  = ((*S).e)*((*S).e);
   (*S).m2  = ((*S).m)*((*S).m);
   (*S).m4  = ((*S).m)*((*S).m)*((*S).m)*((*S).m);
};


void Init_Stats (point *P, PAR Par, stats *S)
{
  //beta
  (*S).beta = 1./(K_B*Par.Temperature);

   //LookUpTable Spin Configuration (4, 2 0, -2, 4)
  (*S).LUTSpinConfig = Allocate_malloc_Ldouble ( (*P).NumberOfNeighbours+1 );
   for (size_i i=0; i<=(*P).NumberOfNeighbours; i++)
    {
      //Look Up Table delle possibili configurazioni di spin nei vicini
      *((*S).LUTSpinConfig+i) = (*P).NumberOfNeighbours - 2*i;
    };
   Init_Stats_ExtensiveObs(P, Par, S);
};
