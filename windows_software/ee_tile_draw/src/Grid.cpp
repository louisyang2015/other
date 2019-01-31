#include <windows.h>

//-----------------------------------------------------------
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")
//-----------------------------------------------------------

#include "global_constants.h"
#include "Component.h"
#include "Grid.h"

//here are the easy functions
Tile::Tile()
{
	x = 0;
	y = 0;
	component_id = 0;
	foreground = Color::Black;
	background = Color::White;
	pNext = NULL;
}
Tile::~Tile()
{
	if(pNext != NULL)
		delete pNext;
	pNext = NULL;
}
Grid::Grid()
{
	int i = 0;
	for(i = 0; i < HASH_TABLE_SIZE; i++)
		m_tiles[i] = NULL;

	x_tiles_per_screen = 7;
	y_tiles_per_screen = 5;

	drawing_area.X = 0;
	drawing_area.Y = 0;
	drawing_area.Width = 0;
	drawing_area.Height = 0;

	for(i = 0; i < MAX_X_TILES_PER_SCREEN + 1; i++)
		x_coords[i] = -1;

	for(i = 0; i < MAX_Y_TILES_PER_SCREEN + 1; i++)
		y_coords[i] = -1;

	init_variables();
}
void Grid::init_variables()
{
	current_x = 1 << 30;
	current_y = 1 << 30;		

	upper_left_tile_x = current_x;
	upper_left_tile_y = current_y;

	user_min_x = current_x;
	user_max_x = current_x;
	user_min_y = current_y;
	user_max_y = current_y;
}
void Grid::set_parent_wnd(HWND hWnd)
{
	parent_wnd = hWnd;
}
Grid::~Grid()
{
	reset();	
}
void Grid::move_up_a_screen()
{
	if((upper_left_tile_y - y_tiles_per_screen) > MIN_TILE_COORD)
	{
		current_y -= y_tiles_per_screen;
		upper_left_tile_y -= y_tiles_per_screen;
		RECT rect = get_drawing_area_rect();
		InvalidateRect (parent_wnd, &rect, TRUE);
	}
}
void Grid::move_left_a_screen()
{
	if((upper_left_tile_x - x_tiles_per_screen) > MIN_TILE_COORD)
	{
		current_x -= x_tiles_per_screen;
		upper_left_tile_x -= x_tiles_per_screen;
		RECT rect = get_drawing_area_rect();
		InvalidateRect (parent_wnd, &rect, TRUE);
	}
}
void Grid::move_right_a_screen()
{
	if((upper_left_tile_x + 2 * x_tiles_per_screen - 1 ) < MAX_TILE_COORD)
	{
		current_x += x_tiles_per_screen;
		upper_left_tile_x += x_tiles_per_screen;
		RECT rect = get_drawing_area_rect();
		InvalidateRect (parent_wnd, &rect, TRUE);
	}
}
void Grid::move_down_a_screen()
{
	if((upper_left_tile_y + 2 * y_tiles_per_screen - 1 ) < MAX_TILE_COORD)
	{
		current_y += y_tiles_per_screen;
		upper_left_tile_y += y_tiles_per_screen;
		RECT rect = get_drawing_area_rect();
		InvalidateRect (parent_wnd, &rect, TRUE);
	}
}
void Grid::add_horz_tile()
{
	zoom(x_tiles_per_screen + 1, y_tiles_per_screen);
	//use set_drawing_area --- to update the x_coords, 
	//y_coords arrays
	set_drawing_area(drawing_area);
	RECT rect = get_drawing_area_rect();
	InvalidateRect (parent_wnd, &rect, TRUE);
}
void Grid::minus_horz_tile()
{
	zoom(x_tiles_per_screen - 1, y_tiles_per_screen);
	set_drawing_area(drawing_area);
	RECT rect = get_drawing_area_rect();
	InvalidateRect (parent_wnd, &rect, TRUE);
}
void Grid::add_vert_tile()
{
	zoom(x_tiles_per_screen, y_tiles_per_screen + 1);
	set_drawing_area(drawing_area);
	RECT rect = get_drawing_area_rect();
	InvalidateRect (parent_wnd, &rect, TRUE);
}
void Grid::minus_vert_tile()
{
	zoom(x_tiles_per_screen, y_tiles_per_screen -1);
	set_drawing_area(drawing_area);
	RECT rect = get_drawing_area_rect();
	InvalidateRect (parent_wnd, &rect, TRUE);
}
void Grid::reset()
{
	int i = 0;
	for(i = 0; i < HASH_TABLE_SIZE; i++)
	{
		if(m_tiles[i] != NULL)
			delete m_tiles[i];

		m_tiles[i] = NULL;
	}

	init_variables();

	RECT rect = get_drawing_area_rect();
	InvalidateRect (parent_wnd, &rect, TRUE);
}
int Grid::compute_address(int x, int y)
{
	int upper_bits = x & ADDRESS_MASK;
	upper_bits = upper_bits << ADDRESS_MASK_SIZE;

	int lower_bits = y & ADDRESS_MASK;
	
	return (upper_bits + lower_bits);
}
RECT Grid::get_tile_rect(int index_x, int index_y)
{
	RECT rect;

	//because the rectangle returned by this function will actually
	//guide the re-paint operations, so the rectangles should be
	//a little larger than the tile themselves just to be safe
	//For example: the tile that the user selected is actually
	//bigger than the basic non-selected tile due to a border
	//marking the user selection
	rect.left = x_coords[index_x] - 5;
	rect.top = y_coords[index_y] - 5;
	rect.right = x_coords[index_x + 1] + 5;
	rect.bottom = y_coords[index_y + 1] + 5;

	return rect;
}
RECT Grid::get_drawing_area_rect()
{
	RECT rect;

	//similar to get_tile_rect(...), the rect being returned
	//needs to be slightly larger than the drawing_area
	rect.left = drawing_area.X - 5;
	rect.top = drawing_area.Y - 5;
	rect.right = drawing_area.X + drawing_area.Width + 5;
	rect.bottom = drawing_area.Y + drawing_area.Height + 5;

	return rect;
}
void Grid::clear_tile()
{
	clear_tile(current_x, current_y);
}
//end of the easy functions


