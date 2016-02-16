//C hello world example
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

//fills a spcific x and y corredinate place on the board
void fillBoard(int x,int y,char** board){ board[x][y] = '@'; }

//prints out the board
void printBoard(int xsize,int ysize,char** board){
	int i,j;
	printf("  ");
	for(i=1;i<ysize-1;i++){
		printf("%d",i);
	}
	printf("\n");
	for(i =1; i<xsize-1;i++){ // starts at one and ends ad size-1 to not print the overflow rows 
		printf("%d:", i );
		for(j =1; j<ysize-1; j++){
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
	for(i =0; i<xsize;i++){
		for(j =0; j<ysize; j++){
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

void syncBoard(char** board, int xsize, int ysize){
	int xx;
	//printf("point 10\n");
	for(xx = 0; xx < ysize;xx++){ //top to bottom
		board[xsize-1][xx] = board[1][xx];
	}
	//printf("point 6\n");
	for(xx = 0; xx < ysize;xx++){ //bottom to top
		board[0][xx] = board[xsize-2][xx];
	}
	//printf("point 7\n");
	for(xx = 0; xx < xsize;xx++){ //left to right
		board[xx][ysize-1] = board[xx][1];
	}
	//printf("point 8\n");
	for(xx = 0; xx < xsize;xx++){ //right to left
		board[xx][0] = board[xx][ysize-2];
	}
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


int main(int argc, char *argv[] ) 
{
	char **board[2];
	int numGens = atoi(argv[1]);
	int currboardNumber = 0;
	int nextBoardNumber = 1;

	int rank, numprocs;
	int number,nextNode;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);


	if( rank ==0){
		printf("NODE: %d awake\n", rank);
		//read in from file
		int i;
		int input1, input2;
		scanf("%d", &input1);
		scanf("%d", &input2);
		int x = input1;
		int y = input2;
		sendBoardSize(numprocs,x,y);
		int ysize = x + 2;
		int xsize = y + 2;
		int testArr[2];
		int getx, gety, nodeToSendTo, spotInNewNode; //get values from input stream
		printf("beginning seed distro\n");
		while(scanf("%d", &getx) == 1 && scanf("%d", &gety) == 1)
		{
			testArr[1] = gety;
			nodeToSendTo = getx / (x / numprocs);
			spotInNewNode = getx % (x / numprocs) + 1 ;
			testArr[0] = spotInNewNode;
			printf("X: %d Y: %d TO: %d\n", getx,gety,nodeToSendTo);
			if(nodeToSendTo != 0){
				MPI_Send(&testArr, 2, MPI_INT,nodeToSendTo , 0, MPI_COMM_WORLD);
			}	
			else{
				printf("saw one form myself, did not send to self");
			}
		}	
		for(i = 1; i< numprocs; i++){
			MPI_Send(&xsize, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
		}
	}
	else{
		int ysize;
		int xsize;
		MPI_Status stat;
		int pointsToRecieve[2];
		MPI_Recv(&xsize, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&ysize, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("NODE: %d X: %d  Y: %d\n", rank,xsize,ysize);
		xsize = xsize + 2; // add padding to each side
		ysize = ysize + 2; // add padding to each side
		MPI_Recv(&pointsToRecieve, 2, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
		while(stat.MPI_TAG != 1){
			printf("NODE: %d  Point Recived X: %d  Y: %d\n", rank, pointsToRecieve[0],pointsToRecieve[1]);
			MPI_Recv(&pointsToRecieve, 2, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
		}
		printf("Recieved end of disrto signal\n");
	}

	int i , j , k , z, tempNum;
	//sizes need to be two larger to make the correct buffer

	
/*
	void intiBoard(xsize,ysize){
		int b,i;
		for(b=0;b<=1;b++){
			board[b] = calloc((size_t)xsize, sizeof(char *));
			for (i=0; i < xsize; i++) {
				board[b][i] = calloc((size_t)ysize, sizeof(char));
			}
		}
	}
	//MPI_Send(&count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	//MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	//make a function for rank zero to send to every rank the x and y of the bpoard they will generate without buffer space






	//create the board with buffer space
	intiBoard(xsize,ysize);


	//reads things in from the input file
	while(scanf("%d", &input1) == 1 && scanf("%d", &input2) == 1)
	{
		fillBoard(input2,input1,board[0]);
	}
	//printf("point 2\n");
	syncBoard(board[0],xsize, ysize);
	//printf("point 4\n");
	resetBoard(xsize,ysize,board[1]);
	//printf("point 5\n");

	printf("there will be %d Generations\n", numGens);
	printBoard(xsize,ysize,board[0]);
	for(i =0; i < numGens; i++){//input1 is number of generations will run for this long
		//in each generation the life will be ticked
		for(k = 1; k < xsize-1;k++){//loops through range of y
			for(z=1; z<ysize-1;z++){//loops through range of x's for each row visited
				tempNum = countNighboors(k,z,board[currboardNumber]);
				syncBoard(board[currboardNumber],xsize,ysize);//does the row magic
				lifeAction(tempNum,k,z,board[currboardNumber],board[nextBoardNumber]);
			}
		}
		currboardNumber = switchBoard(currboardNumber);
		nextBoardNumber = switchBoard(nextBoardNumber);
		resetBoard(xsize,ysize,board[nextBoardNumber]);
		if(i % printInterval == 0){
			printBoard(xsize,ysize,board[currboardNumber]);
		}
	}
	
*/
	  MPI_Finalize();
  return 0;
}

