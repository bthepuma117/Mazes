/*
*
*	Mike Unger
*	Maze gen and solver
*   C++
*   display in ascii
*	opposite sides of maze are not connected
*/

enum DIRECTION{NORTH=0,EAST=1,SOUTH=2,WEST=3};

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
using namespace std;

class Cell
{
public:
	int row,column;
	Cell *surround[4];//keep track of neighbors
	bool links[4]={false};//keep track of linked cells hashtable might be better
	Cell()
	{
		;//TODO
	}
	Cell(int r,int c)
	{
		row=r;
		column=c;

	}
	~Cell()
	{
		//nothing to do here
		
	}
	bool is_at(int dir)
	{
		return surround[dir%4]==NULL;
	}
	void describe()
	{
		cout<<row<<" : "<<column<<endl;
	}
	void set_row(int r)
	{
		row=r;
	}
	void set_col(int c)
	{
		column=c;
	}
/****************************************************************

   FUNCTION:   link

   ARGUMENTS:  direction should line up with the enum as well as if its
   				a bidirectional link

   RETURNS:    void

   NOTES:      links the desired ajacent cells
****************************************************************/
	void link(int dir,bool bidi=true)
	{
		if(surround[dir]!=NULL)
		{
	
			links[dir]=true;//link the cells, ie no wall there
			if(bidi)
			{
				surround[dir]->link((dir+2)%4,false);//get opposite dirction cell 1's east(1) is also cell 2's west(3)

			}
		}
		return;
	}
/****************************************************************

   FUNCTION:   unlink

   ARGUMENTS:  direction: should match an ENUM

   RETURNS:    nothing

   NOTES:      unlinks the desired adjeacent cell
****************************************************************/
	void unlink(int dir,bool bidi=true)
	{
		if(surround[dir]!=NULL)
		{
			links[dir]=false;//unlink cell
			if(bidi)
			{
				surround[dir]->unlink((dir+2)%4,false);//unlink previously linked cell
			}
		}
	}
/****************************************************************

   FUNCTION:   is_linked

   ARGUMENTS:  Cell pointer

   RETURNS:    nothing

   NOTES:      determines if a given pointer is in a cells adj list
****************************************************************/
	bool is_linked(Cell *check)
	{
		for(int i=0; i<4;i++)
		{
			if((check == surround[i]) && links[i])
			{	

				return true;
			}

		}
		return false;

	}
	void unlink_all()
	{
		for (int i=0; i<4; i++)
		{
			links[i]=false;
		}

	}
	


	};
