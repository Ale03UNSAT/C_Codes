//ALLOCATE-FUNCTIONS
point* Allocate_calloc_point( size_i N)
{
  point *temp;
  temp = (point *) calloc ( N, sizeof(point));
  if (temp==NULL)
    {
      fprintf ( stderr, "# ERRROR OCCURRED IN: temp = (point *) calloc( %lld, sizeof(point));. EXITING...\n\a", N);
      exit(ALLOCATING_FAILURE);
    };
  return temp;
};

point** Allocate_calloc_PointerArray_to_point( size_i N)
{
  point **temp;
  temp = (point **) malloc( N*sizeof(point *));

  if (temp==NULL)
    {
      fprintf ( stderr, "# ERRROR OCCURRED IN: temp = (point **) calloc( %lld, sizeof(point *));. EXITING...\n\a", N);
      exit(ALLOCATING_FAILURE);
    };

  //Alloco ogni puntatore dell'array di puntatori
  for (size_i i=0; i<N; i++)
    {
      *(temp+i) = Allocate_calloc_point(1);
    };
  
  return temp;
};

Ldouble* Allocate_calloc_Ldouble( size_i N)
{
  Ldouble *temp;
  temp = (Ldouble *) calloc( N, sizeof(Ldouble));
  if (temp==NULL)
    {
      fprintf ( stderr, "# ERRROR OCCURRED IN: temp = (Ldouble *) calloc( %lld, sizeof(Ldouble));. EXITING...\n\a", N);
      exit(ALLOCATING_FAILURE);
    };
  return temp;
};

Ldouble* Allocate_malloc_Ldouble( size_i N)
{
  Ldouble *temp;
  temp = (Ldouble *) malloc( N * sizeof(Ldouble));
  if (temp==NULL)
    {
      fprintf ( stderr, "# ERRROR OCCURRED IN: temp = (Ldouble *) malloc( %lld * sizeof(Ldouble));. EXITING...\n\a", N);
      exit(ALLOCATING_FAILURE);
    };
  return temp;
};
