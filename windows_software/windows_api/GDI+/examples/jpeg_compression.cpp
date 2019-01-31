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

    CLSID                 encoderClsid;
    EncoderParameters encoderParameters;
    ULONG                 quality;
    Status                stat;

    // Get an image from the disk.
    Image* image = new Image(L"Shapes.bmp");

    // Get the CLSID of the JPEG encoder.
    GetEncoderClsid(L"image/jpeg", &encoderClsid);

    // Before we call Image::Save, we must initialize an
    // EncoderParameters object. The EncoderParameters object
    // has an array of EncoderParameter objects. In this
    // case, there is only one EncoderParameter object in the array.
    // The one EncoderParameter object has an array of values.
    // In this case, there is only one value (of type ULONG)
    // in the array. We will let this value vary from 0 to 100.

    encoderParameters.Count = 1;
    encoderParameters.Parameter[0].Guid = EncoderQuality;
    encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
    encoderParameters.Parameter[0].NumberOfValues = 1;

    // Save the image as a JPEG with quality level 0.
    quality = 0;
    encoderParameters.Parameter[0].Value = &quality;
    stat = image->Save(L"Shapes001.jpg", &encoderClsid, &encoderParameters);

    if(stat == Ok)
        wprintf(L"%s saved successfully.\n", L"Shapes001.jpg");
    else
        wprintf(L"%d  Attempt to save %s failed.\n", stat, L"Shapes001.jpg");

    // Save the image as a JPEG with quality level 50.
    quality = 50;
    encoderParameters.Parameter[0].Value = &quality;
    stat = image->Save(L"Shapes050.jpg", &encoderClsid, &encoderParameters);

    if(stat == Ok)
        wprintf(L"%s saved successfully.\n", L"Shapes050.jpg");
    else
        wprintf(L"%d  Attempt to save %s failed.\n", stat, L"Shapes050.jpg");

        // Save the image as a JPEG with quality level 100.
    quality = 100;
    encoderParameters.Parameter[0].Value = &quality;
    stat = image->Save(L"Shapes100.jpg", &encoderClsid, &encoderParameters);

    if(stat == Ok)
        wprintf(L"%s saved successfully.\n", L"Shapes100.jpg");
    else
        wprintf(L"%d  Attempt to save %s failed.\n", stat, L"Shapes100.jpg");

    delete image;
    GdiplusShutdown(gdiplusToken);
    return 0;
}