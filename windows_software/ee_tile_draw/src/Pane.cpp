#include <windows.h>

//-----------------------------------------------------------
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")
//-----------------------------------------------------------

#include "global_constants.h"
#include "ImageList.h"
#include "Pane.h"

extern ImageList app_image_list; //declared in eetdp.cpp

//easy functions first
void Pane::init()
{
	m_current_selection = 0;
	
	int i;
	for(i = 0; i < PANE_MAX_SELECTION; i++)
		m_component_id[i] = -1;

	//information about what pixels is the object at
	m_drawing_area.X = 0;
	m_drawing_area.Y = 0;
	m_drawing_area.Width = 0;
	m_drawing_area.Height = 0;

	for(i = 0; i < PANE_TILES_HORZ + 1; i++)
		x_coords[i] = -1;

	for(i = 0; i < PANE_TILES_VERT + 1; i++)
		y_coords[i] = -1;
}

Pane::Pane(Color border, Color selected_border, Color foreground,
		Color background, Color selected_background)
{
	m_border = border;
	m_selected_border = selected_border;
	m_foreground = foreground;
	m_background = background;
	m_selected_background = selected_background;

	init();
}

Pane::Pane()
{	
	m_border = Color(0, 0, 255); 	//blue border
	m_selected_border = Color(255, 128, 128); //light red selected border
	m_foreground = Color(128, 128, 255); //blue forecolor
	m_background = Color(0, 255, 255); //light blue background
	m_selected_background = Color(255, 255, 128); //light yellow selected background

	init();
}

void Pane::set_parent_wnd(HWND hWnd)
{
	parent_wnd = hWnd;
}

RECT Pane::get_drawing_area_rect()
{
	RECT rect;
	rect.left = m_drawing_area.X - 1;
	rect.top = m_drawing_area.Y - 1;
	rect.right = rect.left + m_drawing_area.Width + 2;
	rect.bottom = rect.top + m_drawing_area.Height + 2;

	return rect;
}

void Pane::set_component_id(int component_id, int index)
{
	if(index > PANE_MAX_SELECTION) return;

	m_component_id[index] = component_id;
}

void Pane::set_component_id(int class_id, int type_id, int index)
{
	if(index > PANE_MAX_SELECTION) return;

	//orientation ID is always zero for the Pane
	int component_id = class_id << 19;
	component_id = component_id + (type_id << 3);	
	m_component_id[index] = component_id;
}

void Pane::set_hilite(int component_id)
{
	int i = 0;
	
	//try to find "component_id" in the array
	for(i = 0; i < PANE_MAX_SELECTION + 1; i++)
	{
		if(component_id == m_component_id[i])
			m_current_selection = i;
	}
}

int Pane::get_hilite()
{
	return m_component_id[m_current_selection];
}

void Pane::invalidate(HWND hWnd)
{
	RECT rect = get_drawing_area_rect();
	InvalidateRect(hWnd, &rect, TRUE);
}
//end of easy functions


//---------------------------------------------------------
//helper functions

void Pane::set_drawing_area(Rect& rect)
{
	m_drawing_area = rect;
	
	//figure out x_coord, y_coord

	float grid_width = (float)m_drawing_area.Width / (float)PANE_TILES_HORZ;
	float grid_height = (float)m_drawing_area.Height / (float)PANE_TILES_VERT;

	//x_coord's first and last points
	x_coords[0] = m_drawing_area.X;
	x_coords[PANE_TILES_HORZ] = m_drawing_area.X + m_drawing_area.Width - 1;
	//the other points
	int i = 0;
	for(i = 1; i < PANE_TILES_HORZ; i++)
		x_coords[i] = m_drawing_area.X + (int)(i*grid_width);

	//y_coord's first and last points
	y_coords[0] = m_drawing_area.Y;
	y_coords[PANE_TILES_VERT] = m_drawing_area.Y + m_drawing_area.Height - 1;
	//the other points
	for(i = 1; i < PANE_TILES_VERT; i++)
		y_coords[i] = m_drawing_area.Y + (int)(i*grid_height);
}


RECT Pane::get_tile_rect(int index)
{
	//alternative: int x_index = index & 0x1;
	int x_index = index % PANE_TILES_HORZ;
	int y_index = index / PANE_TILES_HORZ;

	RECT rect;
	
	rect.left = x_coords[x_index] - 1;
	rect.top = y_coords[y_index] - 1;
	rect.right = x_coords[x_index + 1] + 2;
	rect.bottom = y_coords[y_index + 1] + 2;	
	
	return rect;
}

//end of helper functions
//---------------------------------------------------------


//---------------------------------------------------------
//public member functions

