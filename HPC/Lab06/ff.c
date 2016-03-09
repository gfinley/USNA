//C hello world example
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <math.h>


int verificationSum(int** forest, int forestSize, int rank){
	int sum = 0;
	
	//printf("Node %d\n",rank);
	//printf("%d%d%d\n",forest[forestSize-3][forestSize-3],forest[forestSize-3][forestSize-2],forest[forestSize-3][forestSize-1]);
	//printf("%d %d\n",forest[forestSize-2][forestSize-3],forest[forestSize-2][forestSize-1]);
	//printf("%d%d%d\n",forest[forestSize-1][forestSize-3],forest[forestSize-1][forestSize-2],forest[forestSize-1][forestSize-1]);

	sum = sum + forest[forestSize-3][forestSize-3];
	sum = sum + forest[forestSize-3][forestSize-1];
	sum = sum + forest[forestSize-3][forestSize-2];
	sum = sum + forest[forestSize-2][forestSize-1];
	sum = sum + forest[forestSize-2][forestSize-3];
	sum = sum + forest[forestSize-1][forestSize-3];
	sum = sum + forest[forestSize-1][forestSize-2];
	sum = sum + forest[forestSize-1][forestSize-1];
	sum = sum + forest[forestSize-2][forestSize-2];




	return sum;
}


