//C hello world example
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

//fills a spcific x and y corredinate place on the board
void fillBoard(int x,int y,char** board){ board[y][x] = '@'; }

//prints out the board
void printBoard(int xsize,int ysize,char** board){
	int i,j;
	printf("  ");
	for(i=1;i<xsize-1;i++){
		printf("%d",i);
	}
	printf("\n");
	for(i =1; i<ysize-1;i++){ // starts at one and ends ad size-1 to not print the overflow rows 
		printf("%d:", i );
		for(j =1; j<xsize-1; j++){
			if (board[i][j] == 0){
				printf(" ");}
			else{
				printf("%c", board[i][j]);}
		}
		printf("\n");
	}
}

//counts the number of neightbors that contain the '@' character
//Do not think that it is nessisary to create the ranges the way I do but what ever not that big of deal
//this still checks the point too, it should not that kind of throws of the math by up too plus one
int countNighboors(int x, int y, char** board){
	int xRange[3] ,yRange[3], i,j;
	int count =0;
	xRange[0] = x-1;
	xRange[1] = x;
	xRange[2] = x + 1;
	yRange[0] = y-1;
	yRange[1] = y;
	yRange[2] = y+1;
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			if(board[xRange[i]][yRange[j]] == '@'){
				count++;
			}
		}
	}
	if( board[xRange[1]][yRange[1]] == '@'){
		count = count -1;
		//returns one less if there is a '@' in the spot
	}
	if(count != 0 ){
	//printf("%d %d %d\n", x,y,count);}
	}
	return count;	 	
}
void resetBoard(int xsize, int ysize, char** board){
	int i,j;
	for(i =0; i<ysize;i++){
		for(j =0; j<xsize; j++){
			board[i][j] = ' ';
		}
	}//zeros out a board so that it is clean for the next generation to be represented
}

void lifeAction(int numNeighboors, int x, int y, char** boardCurr, char** boardNext){
	//printf("point 11 %d %d \n",x,y);
//1. Any live cell with fewer than two live neighbours dies, as if caused by under-population.
//2. Any live cell with two or three live neighbours lives on to the next generation.
//3. Any live cell with more than three live neighbours dies, as if by overcrowding.
//4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction. 
	if(numNeighboors < 2 && boardCurr[x][y] == '@'){ //case 1
		boardNext[x][y] == ' ';
		return;
	}
	if(numNeighboors > 3 && boardCurr[x][y] == '@'){ // case 3
		boardNext[x][y] = ' ';
		return;
	}
	if((numNeighboors == 2 || numNeighboors == 3) && boardCurr[x][y] == '@'){
		boardNext[x][y] = '@';
		return;
	}
	if(numNeighboors == 3 && boardCurr[x][y] != '@'){ //case 4
		boardNext[x][y] = '@';
		return;
	}
	//casae 2 is covered by defualt becaue it changes nothing for the range or [2,3]
}
int switchBoard(int x){
	if(x==1){ return 0;}
	else{return 1;}
}

