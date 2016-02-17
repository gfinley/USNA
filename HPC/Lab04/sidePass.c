#include <stdio.h>
#include <time.h>
#include "mpi.h"
#include <math.h>
#include <stdlib.h>
#include <unistd.h>



//this function takes the 
void  sendRandomMessage( rank, numprocs){
		int randomDestinationNode = rand() % (numprocs);//http://stackoverflow.com/questions/1202687/in-c-how-do-i-get-a-specific-range-of-numbers-from-rand
		while(rank == randomDestinationNode){
			randomDestinationNode = rand() % (numprocs);
		}
		int direction = rand() % (2);
		if(direction ==1 ){ //diection =1 means clockwise
			if((rank + 1) == numprocs){
				MPI_Send(&direction, 1, MPI_INT, 0, randomDestinationNode, MPI_COMM_WORLD);
			}
			else{
				MPI_Send(&direction, 1, MPI_INT, rank +1, randomDestinationNode, MPI_COMM_WORLD);
			}
			char dirStr[] = "Clockwise";
			printf("%d: Sending a message %s to rank %d\n", rank, dirStr,randomDestinationNode);
		}
		else{
			if(rank == 0){
				MPI_Send(&direction, 1, MPI_INT, numprocs-1, randomDestinationNode, MPI_COMM_WORLD);
			}
			else{
				MPI_Send(&direction, 1, MPI_INT, rank -1, randomDestinationNode, MPI_COMM_WORLD);
			}
			char dirStr[] = "CounterClockwise";
			printf("%d: Sending a message %s to rank %d\n", rank, dirStr,randomDestinationNode);
		}
}

int main(int argc, char *argv[] ){
	int rank, numprocs;
	int number,nextNode;
	char dirStr[] = "Clockwise";
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	MPI_Status stat;
	int direction;
	if( rank ==0){
		sendRandomMessage(0, numprocs);
	}

	while(1){
		MPI_Recv(&number, 1, MPI_INT, MPI_ANY_SOURCE , MPI_ANY_TAG , MPI_COMM_WORLD, &stat);
		direction = number;
		if(rank == stat.MPI_TAG){
			printf("%d: Recvd msg from %d; it is a message for me!!\n",rank,stat.MPI_SOURCE);
			sendRandomMessage(rank, numprocs);
		}
		else{
			if( direction ==1){
				char dirStr[] = "Clockwise";
				if((rank + 1) == numprocs){
					nextNode = 0;
					printf("%d: Recvd msg from %d; sending %s to Rank %d\n", rank,stat.MPI_SOURCE, dirStr ,nextNode);
					MPI_Send(&direction, 1, MPI_INT, 0, stat.MPI_TAG, MPI_COMM_WORLD);
					
				}
				else{
					nextNode = rank +1;
					printf("%d: Recvd msg from %d; sending %s to Rank %d\n", rank,stat.MPI_SOURCE, dirStr ,nextNode);
					MPI_Send(&direction, 1, MPI_INT, rank +1, stat.MPI_TAG, MPI_COMM_WORLD);
				}
				
			}
			else{
				char dirStr[] = "CounterClockwise";
				if(rank == 0){
					nextNode = numprocs-1;
					printf("%d: Recvd msg from %d; sending %s to Rank %d\n", rank,stat.MPI_SOURCE,dirStr, nextNode);
					MPI_Send(&direction, 1, MPI_INT, numprocs-1, stat.MPI_TAG, MPI_COMM_WORLD);
				}
				else{
					nextNode = rank -1;
					printf("%d: Recvd msg from %d; sending %s to Rank %d\n", rank,stat.MPI_SOURCE,dirStr, nextNode);
					MPI_Send(&direction, 1, MPI_INT, rank -1,stat.MPI_TAG, MPI_COMM_WORLD);
				}
				
			}
		}
		sleep(1);

	}
	MPI_Finalize();
	}