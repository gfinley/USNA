#include <stdio.h>
#include <time.h>
#include "mpi.h"
#include <math.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char *argv[] ){
 int numprocs;
 int rank;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);

	sleep(rank);
	printf("rank %d: before\n", rank);
	MPI_Barrier(MPI_COMM_WORLD);
	printf("rank %d: after\n", rank);
  MPI_Finalize();
	}