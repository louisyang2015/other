#ifndef LOUIS_COMPONENT_H
#define LOUIS_COMPONENT_H


/*
Usage:

Construct this object by providing the constructor with a device context to draw with.
Use the component.draw(component_id, Rect& rect) function to draw a component at the location rect.

Adjust the color using:
component.setForegroundColor(Color color)

*/

class Component
{
public:
	Component(Graphics* graphics);
	void draw(int component_id, Rect& rect);
	void setForegroundColor(Color color);	

	//utility function
	static int compute_component_id(int class_id, int type_id,
		int orientation_id);

private:
	Graphics* m_graphics; //a POINTER to a graphics object to draw with
	Color m_foreground_color;	

	//helper functions for drawing

	//this will compute a set of 3 destination points so that
	//Component.draw(...) will properly roate the image as the
	//"orientation" number demands
	void get_transform_pt_array(int orientation, Rect& rect, 
		Point* pt_array);
};

#endif