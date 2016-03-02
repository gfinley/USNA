//C hello world example
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <math.h>





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
	for(ii =0; ii<forestSize;ii++){
		for(jj=0; jj< forestSize; jj++){
			forest[ii][jj] = rank;
		}
	}
}
int* getTop(int** forest,int forestSize){
	int ii =0;
	int* topRow = calloc((size_t)forestSize, sizeof(int));
	for(ii=0; ii< forestSize;ii++){
		topRow[0] = forest[0][ii];
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
		bottomRow[0] = forest[0][ii];
	}
	return bottomRow;
}
int** assignBottom(int* bottomRow,int forestSize, int** forest){
	int ii =0;
	for(ii=0; ii< forestSize;ii++){
		forest[forestSize][ii] = bottomRow[ii];
	}
	return forest;
}
int* getLeft(int** forest,int forestSize){
	int ii =0;
	int* leftRow = calloc((size_t)forestSize, sizeof(int));
	for(ii=0; ii< forestSize;ii++){
		leftRow[ii] = forest[ii][0];
	}
	return leftRow;
}
int** assignleft(int* leftRow,int forestSize, int** forest){
	int ii =0;
	for(ii=0; ii< forestSize;ii++){
		forest[ii][0] = leftRow[ii];
	}
	return forest;
}
int* getright(int** forest,int forestSize){
	int ii =0;
	int* rightRow = calloc((size_t)forestSize, sizeof(int));
	for(ii=0; ii< forestSize;ii++){
		rightRow[ii] = forest[ii][forestSize-1];
	}
	return rightRow;
}
int** assignright(int* rightRow,int forestSize, int** forest){
	int ii =0;
	for(ii=0; ii< forestSize;ii++){
		forest[ii][forestSize-1] = rightRow[ii];
	}
	return forest;
}


void haloExchangeStepOne(int rank,int** forest,int forestSize){
	//stage one of Halo exchnage
	//get left columb of 
	int n = sqrt(forestSize);
	int *rightRow = calloc((size_t)forestSize, sizeof(int));

	if(rank ==0){
		rightRow = getright(forest,forestSize);
		MPI_Send(rightRow, forestSize, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
		printf("Node: %d sent to %d\n",rank, rank+1);
	}
	else if(rank % 2 == 0){
		if((rank % n) != 0){
			rightRow = getright(forest,forestSize);
			MPI_Send(rightRow, forestSize, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
			printf("Node: %d sent to %d\n",rank, rank+1);
		}
	}
	else{
		//odd nodes listen left if applicable (all should be)
		MPI_Recv(rightRow, forestSize, MPI_INT, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		forest = assignleft(rightRow,forestSize,forest);
		printf("Node: %d Recieved and saw %d\n", rank, rightRow[0]);
	}
	return;


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
	forest = initForestSection( forestSize, forest);
	fillforest(rank,forest,forestSize);

	haloExchangeStepOne(rank,forest,forestSize);



	MPI_Finalize();
}