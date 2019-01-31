// Create a Bitmap object and load it with the texture image.
Bitmap bitmap(L"Texture1.jpg");
Pen pen(Color(255, 0, 0, 0), 25);

// Initialize the color matrix.
// Notice the value 0.8 in row 4, column 4.
ColorMatrix colorMatrix = {	1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 0.8f, 0.0f,
							0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

// Create an ImageAttributes object and set its color matrix.
ImageAttributes imageAtt;
imageAtt.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault,
    ColorAdjustTypeBitmap);

// First draw a wide black line.
graphics.DrawLine(&pen, Point(10, 35), Point(200, 35));

// Now draw the semitransparent bitmap image.
INT iWidth = bitmap.GetWidth();
INT iHeight = bitmap.GetHeight();
graphics.DrawImage(
    &bitmap, 
    Rect(30, 0, iWidth, iHeight),  // Destination rectangle
    0,                                      // Source rectangle X 
    0,                                      // Source rectangle Y
    iWidth,                                // Source rectangle width
    iHeight,                              // Source rectangle height
    UnitPixel, 
    &imageAtt);
