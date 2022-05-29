#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int rangeRandom(int min, int max) {
	return min + (rand() % max);
}

struct Maze {
	char *grid;
	char width, height;
};

char isCellWallAt(char walls, int whichWall) {
	return ((1 << whichWall) & walls) == (1 << whichWall);
}

struct SizeInfo {
	int width;
	int height;
};

int coordIn(int x, int y, struct SizeInfo sizeInfo) {
	if(
		x < 0 ||
		y < 0 ||
		x > sizeInfo.width ||
		y > sizeInfo.height
	) {
		fprintf(stderr, "[ERR] coordIn(): (x, y) are out of bounds.\n");
		return -1;
	}

	return (y * sizeInfo.width) + x;
}

short coordInMaze(
	char x, 
	char y, 
	struct Maze maze
) {
	if(
		x < 0 ||
		y < 0 ||
		x > maze.width ||
		y > maze.height
	) {
		fprintf(stderr, "[ERR] coordInMaze(): (x, y) are out of bounds.\n");
		return -1;
	}

	return (y * maze.width) + x;
}

char getMazeCellWalls(
	char x, 
	char y, 
	struct Maze maze
) {
	return maze.grid[coordInMaze(x, y, maze)];
}

void setMazeCellWalls(
	char x, 
	char y, 
	char walls, 
	struct Maze *maze
) {
	short coord = coordInMaze(x, y, *maze);
	maze->grid[coord] = walls;
}

char makeWalls(
	char left,
	char top,
	char right,
	char bottom,
	char visited
) {
	return (
		(left) |
		(top << 1) |
		(right << 2) |
		(bottom << 3) |
		(visited << 4)
	);
}

char wallToX(char wall) {
	wall = 1 << wall;
	return -isCellWallAt(wall, 0) + isCellWallAt(wall, 2);
}

char wallToY(char wall) {
	wall = 1 << wall;
	return -isCellWallAt(wall, 1) + isCellWallAt(wall, 3);
}

// maze thing here.
short cellsDone;
short sameCellCount;
void mazify(
	char x,
	char y,
	struct Maze *maze
) {
	if(
		x < 0 ||
		y < 0 ||
		y > maze->height ||
		x > maze->width
	) {
		return;
	}

	if(
		isCellWallAt(
			getMazeCellWalls(x, y, *maze),
			4
		)
	) {
		sameCellCount++;
		return;
	}

	maze->grid[coordInMaze(x, y, *maze)] |= (1 << 4);
	cellsDone++;
	sameCellCount = 0;
	char nextWall = 0;
	char nextX = 0;
	char nextY = 0;
	while(
		sameCellCount <= (maze->width - 1) * (maze->height - 1)
	) {	
		nextWall = rangeRandom(0, 4);
		nextX = x + wallToX(nextWall);
		nextY = y + wallToY(nextWall);
		maze->grid[coordInMaze(x, y, *maze)] ^= 1 << nextWall;
		if(
			nextX >= 0 &&
			nextY >= 0 &&
			nextX <= maze->width &&
			nextY <= maze->height
		) {
			maze->grid[coordInMaze(nextX, nextY, *maze)] 
				^= 1 << ((nextWall + 2) % 4);
			
			mazify(nextX, nextY, maze);
		}
	}
}

void carveMaze(struct Maze *maze) {
	for(unsigned char y = 0; y < maze->height; y++) {
		for(unsigned char x = 0; x < maze->width; x++) {
			mazify(x, y, maze);
		}
	}
}

struct Maze makeMaze(
	char width, 
	char height
) {
	struct Maze result = {
		.width = width,
		.height = height
	};

	result.grid = 
		(char*)malloc(sizeof(char*) * width * height);

	for(char y = 0; y < height; y++) {
		for(char x = 0; x < width; x++) {
			setMazeCellWalls(x, y, makeWalls(1, 1, 1, 1, 0), &result);
		}
	}

	cellsDone = 0;
	sameCellCount = 0;
	carveMaze(&result);
	return result;
}

void freeMaze(struct Maze *maze) {
	free(maze->grid);
}

void printMaze(struct Maze maze) {
	char *buffer =
		(char*)malloc(
			sizeof(char *) * 
			maze.width * 3 * 
			maze.height * 3);

	struct SizeInfo sizeInfo = {
		.width = maze.width * 3,
		.height = maze.height * 3
	};
	
	for(char y = 0; y < maze.height; y++) {
		for(char x = 0; x < maze.width; x++) {
			char cellWalls = getMazeCellWalls(x, y, maze);

			// First row.
			buffer[coordIn((x * 3), (y * 3), sizeInfo)] = 1;
			buffer[coordIn((x * 3) + 1, (y * 3), sizeInfo)] = 
				isCellWallAt(cellWalls, 1);
			
			buffer[coordIn((x * 3) + 2, (y * 3), sizeInfo)] = 1;

			// Second row.
			buffer[coordIn((x * 3), (y * 3) + 1, sizeInfo)] = 
				isCellWallAt(cellWalls, 0);
			
			buffer[coordIn((x * 3) + 1, (y * 3) + 1, sizeInfo)] = 0;
			buffer[coordIn((x * 3) + 2, (y * 3) + 1, sizeInfo)] = 
				isCellWallAt(cellWalls, 2);

			// Third row.
			buffer[coordIn((x * 3), (y * 3) + 2, sizeInfo)] = 1;
			buffer[coordIn((x * 3) + 1, (y * 3) + 2, sizeInfo)] = 
				isCellWallAt(cellWalls, 3);
			
			buffer[coordIn((x * 3) + 2, (y * 3) + 2, sizeInfo)] = 1;
		}
	}

	for(int y = 0; y < sizeInfo.height; y++) {
		for(int x = 0; x < sizeInfo.width; x++) {
			printf("%c", 
				buffer[coordIn(x, y, sizeInfo)]
				 ? '#'
				 : ' '
			);
		}

		printf("\n");
	}

	free(buffer);
}

int main() {
	printf("\x1b[0;0f\x1b[2J\x1b[0;0f");
	srand(time(NULL));
	struct Maze maze = makeMaze(25, 25);
	printMaze(maze);
	freeMaze(&maze);
	printf("\n");
	return 0;
}
