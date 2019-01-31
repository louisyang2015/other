#ifndef LOUIS_PANE_H
#define LOUIS_PANE_H

#endif

/* USAGE:
construct a Pane object using one of the two constructors

the constructor just sets up the colors; further setting up
of the Pane requires calling "set_component_id" for each
of the 10 tiles. The first tile with a component ID of -1 will
be drawn with a rotation symbol. Any tile after this id=-1 tile
will be ignored

call "set_hilite(...)" to set the current selection
call "get_hilite(...)" to get the currently selected component ID

when a mouse clicks, call "mouse_select" to process user
selections

the "draw" function will draw this Pane object

"set_drawing_area" will set the "drawing_area" variable, that
variable represents the rectangle bouding this Pane object
*/

class Pane
{
private:
	
	//information about what tile is selectd and what's at the 
	//tiles
	int m_current_selection;
	int m_component_id[PANE_MAX_SELECTION + 1];

	//information about what pixels is the object at
	Rect m_drawing_area;
	int x_coords[PANE_TILES_HORZ + 1];
	int y_coords[PANE_TILES_VERT + 1];

	//colors used in draw(...)
	Color m_border;
	Color m_selected_border;
	Color m_foreground;
	Color m_background;
	Color m_selected_background;

	//handle to the parent window is needed for refreshing the
	//object
	HWND parent_wnd;

	//these next functions return a "RECT" that is one size bigger
	//than the grid. These RECT are meant to be used for 
	//refreshing the screen area

	//returns a RECT that bounds the m_drawing_area
	RECT get_drawing_area_rect();
	//returns a RECT that bounds the tile with the index number given
	RECT get_tile_rect(int index);	

public:

	Pane();
	Pane(Color border, Color selected_border, Color foreground,
		Color background, Color selected_background);
	void init();	//code shared by the two constructors

	//---------------------------------------------------------

	//when the user selects with the mouse, this function will
	//update "m_current_selection"; it will then return the 
	//m_component_id[m_current_selection] that the user has selected
	//Also, returns 0 if no active tile is selected
	//return -1 if the rotation tile is selected
	int mouse_select(int pixel_x, int pixel_y);

	void draw(Graphics* graphics);

	//either function will set the m_component_id[] array with
	//a component_id --- the 3 argument version will compute the
	//component_id internally
	void set_component_id(int component_id, int index);
	void set_component_id(int class_id, int type_id, int index);

	//attempts to select the component_id --- if it's present in
	//the system that is
	void set_hilite(int component_id);

	//returns the component_id that is currently selected
	int get_hilite();

	void set_drawing_area(Rect& rect);
	void set_parent_wnd(HWND hWnd);

	//invalidates the m_drawing_area
	void invalidate(HWND hWnd);
	
	//---------------------------------------------------------
};

