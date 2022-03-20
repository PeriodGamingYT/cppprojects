#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef int *board;
const int MAX_NUMBER = 2048;
const int MAX_NUMBER_DIGITS = 4;
const int MAX_NUMBER_SPACE = 6;
const int BOARD_SIZE = 4;

int boardWhere(int i, int j) {
	if(i < 0 || j < 0 || i >= BOARD_SIZE || j >= BOARD_SIZE) {
		return -1;
	}

	return (j * BOARD_SIZE) + i;
}

char *makeBoardLine() {
		char *result = (char*)malloc(sizeof(char*) * ((BOARD_SIZE * MAX_NUMBER_SPACE) + 3));
		for(int i = 0; i < ((BOARD_SIZE * MAX_NUMBER_SPACE) + 3); i += MAX_NUMBER_SPACE + 1) {
			result[i] = '+';
			for(int j = 0; j < MAX_NUMBER_SPACE; j++) {
				result[i+j+1] = '-';
			}
		}

		result[strlen(result)] = '+';
		return result;
}

int *makeDefaultBoard() {
	int *result = (int*)malloc(sizeof(int*) * BOARD_SIZE * BOARD_SIZE);
	for(int i = 0; i < BOARD_SIZE; i++) {
		for(int j = 0; j < BOARD_SIZE; j++) {
			result[boardWhere(i, j)] = 0;
		}
	}

	result[0] = 2;
	return result;
}

int numberLength(int number) {
	if(number == 0) {
		return 1;
	}
	
	int exponet = 1;
	int result = 0;
	if(number < 0) {
		result = 1;
		number *= -1;
	}

	while(exponet < number) {
		exponet *= 10;
		result += 1;
	}

	return result;
}

char *boardLine;
void printBoard(board currentBoard) {
	for(int i = 0; i < BOARD_SIZE; i++) {
		printf("%s\n", boardLine);
		for(int j = 0; j < BOARD_SIZE; j++) {
			if(currentBoard[boardWhere(i, j)] > 9999 || currentBoard[boardWhere(i, j)] < 0) {
				return;
			}

			char *emptySpaces = (char*)malloc(
				sizeof(char*) * (MAX_NUMBER_SPACE - numberLength(currentBoard[boardWhere(i, j)]) - 1));
			for(int k = 0; k < MAX_NUMBER_SPACE - numberLength(currentBoard[boardWhere(i, j)]) - 1; k++) {
				emptySpaces[k] = ' ';
			}
			
			printf("| %d%s", currentBoard[boardWhere(i, j)], emptySpaces);
		}

		printf("|\n");
	}

	printf("%s\n", boardLine);
}

int flip(int n) {
	return n == 0 || n == 3 ? (n == 0 ? 3 : 0) : n;
}

int direction(int x, int y) {
	if(x < 0 || y < 0 || x > 1 || y > 1) {
		return 0;
	}
	
	return x | (y << 1);
}

int dirMul(bool isForward) {
	return (BOARD_SIZE - 1) * isForward;
}

int boolInc(bool increase) {
	return increase ? 1 : -1;
}

void lose() {
	printf("You lose!\n\n");
	char buffer;
	scanf("%c", &buffer);
	exit(0);
}

int *compressStrip(int *tempStrip) {
	for(int i = 0; i <= BOARD_SIZE; i += 2) {
		int k = 0;
		int *tempCopy = (int*)malloc(sizeof(int*) * BOARD_SIZE);
		for(int j = 0; j < BOARD_SIZE; j++) {
			tempCopy[j] = 0;
		}
		
		for(int j = 0; j < BOARD_SIZE; j++) {
			if(tempStrip[j] != 0) {
				tempCopy[k] = tempStrip[j];
				k++;
			}
		}

		if(tempCopy[i] == tempCopy[i+1] && tempCopy[i] != 0 && tempCopy[i+1] != 0) {
			tempCopy[i] = tempCopy[i] + tempCopy[i+1];
			tempCopy[i+1] = 0;
			i = 0;
		}

		tempStrip = tempCopy;
	}

	for(int i = 0; i < BOARD_SIZE; i++) {
		for(int j = 0; j < BOARD_SIZE; j++) {
			if(tempStrip[i] == tempStrip[j] && tempStrip[i] != 0 && tempStrip[j] != 0 && i != j) {
				lose();
			}
		}
	}

	return tempStrip;
}

board moveBoard(board currentBoard, int dir) { // 0 | 1 X, 0 | 1 Y.
	bool isDirXForward = (dir & 1) == 1;
	bool isDirYForward = (dir & 2) == 2;
	for(int i = 0; i < BOARD_SIZE && isDirXForward; i++) {
		int *tempStrip = (int*)malloc(sizeof(int*)*BOARD_SIZE);
		for(int j = 0; j < BOARD_SIZE; j++) {
			tempStrip[j] = currentBoard[boardWhere(i, j)];
		}
		
		tempStrip = compressStrip(tempStrip);
		if(isDirXForward) {
			int tempSwap = tempStrip[0];
			tempStrip[0] = tempStrip[BOARD_SIZE - 1];
			tempStrip[BOARD_SIZE - 1] = tempSwap;
			tempSwap = tempStrip[1];
			tempStrip[1] = tempStrip[BOARD_SIZE - 2];
			tempStrip[BOARD_SIZE - 2] = tempSwap;
		}

		for(int j = 0; j < BOARD_SIZE; j++) {
			currentBoard[boardWhere(i, j)] = tempStrip[j];
		}
	}

	for(int i = 0; i < BOARD_SIZE && isDirYForward; i++) {
		int *tempStrip = (int*)malloc(sizeof(int*)*BOARD_SIZE);
		for(int j = 0; j < BOARD_SIZE; j++) {
			tempStrip[j] = currentBoard[boardWhere(j, i)];
		}
		
		tempStrip = compressStrip(tempStrip);
		if(isDirYForward) {
			int tempSwap = tempStrip[0];
			tempStrip[0] = tempStrip[BOARD_SIZE - 1];
			tempStrip[BOARD_SIZE - 1] = tempSwap;
			tempSwap = tempStrip[1];
			tempStrip[1] = tempStrip[BOARD_SIZE - 2];
			tempStrip[BOARD_SIZE - 2] = tempSwap;
		}

		for(int j = 0; j < BOARD_SIZE; j++) {
			currentBoard[boardWhere(j, i)] = tempStrip[j];
		}
	}

	for(int i = 0; i < BOARD_SIZE; i += BOARD_SIZE - 1) {
		for(int j = 0; j < BOARD_SIZE; j += BOARD_SIZE - 1) {
			if(currentBoard[boardWhere(i, j)] == 0) {
				currentBoard[boardWhere(i, j)] = 2;
				return currentBoard;
			}
		}
	}
	
	return currentBoard;
}

bool is2048(board currentBoard) {
	for(int i = 0; i < BOARD_SIZE; i++) {
		for(int j = 0; j < BOARD_SIZE; j++) {
			if(currentBoard[boardWhere(i, j)] >= MAX_NUMBER) {
				return true;
			}
		}	
	}
	
	return false;
}

int main() {
	printf("2048\n");
	board currentBoard = makeDefaultBoard();
	boardLine = makeBoardLine();
	while(!is2048(currentBoard)) {
		printBoard(currentBoard);
		int x, y;
		scanf("%d %d", &x, &y);
		currentBoard = moveBoard(currentBoard, direction(x, y));
		printf("\n\n");
	}

	printBoard(currentBoard);
	return 0;
}
