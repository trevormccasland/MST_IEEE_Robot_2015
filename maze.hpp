
#ifndef MAZE_HPP
#define MAZE_HPP

#include <exception>
#include <map>
#include <algorithm>
#include <cmath>

using namespace std;


Maze::Maze()
{
	// default maze height and width
	scaleMaze(5, 5);
}

Maze::Maze(const int h, const int w)
{
	// store height and width of maze
	scaleMaze(h, w);
}

void Maze::scaleMaze(const int h, const int w)
{
	height = h;
	width = w;

	// delete whatever was previously in verts and edges
	vertices.clear();
	edges.clear();

	// init vertices in vertex vector
	num_vert = h*w;
	Vertex initV;
	for (int i = 0; i < num_vert; i++)
	{
		vertices.push_back(initV);
	}

	// init edge adjacency matrix
	Edge initE;
	vector<Edge> edgeRow(num_vert, initE);
	for (int i = 0; i < num_vert; i++)
	{
		edges.push_back(edgeRow);
	}


	// init adjacency matrix to represent grid
	Edge connectedEdge;
	connectedEdge.status = UNKNOWN;
	for (int i = 0; i < num_vert; i++)
	{
		// convert to xy, and add appropriate verticies
		Coord c = getCoord(i);
		Coord adjc[4];
		adjc[0] = { c.x + 1, c.y };
		adjc[1] = { c.x - 1, c.y };
		adjc[2] = { c.x, c.y + 1 };
		adjc[3] = { c.x, c.y - 1 };

		// go through every vertex adjacent to the ith vertex
		for (int j = 0; j < 4; j++)
		{
			// if the adjacent vertex is in the maze (not outside boundaries)
			if (adjc[j].x >= 0 && adjc[j].x < width && adjc[j].y >= 0 && adjc[j].y < height)
			{
				// convert source and destination to linear indexes
				int toIndex = getIndex(adjc[j].x, adjc[j].y);
				int fromIndex = i; // just hepful to understand ;)

				// update both sides
				edges[toIndex][fromIndex] = connectedEdge;
				edges[fromIndex][toIndex] = connectedEdge;
			}
		}
	}

	return;
}






Maze::~Maze()
{

	// currently empty because of the magic of stl containers
}


Edge& Maze::getEdge(const int k, const int l)
{
	return edges[k][l];
}

void Maze::setEdge(const int k, const int l, const Edge& e)
{
	edges[k][l] = e;
	if (SYMMETRIC_EDGES)
		edges[l][k] = e;
	return;
}

Vertex& Maze::getVertex(const int k)
{
	return vertices[k];
}

void Maze::setVertex(const int k, const Vertex& v)
{
	vertices[k] = v;
	return;
}


void Maze::setStartVertex(const int k)
{
	vertices[k].start = true;
	return;
}

void Maze::setEndVertex(const int k)
{
	vertices[k].end = true;
	return;
}



int Maze::checkWall(const int k, const int l)
{
	Edge e = edges[k][l];
	return e.status;
}


bool Maze::checkWallOpen(const int k, const int l)
{
	if (k >= num_vert || k < 0 || l >= num_vert || l < 0)
		return false;
	return checkWall(k,l) == WALL_DNE;
}



// convertes coords to linear index
int Maze::getIndex(const int x, const int y)
{
	return width*y + x;
}

// convertes a linear indext to a coordinate
Coord Maze::getCoord(const int k)
{
	Coord c;
	c.x = k % width;
	c.y = k / width;
	return c;
}




vector<int> Maze::getValidMoves(const int k)
{
	vector<int> validMoves;

	// go through k's corresponding row in the adjacency matrix
	for (int i = 0; i < num_vert; i++)
	{
		// check if there is a wall
		if (checkWallOpen(k,i))
		{
			validMoves.push_back(i);
		}
	}

	return validMoves;
}


vector<int> Maze::getValidMoves(const int k, vector<int> dontInclude)
{
	vector<int> validMoves = getValidMoves(k);
	for (vector<int>::iterator it = dontInclude.begin(); it != dontInclude.end(); it++)
	{
		// if dontInclude is in validMoves, remove it from validMoves
		vector<int>::iterator vit = std::find(validMoves.begin(), validMoves.end(), *it);
		if (vit != validMoves.end())
			validMoves.erase(vit);
	}

	return validMoves;
}


bool Maze::areConnected(const int k, const int l)
{
	bool connected = false;
	vector<int> path = getPath(k,l);

	if (path.size() > 1)
		connected = true;

	return connected;
}

std::vector<int> Maze::getPath(const int k, const int l)
{
	if ((k < 0) || (k > num_vert) || (l < 0) || (l > num_vert))
		throw;
	vector<int> path;

	// consider the kth element in the path
	path.push_back(k);

	// check if the path is connected
	bool connected = getPath_r(k, l, path);

	return path;
}

bool Maze::getPath_r(const int k, const int l, vector<int>& path)
{
	// get options with the exception of verticies already traversed
	vector<int> options = getValidMoves(k, path);
        for(int i=0; i <options.size(); i++)
        {
          cout<<"option "<<i<<" : "<<options[i]<<endl;
        }
	vector < vector<int> > solutions;

	for (vector<int>::iterator opt = options.begin(); opt != options.end(); ++opt)
	{
		path.push_back(*opt);
		if (*opt == l)
		{
			// just found a solution, so start to bubble back up
			return true;
		}
		else
		{
			// down rabbit hole
			vector<int> newPath = path;
			bool connected = getPath_r(*opt, l, newPath);

			// check if the rabbit hole was a success
			if (connected)
			{
				// add the the list of solutions
				solutions.push_back(newPath);
			}

			// remove this potential path from the list
			path.pop_back();
		}
	}

	// if any solutions completed, choose a best one
	bool connected = false;
	if (solutions.size() > 0)
	{
		// find the shortest path
		int k;
		unsigned int shortestPath = pow(2,31);
		for (unsigned int i = 0; i < solutions.size(); i++)
		{
			if (solutions[i].size() < shortestPath)
			{
				k = i;
				shortestPath = solutions[i].size();
			}
		}

		// set the correct path to be the best solution
		path = solutions[k];


		connected = true;
	}

	// if no options were successful
	return connected;
}



std::vector<int> Maze::getPathExplore(const int k)
{
	vector<int> path;
	int minLength = pow(2,31);
	
	// check all untraversed vectors, and determine if they are connected
	for (int i = 0; i < vertices.size(); i++)
	{

               // cout<<i<<endl;
		// if the path is valid
		vector<int> currentPath = getPath(k, i);
		if (currentPath.size() < minLength && currentPath.size() > 1 && vertices[i].visited == false)
		{
			// record as the path to take
			minLength = currentPath.size();
			path = currentPath;
		}
	}

	return path;
}

int Maze::getExploredNodes()
{
  int numExplored=0;
  for(int i=0; i<vertices.size(); i++)
  {
    if(vertices[i].visited)
    {
       numExplored++;
    }
  }

  return numExplored;
}


#endif
