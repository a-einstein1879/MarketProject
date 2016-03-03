#include "market.h"
#include <stdlib.h>
#include <ctime>

int main() {
	srand(time(0));
	Market *market;
	market = market->getMarket();

	for(int i = 0; i < 20; i++) {
		market->tick();
	}

	system("pause");
	return 0;
};