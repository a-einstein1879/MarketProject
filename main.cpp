#include <ctime>

#include "outputGraphics.h"
#include "market.h"

#include <stdio.h>

int main() {
	srand(time(0));
	Market *market;
	OpenGLInterface *ui;
	market = market->getMarket();
	ui = ui->getOpenGLInterface();
	
	MSG  msg;
	BOOL  done = false;
	while(!done && market->tick()) {
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) {
			if( msg.message == WM_QUIT ) {
				done = true;
				ui->KillGLWindow();
			} else {
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}
	}

	system("pause");
	return 0;
};