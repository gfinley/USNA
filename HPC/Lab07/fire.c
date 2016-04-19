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
	int ii,jj,value;
	double density = 0;
	for(ii =0; ii<forestSize-1;ii++){
		for(jj=0; jj< forestSize-1; jj++){
			 	
			 	if(haveTree()){
			 		forest[ii][jj] = treeAge();
			 		density = density + 1;
			 	}
			 	else{
					forest[ii][jj] = 0;
				}
				if(treeOnFire()){
					forest[ii][jj] = forest[ii][jj] * -1;
					printf("fire started \n");
			}
			
			//forest[ii][jj] = rank;
		}
	}
	//printf("Node %d: Density: %lG\n", rank , density/(forestSize*forestSize));
}
void zeroForest(int rank, int** forest, int forestSize){
	int ii,jj,value;
	double density = 0;
	for(ii =1; ii<forestSize-2;ii++){
		for(jj=1; jj< forestSize-2; jj++){
			 		forest[ii][jj] = 7;
			}
		}
	}
double getDensity(int rank, int** forest, int forestSize){
	int ii,jj,value;
	double treeCount = 0;
	for(ii =1; ii<forestSize-2;ii++){
		for(jj=1; jj< forestSize-2; jj++){
				//printf("%d",forest[ii][jj]);
				if(forest[ii][jj] > 0){
			 		treeCount++;
				}
			}
		}
	return treeCount;
}
double countFire(int rank, int** forest, int forestSize){
	int ii,jj,value;
	double treeCount = 0;
	for(ii =1; ii<forestSize-2;ii++){
		for(jj=1; jj< forestSize-2; jj++){
				//printf("%d",forest[ii][jj]);
				if(forest[ii][jj] < 0){
			 		treeCount++;
			 		//printf("%d",forest[ii][jj]);
				}
			}
		}
	return treeCount;
}
void printNode(int rank, int** forest, int forestSize){
	int ii,jj,value;
	double treeCount = 0;
	printf("---------------------RANK %d----------------------------------\n",rank);
	for(ii =1; ii<forestSize-2;ii++){
		for(jj=1; jj< forestSize-2; jj++){
				//printf("%d",forest[ii][jj]);
			 		printf("%d",forest[ii][jj]);
				
				
			}
			printf("\n");
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
int** haloExchangeStepOne(int rank,int** forest,int forestSize,int n){
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
	return forest;
}
int** haloExchangeStepTwo(int rank,int** forest,int forestSize,int n){
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
	return forest;
}
int** haloExchangeStepThree(int rank,int** forest,int forestSize,int n){
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
	return forest;
}
int** haloExchangeStepFour(int rank,int** forest,int forestSize,int n){
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
	return forest;
}

int haveTree(){
	if( rand() % 100 < 80){
		return 1;
	}
	else{
		return 0;
	}
}
int treeAge(){
	int age = rand() % 5;
	age++;
	if( age > 3){
		return 3;
	}
	else{
		return age;
	}
}
int treeOnFire(){
	int temp = lrand48() % 1000000 ;
	//printf("%d\n",temp);
	if ( temp ==1){
		return 1;
	}
	else{ 
		return 0;
	}
}
int** haloExchange(int rank, int** forest, int forestSize,int n){
	haloExchangeStepOne(rank,forest,forestSize,n);
	haloExchangeStepTwo(rank,forest,forestSize,n);
	haloExchangeStepThree(rank,forest,forestSize,n);
	haloExchangeStepFour(rank,forest,forestSize,n);
	return forest;

}
double stepForwardTwo(int rank,int** oldForest, int** newForest, int forestSize, int xwind, int ywind, int fireSpreadRate){
	int ii,jj; //loop variables
	int onFire = 0; //flag for that spot being on Fire
	int change = 0;
	for(ii=1; ii < forestSize-1; ii++){
		for(jj=1; jj<forestSize-1; jj++){
			if(oldForest[ii][jj]  >= 0){
				newForest[ii][jj] = oldForest[ii][jj];
			}
			else if(oldForest[ii][jj]  < 0){
				newForest[ii][jj] = oldForest[ii][jj] + 1;
				change = change + 1;
			}
			if(oldForest[ii-1][jj] < 0 && oldForest[ii][jj] > 0 ){
				int temp = rand() % 100;
				if(temp < (fireSpreadRate + ywind )){
					newForest[ii][jj] = oldForest[ii][jj]*-1;
					change = change + 1;
				}

			}
			if(oldForest[ii+1][jj] < 0 && oldForest[ii][jj] > 0 ){
				int temp = rand() % 100;
				if(temp < (fireSpreadRate - ywind )){
					newForest[ii][jj] = oldForest[ii][jj]*-1;
					change = change + 1;
				}
			}
			if(oldForest[ii][jj-1] < 0 && oldForest[ii][jj] > 0 ){
				int temp = rand() % 100;
				if(temp < (fireSpreadRate  - xwind)){
					newForest[ii][jj] = oldForest[ii][jj]*-1;
					change = change + 1;
				}
			}
			if(oldForest[ii][jj+1] < 0 && oldForest[ii][jj] > 0 ){
				int temp = rand() % 100;
				if(temp < (fireSpreadRate + xwind )){
					newForest[ii][jj] = oldForest[ii][jj]*-1;
					change = change + 1;
				}
			}
			if(newForest[ii][jj] == 7){
				printf("you should not see this error");
			}

		}
	}
	return change;
}


double stepForward(int rank,int** oldForest, int** newForest, int forestSize, int xwind, int ywind){
	int ii,jj,value;
	double density = 0;
	int change = 0;
	double treeCount = 0;
	for(ii =0; ii<forestSize-1;ii++){
		for(jj=0; jj< forestSize-1; jj++){
			if(oldForest[ii][jj] < 0 && ii > 1 && ii < forestSize-2 && jj >1 && jj < forestSize-2 ){ //forest is on fire at this cell
				newForest[ii][jj] = oldForest[ii][jj] + 1; //move closer to zero on that cell untill zero is reached
				change = change + 1;
				
			}
			else{ //new "forest" gets the value of the old cell, either a zero of a positive value
				if(newForest[ii][jj] == 7){
					newForest[ii][jj] = oldForest[ii][jj];
				}
			}
			if(oldForest[ii][jj] > 0){
				treeCount++;
			}
			//cover the fire spreading case, redundent to if case above but easier to think about this way
			if(oldForest[ii][jj] < 0){
				int northChance = rand() % 100;
				int southChance = rand() % 100;
				int  eastChance = rand() % 100;
				int  westChance = rand() % 100;
				if(northChance < (33+ywind) && ii > 0 ){
					if(oldForest[ii-1][jj] > 0){
						newForest[ii-1][jj] = oldForest[ii-1][jj] *-1;
						change = change + 1;
					}
				}
				if(eastChance <( 33+ xwind) && jj < (forestSize-1)){
					if(oldForest[ii][jj+1] > 0){
						newForest[ii][jj+1] = oldForest[ii][jj+1] *-1;
						change = change + 1;
					}
				}
				if(westChance < (33-xwind) && jj > 0){
					if(oldForest[ii][jj-1] > 0){
						newForest[ii][jj-1] = oldForest[ii][jj-1] *-1;
						change = change + 1;
					}

				}
				if(southChance <(33-ywind) && ii < (forestSize-1)){
					if(oldForest[ii+1][jj] > 0){
						newForest[ii+1][jj] = oldForest[ii+1][jj] *-1;
						change = change + 1;
					}
				}
			}
		}
	}
	//printf("Node %d: Density: %lG\n", rank , treeCount/(forestSize*forestSize));

	return change;
}
int changeForest(int swi){
	if( swi ==0){
		return 1;
	}
	else{
		return 0;
	}
}

int main(int argc, char *argv[] ) 
{

	//variables to be used for MPI
	int xwind =  atoi(argv[3]);
	int ywind =  atoi(argv[4]);
	int rank, numprocs, swi;
	int change;
	MPI_Status stat;
	srand48(time(NULL) + rank);
	srand(time(NULL) + rank);
	//variables to be used for workspace distrobution
	int forestSize;
	//mpi set up
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	

	//get worldsize
	int fireSpreadRate = atoi(argv[2]);
	forestSize = (atoi(argv[1])/ (sqrt(numprocs)))+2;	
	int **forest[2];
	int **oldForest;
	int **newForest;

	int n = sqrt(numprocs);
	forest[0] = initForestSection( forestSize, forest[0]);
	forest[1] = initForestSection( forestSize, forest[1]);
	fillforest(rank,forest[0],forestSize);
	zeroForest(rank,forest[1],forestSize);
	int generations = 0;
	double total;
	int** tempForest;
	oldForest = forest[0];
	newForest = forest[1];
	//printf("starting generations");
	change = 1;
	int changeTotal = 1;
	int counter = 1;
	oldForest = haloExchange(rank, oldForest, forestSize,n);
	//printf("Node %d top: %d right: %d down: %d left: %d\n", rank, oldForest[0][3], oldForest[30][forestSize-1],oldForest[forestSize-1][20],oldForest[40][0]);
	
	while(changeTotal != 0){
		
		oldForest = haloExchange(rank, oldForest, forestSize,n);
		change = stepForwardTwo(rank,oldForest,newForest,forestSize,xwind,ywind,fireSpreadRate);
		//printf("node %d, change: %d\n",rank,change);
		MPI_Allreduce(&change,&changeTotal, 1,MPI_INT, MPI_SUM, MPI_COMM_WORLD);
		generations++;
		tempForest = oldForest;
		oldForest = newForest;
		zeroForest( rank,tempForest,forestSize); 
		newForest = tempForest;
		counter++;

		double treeCount = getDensity(rank, oldForest, forestSize);
		MPI_Allreduce(&treeCount,&total, 1,MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

		if(counter % 10 == 0 && change > 0){
			//printf("Node: %d  Gen: %d Density: %lG, change %d\n",rank, counter, treeCount/(forestSize*forestSize), change);
			//printNode(rank,oldForest,forestSize);
			printf("Density: %lG\n", total/(forestSize*n*forestSize*n));
		}
	}
	//printf("finsihed generation");
	double treeCount = getDensity(rank, oldForest, forestSize);
	MPI_Allreduce(&treeCount,&total, 1,MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
	//printf("%lG\n", total);
	if(rank ==0){
			printf("Density: %lG\n", total/(forestSize*n*forestSize*n));
	}
	

	MPI_Finalize();
}