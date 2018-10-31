
struct blockHead {
  double block;
  int index;
}

int compare( const void* a, const void* b){
  return ((struct blockHead*) b)->block - ((struct blockHead*) a)->block;
}

int main(){
  int M=5;
  int N=2;
  int i, j, count, ii;
  double deconv[N][M] = {
    {0.0, 3.2, 7.5, 0.0, 1.8},
    {0.0, 0.0, 4.2, 8.5, 3.6}
  }
  double *A = (double *) malloc(M * N * sizeof(double));
  struct blockHead* block = (struct blockHead*) malloc(M * sizeof(struct blockHead));
  for(i = 0; i < M; i++){
    block[i].block = deconv[1][i];
    block[i].index = i;
  }
  qsort(block, M, sizeof(struct blockHead), compare);

  count = 0;
  while(A[count] > 0){
    A[count] = block[count].block;
    count++;
  }
  for(i = 0; i < count; i++)
    A[M+i] = deconv[2][block[i].index];

  for(i = 0; i < count; i++){
    if(A[M+i] == 0.0){
      ii=i;
      while(A[M+ii] > 0.0 && ii >= 0) ii--; //left
      dl = A[ii + 1] - A[i];
      ii=i;
      while(A[M+ii] > 0.0 && ii < M) ii++; //right
      dr = A[ii + 1] - A[i];
    }else{

    }
  }
}
