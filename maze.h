
#ifndef MAZE_H
#define MAZE_H

#include <string>
#include <iostream>
#include <vector>


enum wallStatus_t {
	NO_CONNECT,
	UNKNOWN,
	WALL_EXISTS,
	WALL_DNE
};

// choose if the maze graph has symmetric edges
#define SYMMETRIC_EDGES (true)

struct Coord {
	int x;
	int y;
};

struct Edge {
	wallStatus_t status;
        bool pic;
	Edge() : status(NO_CONNECT),pic(true) {}
        
};

// extra information attached to each square
struct Vertex {
	bool visited;
	bool end;
	bool start;
	Vertex() :visited(false),end(false),start(false) {}
};

class Maze
{
private:
	// size of grid
	int height, width;

	// for each square
	int num_vert;
	std::vector<Vertex> vertices;

	// potential walls
	std::vector< std::vector<Edge> > edges;

public:
	// constructor and deconstructor
	Maze(const int h, const int w);
	Maze();
	~Maze();

        int getExploredNodes();

	// management functions
	void scaleMaze(const int h, const int w);

	// acessors for vertices and edges
	Edge& getEdge(const int k, const int l);
	void setEdge(const int k, const int l, const Edge& e);
	Vertex& getVertex(const int k);
	void setVertex(const int k, const Vertex& v);

	// special function accessors
	void setStartVertex(const int k);
	void setEndVertex(const int k);

	// check wall by index or coordinate
	int checkWall(const int k, const int l);
	bool checkWallOpen(const int k, const int l);

	// convertes between coords and linear index
	int getIndex(const int x, const int y);
	Coord getCoord(const int k);

	// return a vector with valid next moves with or without the exception list
	std::vector<int> getValidMoves(const int k);
	std::vector<int> getValidMoves(const int k, std::vector<int> dontInclude);
	
	// pathfinding
	bool areConnected(const int k, const int l);
	std::vector<int> getPath(const int k, const int l);

	// exhaustive search functions
	std::vector<int> getPathExplore(const int k);

private:
	// pathfinding behind the scenes
	bool getPath_r(const int k, const int l, std::vector<int>& path);

};

// include function definitions
#include "maze.hpp"

#endif