//--------------------------------------------------------------
//the more complicated navigation issues

//this function will check to see if the new x_tiles_per_screen
//and y_tiles_per_screen are attainable
void Grid::zoom(int new_x_tiles_per_screen, int new_y_tiles_per_screen)
{
	//boundary checks - quit if any of the following tests are true
	if(new_x_tiles_per_screen < MIN_X_TILES_PER_SCREEN) return;
	if(new_x_tiles_per_screen > MAX_X_TILES_PER_SCREEN) return;
	if(new_y_tiles_per_screen < MIN_Y_TILES_PER_SCREEN) return;
	if(new_y_tiles_per_screen > MAX_Y_TILES_PER_SCREEN) return;
	if(upper_left_tile_x + new_x_tiles_per_screen - 1 > MAX_TILE_COORD) return;
	if(upper_left_tile_y + new_y_tiles_per_screen - 1 > MAX_TILE_COORD) return;
	//end of tests
	
	x_tiles_per_screen = new_x_tiles_per_screen;
	y_tiles_per_screen = new_y_tiles_per_screen;

	//reselect if the current selection is "bumped" off screen
	//by zooming in
	if(current_x >= upper_left_tile_x + x_tiles_per_screen)
		current_x = upper_left_tile_x + x_tiles_per_screen - 1;
	if(current_y >= upper_left_tile_y + y_tiles_per_screen)
		current_y = upper_left_tile_y + y_tiles_per_screen - 1;
}

