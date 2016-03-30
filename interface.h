#ifndef INTERFACE_H
#define INTERFACE_H

#include <windows.h>
#include <glut.h>

#include <stdlib.h>
// Include tchar to define _T
#include <tchar.h>
#include "chart.h"
#include "interfaceClasses.h"
#include "cmn_defines.h"

class OpenGLInterface {
private:
	static OpenGLInterface *p_OpenGLInterface;
	Cmn_Defines *cmn_defines;

	HGLRC	hRC;
	HDC		hDC;
	HWND	hWnd;
	HINSTANCE hInstance;
	
	bool  keys[256];
	bool  active;
	bool  fullscreen;

	void printLineChart(LineChart &lineChart, FigureRectangle rectangle);
	void printHistogram(Histogram &histogram, FigureRectangle rectangle);
	int drawRectangle(FigureRectangle rectangle);
	void drawText(const char *text, int length, float x, float y);
	void drawArgumentLabels(double minArgument, double maxArgument, int numberOfBins, FigureRectangle rectangle);
	void drawValueLabels(double minValue, double maxValue, int numberOfBins, FigureRectangle rectangle);

	/* Window creation */
	int InitGL(GLvoid);
	BOOL CreateGLWindow(LPCWSTR title, int width, int height, int bits, bool fullscreenflag);
	GLvoid ReSizeGLScene(GLsizei width, GLsizei height);

	LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK InitialWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	/* End of window creation */
public:
	OpenGLInterface();
	static OpenGLInterface* getOpenGLInterface();
	GLvoid KillGLWindow(GLvoid);
	
	void printCharts(Histogram &histogram, LineChart &lineChartPrices, LineChart &lineChartNumberOfObjects);
};

#endif INTERFACE_H