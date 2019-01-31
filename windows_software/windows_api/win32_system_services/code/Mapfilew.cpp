//***************************************************************
// From the book "Win32 System Services: The Heart of Windows 98
// and Windows 2000"
// by Marshall Brain
// Published by Prentice Hall
//
// Copyright 1995, by Prentice Hall.
//
// This code demonstrates writing to a mapped file.
//***************************************************************

// mapfilew.cpp

#include <windows.h>
#include <iostream.h>

void main()
{
	HANDLE fileHandle;
	char filename[MAX_PATH];
	HANDLE mapFileHandle;
	char *mapView;
	int i;

	// get the name of a file
	cout << "Enter filename: ";
	cin >> filename;

	// open the file
	fileHandle = CreateFile(filename, 
		GENERIC_READ | GENERIC_WRITE, 0,
		0, OPEN_EXISTING, 0, 0);
	if (fileHandle == INVALID_HANDLE_VALUE)
	{
		cout << "Error number " << GetLastError()
			<< endl;
		return;
	}

	// create the mapping
	mapFileHandle = CreateFileMapping(fileHandle, 0,
		PAGE_READWRITE, 0, 0, "mapfile");
	if (mapFileHandle <= 0)
	{
		cout << "Mapping error " << GetLastError()
			<< endl;
		return;
	}

	// map the view to an address
	mapView = (char *) MapViewOfFile(mapFileHandle,
		FILE_MAP_WRITE, 0, 0, 0);

	// show the file
	cout << mapView;

	// modify the file
	for (i=0; i<20; i++)
		mapView[i] = 'x';

	// clean up
	if (!UnmapViewOfFile(mapView))
		cout << "problem unmapping\n";
	CloseHandle(mapFileHandle);
	CloseHandle(fileHandle);  
}

