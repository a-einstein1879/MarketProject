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
}

OpenGLInterface* OpenGLInterface::getOpenGLInterface() {
	if(!p_OpenGLInterface) {
		p_OpenGLInterface = new OpenGLInterface();
	}
	return p_OpenGLInterface;
}

int OpenGLInterface::interfaceFunction() {
	MSG  msg;              // Структура для хранения сообщения Windows
	BOOL  done = false;            // Логическая переменная для выхода из цикла

	if( !CreateGLWindow( L"NeHe OpenGL окно", 1024, 768, 32, fullscreen ) ) {
		return 0;
	}
	
	while( !done ) {
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) {
			if( msg.message == WM_QUIT ) {
				done = true;
			} else {
				TranslateMessage( &msg );        // Переводим сообщение
				DispatchMessage( &msg );        // Отсылаем сообщение
			}
		} else {
			if( active ) {
/*				if(keys[VK_ESCAPE]) {
					done = true;      // ESC говорит об останове выполнения программы
				} else {*/
					DrawGLScene();
					SwapBuffers( hDC );
//				}
			}
/*			if( keys[VK_F1] ) {
				keys[VK_F1] = false;
				KillGLWindow();
				fullscreen = !fullscreen;
				// Пересоздаём наше OpenGL окно
				if( !CreateGLWindow( _T("NeHe OpenGL структура"), 1024, 768, 32, fullscreen ) ) {
					return 0;        // Выходим, если это невозможно
				}
			}*/
		}
	}
	// Shutdown
	KillGLWindow();                // Разрушаем окно
	return ( msg.wParam );              // Выходим из программы
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
	WindowRect.left=(long)0;              // Установить левую составляющую в 0
	WindowRect.right=(long)width;              // Установить правую составляющую в Width
	WindowRect.top=(long)0;                // Установить верхнюю составляющую в 0
	WindowRect.bottom=(long)height;              // Установить нижнюю составляющую в Height

	fullscreen = fullscreenflag;              // Устанавливаем значение глобальной переменной fullscreen
	hInstance    = GetModuleHandle(NULL);        // Считаем дескриптор нашего приложения
	wc.style    = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;      // Перерисуем при перемещении и создаём скрытый DC
	wc.lpfnWndProc    = &OpenGLInterface::StaticWndProc;          // Процедура обработки сообщений
	wc.cbClsExtra    = 0;              // Нет дополнительной информации для окна
	wc.cbWndExtra    = 0;              // Нет дополнительной информации для окна
	wc.hInstance    = hInstance;            // Устанавливаем дескриптор
	wc.hIcon    = LoadIcon(NULL, IDI_WINLOGO);        // Загружаем иконку по умолчанию
	wc.hCursor    = LoadCursor(NULL, IDC_ARROW);        // Загружаем указатель мышки
	wc.hbrBackground  = NULL;              // Фон не требуется для GL
	wc.lpszMenuName    = NULL;              // Меню в окне не будет
	wc.lpszClassName  = L"OpenGL";            // Устанавливаем имя классу

	if( !RegisterClass( &wc ) ) {
    MessageBox( NULL, L"Failed To Register The Window Class.", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
    return false;                // Выход и возвращение функцией значения false
	}
	if( fullscreen ) {
		DEVMODE dmScreenSettings;            // Режим устройства
		memset( &dmScreenSettings, 0, sizeof( dmScreenSettings ) );    // Очистка для хранения установок
		dmScreenSettings.dmSize=sizeof( dmScreenSettings );      // Размер структуры Devmode
		dmScreenSettings.dmPelsWidth  =   width;        // Ширина экрана
		dmScreenSettings.dmPelsHeight  =   height;        // Высота экрана
		dmScreenSettings.dmBitsPerPel  =   bits;        // Глубина цвета
		dmScreenSettings.dmFields= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;// Режим Пикселя
		// Пытаемся установить выбранный режим и получить результат.  Примечание: CDS_FULLSCREEN убирает панель управления.
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
		dwExStyle  =   WS_EX_APPWINDOW;          // Расширенный стиль окна
		dwStyle    =   WS_POPUP;            // Обычный стиль окна
		ShowCursor( true );
	} else {
		dwExStyle  =   WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;      // Расширенный стиль окна
		dwStyle    =   WS_OVERLAPPEDWINDOW;        // Обычный стиль окна
	}
	AdjustWindowRectEx( &WindowRect, dwStyle, false, dwExStyle );

	if( !( hWnd = CreateWindowEx(  dwExStyle,          // Расширенный стиль для окна
          _T("OpenGL"),          // Имя класса
          title,            // Заголовок окна
          WS_CLIPSIBLINGS |        // Требуемый стиль для окна
          WS_CLIPCHILDREN |        // Требуемый стиль для окна
          dwStyle,          // Выбираемые стили для окна
          0, 0,            // Позиция окна
          WindowRect.right-WindowRect.left,    // Вычисление подходящей ширины
          WindowRect.bottom-WindowRect.top,    // Вычисление подходящей высоты
          NULL,            // Нет родительского
          NULL,            // Нет меню
          hInstance,          // Дескриптор приложения
          NULL ) ) ) {          // Не передаём ничего до WM_CREATE (???)
			  KillGLWindow();                // Восстановить экран
			  MessageBox( NULL, L"Window Creation Error.", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
			  return false;                // Вернуть false
	}

	static  PIXELFORMATDESCRIPTOR pfd=            // pfd сообщает Windows каким будет вывод на экран каждого пикселя
  {
    sizeof(PIXELFORMATDESCRIPTOR),            // Размер дескриптора данного формата пикселей
    1,                  // Номер версии
    PFD_DRAW_TO_WINDOW |              // Формат для Окна
    PFD_SUPPORT_OPENGL |              // Формат для OpenGL
    PFD_DOUBLEBUFFER,              // Формат для двойного буфера
    PFD_TYPE_RGBA,                // Требуется RGBA формат
    bits,                  // Выбирается бит глубины цвета
    0, 0, 0, 0, 0, 0,              // Игнорирование цветовых битов
    0,                  // Нет буфера прозрачности
    0,                  // Сдвиговый бит игнорируется
    0,                  // Нет буфера накопления
    0, 0, 0, 0,                // Биты накопления игнорируются
    32,                  // 32 битный Z-буфер (буфер глубины)
    0,                  // Нет буфера трафарета
    0,                  // Нет вспомогательных буферов
    PFD_MAIN_PLANE,                // Главный слой рисования
    0,                  // Зарезервировано
    0, 0, 0                  // Маски слоя игнорируются
  };

	if( !( hDC = GetDC( hWnd ) ) ) {             // Можем ли мы получить Контекст Устройства?
		KillGLWindow();                // Восстановить экран
		MessageBox( NULL, L"Can't Create A GL Device Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
		return false;                // Вернуть false
	}
	
	if( !( PixelFormat = ChoosePixelFormat( hDC, &pfd ) ) )        // Найден ли подходящий формат пикселя?
  {
    KillGLWindow();                // Восстановить экран
    MessageBox( NULL, L"Can't Find A Suitable PixelFormat.", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
    return false;                // Вернуть false
  }
	
	if( !SetPixelFormat( hDC, PixelFormat, &pfd ) )          // Возможно ли установить Формат Пикселя?
  {
    KillGLWindow();                // Восстановить экран
    MessageBox( NULL, L"Can't Set The PixelFormat.", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
    return false;                // Вернуть false
  }
	
	if( !( hRC = wglCreateContext( hDC ) ) )          // Возможно ли установить Контекст Рендеринга?
  {
    KillGLWindow();                // Восстановить экран
    MessageBox( NULL, L"Can't Create A GL Rendering Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
    return false;                // Вернуть false
  }

	if( !wglMakeCurrent( hDC, hRC ) )            // Попробовать активировать Контекст Рендеринга
  {
    KillGLWindow();                // Восстановить экран
    MessageBox( NULL, L"Can't Activate The GL Rendering Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
    return false;                // Вернуть false
  }

	ShowWindow( hWnd, SW_SHOW );              // Показать окно
	SetForegroundWindow( hWnd );              // Слегка повысим приоритет
	SetFocus( hWnd );                // Установить фокус клавиатуры на наше окно
	ReSizeGLScene( width, height );              // Настроим перспективу для нашего OpenGL экрана.

	if( !InitGL() )                  // Инициализация только что созданного окна
  {
    KillGLWindow();                // Восстановить экран
    MessageBox( NULL, _T("Initialization Failed."), _T("ERROR"), MB_OK | MB_ICONEXCLAMATION );
    return false;                // Вернуть false
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

int OpenGLInterface::DrawGLScene( GLvoid ) {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(-1.0f, 1.0f, 0.0f);  // Слева вверху
	glVertex3f( 1.0f, 1.0f, 0.0f);  // Справа вверху
	glVertex3f( 1.0f,-1.0f, 0.0f);  // Справа внизу
	glVertex3f(-1.0f,-1.0f, 0.0f);  // Слева внизу
	glEnd();
	return true;
}

GLvoid OpenGLInterface::ReSizeGLScene(GLsizei width, GLsizei height) {
	if( height == 0 ) {
		height = 1;
	}
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective( 45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}

LRESULT OpenGLInterface::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)                // Проверка сообщения для окна
  {
	      case WM_ACTIVATE:            // Проверка сообщения активности окна
    {
      if( !HIWORD( wParam ) )          // Проверить состояние минимизации
      {
        active = true;          // Программа активна
      }
      else
      {
        active = false;          // Программа теперь не активна
      }
 
      return 0;            // Возвращаемся в цикл обработки сообщений
    }
	    case WM_SYSCOMMAND:            // Перехватываем системную команду
    {
      switch ( wParam )            // Останавливаем системный вызов
      {
        case SC_SCREENSAVE:        // Пытается ли запустится скринсейвер?
        case SC_MONITORPOWER:        // Пытается ли монитор перейти в режим сбережения энергии?
        return 0;          // Предотвращаем это
      }
      break;              // Выход
    }
	    case WM_CLOSE:              // Мы получили сообщение о закрытие?
    {
      PostQuitMessage( 0 );          // Отправить сообщение о выходе
      return 0;            // Вернуться назад
    }
	    case WM_KEYDOWN:            // Была ли нажата кнопка?
    {
      keys[wParam] = true;          // Если так, мы присваиваем этой ячейке true
      return 0;            // Возвращаемся
    }
	    case WM_KEYUP:              // Была ли отпущена клавиша?
    {
      keys[wParam] = false;          //  Если так, мы присваиваем этой ячейке false
      return 0;            // Возвращаемся
    }
	    case WM_SIZE:              // Изменены размеры OpenGL окна
    {
      ReSizeGLScene( LOWORD(lParam), HIWORD(lParam) );  // Младшее слово=Width, старшее слово=Height
      return 0;            // Возвращаемся
    }
  }
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

LRESULT CALLBACK OpenGLInterface::StaticWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
      if (LONG_PTR user_data = GetWindowLongPtr(hWnd, GWLP_USERDATA)) {
        OpenGLInterface *this_window = reinterpret_cast<OpenGLInterface *>(user_data);
        return this_window->WndProc(hWnd, Msg, wParam, lParam);
      }
      if (Msg == WM_NCCREATE) {
        LPCREATESTRUCT create_struct = reinterpret_cast<LPCREATESTRUCT>(lParam);
        void *lpCreateParam = create_struct->lpCreateParams;
        OpenGLInterface *this_window = reinterpret_cast<OpenGLInterface*>(lpCreateParam);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this_window));
        return this_window->WndProc(hWnd, Msg, wParam, lParam);
      }
      return DefWindowProc(hWnd, Msg, wParam, lParam);
}