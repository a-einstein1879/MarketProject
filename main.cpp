#include "market.h"
#include <stdlib.h>
#include <ctime>

#include "interface.h"

int main() {
	OpenGLInterface *ui;
	ui = ui->getOpenGLInterface();
	ui->interfaceFunction();

	/*srand(time(0));
	Market *market;
	market = market->getMarket();

	while(market->tick()) {}*/

	system("pause");
	return 0;
};