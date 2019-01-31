Point points[] = {Point(40, 60), Point(50, 70), Point(30, 90)};

Pen pen(Color(255, 255, 0, 0), 2);
GraphicsPath path;

// The first figure is started automatically, so there is
// no need to call StartFigure here.
path.AddArc(175, 50, 50, 50, 0.0f, -180.0f);
path.AddLine(100, 0, 250, 20);

path.StartFigure();
path.AddLine(50, 20, 5, 90);
path.AddCurve(points, 3);
path.AddLine(50, 150, 150, 180);
path.CloseFigure();

graphics.DrawPath(&pen, &path);