int Pane::mouse_select(int pixel_x, int pixel_y)
{
	//quit if the click is out of range
	if( (pixel_x < x_coords[0]) ||
		(pixel_x > x_coords[PANE_TILES_HORZ]) ||
		(pixel_y < y_coords[0]) ||
		(pixel_y > y_coords[PANE_TILES_VERT])
		)
		return 0;

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
	for(i = 0; i < PANE_TILES_HORZ; i++)
		if(pixel_x > x_coords[i]) x = i;

	//overwrite x till it's "right"
	for(i = 0; i < PANE_TILES_VERT; i++)
		if(pixel_y > y_coords[i]) y = i;

	//boundary check
	if( (x < 0) || (x > (PANE_TILES_HORZ - 1)) )
		return 0;
	if( (y < 0) || (y > (PANE_TILES_VERT - 1)) )
		return 0;

	int new_selection = y * PANE_TILES_HORZ + x;

	//3 possible outcomes --- the user selected an active tile,
	//or the rotation button tile, or an inactive tile
	enum UserChoice {active, inactive, rotate};
	UserChoice user_choice;

	user_choice = active; // assume this for now

	if(m_component_id[new_selection] == -1)
	{
		//user_choice can't be active 
		user_choice = inactive; //assume this for now

		if(new_selection > 0)
		{
			if(m_component_id[new_selection - 1] != -1)
				user_choice = rotate; //first -1 index is "rotate"
		}
	}

	if(user_choice == rotate) return -1; //signal rotate
	if(user_choice == inactive) return 0; //signal no change

	//remember the old selection
	int old_selection = m_current_selection;

	m_current_selection = new_selection;
	
	//invalidate just the new and old "Tile" area of the grid
	
	//old rectangle:
	RECT rect_old = get_tile_rect(old_selection);	

	//new rectangle:
	RECT rect_new = get_tile_rect(m_current_selection);	

	//old rectangle needs to be erased to remove the old hi-lite
	//but new rectangle can simply be drawn over with the hi-lite
	//and new background color
	InvalidateRect(parent_wnd, &rect_old, TRUE);
	InvalidateRect(parent_wnd, &rect_new, FALSE); //no erase

	//return the component_id of the selected tile
	return m_component_id[m_current_selection];
}

void Pane::draw(Graphics* graphics)
{
	//draw each tile
	int i = 0;
	int j = 0;
	bool rotate_drawn = false; //true after we draw the rotation button

	//draw background
	SolidBrush brush(m_background);
	graphics->FillRectangle(&brush, m_drawing_area);

	for(i = 0; i < PANE_TILES_VERT; i++)
	{
		for(j = 0; j < PANE_TILES_HORZ; j++)
		{
			//now we are at i-th row, j-th column
			
			//the drawing area of the current tile
			Rect rect;
			rect.Y = y_coords[i]; //row i
			rect.X = x_coords[j]; //column j
			rect.Width = x_coords[j+1] - x_coords[j];
			rect.Height = y_coords[i+1] - y_coords[i];

			//use "rect" to draw the hi-lited background

			//but use a smaller version of rect to 
			//draw the tile images
			Rect rect2 = rect;
			int five_percent_x = (int)(rect2.Width * 0.05);
			int five_percent_y = (int)(rect2.Height * 0.05);

			rect2.X = rect.X + five_percent_x;
			rect2.Y = rect.Y + five_percent_y;
			rect2.Width = rect.Width - 2 * five_percent_x;
			rect2.Height = rect.Height - 2 * five_percent_y;

			int index = i * PANE_TILES_HORZ + j;
			int component_id = m_component_id[index];

			if((component_id == -1) && (rotate_drawn == false))
			{				
				rotate_drawn = true; //show rotate button just once				

				//draw the rotate tile				
				Image* image = app_image_list.get_rotate_pic();
				if(image != NULL)
				{
					graphics->DrawImage(image, rect2);
				}
			}
			else if(component_id != -1)
			{
				//draw tile

				//the normal background is already drawn earlier
				//but draw over that normal background if the 
				//item is selected				
				if(index == m_current_selection)
				{
					SolidBrush brush(m_selected_background);
					graphics->FillRectangle(&brush, rect);
				}				

				//draw tile				
				Image* image = app_image_list.get_image(component_id);
				if(image != NULL)
				{
					graphics->DrawImage(image, rect2);
				}
			}
		}		
	}

	//draw the border and grid lines		
	Pen pen(m_border);

	//draw horizontal grid lines	
	for(i = 0; i < PANE_TILES_VERT + 1; i++)
	{		
		graphics->DrawLine(&pen, 
			x_coords[0], 
			y_coords[i],
			x_coords[PANE_TILES_HORZ], 
			y_coords[i]);
	}

	//draw vertical grid lines	
	for(i = 0; i < PANE_TILES_HORZ + 1; i++)
	{
		graphics->DrawLine(&pen, 
			x_coords[i], 
			y_coords[0],
			x_coords[i], 
			y_coords[PANE_TILES_VERT]);
	}

	//draw the hi-lited border
	int x_index = m_current_selection % PANE_TILES_HORZ;
	int y_index = m_current_selection / PANE_TILES_HORZ;

	Pen selection_pen(Color(m_selected_border), 3); //width of pen = 3
	graphics->DrawRectangle(&selection_pen, 
		x_coords[x_index], // x
		y_coords[y_index],	// y
		x_coords[x_index + 1] - x_coords[x_index], //width 
		y_coords[y_index + 1] - y_coords[y_index]); //height
}

//end of public member functions
//---------------------------------------------------------