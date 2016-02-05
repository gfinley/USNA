#include <stdio.h>
#include <time.h>
#include "mpi.h"
#include <math.h>
#include <stdlib.h>

int main(int argc, char *argv[] ){
	
	int rank;
	int numprocs;
	int number;

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	MPI_Status stat;

	if( rank ==0){
		printf("sending a message to rank 3\n");
		int numbertosend = 3;
		MPI_Send(&numbertosend, 1, MPI_INT, 3, 0, MPI_COMM_WORLD);
	}

	while(1){
		MPI_Recv(&number, 1, MPI_INT, MPI_ANY_SOURCE , MPI_ANY_TAG , MPI_COMM_WORLD, &stat);
		printf(" I have recieved a message from rank %d Passing now", stat.MPI_SOURCE);
		if(stat.MPI_SOURCE > rank){
			MPI_Send(&numbertosend, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
		}
		if(stat.MPI_SOURCE < rank){
			MPI_Send(&numbertosend, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD);
		}
		if(stat.MPI_SOURCE == rank){
			printf("message made it to destination");
		}
		

	}



	}