void Grid::mouse_select(int pixel_x, int pixel_y)
{
	//quit if the click is out of range
	if( (pixel_x < x_coords[0]) ||
		(pixel_x > x_coords[x_tiles_per_screen]) ||
		(pixel_y < y_coords[0]) ||
		(pixel_y > y_coords[y_tiles_per_screen])
		)
		return;

	//say the tile the user selected is the upper left tile,
	//then (x, y) = (0, 0); say the user selected the third
	//tile in the 2nd row, then (x, y) = (1, 2), and so on...

	//start with illegal values
	int x = -1;
	int y = -1;

	//to simplify coding (slightly) --- just keep assigning
	//values to x, y --- overwriting the x = 0, y = 0 starting
	//condition

	//overwrite x till it's "right"
	int i = 0;
	for(i = 0; i < x_tiles_per_screen; i++)
		if(pixel_x > x_coords[i]) x = i;

	//overwrite x till it's "right"
	for(i = 0; i < y_tiles_per_screen; i++)
		if(pixel_y > y_coords[i]) y = i;

	//boundary check
	if( (x < 0) || (x > (x_tiles_per_screen - 1)) )
		return;
	if( (y < 0) || (y > (y_tiles_per_screen - 1)) )
		return;

	//remember the old selections
	int old_x = current_x - upper_left_tile_x;
	int old_y = current_y - upper_left_tile_y;

	current_x = upper_left_tile_x + x;
	current_y = upper_left_tile_y + y;

	//invalidate just the new and old "Tile" area of the grid
	
	//old rectangle:
	RECT rect_old = get_tile_rect(old_x, old_y);	

	//new rectangle:
	RECT rect_new = get_tile_rect(x, y);	

	//old rectangle needs to be erased to remove the old hi-lite
	//but new rectangle can simply be drawn over with the hi-lite
	InvalidateRect(parent_wnd, &rect_old, TRUE);
	InvalidateRect(parent_wnd, &rect_new, FALSE); //no erase
}

void Grid::move_up()
{
	//basic range check
	if(current_y <= MIN_TILE_COORD) return;

	//remember the original coordinate 
	int old_x = current_x - upper_left_tile_x;
	int old_y = current_y - upper_left_tile_y;

	current_y--;

	if(old_y == 0)
	{
		//we are moving off the UPPER edge
		upper_left_tile_y = current_y;
		RECT rect = get_drawing_area_rect();
		InvalidateRect (parent_wnd, &rect, TRUE);
	}
	else
	{
		//we are not moving off the UPPER edge

		//the following procedure is the same as the
		//update method used in mouse_select(...)
		//and also the next few move functions
		
		RECT rect_old = get_tile_rect(old_x, old_y);
		RECT rect_new = get_tile_rect(old_x, old_y - 1);

		//the old rectangle must be erased to remove the old
		//hi-lite. But the new rectangle need not be erased
		//because it can be drawn over with the new hi-lite
	
		InvalidateRect(parent_wnd, &rect_old, TRUE);
		InvalidateRect(parent_wnd, &rect_new, FALSE);
	}
}

void Grid::move_left()
{
	//basic range check
	if(current_x <= MIN_TILE_COORD) return;

	//remember the original coordinate 
	int old_x = current_x - upper_left_tile_x;
	int old_y = current_y - upper_left_tile_y;

	current_x--;

	if(old_x == 0)
	{
		//we are moving off the LEFT edge
		upper_left_tile_x = current_x;
		RECT rect = get_drawing_area_rect();
		InvalidateRect (parent_wnd, &rect, TRUE);
	}
	else
	{
		//we are not moving off the LEFT edge
		RECT rect_old = get_tile_rect(old_x, old_y);
		RECT rect_new = get_tile_rect(old_x - 1, old_y);
	
		InvalidateRect(parent_wnd, &rect_old, TRUE);
		InvalidateRect(parent_wnd, &rect_new, FALSE);
	}
}

void Grid::move_right()
{
	//basic range check
	if(current_x >= MAX_TILE_COORD)	return;

	//remember the original coordinate 
	int old_x = current_x - upper_left_tile_x;
	int old_y = current_y - upper_left_tile_y;

	current_x++;

	if(old_x == x_tiles_per_screen - 1)
	{
		//we are moving off the RIGHT edge
		upper_left_tile_x = current_x - x_tiles_per_screen + 1;
		RECT rect = get_drawing_area_rect();
		InvalidateRect (parent_wnd, &rect, TRUE);
	}
	else
	{
		//we are not moving off the RIGHT edge
		RECT rect_old = get_tile_rect(old_x, old_y);
		RECT rect_new = get_tile_rect(old_x + 1, old_y);
	
		InvalidateRect(parent_wnd, &rect_old, TRUE);
		InvalidateRect(parent_wnd, &rect_new, FALSE);
	}
}

