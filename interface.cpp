#include "interface.h"

#include <stdlib.h>
// Include tchar to define _T
#include <tchar.h>

OpenGLInterface* OpenGLInterface::p_OpenGLInterface = 0;

OpenGLInterface::OpenGLInterface() {
	cmn_defines = cmn_defines->getCmn_Defines();
	hRC  = NULL;
	hDC  = NULL;
	hWnd = NULL;

	active = true;
	fullscreen = false;
	
	//CreateGLWindow(L"OpenGL window", 800, 600, 32, fullscreen);
	CreateGLWindow(L"OpenGL window", 1024, 768, 32, fullscreen);
}

OpenGLInterface* OpenGLInterface::getOpenGLInterface() {
	if(!p_OpenGLInterface) {
		p_OpenGLInterface = new OpenGLInterface();
	}
	return p_OpenGLInterface;
}

void OpenGLInterface::drawText(const char *text, int length, float x, float y)
{
	glLoadIdentity();
	glRasterPos2f(x, y);
	for(int i = 0; i < length; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)text[i]);
	}
}

/* TODO: understand why it doesn`t work without & */
void OpenGLInterface::printCharts(Histogram &histogram, LineChart &lineChartPrices, LineChart &lineChartNumberOfObjects) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Color color(0, 1, 0);
	FigureRectangle rectangle(-1, 0, 1, 1);
	rectangle.setColor(color);
	printHistogram(histogram, rectangle);
	rectangle.setFigure(-1, -1, 0, 0);
	printLineChart(lineChartPrices, rectangle);
	rectangle.setFigure(0, -1, 1, 0);
	printLineChart(lineChartNumberOfObjects, rectangle);
	SwapBuffers(hDC);
//	DrawRectangle(rectangle);
//	SwapBuffers(hDC);
}

#include <stdio.h>
void OpenGLInterface::printLineChart(LineChart &lineChart, FigureRectangle rectangle) {
	/* Put coursor back to (0, 0, 0) */
    glLoadIdentity();
	
	drawArgumentLabels(lineChart.getMinArgument(), lineChart.getMaxActiveArgument(), 4, rectangle);
	drawValueLabels(lineChart.getMinValue(), lineChart.getMaxValue(), 4, rectangle);
	rectangle.resize(0.9, 0.93);

	double startX = rectangle.getMiddleX() - rectangle.getSizeX() / 2;
	double startY = rectangle.getMiddleY() - rectangle.getSizeY() / 2;
	double scaleY = rectangle.getSizeY();
	double maxValue = lineChart.getMaxValue();
	double minValue = lineChart.getMinValue();
	/* TODO: Create mode with const and changing boarders */
	if(maxValue < 30) {maxValue = 30;}
	//if(maxValue > 30) {printf("delta = %.2f\n", lineChart.getValue(0, lineChart.getMaxActiveArgument(0) - 1) - lineChart.getValue(1, lineChart.getMaxActiveArgument(1) - 1));}
/*	maxValue = 20;*/
	minValue = 0;

	for(int j = 0; j < lineChart.getNumberOfCharts(); j++) {
		int numberOfArguments = lineChart.getMaxActiveArgument(j);
		double widthStep = rectangle.getSizeX() / numberOfArguments;
		glLineWidth(1);
		Color color = lineChart.getColor(j);
		glColor3f(color.getRed(), color.getGreen(), color.getBlue());
		for(int i = 1; i < numberOfArguments; i++) {
			glBegin(GL_LINES);
				glVertex3f(startX + (i - 1) * widthStep,
				startY + ( lineChart.getValue(j, i - 1) - minValue )/ ( maxValue - minValue ) * scaleY, 0);
				glVertex3f(startX + i       * widthStep,
				startY + ( lineChart.getValue(j, i)     - minValue )/ ( maxValue - minValue ) * scaleY, 0);
			glEnd();
		}
	}
}

