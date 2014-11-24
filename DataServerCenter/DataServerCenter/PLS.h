#include   <stdlib.h> 
#include "Matrix.h"
#include <math.h>
CMatrix Pls_nipals(CMatrix &X,CMatrix &Y,CMatrix &W,CMatrix &T,CMatrix &P,CMatrix &Q,const int &A);

bool Pretreat(CMatrix &X,const char* method,CMatrix &para1,CMatrix &para2);

CMatrix Plscvfold(CMatrix X,CMatrix y,int K_fold,const char *method,int Amax=20);

CMatrix BuildMapLabel(CMatrix X);