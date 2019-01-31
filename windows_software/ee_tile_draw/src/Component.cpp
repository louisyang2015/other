#include <windows.h>
#include <stdio.h>
#include <tchar.h>

//-----------------------------------------------------------
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")
//-----------------------------------------------------------

#include "global_constants.h"
#include "Component.h"
#include "ImageList.h"

extern ImageList app_image_list; //declared in eetdp.cpp

//--------------------------------------------------------------
//first, the easy functions
Component::Component(Graphics* graphics)
{
	m_graphics = graphics;
	m_foreground_color = Color::Black;
}
void Component::setForegroundColor(Color color)
{
	m_foreground_color = color;
}
int Component::compute_component_id(int class_id, int type_id,
		int orientation_id)
{
	int return_val = class_id << 19;
	return_val = return_val + (type_id << 3);
	return_val = return_val + orientation_id;
	return return_val;
}
//end of the easy stuff
//--------------------------------------------------------------


//--------------------------------------------------------------
//this function will start off the drawing

void Component::draw(int component_id, Rect& rect)
{
	if(component_id == 0) return;

	//in most cases, load the file and show it with a transformation
	//the file name is "component_id" with the lowest 2 bits masked
	//out; plus a ".wmf" extension
	int file_id = component_id >> 2;
	file_id = file_id << 2;
	Image* image = app_image_list.get_image(file_id);
	if(image == NULL) return;
	
	//for the "op-amp" and the "2 port black box" components
	//you need to double the size of "rect" because these things
	//cover 2x2 tiles
	int class_id = component_id >> 19;
	int type_id = component_id & 0x7FFF8;

	//if op-amp or "2 port black box"
	if( (class_id == CLASS_ID_OPAMP) ||
		((class_id == CLASS_ID_BLACKBOX) && (type_id == TYPE_ID_2PORT_BOX))
		)
	{
		rect.Width = 2 * rect.Width - 2;
		rect.Height = 2 * rect.Height - 2;
	}

	//take the lowest most 2 bits as the orientation ID
	//actually the lowest 3 bits is the orientatino ID
	//but use the lowest 2 bit to avoid the "one-pixel-off"
	//error --- see documentation "component.htm" for detail
	int orientation_id = component_id & 0x3;

	//these points will be used in the "m_graphics->DrawImage"
	//line later on
	Point destination_pts[3];
	get_transform_pt_array(orientation_id, rect, destination_pts);

	//change the foreground color from black to the color
	//specified by the "m_foreground_color"
	ImageAttributes imageAttributes;
	ColorMap colorMap;
	colorMap.oldColor = Color(255, 0, 0, 0);  // black
	colorMap.newColor = m_foreground_color;

	imageAttributes.SetRemapTable(1, &colorMap);
	//The colors are changed only for rendering — 
	//the color values of the image itself (stored in 
	//an Image or Bitmap object) are not changed.

	m_graphics->DrawImage(image, destination_pts, 3,
		0, 0, image->GetWidth(), image->GetHeight(), UnitPixel,
		&imageAttributes);

	return;
}
//--------------------------------------------------------------

//--------------------------------------------------------------
//helper functions for drawing

//this function finds the destination point array used 
//in "Graphics.DrawImage" --- see "Component.htm", section
//"Drawing a Component" for
//more detail
void Component::get_transform_pt_array(int orientation, Rect& rect, 
		Point* pt_array)
{
	//the 3 points will be taken out of these 8 possibilities
	//for the theory of how this works, see my "Component.htm"
	//section "Drawing a Component"

	Point pts[8];
	//this algorithm works for the case that "rect" is a
	//rectangle
	int half_width = rect.Width / 2;
	int half_height = rect.Height / 2;
	
	pts[0].X = rect.X;
	pts[0].Y = rect.Y + rect.Height;

	pts[1].X = rect.X - half_width;
	pts[1].Y = rect.Y + half_height;

	pts[2].X = rect.X;
	pts[2].Y = rect.Y;

	pts[3].X = rect.X + half_width;
	pts[3].Y = rect.Y - half_height;

	pts[4].X = rect.X + rect.Width;
	pts[4].Y = rect.Y;

	pts[5].X = rect.X + rect.Width + half_width;
	pts[5].Y = rect.Y + half_height;

	pts[6].X = rect.X + rect.Width;
	pts[6].Y = rect.Y + rect.Height;

	pts[7].X = rect.X + half_width;
	pts[7].Y = rect.Y + rect.Height + half_height;


	//upper left
	pt_array[0] = pts[(orientation + 2) % 8];

	//upper right
	pt_array[1] = pts[(orientation + 4) % 8];

	//lower left
	pt_array[2] = pts[orientation];
}
//--------------------------------------------------------------