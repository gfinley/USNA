#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"


int main(int argc, char *argv[] ) 
{
	MPI_Status stat;
	int rank, numprocs;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	long *bufr, *bufr2;
	long *finalBuffer;
	int i; // controlling variable
	finalBuffer = calloc(numprocs,sizeof(long));
	long temp = 0;
	if(rank == 0){
	    long value;
	    FILE * fp;
	    bufr=calloc(1200, sizeof(long));
	    //Read in all the numbers to a giant array
	    fp = fopen("1200.nums", "r");
	    for(i=0; i<1200; i++) {
		if (fscanf(fp, "%ld\n", &value) != 1) {
		    fprintf(stderr, "error on input\n");
		}
		bufr[i] = value;
	    }
	    fclose(fp);
	    //send all the numbers out
	    //first send to each node the size of an array it needs to create
	    int sizeOfBufferForEachNode = 1200/numprocs;
	    //printf("root here 1\n");
	    bufr2=calloc(sizeOfBufferForEachNode, sizeof(long));
		MPI_Bcast(&sizeOfBufferForEachNode, 1, MPI_INT, 0, MPI_COMM_WORLD); //send out number of numbers to expect
		MPI_Scatter(bufr, sizeOfBufferForEachNode, MPI_LONG, bufr2, sizeOfBufferForEachNode, MPI_LONG,0,MPI_COMM_WORLD); //actually send out numbers
		for(i =0; i< sizeOfBufferForEachNode;i++){
			temp = (bufr2[i] * bufr2[i]) + temp; // get the sum of all numbers squared
		}
	}
	else{ //all the not 0 ranks will rcv the size of the buffer they need to allocate
		int sizeOfBufferForEachNode;
		MPI_Bcast(&sizeOfBufferForEachNode, 1, MPI_INT, 0, MPI_COMM_WORLD);
		bufr2=calloc(sizeOfBufferForEachNode, sizeof(long));
		MPI_Scatter(bufr, sizeOfBufferForEachNode, MPI_LONG, bufr2, sizeOfBufferForEachNode, MPI_LONG,0,MPI_COMM_WORLD);
		for(i =0; i< sizeOfBufferForEachNode;i++){
			temp = (bufr2[i] * bufr2[i]) + temp;
		}
	}
	//gather them all together
	MPI_Gather(&temp, 1, MPI_LONG, finalBuffer, 1, MPI_LONG,numprocs-1,MPI_COMM_WORLD);
	if(rank == numprocs -1){ //the final node does the required math at this point and prints it out
		temp = 0;
		for(i =0; i<numprocs; i++){
			temp = temp + finalBuffer[i]; //summing them all together
		}
		printf("Rank %d reports a grand total of: %ld\n",rank+1,temp);
	}







MPI_Finalize();

}