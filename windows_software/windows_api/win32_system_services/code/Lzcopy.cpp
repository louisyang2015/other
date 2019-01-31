//***************************************************************
// From the book "Win32 System Services: The Heart of Windows 98
// and Windows 2000"
// by Marshall Brain
// Published by Prentice Hall
//
// Copyright 1995, by Prentice Hall.
//
// This code copies a compressed file.
//***************************************************************

// lzcopy.cpp

#include <windows.h>
#include <iostream.h>
#include <stdio.h>

void main()
{
	char sourceFilename[MAX_PATH];
	char destFilename[MAX_PATH];
	OFSTRUCT sourceInfo, destInfo;
	INT sourceHandle, destHandle;

	// get source and dest file names
	cout << "Enter source filename: ";
	cin >> sourceFilename;
	cout << "Enter destination filename: ";
	cin >> destFilename;

	// open both files
 	sourceHandle = LZOpenFile(sourceFilename,
		&sourceInfo, OF_READ);
	if (sourceHandle < 0)
	{
		cout << "Source error = " << sourceHandle
			<< endl;
		return;
	}
	destHandle = LZOpenFile(destFilename, &destInfo,
		OF_CREATE);
	if (destHandle < 0)
	{
		cout << "Dest error = " << destHandle
			<< endl;
		return;
	}
	
	// perform the copy and close the files
	cout << "Result of copy = ";
	cout << LZCopy(sourceHandle, destHandle) 
		<< endl;
	LZClose(sourceHandle);
	LZClose(destHandle);
}




