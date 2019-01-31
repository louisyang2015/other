#include <windows.h>
#include <stdio.h>
#include <tchar.h>

//-----------------------------------------------------------
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")
//-----------------------------------------------------------

#include "global_constants.h"
#include "ImageList.h"


//-----------------------------------------------------------
//some easy functions first
ImageListItem::ImageListItem(int component_id)
{
	id = component_id;
	image = NULL;

	//load the file with a transformation
	//the file name is "component_id" with the lowest 2 bits masked
	//out; plus a ".wmf" extension
	int file_id = id >> 2;
	file_id = file_id << 2;
	TCHAR file_name[50];
    swprintf_s(file_name, TEXT("%d.wmf"), file_id);
	image = new Image(file_name);
}

ImageListItem::~ImageListItem()
{
	if (image != NULL) delete image;
	id = 0;
}
ImageList::ImageList()
{	
	//-----------------------------------------------------------
	// Initialize GDI+
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	//-----------------------------------------------------------

	count = 0;

	int i = 0;
	for(i = 0; i < IMAGE_LIST_MAX_SIZE; i++)
		list_items[i] = NULL;

	rotate = NULL;
	rotate = new Image(TEXT("rotate.wmf"));
}
ImageList::~ImageList()
{
	if(rotate != NULL)
	{
		delete rotate;
		rotate = NULL;
	}

	int i = 0;
	for(i = 0; i < IMAGE_LIST_MAX_SIZE; i++)
	{
		if(list_items[i] != NULL)
		{
			delete list_items[i];
			list_items[i] = NULL;
		}
	}

	GdiplusShutdown(gdiplusToken);
}
void ImageList::add(int id)
{
	if(count + 1 <= IMAGE_LIST_MAX_SIZE)
	{
		list_items[count] = new ImageListItem(id);
		count++;
	}
}
Image* ImageList::get_rotate_pic()
{
	return rotate;
}
//end of easy functions first
//-----------------------------------------------------------


//-----------------------------------------------------------
//shell sort and binary search

void ImageList::shell_sort()
{
	//sorts the list_items[0] to list_items[count - 1]
	//according to the field list_items[].id

	int increment = count >> 2; 

	while (increment > 0)
	{
		int i = 0;
		for (i = 0; i < count; i++)
		{
			/* reminder on the shell sort: not a tutorial; for 
			a total description, please search for "shell sort"
			on the Internet.

			You have multiple lists such that:
			list 1: item[0], item[increment], item[2*increment], ...
			list 2: item[1], item[1 + increment], item[1 + 2*increment], ...
			list 3: item[2], item[2 + increment], item[2 + 2*increment], ...
			and so on

			if you sort list 1, then list 2, then so on; then that's
			an inefficient implementation

			we will sort list 1, list 2, etc. in parallel - by going
			through the mother list only once, from item[0] to
			item[count - 1]. Along the way we use insertion sort,
			not on the whole list but on the correct sub-list

			example: say i = 2 + 2*increment
			before the current loop, the list: item[2], item[2 + increment] is in order

			at the end of the current loop, the list: item[2], 
			item[2 + increment], item[2 + 2*increment] should be in order
			*/

			int j = i;

			/*
			we will loop back from item[j] to item[0]; we want to
			sort the list: item[j], item[j - increment], 
			item[j - 2*increment], etc.
			*/

			ImageListItem* temp = list_items[i];

			/* as in typical insertion sort, we will bump items
			upward, toward item[j], and insert temp into the right
			index
			*/

			while ((j >= increment) && 
				(list_items[j-increment]->id > temp->id))
			{
				list_items[j] = list_items[j - increment];
				j = j - increment;
			}
			list_items[j] = temp;
		}
		increment = increment >> 1;
	}
}

Image* ImageList::get_image(int id)
{
	int first = 0;
	int last = count - 1;

	//uses binary search to narrow "first" and "last"
	while(first <= last)
	{
		int mid = (first + last) / 2;  // compute mid point.
		if (id > list_items[mid]->id) 
			first = mid + 1;  // repeat search in top half.
		else if (id < list_items[mid]->id)
			last = mid - 1; // repeat search in bottom half.
		else
			return list_items[mid]->image; // found it. return position
	}

	return NULL;
}

//end of shell sort and binary search
//-----------------------------------------------------------