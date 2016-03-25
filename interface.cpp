#include "interface.h"

#include <stdlib.h>
// Include tchar to define _T
#include <tchar.h>

OpenGLInterface* OpenGLInterface::p_OpenGLInterface = 0;

OpenGLInterface::OpenGLInterface() {
	hRC  = NULL;
	hDC  = NULL;
	hWnd = NULL;

	active = true;
	fullscreen = false;
	
	CreateGLWindow(L"OpenGL window", 800, 600, 32, fullscreen);
	//CreateGLWindow(L"OpenGL window", 1024, 768, 32, fullscreen);
}

OpenGLInterface* OpenGLInterface::getOpenGLInterface() {
	if(!p_OpenGLInterface) {
		p_OpenGLInterface = new OpenGLInterface();
	}
	return p_OpenGLInterface;
}

void OpenGLInterface::printPriceHistogram(Histogram &histogram) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Color color(1, 0, 0);
	FigureRectangle rectangle(-1, 0, 1, 1);
	rectangle.setColor(color);
	printHistogram(histogram, rectangle);
	rectangle.setFigure(-1, -1, 1, 0);
	printHistogram(histogram, rectangle);
	SwapBuffers(hDC);
//	DrawRectangle(rectangle);
//	SwapBuffers(hDC);
}

#define SIDEGAP 0.05

/* TODO: understand why it doesn`t work without & */
void OpenGLInterface::printHistogram(Histogram &histogram, FigureRectangle rectangle) {
	int numberOfBins = histogram.getNumberOfBins();
	int numberOfCharts = histogram.getNumberOfCharts();
	double binWidth = 1 / double(numberOfBins);

	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
	double leftDownX = rectangle.getMiddleX() - rectangle.getSizeX() / 2;
	double leftDownY = rectangle.getMiddleY() - rectangle.getSizeY() / 2;
	double sizeX = rectangle.getSizeX();
	double sizeY = rectangle.getSizeY();
	for(int j = 0; j < numberOfCharts; j++) {
		FigureRectangle histogramColumn;
		Color color = histogram.getColor(j);
		histogramColumn.setColor(color);

		for(int i = 0; i < numberOfBins; i++) {
			histogramColumn.setFigure(leftDownX + i * binWidth * sizeX, leftDownY, leftDownX + ((i + 1) * binWidth - binWidth * SIDEGAP) * sizeX, leftDownY + histogram.getValue(j, i) / histogram.getMaxValue() * sizeY);
			DrawRectangle(histogramColumn);
		}
	}
	glDisable(GL_BLEND);
}

