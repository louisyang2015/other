#ifndef LOUIS_IMAGE_LIST_H
#define LOUIS_IMAGE_LIST_H

/*USAGE:
each image list item has an "image object" and an ID
*/
class ImageListItem
{
public:
	ImageListItem(int component_id);
	~ImageListItem();

	int id; //this ID is the component ID
	//so it includes the class_id, type_id, and orientation_id

	Image* image;
};

/*USAGE:
construct this object globally
call "add(id)" repeatedly to add an image to the list; the class
will load "id.wmf" file into an image object

call "shell_sort()" to sort the images in the list according to 
their ID

retrieve a pointer to an image by using "get_image(id)"

retrieve a pointer to the "rotate" image by calling "get_rotate_pic()"
*/

class ImageList
{
private:
	
	ImageListItem* list_items[IMAGE_LIST_MAX_SIZE];
	int count; //how many items in the ImageListItem array

	//this image has the picture for rotation buttons
	Image* rotate;

	//this class initializes the GDI+
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR			   gdiplusToken;

public:

	ImageList();
	~ImageList();
	
	void add(int id);
	void shell_sort();
	Image* get_image(int id);

	Image* get_rotate_pic(); //returns pointer to "rotate"
};

#endif