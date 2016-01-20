//C hello world example
#include <stdio.h>
#include <stdlib.h>
 
void fillBoard(int x,int y,char** board){
		board[x][y] = '@';
	}


void printBoard(int xsize,int ysize,char** board){
	int i;
	int j;
	for(i =1; i<xsize-1;i++){
		for(j =1; j<ysize-1; j++){
			printf("%c ", board[i][j]);
			}
		printf("\n");
	}
}

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
	return count;	 	
}

int main(int argc, char *argv[] ) 
{
	char **board[2];

	void intiBoard(xsize,ysize){
		int b,i;
		for(b=0;b<=1;b++){
			board[b] = calloc((size_t)xsize, sizeof(char *));
			for (i=0; i < xsize; i++) {
				board[b][i] = calloc((size_t)ysize, sizeof(char));
			}
		}
	}



	int input1, input2;
	//read in from file
	scanf("%d", &input1);
	scanf("%d", &input2);
	int x = input1;
	int y = input2;
	//sizes need to be two larger to make the correct buffer
	int xsize = x + 2;
	int ysize = y + 2;
	//create the board with buffer space
	intiBoard(xsize,ysize);
	//fille board with seeds

	//reads things in from the input file
	while(scanf("%d", &input1) == 1 && scanf("%d", &input2) == 1)
	{
		fillBoard(input1,input2,board[0]);
	}
	printf("%d %d\n",x,y);
	printBoard(x,y,board[0]);
	int k = countNighboors(3,3,board[0]);
	printf("%d %d had %d neighboors",3,3,k);

//  printf("%s",argv[1]);
  return 0;
}

