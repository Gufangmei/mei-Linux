/***********************************************************************************
    author: Gu Fangmei
**********************************************************************************/

#include <math.h> //log2()
#include<vector>
//#include<linux/input.h>
#include<stdlib.h> //rand()
#include <sys/stat.h> //open()
#include <fcntl.h>   //open()
#include <unistd.h> //read()
#include<iostream>
#include<stack>
#include<curses.h>
using namespace std;
#define MAX_POINT 2048
#define WIDTH 4
//int nMatrix[WIDTH][WIDTH];


int maxInMatrix(int matrix[][4],int width){// to find the max number in the matrix
  int max = 0;
  for(int i = 0; i < width; ++i){
    for(int j = 0; j < width; ++j){
      if(max < matrix[i][j])
	max = matrix[i][j];
    }
  }
  return max;
}
bool isWin(int matrix[][4],int width){//to judge if reach the biggest number
  return maxInMatrix(matrix,width) == MAX_POINT;
}
void clearStack(stack<int>& stk){// to clear the stack 
  while(!stk.empty())
    stk.pop();
}
void moveRight(int matrix[][4],int width){
  int nCurr;
  stack<int> stk;
  for(int i = 0; i < width; ++i){ //swith rows
    clearStack(stk);
    for(int j = width - 1; j >= 0; --j){ //go through from right to left
      	if(matrix[i][j] == 0)continue;
	nCurr = j;
	if(stk.size() == 0){//the first non-zero
	  if(j != 3){
	    matrix[i][3] = matrix[i][j];
	    matrix[i][j] = 0;
	  }
	   stk.push(3);continue;
	}
        while(1){
	  if(matrix[i][stk.top()] == matrix[i][nCurr]){
	    matrix[i][stk.top()] *= 2;
	    matrix[i][nCurr] = 0;
	    if(stk.size() == 1)
	      break;
	    nCurr = stk.top();// handle = true;
	    stk.pop();
	  }else{
	    if(stk.top() - nCurr > 1){
	      matrix[i][stk.top()-1] = matrix[i][nCurr];
	      matrix[i][nCurr] = 0;
	      stk.push(stk.top()-1);
	    }else
	      stk.push(nCurr);
	    break;
	  }
	}//while
    }//inner loop
  }//outer loop
}
void moveLeft(int matrix[][4],int width){
  int nCurr;
  stack<int> stk; // to store the previous non-zero numbers
  for(int i = 0; i < width; ++i){ //swith rows
    clearStack(stk);
    for(int j = 0; j < width; ++j){ //go through from right to left
      	if(matrix[i][j] == 0)continue;
	nCurr = j;
	if(stk.size() == 0){//the first non-zero
	  if(j != 0){
	    matrix[i][0] = matrix[i][j];
	    matrix[i][j] = 0;
	  }
	   stk.push(0);continue;
	}
        while(1){
	  if(matrix[i][stk.top()] == matrix[i][nCurr]){
	    matrix[i][stk.top()] *= 2;
	    matrix[i][nCurr] = 0;
	    if(stk.size() == 1)
	      break;
	    nCurr = stk.top();
	    stk.pop();
	  }else{
	    if(nCurr - stk.top() > 1){
	      matrix[i][stk.top()+1] = matrix[i][nCurr];
	      matrix[i][nCurr] = 0;
	      stk.push(stk.top()+1);
	    }else
	      stk.push(nCurr);
	    break;
	  }
	}//while
    }//inner loop
  }//outer loop
}
void moveUp(int matrix[][4],int width){
  int nCurr;
  stack<int> stk;
  for(int j = 0; j < width; ++j){ //swith columns
    clearStack(stk);
    for(int i = 0; i < width; ++i){ //go through from right to left
      	if(matrix[i][j] == 0)continue;
	nCurr = i;
	if(stk.size() == 0){//the first non-zero
	  if(i != 0){
	    matrix[0][j] = matrix[i][j];
	    matrix[i][j] = 0;
	  }
	   stk.push(0);continue;
	}
        while(1){
	  if(matrix[stk.top()][j] == matrix[nCurr][j]){
	    matrix[stk.top()][j] *= 2;
	    matrix[nCurr][j] = 0;
	    if(stk.size() == 1)
	      break;
	    nCurr = stk.top();
	    stk.pop();
	  }else{
	    if(nCurr - stk.top() > 1){
	      matrix[stk.top()+1][j] = matrix[nCurr][j];
	      matrix[nCurr][j] = 0;
	      stk.push(stk.top()+1);
	    }else
	      stk.push(nCurr);
	    break;
	  }
	}//while
    }//inner loop
  }//outer loop
}
void moveDown(int matrix[][4],int width){
  int nCurr;
  stack<int> stk;
  for(int j = 0; j < width; ++j){ //swith columns
    clearStack(stk);
    for(int i = width - 1; i >= 0; --i){ //go through from right to left
      	if(matrix[i][j] == 0)continue;
	nCurr = i;
	if(stk.size() == 0){//the first non-zero
	  if(i != 3){
	    matrix[3][j] = matrix[i][j];
	    matrix[i][j] = 0;
	  }
	   stk.push(3);continue;
	}
        while(1){
	  if(matrix[stk.top()][j] == matrix[nCurr][j]){
	    matrix[stk.top()][j] *= 2;
	    matrix[nCurr][j] = 0;
	    if(stk.size() == 1)
	      break;
	    nCurr = stk.top();// handle = true;
	    stk.pop();
	  }else{
	    if(stk.top() - nCurr > 1){
	      matrix[stk.top()-1][j] = matrix[nCurr][j];
	      matrix[nCurr][j] = 0;
	      stk.push(stk.top()-1);
	    }else
	      stk.push(nCurr);
	    break;
	  }
	}//while
    }//inner loop
  }//outer loop
}