int OpenGLInterface::DrawRectangle(FigureRectangle rectangle) {
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
	
	return true;
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
	WindowRect.left=(long)0;              // ���������� ����� ������������ � 0
	WindowRect.right=(long)width;              // ���������� ������ ������������ � Width
	WindowRect.top=(long)0;                // ���������� ������� ������������ � 0
	WindowRect.bottom=(long)height;              // ���������� ������ ������������ � Height

	fullscreen = fullscreenflag;              // ������������� �������� ���������� ���������� fullscreen
	hInstance    = GetModuleHandle(NULL);        // ������� ���������� ������ ����������
	wc.style    = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;      // ���������� ��� ����������� � ������ ������� DC
	wc.lpfnWndProc    = &OpenGLInterface::InitialWndProc;          // ��������� ��������� ���������
	wc.cbClsExtra    = 0;              // ��� �������������� ���������� ��� ����
	wc.cbWndExtra    = 0;              // ��� �������������� ���������� ��� ����
	wc.hInstance    = hInstance;            // ������������� ����������
	wc.hIcon    = LoadIcon(NULL, IDI_WINLOGO);        // ��������� ������ �� ���������
	wc.hCursor    = LoadCursor(NULL, IDC_ARROW);        // ��������� ��������� �����
	wc.hbrBackground  = NULL;              // ��� �� ��������� ��� GL
	wc.lpszMenuName    = NULL;              // ���� � ���� �� �����
	wc.lpszClassName  = L"OpenGL";            // ������������� ��� ������

	if( !RegisterClass( &wc ) ) {
    MessageBox( NULL, L"Failed To Register The Window Class.", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
    return false;                // ����� � ����������� �������� �������� false
	}

	if( fullscreen ) {
		DEVMODE dmScreenSettings;            // ����� ����������
		memset( &dmScreenSettings, 0, sizeof( dmScreenSettings ) );    // ������� ��� �������� ���������
		dmScreenSettings.dmSize=sizeof( dmScreenSettings );      // ������ ��������� Devmode
		dmScreenSettings.dmPelsWidth  =   width;        // ������ ������
		dmScreenSettings.dmPelsHeight  =   height;        // ������ ������
		dmScreenSettings.dmBitsPerPel  =   bits;        // ������� �����
		dmScreenSettings.dmFields= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;// ����� �������
		// �������� ���������� ��������� ����� � �������� ���������.  ����������: CDS_FULLSCREEN ������� ������ ����������.
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
		dwExStyle  =   WS_EX_APPWINDOW;          // ����������� ����� ����
		dwStyle    =   WS_POPUP;            // ������� ����� ����
		ShowCursor( true );
	} else {
		dwExStyle  =   WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;      // ����������� ����� ����
		dwStyle    =   WS_OVERLAPPEDWINDOW;        // ������� ����� ����
	}
	AdjustWindowRectEx( &WindowRect, dwStyle, false, dwExStyle );

	if( !( hWnd = CreateWindowEx(  dwExStyle,          // ����������� ����� ��� ����
          _T("OpenGL"),          // ��� ������
          title,            // ��������� ����
          WS_CLIPSIBLINGS |        // ��������� ����� ��� ����
          WS_CLIPCHILDREN |        // ��������� ����� ��� ����
          dwStyle,          // ���������� ����� ��� ����
          0, 0,            // ������� ����
          WindowRect.right-WindowRect.left,    // ���������� ���������� ������
          WindowRect.bottom-WindowRect.top,    // ���������� ���������� ������
          NULL,            // ��� �������������
          NULL,            // ��� ����
          hInstance,          // ���������� ����������
          this ) ) ) {          // �� ������� ������ �� WM_CREATE (???)
			  KillGLWindow();                // ������������ �����
			  MessageBox( NULL, L"Window Creation Error.", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
			  return false;                // ������� false
	}

	static  PIXELFORMATDESCRIPTOR pfd=            // pfd �������� Windows ����� ����� ����� �� ����� ������� �������
  {
    sizeof(PIXELFORMATDESCRIPTOR),            // ������ ����������� ������� ������� ��������
    1,                  // ����� ������
    PFD_DRAW_TO_WINDOW |              // ������ ��� ����
    PFD_SUPPORT_OPENGL |              // ������ ��� OpenGL
    PFD_DOUBLEBUFFER,              // ������ ��� �������� ������
    PFD_TYPE_RGBA,                // ��������� RGBA ������
    bits,                  // ���������� ��� ������� �����
    0, 0, 0, 0, 0, 0,              // ������������� �������� �����
    0,                  // ��� ������ ������������
    0,                  // ��������� ��� ������������
    0,                  // ��� ������ ����������
    0, 0, 0, 0,                // ���� ���������� ������������
    32,                  // 32 ������ Z-����� (����� �������)
    0,                  // ��� ������ ���������
    0,                  // ��� ��������������� �������
    PFD_MAIN_PLANE,                // ������� ���� ���������
    0,                  // ���������������
    0, 0, 0                  // ����� ���� ������������
  };

	if( !( hDC = GetDC( hWnd ) ) ) {             // ����� �� �� �������� �������� ����������?
		KillGLWindow();                // ������������ �����
		MessageBox( NULL, L"Can't Create A GL Device Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
		return false;                // ������� false
	}
	
	if( !( PixelFormat = ChoosePixelFormat( hDC, &pfd ) ) )        // ������ �� ���������� ������ �������?
  {
    KillGLWindow();                // ������������ �����
    MessageBox( NULL, L"Can't Find A Suitable PixelFormat.", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
    return false;                // ������� false
  }
	
	if( !SetPixelFormat( hDC, PixelFormat, &pfd ) )          // �������� �� ���������� ������ �������?
  {
    KillGLWindow();                // ������������ �����
    MessageBox( NULL, L"Can't Set The PixelFormat.", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
    return false;                // ������� false
  }
	
	if( !( hRC = wglCreateContext( hDC ) ) )          // �������� �� ���������� �������� ����������?
  {
    KillGLWindow();                // ������������ �����
    MessageBox( NULL, L"Can't Create A GL Rendering Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
    return false;                // ������� false
  }

	if( !wglMakeCurrent( hDC, hRC ) )            // ����������� ������������ �������� ����������
  {
    KillGLWindow();                // ������������ �����
    MessageBox( NULL, L"Can't Activate The GL Rendering Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
    return false;                // ������� false
  }

	ShowWindow( hWnd, SW_SHOW );              // �������� ����
	SetForegroundWindow( hWnd );              // ������ ������� ���������
	SetFocus( hWnd );                // ���������� ����� ���������� �� ���� ����
	ReSizeGLScene( width, height );              // �������� ����������� ��� ������ OpenGL ������.

	if( !InitGL() )                  // ������������� ������ ��� ���������� ����
  {
    KillGLWindow();                // ������������ �����
    MessageBox( NULL, _T("Initialization Failed."), _T("ERROR"), MB_OK | MB_ICONEXCLAMATION );
    return false;                // ������� false
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
	switch (uMsg)                // �������� ��������� ��� ����
  {
	      case WM_ACTIVATE:            // �������� ��������� ���������� ����
    {
      if( !HIWORD( wParam ) )          // ��������� ��������� �����������
      {
        active = true;          // ��������� �������
      }
      else
      {
        active = false;          // ��������� ������ �� �������
      }
 
      return 0;            // ������������ � ���� ��������� ���������
    }
	    case WM_SYSCOMMAND:            // ������������� ��������� �������
    {
      switch ( wParam )            // ������������� ��������� �����
      {
        case SC_SCREENSAVE:        // �������� �� ���������� �����������?
        case SC_MONITORPOWER:        // �������� �� ������� ������� � ����� ���������� �������?
        return 0;          // ������������� ���
      }
      break;              // �����
    }
	    case WM_CLOSE:              // �� �������� ��������� � ��������?
    {
      PostQuitMessage( 0 );          // ��������� ��������� � ������
      return 0;            // ��������� �����
    }
	    case WM_KEYDOWN:            // ���� �� ������ ������?
    {
      keys[wParam] = true;          // ���� ���, �� ����������� ���� ������ true
      return 0;            // ������������
    }
	    case WM_KEYUP:              // ���� �� �������� �������?
    {
      keys[wParam] = false;          //  ���� ���, �� ����������� ���� ������ false
      return 0;            // ������������
    }
	    case WM_SIZE:              // �������� ������� OpenGL ����
    {
      ReSizeGLScene( LOWORD(lParam), HIWORD(lParam) );  // ������� �����=Width, ������� �����=Height
      return 0;            // ������������
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