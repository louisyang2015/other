#include <windows.h>
#include <gdiplus.h>
#include <stdio.h>
using namespace Gdiplus;

INT GetEncoderClsid(const WCHAR* format, CLSID* pClsid);  // helper function

INT main()
{
    // Initialize GDI+.
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    CLSID    encoderClsid;
    Status  stat;
    Image*    image = new Image(L"Bird.bmp");

    // Get the CLSID of the PNG encoder.
    GetEncoderClsid(L"image/png", &encoderClsid);

    stat = image->Save(L"Bird.png", &encoderClsid, NULL);

    if(stat == Ok)
        printf("Bird.png was saved successfully\n");
    else
        printf("Failure: stat = %d\n", stat); 

    delete image;
    GdiplusShutdown(gdiplusToken);
    return 0;
}
