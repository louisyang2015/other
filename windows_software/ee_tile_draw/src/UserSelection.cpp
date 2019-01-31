#include <windows.h>

//-----------------------------------------------------------
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")
//-----------------------------------------------------------

#include "global_constants.h"
#include "UserSelection.h"


//implementation in the order of appearance as in header file

ClassDefault* UserSelection::find(int class_id)
{
	int first = 0;
	int last = count - 1;

	//uses binary search to narrow "first" and "last"
	while(first <= last)
	{
		int mid = (first + last) / 2;  // compute mid point.
		if (class_id > class_id_list[mid]->class_id) 
			first = mid + 1;  // repeat search in top half.
		else if (class_id < class_id_list[mid]->class_id)
			last = mid - 1; // repeat search in bottom half.
		else
			return class_id_list[mid]; // found it. return position
	}

	return NULL;
}

UserSelection::UserSelection()
{
	int i = 0;
	for(i = 0; i < CLASS_ID_LIST_MAX_SIZE; i++)
		class_id_list[i] = NULL;
	
	current_class_id = CLASS_ID_VOLTAGE_SOURCE;
	foreground_color = Color::Black;
	background_color = Color::White;
}

UserSelection::~UserSelection()
{
	int i = 0;
	for(i = 0; i < CLASS_ID_LIST_MAX_SIZE; i++)
	{
		if(class_id_list[i] != NULL)
		{
			delete class_id_list[i];
			class_id_list[i] = NULL;
		}
	}
}

void UserSelection::add(int class_id, int type_id, int orientation_id)
{
	if(count + 1 <= CLASS_ID_LIST_MAX_SIZE)
	{
		class_id_list[count] = new ClassDefault();
		class_id_list[count]->class_id = class_id;
		class_id_list[count]->type_id = type_id;
		class_id_list[count]->orientation_id = orientation_id;
		count++;
	}
}

void UserSelection::shell_sort()
{
	int increment = count >> 2; 

	while (increment > 0)
	{
		int i = 0;
		for (i = 0; i < count; i++)
		{
			int j = i;

			/*
			we will loop back from class_id_list[j] to item[0]; we want to
			sort the list: class_id_list[j], class_id_list[j - increment], 
			class_id_list[j - 2*increment], etc.
			*/

			ClassDefault* temp = class_id_list[i];

			/* as in typical insertion sort, we will bump items
			upward, toward class_id_list[j], and insert temp into the right
			index
			*/

			while ((j >= increment) && 
				(class_id_list[j-increment]->class_id > temp->class_id))
			{
				class_id_list[j] = class_id_list[j - increment];
				j = j - increment;
			}
			class_id_list[j] = temp;
		}
		increment = increment >> 1;
	}
}

void UserSelection::set_type_id(int type_id)
{
	ClassDefault* temp = find(current_class_id);
	if(temp != NULL)
	{
		temp->type_id = type_id;
	}
}

void UserSelection::set_orientation_id(int class_id, int orientation_id)
{
	ClassDefault* temp = find(class_id);
	if(temp != NULL)
	{
		temp->orientation_id = orientation_id;
	}
}

int UserSelection::get_type_id(int class_id)
{
	ClassDefault* temp = find(class_id);
	if(temp != NULL)
	{
		return temp->type_id;
	}
	return -1;
}

int UserSelection::get_orientation_id(int class_id)
{
	ClassDefault* temp = find(class_id);
	if(temp != NULL)
	{
		return temp->orientation_id;
	}
	return -1;
}

int UserSelection::get_current_component_id()
{
	int type_id = get_type_id(current_class_id);
	if(type_id == -1) return -1;

	int orientation_id = get_orientation_id(current_class_id);
	if(orientation_id == -1) return -1;

	int return_val = current_class_id << 19;
	return_val = return_val + (type_id << 3);
	return_val = return_val + orientation_id;
	return return_val;
}