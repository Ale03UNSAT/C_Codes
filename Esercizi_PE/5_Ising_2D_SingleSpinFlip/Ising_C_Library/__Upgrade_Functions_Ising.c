//UPGRADE-FUNCTIONS
void Upgrade_SumOfNeighboursEta (point *P, size_i N, const char *c)
{
  if (!strcmp(c, "HyperCube"))
    {
      size_i pos = (*P).position;
      size_i R  = (*P).row*N           + ((*P).col + 1)%N;
      size_i L  = (*P).row*N           + ((*P).col + N - 1)%N;
      size_i B  = (((*P).row+1)%N)*N   + (*P).col;
      size_i U  = (((*P).row+N-1)%N)*N + (*P).col;

      //printf ("#%lld \t %lld \t %lld \t %lld \t %lld \t %lld \t %lld \t %lld \n", R, (*(*((*P).Neighborhood+RIGHT))).position, B, (*(*((*P).Neighborhood+BOTTOM))).position, L, (*(*((*P).Neighborhood+LEFT))).position, U, (*(*((*P).Neighborhood+UP))).position );
      //Se abbiamo cambiato eta da 0 a 1, allora è sufficiente aggiungere 
      if ( (*P).eta == 1 )
	{
	  (*(P-pos+R)).SumOfNeighboursEta += 1;
	  (*(P-pos+L)).SumOfNeighboursEta += 1;
	  (*(P-pos+U)).SumOfNeighboursEta += 1;
	  (*(P-pos+B)).SumOfNeighboursEta += 1;
	}
      //Altrimenti è sufficiente sottrarlo
      else
	{
	  (*(P-pos+R)).SumOfNeighboursEta -= 1;
	  (*(P-pos+L)).SumOfNeighboursEta -= 1;
	  (*(P-pos+U)).SumOfNeighboursEta -= 1;
	  (*(P-pos+B)).SumOfNeighboursEta -= 1;
	};
    };
};

void Upgrade_ExtensiveObs (point *P, stats *S, size_i site)
{
  //Il meno va messo perhè lo spin è cambiato e quindi la funzione ritornerebbe -DE
  if ( site >= 0 )
    {
      (*S).E  -= Get_DeltaH( P+site, S);
      (*S).M  += 2*Get_PhySpin(P+site);
    }
};

void Upgrade_IntensiveObs (stats *S, size_i N)
{
  //Somma delle osservabili intensive  per la statistica
  (*S).e  += (*S).E/(N*N);
  (*S).m  += (*S).M/(N*N);
  (*S).e2 += ((*S).E)*((*S).E)/(N*N*N*N);
  (*S).m2 += ((*S).M)*((*S).M)/(N*N*N*N);
  (*S).m4 += pow((*S).M/(N*N), 4);
};

