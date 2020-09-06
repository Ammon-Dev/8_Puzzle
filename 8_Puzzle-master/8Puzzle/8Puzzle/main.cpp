#include <iostream>
#include <stack>
#include <stdlib.h>
#include <vector>
#include <queue>
#include <functional>


std::vector<std::vector<int>> goal{ {1,2,3},{4,5,6},{7,8,0} };//the goal state

std::vector<std::vector<int>> initial{ {1,2,3},{4,5,6},{7,0,8} };//the first state

struct Node
{
	Node* parent=NULL;//node from
	std::vector<std::vector<int>> puzzle;//the current puzzle state
	int gapx;//current location of the gap
	int gapy;
	int pathCost;//stores the current path cost
	int fcost;
};

//a comparision class to determine the minimum cost node in the prioity queue
class compare {//compares fcost values and returns true and false based on the values 
public:
	bool operator() (const Node & list, const Node & current)
	{
		return list.fcost > current.fcost;
	}
};

std::priority_queue<Node, std::vector<Node>, compare> pq;//global priority queue that has the minimum costing node at the top

void printPuzzle(Node current)//prints the current puzzle
{
	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			std::cout << current.puzzle[x][y] << " ";
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}


int findH(Node current)//finds the manhatten distance heuristic
{
	int sum=0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (current.puzzle[i][j] != 0) {
				for (int x = 0; x < 3; x++)
				{
					for (int y = 0; y < 3; y++)
					{
						if (current.puzzle[i][j] == goal[x][y])
							sum += abs(i - x) + abs(j - y);
					}
				}
			}
		}
	}
	return sum;
}

int findF(Node current) 
{
	int h = findH(current);
	int sum=0;
	sum += h+current.pathCost;
	return sum;
}


bool confirmGoal(Node current)//compares the current node to the goal
{
	for (int i = 0; i < 3; i++) 
	{
		for (int j = 0; j < 3; j++)
		{
			if (goal[i][j] != current.puzzle[i][j])
				return false;
		}
	}
	return true;
}


//these confirm the directions you can move
bool confirmUp(int gapx) //confirms the gap in the puzzle can go up
{
	if (gapx > 0)//if not in the top row
		return true;
	return false;
}

bool confirmLeft(int gapy)//confirms the gap in the puzzle can go left
{
	if (gapy>0)//if not in the first column
		return true;
	return false;
}

bool confirmRight(int gapy)//confirms the gap in the puzzle can go right
{
	if (gapy<2)//if not in the last column
		return true;
	return false;
}

bool confirmDown(int gapy)//confirms the gap in the puzzle can go down
{
	if (gapy < 2)//if not in the last row
		return true;
	return false;
}


//these swap the appropriate tiles when called and pushs it onto the queue
void moveUp(Node* current) 
{
	Node* temp = new Node;
	temp = current;//makes the next puzzle
	std::swap(temp->puzzle[temp->gapx][temp->gapy], temp->puzzle[temp->gapx-1][temp->gapy]);//swaps the two tiles
	temp->pathCost++;//increases the path cost
	temp->parent = current;//sets the last node to the parent of this one
	temp->fcost = findF(*temp);//gets the full cost of the new node
	pq.push(*temp);//pushes the new node unto the queue
}

void moveDown(Node* current)
{
	Node* temp = new Node;
	temp = current;
	std::swap(temp->puzzle[temp->gapx][temp->gapy], temp->puzzle[temp->gapx + 1][temp->gapy]);//swaps the gap and the tile up
	temp->pathCost++;
	temp->parent = current;
	temp->fcost = findF(*temp);
	pq.push(*temp);
}

void moveLeft(Node* current)
{
	Node* temp = new Node;
	temp = current;
	std::swap(temp->puzzle[temp->gapx][temp->gapy], temp->puzzle[temp->gapx][temp->gapy-1]);//swaps the gap and the tile to the left
	temp->pathCost++;
	temp->parent = current;
	temp->fcost = findF(*temp);
	pq.push(*temp);
}

void moveRight(Node* current)
{
	Node* temp = new Node;
	temp = current;
	std::swap(temp->puzzle[temp->gapx][temp->gapy], temp->puzzle[temp->gapx][temp->gapy + 1]);//swaps the gap and the tile to the right
	temp->pathCost++;
	temp->parent = current;
	temp->fcost = findF(*temp);
	pq.push(*temp);
}


//checks every move and if they can happen calls the function to make the state
void createChild(Node current) 
{
	if (confirmUp(current.gapx))
		moveUp(&current);

	if (confirmDown(current.gapx))
		moveDown(&current);
	
	if (confirmLeft(current.gapy))
		moveLeft(&current);

	if (confirmRight(current.gapy))
		moveRight(&current);
}


void solve()
{
	int fsum;//inital fsum
	Node* start = new Node;//the inital node
	start->puzzle = initial;//sets the state of the first node
	start->pathCost = 0;//sets the path cost of the first node
	fsum = findF(*start);//find the fcost of the first node
	start->fcost = fsum;//sets the fcost of the first node
	start->gapx = 2;//sets the y coord of the gap of the first node
	start->gapy = 1;//sets the x coord of the gap of the first node
	pq.push(*start);//pushes the first node onto the queue
	while (confirmGoal(pq.top())==false)//while not at the end goal
	{
		Node temp;
		temp=pq.top();
		pq.pop();
		printPuzzle(temp);
		createChild(temp);//makes children of the best node on the queue
	}
	int x;
	printPuzzle(pq.top());
	std::cin >> x;
}

int main()
{
	solve();
}
