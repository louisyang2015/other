#ifndef LOUIS_USER_SELECTION_H
#define LOUIS_USER_SELECTION_H

/*
USAGE: 

each class of object has a default type_id and a default
orientation_id

each ClassDefault object means the "class_id" class of 
objects holds the default "type_id" and default "orientation_id"
for that class
*/

class ClassDefault
{
public:
	int class_id;
	int type_id;
	int orientation_id;
};

/* USAGE:

create UserSelection object globally 
keep calling ".add(...)" to fill up the class_id_list object
call ".shell_sort()"

the user of this class can store type_id and orientation_id
for a particular class_id --- ".set_type_id(...)", 
".set_orientation_id(...)"

the user of this class can retrieve type_id, and orientation_id
for a particular class_id --- ".get_type_id(...)",
".get_orientation_id(...)"

in addition, these variables are pulically available
	current_class_id
	current_foreground_color
	current_background_color

this object will be globally constructed
*/

class UserSelection
{
private:
	ClassDefault* class_id_list[CLASS_ID_LIST_MAX_SIZE];
	int count; //length of "class_id_list"

	//helper function --- get a pointer to a ClassDefault object
	//containing the class_id specified; uses binary search on the
	//class_id_list variable.
	//It will return NULL if no ClassDefault object has 
	//the class_id specified
	ClassDefault* find(int class_id);

public:
	UserSelection();
	~UserSelection();

	void add(int class_id, int type_id, int orientation_id);

	//sort the "class_id_list" by the class_id field
	void shell_sort();

	//changes the default "type_id" of the "current_class_id"
	void set_type_id(int type_id);

	//changes the default "orientation_id" of the "current_class_id"
	void set_orientation_id(int class_id, int orientation_id);

	//both of the following return -1 if the ClassDefault
	//with class_id is not found
	int get_type_id(int class_id);
	int get_orientation_id(int class_id);

	//compute component ID from the "current_class_id", and
	//the information from "class_id_list"
	int get_current_component_id();

	//----------------------------------------------------------
	//to track user's current selection:
	int current_class_id;
	Color foreground_color;
	Color background_color;
	//----------------------------------------------------------
};

#endif