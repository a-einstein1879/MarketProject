#include <ctime>

#include "interface.h"
#include "market.h"

#include "histogram.h"
#include <stdio.h>

int main() {
	Histogram histogram(3, 10, 0, 10);
/*	Color color(1, 0, 0);
	histogram.setColor(color, 0);
	color.set(0, 1, 0);
	histogram.setColor(color, 1);
	color.set(0, 0, 1);
	histogram.setColor(color, 2);*/

	histogram.setValue(0, 0, 3);
	histogram.setValue(0, 1, 5);
	histogram.setValue(0, 2, 2);
	histogram.setValue(0, 3, 0);
	histogram.setValue(0, 4, 0);
	histogram.setValue(0, 5, 0);
	histogram.setValue(0, 6, 0);
	histogram.setValue(0, 7, 0);
	histogram.setValue(0, 8, 0);
	
	histogram.setValue(1, 0, 0);
	histogram.setValue(1, 1, 0);
	histogram.setValue(1, 2, 0);
	histogram.setValue(1, 3, 10);
	histogram.setValue(1, 4, 12);
	histogram.setValue(1, 5, 9);
	histogram.setValue(1, 6, 0);
	histogram.setValue(1, 7, 0);
	histogram.setValue(1, 8, 0);
	
	histogram.setValue(2, 0, 0);
	histogram.setValue(2, 1, 0);
	histogram.setValue(2, 2, 0);
	histogram.setValue(2, 3, 0);
	histogram.setValue(2, 4, 0);
	histogram.setValue(2, 5, 0);
	histogram.setValue(2, 6, 13);
	histogram.setValue(2, 7, 17);
	histogram.setValue(2, 8, 6);

	srand(time(0));
	Market *market;
	OpenGLInterface *ui;
	market = market->getMarket();
	ui = ui->getOpenGLInterface();
	
	MSG  msg;
	BOOL  done = false;
	while(!done /*&& market->tick()*/) {
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) {
			if( msg.message == WM_QUIT ) {
				done = true;
				ui->KillGLWindow();
			} else {
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		} else {
			ui->printHistogram(histogram);
	system("pause");
		}
	}

	system("pause");
	return 0;
};