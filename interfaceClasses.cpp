#include "interfaceClasses.h"

Color::Color() {}

Color::Color(double r, double g, double b) {
	red = r;
	green = g;
	blue = b;
}

void Color::set(double r, double g, double b) {
	red = r;
	green = g;
	blue = b;
}

double Color::getRed() {return red;}
double Color::getGreen() {return green;}
double Color::getBlue() {return blue;}

Color& Color::operator=(Color &color) {
	red = color.getRed();
	green = color.getGreen();
	blue = color.getBlue();
	return *this;
}

FigureRectangle::FigureRectangle() {}

FigureRectangle::FigureRectangle(double lDx, double lDy, double rUx, double rUy) {
	middleX = (lDx + rUx) / 2;
	middleY = (lDy + rUy) / 2;
	sizeX = (rUx - lDx) / 2;
	sizeY = (rUy - lDy) / 2;

	leftDownX = middleX - lDx;
	leftDownY = middleY - lDy;
	rightUpX = rUx - middleX;
	rightUpY = rUy - middleY;
}

void FigureRectangle::setFigure(double lDx, double lDy, double rUx, double rUy) {
	middleX = (lDx + rUx) / 2;
	middleY = (lDy + rUy) / 2;
	sizeX = (rUx - lDx) / 2;
	sizeY = (rUy - lDy) / 2;

	leftDownX = middleX - lDx;
	leftDownY = middleY - lDy;
	rightUpX = rUx - middleX;
	rightUpY = rUy - middleY;
}

void FigureRectangle::setColor(Color clr) {
	color = clr;
}

double FigureRectangle::getMiddleX() {
	return middleX;
}

double FigureRectangle::getMiddleY() {
	return middleY;
}

double FigureRectangle::getSizeX() {
	return sizeX;
}

double FigureRectangle::getSizeY() {
	return sizeY;
}

Color FigureRectangle::getColor() {
	return color;
}