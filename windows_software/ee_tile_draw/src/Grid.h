#ifndef LOUIS_GRID_H
#define LOUIS_GRID_H


/*
Usage:

The Tile class is not meant to be used by the outside. It's meant
to be used by the Grid class only. The "Tile" is just a collection
of data --- it has no methods
*/

class Tile
{
public:
	int x; //x coordinate in the grid
	int y; //y coordinate
	int component_id; //can be zero --- which means no picture
	Color foreground;
	Color background;
	Tile* pNext; //next Tile in the link list

	//constructor will give default values to member variables
	Tile();

	//destructor to get rid of the link list "Tile* pNext
	//correctly
	~Tile();
};



/*
Usage:

This object is constructed globally in the program for easy
memory management and access

There are many public member functions for this class, so
a detail description of usage is omitted --- look at the
comments for the public member functions to see what they
do.

But here is the overview of usage:

you can mark, unmark "Tile" objects in the grid:
add_a_tile(...), clear_tile(...), reset()

The currently selected tile is tracked as (current_x, current_y)
and you can change the currently selected tile using:
move_down(), move_up(), move_left(), move_right()
move_down_a_screen(), move_up_a_screen(), move_left_a_screen(), move_right_a_screen()
mouse_select(...)

change the current zoom level (change how many 
tiles are displayed on screen) by accessing:
add_horz_tile(), add_vert_tile(), 
minus_horz_tile(), minus_vert_tile()

The "Grid" object will be drawn on screen within a rectangle called:
drawing_area
and this rectangle is set via the public member function
set_drawing_area(...)

draw_to_screen(&graphics) will draw the tiles that are
currently displayed onto the "graphics" object 

draw_all(&graphics) will draw all tiles to the "graphics"
object

*/

class Grid
{
private:
	Tile* m_tiles[HASH_TABLE_SIZE];

	//these next two numbers keep track of what tile the user is
	//currently on; range is global constant: 
	int current_x;
	int current_y;

	//these numbers  keep track of how many tiles are displayed
	//per screen
	int x_tiles_per_screen;
	int y_tiles_per_screen;

	//these numbers keep track of what is the upper left tile's
	//coordinate
	int upper_left_tile_x;
	int upper_left_tile_y;

	//to track the size of the entire picture, store the range of
	//x, y coordinates drawn by the user
	int user_min_x; //the left most tile drawn by the user
	int user_min_y; //the upper most tile drawn by the user
	int user_max_x; //and so on...
	int user_max_y;
	
	//just about every public method would need to invalidate
	//part of the screen to trigger a redraw
	HWND parent_wnd;
	
	//helper functions

	//this function is the one that handles the zooming in general
	void zoom(int new_x_tiles_per_screen, int new_y_tiles_per_screen);

	//this function returns a RECT structure containing the tile
	//in terms of pixel coordinates
	//example: get_tile_rect(0, 0); returns a RECT containing the
	//upper left tile on screen
	RECT get_tile_rect(int index_x, int index_y);

	//this function will return a RECT structure containing the
	//"drawing_area" suitable for repainting the "drawing_area"
	RECT get_drawing_area_rect();

	//this function maps a coordinate (x, y) to an index in m_tiles
	int compute_address(int x, int y);

	//initializes the member variables; this function is called
	//by the constructor and "reset()"
	void init_variables();

	//this function will reture a pointer to the tile at coordinate
	//(x, y); it will return a NULL if such tile does not exist
	Tile* get_tile(int x, int y);

	//the grid itself won't occupy the entire client area, only
	//a part of it. "drawing_area" is the part of the client window
	//that contains the part of "Grid" that is shown on screen		
	Rect drawing_area;

	//two arrays of x, y coordinates to map out the exact pixel
	//extend of each tile.
	//example: the upper left most tile on screen will extend from
	//(x_coords[0], y_coords[0]) to (x_coords[1] - 1, y_coords[1] - 1)
	int x_coords[MAX_X_TILES_PER_SCREEN + 1];
	int y_coords[MAX_Y_TILES_PER_SCREEN + 1];	

	//special case: the large components needs a larger invalid
	//rectangle so they get redrawn properly
	//this includes the "op-amp"  "2 port black box" components
	//plus two classes of "wire" components
	void check_for_large_rect(RECT& rect, int component_id);

public:
	Grid();
	void set_parent_wnd(HWND hWnd); //this handle is not available at
			//construction time

	~Grid();

	//-------------------------------------------------------
	//navigation functions:
	//user can use cursor keys to navigate from one tile to 
	//the other

	//all of these have error checking to make sure we do not 
	//go beyond the coordination limits --- but none of them
	//return a value to indicate error because these errors
	//are so rare --- from the starting point to the edge of the
	//coordinate system is over 1 billion tiles.

	//so if we somehow do get to the upper edge of the coordinate
	//system, a function that results in us moving off the edge
	//or displaying tiles that are off the edge will simply have 
	//no effect on the internal data structures
	void move_up();
	void move_left();
	void move_right();
	void move_down();

	void move_up_a_screen();
	void move_left_a_screen();
	void move_right_a_screen();
	void move_down_a_screen();

	//user can zoom in and out
	void add_horz_tile();
	void minus_horz_tile();
	void add_vert_tile();
	void minus_vert_tile();

	//user can select a tile with the mouse. This function will
	//update current_x, current_y, given the user's click location
	void mouse_select(int pixel_x, int pixel_y);

	//end of navigation functions
	//-------------------------------------------------------


	//-------------------------------------------------------
	//functions to work with the "Tile" objects in this grid

	//used when the user wants to start a new drawing
	//it will clear delete all Tile objects and point
	//all m_tiles to NULL
	void reset();

	//clear just one tile --- delete it from the m_tiles list if
	//it exists
	void clear_tile(); //clears current tile
	void clear_tile(int x, int y);

	//new tile if the currently selected tile is empty; modify
	//the currently selected tile if it already exists
	void add_a_tile(int component_id, Color foreground, Color background);

	//function to rotate the content of the currently selected tile
	void rotate_tile();

	//function to apply a forecolor to the currently selected tile
	void apply_color(Color foreground, Color background);

	//returns the various IDs of the currently selected tile
	//hence allowing the outside world to determine what is
	//at the currently selected tile
	int get_class_id();
	int get_type_id();
	int get_orientation_id();
	
	//end of functions to work with the "Tile" objects in this grid
	//-------------------------------------------------------


	//-------------------------------------------------------
	//code to draw the "Grid" object
	
	void set_drawing_area(Rect& rect);

	//draw part of this Grid object to screen
	void draw_to_screen(Graphics* graphics);

	//this will update the user_min_x, user_min_y, user_max_x
	//user_max_y variables to see that they truely describe
	//the boundary of the object --- for example, that there is 
	//truely at least one drawn tile at the column of "user_min_x"
	//It is meant to be call before the member function "draw_all" 
	void trim_picture();

	/* draw_all(...), get_picture_width(...), 
	and get_picture_height(...) functions are expected to be
	preceded by trim_picture();
	*/

	//draw all of this Grid object to the "graphics" pointer
	//persumably "graphics" here is base on an image object, 
	//which is in turn based on a file object --- so we are
	//drawing to a file	
	void draw_all(Graphics* graphics);

	//this will return the width of the picture when the
	//whole thing is saved to disk; it is meant to be called
	//before draw_all so the user of this class will know
	//how large an "Image" object to allocate
	int get_picture_width();

	//this will return the height of the picture when the
	//whole thing is saved to disk;
	int get_picture_height();

	//end of code to draw the "Grid" object
	//-------------------------------------------------------
};

#endif