void OpenGLInterface::printHistogram(Histogram &histogram, FigureRectangle rectangle) {
	/* Put coursor back to (0, 0, 0) */
    glLoadIdentity();
	int numberOfBins = histogram.getNumberOfArguments();
	int numberOfCharts = histogram.getNumberOfCharts();
	double binWidth = 1 / double(numberOfBins);

	drawArgumentLabels(histogram.getMinArgument(), histogram.getMaxArgument(), 4, rectangle);
	drawValueLabels(histogram.getMinValue(), histogram.getMaxValue(), 4, rectangle);
	rectangle.resize(0.93, 0.93);

	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
	double leftDownX = rectangle.getMiddleX() - rectangle.getSizeX() / 2;
	double leftDownY = rectangle.getMiddleY() - rectangle.getSizeY() / 2;
	double sizeX = rectangle.getSizeX();
	double sizeY = rectangle.getSizeY();
	double sideGap = cmn_defines->getSideGap();

	for(int j = 0; j < numberOfCharts; j++) {
		FigureRectangle histogramColumn;
		Color color = histogram.getColor(j);
		histogramColumn.setColor(color);

		for(int i = 0; i < numberOfBins; i++) {
			histogramColumn.setFigure(leftDownX + i * binWidth * sizeX, leftDownY, leftDownX + ((i + 1) * binWidth - binWidth * sideGap) * sizeX, leftDownY + histogram.getValue(j, i) / histogram.getMaxValue() * sizeY);
			drawRectangle(histogramColumn);
		}
	}
	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
}

void OpenGLInterface::drawArgumentLabels(double minArgument, double maxArgument, int numberOfBins, FigureRectangle rectangle) {
	double startX = rectangle.getMiddleX() - rectangle.getSizeX() / 2;
	double deltaX = rectangle.getSizeX() / double(numberOfBins);
	double y = rectangle.getMiddleY() - rectangle.getSizeY() / 2;
	double delta = (maxArgument - minArgument) / double(numberOfBins);
	for(int i = 0; i < numberOfBins; i++) {
		char bufer[10];
		_itoa_s(minArgument + delta * i, bufer, 10, 10);
		drawText(bufer, sizeof(bufer) / sizeof(char), startX + deltaX * i, y);
	}
}

void OpenGLInterface::drawValueLabels(double minValue, double maxValue, int numberOfBins, FigureRectangle rectangle) {
	double startY = rectangle.getMiddleY() - rectangle.getSizeY() / 2;
	double deltaY = rectangle.getSizeY() / double(numberOfBins);
	double x = rectangle.getMiddleX() - rectangle.getSizeX() / 2;
	double delta = (maxValue - minValue) / double(numberOfBins);
	for(int i = 0; i < numberOfBins; i++) {
		char bufer[10];
		_itoa_s(minValue + delta * i, bufer, 10, 10);
		drawText(bufer, sizeof(bufer) / sizeof(char), x + 0.02, startY + deltaY * i);
	}
}

void OpenGLInterface::drawRectangle(FigureRectangle rectangle) {
	glLoadIdentity();
	Color color = rectangle.getColor();
	glColor3f(color.getRed(), color.getGreen(), color.getBlue());
	glTranslatef(rectangle.getMiddleX(), rectangle.getMiddleY(), 0.0);
	double halfSizeX = rectangle.getSizeX() / 2;
	double halfSizeY = rectangle.getSizeY() / 2;
	glBegin(GL_QUADS);
		glVertex3f(-halfSizeX, halfSizeY, 0.0);
		glVertex3f(halfSizeX,  halfSizeY, 0.0);
		glVertex3f(halfSizeX, -halfSizeY, 0.0);
		glVertex3f(-halfSizeX,-halfSizeY, 0.0);
	glEnd();
}

int OpenGLInterface::InitGL(GLvoid) {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	return true;
}

