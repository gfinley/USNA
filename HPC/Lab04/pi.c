//C hello world example
#include <stdio.h>
#include <time.h>
#include "mpi.h"
#include <math.h>
#include <stdlib.h>

double pointCalc(){
	double x = (double)rand() / RAND_MAX;
	double y = (double)rand() / RAND_MAX;
	double distance = x*x + y*y; 
	return distance;
}
int main(int argc, char *argv[] )
{
	int rank;
	int numprocs;
	int i;
	int count;
	double distance;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	int totalCount = 0;
	if(rank == 0){
		int numIterations = atoi(argv[1]);//recive number of iterations
		int numberOfIterationsPerSlave = numIterations/numprocs;
		printf("there will be %d iterations split between %d nodes, with %d iterations per\n", numIterations,numprocs,numberOfIterationsPerSlave);
		int xx = 1;
		for(xx = 1; xx< numprocs;xx++){
			MPI_Send(&numberOfIterationsPerSlave, 1, MPI_INT, xx, 0, MPI_COMM_WORLD);
		}
		for( xx =0; xx < numberOfIterationsPerSlave; xx++){
			distance = pointCalc();
			if(distance <= 1){
				totalCount = totalCount + 1;
			}
		}
		//printf("waiting for slaves\n");
		for(xx = 1; xx< numprocs;xx++){
			MPI_Recv(&count, 1, MPI_INT, xx, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			totalCount = totalCount + count;
			//printf("Master: node %d saw a count of %d\n", xx, count);
		}
		double piApprox = 4 * ((double)totalCount/(double)numIterations);
		printf("totalCount: %d, iterations: %d, Approx = %f \n", totalCount,numIterations,piApprox);
	}
	else{
		int count = 0;
		int number;
		MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); //number of iterations from the command line
		srand(time(NULL) + rank);
		int xx = 0;
		for( xx =0; xx < number; xx++){
			distance = pointCalc();
			if(distance <= 1){
				count++;
			}
		}
		//printf("node: %d saw a count of %d\n", rank, count);
		MPI_Send(&count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		//printf("%f from Mr. %d I saw the number %d \n", r, rank, number);
	}
  MPI_Finalize();
}