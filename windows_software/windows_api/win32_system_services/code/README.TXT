Chapter 2 talks about files. The example code demonstrates
a number of file-handling capabilities in the Win32 API.

CHAP2.MAK
  Change the file in the project to compile the 
  different examples in this directory under Visual C++.
  Note that the LZ files require LZ32.LIB, which has 
  already been added to the link settings of the project.

COPYFILE.cpp
  This code copies a file.

DELFILE.CPP
  This code deletes a file.

FILE1.CPP
  This code demonstrates a simple C program that reads a file.

FILE2.CPP
  This code demonstrates a simple C++ program that reads a file.

FILE3.CPP
  This code demonstrates a Win32 program that reads a file.

FILEATTR.CPP
  This code retrieves the attribute bits for a file.

FILEINFO.CPP
  This code demonstrates the GetFileInformationByHandle
  function.

FILELCK1.CPP
  This code shows how to lock part of a file.

FILELCK2.CPP
  This code shows how to lock part of a file with LockFileEx.

FILEPATH.CPP
  This code gets the full path name for a file.

FILESIZE.CPP
  This code demonstrates how to retrieve the size of a file.

FILETIME.CPP
  This code demonstrates how to retrieve a file's creation,
  last access and last write times.

FILEWRIT.CPP
  This code writes structures to a file.

LZCOPY.CPP
  This code copies a compressed file.

LZREAD.CPP
  This code reads from a compressed file.

LZTEST.CPP
  This code creates a file that will be compressed for testing.

MAKEFILE
  Change the APP variable in the makefile to compile the 
  different examples in this directory under the SDK. 
  Note that the LZ files require LZ32.LIB, which has 
  already been added to the makefile.


MAPFILE.CPP
  This code demonstrates a mapped file.

MAPFILEW.CPP
  This code demonstrates writing to a mapped file.

MOVXFILE.CPP
  This code moves a file.

TEMPFILE.CPP
  This code creates a temporary file.