void syncBoardTopBottom(char** board, int xsize, int ysize){
	int xx;
	//printf("point 10\n");
	for(xx = 0; xx < xsize;xx++){ //top to bottom
		board[ysize-1][xx] = board[1][xx];
	}
	//printf("point 6\n");
	for(xx = 0; xx < xsize;xx++){ //bottom to top
		board[0][xx] = board[ysize-2][xx];
	}

	//printf("point 7\n");

	//printf("point 9\n");
}
//function sends each child node a e and y value that represents the size of the board they need
// to simulate does not include padding space
void sendBoardSize(int numprocs, int totalXSize, int totalYSize){
	int xsize = totalXSize/numprocs;
	int ysize = totalYSize;
	int i;
	for(i = 1; i < numprocs; i++){
			MPI_Send(&xsize, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send(&ysize, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
	}
}
//0 = left incomming so halo goes to left of board
//1 = right incoming so halo goes to right of board
void readInHaloSide(char** board, char* array, int side,int  xsize, int ysize){
	int ii;
	for(ii = 0;ii< ysize;ii++){
		if(side ==0){
			board[ii][0] = array[ii];
		}
		else{
			board[ii][xsize-1] = array[ii];
		}
	}
}


int main(int argc, char *argv[] ) 
{
	char **board[2];
	void intiBoard(xsize,ysize){
		int b,i;
		for(b=0;b<=1;b++){
			board[b] = calloc((size_t)ysize, sizeof(char *));
			for (i=0; i < ysize; i++) {
				board[b][i] = calloc((size_t)xsize, sizeof(char));
			}
			resetBoard(xsize,ysize, board[b]);
		}
		}

	
	int numGens = atoi(argv[1]);
	int currboardNumber = 0;
	int nextBoardNumber = 1;
	int ysize;
	int xsize;
	MPI_Status stat;
	int rank, numprocs;
	int number,nextNode;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	int x;
	int y;

	if( rank ==0){
		//printf("NODE: %d awake\n", rank);
		//read in from file
		int i;
		int input1, input2;
		scanf("%d", &input1);
		scanf("%d", &input2);
		x = input1;
		y = input2;
		sendBoardSize(numprocs,x,y);
		xsize = x/numprocs + 2;
		ysize = y + 2;
		int testArr[2];
		intiBoard(xsize,ysize);
		//printf("x:%d y:%d\n",xsize,ysize);
		int getx, gety, nodeToSendTo, spotInNewNode; //get values from input stream
		//printf("beginning seed distro\n");
		while(scanf("%d", &getx) == 1 && scanf("%d", &gety) == 1)
		{
			testArr[1] = gety;
			nodeToSendTo = (getx - 1) / (x / numprocs);
			spotInNewNode = getx - 10 * nodeToSendTo ;
			testArr[0] = spotInNewNode;
			//printf("X: %d Y: %d TO: %d\n", getx,gety,nodeToSendTo);
			if(nodeToSendTo != 0){
				MPI_Send(&testArr, 2, MPI_INT,nodeToSendTo , 0, MPI_COMM_WORLD);
			}	
			else{
				//printf("placing life x: %d y: %d \n", getx,gety);
				fillBoard(getx,gety,board[0]);
				//printf("saw one form myself, did not send to self\n");
			}
		}	
		for(i = 1; i< numprocs; i++){
			MPI_Send(&xsize, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
		}
		MPI_Barrier(MPI_COMM_WORLD);
		//printf("The y size here is %d\n", ysize);
	}
	else{
		
		int pointsToRecieve[2];
		MPI_Recv(&xsize, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&ysize, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		xsize = xsize + 2; // for padding
		ysize = ysize + 2; // for padding
		intiBoard(xsize,ysize);
		MPI_Recv(&pointsToRecieve, 2, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
		while(stat.MPI_TAG != 1){
			//printf("NODE: %d  Point Recived X: %d  Y: %d\n", rank, pointsToRecieve[0],pointsToRecieve[1]);
			fillBoard(pointsToRecieve[0],pointsToRecieve[1],board[0]);
			MPI_Recv(&pointsToRecieve, 2, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
			
		}
		//printf("Node: %d Recieved end of disrto signal\n",rank);
		MPI_Barrier(MPI_COMM_WORLD);
	}
	
	//at this point all of the boards should be set up with life in them time to sync the edges
	//lets create a test halo exchange

	//a loop will go around this for every generation
		char leftArray[ysize] , rightArray[ysize];
		int yy; 
		char leftArrayRecive[ysize],rightArrayRecive[ysize];
		int i,k,z,tempNum;
		//start the halo exchange
		//get the part of the board we are planing on exchanging
		//start with switching the top anb bottoms because that doesn't require comms between nodes
		for(i =0; i < numGens; i++){
			if(rank ==0){
				syncBoardTopBottom(board[currboardNumber],xsize, ysize); //snyc the top and bottom of board before the edges
				for(yy = 0; yy < ysize;yy++){ //left to right
					leftArray[yy] = board[currboardNumber][yy][1];
				}
				MPI_Send(&leftArray, ysize, MPI_CHAR, numprocs-1, 1, MPI_COMM_WORLD);
				MPI_Recv(&leftArrayRecive, sizeof(leftArrayRecive), MPI_CHAR, MPI_ANY_SOURCE , MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
				readInHaloSide(board[currboardNumber], leftArrayRecive,1,xsize,ysize);
				//////////////////at this point the circle should have gone around and every node should have shared its left side
				////////////////// time to start sharing the right node
				for(yy = 0; yy < ysize;yy++){ //left to right
					rightArray[yy] = board[currboardNumber][yy][xsize-2]; //creating the array
				}
				MPI_Send(&rightArray, ysize, MPI_CHAR, 1, 1, MPI_COMM_WORLD);
				MPI_Recv(&rightArrayRecive, sizeof(rightArrayRecive), MPI_CHAR, MPI_ANY_SOURCE , MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
				//printf("Node: %d, Recived From %d \n", rank, stat.MPI_SOURCE );
				readInHaloSide(board[currboardNumber], rightArrayRecive,0,xsize,ysize);
			}
			else{
				
				MPI_Recv(&leftArrayRecive, sizeof(leftArrayRecive), MPI_CHAR, MPI_ANY_SOURCE , MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
				syncBoardTopBottom(board[currboardNumber],xsize, ysize);
				//printf("Node: %d, Recived From %d \n", rank, stat.MPI_SOURCE );
				readInHaloSide(board[currboardNumber], leftArrayRecive,1,xsize,ysize);
				for(yy = 0; yy < ysize;yy++){ //left to right
					leftArray[yy] = board[currboardNumber][yy][1];
				}
				MPI_Send(&leftArray, ysize, MPI_CHAR, rank-1, 1, MPI_COMM_WORLD);
				//////////////////at this point the circle should have gone around and every node should have shared its left side
				////////////////// time to start sharing the right node
				MPI_Recv(&rightArrayRecive, sizeof(rightArrayRecive), MPI_CHAR, MPI_ANY_SOURCE , MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
				readInHaloSide(board[currboardNumber], rightArrayRecive,0,xsize,ysize);
				for(yy = 0; yy < ysize;yy++){ //left to right
					rightArray[yy] = board[currboardNumber][yy][xsize-2];
				}
				MPI_Send(&leftArray, ysize, MPI_CHAR, (rank+1) % numprocs, 1, MPI_COMM_WORLD);
			}

			//at this point the boards should all by synced up and ready to do the life action
			MPI_Barrier(MPI_COMM_WORLD);
			for(k = 1; k < ysize-1;k++){//loops through range of y
				for(z=1; z<xsize-1;z++){//loops through range of x's for each row visited
					tempNum = countNighboors(k,z,board[currboardNumber]);
					lifeAction(tempNum,k,z,board[currboardNumber],board[nextBoardNumber]);
			}
			}

			currboardNumber = switchBoard(currboardNumber);
			nextBoardNumber = switchBoard(nextBoardNumber);
			resetBoard(xsize,ysize,board[nextBoardNumber]);
		}//end of loop for generation, starts over untill generational limit has been reached
		MPI_Barrier(MPI_COMM_WORLD);
		//now we start the consolodation process of this lab
		//the master will be placed in a state of recept, receicing the coodinates of life culturs inside the slave nodes
		//The master will exit this state after receiving numProcs -1 completion codes which are messages with a tag of 0


		if(rank ==0){
			//init new board to store alll the things in 
			//printf("creating final board\n");
			char** finalBoard;
			int lifeLocation[2];
			finalBoard = calloc((size_t)y+2, sizeof(char *));
			for (i=0; i < y+2; i++) {
				finalBoard[i] = calloc((size_t)x+2, sizeof(char));
			}

			resetBoard(x+2,y+2, finalBoard);
			int i,j,k;
			//printf("copying life from master  0 board into new board\n");
			for(i =1; i < ysize-1; i++){
				for(j=1;j < xsize-1; j++){
					if(board[currboardNumber][i][j] == '@'){
						fillBoard(i, j, finalBoard);
					}
				}
			}
			//printf("made it to start of board print\n");
			//printf("made it to end of board print\n");
			int endCount = 0;
			while( endCount != (numprocs - 1)){
				MPI_Recv(&lifeLocation, 2, MPI_INT, MPI_ANY_SOURCE , MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
				if(stat.MPI_TAG == 0){
					endCount++;
					//printf("saw end count\n");
				}
				else{
					//printf("adding in X; %d, Y: %d \n",lifeLocation[0],lifeLocation[1]);
					fillBoard(lifeLocation[0], lifeLocation[1],finalBoard);
				}
			}
			printBoard(x+2,y+2,finalBoard);
		}
		else{
			int xx, yy;
			int lifeLocation[2];
			for(yy=1;yy< ysize-1; yy++){
				for(xx=1;xx< xsize-1;xx++){
					if(board[currboardNumber][yy][xx] == '@'){
						lifeLocation[0] = xx;
						lifeLocation[1] = yy;
						MPI_Send(&lifeLocation, 2, MPI_INT, 0, 1, MPI_COMM_WORLD);
					}
				}
			}
			MPI_Send(&lifeLocation, 2, MPI_INT, 0, 0, MPI_COMM_WORLD);
		}
		//end of relavent coding block
	  MPI_Finalize();
  return 0;
}

