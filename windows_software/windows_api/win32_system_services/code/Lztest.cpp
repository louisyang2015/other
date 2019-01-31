//***************************************************************
// From the book "Win32 System Services: The Heart of Windows 98
// and Windows 2000"
// by Marshall Brain
// Published by Prentice Hall
//
// Copyright 1995, by Prentice Hall.
//
// This code creates a file that will be compressed for testing.
//***************************************************************

// lztest.cpp

#include <iostream.h>
#include <fstream.h>
#include <windows.h>

void main()
{
	char filename[MAX_PATH];
	int x;

	cout << "Enter filename: ";
	cin >> filename;
	ofstream f(filename);
	if (f)
		for (x=0; x<10000; x++)
			f << "This is test line " << x << endl;
	f.close();
}