BOOL OpenGLInterface::CreateGLWindow( LPCWSTR title, int width, int height, int bits, bool fullscreenflag ) {
	GLuint    PixelFormat;
	WNDCLASS  wc;
	DWORD    dwExStyle;
	DWORD    dwStyle;
	
	RECT WindowRect;                // Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;              // Óñòàíîâèòü ëåâóþ ñîñòàâëÿþùóþ â 0
	WindowRect.right=(long)width;              // Óñòàíîâèòü ïðàâóþ ñîñòàâëÿþùóþ â Width
	WindowRect.top=(long)0;                // Óñòàíîâèòü âåðõíþþ ñîñòàâëÿþùóþ â 0
	WindowRect.bottom=(long)height;              // Óñòàíîâèòü íèæíþþ ñîñòàâëÿþùóþ â Height

	fullscreen = fullscreenflag;              // Óñòàíàâëèâàåì çíà÷åíèå ãëîáàëüíîé ïåðåìåííîé fullscreen
	hInstance    = GetModuleHandle(NULL);        // Ñ÷èòàåì äåñêðèïòîð íàøåãî ïðèëîæåíèÿ
	wc.style    = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;      // Ïåðåðèñóåì ïðè ïåðåìåùåíèè è ñîçäà¸ì ñêðûòûé DC
	wc.lpfnWndProc    = &OpenGLInterface::InitialWndProc;          // Ïðîöåäóðà îáðàáîòêè ñîîáùåíèé
	wc.cbClsExtra    = 0;              // Íåò äîïîëíèòåëüíîé èíôîðìàöèè äëÿ îêíà
	wc.cbWndExtra    = 0;              // Íåò äîïîëíèòåëüíîé èíôîðìàöèè äëÿ îêíà
	wc.hInstance    = hInstance;            // Óñòàíàâëèâàåì äåñêðèïòîð
	wc.hIcon    = LoadIcon(NULL, IDI_WINLOGO);        // Çàãðóæàåì èêîíêó ïî óìîë÷àíèþ
	wc.hCursor    = LoadCursor(NULL, IDC_ARROW);        // Çàãðóæàåì óêàçàòåëü ìûøêè
	wc.hbrBackground  = NULL;              // Ôîí íå òðåáóåòñÿ äëÿ GL
	wc.lpszMenuName    = NULL;              // Ìåíþ â îêíå íå áóäåò
	wc.lpszClassName  = L"OpenGL";            // Óñòàíàâëèâàåì èìÿ êëàññó

	if( !RegisterClass( &wc ) ) {
    MessageBox( NULL, L"Failed To Register The Window Class.", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
    return false;                // Âûõîä è âîçâðàùåíèå ôóíêöèåé çíà÷åíèÿ false
	}

	if( fullscreen ) {
		DEVMODE dmScreenSettings;            // Ðåæèì óñòðîéñòâà
		memset( &dmScreenSettings, 0, sizeof( dmScreenSettings ) );    // Î÷èñòêà äëÿ õðàíåíèÿ óñòàíîâîê
		dmScreenSettings.dmSize=sizeof( dmScreenSettings );      // Ðàçìåð ñòðóêòóðû Devmode
		dmScreenSettings.dmPelsWidth  =   width;        // Øèðèíà ýêðàíà
		dmScreenSettings.dmPelsHeight  =   height;        // Âûñîòà ýêðàíà
		dmScreenSettings.dmBitsPerPel  =   bits;        // Ãëóáèíà öâåòà
		dmScreenSettings.dmFields= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;// Ðåæèì Ïèêñåëÿ
		// Ïûòàåìñÿ óñòàíîâèòü âûáðàííûé ðåæèì è ïîëó÷èòü ðåçóëüòàò.  Ïðèìå÷àíèå: CDS_FULLSCREEN óáèðàåò ïàíåëü óïðàâëåíèÿ.
		if( ChangeDisplaySettings( &dmScreenSettings, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL ) {
			if( MessageBox( NULL, L"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", L"NeHe GL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES ) {
				fullscreen = false;
			} else {
				MessageBox( NULL, L"Program Will Now Close.", L"ERROR", MB_OK | MB_ICONSTOP );
				return false;
			}
		}
	}

	if(fullscreen) {
		dwExStyle  =   WS_EX_APPWINDOW;          // Ðàñøèðåííûé ñòèëü îêíà
		dwStyle    =   WS_POPUP;            // Îáû÷íûé ñòèëü îêíà
		ShowCursor( true );
	} else {
		dwExStyle  =   WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;      // Ðàñøèðåííûé ñòèëü îêíà
		dwStyle    =   WS_OVERLAPPEDWINDOW;        // Îáû÷íûé ñòèëü îêíà
	}
	AdjustWindowRectEx( &WindowRect, dwStyle, false, dwExStyle );

	if( !( hWnd = CreateWindowEx(  dwExStyle,          // Ðàñøèðåííûé ñòèëü äëÿ îêíà
          _T("OpenGL"),          // Èìÿ êëàññà
          title,            // Çàãîëîâîê îêíà
          WS_CLIPSIBLINGS |        // Òðåáóåìûé ñòèëü äëÿ îêíà
          WS_CLIPCHILDREN |        // Òðåáóåìûé ñòèëü äëÿ îêíà
          dwStyle,          // Âûáèðàåìûå ñòèëè äëÿ îêíà
          0, 0,            // Ïîçèöèÿ îêíà
          WindowRect.right-WindowRect.left,    // Âû÷èñëåíèå ïîäõîäÿùåé øèðèíû
          WindowRect.bottom-WindowRect.top,    // Âû÷èñëåíèå ïîäõîäÿùåé âûñîòû
          NULL,            // Íåò ðîäèòåëüñêîãî
          NULL,            // Íåò ìåíþ
          hInstance,          // Äåñêðèïòîð ïðèëîæåíèÿ
          this ) ) ) {          // Íå ïåðåäà¸ì íè÷åãî äî WM_CREATE (???)
			  KillGLWindow();                // Âîññòàíîâèòü ýêðàí
			  MessageBox( NULL, L"Window Creation Error.", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
			  return false;                // Âåðíóòü false
	}

	static  PIXELFORMATDESCRIPTOR pfd=            // pfd ñîîáùàåò Windows êàêèì áóäåò âûâîä íà ýêðàí êàæäîãî ïèêñåëÿ
  {
    sizeof(PIXELFORMATDESCRIPTOR),            // Ðàçìåð äåñêðèïòîðà äàííîãî ôîðìàòà ïèêñåëåé
    1,                  // Íîìåð âåðñèè
    PFD_DRAW_TO_WINDOW |              // Ôîðìàò äëÿ Îêíà
    PFD_SUPPORT_OPENGL |              // Ôîðìàò äëÿ OpenGL
    PFD_DOUBLEBUFFER,              // Ôîðìàò äëÿ äâîéíîãî áóôåðà
    PFD_TYPE_RGBA,                // Òðåáóåòñÿ RGBA ôîðìàò
    bits,                  // Âûáèðàåòñÿ áèò ãëóáèíû öâåòà
    0, 0, 0, 0, 0, 0,              // Èãíîðèðîâàíèå öâåòîâûõ áèòîâ
    0,                  // Íåò áóôåðà ïðîçðà÷íîñòè
    0,                  // Ñäâèãîâûé áèò èãíîðèðóåòñÿ
    0,                  // Íåò áóôåðà íàêîïëåíèÿ
    0, 0, 0, 0,                // Áèòû íàêîïëåíèÿ èãíîðèðóþòñÿ
    32,                  // 32 áèòíûé Z-áóôåð (áóôåð ãëóáèíû)
    0,                  // Íåò áóôåðà òðàôàðåòà
    0,                  // Íåò âñïîìîãàòåëüíûõ áóôåðîâ
    PFD_MAIN_PLANE,                // Ãëàâíûé ñëîé ðèñîâàíèÿ
    0,                  // Çàðåçåðâèðîâàíî
    0, 0, 0                  // Ìàñêè ñëîÿ èãíîðèðóþòñÿ
  };

	if( !( hDC = GetDC( hWnd ) ) ) {             // Ìîæåì ëè ìû ïîëó÷èòü Êîíòåêñò Óñòðîéñòâà?
		KillGLWindow();                // Âîññòàíîâèòü ýêðàí
		MessageBox( NULL, L"Can't Create A GL Device Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
		return false;                // Âåðíóòü false
	}
	
	if( !( PixelFormat = ChoosePixelFormat( hDC, &pfd ) ) )        // Íàéäåí ëè ïîäõîäÿùèé ôîðìàò ïèêñåëÿ?
  {
    KillGLWindow();                // Âîññòàíîâèòü ýêðàí
    MessageBox( NULL, L"Can't Find A Suitable PixelFormat.", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
    return false;                // Âåðíóòü false
  }
	
	if( !SetPixelFormat( hDC, PixelFormat, &pfd ) )          // Âîçìîæíî ëè óñòàíîâèòü Ôîðìàò Ïèêñåëÿ?
  {
    KillGLWindow();                // Âîññòàíîâèòü ýêðàí
    MessageBox( NULL, L"Can't Set The PixelFormat.", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
    return false;                // Âåðíóòü false
  }
	
	if( !( hRC = wglCreateContext( hDC ) ) )          // Âîçìîæíî ëè óñòàíîâèòü Êîíòåêñò Ðåíäåðèíãà?
  {
    KillGLWindow();                // Âîññòàíîâèòü ýêðàí
    MessageBox( NULL, L"Can't Create A GL Rendering Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
    return false;                // Âåðíóòü false
  }

	if( !wglMakeCurrent( hDC, hRC ) )            // Ïîïðîáîâàòü àêòèâèðîâàòü Êîíòåêñò Ðåíäåðèíãà
  {
    KillGLWindow();                // Âîññòàíîâèòü ýêðàí
    MessageBox( NULL, L"Can't Activate The GL Rendering Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
    return false;                // Âåðíóòü false
  }

	ShowWindow( hWnd, SW_SHOW );              // Ïîêàçàòü îêíî
	SetForegroundWindow( hWnd );              // Ñëåãêà ïîâûñèì ïðèîðèòåò
	SetFocus( hWnd );                // Óñòàíîâèòü ôîêóñ êëàâèàòóðû íà íàøå îêíî
	ReSizeGLScene( width, height );              // Íàñòðîèì ïåðñïåêòèâó äëÿ íàøåãî OpenGL ýêðàíà.

	if( !InitGL() )                  // Èíèöèàëèçàöèÿ òîëüêî ÷òî ñîçäàííîãî îêíà
  {
    KillGLWindow();                // Âîññòàíîâèòü ýêðàí
    MessageBox( NULL, _T("Initialization Failed."), _T("ERROR"), MB_OK | MB_ICONEXCLAMATION );
    return false;                // Âåðíóòü false
  }

	return true;
}

GLvoid OpenGLInterface::KillGLWindow( GLvoid ) {
	if( fullscreen ) {
		ChangeDisplaySettings( NULL, 0 );
		ShowCursor( true );
	}
	if( hRC ) {
		if( !wglMakeCurrent( NULL, NULL ) ) {
			MessageBox( NULL, L"Release Of DC And RC Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
		}
		if( !wglDeleteContext( hRC ) ) {
			MessageBox( NULL, L"Release Rendering Context Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
		}
		hRC = NULL;
	}
	if( hDC && !ReleaseDC( hWnd, hDC ) ) {
		MessageBox( NULL, L"Release Device Context Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
		hDC = NULL;
	}
	if(hWnd && !DestroyWindow(hWnd)) {
		MessageBox( NULL, L"Could Not Release hWnd.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
		hWnd = NULL;
	}
	if( !UnregisterClass( L"OpenGL", hInstance ) ) {
		MessageBox( NULL, L"Could Not Unregister Class.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hInstance = NULL; 
	}
}

GLvoid OpenGLInterface::ReSizeGLScene(GLsizei width, GLsizei height) {
	if( height == 0 ) {
		height = 1;
	}
	glViewport(0, 0, width, height);
	/*glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective( 45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f );

    glMatrixMode( GL_MODELVIEW );*/
    glLoadIdentity();
}

LRESULT OpenGLInterface::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_ACTIVATE:
		{
			if( !HIWORD( wParam ) ){
				active = true;
			} else {
				active = false;
			}
			return 0;
		}
	case WM_SYSCOMMAND:
		{
			switch ( wParam ) {
			case SC_SCREENSAVE:
			case SC_MONITORPOWER:
			return 0;
			}
			break;
		}
	case WM_CLOSE:
		{
			PostQuitMessage( 0 );
			return 0;
		}
	case WM_KEYDOWN:
		{
			keys[wParam] = true;
			return 0;
		}
	case WM_KEYUP:
		{
			keys[wParam] = false;
			return 0;
		}
	case WM_SIZE:
		{
			ReSizeGLScene( LOWORD(lParam), HIWORD(lParam) );
			return 0;
		}
	}
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}


LRESULT CALLBACK OpenGLInterface::InitialWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
      if (Msg == WM_NCCREATE) {
        LPCREATESTRUCT create_struct = reinterpret_cast<LPCREATESTRUCT>(lParam);
        void *lpCreateParam = create_struct->lpCreateParams;
        OpenGLInterface *this_window = reinterpret_cast<OpenGLInterface *>(lpCreateParam);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this_window));
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&OpenGLInterface::StaticWndProc));
        return this_window->WndProc(hWnd, Msg, wParam, lParam);
      }
      return DefWindowProc(hWnd, Msg, wParam, lParam);
}

LRESULT CALLBACK OpenGLInterface::StaticWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
      LONG_PTR user_data = GetWindowLongPtr(hWnd, GWLP_USERDATA);
      OpenGLInterface *this_window = reinterpret_cast<OpenGLInterface *>(user_data);
      return this_window->WndProc(hWnd, Msg, wParam, lParam);
}
