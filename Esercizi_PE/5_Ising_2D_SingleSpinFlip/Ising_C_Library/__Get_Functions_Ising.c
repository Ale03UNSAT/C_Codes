//GET-FUNCTIONS

int Get_PhySpin (point *P)
{
  // 0 -> (+); 1 -> (-)
  //spin = 1 -2*eta
  return 1 - 2 * (*P).eta;
};

//DeltaH
Ldouble Get_DeltaH (point *P, stats *S)
{
  //DeltaE viene calcolata prima del SSF per valutare la probabilità di accettazione: in questo caso DE = Ds*(-2*S_i);
  // -1.0 * SpinConfig[NumberOfNeighbors] * ( s_f - s_i ) = -1.0 * SpinConfig[NumberOfNeighbors] * ( (-s_k) - s_k ) = SpinConfig[NumberOfNeighbors] * (2.0) * (S_k)
  //printf("%LF\t", (*( (*S).LUTSpinConfig + (*P).SumOfNeighbors_eta))  * (2.) * (Get_PhySpin(P)));
  return ( (*( (*S).LUTSpinConfig + (*P).SumOfNeighboursEta))  * (2.) * (Get_PhySpin(P)) );  
};