void Grid::move_down()
{
	//basic range check
	if(current_y >= MAX_TILE_COORD) return;

	//remember the original coordinate 
	int old_x = current_x - upper_left_tile_x;
	int old_y = current_y - upper_left_tile_y;

	current_y++;

	if(old_y == y_tiles_per_screen - 1)
	{
		//we are moving off the LOWER edge
		upper_left_tile_y = current_y - y_tiles_per_screen + 1;
		RECT rect = get_drawing_area_rect();
		InvalidateRect (parent_wnd, &rect, TRUE);
	}
	else
	{
		//we are not moving off the LOWER edge

		RECT rect_old = get_tile_rect(old_x, old_y);
		RECT rect_new = get_tile_rect(old_x, old_y + 1);
	
		InvalidateRect(parent_wnd, &rect_old, TRUE);
		InvalidateRect(parent_wnd, &rect_new, FALSE);
	}
}

//end of navigation functions
//--------------------------------------------------------------


//--------------------------------------------------------------
//functions to operate on tiles
//add a tile, to delete a tile, rotate a tile, change tile color

void Grid::clear_tile(int x, int y)
{
	//assume that tile(x, y) can't be found in the hash table
	//for now
	bool found = false;

	int index = compute_address(x, y);
	if(m_tiles[index] == NULL) return;
	
	Tile* key = NULL;
	//points to the "Tile" object that we want to delete

	Tile* previous = NULL;
	//points to the "Tile" object right before the one 
	//that we want to delete

	int component_id = 0; //needed by "check_for_large_rect"
	
	//the easier case: if the first item in the list
	if((m_tiles[index]->x == x) && (m_tiles[index]->y == y))
	{
		key = m_tiles[index];
		component_id = key->component_id;
		m_tiles[index] = m_tiles[index]->pNext;

		key->pNext = NULL;
		delete key;
		found = true;
	}
	else
	{
		//quit if the tile is not in the first item and there is no
		//other item in the link list
		if(m_tiles[index]->pNext == NULL)	return;
	}

	if(found == false)
	{
		//now look through the link list
		key = m_tiles[index]->pNext;
		previous = m_tiles[index];

		while(key != NULL)
		{
			if((key->x == x) && (key->y == y)) //then delete it!
			{			
				component_id = key->component_id;
				previous->pNext = key->pNext;
				key->pNext = NULL;

				delete key;
				found = true;
				break;
			}

			previous = key;
			key = key->pNext;
		}
	}

	//check to see if the Tile was found:
	if(found == true)
	{
		//check to see if we need to refresh the tile on screen
		int x = current_x - upper_left_tile_x;
		int y = current_y - upper_left_tile_y;
		if( (0 <= x) &&
			(x < x_tiles_per_screen) &&
			(0 <= y) &&
			(y < y_tiles_per_screen))
		{
			RECT rect = get_tile_rect(x, y);
			check_for_large_rect(rect, component_id);
			InvalidateRect(parent_wnd, &rect, TRUE);
		}
	}

	return;
}

void Grid::add_a_tile(int component_id, Color foreground, Color background)
{
	//if this "add" action is enlarging the size of the overall picture,
	//update user_min_x, user_min_y, user_max_x, user_max_y
	if(current_x < user_min_x) user_min_x = current_x;
	if(current_y < user_min_y) user_min_y = current_y;
	if(current_x > user_max_x) user_max_x = current_x;
	if(current_y > user_max_y) user_max_y = current_y;	


	//when the user first drew an op amp on the tile, and then
	//he drew a resistor over it, the resistor needs to invalidate
	//a 4x4 area to erase the op amp --- normally drawing a resistor
	//just invalidates at 1x1 area. To detect this 4x4 area thing,
	//remember the prior component_id
	int previous_component_id = -1;


	int index = compute_address(current_x, current_y);

	//one case: the index is empty
	if(m_tiles[index] == NULL)
	{
		//new "Tile" object
		Tile* new_tile = new Tile();
		new_tile->x = current_x;
		new_tile->y = current_y;
		new_tile->component_id = component_id;
		new_tile->background = background;
		new_tile->foreground = foreground;
		new_tile->pNext = NULL;

		m_tiles[index] = new_tile; //add to hash table		
	}	
	else
	{
		//we would have to look through the link list and see
		//if we are attempting to add a tile that is already
		//in the link list

		bool found = false;				
		Tile* key = m_tiles[index];
		Tile* previous = NULL; // points to the "Tile" right
			//before the one "key" is pointing to

		while(key != NULL)
		{
			if((key->x == current_x) && (key->y == current_y))
			{						
				//"Tile" object already in memory -- update this
				//older Tile object				
				previous_component_id = key->component_id;
				key->component_id = component_id;
				key->foreground = foreground;
				key->background = background;
				
				found = true;
				break;
			}		
			previous = key;
			key = key->pNext;
		}

		if( (found == false) && (key == NULL) && previous != NULL)
		{
			//"previous" is the last "Tile" in the linked list

			//new "Tile" object
			Tile* new_tile = new Tile();
			new_tile->x = current_x;
			new_tile->y = current_y;
			new_tile->component_id = component_id;
			new_tile->background = background;
			new_tile->foreground = foreground;
			new_tile->pNext = NULL;

			previous->pNext = new_tile;
		}		
	}

	//redraw the current cell
	int x = current_x - upper_left_tile_x;
	int y = current_y - upper_left_tile_y;

	RECT rect = get_tile_rect(x, y);

	/*call "check_for_large_rect(...)" on both the new component_id
	and the previous_component_id. Use the larger rectangle as
	the invalidation rectangle
	*/
	RECT rect2 = rect;
	if(previous_component_id != -1)
		check_for_large_rect(rect2, previous_component_id);

	check_for_large_rect(rect, component_id);

	//use the larger of two rectangles as the invalidation
	//rectangle
	if(rect2.left < rect.left)
		rect = rect2;

	InvalidateRect(parent_wnd, &rect, TRUE);
}