class Map
{
public:
	int rows,columns;
	Cell ** grid;
	Map(int r,int c)
	{
		rows=r;
		columns=c;
		make_grid();
		configure_cells();
		
	}
	~Map()
	{
		

		for(int i=0; i<rows; i++)
		{
			
				delete[] grid[i];
				//deletes all cells :o
			
			
		}
		delete grid;
	}
/****************************************************************

   FUNCTION:   make_grid

   ARGUMENTS:  none

   RETURNS:    nothing

   NOTES:      dynamically creates the 2d cell array

****************************************************************/
	void make_grid()
	{
		//grid = new Cell [rows][columns];
		grid=new Cell*[rows];
		for(int k=0;k<rows;k++)
		{
			grid[k]=new Cell[columns];
		}
		//creates a grid of blank grids :o
		for(int i=0;i<rows;i++)
		{
			for(int j=0;j<columns;j++)
			{
				grid[i][j].set_col(j);
				grid[i][j].set_row(i);
				//init all cells
			}
		}
		
	}
	//to make it wrap around do the math mod either col or row
/****************************************************************

   FUNCTION:   configure cells

   ARGUMENTS:  none

   RETURNS:    nothing

   NOTES:      sets up the surround list for each cell in the grid
   				does not create a wrap around maze 
****************************************************************/
	void configure_cells()
	{
		cout<<"cell config"<<endl;
		for(int i=0;i<rows;i++)
		{
			for(int j=0;j<columns;j++)
			{	
				//set north touching
				if(i!=0)
				{
				   grid[i][j].surround[NORTH]=&grid[i-1][j];
				}
				else
				{
					grid[i][j].surround[NORTH]=NULL;
				}
				//set east touching
				if(j!=(columns-1))
				{
				   grid[i][j].surround[EAST]=&grid[i][j+1];
				}
				else
				{
					grid[i][j].surround[EAST]=NULL;
				}
				//set south touching
				if(i!=(rows-1))
				{
				   grid[i][j].surround[SOUTH]=&grid[i+1][j];
				}
				else
				{
					grid[i][j].surround[SOUTH]=NULL;
				}
				//set west touching
				if(j!=0)
				{
				   grid[i][j].surround[WEST]=&grid[i][j-1];
				}
				else
				{
					grid[i][j].surround[WEST]=NULL;
				}


			}
		}


	}
/****************************************************************

   FUNCTION:   random_cell

   ARGUMENTS:  none

   RETURNS:    a pointer to a random cell

   NOTES:      out of bounds handled by modulo
****************************************************************/
	Cell* random_cell()
	{
		srand(time(NULL));
		int r=rand()%rows;
		int c=rand()%columns;
		//because as they say fuck me up fam
		return &(grid[r][c]);
	}
 /****************************************************************

   FUNCTION:   size

   ARGUMENTS:  none

   RETURNS:    returns size of the grid

   NOTES:      none
****************************************************************/
	int size()
	{
		return columns*rows;
	}
/****************************************************************

   FUNCTION:   get_row

   ARGUMENTS:  row number
   			   empty cell array size 4
   			   must be size 4

   RETURNS:    nothing

   NOTES:      should return a complete row of cells

****************************************************************/
	void get_row(int r, Cell** fill)
	{
		
		r=r%rows;
		
		for(int i=0;i<columns;i++)
		{
			fill[i]=&(grid[r][i]);
		}
		
	}
 /****************************************************************

   FUNCTION:   print

   ARGUMENTS:  none

   RETURNS:    nothing

   NOTES:     prints the ascii representation of the maze
   				naviaget to stringstream?
****************************************************************/
	void print()
	{
		string top,bottom,body,east_b,south_b;
		cout<<"+";
		body="   ";
		
		for(int i=0;i<columns;i++)
		{
			cout<<"---+";

		}
		cout<<endl;
		//get the indiv rows and print to string
		for(int j=0; j<rows;j++)
		{
			top="|";
			bottom="+";
			
			for(int t=0;t<columns;t++)
			{
				east_b= ((grid[j][t].is_linked(&grid[j][(t+1)%columns]))? " " : "|");
				south_b=((grid[j][t].is_linked(&grid[(j+1)%rows][t]))? "   " : "---");
				top+=body;
				top+=east_b;
				bottom+=south_b;
				bottom+="+";

			}
			cout<<top<<endl<<bottom<<endl;
			

		}


	}
	void reset()
	{
		for(int i =0; i<rows; i++)
		{
			for(int j=0; j<columns; j++)
			{
				grid[i][j].unlink_all();
			}


		}

	}
	

} ;
 /****************************************************************

   FUNCTION:   binary_create

   ARGUMENTS:  map

   RETURNS:    nothing

   NOTES:      takes an unconnected map and creates a maze using
   				the binary tree method
****************************************************************/
void binary_create(Map & todo)
{
	int rows=todo.rows;
	int columns=todo.columns;
	int dir;
	srand(time(NULL));
	for(int i=0; i<rows;i++)
	{
		for(int j=0; j<columns;j++)
		{
			dir= rand()%2 + 1; //should gen either East or South
			if((j)==columns-1)
			{
				dir=2;
			}
			if((i)==rows-1)
			{
				dir = 1;
			}
			
			todo.grid[i][j].link(dir);
		}
	}

}

 /****************************************************************

   FUNCTION:   side_winder

   ARGUMENTS:  map

   RETURNS:    nothing

   NOTES:      takes an unconnected map and creates a maze using
   				the side wander method
****************************************************************/

void side_wander(Map &todo)
{

	//North=0,east,south,west;
	bool at_east, at_south,close_out;
	int end_of_last_run=0;
	int col_tot= todo.columns;
	int row_tot= todo.rows;
	Cell * Row[col_tot];//array of cell pointers to hold a row of cells :o
	//dont delete Row because thats not your memory :o
	int cell_run,ran_cell;
	srand(time(NULL));
	//grab a row
	for(int cur_row=0; cur_row<row_tot;cur_row++)
	{	
			
		end_of_last_run=0;
		cell_run=1;//there will always be one cell in a cell run :o
		todo.get_row(cur_row,Row);
		//process the row
		for(int cur_cell=0; cur_cell<col_tot;cur_cell++)
		{
			
			at_east=(Row[cur_cell]->surround[1]==NULL);//is it at the end?
			at_south=(Row[cur_cell]->surround[2]==NULL);//is it at the bottom?
			close_out = (at_east)|| (rand()%2==0);
			
			if(close_out && (!at_south))//dont erase things south on the last row :o
			{
				
				ran_cell=cur_cell;
				ran_cell = (rand()%(cell_run))+end_of_last_run; // select random cell from the cells gone through
				end_of_last_run+=cell_run;
				Row[ran_cell]->link(SOUTH,true);
				cell_run=1;
			}
			else
			{
				cell_run++;		
				Row[cur_cell]->link(EAST);
					
			}

			at_east=at_south=false;

		}
		
	}

}


//simple driver
int main(int argc, char *argv[])
{
	int x = 5;
	int y = 5;
	if(argc >=3)
	{
		x = atoi(argv[1]);
		y = atoi(argv[2]);
	}
	Map maze(y,x);
	binary_create(maze);
	maze.print();
	maze.reset();
	maze.print();
	
	side_wander(maze);
	maze.print();

	return 0;

}