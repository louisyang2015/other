//***************************************************************
// From the book "Win32 System Services: The Heart of Windows 98
// and Windows 2000"
// by Marshall Brain
// Published by Prentice Hall
//
// Copyright 1995, by Prentice Hall..
//
// This code demonstrates a simple C++ program that reads a file.
//***************************************************************

// file2.cpp

#include <iostream.h>
#include <fstream.h>
#include <windows.h>

void main()
{
	char filename[MAX_PATH];
	char c;

	// get the file name
	cout << "Enter filename: ";
	cin >> filename;

	// open the file
	ifstream infile(filename);
	if (infile)
		// read until eof
		while (infile.get(c))
			cout << c;

	// close the file
	infile.close();
}