int** initForestSection(int forestSize, int** forest){
		int i;
		forest = calloc((size_t)forestSize, sizeof(int *));
		for (i=0; i < forestSize; i++) {
			forest[i] = calloc((size_t)forestSize, sizeof(int));
			//printf("%d",i);
		}
		return forest;
}
void fillforest(int rank, int** forest, int forestSize){
	int ii,jj;
	for(ii =1; ii<forestSize-1;ii++){
		for(jj=1; jj< forestSize-1; jj++){
			forest[ii][jj] = rank;
		}
	}
}
int* getTop(int** forest,int forestSize){
	int ii =0;
	int* topRow = calloc((size_t)forestSize, sizeof(int));
	for(ii=0; ii< forestSize;ii++){
		topRow[ii] = forest[1][ii];
	}
	return topRow;
}
int** assignTop(int* topRow,int forestSize, int** forest){
	int ii =0;
	for(ii=0; ii< forestSize;ii++){
		forest[0][ii] = topRow[ii];
	}
	return forest;
}
int* getBottom(int** forest,int forestSize){
	int ii =0;
	int* bottomRow = calloc((size_t)forestSize, sizeof(int));
	for(ii=0; ii< forestSize;ii++){
		bottomRow[ii] = forest[forestSize-2][ii];
	}
	return bottomRow;
}
int** assignBottom(int* bottomRow,int forestSize, int** forest){
	int ii =0;
	for(ii=0; ii< forestSize;ii++){
		forest[forestSize-1][ii] = bottomRow[ii];
	}
	return forest;
}
int* getLeft(int** forest,int forestSize){
	int ii =0;
	int* leftRow = calloc((size_t)forestSize, sizeof(int));
	for(ii=0; ii< forestSize;ii++){
		leftRow[ii] = forest[ii][1];
	}
	return leftRow;
}
int** assignLeft(int* leftRow,int forestSize, int** forest){
	int ii =0;
	for(ii=0; ii< forestSize;ii++){
		forest[ii][0] = leftRow[ii];
	}
	return forest;
}
int* getRight(int** forest,int forestSize){
	int ii =0;
	int* rightRow = calloc((size_t)forestSize, sizeof(int));
	for(ii=0; ii< forestSize;ii++){
		rightRow[ii] = forest[ii][forestSize-2];
	}
	return rightRow;
}
int** assignRight(int* rightRow,int forestSize, int** forest){
	int ii =0;
	for(ii=0; ii< forestSize;ii++){
		forest[ii][forestSize-1] = rightRow[ii];
	}
	return forest;
}
int passLeft(int rank, int n){
	if(rank % n ==0){
		return 0;
	}
	else{
		return 1;
	}
}
int passRight(int rank, int n){
	if(rank %n == (n-1)){
		return 0;
	}
	else{
		return 1;
	}
}
int passUp(int rank, int n){
	if(rank - n < 0){
		return 0;
	}
	else{
		return 1;
	}
}
int passDown(int rank, int n,int  worldsize){
	if(rank+n >= worldsize ){
		return 0;
	}
	else{
		return 1;
	}
}
void haloExchangeStepOne(int rank,int** forest,int forestSize,int n){
	//stage one of Halo exchnage
	//get left columb of 
	MPI_Status stat;
	int *tempRow = calloc((size_t)forestSize, sizeof(int));
	//Get passers I.E. 
	if((rank &1) ==0 && (rank %n != n-1)){
		//printf("Node %d readt to send\n",rank);
		tempRow = getRight(forest,forestSize);
		MPI_Send(tempRow, forestSize, MPI_INT, (rank+1), 0, MPI_COMM_WORLD);
		MPI_Recv(tempRow, forestSize, MPI_INT, (rank+1), MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
		forest = assignRight(tempRow,forestSize,forest);

	}
	if((rank &1) ==1 && (rank %n != 0)){
		//printf("node %d ready to recieve \n",rank);
		MPI_Recv(tempRow, forestSize, MPI_INT, rank-1, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
		forest = assignLeft(tempRow,forestSize,forest);
		tempRow = getLeft(forest,forestSize);
		MPI_Send(tempRow, forestSize, MPI_INT, rank-1, 0, MPI_COMM_WORLD);

	}
}
void haloExchangeStepTwo(int rank,int** forest,int forestSize,int n){
	//stage one of Halo exchnage
	//get left columb of 
	MPI_Status stat;
	int *tempRow = calloc((size_t)forestSize, sizeof(int));
	//Get passers I.E. 
	if((rank &1) ==0 && (rank % n != 0)){
		//printf("Node %d readt to rcv\n",rank);
		MPI_Recv(tempRow, forestSize, MPI_INT, rank-1, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
		forest = assignLeft(tempRow,forestSize,forest);
		tempRow = getLeft(forest,forestSize);
		MPI_Send(tempRow, forestSize, MPI_INT, rank-1%n, 0, MPI_COMM_WORLD);


	}
	if((rank%2 == 1) && (rank%n != n-1)) {
		//printf("node %d ready to send \n",rank);
		tempRow = getRight(forest,forestSize);
		MPI_Send(tempRow, forestSize, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
		MPI_Recv(tempRow, forestSize, MPI_INT, rank+1, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
		forest = assignRight(tempRow,forestSize,forest);
	}
}
void haloExchangeStepThree(int rank,int** forest,int forestSize,int n){
	//stage one of Halo exchnage
	//get left columb of 
	MPI_Status stat;
	int *tempRow = calloc((size_t)forestSize, sizeof(int));
	//Get passers I.E. 
	if(((rank/n)&1) ==1 && (rank-n >= 0)){
		//printf("Node %d readt to rcv\n",rank);
		MPI_Recv(tempRow, forestSize, MPI_INT, rank-n, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
		forest = assignTop(tempRow,forestSize,forest);
		tempRow = getTop(forest,forestSize);
		MPI_Send(tempRow, forestSize, MPI_INT, rank-n, 0, MPI_COMM_WORLD);
		//printf("Node %d finished\n",rank);


	}
	if(((rank/n)&1) ==0 && (rank+n < n*n)){
		//printf("node %d ready to send \n",rank);
		tempRow = getBottom(forest,forestSize);
		MPI_Send(tempRow, forestSize, MPI_INT, (rank+n), 0, MPI_COMM_WORLD);
		MPI_Recv(tempRow, forestSize, MPI_INT, rank+n, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
		forest = assignBottom(tempRow,forestSize,forest);
		//printf("node %d finished \n",rank);
	}
}
void haloExchangeStepFour(int rank,int** forest,int forestSize,int n){
	//stage one of Halo exchnage
	//get left columb of 
	MPI_Status stat;
	int *tempRow = calloc((size_t)forestSize, sizeof(int));
	//Get passers I.E. 
	if(((rank/n)&1) ==1 && (rank+n < n*n)){
		//printf("node %d ready to send \n",rank);
		tempRow = getBottom(forest,forestSize);
		MPI_Send(tempRow, forestSize, MPI_INT, (rank+n), 0, MPI_COMM_WORLD);
		MPI_Recv(tempRow, forestSize, MPI_INT, rank+n, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
		forest = assignBottom(tempRow,forestSize,forest);
	}
	if(((rank/n)&1) ==0 && (rank -n >= 0)){
		//printf("Node %d readt to rcv\n",rank);
		MPI_Recv(tempRow, forestSize, MPI_INT, rank-n, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
		forest = assignTop(tempRow,forestSize,forest);
		tempRow = getTop(forest,forestSize);
		MPI_Send(tempRow, forestSize, MPI_INT, rank-n, 0, MPI_COMM_WORLD);
	}
}
int main(int argc, char *argv[] ) 
{

	//variables to be used for MPI
	int rank, numprocs;
	MPI_Status stat;

	//variables to be used for workspace distrobution
	int forestSize;
	
	//mpi set up
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	

	//get worldsize
	forestSize = (atoi(argv[1])/ (sqrt(numprocs)))+2;	
	int **forest;
	int n = sqrt(numprocs);
	forest = initForestSection( forestSize, forest);
	fillforest(rank,forest,forestSize);

	haloExchangeStepOne(rank,forest,forestSize,n);
	haloExchangeStepTwo(rank,forest,forestSize,n);
	haloExchangeStepThree(rank,forest,forestSize,n);
	haloExchangeStepFour(rank,forest,forestSize,n);

	int sum = verificationSum(forest,forestSize,rank);
	printf("Node %d  Sum:%d\n",rank,sum);

	MPI_Finalize();
}