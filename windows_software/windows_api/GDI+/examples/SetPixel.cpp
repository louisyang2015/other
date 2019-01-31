INT iWidth = bitmap.GetWidth();
INT iHeight = bitmap.GetHeight();
Color color, colorTemp;

for(INT iRow = 0; iRow < iHeight; iRow++)
{
    for(INT iColumn = 0; iColumn < iWidth; iColumn++)
    {
        bitmap.GetPixel(iColumn, iRow, &color);
        colorTemp.SetValue(color.MakeARGB(
            (BYTE)(255 * iColumn / iWidth), 
            color.GetRed(),
            color.GetGreen(),
            color.GetBlue()));
        bitmap.SetPixel(iColumn, iRow, colorTemp);
    }
}

// First draw a wide black line.
Pen pen(Color(255, 0, 0, 0), 25);
graphics.DrawLine(&pen, 10, 35, 200, 35);

// Now draw the modified bitmap.
graphics.DrawImage(&bitmap, 30, 0, iWidth, iHeight);
