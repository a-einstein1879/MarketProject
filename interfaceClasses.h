#ifndef INTERFACECLASSES_H
#define INTERFACECLASSES_H

class Color {
private:
	double red;
	double green;
	double blue;
public:
	Color();
	Color(double r, double g, double b);
	void set(double r, double g, double b);
	double getRed();
	double getGreen();
	double getBlue();
	Color& operator=(Color &color);
};

class FigureRectangle {
private:
	double leftDownX;
	double leftDownY;
	double rightUpX;
	double rightUpY;
	Color color;
	double middleX, middleY;
	double sizeX, sizeY;
public:
	FigureRectangle();
	FigureRectangle(double lDx, double lDy, double rUx, double rUy);
	void setFigure(double lDx, double lDy, double rUx, double rUy);
	void setColor(Color clr);
	void resize(double scaleX, double scaleY);

	double getMiddleX();
	double getMiddleY();
	double getSizeX();
	double getSizeY();
	Color getColor();
};

#endif