void Grid::rotate_tile()
{
	Tile* tile = get_tile(current_x, current_y);
	if(tile == NULL) return;

	//orientation ID is the lowest 3 bits
	int orientation_id = (tile->component_id) & 0x7;
	orientation_id = (orientation_id + 1) % 8;

	int new_component_id = tile->component_id >> 3;
	new_component_id = new_component_id << 3;
	new_component_id = new_component_id + orientation_id;

	tile->component_id = new_component_id;

	RECT rect = get_tile_rect(current_x - upper_left_tile_x, 
		current_y - upper_left_tile_y);
	check_for_large_rect(rect, new_component_id);
	InvalidateRect(parent_wnd, &rect, TRUE);
}
	
void Grid::apply_color(Color foreground, Color background)
{		
	Tile* tile = get_tile(current_x, current_y);

	if(tile == NULL) //empty tile
	{
		add_a_tile(0, foreground, background);
	}
	else
	{
		tile->foreground = foreground;
		tile->background = background;

		//refresh
		RECT rect = get_tile_rect(current_x - upper_left_tile_x, 
			current_y - upper_left_tile_y);
		check_for_large_rect(rect, tile->component_id);
		InvalidateRect(parent_wnd, &rect, TRUE);
	}
}

int Grid::get_class_id()
{
	Tile* tile = get_tile(current_x, current_y);
	if(tile == NULL) return -1;

	return tile->component_id >> 19;
}

int Grid::get_type_id()
{
	Tile* tile = get_tile(current_x, current_y);
	if(tile == NULL) return -1;

	int type_id = tile->component_id & 0x0007FFF8;
	type_id = type_id >> 3;
	return type_id;
}

int Grid::get_orientation_id()
{
	Tile* tile = get_tile(current_x, current_y);
	if(tile == NULL) return -1;

	return (tile->component_id & 0x7);
}

void Grid::check_for_large_rect(RECT& rect, int component_id)
{
	int class_id = component_id >> 19;
	int type_id = component_id & 0x7FFF8;
	type_id = type_id >> 3;

	//if op-amp or "2 port black box"
	if( (class_id == CLASS_ID_OPAMP) ||
		((class_id == CLASS_ID_BLACKBOX) && (type_id == TYPE_ID_2PORT_BOX))
		)
	{
		/* expand rect to a 4x4 area!
		say an op-amp is inserted at "o", then the area to 
		invalidate:
		xxxx
		xOxx
		xxxx
		xxxx
		 */

		//first, know that the rect we get for a single
		//tile is exaggerated
		int width = rect.right - rect.left - 10;
		int height = rect.bottom - rect.top - 10;

		//the real rect is at:
		rect.top = rect.top + 5;
		rect.left = rect.left + 5;

		//now, the reformed rect:
		rect.left = rect.left - width - 9;
		rect.top = rect.top - height - 9;
		rect.right = rect.left + width * 4 + 9;
		rect.bottom = rect.top + height * 4 + 9;
	}

	//There are two classes of "Wire" components that can 
	//have wires drawn into the middle of an adjacent square.
	//So here you shift in the upper left direciton 
	//and enlarge the square
	if(	((class_id == CLASS_ID_WIRE) && (type_id == TYPE_ID_WIRE_ANGLE2)) ||
		((class_id == CLASS_ID_WIRE) && (type_id == TYPE_ID_WIRE_ANGLE3))
		)
	{
		int width = rect.right - rect.left;
		rect.left = rect.left - (int)(width / 2) - 2;
		
		int height = rect.bottom - rect.top;
		rect.top = rect.top - (int)(height / 2) - 2;

		rect.right = rect.left + width * 2 + 4;
		rect.bottom = rect.top + height * 2 + 4;		
	}
}

