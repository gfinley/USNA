#include <stdio.h>
#include <time.h>
#include "mpi.h"
#include <math.h>
#include <stdlib.h>

int main(int argc, char *argv[] ){
	
	int rank;
	int numprocs;
	int number;
	int xx;
	
	
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	MPI_Status stat;

	for(xx = 0; xx < 10; xx++){
		int test = rand() % (numprocs); //http://stackoverflow.com/questions/1202687/in-c-how-do-i-get-a-specific-range-of-numbers-from-rand
		printf("%d\n", test);
	}


	if( rank ==0){
		printf("sending a message to rank 3\n");
		int numbertosend = 3;
		MPI_Send(&numbertosend, 1, MPI_INT, 3, 1, MPI_COMM_WORLD);
	}

	while(1){

		MPI_Recv(&number, 1, MPI_INT, MPI_ANY_SOURCE , MPI_ANY_TAG , MPI_COMM_WORLD, &stat);
		printf(" I have recieved a message from rank %d Passing now\n", stat.MPI_SOURCE);
		if(stat.MPI_SOURCE > stat.MPI_TAG){
			MPI_Send(&number, 1, MPI_INT, rank+1, stat.MPI_TAG, MPI_COMM_WORLD);
		}
		if(stat.MPI_SOURCE < stat.MPI_TAG){
			MPI_Send(&number, 1, MPI_INT, rank-1, stat.MPI_TAG, MPI_COMM_WORLD);
		}
		if(stat.MPI_SOURCE == stat.MPI_TAG){
			printf("message made it to destination");
		}
		

	}

	MPI_Finalize();

	}