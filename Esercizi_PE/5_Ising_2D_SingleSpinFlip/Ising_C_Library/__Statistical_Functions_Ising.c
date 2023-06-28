//Statistical-Functions
Ldouble mean (size_i N, Ldouble X)
{
  return X/((double)N);
}

Ldouble variance (size_i N, Ldouble X, Ldouble X2)
{
  //return sqrt(  1./(double)(N-1.) * (mean(N, X2) - mean(N, X)*mean(N, X)) );
  return mean( N, X2) - mean( N, X)*mean( N, X);
};

Ldouble block_error (PAR P, size_i N, Ldouble X, Ldouble X2)
{
  return sqrt( ((Ldouble)N) / ( ((double)P.MaxTime)/N - 1.) * variance ( P.MaxTime, X, X2) );  
}