//end of functions to operate on tile
//--------------------------------------------------------------


//--------------------------------------------------------------
//functions to draw the tiles

Tile* Grid::get_tile(int x, int y)
{
	int index = compute_address(x, y);
	if(m_tiles[index] == NULL) return NULL;
	
	Tile* key = NULL;
	//points to the "Tile" object that we want to find
	
	//the easier case: if the first item in the list is the
	//"Tile" that we are looking for
	if((m_tiles[index]->x == x) && (m_tiles[index]->y == y))
	{
		key = m_tiles[index];
		return key;
	}
	else
	{
		//quit if the tile is not in the first item and there is no
		//other item in the link list
		if(m_tiles[index]->pNext == NULL)	return NULL;
	}

	//now look through the link list
	key = m_tiles[index]->pNext;	

	while(key != NULL)
	{
		if((key->x == x) && (key->y == y)) //then return it!
		{						
			return key;
		}		
		key = key->pNext;
	}

	//I can reach this point if the Tile at coordinate (x, y)
	//can't be found in m_tiles[index] at all
	return NULL;
}

void Grid::set_drawing_area(Rect& rect)
{
	drawing_area = rect;

	//figure out x_coord, y_coord

	float grid_width = (float)drawing_area.Width / (float)x_tiles_per_screen;
	float grid_height = (float)drawing_area.Height / (float)y_tiles_per_screen;

	//each tile is approximately "grid_width" wide and 
	//"grid_height" wide --- but pixels are discrete numbers
	//and in this application we want the tiles to line up 
	//"properly". So it is best to record the exact INTEGER pixel
	//location the tiles

	//x_coord's first and last points
	x_coords[0] = drawing_area.X;
	x_coords[x_tiles_per_screen] = drawing_area.X + drawing_area.Width - 1;
	//the other points
	int i = 0;
	for(i = 1; i < x_tiles_per_screen; i++)
		x_coords[i] = drawing_area.X + (int)(i*grid_width);

	//y_coord's first and last points
	y_coords[0] = drawing_area.Y;
	y_coords[y_tiles_per_screen] = drawing_area.Y + drawing_area.Height - 1;
	//the other points
	for(i = 1; i < y_tiles_per_screen; i++)
		y_coords[i] = drawing_area.Y + (int)(i*grid_height);

	//make the rest of these arrays -1
	for(i = x_tiles_per_screen + 1; i < MAX_X_TILES_PER_SCREEN + 1; i++)
		x_coords[i] = -1;

	for(i = y_tiles_per_screen + 1; i < MAX_Y_TILES_PER_SCREEN + 1; i++)
		y_coords[i] = -1;
}

