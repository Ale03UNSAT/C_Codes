//Errors
#define ARGV_LACK_FAILURE                      -10
#define NEGATIVE_INPUT_FAILURE                 -11
#define NOT_BI_FAILURE                         -12
#define NEXT_TO_ZERO_KELVIN_VALUE_FAILURE      -13

#define ALLOCATING_FAILURE                     -20
#define LATTICE_START_FAILURE                  -30
#define WRONG_CONST_CHAR_MODE_FAILURE          -40

//Physics Costants
#ifdef __k_b__
#define K_B                       1.380649E-23 
#else
#define K_B                       1.00
#endif

#define TC                        2.268

//Programming Costants
#define Z_VALUE95                 2.58
#define Z_VALUE99                 1.96
#define SEED                      12345679ULL
#define NEXT_TO_ZERO_KELVIN_VALUE 1E-10
#define RIGHT                     0
#define BOTTOM                    1
#define LEFT                      2
#define UP                        3

typedef	signed long long int size_i;
typedef	long double Ldouble;
typedef unsigned char bool;

struct Params
{
  size_i MaxTime;
  size_i NumberOfReps;
  size_i Length;
  Ldouble Temperature; 
};
typedef struct Params PAR;

struct Statistics
{
  //beta = 1/(k_b*T)
  Ldouble beta;
  
  //LookUpTable of the possible spin confingurations in the around of point  P
  // LookUpTable Spin Configuration := Sum_{j in Nieghborhood} ( 1 - 2*s) = NumberOfNeighbors - 2*SumOfNeighbors_eta
  //LookUpTable Spin Configuration (-4 ,-2, 0, 2, 4)
  Ldouble *LUTSpinConfig;

  //Sums of Extensive Observables
  Ldouble E, M;

  //intensive Observables
  Ldouble e, m, e2, m2, m4;
};
typedef struct Statistics stats;

struct Point
{
  size_i position, col, row;
  bool eta; //spin = 1-2eta
  size_i NumberOfNeighbours;
  size_i SumOfNeighboursEta;
  struct Point **Neighbourhood; //Array di puntatori a struttura Neighborhood[NumberOfNeighbors] = pointer to struct Point
};
typedef struct Point point;
