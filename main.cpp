#include "market.h"
#include <stdlib.h>

int main() {
	Market *market;
	market = market->getMarket();

	for(int i = 0; i < 8; i++) {
		market->tick();
	}

	system("pause");
	return 0;
};