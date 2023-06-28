//PRINT-FUNCTIONS
void Print_LatticeSpin (point *L, size_i N)
{
  for (size_i i=0; i<N; i++)
    {
      printf ("# ");
      for (size_i j=0; j<N; j++)
        {
          printf ("%d", (*(L+i*N+j)).eta );
        };
      printf("\n");
    };
  puts("");
};

void Print_SumOfNeighbours (point *L)
{
  printf("%lld \t%lld \t%lld \t%lld \t %lld \t%lld \t%lld \t%lld \n\n", (*(*((*L).Neighbourhood+RIGHT))).SumOfNeighboursEta, (*(*((*L).Neighbourhood+BOTTOM))).SumOfNeighboursEta, (*(*((*L).Neighbourhood+LEFT))).SumOfNeighboursEta, (*(*((*L).Neighbourhood+UP))).SumOfNeighboursEta, (*(*((*L).Neighbourhood+LEFT))).position, (*(*((*L).Neighbourhood+BOTTOM))).position, (*(*((*L).Neighbourhood+LEFT))).position, (*(*((*L).Neighbourhood+UP))).position) ;
};
