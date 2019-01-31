Image                image(L"InputColor.bmp");
ImageAttributes  imageAttributes;
UINT                 width = image.GetWidth();
UINT                 height = image.GetHeight();

ColorMatrix colorMatrix = {
    2.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.2f, 0.2f, 0.2f, 0.0f, 1.0f};

imageAttributes.SetColorMatrix(
    &colorMatrix, 
    ColorMatrixFlagsDefault,
    ColorAdjustTypeBitmap);

graphics.DrawImage(&image, 10, 10); //original image

graphics.DrawImage(
    &image, 
    Rect(120, 10, width, height),  // destination rectangle 
    0, 0,          // upper-left corner of source rectangle 
    width,         // width of source rectangle
    height,        // height of source rectangle
    UnitPixel,
    &imageAttributes); //image with the transformed color