void Grid::draw_to_screen(Graphics* graphics)
{
	if( (drawing_area.Width <= 0) ||
		(drawing_area.Height <= 0))
		return;	

	//ensure the clipping region being used does not extend
	//beyond drawing_area

	Rect clipping_rect = drawing_area;
	clipping_rect.X--;
	clipping_rect.Y--;
	clipping_rect.Width = clipping_rect.Width + 2;
	clipping_rect.Height = clipping_rect.Height + 2;
	Region old_clip;
	graphics->GetClip(&old_clip);
	graphics->SetClip(clipping_rect, CombineModeIntersect);


	//drawing the individula tiles
	//for each tile, we need:
	int x = 0;			//upper left x coordinate
	int y = 0;			//upper left y coordinate
	int width = 0;		//tile's width
	int height = 0;		//tile's height

	Rect rect;		//more concise way to express the above


	//go over each tile one column at a time --- this time, just
	//to draw the background
	int i = 0;	
	int j = 0;
	for(i = 0; i < x_tiles_per_screen; i++)
	{
		int tile_coord_x = upper_left_tile_x + i;

		for(j = 0; j < y_tiles_per_screen; j++)
		{
			//this for loop takes care of one column of tiles
			int tile_coord_y = upper_left_tile_y + j;

			Tile* tile = get_tile(tile_coord_x, tile_coord_y);
			if(tile != NULL)
			{
				//-------------------------------------------------
				//draw the tile background

				rect.X = x_coords[i] - 1;
				rect.Y = y_coords[j] - 1;
				rect.Width = x_coords[i+1] - x_coords[i] + 2;
				rect.Height = y_coords[j+1] - y_coords[j] + 2;

				SolidBrush brush(tile->background);
				graphics->FillRectangle(&brush, rect);				
				
				//end of drawing the tile background
				//-------------------------------------------------
			}
		}
	}


	//preparing to draw the grid lines	
	Pen dash_pen(Color(255, 0, 0, 0)); //color black, width 1
	dash_pen.SetDashStyle(DashStyleDot);

	//draw horizontal grid lines	
	for(i = 1; i < y_tiles_per_screen; i++)
	{		
		graphics->DrawLine(&dash_pen, 
			x_coords[0], 
			y_coords[i],
			x_coords[x_tiles_per_screen], 
			y_coords[i]);
	}

	//draw vertical grid lines	
	for(i = 1; i < x_tiles_per_screen; i++)
	{
		graphics->DrawLine(&dash_pen, 
			x_coords[i], 
			y_coords[0],
			x_coords[i], 
			y_coords[y_tiles_per_screen]);
	}

	
	Component component(graphics);	

	//go over each tile one column at a time --- this time, draw
	//tile component
	for(i = 0; i < x_tiles_per_screen; i++)
	{
		int tile_coord_x = upper_left_tile_x + i;

		for(j = 0; j < y_tiles_per_screen; j++)
		{
			//this for loop takes care of one column of tiles
			int tile_coord_y = upper_left_tile_y + j;

			Tile* tile = get_tile(tile_coord_x, tile_coord_y);
			if(tile != NULL)
			{
				//-------------------------------------------------
				//draw the tile component

				rect.X = x_coords[i] - 1;
				rect.Y = y_coords[j] - 1;
				rect.Width = x_coords[i+1] - x_coords[i] + 2;
				rect.Height = y_coords[j+1] - y_coords[j] + 2;
				
				component.setForegroundColor(tile->foreground);
				component.draw(tile->component_id, rect);
				
				//end of drawing the tile component
				//-------------------------------------------------
			}
		}
	}	


	//draw an outline marking the rectangle containing the grid
	Pen black_pen(Color(255, 0, 0, 0)); //color black, width 1
	graphics->DrawRectangle(&black_pen, drawing_area);

	//draw a rectangle around the currently selected grid
	Pen orange_pen(Color(255, 255, 153, 51), 3); //color orange, width 5
	x = x_coords[current_x - upper_left_tile_x];
	y = y_coords[current_y - upper_left_tile_y];
	width = x_coords[current_x - upper_left_tile_x + 1] - x;
	height = y_coords[current_y - upper_left_tile_y + 1] - y;
	graphics->DrawRectangle(&orange_pen, x, y, width, height);

	//restore previous clipping settings
	graphics->SetClip(&old_clip);
}