void controlDirection(int matrix[][4],int drt){
  //to control which direction the matrix is moving
  switch(drt){ 
  case 0:moveRight(matrix,4);break;
  case 1:moveDown(matrix,4);break;
  case 2:moveLeft(matrix,4);break;
  case 3:moveUp(matrix,4);break;
  }
}
int randFill(int matrix[][4],int width){
  //fill one of the blankets by random
  vector<int> vRow,vCol;
  for(int i = 0; i < width; ++i){
    for(int j = 0; j < width; ++j){
      if(matrix[i][j] == 0){
	vRow.push_back(i);
	vCol.push_back(j);
      }
    }
  }
  if(vRow.size() < 1)return -1; //if none empty blacket exists, then lose 
  int nRand = rand() % vRow.size();
  int nFill = (nRand % 2 == 0 ? 2:4);
  matrix[vRow[nRand]][vCol[nRand]] = nFill;
  return 0;
}
void initMatrix(int matrix[][4],int width){// init the matrix with 3 numbers
  int tmp = rand(),n = 3;
  for(int i = 0; i < width; ++i){
    for(int j = 0; j < width; ++j){
      matrix[i][j] = 0;
    }
  }
  for(int i = 0;i < 3;++i)randFill(matrix,4);
}
void printMatrix(int matrix[][4],int width,int x,int y){
  //print the matrix with different numbers in different colors
  //(x,y) is the beginning position to print the matrix
  init_pair(0,COLOR_WHITE,COLOR_BLACK);
  init_pair(1,COLOR_RED,COLOR_BLACK);
  init_pair(2,COLOR_BLUE,COLOR_BLACK);
  init_pair(3,COLOR_GREEN,COLOR_BLACK);
  init_pair(4,COLOR_YELLOW,COLOR_BLACK);
  init_pair(5,COLOR_MAGENTA,COLOR_BLACK);
  init_pair(6,COLOR_CYAN,COLOR_BLACK);
  init_pair(7,COLOR_RED,COLOR_BLACK);
  init_pair(8,COLOR_BLUE,COLOR_BLACK);
  init_pair(9,COLOR_MAGENTA,COLOR_BLACK);
  init_pair(10,COLOR_GREEN,COLOR_BLACK);
  init_pair(11,COLOR_YELLOW,COLOR_BLACK);
  init_pair(1024,COLOR_WHITE,COLOR_BLACK);
  int diff = 5,height = 1;
  for(int i = 0;i < width; ++i){
    move(x+height*i,y-1);addch('|');
    for(int j = 0;j < width; ++j){
      char tmp[4];
      if(matrix[i][j] < 10){
	if(matrix[i][j] == 0)// if the number is zero, print empty
	  sprintf(tmp,"    ");
	else
	  sprintf(tmp," %d  ",matrix[i][j]);
      }else
      if(matrix[i][j] < 100){
	sprintf(tmp," %d ",matrix[i][j]);
      }else
      if(matrix[i][j] < 1000){
	sprintf(tmp," %d",matrix[i][j]);
      }else
      if(matrix[i][j] < 10000){
	sprintf(tmp,"%d",matrix[i][j]);
      }
      attron(COLOR_PAIR(int(log2(matrix[i][j]))));
      move(x+height*i,y+diff*j);addstr(tmp);
      attroff(COLOR_PAIR(int(log2(matrix[i][j]))));
      move(x+height*i,y+diff*j+4);addch('|');
    }
  }
}
void game(){//main function for the game
  initscr();
  start_color();
  int matrix[4][4];
  initMatrix(matrix,4);
  keypad(stdscr,true);
  int c ; bool isQuit;
  isQuit = false;
  printMatrix(matrix,4,12,19);
  move(6,5);addstr("press \"q\" to exit");
  while(1){
    c = getch();//to dectect the key pressed
    switch(c){
    case 'q':isQuit = true;break;  //key "q" pressed, quit
    case KEY_DOWN:controlDirection(matrix,1);break;
    case KEY_UP:controlDirection(matrix,3);break;
    case KEY_RIGHT:controlDirection(matrix,0);break;
    case KEY_LEFT:controlDirection(matrix,2);break;
    }
    if(isQuit)break;
    if(-1 == randFill(matrix,4)){
      move(5,5);addstr("you lose :(");
    }
    if(isWin(matrix,4)){
      move(5,7);addstr("you win :)");
    }
    printMatrix(matrix,4,12,19);     //print the matrix
  }
  refresh();
  endwin();
}
int main(){
  game();
  return 0;
}