void Grid::trim_picture()
{
	//check to see that the left border contains at least one
	//user drawn picture	
	bool drawn_tile_found = false;
	while((drawn_tile_found == false) && (user_min_x < user_max_x))
		//in other words:
		//quit the loop if: drawn_tile_found is no longer false
		//or when user_min_x == user_max_x
	{
		int i = 0;
		for(i = user_min_y; i <= user_max_y; i++)
		{
			Tile* tile_ptr = get_tile(user_min_x, i);
			if(tile_ptr != NULL)
			{
				drawn_tile_found = true;
				break;
			}
		}
		if(drawn_tile_found == false)
			user_min_x++;
	}

	//check the right border
	drawn_tile_found = false;
	while((drawn_tile_found == false) && (user_min_x < user_max_x))
	{
		int i = 0;
		for(i = user_min_y; i <= user_max_y; i++)
		{
			Tile* tile_ptr = get_tile(user_max_x, i);
			if(tile_ptr != NULL)
			{
				drawn_tile_found = true;
				break;
			}
		}
		if(drawn_tile_found == false)
			user_max_x--;
	}

	//check the upper border
	drawn_tile_found = false;
	while((drawn_tile_found == false) && (user_min_y < user_max_y))
	{
		int i = 0;
		for(i = user_min_x; i <= user_max_x; i++)
		{
			Tile* tile_ptr = get_tile(i, user_min_y);
			if(tile_ptr != NULL)
			{
				drawn_tile_found = true;
				break;
			}
		}
		if(drawn_tile_found == false)
			user_min_y++;
	}

	//check the lower border
	drawn_tile_found = false;
	while((drawn_tile_found == false) && (user_min_y < user_max_y))
	{
		int i = 0;
		for(i = user_min_x; i <= user_max_x; i++)
		{
			Tile* tile_ptr = get_tile(i, user_max_y);
			if(tile_ptr != NULL)
			{
				drawn_tile_found = true;
				break;
			}
		}
		if(drawn_tile_found == false)
			user_max_y--;
	}
}

int Grid::get_picture_width()
{
	//the width of the picture is based on the width of the
	//upper left most tile on screen times the
	//number of tiles in the picture

	int tile_width = x_coords[1] - x_coords[0];

	return (tile_width * (user_max_x - user_min_x + 1));
}
	
int Grid::get_picture_height()
{	
	//the height of the picture is based on the width of the
	//upper left most tile on screen times the
	//number of tiles in the picture

	int tile_height = y_coords[1] - y_coords[0];

	return (tile_height * (user_max_y - user_min_y + 1));
}

void Grid::draw_all(Graphics* graphics)
{	
	//drawing the individula tiles
	//for each tile, we need:
	int x = 0;			//upper left x coordinate
	int y = 0;			//upper left y coordinate
	int width = x_coords[1] - x_coords[0];		//tile's width
	int height = y_coords[1] - y_coords[0];		//tile's height	

	//the bitmap starts out with a black background --- give it a
	//white background instead
	SolidBrush white_brush(Color(255, 255, 255, 255));
	graphics->FillRectangle(&white_brush, 0, 0,
		width * (user_max_x - user_min_x + 1), //width
		height * (user_max_y - user_min_y + 1) //height
		);

	//go over each tile one COLUMN at a time --- this time, just
	//to draw the background
	//use (x, y) as the coordinate of the tile in the picture
	//use (i, j) as the coordinate of the tile in the get_tile(...)
	//function call
	
	int i = user_min_x;
	int j = user_min_y;	
	for(i = user_min_x; i < user_max_x + 1; i++)
	{		
		for(j = user_min_y; j < user_max_y + 1; j++)
		{
			//we are at i-th column, j-th row
			x = (i - user_min_x) * width;
			y = (j - user_min_y) * height;

			Tile* tile = get_tile(i, j);

			if(tile != NULL)
			{
				//-------------------------------------------------
				//draw the tile background

				SolidBrush brush(tile->background);
				graphics->FillRectangle(&brush, 
					x - 1, y - 1, width + 2, height + 2);

				//end of drawing the tile background
				//-------------------------------------------------
			}
		}
	}
	
	//go over each tile one column at a time --- this time, draw
	//tile component

	Component component(graphics);
	Rect rect;

	for(i = user_min_x; i < user_max_x + 1; i++)
	{		
		for(j = user_min_y; j < user_max_y + 1; j++)
		{
			//we are at i-th column, j-th row
			x = (i - user_min_x) * width;
			y = (j - user_min_y) * height;

			Tile* tile = get_tile(i, j);

			if(tile != NULL)
			{
				//-------------------------------------------------
				//draw the tile component

				rect.X = x - 1;
				rect.Y = y - 1;
				rect.Width = width + 2;
				rect.Height = height + 2;

				component.setForegroundColor(tile->foreground);
				component.draw(tile->component_id, rect);

				//end of drawing the tile component
				//-------------------------------------------------
			}
		}
	}
}

//end of functions to draw the tiles
//--------------------------------------------------------------